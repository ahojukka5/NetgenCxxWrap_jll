#include <memory>
#include <string>
#include <tuple>
#include <array>
#include <vector>
#include <jlcxx/jlcxx.hpp>
#include <jlcxx/array.hpp>
#include <meshing.hpp>
#include <nginterface_v2.hpp>

using namespace netgen;
using MeshPtr = std::shared_ptr<Mesh>;

void register_ngx3(jlcxx::Module& mod) {
  // --- Ngx_Mesh::GetMaterialCD<DIM> (via Mesh; same semantics as Ngx impl) ---
  mod.method("GetMaterialCD0", [](const Ngx_Mesh& m, int region_nr) -> std::string {
    return std::string(m.GetMesh()->GetMaterial(region_nr + 1));
  });
  mod.method("GetMaterialCD1", [](const Ngx_Mesh& m, int region_nr) -> std::string {
    return std::string(m.GetMesh()->GetBCName(region_nr));
  });
  mod.method("GetMaterialCD2", [](const Ngx_Mesh& m, int region_nr) -> std::string {
    return std::string(m.GetMesh()->GetCD2Name(region_nr));
  });
  mod.method("GetMaterialCD3", [](const Ngx_Mesh& m, int region_nr) -> std::string {
    return std::string(m.GetMesh()->GetCD3Name(region_nr));
  });

  // --- Ngx_Mesh curved element maps (reference elnr/selnr are 0-based) ---
  mod.method("ElementTransformation33", [](const Ngx_Mesh& m, int elnr,
                                           jlcxx::ArrayRef<double> xi,
                                           jlcxx::ArrayRef<double> x,
                                           jlcxx::ArrayRef<double> dxdxi) {
    m.ElementTransformation<3, 3>(elnr, xi.data(),
                                  x.size() >= 3 ? x.data() : nullptr,
                                  dxdxi.size() >= 9 ? dxdxi.data() : nullptr);
  });
  mod.method("ElementTransformation23", [](const Ngx_Mesh& m, int selnr,
                                           jlcxx::ArrayRef<double> xi,
                                           jlcxx::ArrayRef<double> x,
                                           jlcxx::ArrayRef<double> dxdxi) {
    m.ElementTransformation<2, 3>(selnr, xi.data(),
                                  x.size() >= 3 ? x.data() : nullptr,
                                  dxdxi.size() >= 6 ? dxdxi.data() : nullptr);
  });
  mod.method("ElementTransformation22", [](const Ngx_Mesh& m, int selnr,
                                           jlcxx::ArrayRef<double> xi,
                                           jlcxx::ArrayRef<double> x,
                                           jlcxx::ArrayRef<double> dxdxi) {
    m.ElementTransformation<2, 2>(selnr, xi.data(),
                                  x.size() >= 2 ? x.data() : nullptr,
                                  dxdxi.size() >= 4 ? dxdxi.data() : nullptr);
  });
  mod.method("ElementTransformation13", [](const Ngx_Mesh& m, int segnr,
                                           jlcxx::ArrayRef<double> xi,
                                           jlcxx::ArrayRef<double> x,
                                           jlcxx::ArrayRef<double> dxdxi) {
    m.ElementTransformation<1, 3>(segnr, xi.data(),
                                  x.size() >= 3 ? x.data() : nullptr,
                                  dxdxi.size() >= 3 ? dxdxi.data() : nullptr);
  });
  mod.method("ElementTransformation12", [](const Ngx_Mesh& m, int segnr,
                                           jlcxx::ArrayRef<double> xi,
                                           jlcxx::ArrayRef<double> x,
                                           jlcxx::ArrayRef<double> dxdxi) {
    m.ElementTransformation<1, 2>(segnr, xi.data(),
                                  x.size() >= 2 ? x.data() : nullptr,
                                  dxdxi.size() >= 2 ? dxdxi.data() : nullptr);
  });

  mod.method("MultiElementTransformation33", [](const Ngx_Mesh& m, int elnr, int npts,
                                                jlcxx::ArrayRef<double> xi,
                                                jlcxx::ArrayRef<double> x,
                                                jlcxx::ArrayRef<double> dxdxi) {
    m.MultiElementTransformation<3, 3>(elnr, npts, xi.data(), 3,
                                       x.size() >= size_t(3 * npts) ? x.data() : nullptr, 3,
                                       dxdxi.size() >= size_t(9 * npts) ? dxdxi.data() : nullptr, 9);
  });
  mod.method("MultiElementTransformation22", [](const Ngx_Mesh& m, int selnr, int npts,
                                                jlcxx::ArrayRef<double> xi,
                                                jlcxx::ArrayRef<double> x,
                                                jlcxx::ArrayRef<double> dxdxi) {
    m.MultiElementTransformation<2, 2>(selnr, npts, xi.data(), 2,
                                       x.size() >= size_t(2 * npts) ? x.data() : nullptr, 2,
                                       dxdxi.size() >= size_t(4 * npts) ? dxdxi.data() : nullptr, 4);
  });

  // --- parent edge/face maps (via MeshTopology; 0-based enr/fnr) ---
  mod.method("HasParentEdges", [](const Ngx_Mesh& m) {
    return m.GetMesh()->GetTopology().HasParentEdges();
  });

  mod.method("GetParentEdges", [](const Ngx_Mesh& m, int enr) {
    auto [info, nrs] = m.GetMesh()->GetTopology().GetParentEdges(enr);
    return std::make_tuple(info, nrs[0], nrs[1], nrs[2]);
  });
  mod.method("GetParentFaces", [](const Ngx_Mesh& m, int fnr) {
    auto [info, nrs] = m.GetMesh()->GetTopology().GetParentFaces(fnr);
    return std::make_tuple(info, nrs[0], nrs[1], nrs[2], nrs[3]);
  });

  // --- face topology (fnr 0-based) ---
  mod.method("GetFaceEdges", [](const Ngx_Mesh& m, int fnr, jlcxx::ArrayRef<int> buf) {
    auto fe = m.GetFaceEdges(fnr);
    int n = int(fe.Size());
    for (int i = 0; i < n && i < int(buf.size()); ++i) buf[i] = fe[i];
    return n;
  });

  // --- periodic identification pairs (0-based vertex ids; returns pair count) ---
  mod.method("GetPeriodicVertices", [](const Ngx_Mesh& m, int idnr,
                                       jlcxx::ArrayRef<int> buf) {
    NgArray<INDEX_2> apairs;
    m.GetMesh()->GetIdentifications().GetPairs(idnr + 1, apairs);
    int n = int(apairs.Size());
    for (int i = 0; i < n && 2 * i + 1 < int(buf.size()); ++i) {
      buf[2 * i]     = int(apairs[i].I1()) - IndexBASE<PointIndex>();
      buf[2 * i + 1] = int(apairs[i].I2()) - IndexBASE<PointIndex>();
    }
    return n;
  });

  // --- MPI partition hints (serial build: global id == local; no distant procs) ---
  mod.method("GetGlobalVertexNum", [](const Ngx_Mesh& m, int locnum) -> size_t {
    return m.GetGlobalVertexNum(locnum);
  });
  mod.method("GetDistantProcs", [](const Ngx_Mesh& m, int nodetype, int locnum) {
    auto arr = m.GetDistantProcs(nodetype, locnum);
    return std::vector<int>(arr.begin(), arr.end());
  });

  // --- point location (Ngx_Mesh::FindElementOfPoint; elnr 0-based, -1 if not found) ---
  mod.method("FindElementOfPoint1", [](const Ngx_Mesh& m,
                                       jlcxx::ArrayRef<double> p,
                                       bool build_searchtree,
                                       jlcxx::ArrayRef<int> indices,
                                       double tol) {
    double pt[3] = {p[0], p.size() > 1 ? p[1] : 0.0, p.size() > 2 ? p[2] : 0.0};
    double lami[1] = {0.0};
    int * ip = indices.size() > 0 ? const_cast<int*>(indices.data()) : nullptr;
    int elnr = m.FindElementOfPoint<1>(pt, lami, build_searchtree, ip, int(indices.size()), tol);
    if (elnr < 0) return std::make_tuple(-1, 0.0);
    return std::make_tuple(elnr, lami[0]);
  });
  mod.method("FindElementOfPoint2", [](const Ngx_Mesh& m,
                                       jlcxx::ArrayRef<double> p,
                                       bool build_searchtree,
                                       jlcxx::ArrayRef<int> indices,
                                       double tol) {
    double pt[3] = {p[0], p.size() > 1 ? p[1] : 0.0, p.size() > 2 ? p[2] : 0.0};
    double lami[2] = {0.0, 0.0};
    int * ip = indices.size() > 0 ? const_cast<int*>(indices.data()) : nullptr;
    int elnr = m.FindElementOfPoint<2>(pt, lami, build_searchtree, ip, int(indices.size()), tol);
    SurfaceElementIndex sei(elnr);
    if (!sei.IsValid()) return std::make_tuple(-1, 0.0, 0.0);
    return std::make_tuple(elnr, lami[0], lami[1]);
  });
  mod.method("FindElementOfPoint3", [](const Ngx_Mesh& m,
                                       jlcxx::ArrayRef<double> p,
                                       bool build_searchtree,
                                       jlcxx::ArrayRef<int> indices,
                                       double tol) {
    double pt[3] = {p[0], p.size() > 1 ? p[1] : 0.0, p.size() > 2 ? p[2] : 0.0};
    double lami[4] = {0.0, 0.0, 0.0, 0.0};
    int * ip = indices.size() > 0 ? const_cast<int*>(indices.data()) : nullptr;
    int elnr = m.FindElementOfPoint<3>(pt, lami, build_searchtree, ip, int(indices.size()), tol);
    ElementIndex ei(elnr);
    if (!ei.IsValid()) return std::make_tuple(-1, 0.0, 0.0, 0.0, 0.0);
    return std::make_tuple(elnr, lami[0], lami[1], lami[2], lami[3]);
  });
}
