project(Mesquite)

set(MESQUITE_SOURCES
	src/QualityMetric/AddQualityMetric.cpp
	src/QualityMetric/Smoothness/ASMQualityMetric.cpp
	src/QualityMetric/Shape/AspectRatioGammaQualityMetric.cpp
	src/ObjectiveFunction/CompositeOFAdd.cpp
	src/ObjectiveFunction/CompositeOFMultiply.cpp
	src/ObjectiveFunction/CompositeOFScalarAdd.cpp
	src/ObjectiveFunction/CompositeOFScalarMultiply.cpp
	src/QualityMetric/Shape/ConditionNumberQualityMetric.cpp
	src/QualityImprover/VertexMover/ConjugateGradient/ConjugateGradient.cpp
	src/QualityMetric/Volume/CornerJacobianQualityMetric.cpp
	src/Mesh/CornerTag.cpp
	src/QualityMetric/Smoothness/EdgeLengthQualityMetric.cpp
	src/QualityMetric/Smoothness/EdgeLengthRangeQualityMetric.cpp
	src/Misc/Exponent.cpp
	src/QualityImprover/VertexMover/FeasibleNewton/FeasibleNewton.cpp
	src/Misc/FileTokenizer.cpp
	src/QualityMetric/Shape/GeneralizedConditionNumberQualityMetric.cpp
	src/QualityMetric/Shape/IdealWeightInverseMeanRatio.cpp
	src/QualityMetric/Shape/IdealWeightMeanRatio.cpp
	src/QualityMetric/DFT/I_DFT.cpp
	src/Control/InstructionQueue.cpp
	src/QualityImprover/VertexMover/LaplacianSmoothers/LaplacianSmoother.cpp
	src/ObjectiveFunction/LInfTemplate.cpp
	src/QualityMetric/Volume/LocalSizeQualityMetric.cpp
	src/ObjectiveFunction/LPtoPTemplate.cpp
	src/TargetCalculator/LVQDTargetCalculator.cpp
	src/ObjectiveFunction/MaxTemplate.cpp
	src/QualityImprover/VertexMover/MeanMidNodeMover.cpp
	src/Mesh/MeshImpl.cpp
	src/Mesh/MeshImplData.cpp
	src/Mesh/MeshImplTags.cpp
	src/Mesh/MeshSet.cpp
	src/Misc/MeshTransform.cpp
	src/Misc/MesquiteVersion.cpp
	src/Misc/MsqDebug.cpp
	src/Misc/MsqError.cpp
	src/Misc/MsqFPE.cpp
	src/Misc/MsqHessian.cpp
	src/Misc/MsqInterrupt.cpp
	src/Mesh/MsqMeshEntity.cpp
	src/Misc/MsqTimer.cpp
	src/Mesh/MsqVertex.cpp
	src/QualityMetric/MultiplyQualityMetric.cpp
	src/QualityImprover/VertexMover/NonSmoothSteepestDescent/NonSmoothSteepestDescent.cpp
	src/ObjectiveFunction/ObjectiveFunction.cpp
	src/Mesh/PatchData.cpp
	src/Misc/PlanarDomain.cpp
	src/QualityMetric/PowerQualityMetric.cpp
	src/QualityAssessor/QualityAssessor.cpp
	src/QualityImprover/QualityImprover.cpp
	src/QualityMetric/QualityMetric.cpp
	src/QualityImprover/VertexMover/Randomize/Randomize.cpp
	src/QualityMetric/DFT/RI_DFT.cpp
	src/QualityMetric/ScalarAddQualityMetric.cpp
	src/QualityMetric/ScalarMultiplyQualityMetric.cpp
	src/Control/Wrappers/ShapeImprovementWrapper.cpp
	src/QualityImprover/VertexMover/LaplacianSmoothers/SmartLaplacianSmoother.cpp
	src/Misc/SphericalDomain.cpp
	src/QualityMetric/DFT/sRI_DFT.cpp
	src/QualityImprover/VertexMover/SteepestDescent/SteepestDescent.cpp
	src/TargetCalculator/TargetCalculator.cpp
	src/Control/TerminationCriterion.cpp
	src/Mesh/TopologyInfo.cpp
	src/QualityImprover/TopologyModifier/TopologyModifier.cpp
	src/QualityMetric/Untangle/UntangleBetaQualityMetric.cpp
	src/Misc/Vector3D.cpp
	src/QualityMetric/Shape/VertexConditionNumberQualityMetric.cpp
	src/QualityImprover/VertexMover/VertexMover.cpp
	src/TargetCalculator/WTargetCalculator.cpp)
	
	
set(MESQUITE_INCLUDE_DIRECTORIES
	include
	src/Control
	src/Control/Wrappers
	src/Mesh
	src/Misc
	src/ObjectiveFunction
	src/QualityAssessor
	src/QualityImprover
	src/QualityImprover/TopologyModifier
	src/QualityImprover/VertexMover
	src/QualityImprover/VertexMover/ConjugateGradient
	src/QualityImprover/VertexMover/FeasibleNewton
	src/QualityImprover/VertexMover/LaplacianSmoothers
	src/QualityImprover/VertexMover/NonSmoothSteepestDescent
	src/QualityImprover/VertexMover/Randomize
	src/QualityImprover/VertexMover/SteepestDescent
	src/QualityMetric
	src/QualityMetric/DFT
	src/QualityMetric/Shape
	src/QualityMetric/Smoothness
	src/QualityMetric/Untangle
	src/QualityMetric/Volume
	src/TargetCalculator)


add_definitions(-DMESQUITE)

# configure config header
# -----------------------------------------------------------------------------

check_all_functions(vsprintf vsnprintf cbrt)

check_all_includes(CXX sys/times.h ctime)

test(HAVE_TIMES HAVE_SYS_TIMES_H)
test(HAVE_CLOCK HAVE_CTIME)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include)
configure_file(include/mesquite_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/mesquite_config.h)

# -----------------------------------------------------------------------------


add_library(mesquite_95 SHARED ${MESQUITE_SOURCES})
target_include_directories(mesquite_95 PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/include ${MESQUITE_INCLUDE_DIRECTORIES})

INSTALL(TARGETS mesquite_95 RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
