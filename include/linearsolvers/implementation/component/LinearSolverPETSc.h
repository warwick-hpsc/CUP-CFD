/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the declarations for the LinearSolverInterface class
 */

#ifndef CUPCFD_LINEARSOLVERS_LINEAR_SOLVER_PETSC_INCLUDE_H
#define CUPCFD_LINEARSOLVERS_LINEAR_SOLVER_PETSC_INCLUDE_H

// Base Class
#include "LinearSolverInterface.h"

// Data Structures
#include "SparseMatrix.h"

// Error Codes
#include "Error.h"

// Parallel Communicator
#include "Communicator.h"

#include "LinearSolverPETScAlgorithm.h"


// Third party
#include "petsc.h"

namespace cupcfd
{
	namespace linearsolvers
	{
		template <class C, class I, class T>
		class LinearSolverPETSc : public LinearSolverInterface<C,I,T>
		{
			public:
				// === Members ===

				/** Number of Local Rows **/
				PetscInt mLocal;

				/** Matrix Object for this solver (sparse matrix) **/
				Mat a;

				/** Vector Object for this solver **/
				Vec x;

				/** Vector Object for this solver (results vector) **/
				Vec b;

				/** Row Index Range for Vector b across processes, indexed by rank **/
				const PetscInt * bRanges;

				/** Row Index Range for Vector x across processes, indexed by rank **/
				const PetscInt * xRanges;

				/** Row Index Range for Matrix A across processes, indexed by rank **/
				const PetscInt * aRanges;

				LinearSolverPETScAlgorithm * algSolver;

				// === Constructors/Deconstructors ===

				/**
				 * Create the linear solver object and setup the internal configuration and data structures
				 * (but not the data contents)
				 *
				 * The provided matrix is used to inform memory allocation based on its non-zero structure -
				 * its data contents are not transferred.
				 * In parallel setups, this matrix may be distributed and thus the matrix on each rank only
				 * contains a portion of the matrix that has local data. This will be handled for parallel
				 * scenarios such that the overall matrix is built correctly. However, the indexes in
				 * the matrix on each rank must be global to ensure the overall matrix is correctly reconstructed,
				 * and the total number of rows/columns set in the matrix must be the global amount.
				 *
				 * Matrix preallocation is done using the matrix provided, and as such the non-zero structure
				 * of matrices used to set values must not differ from the one provided here in the constructor.
				 *
				 * Solvers can be reused for new sets of matrix values as long as the matrices have the same
				 * non-zero structure (and thus avoid preallocation overheads). If subsequent matrices
				 * have different non-zero structures, they should be used to create a new LinearSolverPETSc object.
				 *
				 * This leaves the solver in a state where it is ready to have data set via the setValues
				 * functions before running the solve function
				 *
				 * @param comm The communicator to be used for the linear solve. If this is a serial
				 * linear solver, ensure that this communicator is of size 1.
				 * @param algorithm The identifier for which PETSc solver/preconditioner configuration to use
				 * @param rTol The rTolerance to use
				 * @param eTol The eTolerance to use
				 * @param matrix The matrix used to inform the non-zero data structure for memory allocation
				 *
				 * @tparam C The implementation class of the Sparse Matrix
				 * @tparam I The type of the indexing system
				 * @tparam T The data type of the matrix non-zero data
				 */
				LinearSolverPETSc(cupcfd::comm::Communicator& comm, PETScAlgorithm algorithm, T rTol, T eTol,
								  cupcfd::data_structures::SparseMatrix<C,I,T>& matrix);

				/**
				 * Deconstructor
				 */
				~LinearSolverPETSc();

				// === Overloaded Inherited Methods ===
				void resetVectorX();
				void resetVectorB();
				void resetMatrixA();
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes setValuesVectorX(T scalar);
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes setValuesVectorX(T * scalars, I nScalars, I * indexes, I nIndexes, I indexBase);
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes setValuesVectorB(T scalar);
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes setValuesVectorB(T * scalars, I nScalars, I * indexes, I nIndexes, I indexBase);
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes clearVectorX();
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes clearVectorB();

				/**
				 * Reset all data storage components of the object to an unset state.
				 * This does not affect the expected number of rows/columns or the communicator.
				 * It also does not affect the selection of PETSc algorithm.
				 *
				 * @tparam C The implementation class of the Sparse Matrix
				 * @tparam I The type of the indexing system
				 * @tparam T The data type of the matrix non-zero data
				 */
				void reset();
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes setupVectorX();
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes setupVectorB();
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes setupMatrixA(cupcfd::data_structures::SparseMatrix<C,I,T>& matrix);
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes setup(cupcfd::data_structures::SparseMatrix<C,I,T>& matrix);
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes setValuesMatrixA(cupcfd::data_structures::SparseMatrix<C,I,T>& matrix);
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getValuesVectorX(T ** result, I * nResult);
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getValuesVectorX(T ** result, I * nResult,
																	I * indexes, I nIndexes, I indexBase);
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getValuesVectorB(T ** result, I * nResult);
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getValuesVectorB(T ** result, I * nResult,
																	I * indexes, I nIndexes, I indexBase);
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getValuesMatrixA(cupcfd::data_structures::SparseMatrix<C,I,T>& matrix);
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes clearMatrixA();
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes solve();
		};
	}
}

#endif
