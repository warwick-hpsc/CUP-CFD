/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the declarations for the LinearSolverConfigPETSc class
 */

#ifndef CUPCFD_LINEARSOLVERS_LINEAR_SOLVER_CONFIG_PETSC_INCLUDE_H
#define CUPCFD_LINEARSOLVERS_LINEAR_SOLVER_CONFIG_PETSC_INCLUDE_H

#include "Error.h"
#include "LinearSolverPETSc.h"
#include "LinearSolverPETScAlgorithm.h"
#include "SparseMatrix.h"

#include "LinearSolverConfig.h"

namespace cupcfd
{
	namespace linearsolvers
	{
		/**
		 *
		 */
		template <class C, class I, class T>
		class LinearSolverConfigPETSc : public LinearSolverConfig<C,I,T>
		{
			public:
				// === Members ===

				/** Identify which PETSc Algorithm to use **/
				PETScAlgorithm solverAlg;

				/** **/
				T rTol;

				/** **/
				T eTol;

				// === Constructors/Deconstructors ===

				/**
				 *
				 */
				LinearSolverConfigPETSc(PETScAlgorithm solverAlg, T eTol, T rTol);

				/**
				 *
				 */
				LinearSolverConfigPETSc(const LinearSolverConfigPETSc<C,I,T>& source);

				/**
				 *
				 */
				~LinearSolverConfigPETSc();

				// === Methods ===

				void operator=(const LinearSolverConfigPETSc<C,I,T>& source);
				LinearSolverConfigPETSc<C,I,T> * clone();

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes buildLinearSolver(LinearSolverInterface<C,I,T> ** solverSystem,
															 cupcfd::data_structures::SparseMatrix<C,I,T>& matrix,
															 cupcfd::comm::Communicator& solverComm);

		};
	}
}

// Include Header Level Definitions
#include "LinearSolverConfigPETSc.ipp"

#endif
