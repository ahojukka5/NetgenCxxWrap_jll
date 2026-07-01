// occ.cpp — entry point for the OCC CxxWrap module (Netgen.OCC submodule).
// Each register_* function is defined in its own translation unit.
#include <jlcxx/jlcxx.hpp>

void register_occ_toptools_list(jlcxx::Module&);
void register_occ_gp(jlcxx::Module&);
void register_occ_topology(jlcxx::Module&);
void register_occ_geom(jlcxx::Module&);
void register_occ_builders(jlcxx::Module&);
void register_occ_io(jlcxx::Module&);
void register_occ_props(jlcxx::Module&);
void register_occ_fillet(jlcxx::Module&);
void register_occ_topo2(jlcxx::Module&);
void register_occ_adaptor(jlcxx::Module&);
void register_occ_analysis(jlcxx::Module&);
void register_occ_sweep(jlcxx::Module&);
void register_occ_wire(jlcxx::Module&);
void register_occ_sewing(jlcxx::Module&);
void register_occ_extrema(jlcxx::Module&);
void register_occ_builder2(jlcxx::Module&);
void register_occ_intersect(jlcxx::Module&);
void register_occ_shape_analysis(jlcxx::Module&);
void register_occ_breptools(jlcxx::Module&);
void register_occ_mesh(jlcxx::Module&);
void register_occ_shapefix2(jlcxx::Module&);
void register_occ_shapeupgrade(jlcxx::Module&);
void register_occ_feat(jlcxx::Module&);
void register_occ_thicksolid(jlcxx::Module&);
void register_occ_draft(jlcxx::Module&);
void register_occ_breplib(jlcxx::Module&);

JLCXX_MODULE define_julia_module_occ(jlcxx::Module& mod)
{
  register_occ_gp(mod);        // gp_Pnt, gp_Vec, gp_Dir, gp_Ax*, gp_Trsf, ...
  register_occ_topology(mod);  // TopoDS_Shape + subtypes, TopExp_Explorer, TopoDS_Iterator
  register_occ_toptools_list(mod);  // TopTools_ListOfShape — needs TopoDS_Shape registered above;
                                     // must itself precede occ_builders/occ_thicksolid/occ_breplib
  register_occ_geom(mod);       // Geom_Curve/Geom_Surface (Handle-managed) — must precede
                                 // occ_builders (MakeEdge/MakeFace Handle(Geom_*) ctors)
  register_occ_builders(mod);  // BRepPrimAPI_*, BRepBuilderAPI_*, BRepAlgoAPI_*
  register_occ_io(mod);        // BRepTools, STEPControl, IGESControl, OCCGeometry
  register_occ_props(mod);     // GProp_GProps, Bnd_Box, BRepGProp/BRepBndLib, BRep_Tool::Pnt
  register_occ_fillet(mod);    // BRepFilletAPI_MakeFillet, BRepFilletAPI_MakeChamfer
  register_occ_topo2(mod);     // TopTools_IndexedMapOfShape, TopExp::MapShapes, TopAbs constants
  register_occ_adaptor(mod);   // BRepAdaptor_Curve, BRepAdaptor_Surface
  register_occ_analysis(mod);  // BRepCheck_Analyzer, ShapeFix_Shape, BRep_Tool scalars
  register_occ_sweep(mod);     // BRepOffsetAPI_MakePipe, ThruSections, MakeOffsetShape
  register_occ_wire(mod);      // BRepTools_WireExplorer, BRepLProp_SLProps, BRepLProp_CLProps
  register_occ_sewing(mod);    // BRepBuilderAPI_Sewing, BRepClass3d_SolidClassifier
  register_occ_extrema(mod);        // BRepExtrema_DistShapeShape, ShapeAnalysis_ShapeContents
  register_occ_builder2(mod);       // BRep_Builder
  register_occ_intersect(mod);      // IntCurvesFace_ShapeIntersector
  register_occ_shape_analysis(mod); // ShapeAnalysis_FreeBounds, ShapeAnalysis_Shell
  register_occ_breptools(mod);      // BRepTools free functions
  register_occ_mesh(mod);           // BRepMesh_IncrementalMesh, BRepAlgoAPI_Check
  register_occ_shapefix2(mod);      // ShapeFix_FreeBounds, ShapeFix_ShapeTolerance, ShapeFix_Wireframe
  register_occ_shapeupgrade(mod);   // ShapeUpgrade_UnifySameDomain
  register_occ_feat(mod);           // BRepFeat_MakePrism, BRepFeat_MakeRevol
  register_occ_thicksolid(mod);     // BRepOffsetAPI_MakeThickSolid
  register_occ_draft(mod);          // BRepOffsetAPI_DraftAngle
  register_occ_breplib(mod);        // BRepLib repair/regularization free functions
}
