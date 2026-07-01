// occ_topo2.cpp — 1:1 CxxWrap bindings for OCC bulk sub-shape enumeration.
#include <jlcxx/jlcxx.hpp>

#include <TopTools_IndexedMapOfShape.hxx>
#include <TopExp.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <TopAbs_State.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>

void register_occ_topo2(jlcxx::Module& mod) {
  mod.add_type<TopTools_IndexedMapOfShape>("TopTools_IndexedMapOfShape").constructor<>();

  mod.method("Extent",    [](const TopTools_IndexedMapOfShape& m) { return int(m.Extent()); });
  mod.method("Contains",  [](const TopTools_IndexedMapOfShape& m, const TopoDS_Shape& s) { return bool(m.Contains(s)); });
  mod.method("FindIndex", [](const TopTools_IndexedMapOfShape& m, const TopoDS_Shape& s) { return int(m.FindIndex(s)); });
  mod.method("FindKey",   [](const TopTools_IndexedMapOfShape& m, int i) -> TopoDS_Shape { return m.FindKey(i); });
  mod.method("Clear",     [](TopTools_IndexedMapOfShape& m) { m.Clear(); });

  mod.method("TopExp_MapShapes", [](const TopoDS_Shape& s, int toFind, TopTools_IndexedMapOfShape& map) {
    TopExp::MapShapes(s, TopAbs_ShapeEnum(toFind), map);
  });

  mod.method("TopAbs_COMPOUND",  []() { return int(TopAbs_COMPOUND); });
  mod.method("TopAbs_COMPSOLID", []() { return int(TopAbs_COMPSOLID); });
  mod.method("TopAbs_SOLID",     []() { return int(TopAbs_SOLID); });
  mod.method("TopAbs_SHELL",     []() { return int(TopAbs_SHELL); });
  mod.method("TopAbs_FACE",      []() { return int(TopAbs_FACE); });
  mod.method("TopAbs_WIRE",      []() { return int(TopAbs_WIRE); });
  mod.method("TopAbs_EDGE",      []() { return int(TopAbs_EDGE); });
  mod.method("TopAbs_VERTEX",    []() { return int(TopAbs_VERTEX); });
  mod.method("TopAbs_SHAPE",     []() { return int(TopAbs_SHAPE); });

  mod.method("TopAbs_IN",      []() { return int(TopAbs_IN); });
  mod.method("TopAbs_OUT",     []() { return int(TopAbs_OUT); });
  mod.method("TopAbs_ON",      []() { return int(TopAbs_ON); });
  mod.method("TopAbs_UNKNOWN", []() { return int(TopAbs_UNKNOWN); });

  mod.method("TopExp_FirstVertex", [](const TopoDS_Edge& e, bool orient) -> TopoDS_Vertex {
    return TopExp::FirstVertex(e, orient);
  });
  mod.method("TopExp_LastVertex", [](const TopoDS_Edge& e, bool orient) -> TopoDS_Vertex {
    return TopExp::LastVertex(e, orient);
  });
}
