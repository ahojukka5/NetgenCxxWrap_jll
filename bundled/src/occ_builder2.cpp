// occ_builder2.cpp — 1:1 CxxWrap bindings for BRep_Builder (compound/shell construction).
#include <jlcxx/jlcxx.hpp>

#include <BRep_Builder.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Shell.hxx>

namespace jlcxx {
  template<> struct IsMirroredType<BRep_Builder> : std::false_type { };
}

void register_occ_builder2(jlcxx::Module& mod) {
  mod.add_type<BRep_Builder>("BRep_Builder").constructor<>();

  mod.method("MakeCompound", [](BRep_Builder& b, TopoDS_Compound& c) {
    b.MakeCompound(c);
  });
  mod.method("MakeShell", [](BRep_Builder& b, TopoDS_Shell& s) {
    b.MakeShell(s);
  });
  mod.method("Add", [](BRep_Builder& b, TopoDS_Shape& s, const TopoDS_Shape& c) {
    b.Add(s, c);
  });
  mod.method("Remove", [](BRep_Builder& b, TopoDS_Shape& s, const TopoDS_Shape& c) {
    b.Remove(s, c);
  });
}
