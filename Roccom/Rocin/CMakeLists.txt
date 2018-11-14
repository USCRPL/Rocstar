project(Rocin)

set (ENABLE_CGNS FALSE CACHE BOOL "Build with CGNS Support")

IF(ENABLE_CGNS)
  add_definitions ( -DUSE_CGNS )
ENDIF()


find_library(ZLIB z)
find_library(JPEG_LIB jpeg)
find_library(PTHREAD_LIB pthread)

set (ROCIN_SRCS src/Rocin.C src/read_parameter_file.C)
IF(NOT HAVE_GLOB_H)
  list (APPEND ROCIN_SRCS src/Directory.C)
ENDIF()

set (TEST_SRCS test/intest1.C test/intest2.C test/printin.C test/param_test.C)
set (UTIL_SRCS util/hdf2plt.C util/printfeas.C util/hdf2vtk util/hdf2pltV2 util/sepin util/plagprep)
set (RHDF4_SRCS src/HDF4.C)
IF(pthread_ENABLED)
 list(APPEND RHDF4_SRCS src/Sync.C)
ENDIF()

set (ALL_SRCS "${ROCIN_SRCS} ${TEST_SRCS} ${UTIL_SRCS} ${RHDF4_SRCS}")
set(LIB_SRCS ${ROCIN_SRCS})

include_directories(include)

add_library(Rocin ${ROCIN_SRCS})
add_library(RHDF4 ${RHDF4_SRCS})

target_link_libraries(RHDF4 Roccom hdf4 ${JPEG_LIB} ${ZLIB})
IF(cgns_ENABLED)
  target_link_libraries(Rocin cgns)
ENDIF()
target_link_libraries(Rocin Roccom RHDF4)
IF(pthread_ENABLED)
  targets_link_libraries(Rocin RHDF4 LIBRARIES Threads::Threads)
ENDIF()
target_link_libraries(Rocin mpi_cxx)

target_include_directories(RHDF4 PUBLIC include)
target_include_directories(Rocin PUBLIC include)

# Test executables
add_executable(intest1 test/intest1.C)
target_link_libraries(intest1 Rocin )
add_executable(intest2 test/intest2.C)
target_link_libraries(intest2 Rocin)
add_executable(printin test/printin.C)
target_link_libraries(printin Rocin)
add_executable(param_test test/param_test.C)
target_link_libraries(param_test Rocin)

# Utilities
add_executable(hdf2plt util/hdf2plt.C)
target_link_libraries(hdf2plt Rocin)
add_executable(printfeas util/printfeas.C)
target_link_libraries(printfeas Rocin)
add_executable(hdf2vtk util/hdf2vtk.C)
target_link_libraries(hdf2vtk Rocin)
#add_executable(hdf2pltV2 util/hdf2pltV2.C)
#target_link_libraries(hdf2pltV2 Rocin)
add_executable(sepin util/sepin.C)
target_link_libraries(sepin Rocin)
add_executable(plagprep util/plagprep.C)
target_link_libraries(plagprep Rocin)

INSTALL(TARGETS RHDF4 Rocin hdf2plt printfeas hdf2vtk sepin plagprep RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
