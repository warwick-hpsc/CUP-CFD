/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Class Declaration for the LinearSolverConfigSource class.
 */

#ifndef CUPCFD_LINEARSOLVERS_SOURCE_LINEAR_SOLVER_CONFIG_SOURCE_INCLUDE_H
#define CUPCFD_LINEARSOLVERS_SOURCE_LINEAR_SOLVER_CONFIG_SOURCE_INCLUDE_H

#include "LinearSolverConfig.h"

namespace cupcfd
{
	namespace linearsolvers
	{
		/**
		 */
		template <class C, class I, class T>
		class LinearSolverConfigSource
		{
			public:

				/**
				 */
				LinearSolverConfigSource();

				/**
				 * Deconstructor
				 */
				virtual ~LinearSolverConfigSource();

				// === Concrete Methods ===

				/**
				 *
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes buildLinearSolverConfig(LinearSolverConfig<C,I,T> ** linearSolverConfig) = 0;

				// === Pure Virtual Methods ===
		};
	}
}

// Include Header Level Definitions
#include "LinearSolverConfigSource.ipp"

#endif
