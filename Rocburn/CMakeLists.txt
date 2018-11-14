project(Rocburn)

FILE (GLOB CORE_SRCS ./*.f90)
FILE (GLOB APN_SRCS  ./Rocburn_APN/*.f90)
FILE (GLOB ZN_SRCS   ./Rocburn_ZN/*.f90)
FILE (GLOB PY9_SRCS  ./Rocburn_PY/*.f90)
FILE (GLOB PYF_SRCS  ./Rocburn_PY/*.f)
SET(LIB_SRCS ${CORE_SRCS} ${APN_SRCS} ${ZN_SRCS} ${PY9_SRCS} ${PYF_SRCS})
SET(ALL_SRCS ${LIB_SRCS})

add_library(Rocburn ${LIB_SRCS})

target_link_libraries(Rocburn Rocout Rocin Roccomf Roccom metis mpi_fortran)

install_libraries(Rocburn)