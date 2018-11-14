project(Rocprop)

set (LIB_SRCS src/Propagation_3.C src/MarkerParticles_3.C src/FaceOffset_3.C src/Rocprop.C
              src/NuLaplacian.C src/cons_diff.C src/AnisotropicSmoothing.C src/quadric_analysis.C
              src/detect_features.C)
set (TEST_SRCS test/proptest.C test/mptest.C test/compare_normals.C test/rotate.C
               test/advectest.C test/detectfeas.C test/PointPropagate.C)
set (ALL_SRCS ${LIB_SRCS} ${TEST_SRCS})

add_library(Rocprop ${LIB_SRCS})

target_link_libraries(Rocprop Rocsurf Rocmap Rocblas Roccom mpi_cxx)

target_include_directories(Rocprop PUBLIC include)

add_executable(proptest test/proptest.C)
add_executable(mptest test/mptest.C)
add_executable(compare_normals test/compare_normals.C)
add_executable(rotate test/rotate.C)
add_executable(advectest test/advectest.C test/PointPropagate.C)
add_executable(detectfeas test/detectfeas.C)

targets_link_libraries(proptest mptest compare_normals rotate advectest detectfeas LIBRARIES Rocprop)

INSTALL(TARGETS Rocprop RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
