// occ_wire.cpp — 1:1 CxxWrap bindings for BRepTools_WireExplorer and BRepLProp_SLProps.
#include <jlcxx/jlcxx.hpp>
#include <BRepTools_WireExplorer.hxx>
#include <BRepLProp_SLProps.hxx>
#include <BRepLProp_CLProps.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <gp_Dir.hxx>

void register_occ_wire(jlcxx::Module& mod) {
  // ---- add_type calls first ----

  mod.add_type<BRepLProp_CLProps>("BRepLProp_CLProps")
     .constructor<const BRepAdaptor_Curve&, int, double>()
     .constructor<const BRepAdaptor_Curve&, double, int, double>()
     .constructor<int, double>();

  mod.add_type<BRepTools_WireExplorer>("BRepTools_WireExplorer")
     .constructor<>()
     .constructor<const TopoDS_Wire&>()
     .constructor<const TopoDS_Wire&, const TopoDS_Face&>();

  mod.add_type<BRepLProp_SLProps>("BRepLProp_SLProps")
     .constructor<const BRepAdaptor_Surface&, double, double, int, double>()
     .constructor<const BRepAdaptor_Surface&, int, double>()
     .constructor<int, double>();

  // ---- BRepTools_WireExplorer methods ----

  mod.method("Init", [](BRepTools_WireExplorer& e, const TopoDS_Wire& wire) {
    e.Init(wire);
  });
  mod.method("Init", [](BRepTools_WireExplorer& e, const TopoDS_Wire& wire, const TopoDS_Face& face) {
    e.Init(wire, face);
  });
  mod.method("More", [](const BRepTools_WireExplorer& e) -> bool {
    return bool(e.More());
  });
  mod.method("Next", [](BRepTools_WireExplorer& e) {
    e.Next();
  });
  mod.method("Current", [](const BRepTools_WireExplorer& e) -> TopoDS_Edge {
    return e.Current();
  });
  mod.method("CurrentVertex", [](const BRepTools_WireExplorer& e) -> TopoDS_Vertex {
    return e.CurrentVertex();
  });

  // ---- BRepLProp_SLProps methods ----

  mod.method("SetParameters", [](BRepLProp_SLProps& props, double u, double v) {
    props.SetParameters(u, v);
  });
  mod.method("IsNormalDefined", [](BRepLProp_SLProps& props) -> bool {
    return bool(props.IsNormalDefined());
  });
  mod.method("Normal", [](BRepLProp_SLProps& props) -> gp_Dir {
    return props.Normal();
  });
  mod.method("IsCurvatureDefined", [](BRepLProp_SLProps& props) -> bool {
    return bool(props.IsCurvatureDefined());
  });
  mod.method("MinCurvature", [](BRepLProp_SLProps& props) -> double {
    return props.MinCurvature();
  });
  mod.method("MaxCurvature", [](BRepLProp_SLProps& props) -> double {
    return props.MaxCurvature();
  });
  mod.method("MeanCurvature", [](BRepLProp_SLProps& props) -> double {
    return props.MeanCurvature();
  });
  mod.method("GaussianCurvature", [](BRepLProp_SLProps& props) -> double {
    return props.GaussianCurvature();
  });

  // ---- BRepLProp_CLProps methods ----

  mod.method("SetParameter", [](BRepLProp_CLProps& p, double u) {
    p.SetParameter(u);
  });
  mod.method("SetCurve", [](BRepLProp_CLProps& p, const BRepAdaptor_Curve& c) {
    p.SetCurve(c);
  });
  mod.method("Value", [](BRepLProp_CLProps& p) -> gp_Pnt {
    return p.Value();
  });
  mod.method("D1", [](BRepLProp_CLProps& p) -> gp_Vec { return p.D1(); });
  mod.method("D2", [](BRepLProp_CLProps& p) -> gp_Vec { return p.D2(); });
  mod.method("IsTangentDefined", [](BRepLProp_CLProps& p) -> bool {
    return bool(p.IsTangentDefined());
  });
  mod.method("Tangent", [](BRepLProp_CLProps& p, gp_Dir& t) { p.Tangent(t); });
  mod.method("Normal", [](BRepLProp_CLProps& p, gp_Dir& n) { p.Normal(n); });
  mod.method("CentreOfCurvature", [](BRepLProp_CLProps& p, gp_Pnt& c) { p.CentreOfCurvature(c); });
  mod.method("Curvature", [](BRepLProp_CLProps& p) -> double { return p.Curvature(); });
}
