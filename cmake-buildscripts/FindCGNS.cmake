# CGNS (CFD General Notation System) library find module
#
# Variables: 
#   CGNS_INCLUDE_DIRS - directory containing cgnslib.h
#   CGNS_LIBRARIES - libraries to link for cgns
#   CGNS_FOUND - whether or not CGNS was found

find_path(CGNS_INCLUDE_DIR NAMES cgnslib.h DOC "directory containing cgnslib.h")

find_library(CGNS_LIBRARY NAMES cgns DOC "Path to the CGNS library.")

# handle HDF5 dependency
include(CMakeFindDependencyMacro)
find_dependency(HDF5)

set(CGNS_LIBRARIES ${CGNS_LIBRARY} ${HDF5_LIBRARIES})
set(CGNS_INCLUDE_DIRS ${CGNS_INCLUDE_DIR} ${HDF5_INCLUDE_DIRS})

if(HDF5_FOUND AND EXISTS "${CGNS_LIBRARY}")
	# now check if the library we found actually works 
	set(CMAKE_REQUIRED_LIBRARIES ${CGNS_LIBRARIES})
	check_function_exists(cgio_cleanup CGNS_IS_LINKABLE)
	mark_as_advanced(CGNS_IS_LINKABLE)
else()
	set(CGNS_IS_LINKABLE FALSE)
endif()

mark_as_advanced(CGNS_INCLUDE_DIR CGNS_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CGNS DEFAULT_MSG CGNS_LIBRARY CGNS_INCLUDE_DIR HDF5_FOUND CGNS_IS_LINKABLE)
