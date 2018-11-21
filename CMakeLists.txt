cmake_minimum_required (VERSION 3.1)
project(Rocstar LANGUAGES NONE VERSION 1.0)

# configuration scripts 
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------

# initialization and include paths
include(cmake-buildscripts/AmberBuildSystemInit.cmake NO_POLICY_SCOPE)
cmake_policy(VERSION 3.1)

# now enable the languages
enable_language(C CXX Fortran)

include(AmberBuildSystem2ndInit)	

# use MPI
include(MPIConfig)

# use Amber compiler flags
include(CompilerFlags)
#...but, for our purposes we can always used the optimized flags
add_flags(C ${OPT_CFLAGS})
add_flags(CXX ${OPT_CXXFLAGS})
add_flags(Fortran ${OPT_FFLAGS})

# 3rd party tools
set(REQUIRED_3RDPARTY_TOOLS ) 
if(USE_FFT)
	list(APPEND REQUIRED_3RDPARTY_TOOLS fftw)
endif()

# whether blas and lapack are required
set(LINALG_LIBS_REQUIRED TRUE)
# programs where INTERNAL is a valid status
set(BUNDLED_3RDPARTY_TOOLS metis)
set(REQUIRED_3RDPARTY_TOOLS hdf4) 
include(3rdPartyTools)
include(NetlibConfig)

# global compile flags
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------

if(hdf4_ENABLED)
	add_definitions(-DUSE_HDF=1)
endif()

if(pthread_ENABLED)
	add_definitions(-DUSE_PTHREADS)
endif()

# detect glob support
check_all_includes(CXX glob.h)
if(NOT HAVE_GLOB_H)
	add_definitions(-D_NO_GLOB_)
endif()

if(STATIC)
	add_definitions(-DSTATIC_LINK=1)
endif()

# generate Fortran to C name-mangling macros
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include)
FortranCInterface_HEADER(${CMAKE_CURRENT_BINARY_DIR}/include/FC.h MACRO_NAMESPACE "FC_")
FortranCInterface_VERIFY(CXX)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)


# force executables to be saved to the lib and bin folders in the binary dir
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single directory for all executables.")
set (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib CACHE PATH "Single directory for all libraries and archives.")
mark_as_advanced (LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)

# Add testing support from IRAD
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------
INCLUDE(CTest)
IF(BUILD_TESTING)
  SET(BUILDNAME "" CACHE STRING "Name of build on the dashboard")
  MARK_AS_ADVANCED(BUILDNAME)
ENDIF(BUILD_TESTING)

# include IRAD common library
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------
add_subdirectory(IRAD)
SET(IRAD_INCPATH ${CMAKE_SOURCE_DIR}/IRAD/include)
SET(RUNTEST ${EXECUTABLE_OUTPUT_PATH}/runtest)
SET(TESTRESULT ${EXECUTABLE_OUTPUT_PATH}/testresults)
SET(IRAD_LIB IRAD)

# rocflu/rocflo optional part configuration
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------

set (ENABLE_TURB    TRUE CACHE BOOL "Rocflu/Rocflo: enable Turbulence")
set (ENABLE_STATS   TRUE CACHE BOOL "Rocflu/Rocflo: enable Statistics")
set (ENABLE_PLAG    TRUE CACHE BOOL "Rocflu/Rocflo: enable Lagrangian Particles Multiphase Flow")
set (ENABLE_PEUL    TRUE CACHE BOOL "Rocflu/Rocflo: enable Eulerian Multiphase Flow")
set (ENABLE_PERI    TRUE CACHE BOOL "Rocflu/Rocflo: enable Periodic")
set (ENABLE_INRT    TRUE CACHE BOOL "Rocflu/Rocflo: enable Particle Interaction")
set (ENABLE_RADI    FALSE CACHE BOOL "Rocflu/Rocflo: enable Radiation") # note: currently has compile errors and doesn't build
set (ENABLE_SPEC    TRUE CACHE BOOL "Rocflu/Rocflo: enable Species")

# add main subdirectories
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------

add_subdirectory(metis-4.0.3)
add_subdirectory(Roccom)
add_subdirectory(Rocface)
add_subdirectory(Rocprop)
add_subdirectory(Rocmop)
add_subdirectory(Rocon)
add_subdirectory(Rocman)
add_subdirectory(RocfluidMP)
add_subdirectory(Rocfrac)
add_subdirectory(Rocburn)
add_subdirectory(Rocprep)
add_subdirectory(testing)

# add top-level rocstar executable
#---------------------------------------------------------------------------------------------------------------------------------------------------------------------

add_executable(rocstar src/rocstar.C)
target_link_libraries(rocstar Rocman Roccomf Roccom ${IRAD_LIB} mpi_cxx)

INSTALL(TARGETS rocstar RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

# Add target to build the documentation

FIND_PACKAGE(Doxygen)
option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" ${DOXYGEN_FOUND})

IF(BUILD_DOCUMENTATION)

  IF(NOT DOXYGEN_FOUND)
    MESSAGE(FATAL_ERROR "Doxygen is needed to build the documentation.")
  ENDIF()
  
#  FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/doc DESTINATION
#  ${CMAKE_CURRENT_BINARY_DIR})
  
  
  SET( doxyfile_in          ${CMAKE_CURRENT_SOURCE_DIR}/Docs/Rocstar.cfg.in )
  SET( doxyfile             ${CMAKE_CURRENT_BINARY_DIR}/Rocstar.cfg        )
  SET( documentation        ${CMAKE_CURRENT_BINARY_DIR}/Docs                 )

  CONFIGURE_FILE( ${doxyfile_in} ${doxyfile} @ONLY )

  ADD_CUSTOM_COMMAND( OUTPUT ${documentation}
                      COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
                      MAIN_DEPENDENCY ${doxyfile} ${doxyfile_in}
                      COMMENT "Generating documentation")

  ADD_CUSTOM_TARGET( Docs DEPENDS ${documentation} )

  INSTALL( DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Docs DESTINATION
share/Rocstar )

  FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Docs/RocstarQuickStart.pdf DESTINATION
${CMAKE_CURRENT_BINARY_DIR}/Docs/)
  FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Docs/legacy DESTINATION
${CMAKE_CURRENT_BINARY_DIR}/Docs/)

ENDIF()

INCLUDE(CPack)

# Generate install rule to bundle and fix OS X dependencies
add_subdirectory(cmake-buildscripts/BundleOSXDependencies)

print_build_report()

