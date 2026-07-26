#include <memory>
#include <jlcxx/jlcxx.hpp>
#include <meshing.hpp>
using namespace netgen;
using MeshPtr = std::shared_ptr<Mesh>;

void register_mesh(jlcxx::Module& mod) {
  // Types
  mod.add_type<Point3d>("Point3d").constructor<double, double, double>();
  mod.add_type<Vec3d>("Vec3d").constructor<double, double, double>();
  auto meshpoint_t = mod.add_type<MeshPoint>("MeshPoint");
  meshpoint_t.method([](const MeshPoint& p, int i) { return p(i); });
  mod.add_type<Element>("Element").constructor<int>();
  mod.add_type<Element2d>("Element2d").constructor<int>();
  mod.add_type<MeshTopology>("MeshTopology");

  // Point3d / Vec3d
  mod.method("X", [](const Point3d& p) { return p.X(); });
  mod.method("Y", [](const Point3d& p) { return p.Y(); });
  mod.method("Z", [](const Point3d& p) { return p.Z(); });
  mod.method("X", [](const Vec3d& v) { return v.X(); });
  mod.method("Y", [](const Vec3d& v) { return v.Y(); });
  mod.method("Z", [](const Vec3d& v) { return v.Z(); });
  mod.method("Length", [](const Vec3d& v) { return v.Length(); });

  // Element / Element2d
  mod.method("GetNP", [](const Element& e) { return e.GetNP(); });
  mod.method("GetNV", [](const Element& e) { return int(e.GetNV()); });
  mod.method("GetType", [](const Element& e) { return int(e.GetType()); });
  mod.method("GetIndex", [](const Element& e) { return e.GetIndex(); });
  mod.method("SetIndex", [](Element& e, int index) { e.SetIndex(index); });
  mod.method("PNum", [](const Element& e, int i) { return int(e.PNum(i)); });
  mod.method("SetPNum", [](Element& e, int i, int point) { e.PNum(i) = PointIndex(point); });
  mod.method("SetRefinementFlag", [](Element& e, bool f) { e.SetRefinementFlag(f); });
  mod.method("TestRefinementFlag", [](const Element& e) { return e.TestRefinementFlag(); });
  mod.method("SetStrongRefinementFlag", [](Element& e, bool f) { e.SetStrongRefinementFlag(f); });
  mod.method("TestStrongRefinementFlag", [](const Element& e) { return e.TestStrongRefinementFlag(); });
  mod.method("GetNP", [](const Element2d& e) { return e.GetNP(); });
  mod.method("GetNV", [](const Element2d& e) { return int(e.GetNV()); });
  mod.method("GetType", [](const Element2d& e) { return int(e.GetType()); });
  mod.method("GetIndex", [](const Element2d& e) { return e.GetIndex(); });
  mod.method("SetIndex", [](Element2d& e, int index) { e.SetIndex(index); });
  mod.method("PNum", [](const Element2d& e, int i) { return int(e.PNum(i)); });
  mod.method("SetPNum", [](Element2d& e, int i, int point) { e.PNum(i) = PointIndex(point); });
  mod.method("SetRefinementFlag", [](Element2d& e, bool f) { e.SetRefinementFlag(f); });
  mod.method("TestRefinementFlag", [](const Element2d& e) { return e.TestRefinementFlag(); });
  mod.method("SetStrongRefinementFlag", [](Element2d& e, bool f) { e.SetStrongRefinementFlag(f); });
  mod.method("TestStrongRefinementFlag", [](const Element2d& e) { return e.TestStrongRefinementFlag(); });

  // MeshTopology
  mod.method("GetNEdges", [](const MeshTopology& t) { return int(t.GetNEdges()); });
  mod.method("GetNFaces", [](const MeshTopology& t) { return int(t.GetNFaces()); });
}
