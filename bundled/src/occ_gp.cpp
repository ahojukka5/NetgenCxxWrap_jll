// occ_gp.cpp — 1:1 CxxWrap bindings for OCC gp_ value types (3D, 2D, analytic).
#include <jlcxx/jlcxx.hpp>

#include <gp_XYZ.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <gp_Dir.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Ax3.hxx>
#include <gp_Trsf.hxx>
#include <gp_XY.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Vec2d.hxx>
#include <gp_Dir2d.hxx>
#include <gp_Ax2d.hxx>
#include <gp_Trsf2d.hxx>
#include <gp_Lin.hxx>
#include <gp_Circ.hxx>
#include <gp_Pln.hxx>
#include <gp_Elips.hxx>
#include <gp_Parab.hxx>
#include <gp_Hypr.hxx>
#include <gp_Cylinder.hxx>
#include <gp_Cone.hxx>
#include <gp_Sphere.hxx>
#include <gp_Torus.hxx>
#include <gp_Mat.hxx>
#include <gp_GTrsf.hxx>

void register_occ_gp(jlcxx::Module& mod) {
  // ===== gp: 3D value types =================================================
  mod.add_type<gp_XYZ>("gp_XYZ").constructor<>().constructor<double, double, double>();
  mod.method("X", [](const gp_XYZ& p) { return p.X(); });
  mod.method("Y", [](const gp_XYZ& p) { return p.Y(); });
  mod.method("Z", [](const gp_XYZ& p) { return p.Z(); });
  mod.method("Coord", [](const gp_XYZ& p, int i) { return p.Coord(i); });

  mod.add_type<gp_Pnt>("gp_Pnt").constructor<>().constructor<double, double, double>();
  mod.method("X", [](const gp_Pnt& p) { return p.X(); });
  mod.method("Y", [](const gp_Pnt& p) { return p.Y(); });
  mod.method("Z", [](const gp_Pnt& p) { return p.Z(); });
  mod.method("Coord", [](const gp_Pnt& p, int i) { return p.Coord(i); });
  mod.method("SetX", [](gp_Pnt& p, double v) { p.SetX(v); });
  mod.method("SetY", [](gp_Pnt& p, double v) { p.SetY(v); });
  mod.method("SetZ", [](gp_Pnt& p, double v) { p.SetZ(v); });
  mod.method("Distance", [](const gp_Pnt& a, const gp_Pnt& b) { return a.Distance(b); });

  mod.add_type<gp_Vec>("gp_Vec").constructor<>().constructor<double, double, double>();
  mod.method("X", [](const gp_Vec& v) { return v.X(); });
  mod.method("Y", [](const gp_Vec& v) { return v.Y(); });
  mod.method("Z", [](const gp_Vec& v) { return v.Z(); });
  mod.method("Magnitude", [](const gp_Vec& v) { return v.Magnitude(); });

  mod.add_type<gp_Dir>("gp_Dir").constructor<>().constructor<double, double, double>();
  mod.method("X", [](const gp_Dir& d) { return d.X(); });
  mod.method("Y", [](const gp_Dir& d) { return d.Y(); });
  mod.method("Z", [](const gp_Dir& d) { return d.Z(); });

  mod.add_type<gp_Ax1>("gp_Ax1").constructor<>().constructor<const gp_Pnt&, const gp_Dir&>();
  mod.method("Location",  [](const gp_Ax1& a) { return a.Location(); });
  mod.method("Direction", [](const gp_Ax1& a) { return a.Direction(); });

  mod.add_type<gp_Ax2>("gp_Ax2").constructor<>()
     .constructor<const gp_Pnt&, const gp_Dir&>()
     .constructor<const gp_Pnt&, const gp_Dir&, const gp_Dir&>();
  mod.method("Location",   [](const gp_Ax2& a) { return a.Location(); });
  mod.method("Direction",  [](const gp_Ax2& a) { return a.Direction(); });
  mod.method("XDirection", [](const gp_Ax2& a) { return a.XDirection(); });
  mod.method("YDirection", [](const gp_Ax2& a) { return a.YDirection(); });

  mod.add_type<gp_Ax3>("gp_Ax3").constructor<>().constructor<const gp_Pnt&, const gp_Dir&>();

  mod.add_type<gp_Trsf>("gp_Trsf").constructor<>();
  mod.method("SetTranslation", [](gp_Trsf& t, const gp_Vec& v)           { t.SetTranslation(v); });
  mod.method("SetRotation",    [](gp_Trsf& t, const gp_Ax1& a, double ang) { t.SetRotation(a, ang); });
  mod.method("SetScale",       [](gp_Trsf& t, const gp_Pnt& p, double s) { t.SetScale(p, s); });

  // ===== gp: 2D value types =================================================
  mod.add_type<gp_XY>("gp_XY").constructor<>().constructor<double, double>();
  mod.method("X", [](const gp_XY& p) { return p.X(); });
  mod.method("Y", [](const gp_XY& p) { return p.Y(); });

  mod.add_type<gp_Pnt2d>("gp_Pnt2d").constructor<>().constructor<double, double>();
  mod.method("X", [](const gp_Pnt2d& p) { return p.X(); });
  mod.method("Y", [](const gp_Pnt2d& p) { return p.Y(); });
  mod.method("Distance", [](const gp_Pnt2d& a, const gp_Pnt2d& b) { return a.Distance(b); });

  mod.add_type<gp_Vec2d>("gp_Vec2d").constructor<>().constructor<double, double>();
  mod.add_type<gp_Dir2d>("gp_Dir2d").constructor<>().constructor<double, double>();
  mod.add_type<gp_Ax2d>("gp_Ax2d").constructor<>()
     .constructor<const gp_Pnt2d&, const gp_Dir2d&>();
  mod.add_type<gp_Trsf2d>("gp_Trsf2d").constructor<>();

  // ===== gp: analytic primitives ============================================
  mod.add_type<gp_Lin>("gp_Lin").constructor<>()
     .constructor<const gp_Pnt&, const gp_Dir&>();
  mod.add_type<gp_Circ>("gp_Circ").constructor<>()
     .constructor<const gp_Ax2&, double>();
  mod.method("Radius", [](const gp_Circ& c) { return c.Radius(); });
  mod.add_type<gp_Pln>("gp_Pln").constructor<>()
     .constructor<const gp_Pnt&, const gp_Dir&>();
  mod.add_type<gp_Elips>("gp_Elips").constructor<>()
     .constructor<const gp_Ax2&, double, double>();
  mod.method("MajorRadius", [](const gp_Elips& e) { return e.MajorRadius(); });
  mod.method("MinorRadius", [](const gp_Elips& e) { return e.MinorRadius(); });

  mod.add_type<gp_Parab>("gp_Parab").constructor<>()
     .constructor<const gp_Ax2&, double>();
  mod.method("Focal",    [](const gp_Parab& p) -> double { return p.Focal(); });
  mod.method("Focus",    [](const gp_Parab& p) -> gp_Pnt { return p.Focus(); });
  mod.method("Parameter",[](const gp_Parab& p) -> double { return p.Parameter(); });
  mod.method("Directrix",[](const gp_Parab& p) -> gp_Ax1 { return p.Directrix(); });
  mod.method("Axis",     [](const gp_Parab& p) -> gp_Ax1 { return p.Axis(); });
  mod.method("Location", [](const gp_Parab& p) -> gp_Pnt { return p.Location(); });
  mod.method("XAxis",    [](const gp_Parab& p) -> gp_Ax1 { return p.XAxis(); });
  mod.method("YAxis",    [](const gp_Parab& p) -> gp_Ax1 { return p.YAxis(); });
  mod.method("SetFocal",   [](gp_Parab& p, double f)        { p.SetFocal(f); });
  mod.method("SetLocation",[](gp_Parab& p, const gp_Pnt& q) { p.SetLocation(q); });

  mod.add_type<gp_Hypr>("gp_Hypr").constructor<>()
     .constructor<const gp_Ax2&, double, double>();
  mod.method("Eccentricity", [](const gp_Hypr& h) -> double  { return h.Eccentricity(); });
  mod.method("Focal",        [](const gp_Hypr& h) -> double  { return h.Focal(); });
  mod.method("MajorRadius",  [](const gp_Hypr& h) -> double  { return h.MajorRadius(); });
  mod.method("MinorRadius",  [](const gp_Hypr& h) -> double  { return h.MinorRadius(); });
  mod.method("Parameter",    [](const gp_Hypr& h) -> double  { return h.Parameter(); });
  mod.method("Asymptote1",   [](const gp_Hypr& h) -> gp_Ax1 { return h.Asymptote1(); });
  mod.method("Asymptote2",   [](const gp_Hypr& h) -> gp_Ax1 { return h.Asymptote2(); });
  mod.method("OtherBranch",  [](const gp_Hypr& h) -> gp_Hypr { return h.OtherBranch(); });
  mod.method("Focus1",       [](const gp_Hypr& h) -> gp_Pnt { return h.Focus1(); });
  mod.method("Focus2",       [](const gp_Hypr& h) -> gp_Pnt { return h.Focus2(); });
  mod.method("Axis",         [](const gp_Hypr& h) -> gp_Ax1 { return h.Axis(); });
  mod.method("Location",     [](const gp_Hypr& h) -> gp_Pnt { return h.Location(); });
  mod.method("SetMajorRadius",[](gp_Hypr& h, double r) { h.SetMajorRadius(r); });
  mod.method("SetMinorRadius",[](gp_Hypr& h, double r) { h.SetMinorRadius(r); });

  // ===== gp: analytic surfaces ===============================================
  mod.add_type<gp_Cylinder>("gp_Cylinder").constructor<>()
     .constructor<const gp_Ax3&, double>();
  mod.method("SetAxis",    [](gp_Cylinder& c, const gp_Ax1& a) { c.SetAxis(a); });
  mod.method("SetLocation",[](gp_Cylinder& c, const gp_Pnt& p) { c.SetLocation(p); });
  mod.method("SetPosition",[](gp_Cylinder& c, const gp_Ax3& a) { c.SetPosition(a); });
  mod.method("SetRadius",  [](gp_Cylinder& c, double r)        { c.SetRadius(r); });
  mod.method("Axis",       [](const gp_Cylinder& c) -> gp_Ax1 { return c.Axis(); });
  mod.method("Position",   [](const gp_Cylinder& c) -> gp_Ax3 { return c.Position(); });
  mod.method("Location",   [](const gp_Cylinder& c) -> gp_Pnt { return c.Location(); });
  mod.method("Radius",     [](const gp_Cylinder& c) -> double { return c.Radius(); });
  mod.method("XAxis",      [](const gp_Cylinder& c) -> gp_Ax1 { return c.XAxis(); });
  mod.method("YAxis",      [](const gp_Cylinder& c) -> gp_Ax1 { return c.YAxis(); });
  mod.method("Direct",     [](const gp_Cylinder& c) -> bool   { return bool(c.Direct()); });

  mod.add_type<gp_Cone>("gp_Cone").constructor<>()
     .constructor<const gp_Ax3&, double, double>();
  mod.method("SetAxis",     [](gp_Cone& c, const gp_Ax1& a) { c.SetAxis(a); });
  mod.method("SetLocation", [](gp_Cone& c, const gp_Pnt& p) { c.SetLocation(p); });
  mod.method("SetPosition", [](gp_Cone& c, const gp_Ax3& a) { c.SetPosition(a); });
  mod.method("SetRadius",   [](gp_Cone& c, double r)        { c.SetRadius(r); });
  mod.method("SetSemiAngle",[](gp_Cone& c, double a)        { c.SetSemiAngle(a); });
  mod.method("Apex",        [](const gp_Cone& c) -> gp_Pnt { return c.Apex(); });
  mod.method("Axis",        [](const gp_Cone& c) -> gp_Ax1 { return c.Axis(); });
  mod.method("Position",    [](const gp_Cone& c) -> gp_Ax3 { return c.Position(); });
  mod.method("Location",    [](const gp_Cone& c) -> gp_Pnt { return c.Location(); });
  mod.method("RefRadius",   [](const gp_Cone& c) -> double { return c.RefRadius(); });
  mod.method("SemiAngle",   [](const gp_Cone& c) -> double { return c.SemiAngle(); });
  mod.method("XAxis",       [](const gp_Cone& c) -> gp_Ax1 { return c.XAxis(); });
  mod.method("YAxis",       [](const gp_Cone& c) -> gp_Ax1 { return c.YAxis(); });
  mod.method("Direct",      [](const gp_Cone& c) -> bool   { return bool(c.Direct()); });

  mod.add_type<gp_Sphere>("gp_Sphere").constructor<>()
     .constructor<const gp_Ax3&, double>();
  mod.method("SetLocation",[](gp_Sphere& s, const gp_Pnt& p) { s.SetLocation(p); });
  mod.method("SetPosition",[](gp_Sphere& s, const gp_Ax3& a) { s.SetPosition(a); });
  mod.method("SetRadius",  [](gp_Sphere& s, double r)        { s.SetRadius(r); });
  mod.method("Area",       [](const gp_Sphere& s) -> double { return s.Area(); });
  mod.method("Volume",     [](const gp_Sphere& s) -> double { return s.Volume(); });
  mod.method("Position",   [](const gp_Sphere& s) -> gp_Ax3 { return s.Position(); });
  mod.method("Location",   [](const gp_Sphere& s) -> gp_Pnt { return s.Location(); });
  mod.method("Radius",     [](const gp_Sphere& s) -> double { return s.Radius(); });
  mod.method("XAxis",      [](const gp_Sphere& s) -> gp_Ax1 { return s.XAxis(); });
  mod.method("YAxis",      [](const gp_Sphere& s) -> gp_Ax1 { return s.YAxis(); });
  mod.method("Direct",     [](const gp_Sphere& s) -> bool   { return bool(s.Direct()); });

  mod.add_type<gp_Torus>("gp_Torus").constructor<>()
     .constructor<const gp_Ax3&, double, double>();
  mod.method("SetAxis",        [](gp_Torus& t, const gp_Ax1& a) { t.SetAxis(a); });
  mod.method("SetLocation",    [](gp_Torus& t, const gp_Pnt& p) { t.SetLocation(p); });
  mod.method("SetPosition",    [](gp_Torus& t, const gp_Ax3& a) { t.SetPosition(a); });
  mod.method("SetMajorRadius", [](gp_Torus& t, double r)        { t.SetMajorRadius(r); });
  mod.method("SetMinorRadius", [](gp_Torus& t, double r)        { t.SetMinorRadius(r); });
  mod.method("Area",           [](const gp_Torus& t) -> double { return t.Area(); });
  mod.method("Volume",         [](const gp_Torus& t) -> double { return t.Volume(); });
  mod.method("Axis",           [](const gp_Torus& t) -> gp_Ax1 { return t.Axis(); });
  mod.method("Position",       [](const gp_Torus& t) -> gp_Ax3 { return t.Position(); });
  mod.method("Location",       [](const gp_Torus& t) -> gp_Pnt { return t.Location(); });
  mod.method("MajorRadius",    [](const gp_Torus& t) -> double { return t.MajorRadius(); });
  mod.method("MinorRadius",    [](const gp_Torus& t) -> double { return t.MinorRadius(); });
  mod.method("XAxis",          [](const gp_Torus& t) -> gp_Ax1 { return t.XAxis(); });
  mod.method("YAxis",          [](const gp_Torus& t) -> gp_Ax1 { return t.YAxis(); });
  mod.method("Direct",         [](const gp_Torus& t) -> bool   { return bool(t.Direct()); });

  mod.add_type<gp_Mat>("gp_Mat").constructor<>();
  mod.method("Value", [](const gp_Mat& m, int row, int col) { return m.Value(row, col); });

  mod.add_type<gp_GTrsf>("gp_GTrsf").constructor<>()
     .constructor<const gp_Trsf&>();
  mod.method("SetValue",           [](gp_GTrsf& t, int row, int col, double v) { t.SetValue(row, col, v); });
  mod.method("Value",              [](const gp_GTrsf& t, int row, int col) { return t.Value(row, col); });
  mod.method("SetVectorialPart",   [](gp_GTrsf& t, const gp_Mat& m) { t.SetVectorialPart(m); });
  mod.method("VectorialPart",      [](const gp_GTrsf& t) -> gp_Mat { return t.VectorialPart(); });
  mod.method("SetTranslationPart", [](gp_GTrsf& t, const gp_XYZ& v) { t.SetTranslationPart(v); });
  mod.method("TranslationPart",    [](const gp_GTrsf& t) -> gp_XYZ { return t.TranslationPart(); });
  mod.method("IsNegative",         [](const gp_GTrsf& t) -> bool { return bool(t.IsNegative()); });
}
