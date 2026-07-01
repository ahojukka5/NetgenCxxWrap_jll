// occ_draft.cpp — 1:1 CxxWrap bindings for BRepOffsetAPI_DraftAngle.
#include <jlcxx/jlcxx.hpp>

#include <BRepOffsetAPI_DraftAngle.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <gp_Dir.hxx>
#include <gp_Pln.hxx>

void register_occ_draft(jlcxx::Module& mod) {
  mod.add_type<BRepOffsetAPI_DraftAngle>("BRepOffsetAPI_DraftAngle")
     .constructor<>()
     .constructor<const TopoDS_Shape&>();

  mod.method("Init", [](BRepOffsetAPI_DraftAngle& m, const TopoDS_Shape& s) { m.Init(s); });
  mod.method("Clear", [](BRepOffsetAPI_DraftAngle& m) { m.Clear(); });
  mod.method("Add", [](BRepOffsetAPI_DraftAngle& m, const TopoDS_Face& f, const gp_Dir& dir,
                        double angle, const gp_Pln& neutralPlane, bool flag) {
    m.Add(f, dir, angle, neutralPlane, flag);
  });
  mod.method("AddDone", [](const BRepOffsetAPI_DraftAngle& m) -> bool { return bool(m.AddDone()); });
  mod.method("Build", [](BRepOffsetAPI_DraftAngle& m) { m.Build(); });
  mod.method("Shape", [](BRepOffsetAPI_DraftAngle& m) -> TopoDS_Shape { return m.Shape(); });
}
