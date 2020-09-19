/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the LinearSolverPETScAlgorithm class
 */

#include "LinearSolverPETScAlgorithm.h"

namespace cupcfd
{
	namespace linearsolvers
	{
		LinearSolverPETScAlgorithm::LinearSolverPETScAlgorithm(cupcfd::comm::Communicator& comm, PETScAlgorithm alg, PetscReal rTol, PetscReal eTol)
		:alg(alg),
		 rTol(rTol),
		 eTol(eTol)
		{
			// Create the initial 'empty' KSP object
			KSPCreate(comm.comm, &this->petscSolver);
			KSPSetTolerances(this->petscSolver, rTol, eTol, PETSC_DEFAULT, PETSC_DEFAULT);

			switch(this->alg)
			{
				case PETSC_KSP_CMDLINE:	setupPETScCommandLine();
										break;

				case PETSC_KSP_CGAMG:	setupPETScCGAMG();
										break;

				default:				setupPETScCommandLine();
										break;
			}
		}

		LinearSolverPETScAlgorithm::~LinearSolverPETScAlgorithm()
		{

		}

		cupcfd::error::eCodes LinearSolverPETScAlgorithm::solve(Mat * a, Vec * b, Vec * x)
		{
			// Final Assembly and Final KSP Setup
			if (MatAssemblyBegin(*a, MAT_FINAL_ASSEMBLY)) {
				return cupcfd::error::E_PETSC_ERROR;
			}

			if (MatAssemblyEnd(*a, MAT_FINAL_ASSEMBLY)) {
				return cupcfd::error::E_PETSC_ERROR;
			}

			if (KSPSetOperators(this->petscSolver, *a, *a)) {
				return cupcfd::error::E_PETSC_ERROR;
			}

			// Setup Internal Data Structures
			if (KSPSetUp(this->petscSolver)) {
				return cupcfd::error::E_PETSC_ERROR;
			}

			// Solve
			if (KSPSolve(this->petscSolver, *b, *x)) {
				return cupcfd::error::E_PETSC_ERROR;
			}
			if (KSPGetConvergedReason(this->petscSolver, &(this->petscReason))) {
				return cupcfd::error::E_PETSC_ERROR;
			}

			return cupcfd::error::E_SUCCESS;
		}

		cupcfd::error::eCodes LinearSolverPETScAlgorithm::setupPETScCommandLine()
		{
			if (KSPSetFromOptions(this->petscSolver)) {
				return cupcfd::error::E_PETSC_ERROR;
			}

			return cupcfd::error::E_SUCCESS;
		}

		cupcfd::error::eCodes LinearSolverPETScAlgorithm::setupPETScCGAMG()
		{
			if (KSPSetType(this->petscSolver, KSPCG)) {
				return cupcfd::error::E_PETSC_ERROR;
			}
			if (KSPGetPC(this->petscSolver, &this->petscPrecon)) {
				return cupcfd::error::E_PETSC_ERROR;
			}
			if (PCSetType(this->petscPrecon, PCGAMG)) {
				return cupcfd::error::E_PETSC_ERROR;
			}

			// Starting out with default values from
			// https://lists.mcs.anl.gov/pipermail/petsc-users/2015-June/025844.html

			// -pc_gamg_nsmooths
			if (PCGAMGSetNSmooths(this->petscPrecon, 1)) {
				return cupcfd::error::E_PETSC_ERROR;
			}

			// -pc_gamg_threshold
			PetscReal threshold = 0.02;
			if (PCGAMGSetThreshold(this->petscPrecon, &threshold, 1)) {
				return cupcfd::error::E_PETSC_ERROR;
			}
			
			return cupcfd::error::E_SUCCESS;
		}
	}
}
