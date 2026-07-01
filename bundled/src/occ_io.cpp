// occ_io.cpp — 1:1 CxxWrap bindings for OCC I/O (BRepTools, STEP, IGES) +
// Netgen's OCCGeometry constructor from a TopoDS_Shape.
#include <string>

#include <jlcxx/jlcxx.hpp>

#include <TopoDS_Shape.hxx>

#include <BRep_Builder.hxx>
#include <BRepTools.hxx>

#include <STEPControl_Reader.hxx>
#include <STEPControl_Writer.hxx>
#include <IGESControl_Reader.hxx>
#include <IGESControl_Writer.hxx>

#include <occgeom.hpp>

void register_occ_io(jlcxx::Module& mod) {
  using namespace netgen;
  using GeoPtr = std::shared_ptr<NetgenGeometry>;

  // ===== BRepTools: BREP native format =======================================
  // BRepTools::Write/Read are free functions. Read needs a BRep_Builder;
  // no state is carried, so one is created internally per call.
  mod.method("BRepTools_Write", [](const TopoDS_Shape& s, const std::string& f) {
    return bool(BRepTools::Write(s, f.c_str()));
  });
  mod.method("BRepTools_Read", [](TopoDS_Shape& s, const std::string& f) {
    BRep_Builder b;
    return bool(BRepTools::Read(s, f.c_str(), b));
  });

  // ===== STEPControl ==========================================================
  mod.add_type<STEPControl_Reader>("STEPControl_Reader").constructor<>();
  mod.method("ReadFile", [](STEPControl_Reader& r, const std::string& f) {
    return int(r.ReadFile(f.c_str()));
  });
  mod.method("TransferRoots", [](STEPControl_Reader& r) { return r.TransferRoots(); });
  mod.method("NbShapes",      [](STEPControl_Reader& r) { return r.NbShapes(); });
  mod.method("OneShape",      [](STEPControl_Reader& r) -> TopoDS_Shape { return r.OneShape(); });

  mod.add_type<STEPControl_Writer>("STEPControl_Writer").constructor<>();
  mod.method("Transfer", [](STEPControl_Writer& w, const TopoDS_Shape& s, int mode) {
    return int(w.Transfer(s, STEPControl_StepModelType(mode)));
  });
  mod.method("Write", [](STEPControl_Writer& w, const std::string& f) {
    return int(w.Write(f.c_str()));
  });

  // ===== IGESControl ==========================================================
  mod.add_type<IGESControl_Reader>("IGESControl_Reader").constructor<>();
  mod.method("ReadFile", [](IGESControl_Reader& r, const std::string& f) {
    return int(r.ReadFile(f.c_str()));
  });
  mod.method("TransferRoots", [](IGESControl_Reader& r) { return r.TransferRoots(); });
  mod.method("OneShape",      [](IGESControl_Reader& r) -> TopoDS_Shape { return r.OneShape(); });

  mod.add_type<IGESControl_Writer>("IGESControl_Writer").constructor<>();
  mod.method("AddShape", [](IGESControl_Writer& w, const TopoDS_Shape& s) {
    return bool(w.AddShape(s));
  });
  mod.method("Write", [](IGESControl_Writer& w, const std::string& f) {
    return bool(w.Write(f.c_str()));
  });

  // ===== Netgen: OCCGeometry from a TopoDS_Shape =============================
  // OCCGeometry(shape) ctor runs BuildFMap → result is mesh-ready.
  // Returned as NetgenGeometry base (shared_ptr) to feed GenerateMesh etc.
  mod.method("OCCGeometry", [](const TopoDS_Shape& s) -> GeoPtr {
    return std::make_shared<OCCGeometry>(s);
  });
}
