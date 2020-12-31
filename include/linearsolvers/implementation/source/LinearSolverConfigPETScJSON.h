/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Class Declaration for the LinearSolverConfigPETScJSON class.
 */

#ifndef CUPCFD_LINEARSOLVERS_SOURCE_LINEAR_SOLVER_CONFIG_PETSC_SOURCE_JSON_INCLUDE_H
#define CUPCFD_LINEARSOLVERS_SOURCE_LINEAR_SOLVER_CONFIG_PETSC_SOURCE_JSON_INCLUDE_H

// C++ Std Lib
#include <string>
#include <vector>

// Error Codes
#include "Error.h"

#include "LinearSolverConfigPETSc.h"
#include "LinearSolverPETScAlgorithm.h"

#include "LinearSolverConfigSource.h"

// JsonCPP - Supplied as standalone in include/io/jsoncpp
#include "json.h"
#include "json-forwards.h"

namespace cupcfd
{
	namespace linearsolvers
	{
		/**
		 * Defines a interface for accessing PETSc linear solver configuration
		 * options from a JSON data source.
		 *
		 * === Fields ===
		 *
		 * Required:
		 * Algorithm: String. Accepted Values: "CommandLine" , "CGAMG"
		 * Defines algorithmic behaviour to use for the PETSc linear solve
		 * CGAMG: Use the predefined CG solver with AMG preconditioner
		 * CommandLine: Parse the commandline for petsc solver options
		 *
		 * eTol: Floating Point Number.
		 * Define the value of the eTolerance to use for the solve.
		 *
		 * rTol: Floating point Number
		 * Define the value of the rTolerance to use for the solve.
		 *
		 * Optional:
		 * None
		 *
		 */
		template <class C, class I, class T>
		class LinearSolverConfigPETScJSON : public LinearSolverConfigSource<C,I,T>
		{
			public:
				// === Members ===

				/** Json Data Store containing fields for this JSON source **/
				Json::Value configData;

				// === Constructors/Deconstructors ===

				/**
				 * Parse the JSON record provided for values belonging to a PETSc Linear Solver entry
				 *
				 * @param parseJSON The contents of a JSON record with the appropriate fields
				 */
				LinearSolverConfigPETScJSON(Json::Value& parseJSON);

				/**
				 * Deconstructor
				 */
				~LinearSolverConfigPETScJSON();

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getPETScAlgorithm(PETScAlgorithm * solverAlg);

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getETol(T * eTol);

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getRTol(T * rTol);

				// === Concrete Methods ===

				/**
				 *
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes buildLinearSolverConfig(LinearSolverConfig<C,I,T> ** linearSolverConfig);

				// === Pure Virtual Methods ===
		};
	}
}

// Include Header Level Definitions
#include "LinearSolverConfigPETScJSON.ipp"

#endif
