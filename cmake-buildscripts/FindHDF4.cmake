# libhdf4 find module
#
# Variables: 
#   HDF4_INCLUDE_DIR - directory containing hdf.h
#   HDF4_LIBRARIES - libraries to link for hdf4
#   HDF4_FOUND - whether or not HDF4 was found

find_path(HDF4_INCLUDE_DIR NAMES hdf.h PATH_SUFFIXES hdf DOC "directory containing hdf.h")

find_library(HDF4_MFHDF_LIBRARY NAMES mfhdf DOC "Path to hdf4's mfhdf library.")
find_library(HDF4_DF_LIBRARY NAMES df DOC "Path to hdf4's df library.")

set(HDF4_LIBRARIES ${HDF4_MFHDF_LIBRARY} ${HDF4_DF_LIBRARY})

if(EXISTS "${HDF4_DF_LIBRARY}")
	# now check if the library we found actually works 
	set(CMAKE_REQUIRED_LIBRARIES ${HDF4_DF_LIBRARY})
	check_function_exists(Hopen HDF4_IS_LINKABLE)
	mark_as_advanced(HDF4_IS_LINKABLE)
else()
	set(HDF4_IS_LINKABLE FALSE)
endif()

mark_as_advanced(HDF4_INCLUDE_DIR HDF4_MFHDF_LIBRARY HDF4_DF_LIBRARY HDF4_IS_LINKABLE)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(HDF4 DEFAULT_MSG HDF4_INCLUDE_DIR HDF4_MFHDF_LIBRARY HDF4_DF_LIBRARY HDF4_IS_LINKABLE)
