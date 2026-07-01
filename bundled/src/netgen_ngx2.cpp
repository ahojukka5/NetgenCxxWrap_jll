#include <memory>
#include <jlcxx/jlcxx.hpp>
#include <jlcxx/array.hpp>
#include <meshing.hpp>
#include <meshing/meshfunc.hpp>
#include <nginterface_v2.hpp>

using namespace netgen;
using MeshPtr = std::shared_ptr<Mesh>;

void register_ngx2(jlcxx::Module& mod) {
  // --- Remaining Ngx_Mesh methods ---
  // (Ngx_Mesh type is already registered in register_geometry)

  // GetPoint: Ng_Point wraps a double[3] pointer; extract into Point3d
  mod.method("GetPoint", [](const Ngx_Mesh& m, int nr) {
    auto p = m.GetPoint(nr);
    return Point3d(p[0], p[1], p[2]);
  });

  // GetElementIndex is templated on DIM; DIM=3 wraps volume elements
  mod.method("GetElementIndex", [](const Ngx_Mesh& m, int nr) -> int {
    return m.GetElementIndex<3>(size_t(nr));
  });

  // hp-refinement level per element per direction
  mod.method("GetHPElementLevel", [](const Ngx_Mesh& m, int ei, int dir) {
    return m.GetHPElementLevel(ei, dir);
  });

  // Periodic identification info
  mod.method("GetNIdentifications",   [](const Ngx_Mesh& m) { return m.GetNIdentifications(); });
  mod.method("GetIdentificationType", [](const Ngx_Mesh& m, int idnr) {
    return m.GetIdentificationType(idnr);
  });

  // Surface element → geometry index mapping
  mod.method("GetSurfaceElementSurfaceNumber", [](const Ngx_Mesh& m, int ei) {
    return m.GetSurfaceElementSurfaceNumber(size_t(ei));
  });
  mod.method("GetSurfaceElementFDNumber", [](const Ngx_Mesh& m, int ei) {
    return m.GetSurfaceElementFDNumber(size_t(ei));
  });

  // hp polynomial orders
  mod.method("GetElementOrder", [](const Ngx_Mesh& m, int enr) {
    return m.GetElementOrder(enr);
  });
  // GetElementOrders fills ox, oy, oz; use 3-element ArrayRef buffer
  mod.method("GetElementOrders", [](const Ngx_Mesh& m, int enr, jlcxx::ArrayRef<int> buf) {
    int ox = 0, oy = 0, oz = 0;
    m.GetElementOrders(enr, &ox, &oy, &oz);
    if (buf.size() >= 3) { buf[0] = ox; buf[1] = oy; buf[2] = oz; }
  });
  mod.method("GetSurfaceElementOrder", [](const Ngx_Mesh& m, int enr) {
    return m.GetSurfaceElementOrder(enr);
  });
  // GetSurfaceElementOrders fills ox, oy; use 2-element ArrayRef buffer
  mod.method("GetSurfaceElementOrders", [](const Ngx_Mesh& m, int enr, jlcxx::ArrayRef<int> buf) {
    int ox = 0, oy = 0;
    m.GetSurfaceElementOrders(enr, &ox, &oy);
    if (buf.size() >= 2) { buf[0] = ox; buf[1] = oy; }
  });

  // Cluster representative IDs (hp-refinement)
  mod.method("GetClusterRepVertex",  [](const Ngx_Mesh& m, int vi)  { return m.GetClusterRepVertex(vi); });
  mod.method("GetClusterRepEdge",    [](const Ngx_Mesh& m, int edi) { return m.GetClusterRepEdge(edi); });
  mod.method("GetClusterRepFace",    [](const Ngx_Mesh& m, int fai) { return m.GetClusterRepFace(fai); });
  mod.method("GetClusterRepElement", [](const Ngx_Mesh& m, int eli) { return m.GetClusterRepElement(eli); });

  // Element ↔ face connectivity (buffer-fill, returns face count)
  mod.method("GetElement_Faces", [](const Ngx_Mesh& m, int elnr, jlcxx::ArrayRef<int> buf) {
    int faces[6] = {0};
    int n = m.GetElement_Faces(elnr, faces);
    for (int i = 0; i < n && i < int(buf.size()); ++i) buf[i] = faces[i];
    return n;
  });

  // Surface element → face index
  mod.method("GetSurfaceElement_Face", [](const Ngx_Mesh& m, int selnr) {
    return m.GetSurfaceElement_Face(selnr);
  });

  // --- Free meshing functions (meshing/meshfunc.hpp) ---
  // Returns int (MESHING3_RESULT enum: 0=OK, 1=GIVEUP, 2=NEGVOL, etc.)
  mod.method("MeshVolume", [](const MeshingParameters& mp, const MeshPtr& m) {
    return int(MeshVolume(mp, *m));
  });
  mod.method("OptimizeVolume", [](const MeshingParameters& mp, const MeshPtr& m) {
    return int(OptimizeVolume(mp, *m));
  });
  mod.method("RemoveIllegalElements", [](const MeshPtr& m, int domain) {
    RemoveIllegalElements(*m, domain);
  });
  mod.method("ConformToFreeSegments", [](const MeshPtr& m, int domain) {
    ConformToFreeSegments(*m, domain);
  });
}
