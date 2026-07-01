# NetgenCxxWrap / Netgen.jl — CxxWrap design

## What this is

`Netgen` is a **CxxWrap-based Julia binding and extension layer for the exported
C++ API of NGSolve/Netgen**, with additional Julia-side utilities for
geometry-backed mesh hierarchies and Oodi.jl GMG integration.

The native binding is `libnetgen_cxxwrap` (built by `NetgenCxxWrap_jll`), a
[CxxWrap](https://github.com/JuliaInterop/CxxWrap.jl) module linked against the
prebuilt NGSolve/Netgen libraries. The Julia package `Netgen.jl` loads it via
`@wrapmodule`/`@initcxx` and layers Julian conveniences on top.

## Why CxxWrap (and not a hand-written C ABI)

An earlier iteration hand-wrote a plain C ABI (`extern "C"` wrappers) over
Netgen. That was abandoned: it meant flattening Netgen's rich C++ API into
hundreds of bespoke C functions and marshalling helpers by hand — slow to write,
broad surface to maintain, and inventing a parallel API. CxxWrap instead binds
the **exported C++ classes and methods directly** (`Mesh`, `MeshTopology`,
`MeshingParameters`, `OCCGeometry`, `Refinement`, …), handles the
Julia↔C++ conversions, and lets the binding stay close to Netgen's own names.
The package is meant to be boring, broad, and maintainable — a wrapper, not a
reimplementation.

## Package stack

```
NGSolveNetgen_jll   upstream NGSolve/Netgen binary (+ OCC). Stays close to upstream;
                    never patched just to expose hidden symbols.
NetgenCxxWrap_jll   builds libnetgen_cxxwrap: a CxxWrap module linked against
                    NGSolveNetgen_jll + OCCT_jll + libcxxwrap_julia_jll. Boring,
                    comprehensive wrapper; no logic of its own.
Netgen.jl          loads libnetgen_cxxwrap via CxxWrap.@wrapmodule; adds Julian
                    conveniences (points, tetrahedra, generate_mesh, refine!,
                    uniform_hierarchy, …) and, later, Oodi GMG helpers.
Oodi.jl            uses Netgen.jl as the geometry-backed mesh-hierarchy backend.
```

## Relationship to NGSolveNetgen_jll

`NetgenCxxWrap_jll` **depends on** `NGSolveNetgen_jll` and links its prebuilt
`libnglib`/`libngcore` plus their installed headers. It does not rebuild Netgen
and does not vendor Netgen internals.

## Known hidden-symbol limitation

A separately-linked wrapper can only use **exported** Netgen symbols; CxxWrap
does not bypass dynamic-symbol visibility. The CSG primitive constructors
(`OrthoBrick`, `Sphere`, `Solid(Primitive*)`, `Solid::ball`) are hidden in the
stock build, so the wrapper does **not** construct CSG geometries directly. This
is why the binding does not expose CSG primitive construction.

## Why OCC/BREP/STEP is the primary geometry route

The OCC loaders (`LoadOCC_STEP/IGES/BREP`), `NetgenGeometry::GenerateMesh`,
`Refinement::Refine`, and the `Mesh`/`MeshTopology` accessors are all exported,
so the full OCC pipeline works against the stock binary with no patches and no
hidden symbols:

```
OCC / BREP / STEP / IGES geometry
→ Netgen OCC loader (LoadOCC_BREP/STEP/IGES)
→ Netgen mesh generation (NetgenGeometry::GenerateMesh)
→ Netgen refinement (Refinement::Refine, geometry-aware)
→ Netgen.jl mesh extraction + hierarchy utilities (points, tetrahedra, …)
→ Oodi.jl GMG integration
```

A secondary CSG route (Netgen.jl geometry DSL → serialize to `.geo`/CSG text →
Netgen's own parser/loader) can be added later: Netgen's own parser may use the
hidden constructors internally, which is fine. External wrapper code must not.

## What is wrapped — strict 1:1, no invented names

Every binding forwards to exactly one Netgen member and carries Netgen's own
name. There are no combiner functions and no renames; container-returning methods
that need bulk transfer are deferred to a later pointer + `unsafe_load` round
(higher-level logic belongs in `Netgen.jl`). Currently bound:

- value types `Point3d`, `Vec3d` (`X`/`Y`/`Z`, `Vec3d::Length`);
- `MeshPoint` (coordinates via the `operator()(i)` functor, 0-based, as in Netgen);
- `Element` / `Element2d` (`GetNP`, `GetNV`, `GetType`, `GetIndex`, `PNum`, and
  the refinement-flag setters/testers `SetRefinementFlag`, `TestRefinementFlag`,
  `SetStrongRefinementFlag`, `TestStrongRefinementFlag`);
- `MeshingParameters` (field accessors `maxh`/`maxh!`, `minh`/`minh!`,
  `grading`/`grading!`, `optsteps2d`/`!`, `optsteps3d`/`!`, `secondorder`/`!`);
- `BisectionOptions` (field accessors `maxlevel`/`!`, `usemarkedelements`/`!`,
  `refine_hp`/`!`, `refine_p`/`!`, `onlyonce`/`!`);
- `MeshTopology` (`GetNEdges`, `GetNFaces`);
- `NetgenGeometry::GenerateMesh`, `NetgenGeometry::GetRefinement`;
- `Refinement::Refine` (uniform), `Refinement::Bisect` (marked/adaptive),
  `Refinement::MakeSecondOrder`;
- `Mesh` (handle = `std::shared_ptr<Mesh>`): `GetNP`, `GetNV`, `GetNE`, `GetNSE`,
  `GetNSeg`, `GetDimension`, `GetNDomains`, `GetNFD`, `UpdateTopology`,
  `GetTopology`, `GetGeometry`, `SetGeometry`, `Save`, `Load`, `Point`,
  `VolumeElement`, `SurfaceElement` (mutable, so flags can be set), `Compress`,
  `CalcLocalH`, `GetTimeStamp`, `SetNextTimeStamp`, `BuildCurvedElements`, plus
  the `new_mesh` allocator and `assign` (the binding-layer spelling of
  `Mesh::operator=`, since Julia has no overloadable `=`; copies points/elements/
  geometry but not the refinement history, so the copy is ready to re-refine);
- `Ngx_Mesh` (Netgen's multigrid interface; constructed from a
  `std::shared_ptr<Mesh>`): `Valid`, `GetDimension`, `GetNLevels`, `GetNVLevel`,
  `GetNElements`, `GetNNodes`, `GetParentNodes`, `GetParentElement`,
  `GetParentSElement`, `Curve`, `GetCurveOrder`, `UpdateTopology` — the
  refinement-hierarchy (levels + parent maps) read side. Its indices are 0-based
  with `-1` = none; `Netgen.jl` normalizes to 1-based / `0` = none.
- material / boundary labels: `GetMaterial`/`SetMaterial`, `GetBCName`/`SetBCName`
  (1-based region numbers, as carried by `Element*::GetIndex`);
- OCC loaders `LoadOCC_STEP`, `LoadOCC_IGES`, `LoadOCC_BREP` (each separately —
  no combined loader);
- OCC **construction** from OpenCASCADE primitives: `OCC_Box`, `OCC_Sphere`,
  `OCC_Cylinder` (build a `TopoDS_Shape` via `BRepPrimAPI_*` and wrap it in an
  `OCCGeometry`, whose ctor runs `BuildFMap` so it is mesh-ready). OpenCASCADE
  modeling is OCCT's API, not Netgen's, but the wrapper already links OCCT;
- **2D geometry** (`geom2d/csg2d`): `Circle`, `Rectangle` → `Solid2d`; boolean
  ops `+`/`*`/`-` (union/intersection/difference, bound on Julia `Base` via
  `set_override_module`); inline attribute setters `BC`/`Maxh`/`Mat` (the
  non-exported `Move`/`Scale`/`Rotate` are omitted); `CSG2d` container with
  `Add`, `GenerateSplineGeometry` (→ a `SplineGeometry2d`, used as a
  `NetgenGeometry`) and `GenerateMesh`. 2D refinement projects boundary nodes
  onto the splines (curved boundaries are followed).

Julian conveniences in `Netgen.jl` (the only place higher-level logic lives):
`load_step/iges/brep`, `load_geometry` (extension dispatch), `generate_mesh(geom;
maxh)`, `points`, `tetrahedra`, `surface_triangles`, `refine!` (uniform),
`mark_for_refinement!` + `bisect!` (adaptive/marked), `make_second_order!`, the
GMG-hierarchy readers `num_levels`, `level_nvertices`, `parent_nodes`
(prolongation stencil), `parent_elements`, `parent_surface_elements`, and the
multi-level builder `copy_mesh` / `uniform_hierarchy` → `MeshHierarchy` (a stack
of nested distinct meshes with per-level `prolongation`, `coarsest`/`finest`).

## How this supports the Oodi GMG roadmap

Geometry-aware refinement (new boundary points project onto the true OCC
surface) plus the refinement-hierarchy parent maps (`mlbetweennodes` →
`point_parents`) are the raw ingredients for prolongation/restriction operators.
`Netgen.jl` composes mesh generation + refinement into hierarchies
(`uniform_hierarchy`, later an adaptive driver with synthetic indicators); Oodi
consumes extracted points/connectivity/topology/tags into its mesh carrier,
function spaces, matrix-free operators, and GMG transfers.

## Status (built & tested locally, macOS arm64)

Phases 1–8 are working and tested against the stock NGSolveNetgen artifact via a
local CxxWrap build (`Netgen.jl/gen/build_local.jl`): module load, value types,
mesh core + extraction, OCC load + mesh generation, uniform refinement + parent
maps, topology, and uniform hierarchies. The adaptive-hierarchy driver (Phase 9)
and Oodi integration (Phase 10) are future Julia-side work. Cross-platform
binaries come from `NetgenCxxWrap_jll/build_tarballs.jl` once `NGSolveNetgen_jll`
is registered (the recipe `Dependency`s resolve from the registry).
