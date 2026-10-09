# NetgenCxxWrap — CxxWrap design

## What this is

`libnetgen_cxxwrap` is a CxxWrap binding of the exported C++ API of
NGSolve/Netgen. Delone.jl loads that module and adds the Julia-side
utilities for geometry-backed mesh hierarchies.

The native binding is `libnetgen_cxxwrap` (built by `NetgenCxxWrap_jll`), a
[CxxWrap](https://github.com/JuliaInterop/CxxWrap.jl) module linked against the
prebuilt NGSolve/Netgen libraries. Delone.jl loads it via
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
Delone.jl          loads libnetgen_cxxwrap via CxxWrap.@wrapmodule and adds
                    the Julian conveniences and hierarchy helpers.
consumer           uses Delone.jl as the geometry-backed mesh-hierarchy backend.
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
→ Delone.jl mesh extraction and hierarchy utilities
→ consumer GMG integration
```

A secondary CSG route (a Julia geometry DSL → serialize to `.geo`/CSG text →
Netgen's own parser/loader) can be added later: Netgen's own parser may use the
hidden constructors internally, which is fine. External wrapper code must not.

## What is wrapped

A wrapped name matches a Netgen member when CxxWrap can spell it.
Allocators are `new_mesh`, `new_localh`, and `new_point3dtree`. `assign`
is `Mesh::operator=`. A template takes a dimension suffix
(`SetRefinementFlag2` / `3`, `ElementTransformation33` / `23` / `22` /
`13` / `12`, `MultiElementTransformation33` / `22`,
`FindElementOfPoint1` / `2` / `3`). An overload takes a qualifier:
`GetHPointIndex` is `Mesh::GetH(PointIndex)`, `GetFaceDescriptorMut` is
the mutable `GetFaceDescriptor`, and `GetRegionNameVolume` / `Surface` /
`Segment` are `GetRegionName`. `NgxRefine` is `Ngx_Mesh::Refine`.
`EnableTopologyTable` is `MeshTopology::EnableTable`. `GetMaterialCD0`–`3`
forward to `Mesh::GetMaterial`, `GetBCName`, `GetCD2Name`, and
`GetCD3Name`. The OCC bridge does not expose a Julia `TopoDS` type, and
`LoadSplineGeometry2d` is `SplineGeometry2d::Load`. OpenCASCADE modeling
lives in OpenCascadeCxxWrap. Higher-level logic belongs in Delone.jl.
Currently bound:

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
  with `-1` = none; Delone.jl normalizes to 1-based / `0` = none.
- material / boundary labels: `GetMaterial`/`SetMaterial`, `GetBCName`/`SetBCName`
  (1-based region numbers, as carried by `Element*::GetIndex`);
- OCC loaders `LoadOCC_STEP`, `LoadOCC_IGES`, `LoadOCC_BREP` (each separately —
  no combined loader);
- OCC bridge in `netgen_occ_bridge.cpp`: `OCCGeometry_from_brep_string`,
  `OCC_NrFaces`, `OCC_FaceBoundingBox`, `OCC_IdentifyFacesBulk`,
  `OCC_RebuildGeometry`. There is no `OCC_Box`, `OCC_Sphere`, or
  `OCC_Cylinder`;
- `netgen_ngx2.cpp`: hp orders, `NgxRefine`, `HPRefinement`, `SplitAlfeld`,
  cluster representatives, and `MeshVolume` / `OptimizeVolume`;
- `netgen_ngx3.cpp`: element transformations, parent edges and faces,
  and periodic vertex pairs;
- `netgen_stl.cpp`: `STLGeometry`, `STLParameters`, `LoadSTL`;
- `netgen_gprim.cpp`: `Box3d`, `Point3dTree`, `LoadSplineGeometry2d`;
- `netgen_mesh2.cpp`: `EdgeDescriptor` and the remaining `Mesh` methods
  (`GetBox`, local mesh size, splits, open elements, region names);
- **2D geometry** (`geom2d/csg2d`): `Circle`, `Rectangle` → `Solid2d`; boolean
  ops `+`/`*`/`-` (union/intersection/difference, bound on Julia `Base` via
  `set_override_module`); inline attribute setters `BC`/`Maxh`/`Mat` (the
  non-exported `Move`/`Scale`/`Rotate` are omitted); `CSG2d` container with
  `Add`, `GenerateSplineGeometry` (→ a `SplineGeometry2d`, used as a
  `NetgenGeometry`) and `GenerateMesh`. 2D refinement projects boundary nodes
  onto the splines (curved boundaries are followed).

Julian conveniences live in Delone.jl. This JLL does not add them.

## How this supports the GMG roadmap

Geometry-aware refinement (new boundary points project onto the true OCC
surface) plus the refinement-hierarchy parent maps (`mlbetweennodes` →
`point_parents`) are the raw ingredients for prolongation/restriction operators.
Delone.jl is where mesh generation and refinement are composed into
hierarchies. A consumer takes extracted points, connectivity, topology,
and tags into its own mesh carrier.

## Status

This note describes the source tree. It is not a record of a passing
local run. Commit `2985283` states that rebuilding through
`gen/build_local.jl` segfaults on basic STEP loading even from this
repo's unmodified prior source, and that the failure looks like a local
toolchain mismatch rather than that change. The binary Delone.jl
consumes is the `libnetgen_cxxwrap` artifact named in the README;
commits after `301822b`, including `2985283`, are not in that artifact.
