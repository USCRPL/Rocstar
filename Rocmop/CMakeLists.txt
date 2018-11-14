project(Rocmop)

add_subdirectory(External/mesquite_0_9_5)

set(MESQUITE_DIR ${PROJECT_SOURCE_DIR}/External/mesquite_0_9_5)
set(ROCMOP2_SRCS src/Rocmop_2.C src/smooth_mesquite_2.C src/smooth_mesquite_ng_2.C
                 src/smooth_medial_2.C)
set(ROCMOP_SRCS src/Rocmop_1.C src/smooth_mesquite_1.C src/smooth_mesquite_ng_1.C 
                src/smooth_medial_1.C)
set (LIB_SRCS src/geometry.C src/Matrix.C src/Algebraic_Metrics_2.C src/Algebraic_Metrics_3.C
              src/Geometric_Metrics_2.C src/Geometric_Metrics_3.C src/PN_patch.C)
			  
IF(ROCMOP2)
  set(LIB_SRCS ${LIB_SRCS} ${ROCMOP2_SRCS} src/MesqPane_95.C)
ELSE()
  set(LIB_SRCS ${LIB_SRCS} ${ROCMOP_SRCS} src/MesqPane_95.C)
ENDIF()

set (UTIL_SRCS util/chkPconnGRecv.cc util/RocMeshData.cc)
set (TEST_SRCS test/add_aspect_ratios.C test/build_meshes.C test/metric_demo.C 
               test/smooth_volume.C)
set (ALL_SRCS ${LIB_SRCS} ${TEST_SRCS} ${UTIL_SRCS})


add_definitions(-DMESQUITE)

add_library(Rocmop ${LIB_SRCS})

target_link_libraries(Rocmop Rocsurf Rocmap Rocblas Rocout Rocin Roccom mesquite_95 m mpi_cxx)

target_include_directories(Rocmop PUBLIC include)

add_executable(chkPconnGRecv ${UTIL_SRCS})
add_executable(add_aspect_ratios test/add_aspect_ratios.C)
add_executable(build_meshes test/build_meshes.C)
add_executable(metric_demo test/metric_demo.C)
add_executable(smooth_volume test/smooth_volume.C)
target_link_libraries(chkPconnGRecv Rocmop)
target_link_libraries(add_aspect_ratios Rocmop)
target_link_libraries(build_meshes Rocmop)
target_link_libraries(metric_demo Rocmop)
target_link_libraries(smooth_volume Rocmop)

INSTALL(TARGETS Rocmop RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
