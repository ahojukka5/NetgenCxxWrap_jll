#include <jlcxx/jlcxx.hpp>

#include <BRepOffsetAPI_MakePipe.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>
#include <BRepOffsetAPI_MakeOffsetShape.hxx>
#include <BRepOffsetAPI_MakeOffset.hxx>
#include <GeomAbs_JoinType.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>

void register_occ_sweep(jlcxx::Module& mod)
{
  // --- Type registrations (all before any mod.method calls) ---

  mod.add_type<BRepOffsetAPI_MakePipe>("BRepOffsetAPI_MakePipe")
    .constructor<const TopoDS_Wire&, const TopoDS_Shape&>();

  mod.add_type<BRepOffsetAPI_ThruSections>("BRepOffsetAPI_ThruSections")
    .constructor<>()
    .constructor<bool>()
    .constructor<bool, bool>();

  mod.add_type<BRepOffsetAPI_MakeOffsetShape>("BRepOffsetAPI_MakeOffsetShape")
    .constructor<>();

  mod.add_type<BRepOffsetAPI_MakeOffset>("BRepOffsetAPI_MakeOffset")
    .constructor<>();

  // --- BRepOffsetAPI_MakePipe methods ---

  mod.method("Shape", [](BRepOffsetAPI_MakePipe& pipe) -> TopoDS_Shape {
    return pipe.Shape();
  });

  mod.method("Build", [](BRepOffsetAPI_MakePipe& pipe) {
    pipe.Build();
  });

  mod.method("IsDone", [](BRepOffsetAPI_MakePipe& pipe) -> bool {
    return bool(pipe.IsDone());
  });

  mod.method("ErrorOnSurface", [](BRepOffsetAPI_MakePipe& pipe) -> double {
    return pipe.ErrorOnSurface();
  });

  // --- BRepOffsetAPI_ThruSections methods ---

  mod.method("AddWire", [](BRepOffsetAPI_ThruSections& t, const TopoDS_Wire& wire) {
    t.AddWire(wire);
  });

  mod.method("AddVertex", [](BRepOffsetAPI_ThruSections& t, const TopoDS_Vertex& v) {
    t.AddVertex(v);
  });

  mod.method("Build", [](BRepOffsetAPI_ThruSections& t) {
    t.Build();
  });

  mod.method("IsDone", [](BRepOffsetAPI_ThruSections& t) -> bool {
    return bool(t.IsDone());
  });

  mod.method("Shape", [](BRepOffsetAPI_ThruSections& t) -> TopoDS_Shape {
    return t.Shape();
  });

  mod.method("CheckCompatibility", [](BRepOffsetAPI_ThruSections& t, bool check) {
    t.CheckCompatibility(check);
  });

  // --- BRepOffsetAPI_MakeOffsetShape methods ---

  mod.method("PerformBySimple", [](BRepOffsetAPI_MakeOffsetShape& m,
                                    const TopoDS_Shape& shape,
                                    double offset) {
    m.PerformBySimple(shape, offset);
  });

  mod.method("IsDone", [](BRepOffsetAPI_MakeOffsetShape& m) -> bool {
    return bool(m.IsDone());
  });

  mod.method("Shape", [](BRepOffsetAPI_MakeOffsetShape& m) -> TopoDS_Shape {
    return m.Shape();
  });

  // --- BRepOffsetAPI_MakeOffset methods ---

  mod.method("Init", [](BRepOffsetAPI_MakeOffset& m, const TopoDS_Face& f, int jt, bool isOpen) {
    m.Init(f, GeomAbs_JoinType(jt), isOpen);
  });
  mod.method("AddWire", [](BRepOffsetAPI_MakeOffset& m, const TopoDS_Wire& w) {
    m.AddWire(w);
  });
  mod.method("Perform", [](BRepOffsetAPI_MakeOffset& m, double offset, double alt) {
    m.Perform(offset, alt);
  });
  mod.method("Build", [](BRepOffsetAPI_MakeOffset& m) {
    m.Build();
  });
  mod.method("IsDone", [](BRepOffsetAPI_MakeOffset& m) -> bool {
    return bool(m.IsDone());
  });
  mod.method("Shape", [](BRepOffsetAPI_MakeOffset& m) -> TopoDS_Shape {
    return m.Shape();
  });

  // --- GeomAbs_JoinType constants ---

  mod.method("GeomAbs_Arc",          []() { return int(GeomAbs_Arc); });
  mod.method("GeomAbs_Tangent",      []() { return int(GeomAbs_Tangent); });
  mod.method("GeomAbs_Intersection", []() { return int(GeomAbs_Intersection); });
}
