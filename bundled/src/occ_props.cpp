// occ_props.cpp — 1:1 CxxWrap bindings for OCC mass/geometry properties,
// bounding-box utilities, and BRep_Tool::Pnt for vertex point extraction.
#include <jlcxx/jlcxx.hpp>

#include <GProp_GProps.hxx>
#include <GProp_PrincipalProps.hxx>
#include <BRepGProp.hxx>
#include <Bnd_Box.hxx>
#include <Bnd_OBB.hxx>
#include <BRepBndLib.hxx>
#include <BRep_Tool.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Vertex.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <gp_Ax1.hxx>
#include <gp_Mat.hxx>
#include <gp_XYZ.hxx>

namespace jlcxx {
  template<> struct IsMirroredType<GProp_PrincipalProps> : std::false_type { };
  template<> struct IsMirroredType<Bnd_OBB> : std::false_type { };
}

void register_occ_props(jlcxx::Module& mod) {
  // ---- add_type calls first ----

  mod.add_type<GProp_GProps>("GProp_GProps")
    .constructor<>();

  mod.add_type<GProp_PrincipalProps>("GProp_PrincipalProps")
    .constructor<>();

  mod.add_type<Bnd_Box>("Bnd_Box")
    .constructor<>();

  mod.add_type<Bnd_OBB>("Bnd_OBB")
    .constructor<>()
    .constructor<const gp_Pnt&, const gp_Dir&, const gp_Dir&, const gp_Dir&, double, double, double>();

  // ---- GProp_GProps methods ----

  mod.method("Mass",                [](const GProp_GProps& g) { return g.Mass(); });
  mod.method("CentreOfMass",        [](const GProp_GProps& g) { return g.CentreOfMass(); });
  mod.method("MatrixOfInertia",     [](const GProp_GProps& g) -> gp_Mat { return g.MatrixOfInertia(); });
  mod.method("PrincipalProperties", [](const GProp_GProps& g) -> GProp_PrincipalProps {
    return g.PrincipalProperties();
  });
  mod.method("MomentOfInertia", [](const GProp_GProps& g, const gp_Ax1& a) -> double {
    return g.MomentOfInertia(a);
  });

  // ---- GProp_PrincipalProps methods ----

  mod.method("HasSymmetryAxis",    [](const GProp_PrincipalProps& p) -> bool { return bool(p.HasSymmetryAxis()); });
  mod.method("HasSymmetryPoint",   [](const GProp_PrincipalProps& p) -> bool { return bool(p.HasSymmetryPoint()); });
  mod.method("FirstAxisOfInertia", [](const GProp_PrincipalProps& p) -> gp_Vec { return p.FirstAxisOfInertia(); });
  mod.method("SecondAxisOfInertia",[](const GProp_PrincipalProps& p) -> gp_Vec { return p.SecondAxisOfInertia(); });
  mod.method("ThirdAxisOfInertia", [](const GProp_PrincipalProps& p) -> gp_Vec { return p.ThirdAxisOfInertia(); });

  // ---- Bnd_Box methods ----

  mod.method("IsVoid",     [](const Bnd_Box& b) { return bool(b.IsVoid()); });
  mod.method("IsOpenXmin", [](const Bnd_Box& b) { return bool(b.IsOpenXmin()); });
  mod.method("IsOpenXmax", [](const Bnd_Box& b) { return bool(b.IsOpenXmax()); });
  mod.method("IsOpenYmin", [](const Bnd_Box& b) { return bool(b.IsOpenYmin()); });
  mod.method("IsOpenYmax", [](const Bnd_Box& b) { return bool(b.IsOpenYmax()); });
  mod.method("IsOpenZmin", [](const Bnd_Box& b) { return bool(b.IsOpenZmin()); });
  mod.method("IsOpenZmax", [](const Bnd_Box& b) { return bool(b.IsOpenZmax()); });

  mod.method("CornerMin_X", [](const Bnd_Box& b) {
    double xmin, ymin, zmin, xmax, ymax, zmax;
    b.Get(xmin, ymin, zmin, xmax, ymax, zmax);
    return xmin;
  });
  mod.method("CornerMin_Y", [](const Bnd_Box& b) {
    double xmin, ymin, zmin, xmax, ymax, zmax;
    b.Get(xmin, ymin, zmin, xmax, ymax, zmax);
    return ymin;
  });
  mod.method("CornerMin_Z", [](const Bnd_Box& b) {
    double xmin, ymin, zmin, xmax, ymax, zmax;
    b.Get(xmin, ymin, zmin, xmax, ymax, zmax);
    return zmin;
  });
  mod.method("CornerMax_X", [](const Bnd_Box& b) {
    double xmin, ymin, zmin, xmax, ymax, zmax;
    b.Get(xmin, ymin, zmin, xmax, ymax, zmax);
    return xmax;
  });
  mod.method("CornerMax_Y", [](const Bnd_Box& b) {
    double xmin, ymin, zmin, xmax, ymax, zmax;
    b.Get(xmin, ymin, zmin, xmax, ymax, zmax);
    return ymax;
  });
  mod.method("CornerMax_Z", [](const Bnd_Box& b) {
    double xmin, ymin, zmin, xmax, ymax, zmax;
    b.Get(xmin, ymin, zmin, xmax, ymax, zmax);
    return zmax;
  });

  mod.method("Add",     [](Bnd_Box& b, const gp_Pnt& p)  { b.Add(p); });
  mod.method("Enlarge", [](Bnd_Box& b, double tol)        { b.Enlarge(tol); });
  mod.method("IsOut",   [](const Bnd_Box& b, const gp_Pnt& p) { return bool(b.IsOut(p)); });

  // ---- Bnd_OBB methods ----

  mod.method("IsVoid",   [](const Bnd_OBB& b) -> bool { return bool(b.IsVoid()); });
  mod.method("IsAABox",  [](const Bnd_OBB& b) -> bool { return bool(b.IsAABox()); });
  mod.method("Center",   [](const Bnd_OBB& b) -> gp_XYZ { return b.Center(); });
  mod.method("XDirection", [](const Bnd_OBB& b) -> gp_XYZ { return b.XDirection(); });
  mod.method("YDirection", [](const Bnd_OBB& b) -> gp_XYZ { return b.YDirection(); });
  mod.method("ZDirection", [](const Bnd_OBB& b) -> gp_XYZ { return b.ZDirection(); });
  mod.method("XHSize", [](const Bnd_OBB& b) -> double { return b.XHSize(); });
  mod.method("YHSize", [](const Bnd_OBB& b) -> double { return b.YHSize(); });
  mod.method("ZHSize", [](const Bnd_OBB& b) -> double { return b.ZHSize(); });
  mod.method("SetCenter",     [](Bnd_OBB& b, const gp_Pnt& p)  { b.SetCenter(p); });
  mod.method("SetXComponent", [](Bnd_OBB& b, const gp_Dir& d, double hs) { b.SetXComponent(d, hs); });
  mod.method("SetYComponent", [](Bnd_OBB& b, const gp_Dir& d, double hs) { b.SetYComponent(d, hs); });
  mod.method("SetZComponent", [](Bnd_OBB& b, const gp_Dir& d, double hs) { b.SetZComponent(d, hs); });
  mod.method("Enlarge", [](Bnd_OBB& b, double gap)              { b.Enlarge(gap); });
  mod.method("Add",     [](Bnd_OBB& b, const gp_Pnt& p)         { b.Add(p); });
  mod.method("Add",     [](Bnd_OBB& b, const Bnd_OBB& other)    { b.Add(other); });
  mod.method("IsOut",   [](const Bnd_OBB& b, const Bnd_OBB& other) -> bool { return bool(b.IsOut(other)); });
  mod.method("IsOut",   [](const Bnd_OBB& b, const gp_Pnt& p)   -> bool { return bool(b.IsOut(p)); });
  mod.method("IsCompletelyInside", [](const Bnd_OBB& b, const Bnd_OBB& other) -> bool {
    return bool(b.IsCompletelyInside(other));
  });

  // ---- BRepGProp free functions ----

  mod.method("BRepGProp_LinearProperties",  [](const TopoDS_Shape& s, GProp_GProps& g) {
    BRepGProp::LinearProperties(s, g);
  });
  mod.method("BRepGProp_SurfaceProperties", [](const TopoDS_Shape& s, GProp_GProps& g) {
    BRepGProp::SurfaceProperties(s, g);
  });
  mod.method("BRepGProp_VolumeProperties",  [](const TopoDS_Shape& s, GProp_GProps& g) {
    BRepGProp::VolumeProperties(s, g);
  });

  // ---- BRepBndLib free function ----

  mod.method("BRepBndLib_Add", [](const TopoDS_Shape& s, Bnd_Box& b) {
    BRepBndLib::Add(s, b);
  });
  mod.method("BRepBndLib_AddOBB", [](const TopoDS_Shape& s, Bnd_OBB& b,
                                     bool useTriangulation, bool optimal, bool useTolerance) {
    BRepBndLib::AddOBB(s, b, useTriangulation, optimal, useTolerance);
  });

  // ---- BRep_Tool::Pnt (vertex point extraction) ----

  mod.method("BRep_Tool_Pnt", [](const TopoDS_Vertex& v) {
    return BRep_Tool::Pnt(v);
  });
}
