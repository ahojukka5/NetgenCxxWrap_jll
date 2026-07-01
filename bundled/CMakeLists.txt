# Build libnetgen_cxxwrap — the CxxWrap-based Julia binding for Netgen's exported
# C++ API. It links the prebuilt libnglib/libngcore (from NGSolveNetgen_jll) and
# the OpenCASCADE toolkits (from OCCT_jll), plus JlCxx/libcxxwrap_julia. It uses
# only EXPORTED Netgen symbols (no hidden CSG primitive constructors).
#
#   NETGEN_ROOT : install root with Netgen headers (include/, include/include/)
#                 and libs (lib/). BinaryBuilder: the prefix; local dev: the
#                 extracted NGSolveNetgen artifact (pass -DNETGEN_ROOT=...).
#   JlCxx       : found via find_package(JlCxx) on CMAKE_PREFIX_PATH (the CxxWrap
#                 / libcxxwrap_julia prefix).
#   OpenCASCADE : found via find_package on CMAKE_PREFIX_PATH (OCCT_jll).
cmake_minimum_required(VERSION 3.16)
project(NetgenCxxWrap CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NETGEN_ROOT "${CMAKE_INSTALL_PREFIX}" CACHE PATH "Netgen install root (headers + libs)")

# Match the defines Netgen's public headers were compiled with (USE_OCC build).
add_compile_definitions(OCCGEOMETRY OCC_CONVERT_SIGNALS NXG_HAS_OCC)

find_package(JlCxx CONFIG REQUIRED)
find_package(OpenCASCADE REQUIRED)

add_library(netgen_cxxwrap SHARED
  src/netgen.cpp
  src/netgen_mesh.cpp
  src/netgen_geometry.cpp
  src/netgen_geom2d.cpp
  src/netgen_extras.cpp
  src/netgen_stl.cpp
  src/netgen_gprim.cpp
  src/netgen_mesh2.cpp
  src/netgen_ngx2.cpp
  src/occ.cpp
  src/occ_gp.cpp
  src/occ_topology.cpp
  src/occ_builders.cpp
  src/occ_io.cpp
  src/occ_props.cpp
  src/occ_fillet.cpp
  src/occ_topo2.cpp
  src/occ_adaptor.cpp
  src/occ_analysis.cpp
  src/occ_sweep.cpp
  src/occ_wire.cpp
  src/occ_sewing.cpp
  src/occ_extrema.cpp
  src/occ_builder2.cpp
  src/occ_intersect.cpp
  src/occ_shape_analysis.cpp
  src/occ_breptools.cpp
  src/occ_mesh.cpp
  src/occ_shapefix2.cpp
  src/occ_shapeupgrade.cpp
  src/occ_toptools_list.cpp
  src/occ_geom.cpp
  src/occ_feat.cpp
  src/occ_thicksolid.cpp
  src/occ_draft.cpp
  src/occ_breplib.cpp
)
target_compile_features(netgen_cxxwrap PRIVATE cxx_std_17)

target_include_directories(netgen_cxxwrap PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${NETGEN_ROOT}/include
  ${NETGEN_ROOT}/include/include
  ${OpenCASCADE_INCLUDE_DIR}
)

target_link_directories(netgen_cxxwrap PRIVATE ${NETGEN_ROOT}/lib)
target_link_libraries(netgen_cxxwrap PRIVATE
  JlCxx::cxxwrap_julia
  nglib ngcore
  ${OpenCASCADE_LIBRARIES}
)

# CxxWrap requires DEFAULT symbol visibility so its type_info-keyed type-factory
# registry is shared across the libcxxwrap_julia / libnetgen_cxxwrap boundary
# (hidden visibility breaks type registration at load time).
set_target_properties(netgen_cxxwrap PROPERTIES
  CXX_VISIBILITY_PRESET default
  VISIBILITY_INLINES_HIDDEN OFF)

install(TARGETS netgen_cxxwrap
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib)
