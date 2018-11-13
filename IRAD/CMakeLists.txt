
project(IRAD)

set (LIB_SOURCES src/ComLine.C src/Parameters.C src/Configuration.C src/Profiler.C src/UnixUtils.C)
set(UTIL_SOURCES src/TestUtil.C src/TestTCPInterface.C src/TestResults.C src/CheckResults.C src/RunTest.C src/profane.C src/DiffDataFiles.C)

if(MPI)
	list(APPEND LIB_SOURCES src/COMM.C)
endif()

IF(APPLE)
  add_definitions(-DDARWIN)
ENDIF()

FILE(GLOB INC_FILES include/*.H)

add_library(IRAD SHARED ${LIB_SOURCES})
target_link_libraries(IRAD mpi_cxx)
target_include_directories(IRAD PUBLIC include ${CMAKE_CURRENT_BINARY_DIR}/include)

add_executable(iradutil_test src/TestUtil.C)
add_executable(tcpinterface_test src/TestTCPInterface.C)
add_executable(testresults src/TestResults.C)
add_executable(checkresults src/CheckResults.C)
add_executable(runtest src/RunTest.C)
add_executable(profane src/profane.C)
add_executable(diffdatafiles src/DiffDataFiles.C)

targets_link_libraries(iradutil_test tcpinterface_test testresults checkresults runtest profane diffdatafiles
	LIBRARIES IRAD)

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
