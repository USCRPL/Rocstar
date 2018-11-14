project(Rocface)

set (RFC_SRCS src/Base/RFC_Window_base.C src/Base/RFC_Window_base_IO.C src/Base/RFC_Window_base_IO_tecplot.C 
              src/Base/RFC_Window_base_IO_binary.C src/Overlay/RFC_Window_overlay.C src/Overlay/RFC_Window_overlay_fea.C
              src/Overlay/RFC_Window_overlay_IO.C src/Overlay/Overlay_primitives.C src/Overlay/Overlay_0d.C 
              src/Overlay/Overlay_init.C src/Overlay/Overlay.C src/Overlay/Overlay_IO.C src/Base/Vector_n.C
              src/Transfer/RFC_Window_transfer.C src/Transfer/RFC_Window_transfer_comm.C src/Transfer/Transfer_base.C
              src/Transfer/Transfer_2f.C src/Transfer/Transfer_2n.C src/Rocface.C)
#              src/Base/writer.C src/Base/rfc_assertions.C)
#              src/Transfer/Transfer_2f.C src/Transfer/Transfer_2n.C src/Rocface.C src/Overlay/Triangulation.C 
set (TEST_SRCS test/ex1.C test/ex2.C test/ex3.C test/ex4.C test/ex5.C test/plot3d.C test/reptrans.C test/readsdv.C test/ex6DataTransfer.C test/meshio.C)
set (UTIL_SRCS util/surfdiver.C util/rfctest.C util/autosurfer.C util/surfjumper.C util/surfextractor.C)
set (CGAL_SRCS CGAL/src/Color.C CGAL/src/Double.C CGAL/src/Origin.C CGAL/src/aff_transformation_tags.C CGAL/src/assertions.C CGAL/src/io.C)
set (LIB_SRCS ${RFC_SRCS} ${CGAL_SRCS})
set (ALL_SRCS ${LIB_SRCS} ${TEST_SRCS} ${UTIL_SRCS})

add_library(Rocface ${LIB_SRCS})
target_link_libraries(Rocface Rocsurf Rocmap Rocsurf Roccom mpi_cxx)

target_include_directories(Rocface PUBLIC include include/Base include/Transfer include/Overlay CGAL/include)

add_executable(ex1 test/ex1.C test/meshio.C)
add_executable(ex2 test/ex2.C test/meshio.C)
add_executable(ex3 test/ex3.C test/meshio.C)
add_executable(ex4 test/ex4.C test/meshio.C)
add_executable(ex5 test/ex5.C test/meshio.C)
add_executable(ex6DataTransfer test/ex6DataTransfer.C test/meshio.C)
add_executable(plot3d test/plot3d.C test/meshio.C)
add_executable(reptrans test/reptrans.C test/meshio.C)
add_executable(readsdv test/readsdv.C test/meshio.C)

targets_link_libraries(ex1 ex2 ex3 ex4 ex5 ex6DataTransfer plot3d reptrans readsdv LIBRARIES Rocface)

add_executable(surfdiver util/surfdiver.C)
target_link_libraries(surfdiver Rocface)
add_executable(rfctest util/rfctest.C)
target_link_libraries(rfctest Rocface)
add_executable(autosurfer util/autosurfer.C)
target_link_libraries(autosurfer Rocface)
add_executable(surfjumper util/surfjumper.C)
target_link_libraries(surfjumper Rocface)
add_executable(surfextractor util/surfextractor.C)
target_link_libraries(surfextractor Rocface)

INSTALL(TARGETS Rocface surfdiver rfctest autosurfer surfjumper surfextractor RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
