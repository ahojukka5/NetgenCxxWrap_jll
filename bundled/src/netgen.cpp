// NetgenCxxWrap — a strict, boring 1:1 CxxWrap binding of NGSolve/Netgen's
// exported C++ API. Every wrapped name matches Netgen's own. No invented or
// combiner functions. All higher-level logic lives in Netgen.jl.
// One unavoidable exception: `new_mesh` (shared_ptr allocator for Mesh).
// Built against NGSolveNetgen_jll (stock build; exported symbols only).
// OCC modeling kernel is a SEPARATE module in occ.cpp (define_julia_module_occ).
#include <jlcxx/jlcxx.hpp>

void register_mesh(jlcxx::Module&);
void register_geometry(jlcxx::Module&);
void register_geom2d(jlcxx::Module&);
void register_extras(jlcxx::Module&);
void register_stl(jlcxx::Module&);
void register_gprim(jlcxx::Module&);
void register_mesh2(jlcxx::Module&);
void register_ngx2(jlcxx::Module&);
void register_ngx3(jlcxx::Module&);

JLCXX_MODULE define_julia_module(jlcxx::Module& mod)
{
  register_mesh(mod);       // Point3d, Vec3d, MeshPoint, Element, Element2d, MeshTopology
  register_geometry(mod);   // MeshingParameters, BisectionOptions, NetgenGeometry, Refinement,
                             // Mesh, Ngx_Mesh, LoadOCC_* free fns
  register_geom2d(mod);     // Solid2d, CSG2d, Circle, Rectangle
  register_extras(mod);     // Segment, FaceDescriptor, LocalH, additional Mesh/MeshTopology methods
  register_stl(mod);        // STLGeometry, STLParameters
  register_gprim(mod);      // Box3d, Point3dTree, SplineGeometry2d
  register_mesh2(mod);      // EdgeDescriptor, GetBox, remaining Mesh methods, LocalH::Copy/Delete
  register_ngx2(mod);       // Ngx_Mesh hp/order/refine; MeshVolume/OptimizeVolume free fns
  register_ngx3(mod);       // Ngx_Mesh transforms, parent edge/face, periodic, partition hints
}
