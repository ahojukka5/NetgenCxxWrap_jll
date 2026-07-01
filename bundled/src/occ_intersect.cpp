// occ_intersect.cpp — 1:1 CxxWrap bindings for ray/line-shape intersection.
#include <jlcxx/jlcxx.hpp>

#include <IntCurvesFace_ShapeIntersector.hxx>
#include <IntCurveSurface_TransitionOnCurve.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <gp_Lin.hxx>
#include <gp_Pnt.hxx>

void register_occ_intersect(jlcxx::Module& mod) {
  // ---- add_type calls first ----

  mod.add_type<IntCurvesFace_ShapeIntersector>("IntCurvesFace_ShapeIntersector")
     .constructor<>();

  // ---- methods ----

  mod.method("Load", [](IntCurvesFace_ShapeIntersector& s, const TopoDS_Shape& sh, double tol) {
    s.Load(sh, tol);
  });
  mod.method("Perform", [](IntCurvesFace_ShapeIntersector& s, const gp_Lin& l, double pinf, double psup) {
    s.Perform(l, pinf, psup);
  });
  mod.method("PerformNearest", [](IntCurvesFace_ShapeIntersector& s, const gp_Lin& l, double pinf, double psup) {
    s.PerformNearest(l, pinf, psup);
  });
  mod.method("IsDone",  [](const IntCurvesFace_ShapeIntersector& s) -> bool { return bool(s.IsDone()); });
  mod.method("NbPnt",   [](const IntCurvesFace_ShapeIntersector& s) -> int  { return int(s.NbPnt()); });
  mod.method("Pnt",     [](const IntCurvesFace_ShapeIntersector& s, int i) -> gp_Pnt { return s.Pnt(i); });
  mod.method("WParameter", [](const IntCurvesFace_ShapeIntersector& s, int i) -> double { return s.WParameter(i); });
  mod.method("UParameter", [](const IntCurvesFace_ShapeIntersector& s, int i) -> double { return s.UParameter(i); });
  mod.method("VParameter", [](const IntCurvesFace_ShapeIntersector& s, int i) -> double { return s.VParameter(i); });
  mod.method("State", [](const IntCurvesFace_ShapeIntersector& s, int i) -> int {
    return int(s.State(i));
  });
  mod.method("Face", [](const IntCurvesFace_ShapeIntersector& s, int i) -> TopoDS_Face {
    return s.Face(i);
  });
  mod.method("Transition", [](const IntCurvesFace_ShapeIntersector& s, int i) -> int {
    return int(s.Transition(i));
  });
  mod.method("SortResult", [](IntCurvesFace_ShapeIntersector& s) { s.SortResult(); });

  // ---- IntCurveSurface_TransitionOnCurve constants ----

  mod.method("IntCurveSurface_Tangent", []() { return int(IntCurveSurface_Tangent); });
  mod.method("IntCurveSurface_In",      []() { return int(IntCurveSurface_In); });
  mod.method("IntCurveSurface_Out",     []() { return int(IntCurveSurface_Out); });
}
