// occ_toptools_list.cpp — 1:1 CxxWrap bindings for TopTools_ListOfShape.
// Must be registered before any translation unit whose mod.method bindings
// take a TopTools_ListOfShape parameter (occ_builders.cpp, occ_thicksolid.cpp,
// occ_breplib.cpp) — see registration order in occ.cpp.
#include <jlcxx/jlcxx.hpp>

#include <TopTools_ListOfShape.hxx>
#include <TopoDS_Shape.hxx>

void register_occ_toptools_list(jlcxx::Module& mod) {
  mod.add_type<TopTools_ListOfShape>("TopTools_ListOfShape").constructor<>();

  mod.method("Append", [](TopTools_ListOfShape& l, const TopoDS_Shape& s) {
    l.Append(s);
  });
  mod.method("Extent", [](const TopTools_ListOfShape& l) -> int {
    return int(l.Extent());
  });
  mod.method("IsEmpty", [](const TopTools_ListOfShape& l) -> bool {
    return bool(l.IsEmpty());
  });
  mod.method("First", [](const TopTools_ListOfShape& l) -> TopoDS_Shape {
    return l.First();
  });
  mod.method("Last", [](const TopTools_ListOfShape& l) -> TopoDS_Shape {
    return l.Last();
  });
  mod.method("Clear", [](TopTools_ListOfShape& l) {
    l.Clear();
  });
}
