// occ_breplib.cpp — 1:1 CxxWrap bindings for BRepLib repair/regularization
// free functions, plus the GeomAbs_Shape continuity constants they take.
#include <jlcxx/jlcxx.hpp>

#include <BRepLib.hxx>
#include <GeomAbs_Shape.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Solid.hxx>
#include <TopTools_ListOfShape.hxx>

void register_occ_breplib(jlcxx::Module& mod) {
  mod.method("BRepLib_CheckSameRange", [](const TopoDS_Edge& e, double confusion) -> bool {
    return bool(BRepLib::CheckSameRange(e, confusion));
  });
  mod.method("BRepLib_SameRange", [](const TopoDS_Edge& e, double tol) {
    BRepLib::SameRange(e, tol);
  });
  mod.method("BRepLib_BuildCurve3d", [](const TopoDS_Edge& e, double tol, int continuity,
                                         int maxDegree, int maxSegment) -> bool {
    return bool(BRepLib::BuildCurve3d(e, tol, GeomAbs_Shape(continuity), maxDegree, maxSegment));
  });
  mod.method("BRepLib_BuildCurves3d", [](const TopoDS_Shape& s, double tol, int continuity,
                                          int maxDegree, int maxSegment) -> bool {
    return bool(BRepLib::BuildCurves3d(s, tol, GeomAbs_Shape(continuity), maxDegree, maxSegment));
  });
  mod.method("BRepLib_BuildCurves3d", [](const TopoDS_Shape& s) -> bool {
    return bool(BRepLib::BuildCurves3d(s));
  });
  mod.method("BRepLib_SameParameter", [](const TopoDS_Edge& e, double tol) {
    BRepLib::SameParameter(e, tol);
  });
  mod.method("BRepLib_SameParameter", [](const TopoDS_Shape& s, double tol, bool forced) {
    BRepLib::SameParameter(s, tol, forced);
  });
  mod.method("BRepLib_OrientClosedSolid", [](TopoDS_Solid& solid) -> bool {
    return bool(BRepLib::OrientClosedSolid(solid));
  });
  mod.method("BRepLib_EncodeRegularity", [](const TopoDS_Shape& s, double tolAng) {
    BRepLib::EncodeRegularity(s, tolAng);
  });
  mod.method("BRepLib_EncodeRegularity", [](const TopoDS_Shape& s, const TopTools_ListOfShape& le,
                                             double tolAng) {
    BRepLib::EncodeRegularity(s, le, tolAng);
  });
  mod.method("BRepLib_EncodeRegularity", [](TopoDS_Edge& e, const TopoDS_Face& f1,
                                             const TopoDS_Face& f2, double tolAng) {
    BRepLib::EncodeRegularity(e, f1, f2, tolAng);
  });

  mod.method("GeomAbs_C0", []() { return int(GeomAbs_C0); });
  mod.method("GeomAbs_G1", []() { return int(GeomAbs_G1); });
  mod.method("GeomAbs_C1", []() { return int(GeomAbs_C1); });
  mod.method("GeomAbs_G2", []() { return int(GeomAbs_G2); });
  mod.method("GeomAbs_C2", []() { return int(GeomAbs_C2); });
  mod.method("GeomAbs_C3", []() { return int(GeomAbs_C3); });
  mod.method("GeomAbs_CN", []() { return int(GeomAbs_CN); });
}
