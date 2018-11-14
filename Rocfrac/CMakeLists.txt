project(Rocfrac)

set (ROCSTAR TRUE)

FILE (GLOB LIBFRAC_SRCS Source/*.f90)

LIST (APPEND LIBFRAC_SRCS Source/vinci_lass.c)
FILE (GLOB PREP_SRCS utilities/RocfracPrep/*.f90)
SET(LIB_SRCS ${LIBFRAC_SRCS})
SET(UTIL_SRCS ${PREP_SRCS})
SET(ALL_SRCS ${LIB_SRCS} ${UTIL_SRCS})

add_library(Rocfrac ${LIB_SRCS})
target_link_libraries(Rocfrac Rocout Rocin Roccomf Roccom Rocman)

ADD_EXECUTABLE(rfracprep ${PREP_SRCS})
set_property(TARGET rfracprep PROPERTY LINKER_LANGUAGE Fortran)
TARGET_LINK_LIBRARIES(rfracprep Rocout Rocin Roccomf Roccom metis)

INSTALL(TARGETS Rocfrac rfracprep RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
