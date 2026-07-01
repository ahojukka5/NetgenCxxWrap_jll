#include <memory>
#include <string>
#include <jlcxx/jlcxx.hpp>
#include <meshing.hpp>
#include <geom2d/csg2d.hpp>
using namespace netgen;
using MeshPtr = std::shared_ptr<Mesh>;
using GeoPtr = std::shared_ptr<NetgenGeometry>;

void register_geom2d(jlcxx::Module& mod) {
  auto solid2d_t = mod.add_type<Solid2d>("Solid2d");
  mod.add_type<CSG2d>("CSG2d").constructor<>();

  mod.method("Circle", [](double cx, double cy, double r, const std::string& name, const std::string& bc) {
    return Circle(Point<2>(cx, cy), r, name, bc);
  });
  mod.method("Rectangle", [](double x0, double y0, double x1, double y1, const std::string& mat, const std::string& bc) {
    return Rectangle(Point<2>(x0, y0), Point<2>(x1, y1), mat, bc);
  });
  mod.set_override_module(jl_base_module);
  mod.method("+", [](const Solid2d& a, const Solid2d& b) { return a + b; });
  mod.method("*", [](const Solid2d& a, const Solid2d& b) { return a * b; });
  mod.method("-", [](const Solid2d& a, const Solid2d& b) { return a - b; });
  mod.unset_override_module();
  solid2d_t.method("BC", [](Solid2d& s, const std::string& bc) { s.BC(bc); });
  solid2d_t.method("Maxh", [](Solid2d& s, double h) { s.Maxh(h); });
  solid2d_t.method("Mat", [](Solid2d& s, const std::string& m) { s.Mat(m); });
  mod.method("Add", [](CSG2d& g, const Solid2d& s) { g.Add(s); });
  mod.method("GenerateSplineGeometry", [](CSG2d& g) -> GeoPtr { return g.GenerateSplineGeometry(); });
  mod.method("GenerateMesh", [](CSG2d& g, MeshingParameters& mp) { return g.GenerateMesh(mp); });
}
