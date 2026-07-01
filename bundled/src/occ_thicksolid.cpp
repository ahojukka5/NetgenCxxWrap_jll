// occ_thicksolid.cpp — 1:1 CxxWrap bindings for BRepOffsetAPI_MakeThickSolid.
#include <jlcxx/jlcxx.hpp>

#include <BRepOffsetAPI_MakeThickSolid.hxx>
#include <BRepOffset_Mode.hxx>
#include <GeomAbs_JoinType.hxx>
#include <TopoDS_Shape.hxx>
#include <TopTools_ListOfShape.hxx>

void register_occ_thicksolid(jlcxx::Module& mod) {
  mod.add_type<BRepOffsetAPI_MakeThickSolid>("BRepOffsetAPI_MakeThickSolid")
     .constructor<>();

  mod.method("MakeThickSolidBySimple", [](BRepOffsetAPI_MakeThickSolid& m,
                                           const TopoDS_Shape& s, double offsetValue) {
    m.MakeThickSolidBySimple(s, offsetValue);
  });
  mod.method("MakeThickSolidByJoin", [](BRepOffsetAPI_MakeThickSolid& m,
                                         const TopoDS_Shape& s,
                                         const TopTools_ListOfShape& closingFaces,
                                         double offset, double tol,
                                         int mode, bool intersection, bool selfInter,
                                         int join, bool removeIntEdges) {
    m.MakeThickSolidByJoin(s, closingFaces, offset, tol, BRepOffset_Mode(mode),
                            intersection, selfInter, GeomAbs_JoinType(join), removeIntEdges);
  });
  mod.method("Modified", [](BRepOffsetAPI_MakeThickSolid& m, const TopoDS_Shape& s) -> TopTools_ListOfShape {
    return m.Modified(s);
  });
  mod.method("Build", [](BRepOffsetAPI_MakeThickSolid& m) { m.Build(); });
  mod.method("Shape", [](BRepOffsetAPI_MakeThickSolid& m) -> TopoDS_Shape { return m.Shape(); });
  mod.method("IsDone", [](const BRepOffsetAPI_MakeThickSolid& m) -> bool { return bool(m.IsDone()); });

  mod.method("BRepOffset_Skin",       []() { return int(BRepOffset_Skin); });
  mod.method("BRepOffset_Pipe",       []() { return int(BRepOffset_Pipe); });
  mod.method("BRepOffset_RectoVerso", []() { return int(BRepOffset_RectoVerso); });
}
