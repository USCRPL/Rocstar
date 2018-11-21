# libhdf4 find module
#
# Variables: 
#   HDF4_INCLUDE_DIRS - directories to include for hdf4
#   HDF4_LIBRARIES - libraries to link for hdf4
#   HDF4_FOUND - whether or not HDF4 was found

find_path(HDF4_INCLUDE_DIR NAMES hdf.h PATH_SUFFIXES hdf DOC "directory containing hdf.h")

find_library(HDF4_MFHDF_LIBRARY NAMES mfhdf-static DOC "Path to hdf4's mfhdf library.")
find_library(HDF4_DF_LIBRARY NAMES df df-static hdf hdf-static DOC "Path to hdf4's hdf library.")

set(HDF4_INCLUDE_DIRS ${HDF4_INCLUDE_DIR})
set(HDF4_LIBRARIES ${HDF4_MFHDF_LIBRARY} ${HDF4_DF_LIBRARY})

# hdf4 may also depend on libjpeg and zlib, so try to find and link them if they're installed
include(CMakeFindDependencyMacro)
find_dependency(JPEG)
if(JPEG_FOUND)
	list(APPEND HDF4_INCLUDE_DIRS ${JPEG_INCLUDE_DIR})
	list(APPEND HDF4_LIBRARIES ${JPEG_LIBRARIES})
endif()

find_dependency(ZLIB)
if(ZLIB_FOUND)
	list(APPEND HDF4_INCLUDE_DIRS ${ZLIB_INCLUDE_DIRS})
	list(APPEND HDF4_LIBRARIES ${ZLIB_LIBRARIES})
endif()

# also, on systems without XDR in the standard library, HDF4 builds its own xdr library, so link that if it exists
find_library(HDF4_XDR_LIBRARY NAMES xdr xdr-static)
if(HDF4_XDR_LIBRARY)
	list(APPEND HDF4_LIBRARIES ${HDF4_XDR_LIBRARY})
endif()

# ...and on Windows, hdf4 requires Winsock2
if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
	list(APPEND HDF4_LIBRARIES ws2_32)
endif()

if(HDF4_DF_LIBRARY)
	# now check if the library we found actually works 
	set(CMAKE_REQUIRED_LIBRARIES ${HDF4_LIBRARIES})
	check_function_exists(Hopen HDF4_IS_LINKABLE)
	mark_as_advanced(HDF4_IS_LINKABLE)
	set(CMAKE_REQUIRED_LIBRARIES "")
else()
	set(HDF4_IS_LINKABLE FALSE)
endif()

mark_as_advanced(HDF4_INCLUDE_DIR HDF4_MFHDF_LIBRARY HDF4_DF_LIBRARY HDF4_IS_LINKABLE)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(HDF4 DEFAULT_MSG HDF4_INCLUDE_DIR HDF4_MFHDF_LIBRARY HDF4_DF_LIBRARY HDF4_IS_LINKABLE)
