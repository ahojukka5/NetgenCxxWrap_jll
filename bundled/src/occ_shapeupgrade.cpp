// occ_shapeupgrade.cpp — 1:1 CxxWrap bindings for ShapeUpgrade_UnifySameDomain.
#include <jlcxx/jlcxx.hpp>

#include <ShapeUpgrade_UnifySameDomain.hxx>
#include <TopoDS_Shape.hxx>

namespace jlcxx {
  template<> struct IsMirroredType<ShapeUpgrade_UnifySameDomain> : std::false_type { };
}

void register_occ_shapeupgrade(jlcxx::Module& mod) {
  mod.add_type<ShapeUpgrade_UnifySameDomain>("ShapeUpgrade_UnifySameDomain")
     .constructor<>()
     .constructor<const TopoDS_Shape&, bool, bool, bool>();

  mod.method("Initialize", [](ShapeUpgrade_UnifySameDomain& u, const TopoDS_Shape& shape,
                               bool unifyEdges, bool unifyFaces, bool concatBSplines) {
    u.Initialize(shape, unifyEdges, unifyFaces, concatBSplines);
  });
  mod.method("AllowInternalEdges", [](ShapeUpgrade_UnifySameDomain& u, bool value) {
    u.AllowInternalEdges(value);
  });
  mod.method("KeepShape", [](ShapeUpgrade_UnifySameDomain& u, const TopoDS_Shape& shape) {
    u.KeepShape(shape);
  });
  mod.method("SetSafeInputMode", [](ShapeUpgrade_UnifySameDomain& u, bool value) {
    u.SetSafeInputMode(value);
  });
  mod.method("SetLinearTolerance", [](ShapeUpgrade_UnifySameDomain& u, double value) {
    u.SetLinearTolerance(value);
  });
  mod.method("SetAngularTolerance", [](ShapeUpgrade_UnifySameDomain& u, double value) {
    u.SetAngularTolerance(value);
  });
  mod.method("Build", [](ShapeUpgrade_UnifySameDomain& u) {
    u.Build();
  });
  mod.method("Shape", [](const ShapeUpgrade_UnifySameDomain& u) -> TopoDS_Shape {
    return u.Shape();
  });
}
