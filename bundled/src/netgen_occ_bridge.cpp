// netgen_occ_bridge.cpp — minimal Netgen↔OCC bridge (BREP string → OCCGeometry).
// TopoDS_Shape is internal; not exposed as a Julia type from NetgenCxxWrap.
#include <sstream>
#include <stdexcept>
#include <string>

#include <jlcxx/jlcxx.hpp>

#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <TopoDS_Shape.hxx>

#include <occgeom.hpp>

void register_occ_bridge(jlcxx::Module& mod) {
  using namespace netgen;
  using GeoPtr = std::shared_ptr<NetgenGeometry>;

  mod.method("OCCGeometry_from_brep_string", [](const std::string& brep) -> GeoPtr {
    TopoDS_Shape shape;
    BRep_Builder builder;
    std::istringstream in(brep);
    BRepTools::Read(shape, in, builder);
    if (shape.IsNull())
      throw std::runtime_error("OCCGeometry_from_brep_string: failed to parse BREP data");
    return std::make_shared<OCCGeometry>(shape);
  });
}
