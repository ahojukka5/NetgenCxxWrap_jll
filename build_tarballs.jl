# Note that this script can accept some limited command-line arguments, run
# `julia build_tarballs.jl --help` to see a usage message.
using BinaryBuilder, Pkg

# NetgenCxxWrap — a CxxWrap-based Julia binding for the exported C++ API of
# NGSolve/Netgen. This JLL builds `libnetgen_cxxwrap`, a CxxWrap module linked
# against the prebuilt NGSolve/Netgen libraries (NGSolveNetgen_jll), the
# OpenCASCADE toolkits (OCCT_jll) and JlCxx/libcxxwrap_julia
# (libcxxwrap_julia_jll). It is a boring, comprehensive wrapper: it exposes
# exported Netgen C++ functionality to Julia and contains no logic of its own.
# Higher-level utilities live in the Netgen.jl package.
#
# It uses only EXPORTED Netgen symbols (OCC loaders, Mesh/MeshTopology accessors,
# NetgenGeometry/Refinement); it does not touch the hidden CSG primitive
# constructors. Because a BinaryBuilder `Dependency` resolves from the registry,
# this recipe can only be built once NGSolveNetgen_jll is registered.
name = "NetgenCxxWrap"
version = v"0.1.0"

# Only the CxxWrap module sources; Netgen, OCC and JlCxx come from dependencies.
sources = [
    DirectorySource("./bundled"),
]

# Bash recipe: configure + build the bundled CxxWrap module. NETGEN_ROOT and the
# JlCxx / OpenCASCADE CMake packages are all found under ${prefix}, where the
# dependencies install.
script = raw"""
cd ${WORKSPACE}/srcdir
mkdir build
cd build
cmake .. \
    -DCMAKE_INSTALL_PREFIX=${prefix} \
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TARGET_TOOLCHAIN} \
    -DCMAKE_BUILD_TYPE=Release \
    -DNETGEN_ROOT=${prefix} \
    -DJlCxx_DIR=${prefix}/lib/cmake/JlCxx \
    -DCMAKE_PREFIX_PATH=${prefix}
make -j${nproc}
make install
install_license ${WORKSPACE}/srcdir/LICENSE
"""

# Match NGSolveNetgen_jll's platform set (we link its libraries). CxxWrap modules
# are also constrained to the platforms libcxxwrap_julia_jll supports.
platforms = supported_platforms()
platforms = filter!(p -> arch(p) != "armv6l" && !Sys.iswindows(p), platforms)
platforms = expand_cxxstring_abis(platforms)

# The products that we will ensure are always built
products = [
    LibraryProduct("libnetgen_cxxwrap", :libnetgen_cxxwrap),
]

# Dependencies that must be installed before this package can be built
dependencies = [
    Dependency("NGSolveNetgen_jll"),
    Dependency("OCCT_jll"; compat="7.9.3"),
    Dependency("Zlib_jll"),
    # JlCxx / libcxxwrap_julia: must match the CxxWrap.jl version used at runtime.
    Dependency("libcxxwrap_julia_jll"),
]

# Build the tarballs, and possibly a `build.jl` as well.
build_tarballs(ARGS, name, version, sources, script, platforms, products, dependencies;
               julia_compat="1.6", preferred_gcc_version=v"10")
