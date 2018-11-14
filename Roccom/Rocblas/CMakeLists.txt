project(Rocblas)

set (BLASLIB_SRCS src/Rocblas.C src/axpy.C src/dots.C src/op2args.C src/op3args.C)
set (ALL_BLAS_SRCS "${BLASLIB_SRCS}")
set (TEST_SRCS test/blastest.C)

add_library(Rocblas ${BLASLIB_SRCS})

target_link_libraries(Rocblas Roccom mpi_cxx)
target_include_directories(Rocblas PUBLIC include)

add_executable(blastest ${TEST_SRCS})
target_link_libraries(blastest Rocblas)

install_libraries(Rocblas)
