/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the LinearSolverPETScAlgorithm class
 */

#ifndef CUPCFD_LINEAR_SOLVER_PETSC_ALGORITHM_INCLUDE_H
#define CUPCFD_LINEAR_SOLVER_PETSC_ALGORITHM_INCLUDE_H

// Error Codes
#include "Error.h"

#include "Communicator.h"

// Third party
#include "petsc.h"

namespace cupcfd
{
	namespace linearsolvers
	{
		/**
		 * The selection of configurations available for use by the PETSc solver.
		 * Each represents a particular preconditioner/solver selection, combined with
		 * certain parameter values. This provides access to a selection of preconfigured settings.
		 *
		 * If a custom defined selection is desired that is not provided here,
		 * this can be set via the command line as per typical PETSc command line
		 * options by using PETSC_KSP_CMDLINE as the identifier.
		 */
		enum PETScAlgorithm
		{
			PETSC_KSP_CMDLINE,
			PETSC_KSP_CGAMG
		};

		/**
		 * Wrapper class for objects that determine the PETSc algorithm settings -
		 * i.e. the Solver, Preconditioner and any settings for these.
		 *
		 * A selection of preconfigured options are provided, otherwise the objects can
		 * be set up using the standard PETSc command line options if the appropriate
		 * identifier is selected when creating an object of this class.
		 */
		class LinearSolverPETScAlgorithm
		{
			public:
				// === Members ===
				/** PETSc Solver **/
				KSP petscSolver;

				/** PETSc Preconditioner **/
				PC  petscPrecon;

				/** PETSc Convergence Reason **/
				KSPConvergedReason petscReason;

				/** Solver/Preconditioner Selection Identifier **/
				PETScAlgorithm alg;

				/** **/
				PetscReal rTol;

				/** **/
				PetscReal eTol;

				// === Constructors/Deconstructors ===

				/**
				 *
				 */
				LinearSolverPETScAlgorithm(cupcfd::comm::Communicator& comm, PETScAlgorithm alg, PetscReal rTol, PetscReal eTol);

				/**
				 * Deconstructor
				 */
				~LinearSolverPETScAlgorithm();

				/**
				 * Setup the PETSc Solver Objects ready for solving
				 */
				cupcfd::error::eCodes solve(Mat * a, Vec * b, Vec * x);

				/**
				 * Setup the PETSc objects for using a CG Solver + AMG Preconditioner
				 */
				cupcfd::error::eCodes setupPETScCGAMG();

				/**
				 * Set the PETSc objects t use the configuration on the command line
				 */
				cupcfd::error::eCodes setupPETScCommandLine();
		};
	}
}

// Include Header Level Definitions
#include "LinearSolverPETScAlgorithm.ipp"

#endif
