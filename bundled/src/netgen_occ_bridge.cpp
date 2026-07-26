// netgen_occ_bridge.cpp — minimal Netgen↔OCC bridge used by Delone.
// TopoDS_Shape stays internal; Julia receives only NetgenGeometry handles,
// face indices, bounding boxes, and identification counts.
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

#include <jlcxx/array.hpp>
#include <jlcxx/jlcxx.hpp>

#include <Bnd_Box.hxx>
#include <BRep_Builder.hxx>
#include <BRepBndLib.hxx>
#include <BRepTools.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>

#include <occgeom.hpp>

namespace {
using GeoPtr = std::shared_ptr<netgen::NetgenGeometry>;
using OCCPtr = std::shared_ptr<netgen::OCCGeometry>;

OCCPtr require_occ_geometry(const GeoPtr& geom) {
  auto occ = std::dynamic_pointer_cast<netgen::OCCGeometry>(geom);
  if (!occ)
    throw std::invalid_argument("geometry is not OCC-backed");
  return occ;
}

netgen::ListOfShapes face_list(
    const OCCPtr& occ, jlcxx::ArrayRef<int> indices) {
  netgen::ListOfShapes result;
  result.reserve(indices.size());
  for (std::size_t i = 0; i < indices.size(); ++i) {
    const int index = indices[i];
    if (index < 1 || index > occ->fmap.Extent())
      throw std::out_of_range("OCC face index out of range");
    result.push_back(occ->fmap(index));
  }
  return result;
}
} // namespace

void register_occ_bridge(jlcxx::Module& mod) {
  using namespace netgen;

  mod.method("OCCGeometry_from_brep_string", [](const std::string& brep) -> GeoPtr {
    TopoDS_Shape shape;
    BRep_Builder builder;
    std::istringstream in(brep);
    BRepTools::Read(shape, in, builder);
    if (shape.IsNull())
      throw std::runtime_error("OCCGeometry_from_brep_string: failed to parse BREP data");
    return std::make_shared<OCCGeometry>(shape);
  });

  mod.method("OCC_NrFaces", [](const GeoPtr& geom) {
    return require_occ_geometry(geom)->fmap.Extent();
  });

  mod.method("OCC_FaceBoundingBox",
             [](const GeoPtr& geom, int face_index, jlcxx::ArrayRef<double> out) {
    auto occ = require_occ_geometry(geom);
    if (face_index < 1 || face_index > occ->fmap.Extent())
      throw std::out_of_range("OCC face index out of range");
    if (out.size() < 6)
      throw std::invalid_argument("bounding-box output must contain at least 6 values");

    Bnd_Box box;
    BRepBndLib::Add(TopoDS::Face(occ->fmap(face_index)), box);
    Standard_Real xmin, ymin, zmin, xmax, ymax, zmax;
    box.Get(xmin, ymin, zmin, xmax, ymax, zmax);
    out[0] = xmin;
    out[1] = ymin;
    out[2] = zmin;
    out[3] = xmax;
    out[4] = ymax;
    out[5] = zmax;
  });

  mod.method("OCC_IdentifyFacesBulk",
             [](const GeoPtr& geom,
                jlcxx::ArrayRef<int> from_indices,
                jlcxx::ArrayRef<int> to_indices,
                const std::string& name,
                int type,
                double tx,
                double ty,
                double tz) {
    auto occ = require_occ_geometry(geom);
    auto from = face_list(occ, from_indices);
    auto to = face_list(occ, to_indices);
    Transformation<3> translation(Vec<3>(tx, ty, tz));
    return static_cast<int>(Identify(
        from,
        to,
        name,
        static_cast<Identifications::ID_TYPE>(type),
        translation));
  });

  mod.method("OCC_RebuildGeometry", [](const GeoPtr& geom) -> GeoPtr {
    auto occ = require_occ_geometry(geom);
    return std::make_shared<OCCGeometry>(occ->shape);
  });
}
