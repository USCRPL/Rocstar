project(Rocout)

IF(cgns_ENABLED)
  add_definitions ( -DUSE_CGNS )
ENDIF()

IF(cgns_ENABLED)
   set (ROCOUT_SRCS src/Rocout.C src/Rocout_hdf4.C src/write_parameter_file.C src/Rocout_cgns.C)
ELSE()
   set (ROCOUT_SRCS src/Rocout.C src/Rocout_hdf4.C src/write_parameter_file.C)
ENDIF()
set (TEST_SRCS test/outtest.C test/param_outtest.C)
set (UTIL_SRCS util/ghostbuster.C)

set (ALL_SRCS "${ROCOUT_SRCS} ${TEST_SRCS} ${UTIL_SRCS}")
set(LIB_SRCS ${ROCOUT_SRCS})
#set (TEST_SRCS test/maptest.C)

add_library(Rocout SHARED ${ROCOUT_SRCS})
IF(cgns_ENABLED)
  target_link_libraries(Rocout cgns)
ENDIF()
target_link_libraries(Rocout Rocin mpi_cxx)

target_include_directories(Rocout PUBLIC include)

# Test executables
add_executable(outtest test/outtest.C)
target_link_libraries(outtest Rocout)

# Utilities
IF(cgns_ENABLED)
  add_executable(ghostbuster util/ghostbuster.C)
  target_link_libraries(ghostbuster Rocout)
ENDIF()

install_libraries(Rocout)
IF(cgns_ENABLED)
  INSTALL(TARGETS ghostbuster RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
ENDIF()
