// occ_adaptor.cpp — 1:1 CxxWrap bindings for BRepAdaptor_Curve and BRepAdaptor_Surface.
#include <jlcxx/jlcxx.hpp>
#include <BRepAdaptor_Curve.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <gp_Dir.hxx>

void register_occ_adaptor(jlcxx::Module& mod) {
  mod.add_type<BRepAdaptor_Curve>("BRepAdaptor_Curve")
     .constructor<>()
     .constructor<const TopoDS_Edge&>()
     .constructor<const TopoDS_Edge&, const TopoDS_Face&>();

  mod.add_type<BRepAdaptor_Surface>("BRepAdaptor_Surface")
     .constructor<>()
     .constructor<const TopoDS_Face&>();

  // BRepAdaptor_Curve methods
  mod.method("FirstParameter", [](const BRepAdaptor_Curve& c) -> double {
    return c.FirstParameter();
  });
  mod.method("LastParameter", [](const BRepAdaptor_Curve& c) -> double {
    return c.LastParameter();
  });
  mod.method("Tolerance", [](const BRepAdaptor_Curve& c) -> double {
    return c.Tolerance();
  });
  mod.method("IsClosed", [](const BRepAdaptor_Curve& c) -> bool {
    return c.IsClosed();
  });
  mod.method("IsPeriodic", [](const BRepAdaptor_Curve& c) -> bool {
    return c.IsPeriodic();
  });
  mod.method("Value", [](const BRepAdaptor_Curve& c, double t) -> gp_Pnt {
    return c.Value(t);
  });
  mod.method("D0", [](const BRepAdaptor_Curve& c, double t, gp_Pnt& p) {
    c.D0(t, p);
  });
  mod.method("D1", [](const BRepAdaptor_Curve& c, double t, gp_Pnt& p, gp_Vec& v) {
    c.D1(t, p, v);
  });
  mod.method("D2", [](const BRepAdaptor_Curve& c, double t, gp_Pnt& p, gp_Vec& v1, gp_Vec& v2) {
    c.D2(t, p, v1, v2);
  });

  // BRepAdaptor_Surface methods
  mod.method("FirstUParameter", [](const BRepAdaptor_Surface& s) -> double {
    return s.FirstUParameter();
  });
  mod.method("LastUParameter", [](const BRepAdaptor_Surface& s) -> double {
    return s.LastUParameter();
  });
  mod.method("FirstVParameter", [](const BRepAdaptor_Surface& s) -> double {
    return s.FirstVParameter();
  });
  mod.method("LastVParameter", [](const BRepAdaptor_Surface& s) -> double {
    return s.LastVParameter();
  });
  mod.method("Tolerance", [](const BRepAdaptor_Surface& s) -> double {
    return s.Tolerance();
  });
  mod.method("Value", [](const BRepAdaptor_Surface& s, double u, double v) -> gp_Pnt {
    return s.Value(u, v);
  });
  mod.method("D0", [](const BRepAdaptor_Surface& s, double u, double v, gp_Pnt& p) {
    s.D0(u, v, p);
  });
  mod.method("D1", [](const BRepAdaptor_Surface& s, double u, double v, gp_Pnt& p, gp_Vec& du, gp_Vec& dv) {
    s.D1(u, v, p, du, dv);
  });
  mod.method("Normal", [](const BRepAdaptor_Surface& s, double u, double v) -> gp_Dir {
    gp_Pnt p; gp_Vec d1u, d1v;
    s.D1(u, v, p, d1u, d1v);
    return gp_Dir(d1u.Crossed(d1v));
  });
}
