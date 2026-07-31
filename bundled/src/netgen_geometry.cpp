#include <memory>
#include <string>
#include <jlcxx/jlcxx.hpp>
#include <jlcxx/array.hpp>
#include <meshing.hpp>
#include <occgeom.hpp>
#include <nginterface_v2.hpp>
using namespace netgen;
using MeshPtr = std::shared_ptr<Mesh>;
using GeoPtr = std::shared_ptr<NetgenGeometry>;

void register_geometry(jlcxx::Module& mod) {
  // Types
  mod.add_type<MeshingParameters>("MeshingParameters").constructor<>();
  mod.add_type<BisectionOptions>("BisectionOptions").constructor<>();
  mod.add_type<NetgenGeometry>("NetgenGeometry");
  mod.add_type<Refinement>("Refinement");
  mod.add_type<Mesh>("Mesh");
  mod.add_type<Ngx_Mesh>("Ngx_Mesh").constructor<std::shared_ptr<Mesh>>();

  // MeshingParameters fields
  mod.method("maxh", [](const MeshingParameters& mp) { return mp.maxh; });
  mod.method("maxh!", [](MeshingParameters& mp, double v) { mp.maxh = v; });
  mod.method("minh", [](const MeshingParameters& mp) { return mp.minh; });
  mod.method("minh!", [](MeshingParameters& mp, double v) { mp.minh = v; });
  mod.method("grading", [](const MeshingParameters& mp) { return mp.grading; });
  mod.method("grading!", [](MeshingParameters& mp, double v) { mp.grading = v; });
  mod.method("optsteps2d", [](const MeshingParameters& mp) { return mp.optsteps2d; });
  mod.method("optsteps2d!", [](MeshingParameters& mp, int v) { mp.optsteps2d = v; });
  mod.method("optsteps3d", [](const MeshingParameters& mp) { return mp.optsteps3d; });
  mod.method("optsteps3d!", [](MeshingParameters& mp, int v) { mp.optsteps3d = v; });
  mod.method("secondorder", [](const MeshingParameters& mp) { return mp.secondorder; });
  mod.method("secondorder!", [](MeshingParameters& mp, bool v) { mp.secondorder = v; });

  // BisectionOptions fields
  mod.method("maxlevel", [](const BisectionOptions& o) { return o.maxlevel; });
  mod.method("maxlevel!", [](BisectionOptions& o, int v) { o.maxlevel = v; });
  mod.method("usemarkedelements", [](const BisectionOptions& o) { return o.usemarkedelements; });
  mod.method("usemarkedelements!", [](BisectionOptions& o, int v) { o.usemarkedelements = v; });
  mod.method("refine_hp", [](const BisectionOptions& o) { return o.refine_hp; });
  mod.method("refine_hp!", [](BisectionOptions& o, bool v) { o.refine_hp = v; });
  mod.method("refine_p", [](const BisectionOptions& o) { return o.refine_p; });
  mod.method("refine_p!", [](BisectionOptions& o, bool v) { o.refine_p = v; });
  mod.method("onlyonce", [](const BisectionOptions& o) { return o.onlyonce; });
  mod.method("onlyonce!", [](BisectionOptions& o, bool v) { o.onlyonce = v; });

  // NetgenGeometry / Refinement
  mod.method("GenerateMesh", [](const GeoPtr& g, const MeshPtr& m, MeshingParameters& mp) {
    return g->GenerateMesh(const_cast<MeshPtr&>(m), mp);
  });
  mod.method("GetRefinement", [](const GeoPtr& g) -> const Refinement& { return g->GetRefinement(); });
  mod.method("Refine", [](const Refinement& r, const MeshPtr& m) { r.Refine(*m); });
  mod.method("Bisect", [](const Refinement& r, const MeshPtr& m, BisectionOptions& o) { r.Bisect(*m, o); });
  mod.method("MakeSecondOrder", [](const Refinement& r, const MeshPtr& m) { r.MakeSecondOrder(*m); });

  // Mesh
  mod.method("new_mesh", []() { return std::make_shared<Mesh>(); });
  mod.method("assign", [](const MeshPtr& dst, const MeshPtr& src) {
    *dst = *src;
    // Mesh::operator= does not carry over periodic/close-surface
    // identifications: Identifications holds a reference back to its owning
    // Mesh, so dst keeps the empty Identifications it was constructed with.
    // Re-register src's entries against dst's own Identifications by their
    // public point-pair API instead of copy-assigning the object itself.
    auto& src_ident = src->GetIdentifications();
    auto& dst_ident = dst->GetIdentifications();
    int max_nr = src_ident.GetMaxNr();
    for (int nr = 1; nr <= max_nr; ++nr) {
      NgArray<INDEX_2> pairs;
      src_ident.GetPairs(nr, pairs);
      for (int i = 0; i < pairs.Size(); ++i) {
        dst_ident.Add(pairs[i].I1(), pairs[i].I2(), nr);
      }
      dst_ident.SetType(nr, src_ident.GetType(nr));
      dst_ident.SetName(nr, src_ident.GetName(nr));
    }
  });
  mod.method("GetNP", [](const MeshPtr& m) { return m->GetNP(); });
  mod.method("GetNV", [](const MeshPtr& m) { return int(m->GetNV()); });
  mod.method("GetNE", [](const MeshPtr& m) { return m->GetNE(); });
  mod.method("GetNSE", [](const MeshPtr& m) { return m->GetNSE(); });
  mod.method("GetNSeg", [](const MeshPtr& m) { return m->GetNSeg(); });
  mod.method("GetDimension", [](const MeshPtr& m) { return m->GetDimension(); });
  mod.method("GetNDomains", [](const MeshPtr& m) { return m->GetNDomains(); });
  mod.method("GetNFD", [](const MeshPtr& m) { return m->GetNFD(); });
  mod.method("UpdateTopology", [](const MeshPtr& m) { m->UpdateTopology(); });
  mod.method("GetTopology", [](const MeshPtr& m) -> const MeshTopology& { return m->GetTopology(); });
  mod.method("GetGeometry", [](const MeshPtr& m) { return m->GetGeometry(); });
  mod.method("SetGeometry", [](const MeshPtr& m, GeoPtr g) { m->SetGeometry(g); });
  mod.method("Save", [](const MeshPtr& m, const std::string& f) { m->Save(f); });
  mod.method("Load", [](const MeshPtr& m, const std::string& f) { m->Load(f); });
  mod.method("GetMaterial", [](const MeshPtr& m, int domnr) -> std::string { return std::string(m->GetMaterial(domnr)); });
  mod.method("SetMaterial", [](const MeshPtr& m, int domnr, const std::string& s) { m->SetMaterial(domnr, s); });
  mod.method("GetBCName", [](const MeshPtr& m, int bcnr) -> std::string { return std::string(m->GetBCName(bcnr)); });
  mod.method("SetBCName", [](const MeshPtr& m, int bcnr, const std::string& s) { m->SetBCName(bcnr, s); });
  mod.method("Point", [](const MeshPtr& m, int i) -> MeshPoint { return m->Point(i); });
  mod.method("VolumeElement", [](const MeshPtr& m, int i) -> Element& { return m->VolumeElement(i); });
  mod.method("SurfaceElement", [](const MeshPtr& m, int i) -> Element2d& { return m->SurfaceElement(i); });
  mod.method("Compress", [](const MeshPtr& m) { m->Compress(); });
  mod.method("CalcLocalH", [](const MeshPtr& m, double grading) { m->CalcLocalH(grading); });
  mod.method("GetTimeStamp", [](const MeshPtr& m) { return m->GetTimeStamp(); });
  mod.method("SetNextTimeStamp", [](const MeshPtr& m) { m->SetNextTimeStamp(); });
  mod.method("BuildCurvedElements", [](const MeshPtr& m, const Refinement& r, int order) {
    m->BuildCurvedElements(&r, order);
  });

  // Ngx_Mesh
  mod.method("Valid", [](const Ngx_Mesh& m) { return m.Valid(); });
  mod.method("GetDimension", [](const Ngx_Mesh& m) { return m.GetDimension(); });
  mod.method("GetNLevels", [](const Ngx_Mesh& m) { return m.GetNLevels(); });
  mod.method("GetNVLevel", [](const Ngx_Mesh& m, int level) { return int(m.GetNVLevel(level)); });
  mod.method("GetNElements", [](const Ngx_Mesh& m, int dim) { return m.GetNElements(dim); });
  mod.method("GetNNodes", [](const Ngx_Mesh& m, int nt) { return m.GetNNodes(nt); });
  mod.method("GetParentNodes", [](const Ngx_Mesh& m, int ni, jlcxx::ArrayRef<int> parents) {
    m.GetParentNodes(ni, parents.data());
  });
  mod.method("GetParentElement", [](const Ngx_Mesh& m, int ei) { return m.GetParentElement(ei); });
  mod.method("GetParentSElement", [](const Ngx_Mesh& m, int ei) { return m.GetParentSElement(ei); });
  mod.method("GetCurveOrder", [](Ngx_Mesh& m) { return m.GetCurveOrder(); });
  mod.method("Curve", [](Ngx_Mesh& m, int order) { m.Curve(order); });
  mod.method("UpdateTopology", [](Ngx_Mesh& m) { m.UpdateTopology(); });

  // OCC loaders (free functions)
  mod.method("LoadOCC_STEP", [](const std::string& f) { return GeoPtr(LoadOCC_STEP(f)); });
  mod.method("LoadOCC_IGES", [](const std::string& f) { return GeoPtr(LoadOCC_IGES(f)); });
  mod.method("LoadOCC_BREP", [](const std::string& f) { return GeoPtr(LoadOCC_BREP(f)); });
}
