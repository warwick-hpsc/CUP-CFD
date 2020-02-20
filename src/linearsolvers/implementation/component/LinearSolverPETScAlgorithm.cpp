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
				PETSC_KSP_CMDLINE:	setupPETScCommandLine();
									break;

				PETSC_KSP_CGAMG:	setupPETScCGAMG();
									break;

				default:			setupPETScCommandLine();
									break;
			}
		}

		LinearSolverPETScAlgorithm::~LinearSolverPETScAlgorithm()
		{

		}

		cupcfd::error::eCodes LinearSolverPETScAlgorithm::solve(Mat * a, Vec * b, Vec * x)
		{
			// Final Assembly and Final KSP Setup
			MatAssemblyBegin(*a, MAT_FINAL_ASSEMBLY);
			MatAssemblyEnd(*a, MAT_FINAL_ASSEMBLY);

			KSPSetOperators(this->petscSolver, *a, *a);

			// Setup Internal Data Structures
			KSPSetUp(this->petscSolver);

			// Solve
			KSPSolve(this->petscSolver, *b, *x);
			KSPGetConvergedReason(this->petscSolver, &(this->petscReason));

			return cupcfd::error::E_SUCCESS;
		}

		cupcfd::error::eCodes LinearSolverPETScAlgorithm::setupPETScCommandLine()
		{
			KSPSetFromOptions(this->petscSolver);
			return cupcfd::error::E_SUCCESS;
		}

		cupcfd::error::eCodes LinearSolverPETScAlgorithm::setupPETScCGAMG()
		{
			KSPSetType(this->petscSolver, KSPCG);
			KSPGetPC(this->petscSolver, &this->petscPrecon);
			PCSetType(this->petscPrecon, PCGAMG);

			// Starting out with default values from
			// https://lists.mcs.anl.gov/pipermail/petsc-users/2015-June/025844.html

			// -pc_gamg_nsmooths
			PetscInt aggNSmooths = 1;
			PCGAMGSetNSmooths(this->petscPrecon, 1);

			// -pc_gamg_threshold
			PetscReal threshold = 0.02;
			PCGAMGSetThreshold(this->petscPrecon, &threshold, 1);
		}
	}
}
