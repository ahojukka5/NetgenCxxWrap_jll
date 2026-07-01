# Wrapping plan — Netgen + OpenCASCADE, strict 1:1

Goal: a **boring, comprehensive 1:1 wrapper**. Each binding exposes a real Netgen
or OpenCASCADE class/function under its own name (`gp_Pnt`, `gp_Ax2`,
`TopoDS_Shape`, `BRepPrimAPI_MakeCylinder`, `Mesh`, `Refinement`, …). **No
combiner / convenience functions** (no `OCC_Cylinder`). All higher-level logic
lives in consuming packages.

## Scope of "all"

OpenCASCADE ships **7233** headers. The overwhelming majority are internal:
STEP/IGES exchange schema (`StepBasic_*`, `IGESGeom_*`, ~2500 headers),
visualization (`AIS_*`, `Prs3d_*`, `Graphic3d_*`), HLR, meshing internals
(`BRepMesh_*`), data-exchange plumbing, etc. Wrapping those is neither feasible
nor useful. **"All" here = the geometry modeling kernel** — the classes a caller
actually uses to build, query, transform, import/export and combine shapes, plus
Netgen's public mesh/geometry/refinement API. That is the list below. If a
specific extra class is needed later, it is added the same mechanical way.

Legend: ✅ wrapped · ◻ planned (same pattern, not yet bound).

---

## OpenCASCADE — `gp` (value geometry types)

3D: ✅ `gp_XYZ` ✅ `gp_Pnt` ✅ `gp_Vec` ✅ `gp_Dir` ✅ `gp_Ax1` ✅ `gp_Ax2`
✅ `gp_Ax3` ✅ `gp_Trsf`
2D: ✅ `gp_XY` ✅ `gp_Pnt2d` ✅ `gp_Vec2d` ✅ `gp_Dir2d` ✅ `gp_Ax2d` ✅ `gp_Ax22d`
✅ `gp_Trsf2d`
Analytic curves/surfaces: ✅ `gp_Lin` ✅ `gp_Circ` ✅ `gp_Elips` ✅ `gp_Pln`
✅ `gp_Cylinder` ✅ `gp_Cone` ✅ `gp_Sphere` ✅ `gp_Torus`
◻ `gp_Lin2d` `gp_Circ2d` `gp_Parab` `gp_Hypr` `gp_Mat` `gp_Mat2d` `gp_GTrsf`
`gp_Quaternion`

Each: the documented constructors + coordinate/axis accessors (`X/Y/Z`, `Coord`,
`Location`, `Direction`, `Distance`, `SetX…`), and for `gp_Trsf` the
`SetTranslation`/`SetRotation`/`SetScale`/`Value` setters.

## OpenCASCADE — topology (`TopoDS`, `TopAbs`, `TopExp`, `TopTools`)

✅ `TopoDS_Shape` (`IsNull`, `ShapeType`, `IsEqual`, `IsSame`, `Orientation`,
`Reversed`, `Nullify`)
✅ `TopoDS_Vertex` `TopoDS_Edge` `TopoDS_Wire` `TopoDS_Face` `TopoDS_Shell`
`TopoDS_Solid` `TopoDS_Compound` `TopoDS_CompSolid` (registered with
`TopoDS_Shape` as base)
✅ `TopAbs_ShapeEnum` constants (COMPOUND…VERTEX), `TopAbs_Orientation`
✅ `TopoDS::Vertex/Edge/Wire/Face/Shell/Solid/Compound` (downcasts)
✅ `TopExp_Explorer` (`Init`, `More`, `Next`, `Current`, `Value`) — sub-shape
traversal
✅ `TopoDS_Iterator`
◻ `TopTools_IndexedMapOfShape` / `TopExp::MapShapes` (bulk sub-shape collection)

## OpenCASCADE — modeling algorithms

Primitives (`BRepPrimAPI`): ✅ `MakeBox` ✅ `MakeCylinder` ✅ `MakeSphere`
✅ `MakeCone` ✅ `MakeTorus` ✅ `MakePrism` ✅ `MakeRevol` ◻ `MakeWedge`
`MakeHalfSpace`
Builders (`BRepBuilderAPI`): ✅ `MakeVertex` ✅ `MakeEdge` ✅ `MakeWire`
✅ `MakeFace` ✅ `MakeSolid` ✅ `MakePolygon` ✅ `MakeShell` ✅ `Transform`
✅ `Copy` ◻ `GTransform` `Sewing` `NurbsConvert` `MakeEdge2d`
Booleans (`BRepAlgoAPI`): ✅ `Fuse` ✅ `Cut` ✅ `Common` ✅ `Section`
◻ `Splitter` `Defeaturing`
Fillets/offsets: ◻ `BRepFilletAPI_MakeFillet` `MakeChamfer`
`BRepOffsetAPI_MakeThickSolid` `MakePipe` `ThruSections` `MakeOffset`
Each `Make*`/boolean: documented constructors + `Build`/`Shape`/`IsDone` and the
typed result accessor (`Solid()`, `Edge()`, `Wire()`, `Face()`).

## OpenCASCADE — curves/surfaces & import/export

◻ `Geom_*` / `Geom2d_*` (Handle-based; `GC_MakeSegment`, `GC_MakeArcOfCircle`,
`GCE2d_*`) — needs `Handle()` (`opencascade::handle`) support; second installment.
✅ `BRep_Builder`
✅ `BRepTools::Write` / `BRepTools::Read` (BREP I/O)
✅ `STEPControl_Reader` / `STEPControl_Writer`
✅ `IGESControl_Reader` / `IGESControl_Writer`
◻ `BRepGProp` / `GProp_GProps` (mass/area/volume properties)
◻ `BRepBndLib` / `Bnd_Box` (bounding boxes)

---

## Netgen — public C++ API (already wrapped, see netgen.cpp)

✅ `Mesh` (handle `shared_ptr<Mesh>`): counts, `Point`, `VolumeElement`,
`SurfaceElement`, `UpdateTopology`, `GetTopology`, `Get/SetGeometry`, `Save`,
`Load`, `Compress`, `CalcLocalH`, `Get/SetTimeStamp`, `BuildCurvedElements`,
`GetMaterial`/`GetBCName` (+ setters), allocator `new_mesh`, copy `assign`
(`Mesh::operator=`).
✅ `MeshPoint`, `Element`, `Element2d` (incl. refinement flags), `MeshTopology`
(`GetNEdges`, `GetNFaces`), `Point3d`, `Vec3d`.
✅ `MeshingParameters`, `BisectionOptions` (public fields).
✅ `NetgenGeometry`, `Refinement` (`Refine`, `Bisect`, `MakeSecondOrder`),
`GetRefinement`, `GenerateMesh`.
✅ `Ngx_Mesh` (multigrid hierarchy: `GetNLevels`, `GetNVLevel`, `GetParentNodes`,
`GetParentElement`, `GetParentSElement`, `Curve`, …).
✅ OCC import `LoadOCC_STEP/IGES/BREP`; ✅ `OCCGeometry(TopoDS_Shape)` ctor.
✅ 2D `geom2d`/`csg2d`: `Circle`, `Rectangle`, `Solid2d` (`+`/`*`/`-`,
`BC`/`Maxh`/`Mat`), `CSG2d` (`Add`, `GenerateSplineGeometry`, `GenerateMesh`).
◻ `MeshTopology` connectivity (`GetEdgeVertices`, `GetFaceVertices`, element→edge/
face), segments (`LineSegment`), `Mesh::ImproveMesh`, `ZRefinement`.

## Conventions

- gp/TopoDS/algorithm classes are CxxWrap **value types** (copyable). Algorithm
  `Make*` classes are constructed, then `Shape()`/typed accessor pulls the result.
- `TopoDS_*` subtypes register with `jlcxx::julia_base_type<TopoDS_Shape>()`.
- OCC is its own CxxWrap module (`define_julia_module_occ` in occ.cpp), so the
  raw names are defined in the Julia `Netgen.OCC` submodule (`OCC.gp_Pnt`,
  `OCC.BRepPrimAPI_MakeCylinder`, …) — not in `Netgen`. No renaming, no combiners.
