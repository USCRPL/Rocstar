project(Rocon)

set (LIB_SRCS src/Rocon.C src/ray-triangle.C src/GEM.C
              src/GeoPartFunctions.C src/Partition.C
              src/RemeshFunctions.C src/RocfluFunctions.C
              src/TRAIL.C src/TransferFunctions.C
              src/PMesh.C src/BSMesh.C src/Mesh.C src/GeoPrimitives.C)
set (UTIL_SRCS src/flotsam.C src/genflu.C src/wrl2hdf.C)
set (ALL_SRCS ${LIB_SRCS} ${UTIL_SRCS})

add_library(Rocon ${LIB_SRCS})

target_link_libraries(Rocon Roccom IRAD mpi_cxx)

target_include_directories(Rocon PUBLIC include)

add_executable(flotsam src/flotsam.C)
add_executable(genflu  src/genflu.C )
#add_executable(genstar src/genstar.C)
add_executable(wrl2hdf src/wrl2hdf.C)
#add_executable(profane src/profane.C)
target_link_libraries(flotsam Rocon)
target_link_libraries(genflu  Rocon)
#target_link_libraries(genstar Rocon)
target_link_libraries(wrl2hdf Rocon)
#target_link_libraries(profane Rocon)

INSTALL(TARGETS Rocon flotsam genflu wrl2hdf RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
