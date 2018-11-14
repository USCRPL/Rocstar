project(Rocman)

set (LIB_SRCS src/rocstar_driver.C src/Action.C src/Scheduler.C src/Coupling.C src/Agent.C src/SolidAgent.C 
              src/BurnAgent.C src/basic_actions.C src/transfer_actions.C src/Interpolate.C src/builtin_couplings.C
              src/derived_couplings.C src/Surfdiver.C src/FluidAgent.C)
set (ALL_SRCS ${LIB_SRCS})

add_library(Rocman ${LIB_SRCS})

target_link_libraries(Rocman Roccom mpi_cxx)
target_include_directories(Rocman PUBLIC include)

INSTALL(TARGETS Rocman RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
