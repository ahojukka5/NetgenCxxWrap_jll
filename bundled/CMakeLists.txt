# Build libnetgen_cxxwrap — the CxxWrap-based Julia binding for Netgen's exported
# C++ API. It links the prebuilt libnglib/libngcore (from NGSolveNetgen_jll) and
# the OpenCASCADE toolkits (from OCCT_jll) for the internal BREP bridge only.
cmake_minimum_required(VERSION 3.16)
project(NetgenCxxWrap CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NETGEN_ROOT "${CMAKE_INSTALL_PREFIX}" CACHE PATH "Netgen install root (headers + libs)")

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
  src/netgen_ngx3.cpp
  src/netgen_occ_bridge.cpp
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

set_target_properties(netgen_cxxwrap PROPERTIES
  CXX_VISIBILITY_PRESET default
  VISIBILITY_INLINES_HIDDEN OFF)

install(TARGETS netgen_cxxwrap
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib)
