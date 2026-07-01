#include <memory>
#include <string>
#include <vector>
#include <jlcxx/jlcxx.hpp>
#include <jlcxx/array.hpp>
#include <meshing.hpp>
#include <meshing/localh.hpp>

using namespace netgen;
using MeshPtr = std::shared_ptr<Mesh>;

void register_extras(jlcxx::Module& mod) {
  // --- Type registrations (before any method that references them) ---

  // Segment
  mod.add_type<Segment>("Segment").constructor<>();

  // FaceDescriptor
  mod.add_type<FaceDescriptor>("FaceDescriptor")
     .constructor<>()
     .constructor<int, int, int, int>();

  // LocalH — not easily value-constructible from Julia's perspective;
  // use a free-function allocator (new_localh) that returns shared_ptr.
  mod.add_type<LocalH>("LocalH");

  // --- Segment methods ---
  mod.method("GetNP",    [](const Segment& s) { return s.GetNP(); });
  mod.method("GetIndex", [](const Segment& s) { return s.GetIndex(); });
  mod.method("SetIndex", [](Segment& s, int i) { s.SetIndex(i); });
  // 1-based access to pnums (returns int for easy Julia use)
  mod.method("PNum",     [](const Segment& s, int i) { return int(s[i-1]); });

  // --- FaceDescriptor methods ---
  mod.method("SurfNr",      [](const FaceDescriptor& fd) { return fd.SurfNr(); });
  mod.method("DomainIn",    [](const FaceDescriptor& fd) { return fd.DomainIn(); });
  mod.method("DomainOut",   [](const FaceDescriptor& fd) { return fd.DomainOut(); });
  mod.method("TLOSurface",  [](const FaceDescriptor& fd) { return fd.TLOSurface(); });
  mod.method("BCProperty",  [](const FaceDescriptor& fd) { return fd.BCProperty(); });
  mod.method("GetBCName",   [](const FaceDescriptor& fd) -> std::string { return std::string(fd.GetBCName()); });
  mod.method("SetDomainIn",  [](FaceDescriptor& fd, int v) { fd.SetDomainIn(v); });
  mod.method("SetDomainOut", [](FaceDescriptor& fd, int v) { fd.SetDomainOut(v); });
  mod.method("SetBCProperty",[](FaceDescriptor& fd, int v) { fd.SetBCProperty(v); });
  mod.method("SetBCName",    [](FaceDescriptor& fd, const std::string& s) { fd.SetBCName(s); });

  // --- LocalH allocator and methods ---
  // LocalH(Point<3> pmin, Point<3> pmax, double grading); Point3d converts to Point<3>.
  mod.method("new_localh", [](const Point3d& pmin, const Point3d& pmax, double g) {
    return std::make_shared<LocalH>(Point<3>(pmin), Point<3>(pmax), g);
  });
  mod.method("SetH",    [](const std::shared_ptr<LocalH>& lh, const Point3d& p, double h) { lh->SetH(Point<3>(p), h); });
  mod.method("GetH",    [](const std::shared_ptr<LocalH>& lh, const Point3d& p) -> double { return lh->GetH(Point<3>(p)); });
  mod.method("GetMinH", [](const std::shared_ptr<LocalH>& lh, const Point3d& pmin, const Point3d& pmax) {
    return lh->GetMinH(Point<3>(pmin), Point<3>(pmax));
  });

  // --- Additional Mesh methods that reference Segment/FaceDescriptor ---
  // Segment/FaceDescriptor are now registered above, so methods that return
  // references to them can be added here safely.

  mod.method("AddPoint",          [](const MeshPtr& m, const Point3d& p) { return int(m->AddPoint(p)); });
  mod.method("AddVolumeElement",  [](const MeshPtr& m, const Element& el) { return int(m->AddVolumeElement(el)); });
  mod.method("AddSurfaceElement", [](const MeshPtr& m, const Element2d& el) { return int(m->AddSurfaceElement(el)); });
  mod.method("AddSegment",        [](const MeshPtr& m, const Segment& s) { return int(m->AddSegment(s)); });
  mod.method("LineSegment",       [](const MeshPtr& m, int i) -> Segment& { return m->LineSegment(i); });
  mod.method("GetFaceDescriptor", [](const MeshPtr& m, int i) -> const FaceDescriptor& { return m->GetFaceDescriptor(i); });
  mod.method("GetFaceDescriptorMut", [](const MeshPtr& m, int i) -> FaceDescriptor& { return m->GetFaceDescriptor(i); });
  mod.method("RestrictLocalH",    [](const MeshPtr& m, const Point3d& p, double h) { m->RestrictLocalH(p, h); });
  mod.method("ImproveMesh",       [](const MeshPtr& m, const MeshingParameters& mp) { m->ImproveMesh(mp); });
  mod.method("CheckVolumeMesh",          [](const MeshPtr& m) { return m->CheckVolumeMesh(); });
  mod.method("CheckConsistentBoundary",  [](const MeshPtr& m) { return m->CheckConsistentBoundary(); });

  // --- Additional MeshTopology connectivity methods ---
  // GetEdgeVertices: fills a caller-supplied 2-element buffer [v1, v2].
  mod.method("GetEdgeVertices", [](const MeshTopology& t, int enr, jlcxx::ArrayRef<int> buf) {
    auto vv = t.GetEdgeVertices(enr);   // returns std::array<PointIndex,2>
    if (buf.size() >= 2) { buf[0] = int(vv[0]); buf[1] = int(vv[1]); }
  });

  // GetFaceVertices: fills a caller-supplied buffer; returns actual vertex count.
  mod.method("GetFaceVertices", [](const MeshTopology& t, int fnr, jlcxx::ArrayRef<int> buf) {
    NgArray<int> verts;
    t.GetFaceVertices(fnr, verts);
    int n = int(verts.Size());
    for (int i = 0; i < n && i < int(buf.size()); ++i) buf[i] = verts[i];
    return n;
  });

  // GetFaceEdges: fills a caller-supplied buffer; returns actual edge count.
  mod.method("GetFaceEdges", [](const MeshTopology& t, int fnr, jlcxx::ArrayRef<int> buf) {
    NgArray<int> edges;
    t.GetFaceEdges(fnr, edges);
    int n = int(edges.Size());
    for (int i = 0; i < n && i < int(buf.size()); ++i) buf[i] = edges[i];
    return n;
  });
}
