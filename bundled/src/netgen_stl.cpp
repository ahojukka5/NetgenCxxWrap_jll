#include <memory>
#include <string>
#include <fstream>
#include <jlcxx/jlcxx.hpp>
#include <meshing.hpp>
#include <stlgeom/stlgeom.hpp>
#include <stlgeom/stltool.hpp>

using namespace netgen;
using MeshPtr = std::shared_ptr<Mesh>;
using GeoPtr = std::shared_ptr<NetgenGeometry>;

void register_stl(jlcxx::Module& mod) {
  // Types first
  mod.add_type<STLParameters>("STLParameters").constructor<>();
  mod.add_type<STLGeometry>("STLGeometry");

  // STLParameters fields
  mod.method("yangle",           [](const STLParameters& p) { return p.yangle; });
  mod.method("yangle!",          [](STLParameters& p, double v) { p.yangle = v; });
  mod.method("contyangle",       [](const STLParameters& p) { return p.contyangle; });
  mod.method("contyangle!",      [](STLParameters& p, double v) { p.contyangle = v; });
  mod.method("edgecornerangle",  [](const STLParameters& p) { return p.edgecornerangle; });
  mod.method("edgecornerangle!", [](STLParameters& p, double v) { p.edgecornerangle = v; });
  mod.method("chartangle",       [](const STLParameters& p) { return p.chartangle; });
  mod.method("chartangle!",      [](STLParameters& p, double v) { p.chartangle = v; });
  mod.method("outerchartangle",  [](const STLParameters& p) { return p.outerchartangle; });
  mod.method("outerchartangle!", [](STLParameters& p, double v) { p.outerchartangle = v; });
  mod.method("usesearchtree",    [](const STLParameters& p) { return p.usesearchtree; });
  mod.method("usesearchtree!",   [](STLParameters& p, int v) { p.usesearchtree = v; });
  mod.method("recalc_h_opt",     [](const STLParameters& p) { return p.recalc_h_opt; });
  mod.method("recalc_h_opt!",    [](STLParameters& p, bool v) { p.recalc_h_opt = v; });

  // STLGeometry: load from file
  mod.method("LoadSTL", [](const std::string& fname) {
    std::ifstream in(fname);
    return std::shared_ptr<STLGeometry>(STLGeometry::Load(in));
  });

  // STLGeometry: topology queries (from STLTopology base)
  mod.method("GetNT", [](const std::shared_ptr<STLGeometry>& g) { return g->GetNT(); });
  mod.method("GetNP", [](const std::shared_ptr<STLGeometry>& g) { return g->GetNP(); });

  // STLGeometry: mesh generation via NetgenGeometry base
  mod.method("GenerateMesh", [](const std::shared_ptr<STLGeometry>& g,
                                const MeshPtr& m, MeshingParameters& mp) {
    auto gp = std::static_pointer_cast<NetgenGeometry>(g);
    return gp->GenerateMesh(const_cast<MeshPtr&>(m), mp);
  });

  // STLGeometry: refinement via NetgenGeometry base
  mod.method("GetRefinement", [](const std::shared_ptr<STLGeometry>& g) -> const Refinement& {
    auto gp = std::static_pointer_cast<NetgenGeometry>(g);
    return gp->GetRefinement();
  });
}
