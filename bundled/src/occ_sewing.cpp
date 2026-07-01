#include <jlcxx/jlcxx.hpp>

#include <BRepBuilderAPI_Sewing.hxx>
#include <BRepClass3d_SolidClassifier.hxx>
#include <BRepClass_FaceClassifier.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <gp_Pnt.hxx>
#include <gp_Pnt2d.hxx>

void register_occ_sewing(jlcxx::Module& mod)
{
    mod.add_type<BRepClass_FaceClassifier>("BRepClass_FaceClassifier")
        .constructor<>();

    mod.add_type<BRepBuilderAPI_Sewing>("BRepBuilderAPI_Sewing")
        .constructor<>()
        .constructor<double>();

    mod.add_type<BRepClass3d_SolidClassifier>("BRepClass3d_SolidClassifier")
        .constructor<>()
        .constructor<const TopoDS_Shape&>();

    mod.method("Add", [](BRepBuilderAPI_Sewing& sewer, const TopoDS_Shape& shape) {
        sewer.Add(shape);
    });
    mod.method("Perform", [](BRepBuilderAPI_Sewing& sewer) {
        sewer.Perform();
    });
    mod.method("SewedShape", [](BRepBuilderAPI_Sewing& sewer) -> TopoDS_Shape {
        return sewer.SewedShape();
    });
    mod.method("SetTolerance", [](BRepBuilderAPI_Sewing& sewer, double tol) {
        sewer.SetTolerance(tol);
    });
    mod.method("Tolerance", [](BRepBuilderAPI_Sewing& sewer) -> double {
        return sewer.Tolerance();
    });
    mod.method("NbFreeEdges", [](BRepBuilderAPI_Sewing& sewer) -> int {
        return int(sewer.NbFreeEdges());
    });
    mod.method("NbContigousEdges", [](BRepBuilderAPI_Sewing& sewer) -> int {
        return int(sewer.NbContigousEdges());
    });
    mod.method("NbMultipleEdges", [](BRepBuilderAPI_Sewing& sewer) -> int {
        return int(sewer.NbMultipleEdges());
    });

    mod.method("Load", [](BRepClass3d_SolidClassifier& clf, const TopoDS_Shape& shape) {
        clf.Load(shape);
    });
    mod.method("Perform", [](BRepClass3d_SolidClassifier& clf, const gp_Pnt& pnt, double tol) {
        clf.Perform(pnt, tol);
    });
    mod.method("PerformInfinitePoint", [](BRepClass3d_SolidClassifier& clf, double tol) {
        clf.PerformInfinitePoint(tol);
    });
    mod.method("State", [](BRepClass3d_SolidClassifier& clf) -> int {
        return int(clf.State());
    });
    mod.method("IsOnAFace", [](BRepClass3d_SolidClassifier& clf) -> bool {
        return bool(clf.IsOnAFace());
    });

    // ---- BRepClass_FaceClassifier methods ----

    mod.method("Perform", [](BRepClass_FaceClassifier& fc, const TopoDS_Face& f,
                              const gp_Pnt2d& p, double tol) {
        fc.Perform(f, p, tol);
    });
    mod.method("Perform", [](BRepClass_FaceClassifier& fc, const TopoDS_Face& f,
                              const gp_Pnt& p, double tol) {
        fc.Perform(f, p, tol);
    });
    mod.method("State", [](BRepClass_FaceClassifier& fc) -> int {
        return int(fc.State());
    });
}
