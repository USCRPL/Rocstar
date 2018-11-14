project(Rocmap)

#set (MAPLIB_SRCS src/Rocmap.C src/Pane_boundary.C src/Pane_connectivity.C src/Pane_communicator.C 
#                 src/Dual_connectivity.C src/Simple_manifold_2.C src/Pane_ghost_connectivity.C src/KD_tree_3.C)
set (MAPLIB_SRCS src/Rocmap.C src/Pane_boundary.C src/Pane_connectivity.C src/Pane_communicator.C 
                 src/Dual_connectivity.C src/Simple_manifold_2.C src/Pane_ghost_connectivity.C)
set (UTIL_SRCS util/addpconn.C)
set (ALL_MAP_SRCS "${MAPLIB_SRCS} ${UTIL_SRCS}")

add_library(Rocmap ${MAPLIB_SRCS})
target_link_libraries(Rocmap Rocin Rocout Roccom mpi_cxx)

target_include_directories(Rocmap PUBLIC include)

add_executable(pconntest test/pconntest.C)
target_link_libraries(pconntest Rocmap)
add_executable(pcommtest test/pcommtest.C)
target_link_libraries(pcommtest Rocmap)
add_executable(pcommpartest test/pcommpartest.C)
target_link_libraries(pcommpartest Rocmap)
add_executable(bordertest_hex test/bordertest_hex.C)
target_link_libraries(bordertest_hex Rocmap)
add_executable(bordertestg_hex test/bordertestg_hex.C)
target_link_libraries(bordertestg_hex Rocmap)
add_executable(bordertest_struc test/bordertest_struc.C)
target_link_libraries(bordertest_struc Rocmap)
add_executable(addpconn util/addpconn.C)
target_link_libraries(addpconn Rocmap)

#ADD_TEST(RunAllTests ${EXECUTABLE_OUTPUT_PATH}/testx testresults.txt)
#ADD_TEST(TestObject:ValidHandle ${TEST_RESULTS} TestObject:ValidHandle testresults.txt)
#ADD_TEST(TestObject:Exists ${TEST_RESULTS} TestObject:Exists testresults.txt)

install_libraries(Rocmap)
