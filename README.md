# Rocstar
Rocstar multiphysics simulation application

Rocstar is a multiphysics simulation application designed to do fluid-structure interaction (FSI) across moving, reacting interfaces. Rocstar couples multiple domain-specific simulation packages and disparately discretized domains and provides several simulation-supporting services including conservative and accurate data transfer, surface propagation, and parallel I/O. Rocstar is MPI parallel and routinely executes large simulations on massively parallel platforms. Rocstar was originally developed at the University of Illinois Center for Simulation of Advanced Rockets (CSAR) under Department of Energy ASCI program funding. Ongoing development of Rocstar is conducted by Illinois Rocstar LLC with company IR&D and continued DOE SBIR funding.

You can access Rocstar Doxygen at: http://illinoisrocstar.github.io/Rocstar-legacy/index.html

## Major changes in RPL fork
- Added support for building on a C++11 compiler
- Integrated metis, no more installing it manually.
- Support for building on Windows (though tests don't currently pass, I don't know enough to debug them)
- Cleaned up and rewrote the CMake build system to be less of a steaming pile that throws syntax errors at every opportunity

## Building this fork
1. Install C, C++, and Fortran compilers.
2. Install libhdf4, lapack/blas (or MKL), and (optionally) cgns.  Note: Metis is not needed.
3. Create a build dir and cd into it
4. Run `cmake .. -DCMAKE_INSTALL_PREFIX=<path/to/install/dir>` (No environment variables needed)
5. Run `make -j4`
6. Run `make -j4 install`
7. You can find rocstar in the install directory.  You might want to add it to your PATH.
8. Optionally, you can also run the tests with `ctest . --output-on-failure` from the build dir.


