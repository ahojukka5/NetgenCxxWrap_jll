// occ_shapefix2.cpp — 1:1 CxxWrap bindings for ShapeFix_FreeBounds, ShapeFix_ShapeTolerance,
// and ShapeFix_Wireframe.
#include <jlcxx/jlcxx.hpp>

#include <ShapeFix_FreeBounds.hxx>
#include <ShapeFix_ShapeTolerance.hxx>
#include <ShapeFix_Wireframe.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Compound.hxx>
#include <TopAbs_ShapeEnum.hxx>

void register_occ_shapefix2(jlcxx::Module& mod) {
  // ---- add_type calls first ----

  mod.add_type<ShapeFix_FreeBounds>("ShapeFix_FreeBounds")
     .constructor<>()
     .constructor<const TopoDS_Shape&, double, double, bool, bool>()
     .constructor<const TopoDS_Shape&, double, bool, bool>();

  mod.add_type<ShapeFix_ShapeTolerance>("ShapeFix_ShapeTolerance")
     .constructor<>();

  mod.add_type<ShapeFix_Wireframe>("ShapeFix_Wireframe")
     .constructor<>()
     .constructor<const TopoDS_Shape&>();

  // ---- ShapeFix_FreeBounds methods ----

  mod.method("GetClosedWires", [](const ShapeFix_FreeBounds& fb) -> TopoDS_Compound {
    return fb.GetClosedWires();
  });
  mod.method("GetOpenWires", [](const ShapeFix_FreeBounds& fb) -> TopoDS_Compound {
    return fb.GetOpenWires();
  });
  mod.method("GetShape", [](const ShapeFix_FreeBounds& fb) -> TopoDS_Shape {
    return fb.GetShape();
  });

  // ---- ShapeFix_ShapeTolerance methods ----

  mod.method("LimitTolerance", [](const ShapeFix_ShapeTolerance& st, const TopoDS_Shape& shape,
                                   double tmin, double tmax, int styp) -> bool {
    return bool(st.LimitTolerance(shape, tmin, tmax, TopAbs_ShapeEnum(styp)));
  });
  mod.method("SetTolerance", [](const ShapeFix_ShapeTolerance& st, const TopoDS_Shape& shape,
                                 double preci, int styp) {
    st.SetTolerance(shape, preci, TopAbs_ShapeEnum(styp));
  });

  // ---- ShapeFix_Wireframe methods ----

  mod.method("Load", [](ShapeFix_Wireframe& wf, const TopoDS_Shape& shape) {
    wf.Load(shape);
  });
  mod.method("FixWireGaps", [](ShapeFix_Wireframe& wf) -> bool {
    return bool(wf.FixWireGaps());
  });
  mod.method("FixSmallEdges", [](ShapeFix_Wireframe& wf) -> bool {
    return bool(wf.FixSmallEdges());
  });
  mod.method("Shape", [](ShapeFix_Wireframe& wf) -> TopoDS_Shape {
    return wf.Shape();
  });
  mod.method("SetLimitAngle", [](ShapeFix_Wireframe& wf, double angle) {
    wf.SetLimitAngle(angle);
  });
  mod.method("LimitAngle", [](const ShapeFix_Wireframe& wf) -> double {
    return wf.LimitAngle();
  });
}
