// occ_extrema.cpp — 1:1 CxxWrap bindings for shape distance and content analysis.
#include <jlcxx/jlcxx.hpp>

#include <BRepExtrema_DistShapeShape.hxx>
#include <ShapeAnalysis_ShapeContents.hxx>
#include <TopoDS_Shape.hxx>
#include <gp_Pnt.hxx>

void register_occ_extrema(jlcxx::Module& mod) {
  // ---- add_type calls first ----

  mod.add_type<BRepExtrema_DistShapeShape>("BRepExtrema_DistShapeShape")
     .constructor<>()
     .constructor<const TopoDS_Shape&, const TopoDS_Shape&>();

  mod.add_type<ShapeAnalysis_ShapeContents>("ShapeAnalysis_ShapeContents")
     .constructor<>();

  // ---- BRepExtrema_DistShapeShape methods ----

  mod.method("Perform", [](BRepExtrema_DistShapeShape& d) -> bool {
    return bool(d.Perform());
  });
  mod.method("IsDone", [](const BRepExtrema_DistShapeShape& d) -> bool {
    return bool(d.IsDone());
  });
  mod.method("Value", [](const BRepExtrema_DistShapeShape& d) -> double {
    return d.Value();
  });
  mod.method("NbSolution", [](const BRepExtrema_DistShapeShape& d) -> int {
    return int(d.NbSolution());
  });
  mod.method("PointOnShape1", [](const BRepExtrema_DistShapeShape& d, int idx) -> gp_Pnt {
    return d.PointOnShape1(idx);
  });
  mod.method("PointOnShape2", [](const BRepExtrema_DistShapeShape& d, int idx) -> gp_Pnt {
    return d.PointOnShape2(idx);
  });
  mod.method("InnerSolution", [](const BRepExtrema_DistShapeShape& d) -> bool {
    return bool(d.InnerSolution());
  });

  // ---- ShapeAnalysis_ShapeContents methods ----

  mod.method("Perform", [](ShapeAnalysis_ShapeContents& c, const TopoDS_Shape& s) {
    c.Perform(s);
  });
  mod.method("NbSolids",   [](const ShapeAnalysis_ShapeContents& c) -> int { return int(c.NbSolids()); });
  mod.method("NbShells",   [](const ShapeAnalysis_ShapeContents& c) -> int { return int(c.NbShells()); });
  mod.method("NbFaces",    [](const ShapeAnalysis_ShapeContents& c) -> int { return int(c.NbFaces()); });
  mod.method("NbWires",    [](const ShapeAnalysis_ShapeContents& c) -> int { return int(c.NbWires()); });
  mod.method("NbEdges",    [](const ShapeAnalysis_ShapeContents& c) -> int { return int(c.NbEdges()); });
  mod.method("NbVertices", [](const ShapeAnalysis_ShapeContents& c) -> int { return int(c.NbVertices()); });
}
