// occ_geom.cpp — CxxWrap bindings for the Handle(*)-managed Geom_Curve /
// Geom_Surface hierarchy: base virtual interface, NURBS/Bezier curve
// construction, curve fitting, point projection, curve-curve extrema.
//
// Must be registered (register_occ_geom) right after register_occ_topology and
// before register_occ_builders — occ_builders.cpp's extended MakeEdge/MakeFace
// constructors reference Handle(Geom_Curve)/Handle(Geom_Surface), which must
// already be add_type'd by the time those add_type chains are built.
#include "occ_handle_traits.hpp"
#include <jlcxx/array.hpp>

#include <Geom_Curve.hxx>
#include <Geom_Surface.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_BezierCurve.hxx>
#include <GeomAPI_PointsToBSpline.hxx>
#include <GeomAPI_ProjectPointOnCurve.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>
#include <GeomAPI_ExtremaCurveCurve.hxx>

#include <TColgp_Array1OfPnt.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColStd_Array1OfInteger.hxx>

#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <GeomAbs_Shape.hxx>

namespace {
  // Marshal a flat, interleaved-XYZ jlcxx::ArrayRef<double> into a 1-based
  // TColgp_Array1OfPnt. Mechanical type-bridging, not OCCT business logic --
  // the same category of exception already used for discarding multi-out-params.
  TColgp_Array1OfPnt PolesFromFlat(jlcxx::ArrayRef<double> flatXYZ) {
    int n = int(flatXYZ.size()) / 3;
    TColgp_Array1OfPnt poles(1, n);
    for (int i = 0; i < n; ++i) {
      poles.SetValue(i + 1, gp_Pnt(flatXYZ[3*i], flatXYZ[3*i+1], flatXYZ[3*i+2]));
    }
    return poles;
  }
  TColStd_Array1OfReal RealsFrom(jlcxx::ArrayRef<double> vals) {
    int n = int(vals.size());
    TColStd_Array1OfReal arr(1, n);
    for (int i = 0; i < n; ++i) arr.SetValue(i + 1, vals[i]);
    return arr;
  }
  TColStd_Array1OfInteger IntsFrom(jlcxx::ArrayRef<int32_t> vals) {
    int n = int(vals.size());
    TColStd_Array1OfInteger arr(1, n);
    for (int i = 0; i < n; ++i) arr.SetValue(i + 1, vals[i]);
    return arr;
  }
}

void register_occ_geom(jlcxx::Module& mod) {
  // Register the opencascade::handle<T> smart-pointer TEMPLATE itself (distinct
  // from the IsSmartPointerType trait in occ_handle_traits.hpp) -- this must
  // happen exactly once, before any Handle(X) type is referenced in a wrapped
  // signature. CxxWrap does this automatically for std::shared_ptr/unique_ptr/
  // weak_ptr in its own bootstrap; opencascade::handle is our custom template,
  // so we register it ourselves the same way.
  jlcxx::add_smart_pointer<opencascade::handle>(mod, "Handle");

  mod.add_type<Geom_Curve>("Geom_Curve");
  mod.add_type<Geom_Surface>("Geom_Surface");

  // ---- Geom_Curve ----
  mod.method("Value", [](const Handle(Geom_Curve)& c, double u) -> gp_Pnt { return c->Value(u); });
  mod.method("D1", [](const Handle(Geom_Curve)& c, double u) -> gp_Vec {
    gp_Pnt p; gp_Vec v1;
    c->D1(u, p, v1);
    return v1;
  });
  mod.method("FirstParameter", [](const Handle(Geom_Curve)& c) -> double { return c->FirstParameter(); });
  mod.method("LastParameter",  [](const Handle(Geom_Curve)& c) -> double { return c->LastParameter(); });
  mod.method("IsClosed",   [](const Handle(Geom_Curve)& c) -> bool { return bool(c->IsClosed()); });
  mod.method("IsPeriodic", [](const Handle(Geom_Curve)& c) -> bool { return bool(c->IsPeriodic()); });
  mod.method("Continuity", [](const Handle(Geom_Curve)& c) -> int { return int(c->Continuity()); });
  mod.method("Reversed",   [](const Handle(Geom_Curve)& c) -> Handle(Geom_Curve) { return c->Reversed(); });

  // ---- Geom_Surface ----
  mod.method("Value", [](const Handle(Geom_Surface)& s, double u, double v) -> gp_Pnt { return s->Value(u, v); });
  mod.method("IsUClosed",   [](const Handle(Geom_Surface)& s) -> bool { return bool(s->IsUClosed()); });
  mod.method("IsVClosed",   [](const Handle(Geom_Surface)& s) -> bool { return bool(s->IsVClosed()); });
  mod.method("IsUPeriodic", [](const Handle(Geom_Surface)& s) -> bool { return bool(s->IsUPeriodic()); });
  mod.method("IsVPeriodic", [](const Handle(Geom_Surface)& s) -> bool { return bool(s->IsVPeriodic()); });
  mod.method("Continuity",  [](const Handle(Geom_Surface)& s) -> int { return int(s->Continuity()); });
  mod.method("FirstUParameter", [](const Handle(Geom_Surface)& s) -> double {
    double u1, u2, v1, v2; s->Bounds(u1, u2, v1, v2); return u1;
  });
  mod.method("LastUParameter", [](const Handle(Geom_Surface)& s) -> double {
    double u1, u2, v1, v2; s->Bounds(u1, u2, v1, v2); return u2;
  });
  mod.method("FirstVParameter", [](const Handle(Geom_Surface)& s) -> double {
    double u1, u2, v1, v2; s->Bounds(u1, u2, v1, v2); return v1;
  });
  mod.method("LastVParameter", [](const Handle(Geom_Surface)& s) -> double {
    double u1, u2, v1, v2; s->Bounds(u1, u2, v1, v2); return v2;
  });

  // ---- Concrete curve construction (free-function factories, named like the
  // class -- exactly the new_mesh() precedent for smart-pointer-managed types) ----
  mod.method("Geom_BSplineCurve", [](jlcxx::ArrayRef<double> poles, jlcxx::ArrayRef<double> knots,
                                      jlcxx::ArrayRef<int32_t> mults, int degree,
                                      bool periodic) -> Handle(Geom_Curve) {
    TColgp_Array1OfPnt p = PolesFromFlat(poles);
    TColStd_Array1OfReal k = RealsFrom(knots);
    TColStd_Array1OfInteger m = IntsFrom(mults);
    return new Geom_BSplineCurve(p, k, m, degree, periodic);
  });
  mod.method("Geom_BezierCurve", [](jlcxx::ArrayRef<double> poles) -> Handle(Geom_Curve) {
    TColgp_Array1OfPnt p = PolesFromFlat(poles);
    return new Geom_BezierCurve(p);
  });
  mod.method("Geom_BezierCurve", [](jlcxx::ArrayRef<double> poles,
                                     jlcxx::ArrayRef<double> weights) -> Handle(Geom_Curve) {
    TColgp_Array1OfPnt p = PolesFromFlat(poles);
    TColStd_Array1OfReal w = RealsFrom(weights);
    return new Geom_BezierCurve(p, w);
  });

  // ---- Curve fitting ----
  mod.method("GeomAPI_PointsToBSpline", [](jlcxx::ArrayRef<double> points, int degMin, int degMax,
                                            int continuity, double tol3d) -> Handle(Geom_Curve) {
    TColgp_Array1OfPnt p = PolesFromFlat(points);
    GeomAPI_PointsToBSpline fitter(p, degMin, degMax, GeomAbs_Shape(continuity), tol3d);
    return fitter.Curve();
  });

  // ---- Point projection ----
  mod.add_type<GeomAPI_ProjectPointOnCurve>("GeomAPI_ProjectPointOnCurve")
     .constructor<const gp_Pnt&, const Handle(Geom_Curve)&>()
     .constructor<const gp_Pnt&, const Handle(Geom_Curve)&, double, double>();
  mod.method("NbPoints", [](const GeomAPI_ProjectPointOnCurve& p) -> int { return p.NbPoints(); });
  mod.method("Point",    [](const GeomAPI_ProjectPointOnCurve& p, int i) -> gp_Pnt { return p.Point(i); });
  mod.method("Distance", [](const GeomAPI_ProjectPointOnCurve& p, int i) -> double { return p.Distance(i); });
  mod.method("NearestPoint", [](const GeomAPI_ProjectPointOnCurve& p) -> gp_Pnt { return p.NearestPoint(); });
  mod.method("LowerDistance", [](const GeomAPI_ProjectPointOnCurve& p) -> double { return p.LowerDistance(); });
  mod.method("LowerDistanceParameter", [](const GeomAPI_ProjectPointOnCurve& p) -> double {
    return p.LowerDistanceParameter();
  });

  mod.add_type<GeomAPI_ProjectPointOnSurf>("GeomAPI_ProjectPointOnSurf")
     .constructor<const gp_Pnt&, const Handle(Geom_Surface)&>();
  mod.method("NbPoints", [](const GeomAPI_ProjectPointOnSurf& p) -> int { return p.NbPoints(); });
  mod.method("Point",    [](const GeomAPI_ProjectPointOnSurf& p, int i) -> gp_Pnt { return p.Point(i); });
  mod.method("Distance", [](const GeomAPI_ProjectPointOnSurf& p, int i) -> double { return p.Distance(i); });
  mod.method("NearestPoint", [](const GeomAPI_ProjectPointOnSurf& p) -> gp_Pnt { return p.NearestPoint(); });
  mod.method("LowerDistance", [](const GeomAPI_ProjectPointOnSurf& p) -> double { return p.LowerDistance(); });
  mod.method("IsDone", [](const GeomAPI_ProjectPointOnSurf& p) -> bool { return bool(p.IsDone()); });

  // ---- Curve-curve extrema ----
  mod.add_type<GeomAPI_ExtremaCurveCurve>("GeomAPI_ExtremaCurveCurve")
     .constructor<const Handle(Geom_Curve)&, const Handle(Geom_Curve)&>();
  mod.method("NbExtrema",  [](const GeomAPI_ExtremaCurveCurve& e) -> int { return e.NbExtrema(); });
  mod.method("Distance",   [](const GeomAPI_ExtremaCurveCurve& e, int i) -> double { return e.Distance(i); });
  mod.method("IsParallel", [](const GeomAPI_ExtremaCurveCurve& e) -> bool { return bool(e.IsParallel()); });
  mod.method("LowerDistance", [](const GeomAPI_ExtremaCurveCurve& e) -> double { return e.LowerDistance(); });
  mod.method("TotalLowerDistance", [](GeomAPI_ExtremaCurveCurve& e) -> double { return e.TotalLowerDistance(); });
}
