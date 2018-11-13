
project(Roccom)
ENABLE_LANGUAGE( CXX Fortran )
include(FortranCInterface)
FortranCInterface_HEADER(${PROJECT_SOURCE_DIR}/include/FC.h MACRO_NAMESPACE "FC_")
FortranCInterface_VERIFY(CXX QUIET)
set (BUILD_STATIC FALSE CACHE BOOL "Build static COM library")
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single directory for all executables.")
set (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib CACHE PATH "Single directory for all libraries and archives.")
mark_as_advanced (LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)

# configure dl-config.h
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include)
configure_file(include/dl-config.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/dl-config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

#INCLUDE(CTest)

find_library(DL_LIB dl)
set (LIB_SRCS src/roccom_c.C src/assertion.C src/roccom_exception.C src/commpi.C src/Roccom_base.C src/Attribute.C src/Connectivity.C src/Window.C src/Pane.C src/Element_accessors.C)
set (FLIB_SRCS src/roccom_f.C src/m_pointers.f90 src/utilities.f90)
set (ALL_SRCS "${LIB_SRCS} ${FLIB_SRCS}")
set_source_files_properties(${ALL_SRCS} PROPERTIES COMPILE_FLAGS "-fPIC" )
#set(TEST_SRCS src/COMTest.C)

# rpath settings
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


IF(NOT BUILD_STATIC)
  add_library(Roccom SHARED ${LIB_SRCS})
  add_library(Roccomf SHARED ${FLIB_SRCS})
ELSE()
  add_library(Roccom STATIC ${LIB_SRCS})
  add_library(Roccomf STATIC ${FLIB_SRCS})
  add_definitions( -DSTATIC_LINK )
ENDIF()
target_link_libraries(Roccom ${DL_LIB} mpi_cxx)
target_link_libraries(Roccomf Roccom mpi_fortran)

target_include_directories(Roccom PUBLIC include)
target_include_directories(Roccomf PUBLIC include)

add_subdirectory(Rocblas)
add_subdirectory(Rocin)
add_subdirectory(Rocout)
add_subdirectory(Rocmap)
add_subdirectory(Rocsurf)

INSTALL(TARGETS Roccom Roccomf RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
