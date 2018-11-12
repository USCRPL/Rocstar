cmake_minimum_required(VERSION 2.8)
project(IRAD)

# Compiled objects output paths
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single directory for all executables.")
set (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib CACHE PATH "Single directory for all libraries and archives.")
mark_as_advanced (LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)
# Set up RPATH
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
INCLUDE(CTest)
IF(BUILD_TESTING)
  SET(BUILDNAME "${BUILDNAME}" CACHE STRING "Name of build on the dashboard")
  MARK_AS_ADVANCED(BUILDNAME)
ENDIF(BUILD_TESTING)

# Project-specific constructs
include_directories(include ${CMAKE_BINARY_DIR}/include )
find_package(MPI REQUIRED)

set (LIB_SOURCES src/ComLine.C src/Parameters.C src/Configuration.C src/Profiler.C src/UnixUtils.C)
set(UTIL_SOURCES src/TestUtil.C src/TestTCPInterface.C src/TestResults.C src/CheckResults.C src/RunTest.C src/profane.C src/DiffDataFiles.C)
IF(MPI_CXX_COMPILER)
#  set (CMAKE_CXX_COMPILER ${MPI_CXX_COMPILER})
  set (LIB_SOURCES ${LIB_SOURCES} src/COMM.C)
  IF(MPI_COMPILE_FLAGS)
    set_source_files_properties(${LIB_SOURCES} PROPERTIES COMPILE_FLAGS "${MPI_CXX_COMPILE_FLAGS}")
    set_source_files_properties(${UTIL_SOURCES} PROPERTIES COMPILE_FLAGS "${MPI_CXX_COMPILE_FLAGS}")
  ENDIF()
  add_definitions( -D_IRAD_MPI_ )
  include_directories(${MPI_INCLUDE_PATH})
ENDIF()
IF(APPLE)
  add_definitions(-DDARWIN)
ENDIF()


FILE(GLOB INC_FILES include/*.H)

add_library(IRAD SHARED ${LIB_SOURCES})
TARGET_LINK_LIBRARIES(IRAD ${MPI_CXX_LIBRARIES})
add_executable(iradutil_test src/TestUtil.C)
add_executable(tcpinterface_test src/TestTCPInterface.C)
add_executable(testresults src/TestResults.C)
add_executable(checkresults src/CheckResults.C)
add_executable(runtest src/RunTest.C)
add_executable(profane src/profane.C)
add_executable(diffdatafiles src/DiffDataFiles.C)
IF(MPI_LINK_FLAGS)
  SET_TARGET_PROPERTIES(IRAD iradutil_test tcpinterface_test testresults checkresults runtest
    profane diffdatafiles PROPERTIES LINK_FLAGS "${MPI_CXX_LINK_FLAGS}")
ENDIF()
target_link_libraries(profane IRAD ${MPI_CXX_LIBRARIES})
target_link_libraries(runtest IRAD ${MPI_CXX_LIBRARIES})
target_link_libraries(iradutil_test IRAD ${MPI_CXX_LIBRARIES})
target_link_libraries(checkresults IRAD ${MPI_CXX_LIBRARIES})
target_link_libraries(tcpinterface_test IRAD ${MPI_CXX_LIBRARIES})
target_link_libraries(diffdatafiles IRAD ${MPI_CXX_LIBRARIES})

ADD_TEST(IRAD::RunUtilTests ${EXECUTABLE_OUTPUT_PATH}/iradutil_test iradutil_testresults.txt)
ADD_TEST(IRAD::GetNextContent:CommentsAndWhiteSpace ${EXECUTABLE_OUTPUT_PATH}/testresults GetNextContent:CommentsAndWhiteSpace iradutil_testresults.txt)
ADD_TEST(IRAD::GetNextContent:StreamObject ${EXECUTABLE_OUTPUT_PATH}/testresults GetNextContent:StreamObject iradutil_testresults.txt)


INSTALL(FILES ${INC_FILES} DESTINATION include)
INSTALL(TARGETS IRAD checkresults testresults profane runtest diffdatafiles RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

# Add target to build the documentation

FIND_PACKAGE(Doxygen)
option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" ${DOXYGEN_FOUND})

IF(BUILD_DOCUMENTATION)

  IF(NOT DOXYGEN_FOUND)
    MESSAGE(FATAL_ERROR "Doxygen is needed to build the documentation.")
  ENDIF()
  
  SET( doxyfile_in          ${CMAKE_CURRENT_SOURCE_DIR}/doc/IRAD.cfg.in )
  SET( doxyfile             ${CMAKE_CURRENT_BINARY_DIR}/IRAD.cfg )
  SET( documentation        ${CMAKE_CURRENT_BINARY_DIR}/docs )

  CONFIGURE_FILE( ${doxyfile_in} ${doxyfile} @ONLY )

  ADD_CUSTOM_COMMAND( OUTPUT ${documentation}
                      COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
                      MAIN_DEPENDENCY ${doxyfile} ${doxyfile_in}
                      DEPENDS IRAD ${doxyfile} ${doxy_extra_files}
                      COMMENT "Generating documentation")

  ADD_CUSTOM_TARGET( docs DEPENDS ${documentation} )
  INSTALL( DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc DESTINATION share/IRAD )
else()
  message(STATUS "IRAD: not building Doxygen documentation")
ENDIF()
