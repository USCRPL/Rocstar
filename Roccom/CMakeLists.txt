project(Roccom)

# configure dl-config.h
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include)
configure_file(include/dl-config.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/dl-config.h)

find_library(DL_LIB dl)

set (LIB_SRCS src/roccom_c.C src/assertion.C src/roccom_exception.C src/commpi.C src/Roccom_base.C src/Attribute.C src/Connectivity.C src/Window.C src/Pane.C src/Element_accessors.C)
set (FLIB_SRCS src/roccom_f.C src/m_pointers.f90 src/utilities.f90)
set (ALL_SRCS "${LIB_SRCS} ${FLIB_SRCS}")

add_library(Roccom ${LIB_SRCS})
add_library(Roccomf ${FLIB_SRCS})

target_link_libraries(Roccom ${DL_LIB} mpi_cxx)
target_link_libraries(Roccomf Roccom mpi_fortran)

target_include_directories(Roccom PUBLIC include ${CMAKE_CURRENT_BINARY_DIR}/include)
target_include_directories(Roccomf PUBLIC include ${CMAKE_CURRENT_BINARY_DIR}/include)

add_subdirectory(Rocblas)
add_subdirectory(Rocin)
add_subdirectory(Rocout)
add_subdirectory(Rocmap)
add_subdirectory(Rocsurf)

install_libraries(Roccom Roccomf)
