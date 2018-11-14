project(Rocsurf)

set (SURFLIB_SRCS src/Rocsurf.C src/Manifold_2.C src/Generic_element_2.C src/interpolate_to_centers.C 
                 src/compute_element_normals.C src/compute_element_areas.C src/compute_bounded_volumes.C src/compute_curvature.C)
set (TEST_SRCS test/surfmap.C test/surftest1.C test/surftest2.C test/imtest.C test/im2hdf.C test/smf2hdf.C test/neu2hdf.C
     test/mcntest.C test/obj2hdf.C test/msh2hdf.C test/serialize.C)
set (ALL_SURF_SRCS "${SURFLIB_SRCS} ${TEST_SRCS}")

add_library(Rocsurf ${SURFLIB_SRCS})

target_link_libraries(Rocsurf Rocmap Rocblas Rocout mpi_cxx)

target_include_directories(Rocsurf PUBLIC include test)

add_executable(surfmap test/surfmap.C)
target_link_libraries(surfmap Rocsurf)
add_executable(surftest1 test/surftest1.C)
target_link_libraries(surftest1 Rocsurf)
add_executable(surftest2 test/surftest2.C)
target_link_libraries(surftest2 Rocsurf)
add_executable(imtest test/imtest.C)
target_link_libraries(imtest Rocsurf)
add_executable(im2hdf test/im2hdf.C)
target_link_libraries(im2hdf Rocsurf)
add_executable(smf2hdf test/smf2hdf.C)
target_link_libraries(smf2hdf Rocsurf)
add_executable(neu2hdf test/neu2hdf.C)
target_link_libraries(neu2hdf Rocsurf)
add_executable(mcntest test/mcntest.C)
target_link_libraries(mcntest Rocsurf)
add_executable(obj2hdf test/obj2hdf.C)
target_link_libraries(obj2hdf Rocsurf)
add_executable(msh2hdf test/msh2hdf.C)
target_link_libraries(msh2hdf Rocsurf)
add_executable(serialize test/serialize.C)
target_link_libraries(serialize Rocsurf)

install_libraries(Rocsurf)