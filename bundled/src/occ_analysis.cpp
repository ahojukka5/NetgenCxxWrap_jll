// occ_analysis.cpp — 1:1 CxxWrap bindings for BRepCheck_Analyzer, ShapeFix_Shape,
// and BRep_Tool free functions.
#include "occ_handle_traits.hpp"
#include <jlcxx/jlcxx.hpp>

#include <BRepCheck_Analyzer.hxx>
#include <ShapeFix_Shape.hxx>
#include <BRep_Tool.hxx>
#include <Geom_Curve.hxx>
#include <Geom_Surface.hxx>

#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Vertex.hxx>

void register_occ_analysis(jlcxx::Module& mod)
{
    // ------------------------------------------------------------------ types
    mod.add_type<BRepCheck_Analyzer>("BRepCheck_Analyzer")
        .constructor<const TopoDS_Shape&>()
        .constructor<const TopoDS_Shape&, bool>();   // bool = GeomControls

    mod.add_type<ShapeFix_Shape>("ShapeFix_Shape")
        .constructor<>()
        .constructor<const TopoDS_Shape&>();

    // --------------------------------------------------------------- methods

    // BRepCheck_Analyzer
    mod.method("IsValid", [](const BRepCheck_Analyzer& a) {
        return bool(a.IsValid());
    });
    mod.method("IsValid", [](const BRepCheck_Analyzer& a, const TopoDS_Shape& shape) {
        return bool(a.IsValid(shape));
    });

    // ShapeFix_Shape
    mod.method("Init", [](ShapeFix_Shape& fixer, const TopoDS_Shape& shape) {
        fixer.Init(shape);
    });
    mod.method("Perform", [](ShapeFix_Shape& fixer) {
        return bool(fixer.Perform());
    });
    mod.method("Shape", [](ShapeFix_Shape& fixer) {
        return fixer.Shape();
    });
    mod.method("SetPrecision", [](ShapeFix_Shape& fixer, double prec) {
        fixer.SetPrecision(prec);
    });
    mod.method("SetMinTolerance", [](ShapeFix_Shape& fixer, double tol) {
        fixer.SetMinTolerance(tol);
    });
    mod.method("SetMaxTolerance", [](ShapeFix_Shape& fixer, double tol) {
        fixer.SetMaxTolerance(tol);
    });

    // BRep_Tool free functions
    mod.method("BRep_Tool_ToleranceEdge",   [](const TopoDS_Edge& e)   { return BRep_Tool::Tolerance(e); });
    mod.method("BRep_Tool_ToleranceFace",   [](const TopoDS_Face& f)   { return BRep_Tool::Tolerance(f); });
    mod.method("BRep_Tool_ToleranceVertex", [](const TopoDS_Vertex& v) { return BRep_Tool::Tolerance(v); });

    mod.method("BRep_Tool_IsClosed", [](const TopoDS_Shape& s) {
        return bool(BRep_Tool::IsClosed(s));
    });

    mod.method("BRep_Tool_Degenerated",   [](const TopoDS_Edge& e) { return bool(BRep_Tool::Degenerated(e)); });
    mod.method("BRep_Tool_SameParameter", [](const TopoDS_Edge& e) { return bool(BRep_Tool::SameParameter(e)); });

    mod.method("BRep_Tool_FirstParameter", [](const TopoDS_Edge& e) {
        double first, last;
        BRep_Tool::Range(e, first, last);
        return first;
    });
    mod.method("BRep_Tool_LastParameter", [](const TopoDS_Edge& e) {
        double first, last;
        BRep_Tool::Range(e, first, last);
        return last;
    });

    mod.method("BRep_Tool_Curve", [](const TopoDS_Edge& e) -> Handle(Geom_Curve) {
        double first, last;
        return BRep_Tool::Curve(e, first, last);
    });
    mod.method("BRep_Tool_Surface", [](const TopoDS_Face& f) -> Handle(Geom_Surface) {
        return BRep_Tool::Surface(f);
    });
}
