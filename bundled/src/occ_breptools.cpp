// occ_breptools.cpp — 1:1 CxxWrap bindings for BRepTools free functions.
#include <jlcxx/jlcxx.hpp>

#include <BRepTools.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Vertex.hxx>

void register_occ_breptools(jlcxx::Module& mod) {
  mod.method("BRepTools_OuterWire",
      [](const TopoDS_Face& f) -> TopoDS_Wire { return BRepTools::OuterWire(f); });
  mod.method("BRepTools_Compare",
      [](const TopoDS_Vertex& v1, const TopoDS_Vertex& v2) -> bool {
          return bool(BRepTools::Compare(v1, v2)); });
  mod.method("BRepTools_Compare",
      [](const TopoDS_Edge& e1, const TopoDS_Edge& e2) -> bool {
          return bool(BRepTools::Compare(e1, e2)); });
  mod.method("BRepTools_IsReallyClosed",
      [](const TopoDS_Edge& e, const TopoDS_Face& f) -> bool {
          return bool(BRepTools::IsReallyClosed(e, f)); });
  mod.method("BRepTools_Update",
      [](const TopoDS_Shape& s) { BRepTools::Update(s); });
  mod.method("BRepTools_CleanGeometry",
      [](const TopoDS_Shape& s) { BRepTools::CleanGeometry(s); });
  mod.method("BRepTools_RemoveUnusedPCurves",
      [](const TopoDS_Shape& s) { BRepTools::RemoveUnusedPCurves(s); });
  mod.method("BRepTools_UpdateFaceUVPoints",
      [](const TopoDS_Face& f) { BRepTools::UpdateFaceUVPoints(f); });
  mod.method("BRepTools_Clean",
      [](const TopoDS_Shape& s, bool relocate) { BRepTools::Clean(s, relocate); });
}
