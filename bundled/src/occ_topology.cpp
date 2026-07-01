// occ_topology.cpp — 1:1 CxxWrap bindings for OCC topology types.
#include <jlcxx/jlcxx.hpp>

#include <TopAbs_ShapeEnum.hxx>
#include <TopAbs_Orientation.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shell.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_CompSolid.hxx>
#include <TopoDS_Iterator.hxx>
#include <TopExp_Explorer.hxx>

// CxxWrap needs these before add_type<TopoDS_Vertex>(... julia_base_type<TopoDS_Shape>())
namespace jlcxx {
  template<> struct SuperType<TopoDS_Vertex>    { typedef TopoDS_Shape type; };
  template<> struct SuperType<TopoDS_Edge>      { typedef TopoDS_Shape type; };
  template<> struct SuperType<TopoDS_Wire>      { typedef TopoDS_Shape type; };
  template<> struct SuperType<TopoDS_Face>      { typedef TopoDS_Shape type; };
  template<> struct SuperType<TopoDS_Shell>     { typedef TopoDS_Shape type; };
  template<> struct SuperType<TopoDS_Solid>     { typedef TopoDS_Shape type; };
  template<> struct SuperType<TopoDS_Compound>  { typedef TopoDS_Shape type; };
  template<> struct SuperType<TopoDS_CompSolid> { typedef TopoDS_Shape type; };
}

void register_occ_topology(jlcxx::Module& mod) {
  mod.add_type<TopoDS_Shape>("TopoDS_Shape").constructor<>();
  mod.method("IsNull",      [](const TopoDS_Shape& s) { return bool(s.IsNull()); });
  mod.method("ShapeType",   [](const TopoDS_Shape& s) { return int(s.ShapeType()); });
  mod.method("Orientation", [](const TopoDS_Shape& s) { return int(s.Orientation()); });
  mod.method("IsSame",      [](const TopoDS_Shape& a, const TopoDS_Shape& b) { return bool(a.IsSame(b)); });
  mod.method("IsEqual",     [](const TopoDS_Shape& a, const TopoDS_Shape& b) { return bool(a.IsEqual(b)); });
  mod.method("NbChildren",  [](const TopoDS_Shape& s) { return int(s.NbChildren()); });
  mod.method("Reversed",    [](const TopoDS_Shape& s) { return s.Reversed(); });
  mod.method("Nullify",     [](TopoDS_Shape& s) { s.Nullify(); });

  mod.add_type<TopoDS_Vertex>  ("TopoDS_Vertex",   jlcxx::julia_base_type<TopoDS_Shape>()).constructor<>();
  mod.add_type<TopoDS_Edge>    ("TopoDS_Edge",      jlcxx::julia_base_type<TopoDS_Shape>()).constructor<>();
  mod.add_type<TopoDS_Wire>    ("TopoDS_Wire",      jlcxx::julia_base_type<TopoDS_Shape>()).constructor<>();
  mod.add_type<TopoDS_Face>    ("TopoDS_Face",      jlcxx::julia_base_type<TopoDS_Shape>()).constructor<>();
  mod.add_type<TopoDS_Shell>   ("TopoDS_Shell",     jlcxx::julia_base_type<TopoDS_Shape>()).constructor<>();
  mod.add_type<TopoDS_Solid>   ("TopoDS_Solid",     jlcxx::julia_base_type<TopoDS_Shape>()).constructor<>();
  mod.add_type<TopoDS_Compound>("TopoDS_Compound",  jlcxx::julia_base_type<TopoDS_Shape>()).constructor<>();
  mod.add_type<TopoDS_CompSolid>("TopoDS_CompSolid",jlcxx::julia_base_type<TopoDS_Shape>()).constructor<>();

  // TopoDS:: downcasts (free functions in the OCCT namespace)
  mod.method("TopoDS_Vertex",    [](const TopoDS_Shape& s) { return TopoDS::Vertex(s); });
  mod.method("TopoDS_Edge",      [](const TopoDS_Shape& s) { return TopoDS::Edge(s); });
  mod.method("TopoDS_Wire",      [](const TopoDS_Shape& s) { return TopoDS::Wire(s); });
  mod.method("TopoDS_Face",      [](const TopoDS_Shape& s) { return TopoDS::Face(s); });
  mod.method("TopoDS_Shell",     [](const TopoDS_Shape& s) { return TopoDS::Shell(s); });
  mod.method("TopoDS_Solid",     [](const TopoDS_Shape& s) { return TopoDS::Solid(s); });
  mod.method("TopoDS_Compound",  [](const TopoDS_Shape& s) { return TopoDS::Compound(s); });

  // TopAbs_ShapeEnum is passed as int from Julia; Init casts it back.
  mod.add_type<TopExp_Explorer>("TopExp_Explorer").constructor<>();
  mod.method("Init", [](TopExp_Explorer& e, const TopoDS_Shape& s, int toFind) {
    e.Init(s, TopAbs_ShapeEnum(toFind));
  });
  mod.method("More",    [](const TopExp_Explorer& e) { return bool(e.More()); });
  mod.method("Next",    [](TopExp_Explorer& e) { e.Next(); });
  mod.method("Current", [](const TopExp_Explorer& e) -> TopoDS_Shape { return e.Current(); });
  mod.method("Value",   [](const TopExp_Explorer& e) -> TopoDS_Shape { return e.Value(); });

  mod.add_type<TopoDS_Iterator>("TopoDS_Iterator").constructor<>()
     .constructor<const TopoDS_Shape&>();
  mod.method("More",  [](const TopoDS_Iterator& e) { return bool(e.More()); });
  mod.method("Next",  [](TopoDS_Iterator& e) { e.Next(); });
  mod.method("Value", [](const TopoDS_Iterator& e) -> TopoDS_Shape { return e.Value(); });
}
