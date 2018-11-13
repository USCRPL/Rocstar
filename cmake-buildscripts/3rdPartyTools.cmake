#  This file configures which 3rd party tools are built in and which are used from the system.
#  NOTE: must be included after WhichTools.cmake

message(STATUS "Checking whether to use built-in libraries...")


#List of 3rd party tools.
set(3RDPARTY_TOOLS
blas
lapack 
libm
mkl)

set(3RDPARTY_TOOL_USES
"for fundamental linear algebra calculations"                                     
"for fundamental linear algebra calculations"                                                                    
"for fundamental math routines if they are not contained in the C library"
"alternate implementation of lapack and blas that is tuned for speed")                                                  


#sets a tool to external, internal, or disabled
#STATUS=EXTERNAL, INTERNAL, or DISABLED
macro(set_3rdparty TOOL STATUS)
	set(${TOOL}_INTERNAL FALSE)
	set(${TOOL}_EXTERNAL FALSE)
	set(${TOOL}_DISABLED FALSE)
	set(${TOOL}_ENABLED TRUE)
	
	
	if(${STATUS} STREQUAL EXTERNAL)
		set(${TOOL}_EXTERNAL TRUE)
	elseif(${STATUS} STREQUAL INTERNAL)
		
		#the only way to get this message would be to use FORCE_INTERNAL_LIBS incorrectly, unless someone messed up somewhere
		if("${BUNDLED_3RDPARTY_TOOLS}" MATCHES ${TOOL})
				set(${TOOL}_INTERNAL TRUE)
		else()
			if(INSIDE_AMBER)
				# getting here means there's been a programming error
				message(FATAL_ERROR "3rd party program ${TOOL} is not bundled and cannot be built inside ${PROJECT_NAME}.")
			elseif("${REQUIRED_3RDPARTY_TOOLS}" MATCHES ${TOOL})
				# kind of a kludge - even when we're in a submodule, things will still get set top internal, so we just treat internal equal to disabled
				message(FATAL_ERROR "3rd party program ${TOOL} is required, but was not found.")
			else()
				# we're in a submodule, and it's not required, so it's OK that the tool is not bundled
				set(${TOOL}_DISABLED TRUE)
				set(${TOOL}_ENABLED FALSE)
				
			endif()
		endif()
	
	else()
		list_contains(TOOL_REQUIRED ${TOOL} ${REQUIRED_3RDPARTY_TOOLS})
				
		if(TOOL_REQUIRED)
			message(FATAL_ERROR "3rd party program ${TOOL} is required to build Amber, but it is disabled.")
		endif()
		
		set(${TOOL}_DISABLED TRUE)
		set(${TOOL}_ENABLED FALSE)
		
	endif()	
endmacro(set_3rdparty)

#------------------------------------------------------------------------------
#  OS threading library (not really a 3rd party tool)
#------------------------------------------------------------------------------
set(CMAKE_THREAD_PREFER_PTHREAD TRUE) #Yeah, we're biased.
find_package(Threads)

# first, figure out which tools we need
# -------------------------------------------------------------------------------------------------------------------------------

# if NEEDED_3RDPARTY_TOOLS is not passed in, assume that all of them are needed
if(NOT DEFINED NEEDED_3RDPARTY_TOOLS)
	set(NEEDED_3RDPARTY_TOOLS "${3RDPARTY_TOOLS}")
endif()

if(NOT DEFINED BUNDLED_3RDPARTY_TOOLS)
	set(BUNDLED_3RDPARTY_TOOLS "")
endif()

foreach(TOOL ${3RDPARTY_TOOLS})
	list(FIND NEEDED_3RDPARTY_TOOLS ${TOOL} TOOL_INDEX)
	
	test(NEED_${TOOL} NOT "${TOOL_INDEX}" EQUAL -1)
endforeach()

if(("${NEEDED_3RDPARTY_TOOLS}" MATCHES "mkl" OR "${NEEDED_3RDPARTY_TOOLS}" MATCHES "blas" OR "${NEEDED_3RDPARTY_TOOLS}" MATCHES "lapack")
	AND NOT ("${NEEDED_3RDPARTY_TOOLS}" MATCHES "mkl" AND "${NEEDED_3RDPARTY_TOOLS}" MATCHES "blas" AND "${NEEDED_3RDPARTY_TOOLS}" MATCHES "lapack"))
	message(FATAL_ERROR "If any of mkl, blas, and lapack are put into NEEDED_3RDPARTY_TOOLS, them you must put all of them in since mkl replaces blas and lapack")
endif()

# 1st pass checking
# -------------------------------------------------------------------------------------------------------------------------------

#------------------------------------------------------------------------------
#  MKL (near the top because it contains lapack, and blas)
#------------------------------------------------------------------------------

if(NEED_mkl)
	set(MKL_MULTI_THREADED ${OPENMP})
	
	set(MKL_NEEDINCLUDES FALSE)
	set(MKL_NEEDEXTRA FALSE)
	
	# Static MKL is not supported at this time.
	# <long_explanation>
	# MKL has a fftw3 compatibility interface.  Wierdly enough, this interface is spread out between several different libraries: the main interface library, the 
	# cdft library, and the actual fftw3 interface library (which is distributed as source code, not a binary).
	# So, even though we don't use the fftw3 interface, there are symbols in the main MKL libraries which conflict with the symbols from fftw3.
	# Oddly, on many platforms, the linker handles this fine.  However, in at least one case (the SDSC supercomputer Comet, running a derivative of CentOS),
	# ld balks at this multiple definition, and refuses to link programs which use MKL and fftw, but ONLY when BOTH of them are built as static libraries.
	# Why this is, I'm not sure.  I do know that it's better to build fftw3 as static and use mkl as shared (because mkl is a system library)
	# then the other way around, so that's what I do
	# </long_explanation>
	set(MKL_STATIC FALSE)
	find_package(MKL)
	
	if(MKL_FOUND)
		set_3rdparty(mkl EXTERNAL)
	else()
		set_3rdparty(mkl DISABLED)
	endif()
endif()

#------------------------------------------------------------------------------
#  Netlib libraries
#------------------------------------------------------------------------------

if(NEED_blas) # because of the earlier check, we can be sure that NEED_blas == NEED_lapack

	# this calls FindBLAS
	find_package(LAPACKFixed)
	
	if(BLAS_FOUND)
		set_3rdparty(blas EXTERNAL)
	else()
		set_3rdparty(blas INTERNAL)
	endif()
	
	if(LAPACK_FOUND)
		set_3rdparty(lapack EXTERNAL)
	else()
		set_3rdparty(lapack INTERNAL)
	endif()
endif()

if(NEED_arpack)
	#  ARPACK
	find_package(ARPACK)
	
	if(ARPACK_FOUND)
		set_3rdparty(arpack EXTERNAL)
	else()
		set_3rdparty(arpack INTERNAL)
	endif()
endif()

#------------------------------------------------------------------------------
#  Math library
#------------------------------------------------------------------------------ 

if(NEED_libm)
	# figure out if we need a math library
	# we actually need to be a little careful here, because most of the math.h functions are defined as GCC intrinsics, so check_function_exists() might not find them.
	# So, we use this c source instead
	set(CMAKE_REQUIRED_LIBRARIES "")
	set(CHECK_SINE_C_SOURCE "#include <math.h>
	
	int main(int argc, char** args)
	{
		return sin(argc - 1);
	}")
	
	check_c_source_compiles("${CHECK_SINE_C_SOURCE}" STDLIB_HAVE_SIN)
	
	if(STDLIB_HAVE_SIN)
		message(STATUS "Found math library functions in standard library.")
		set(LIBM "")
	else()
		find_library(LIBM NAMES m math libm)
		if(LIBM)
			set(CMAKE_REQUIRED_LIBRARIES ${LIBM})
			check_c_source_compiles("${CHECK_SINE_C_SOURCE}" LIBM_HAVE_SIN)
	
			if(LIBM_HAVE_SIN)
				message(STATUS "Found math library functions in math library \"${LIBM}\".")
			else()
				# Cause the try_compile to be retried
				unset(LIBM_HAVE_SIN CACHE)
				message(FATAL_ERROR "Could not find math functions in the standard library or the math library \"${LIBM}\".  Please set the LIBM variable to point to the library containing these functions.")
			endif()
		else()
			message(FATAL_ERROR "Could not find math functions in the standard library.  Please set the LIBM variable to point to the library containing these functions.")
		endif()
	endif()
	
	if(NOT STDLIB_HAVE_SIN)
		list(APPEND REQUIRED_3RDPARTY_TOOLS libm)
	endif()
			
	if(LIBM AND NOT STDLIB_HAVE_SIN)
		set_3rdparty(libm EXTERNAL)
	else()
		set_3rdparty(libm DISABLED)
	endif()
endif()

# Apply user overrides
# -------------------------------------------------------------------------------------------------------------------------------------------------------

set(FORCE_EXTERNAL_LIBS "" CACHE STRING "3rd party libraries to force using the system version of. Accepts a semicolon-seperated list of library names from the 3rd Party Libraries section of the build report.")
set(FORCE_INTERNAL_LIBS "" CACHE STRING "3rd party libraries to force to build inside Amber. Accepts a semicolon-seperated list of library names from the 3rd Party Libraries section of the build report.")
set(FORCE_DISABLE_LIBS "" CACHE STRING "3rd party libraries to force Amber to not use at all. Accepts a semicolon-seperated list of library names from the 3rd Party Libraries section of the build report.")

foreach(TOOL ${FORCE_EXTERNAL_LIBS})
	colormsg(YELLOW "Forcing ${TOOL} to be sourced externally")

	list_contains(VALID_TOOL ${TOOL} ${3RDPARTY_TOOLS})
	
	if(NOT VALID_TOOL)
		message(FATAL_ERROR "${TOOL} is not a valid 3rd party library name.")
	endif()
	
	set_3rdparty(${TOOL} EXTERNAL)
endforeach()

foreach(TOOL ${FORCE_INTERNAL_LIBS})
	colormsg(GREEN "Forcing ${TOOL} to be built internally")

	list_contains(VALID_TOOL ${TOOL} ${3RDPARTY_TOOLS})
	
	if(NOT VALID_TOOL)
		message(FATAL_ERROR "${TOOL} is not a valid 3rd party library name.")
	endif()
	
	set_3rdparty(${TOOL} INTERNAL)
endforeach()

foreach(TOOL ${FORCE_DISABLE_LIBS})
	colormsg(HIRED "Forcing ${TOOL} to be disabled")

	list_contains(VALID_TOOL ${TOOL} ${3RDPARTY_TOOLS})
	
	if(NOT VALID_TOOL)
		message(FATAL_ERROR "${TOOL} is not a valid 3rd party library name.")
	endif()
	
	set_3rdparty(${TOOL} DISABLED)
endforeach()

# force all unneeded tools to be disabled
foreach(TOOL ${3RDPARTY_TOOLS})
	list(FIND NEEDED_3RDPARTY_TOOLS ${TOOL} TOOL_INDEX)
	
	if(${TOOL_INDEX} EQUAL -1)
		set_3rdparty(${TOOL} DISABLED)
	endif()
	
endforeach()

# check math library configuration
if(LINALG_LIBS_REQUIRED AND NOT (mkl_ENABLED OR (blas_ENABLED AND lapack_ENABLED)))
	message(FATAL_ERROR "You must enable a linear algebra library -- either blas and lapack, or mkl")
endif()

if(mkl_ENABLED AND (blas_ENABLED AND lapack_ENABLED))
	# prefer MKL to BLAS
	set_3rdparty(blas DISABLED)
	set_3rdparty(lapack DISABLED)
endif()

# Now that we know which libraries we need, set them up properly.
# -------------------------------------------------------------------------------------------------------------------------------------------------------

#------------------------------------------------------------------------------
#  MKL
#------------------------------------------------------------------------------


if(mkl_ENABLED)
	if(NOT MKL_FOUND)
		message(FATAL_ERROR "You enabled MKL, but it was not found.")
	endif()
	
	if(MIXING_COMPILERS AND OPENMP)
		message(WARNING "You are using different compilers from different vendors together.  This may cause link errors with MKL and OpenMP.  There is no way around this.")
	endif()
	
	if(mkl_ENABLED AND (blas_ENABLED OR lapack_ENABLED))
		message(FATAL_ERROR "MKL replaces blas and lapack!  They can't be enabled when MKL is in use!")
	endif()
	
	# add to library tracker
	import_libraries(mkl LIBRARIES ${MKL_FORTRAN_LIBRARIES} INCLUDES ${MKL_INCLUDE_DIRS})
endif()

#------------------------------------------------------------------------------
#  Netlib libraries
#------------------------------------------------------------------------------

# BLAS
if(blas_INTERNAL)
	list(APPEND 3RDPARTY_SUBDIRS blas)
elseif(blas_EXTERNAL)
	import_libraries(blas LIBRARIES "${BLAS_LIBRARIES}")
endif()

#  LAPACK
if(lapack_INTERNAL)
	list(APPEND 3RDPARTY_SUBDIRS lapack)
elseif(lapack_EXTERNAL)
	import_libraries(lapack LIBRARIES "${LAPACK_LIBRARIES}")
endif()


#  ARPACK
if(arpack_EXTERNAL)
	if(NOT EXISTS "${ARPACK_LIBRARY}")
		message(FATAL_ERROR "arpack was set to be sourced externally, but it was not found!")
	endif()
	
	import_library(arpack ${ARPACK_LIBRARY})

	if(NOT ARPACK_HAS_ARSECOND)
		message(STATUS "System arpack is missing the arsecond_ function.  That function will be built inside amber")
	endif()

elseif(arpack_INTERNAL)
	list(APPEND 3RDPARTY_SUBDIRS arpack)
endif()

#------------------------------------------------------------------------------
#  Math library
#------------------------------------------------------------------------------ 
if(libm_EXTERNAL)	
	import_library(libm ${LIBM})
endif()
