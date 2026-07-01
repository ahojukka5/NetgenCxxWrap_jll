#include <jlcxx/jlcxx.hpp>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Face.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepFilletAPI_MakeChamfer.hxx>

void register_occ_fillet(jlcxx::Module& mod) {
  mod.add_type<BRepFilletAPI_MakeFillet>("BRepFilletAPI_MakeFillet")
     .constructor<const TopoDS_Shape&>();

  mod.add_type<BRepFilletAPI_MakeChamfer>("BRepFilletAPI_MakeChamfer")
     .constructor<const TopoDS_Shape&>();

  mod.method("Add",        [](BRepFilletAPI_MakeFillet& m, double r, const TopoDS_Edge& e) { m.Add(r, e); });
  mod.method("Add",        [](BRepFilletAPI_MakeFillet& m, double r1, double r2, const TopoDS_Edge& e) {
    m.Add(r1, r2, e);
  });
  mod.method("SetRadius",  [](BRepFilletAPI_MakeFillet& m, double radius, int ic, int iinc) {
    m.SetRadius(radius, ic, iinc);
  });
  mod.method("SetRadius",  [](BRepFilletAPI_MakeFillet& m, double r1, double r2, int ic, int iinc) {
    m.SetRadius(r1, r2, ic, iinc);
  });
  mod.method("SetRadius",  [](BRepFilletAPI_MakeFillet& m, double radius, int ic, const TopoDS_Edge& e) {
    m.SetRadius(radius, ic, e);
  });
  mod.method("SetRadius",  [](BRepFilletAPI_MakeFillet& m, double radius, int ic, const TopoDS_Vertex& v) {
    m.SetRadius(radius, ic, v);
  });
  mod.method("Edge",       [](const BRepFilletAPI_MakeFillet& m, int ic, int iinc) -> TopoDS_Edge {
    return m.Edge(ic, iinc);
  });
  mod.method("Radius",     [](BRepFilletAPI_MakeFillet& m, int ic) -> double { return m.Radius(ic); });
  mod.method("Radius",     [](BRepFilletAPI_MakeFillet& m, int ic, const TopoDS_Edge& e) -> double {
    return m.Radius(ic, e);
  });
  mod.method("IsConstant", [](BRepFilletAPI_MakeFillet& m, int ic) -> bool { return bool(m.IsConstant(ic)); });
  mod.method("IsConstant", [](BRepFilletAPI_MakeFillet& m, int ic, const TopoDS_Edge& e) -> bool {
    return bool(m.IsConstant(ic, e));
  });
  mod.method("Build",      [](BRepFilletAPI_MakeFillet& m) { m.Build(); });
  mod.method("IsDone",     [](const BRepFilletAPI_MakeFillet& m) { return bool(m.IsDone()); });
  mod.method("Shape",      [](BRepFilletAPI_MakeFillet& m) -> TopoDS_Shape { return m.Shape(); });
  mod.method("NbContours", [](const BRepFilletAPI_MakeFillet& m) { return m.NbContours(); });

  mod.method("Add",    [](BRepFilletAPI_MakeChamfer& m, double d, const TopoDS_Edge& e) { m.Add(d, e); });
  mod.method("Add",    [](BRepFilletAPI_MakeChamfer& m, double dis1, double dis2,
                           const TopoDS_Edge& e, const TopoDS_Face& f) {
    m.Add(dis1, dis2, e, f);
  });
  mod.method("AddDA",  [](BRepFilletAPI_MakeChamfer& m, double dis, double angle,
                           const TopoDS_Edge& e, const TopoDS_Face& f) {
    m.AddDA(dis, angle, e, f);
  });
  mod.method("SetDists", [](BRepFilletAPI_MakeChamfer& m, double dis1, double dis2,
                             int ic, const TopoDS_Face& f) {
    m.SetDists(dis1, dis2, ic, f);
  });
  mod.method("SetDistAngle", [](BRepFilletAPI_MakeChamfer& m, double dis, double angle,
                                 int ic, const TopoDS_Face& f) {
    m.SetDistAngle(dis, angle, ic, f);
  });
  mod.method("Build",  [](BRepFilletAPI_MakeChamfer& m) { m.Build(); });
  mod.method("IsDone", [](const BRepFilletAPI_MakeChamfer& m) { return bool(m.IsDone()); });
  mod.method("Shape",  [](BRepFilletAPI_MakeChamfer& m) -> TopoDS_Shape { return m.Shape(); });
}
