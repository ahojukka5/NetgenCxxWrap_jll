// occ_shape_analysis.cpp — 1:1 CxxWrap bindings for free-boundary and shell analysis.
#include <jlcxx/jlcxx.hpp>

#include <ShapeAnalysis_FreeBounds.hxx>
#include <ShapeAnalysis_Shell.hxx>
#include <ShapeAnalysis_Edge.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Vertex.hxx>

void register_occ_shape_analysis(jlcxx::Module& mod) {
  // ---- add_type calls first ----

  mod.add_type<ShapeAnalysis_FreeBounds>("ShapeAnalysis_FreeBounds")
     .constructor<>()
     .constructor<const TopoDS_Shape&, double, bool, bool>()
     .constructor<const TopoDS_Shape&, bool, bool, bool>();

  mod.add_type<ShapeAnalysis_Shell>("ShapeAnalysis_Shell")
     .constructor<>();

  mod.add_type<ShapeAnalysis_Edge>("ShapeAnalysis_Edge")
     .constructor<>();

  // ---- ShapeAnalysis_FreeBounds methods ----

  mod.method("GetClosedWires", [](const ShapeAnalysis_FreeBounds& fb) -> TopoDS_Compound {
    return fb.GetClosedWires();
  });
  mod.method("GetOpenWires", [](const ShapeAnalysis_FreeBounds& fb) -> TopoDS_Compound {
    return fb.GetOpenWires();
  });

  // ---- ShapeAnalysis_Shell methods ----

  mod.method("LoadShells", [](ShapeAnalysis_Shell& sh, const TopoDS_Shape& shape) {
    sh.LoadShells(shape);
  });
  mod.method("CheckOrientedShells", [](ShapeAnalysis_Shell& sh, const TopoDS_Shape& shape,
                                       bool alsofree, bool checkinternaledges) -> bool {
    return bool(sh.CheckOrientedShells(shape, alsofree, checkinternaledges));
  });
  mod.method("IsLoaded",   [](ShapeAnalysis_Shell& sh, const TopoDS_Shape& shape) -> bool {
    return bool(sh.IsLoaded(shape));
  });
  mod.method("NbLoaded",   [](ShapeAnalysis_Shell& sh) -> int { return int(sh.NbLoaded()); });
  mod.method("HasBadEdges",[](ShapeAnalysis_Shell& sh) -> bool { return bool(sh.HasBadEdges()); });
  mod.method("BadEdges",   [](ShapeAnalysis_Shell& sh) -> TopoDS_Compound { return sh.BadEdges(); });
  mod.method("HasFreeEdges",[](ShapeAnalysis_Shell& sh) -> bool { return bool(sh.HasFreeEdges()); });
  mod.method("FreeEdges",  [](ShapeAnalysis_Shell& sh) -> TopoDS_Compound { return sh.FreeEdges(); });
  mod.method("HasConnectedEdges", [](ShapeAnalysis_Shell& sh) -> bool {
    return bool(sh.HasConnectedEdges());
  });
  mod.method("Clear", [](ShapeAnalysis_Shell& sh) { sh.Clear(); });

  // ---- ShapeAnalysis_Edge methods ----

  mod.method("HasCurve3d", [](const ShapeAnalysis_Edge& sae, const TopoDS_Edge& e) -> bool {
    return bool(sae.HasCurve3d(e));
  });
  mod.method("IsClosed3d", [](const ShapeAnalysis_Edge& sae, const TopoDS_Edge& e) -> bool {
    return bool(sae.IsClosed3d(e));
  });
  mod.method("HasPCurve", [](const ShapeAnalysis_Edge& sae, const TopoDS_Edge& e,
                              const TopoDS_Face& f) -> bool {
    return bool(sae.HasPCurve(e, f));
  });
  mod.method("IsSeam", [](const ShapeAnalysis_Edge& sae, const TopoDS_Edge& e,
                           const TopoDS_Face& f) -> bool {
    return bool(sae.IsSeam(e, f));
  });
  mod.method("FirstVertex", [](const ShapeAnalysis_Edge& sae, const TopoDS_Edge& e) -> TopoDS_Vertex {
    return sae.FirstVertex(e);
  });
  mod.method("LastVertex", [](const ShapeAnalysis_Edge& sae, const TopoDS_Edge& e) -> TopoDS_Vertex {
    return sae.LastVertex(e);
  });
}
