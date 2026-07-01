# NetgenCxxWrap_jll

BinaryBuilder recipe for **`libnetgen_cxxwrap`** — a [CxxWrap](https://github.com/JuliaInterop/CxxWrap.jl)
module that binds the exported C++ API of NGSolve/Netgen for use from Julia.

It is intentionally **boring and comprehensive**: a wrapper that exposes Netgen
C++ functionality (mesh, geometry, topology, refinement) to Julia, with no logic
of its own. Higher-level utilities live in the `Netgen.jl` package.

## Design

- Builds `libnetgen_cxxwrap` (a `JLCXX_MODULE`) from a single
  `bundled/src/netgen.cpp`.
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

See [`docs/NEXTGEN_CXXWRAP_DESIGN.md`](docs/NEXTGEN_CXXWRAP_DESIGN.md).

## Status

`build_tarballs.jl` is authored but not yet built/registered: a BinaryBuilder
`Dependency` resolves from the registry, so it can only build once
`NGSolveNetgen_jll` is registered. For development, build locally with
`Netgen.jl/gen/build_local.jl` (links the extracted artifact + the CxxWrap
prefix; this platform only).

## Layout

```
build_tarballs.jl          # recipe: deps NGSolveNetgen_jll + OCCT_jll + libcxxwrap_julia_jll
bundled/
  LICENSE                  # MIT (wrapper); links LGPL Netgen/OCC at run time
  CMakeLists.txt           # builds libnetgen_cxxwrap (JlCxx + nglib + OCC)
  src/
    netgen.cpp             # the entire 1:1 CxxWrap module (define_julia_module)
docs/NEXTGEN_CXXWRAP_DESIGN.md
```
