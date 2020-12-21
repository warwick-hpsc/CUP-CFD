/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the declarations for the LinearSolverConfig class
 */

#ifndef CUPCFD_LINEARSOLVERS_LINEAR_SOLVER_CONFIG_INCLUDE_H
#define CUPCFD_LINEARSOLVERS_LINEAR_SOLVER_CONFIG_INCLUDE_H

#include "Error.h"
#include "SparseMatrix.h"
#include "LinearSolverInterface.h"
#include "Communicator.h"

namespace cupcfd
{
	namespace linearsolvers
	{
		/**
		 *
		 */
		template <class C, class I, class T>
		class LinearSolverConfig
		{
			public:
				// === Members ===

				// === Constructors/Deconstructors ===

				/**
				 *
				 */
				LinearSolverConfig();

				/**
				 *
				 */
				LinearSolverConfig(const LinearSolverConfig<C,I,T>& source);

				/**
				 *
				 */
				virtual ~LinearSolverConfig();

				// === Methods ===

				virtual LinearSolverConfig<C,I,T> * clone() = 0;

				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes buildLinearSolver(LinearSolverInterface<C,I,T> ** solverSystem,
															 cupcfd::data_structures::SparseMatrix<C,I,T>& matrix,
															 cupcfd::comm::Communicator& solverComm) = 0;

		};
	}
}

// Include Header Level Definitions
#include "LinearSolverConfig.ipp"

#endif
