#include <memory>
#include <string>
#include <filesystem>
#include <vector>
#include <jlcxx/jlcxx.hpp>
#include <jlcxx/array.hpp>
#include <meshing.hpp>
#include <meshing/meshfunc.hpp>
#include <gprim/geom3d.hpp>

using namespace netgen;
using MeshPtr = std::shared_ptr<Mesh>;

void register_mesh2(jlcxx::Module& mod) {
  // --- EdgeDescriptor type ---
  mod.add_type<EdgeDescriptor>("EdgeDescriptor")
     .constructor<>()
     .constructor<int, int, int>();  // edgenr, surfnr1, surfnr2

  mod.method("EdgeNr",          [](const EdgeDescriptor& ed) { return ed.EdgeNr(); });
  mod.method("SetEdgeNr",       [](EdgeDescriptor& ed, int nr) { ed.SetEdgeNr(nr); });
  mod.method("SurfNr",          [](const EdgeDescriptor& ed, int i) { return ed.SurfNr(i); });
  mod.method("SetSurfNr",       [](EdgeDescriptor& ed, int i, int nr) { ed.SetSurfNr(i, nr); });
  mod.method("GetName",         [](const EdgeDescriptor& ed) -> std::string { return std::string(ed.GetName()); });
  mod.method("SetName",         [](EdgeDescriptor& ed, const std::string& s) { ed.SetName(s); });
  mod.method("SingEdgeLeft",    [](const EdgeDescriptor& ed) { return ed.SingEdgeLeft(); });
  mod.method("SetSingEdgeLeft", [](EdgeDescriptor& ed, double s) { ed.SetSingEdgeLeft(s); });
  mod.method("SingEdgeRight",   [](const EdgeDescriptor& ed) { return ed.SingEdgeRight(); });
  mod.method("SetSingEdgeRight",[](EdgeDescriptor& ed, double s) { ed.SetSingEdgeRight(s); });

  // --- Mesh: bounding box (Box3d is already registered in register_gprim) ---
  mod.method("GetBox", [](const MeshPtr& m) {
    Point3d pmin, pmax;
    m->GetBox(pmin, pmax);
    return Box3d(pmin, pmax);
  });

  // --- Mesh: h-field queries ---
  mod.method("GetH", [](const MeshPtr& m, const Point3d& p) { return m->GetH(p); });
  mod.method("GetHPointIndex", [](const MeshPtr& m, int pi) {
    return m->GetH(PointIndex(pi));
  });

  // --- Mesh: global size control ---
  mod.method("SetGlobalH",  [](const MeshPtr& m, double h) { m->SetGlobalH(h); });
  mod.method("SetMinimalH", [](const MeshPtr& m, double h) { m->SetMinimalH(h); });

  // SetLocalH with bounding box (Point3d implicitly converts to Point<3>)
  mod.method("SetLocalH", [](const MeshPtr& m, const Point3d& pmin, const Point3d& pmax, double grading) {
    m->SetLocalH(Point<3>(pmin), Point<3>(pmax), grading);
  });
  // SetLocalH with a pre-built LocalH object (LocalH registered in register_extras)
  mod.method("SetLocalH", [](const MeshPtr& m, const std::shared_ptr<LocalH>& lh) {
    m->SetLocalH(lh);
  });

  // --- Mesh: curvature/distance-based local sizing ---
  mod.method("CalcLocalHFromSurfaceCurvature", [](const MeshPtr& m, double grading, double elperr) {
    m->CalcLocalHFromSurfaceCurvature(grading, elperr);
  });
  mod.method("CalcLocalHFromPointDistances", [](const MeshPtr& m, double grading) {
    m->CalcLocalHFromPointDistances(grading);
  });

  // --- Mesh: quality analysis ---
  mod.method("CheckOverlappingBoundary", [](const MeshPtr& m) { return m->CheckOverlappingBoundary(); });
  mod.method("AverageH",     [](const MeshPtr& m, int surfnr) { return m->AverageH(surfnr); });
  mod.method("CalcTotalBad", [](const MeshPtr& m, const MeshingParameters& mp) {
    return m->CalcTotalBad(mp);
  });
  // CalcMinMaxAngle: void wrapper — function side-effects mesh quality reporting
  mod.method("CalcMinMaxAngle", [](const MeshPtr& m, double badellimit) {
    m->CalcMinMaxAngle(badellimit, nullptr);
  });
  mod.method("MarkIllegalElements", [](const MeshPtr& m, int domain) {
    return m->MarkIllegalElements(domain);
  });

  // --- Mesh: topology splitting ---
  mod.method("Split2Tets",               [](const MeshPtr& m) { m->Split2Tets(); });
  mod.method("SplitIntoParts",           [](const MeshPtr& m) { m->SplitIntoParts(); });
  mod.method("SplitSeparatedFaces",      [](const MeshPtr& m) { m->SplitSeparatedFaces(); });
  mod.method("SurfaceMeshOrientation",   [](const MeshPtr& m) { m->SurfaceMeshOrientation(); });
  mod.method("BuildElementSearchTree",   [](const MeshPtr& m, int dim) { m->BuildElementSearchTree(dim); });
  mod.method("SplitFacesByAdjacentDomains", [](const MeshPtr& m) { m->SplitFacesByAdjacentDomains(); });
  mod.method("PureTrigMesh", [](const MeshPtr& m, int faceindex) { return m->PureTrigMesh(faceindex); });
  mod.method("PureTetMesh",  [](const MeshPtr& m) { return m->PureTetMesh(); });

  // --- Mesh: mutation ---
  mod.method("SetDimension",         [](const MeshPtr& m, int dim) { m->SetDimension(dim); });
  mod.method("ClearVolumeElements",  [](const MeshPtr& m) { m->ClearVolumeElements(); });
  mod.method("ClearSegments",        [](const MeshPtr& m) { m->ClearSegments(); });
  mod.method("DeleteMesh",           [](const MeshPtr& m) { m->DeleteMesh(); });

  // --- Mesh: element replacement ---
  mod.method("SetSurfaceElement", [](const MeshPtr& m, int sei, const Element2d& el) {
    m->SetSurfaceElement(SurfaceElementIndex(sei), el);
  });
  mod.method("SetVolumeElement", [](const MeshPtr& m, int sei, const Element& el) {
    m->SetVolumeElement(ElementIndex(sei), el);
  });

  // --- Mesh: face element lookup (buffer-fill, returns count) ---
  mod.method("GetSurfaceElementsOfFace", [](const MeshPtr& m, int facenr, jlcxx::ArrayRef<int> buf) {
    Array<SurfaceElementIndex> sei;
    m->GetSurfaceElementsOfFace(facenr, sei);
    int n = int(sei.Size());
    for (int i = 0; i < n && i < int(buf.size()); ++i) buf[i] = int(sei[i]);
    return n;
  });

  // --- Mesh: boundary tracking ---
  mod.method("AddLockedPoint",               [](const MeshPtr& m, int pi) { m->AddLockedPoint(PointIndex(pi)); });
  mod.method("FindOpenElements",             [](const MeshPtr& m, int dom) { m->FindOpenElements(dom); });
  mod.method("FindOpenSegments",             [](const MeshPtr& m, int surfnr) { m->FindOpenSegments(surfnr); });
  mod.method("RemoveOneLayerSurfaceElements",[](const MeshPtr& m) { m->RemoveOneLayerSurfaceElements(); });

  // --- Mesh: h-restriction and file I/O ---
  mod.method("RestrictLocalHLine", [](const MeshPtr& m, const Point3d& p1, const Point3d& p2,
                                      double hloc, int layer) {
    m->RestrictLocalHLine(p1, p2, hloc, layer);
  });
  mod.method("LoadLocalMeshSize", [](const MeshPtr& m, const std::string& path) {
    m->LoadLocalMeshSize(std::filesystem::path(path));
  });
  mod.method("Merge", [](const MeshPtr& m, const std::string& path) {
    m->Merge(std::filesystem::path(path));
  });
  mod.method("ElementError", [](const MeshPtr& m, int eli, const MeshingParameters& mp) {
    return m->ElementError(eli, mp);
  });
  mod.method("GetSubMesh", [](const MeshPtr& m, const std::string& domains, const std::string& faces) {
    return m->GetSubMesh(domains, faces);
  });

  // --- codimension / per-element region names (Mesh API) ---
  mod.method("GetCD2Name", [](const MeshPtr& m, int cd2nr) -> std::string {
    return std::string(m->GetCD2Name(cd2nr));
  });
  mod.method("GetCD3Name", [](const MeshPtr& m, int cd3nr) -> std::string {
    return std::string(m->GetCD3Name(cd3nr));
  });
  mod.method("GetRegionNameVolume", [](const MeshPtr& m, int enr) -> std::string {
    return std::string(m->GetRegionName(ElementIndex(enr)));
  });
  mod.method("GetRegionNameSurface", [](const MeshPtr& m, int senr) -> std::string {
    return std::string(m->GetRegionName(SurfaceElementIndex(senr)));
  });
  mod.method("GetRegionNameSegment", [](const MeshPtr& m, int segnr) -> std::string {
    return std::string(m->GetRegionName(SegmentIndex(segnr)));
  });

  // --- MeshTopology: GetVerticesEdge (find edge from two vertex indices; different from GetEdgeVertices) ---
  mod.method("GetVerticesEdge", [](const MeshTopology& t, int v1, int v2) -> int {
    return int(t.GetVerticesEdge(PointIndex(v1), PointIndex(v2)));
  });

  // EnableTable via Mesh (GetTopology() returns const ref, need const_cast)
  mod.method("EnableTopologyTable", [](const MeshPtr& m, const std::string& name, bool set) {
    const_cast<MeshTopology&>(m->GetTopology()).EnableTable(name, set);
  });

  // --- LocalH: Copy (unique_ptr -> shared_ptr) and Delete ---
  mod.method("Copy", [](const std::shared_ptr<LocalH>& lh) -> std::shared_ptr<LocalH> {
    return std::shared_ptr<LocalH>(lh->Copy().release());
  });
  mod.method("Delete", [](const std::shared_ptr<LocalH>& lh) { lh->Delete(); });

}
