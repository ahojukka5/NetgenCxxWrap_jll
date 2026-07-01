#include <memory>
#include <string>
#include <vector>
#include <jlcxx/jlcxx.hpp>
#include <meshing.hpp>
#include <gprim/geom3d.hpp>
#include <gprim/adtree.hpp>
#include <geom2d/geometry2d.hpp>

using namespace netgen;
using MeshPtr = std::shared_ptr<Mesh>;
using GeoPtr = std::shared_ptr<NetgenGeometry>;

void register_gprim(jlcxx::Module& mod) {
  // Types first
  mod.add_type<Box3d>("Box3d")
     .constructor<Point3d, Point3d>();

  mod.add_type<Point3dTree>("Point3dTree");

  mod.add_type<SplineGeometry2d>("SplineGeometry2d");

  // Box3d methods
  mod.method("PMin",      [](const Box3d& b) { return b.PMin(); });
  mod.method("PMax",      [](const Box3d& b) { return b.PMax(); });
  mod.method("MinX",      [](const Box3d& b) { return b.MinX(); });
  mod.method("MaxX",      [](const Box3d& b) { return b.MaxX(); });
  mod.method("MinY",      [](const Box3d& b) { return b.MinY(); });
  mod.method("MaxY",      [](const Box3d& b) { return b.MaxY(); });
  mod.method("MinZ",      [](const Box3d& b) { return b.MinZ(); });
  mod.method("MaxZ",      [](const Box3d& b) { return b.MaxZ(); });
  mod.method("IsIn",      [](const Box3d& b, const Point3d& p) { return b.IsIn(p); });
  mod.method("Intersect", [](const Box3d& b1, const Box3d& b2) { return b1.Intersect(b2); });

  // Point3dTree: factory allocator + methods
  // Constructor takes Point<3>; Point3d has operator Point<3>() for implicit conversion.
  mod.method("new_point3dtree", [](const Point3d& pmin, const Point3d& pmax) {
    return std::make_shared<Point3dTree>(Point<3>(pmin), Point<3>(pmax));
  });
  mod.method("Insert", [](const std::shared_ptr<Point3dTree>& t, const Point3d& p, int pi) {
    t->Insert(Point<3>(p), pi);
  });
  mod.method("GetIntersecting", [](const std::shared_ptr<Point3dTree>& t,
                                   const Point3d& pmin, const Point3d& pmax) {
    NgArray<int> pis;
    t->GetIntersecting(Point<3>(pmin), Point<3>(pmax), pis);
    std::vector<int> res(pis.Size());
    for (int i = 0; i < pis.Size(); i++) res[i] = pis[i];
    return res;
  });

  // SplineGeometry2d: factory allocator + load
  mod.method("LoadSplineGeometry2d", [](const std::string& fname) {
    auto g = std::make_shared<SplineGeometry2d>();
    g->Load(fname);
    return std::static_pointer_cast<NetgenGeometry>(g);
  });
}
