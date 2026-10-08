# NetgenCxxWrap_jll

BinaryBuilder recipe for **`libnetgen_cxxwrap`** — a [CxxWrap](https://github.com/JuliaInterop/CxxWrap.jl)
module that binds the exported C++ API of NGSolve/Netgen for use from Julia.

It is intentionally **boring and comprehensive**: a wrapper that exposes Netgen
C++ functionality (mesh, geometry, topology, refinement) to Julia, with no logic
of its own. Higher-level utilities live in **Delone.jl**.

## OpenCASCADE split (done)

OCCT modeling bindings moved to **[`ahojukka5/OpenCascadeCxxWrap`](https://github.com/ahojukka5/OpenCascadeCxxWrap)**.
This JLL keeps Netgen meshing plus **`OCCGeometry_from_brep_string`** only.

## Design

- Builds `libnetgen_cxxwrap` (a `JLCXX_MODULE`). `bundled/CMakeLists.txt`
  compiles eleven translation units; `bundled/src/netgen.cpp` is only the
  registrar (`define_julia_module` plus `register_*` calls).
- **Strict 1:1 wrapping**: every wrapped name matches Netgen's own C++ name
  (`Mesh::GetNP` → `GetNP`, `UpdateTopology`, `GetTopology`, `GetNEdges`,
  `LoadOCC_STEP`, `GenerateMesh`, `Refine`, `Point`, `VolumeElement`, `PNum`, …)
  and forwards to exactly one Netgen member — no invented or combiner functions.
  The one unavoidable exception is `new_mesh`, the `std::shared_ptr<Mesh>`
  allocator (CxxWrap cannot expose the `Mesh` constructor under the type name,
  and `Mesh` is not value-copyable).
- **Depends on** `NGSolveNetgen_jll` (links prebuilt `libnglib`/`libngcore` +
  headers), `OCCT_jll` (OpenCASCADE), and `libcxxwrap_julia_jll` (JlCxx).
- Uses only **exported** Netgen symbols. The hidden CSG primitive constructors
  are not wrapped; OCC/BREP/STEP/IGES is the primary geometry route.
- Does not rebuild or patch upstream Netgen.

See [`docs/CXXWRAP_DESIGN.md`](docs/CXXWRAP_DESIGN.md).

## Status

`build_tarballs.jl` is authored but not yet built/registered: a BinaryBuilder
`Dependency` resolves from the registry, so it can only build once
`NGSolveNetgen_jll` is registered (there is no
`JuliaBinaryWrappers/NGSolveNetgen_jll.jl`).

The live binary is the **`libnetgen_cxxwrap`** artifact on
[`oodi-artifacts`](https://github.com/ahojukka5/oodi-artifacts), consumed by
Delone.jl. Latest published tree is
[`libnetgen_cxxwrap-d2a7f166`](https://github.com/ahojukka5/oodi-artifacts/releases/tag/libnetgen_cxxwrap-d2a7f166)
(2026-07-29), built from this repo at `301822b`. Later `master` commits —
including the identification-copy fix in `2985283` — are not in that artifact.

## Layout

```
LICENSE                    # MIT (wrapper)
build_tarballs.jl          # recipe: deps NGSolveNetgen_jll + OCCT_jll + libcxxwrap_julia_jll
bundled/
  LICENSE                  # MIT (wrapper); links LGPL Netgen/OCC at run time
  CMakeLists.txt           # builds libnetgen_cxxwrap from the eleven TUs below
  src/
    netgen.cpp             # registrar: define_julia_module → register_*
    netgen_mesh.cpp        # Point3d, Vec3d, MeshPoint, Element, Element2d, MeshTopology
    netgen_geometry.cpp    # MeshingParameters, BisectionOptions, NetgenGeometry,
                           # Refinement, Mesh, Ngx_Mesh, LoadOCC_* free fns
    netgen_geom2d.cpp      # Solid2d, CSG2d, Circle, Rectangle
    netgen_extras.cpp      # Segment, FaceDescriptor, LocalH, extra Mesh/MeshTopology
    netgen_stl.cpp         # STLGeometry, STLParameters
    netgen_gprim.cpp       # Box3d, Point3dTree, SplineGeometry2d
    netgen_mesh2.cpp       # EdgeDescriptor, GetBox, remaining Mesh methods
    netgen_ngx2.cpp        # Ngx_Mesh hp/order/refine; MeshVolume/OptimizeVolume
    netgen_ngx3.cpp        # Ngx_Mesh transforms, parent edge/face, periodic, partition
    netgen_occ_bridge.cpp  # BREP string → OCCGeometry (internal; no Julia TopoDS)
docs/CXXWRAP_DESIGN.md
```
