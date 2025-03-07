# This script sets up a target based on the math libraries currently configured:
# netlib -- linear algebra libraries (blas and lapack), arpack, and whatever supporting math libraries are necessary.
# do NOT link any of the netlib libraries directly except through the netlib target.  That will not work when MKL is enabled.  
# Also, make ABSOLUTELY sure that fftw is AHEAD OF netlib in the link order of any targets that use them together.
# This is so that it overrides MKL's weird partial fftw interface.  If you do not do this, you may get undefined behavior.

set(NETLIB_LIBRARIES "")

# basic linear algebra libraries
if(mkl_ENABLED)
    list(APPEND NETLIB_LIBRARIES ${MKL_FORTRAN_LIBRARIES})
    
   	#this affects all Amber programs
    add_definitions(-DMKL)
else()
	list(APPEND NETLIB_LIBRARIES blas lapack)
endif()

# link system math library if it exists
if(libm_ENABLED)
	list(APPEND NETLIB_LIBRARIES ${LIBM})
endif()

# --------------------------------------------------------------------

import_libraries(netlib LIBRARIES ${NETLIB_LIBRARIES})
