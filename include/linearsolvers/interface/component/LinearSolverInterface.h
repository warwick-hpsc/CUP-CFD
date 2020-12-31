/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the declarations for the LinearSolver class
 */

#ifndef CUPCFD_LINEARSOLVERS_LINEAR_SOLVER_INTERFACE_INCLUDE_H
#define CUPCFD_LINEARSOLVERS_LINEAR_SOLVER_INTERFACE_INCLUDE_H

#include "Error.h"
#include "SparseMatrix.h"
#include "Communicator.h"

namespace cupcfd
{
	namespace linearsolvers
	{
		/**
		 * Linear Solver Interface Class for setting up and running linear solver
		 * operations on sparse matrices.
		 *
		 * @tparam C The implementation class of the Sparse Matrix
		 * @tparam I The type of the indexing system
		 * @tparam T The data type of the matrix non-zero data
		 */
		template <class C, class I, class T>
		class LinearSolverInterface
		{
			public:
				// === Members ===

				/** Communicator to use **/
				cupcfd::comm::Communicator comm;

				/** Number of expected global rows in the matrix **/
				I mGlobal;

				/** Number of expected global columns in the matrix **/
				I nGlobal;

				// === Constructors/Deconstructors ===

				/**
				 * Define the characteristics of the Linear Solver.
				 *
				 * @param comm The communicator to be used for the linear solve. If this is a serial
				 * linear solver, ensure that this communicator is of size 1.
				 * @param nRows The number of expected rows of the linear solver matrix, size of the vectors
				 * @param nCols The number of expected columns of the linear solver matrix
				 *
				 * @tparam C The implementation class of the Sparse Matrix
				 * @tparam I The type of the indexing system
				 * @tparam T The data type of the matrix non-zero data
				 */
				LinearSolverInterface(cupcfd::comm::Communicator comm, I nRows, I nCols);

				/**
				 * Deconstructor.
				 *
				 * Frees any allocated pointers on the heap.
				 *
				 * @tparam C The implementation class of the Sparse Matrix
				 * @tparam I The type of the indexing system
				 * @tparam T The data type of the matrix non-zero data
				 */
				virtual ~LinearSolverInterface();

				// === Concrete Methods ===

				/**
				 * Set both of the tolerance values for this linear solver system
				 *
				 * @param rTol The r Tolerance
				 * @param eTol The e Tolerance
				 *
				 * @tparam C The implementation class of the Sparse Matrix
				 * @tparam I The type of the indexing system
				 * @tparam T The data type of the matrix non-zero data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				//cupcfd::error::eCodes setTolerances(T rTol, T eTol);

				/**
				 * Reset both of the tolerances to zero
				 *
				 * @tparam C The implementation class of the Sparse Matrix
				 * @tparam I The type of the indexing system
				 * @tparam T The data type of the matrix non-zero data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				//cupcfd::error::eCodes resetTolerances();

				// === Pure Virtual Methods ===

				/**
				 * Reset all data storage components of the object to an unset state.
				 * This does not affect the expected number of rows/columns or the communicator.
				 *
				 * @tparam C The implementation class of the Sparse Matrix
				 * @tparam I The type of the indexing system
				 * @tparam T The data type of the matrix non-zero data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes reset() = 0;

				/**
				 * Reset the vector X to an unconstructed/unset state. This should
				 * free any memory assigned to the Vector X.
				 *
				 * This is a collective operation in parallel setups.
				 *
				 * @tparam C The implementation class of the Sparse Matrix
				 * @tparam I The type of the indexing system
				 * @tparam T The data type of the matrix non-zero data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes resetVectorX() = 0;

				/**
				 * Reset the vector B to an unconstructed/unset state. This should
				 * free any memory assigned to the Vector B.
				 *
				 * This is a collective operation in parallel setups.
				 *
				 * @tparam C The implementation class of the Sparse Matrix
				 * @tparam I The type of the indexing system
				 * @tparam T The data type of the matrix non-zero data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes resetVectorB() = 0;

				/**
				 * Reset the matrix A to an unconstructed/unset state. This should
				 * free any memory assigned to the Matrix A.
				 *
				 * This is a collective operation in parallel setups.
				 *
				 * @tparam C The implementation class of the Sparse Matrix
				 * @tparam I The type of the indexing system
				 * @tparam T The data type of the matrix non-zero data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes resetMatrixA() = 0;

				/**
				 * Setup the Vector X so it is ready to accept values and suitable for a solve.
				 * Sizes are determined by the class member internal values
				 *
				 * @tparam C The implementation class of the Sparse Matrix
				 * @tparam I The type of the indexing system
				 * @tparam T The data type of the matrix non-zero data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_LINEARSOLVER_ROW_SIZE_UNSET The row size is an invalid size of 0 or less
				 * @retval cupcfd::error::E_LINEARSOLVER_INVALID_COMM The solver communicator is not PETSC_COMM_SELF
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes setupVectorX() = 0;

				/**
				 * Setup the vector B so it is ready to accept values and suitable for a solve.
				 * Sizes should be determined by the class member internal values
				 *
				 * @tparam C The implementation class of the Sparse Matrix
				 * @tparam I The type of the indexing system
				 * @tparam T The data type of the matrix non-zero data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_LINEARSOLVER_ROW_SIZE_UNSET The row size is an invalid size of 0 or less.
				 * @retval cupcfd::error::E_LINEARSOLVER_INVALID_COMM The solver communicator is not PETSC_COMM_SELF
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes setupVectorB() = 0;

				/**
				 * Setup the Matrix A so it is ready to accept values and suitable for a solve.
				 * Sizes are determined by the internal member values.
				 *
				 * Note: This method does not set the initial values of the matrix - this is left for
				 * setValues functions. It merely ensures that any internal matrix data structures are ready to accept data.
				 * Their construction may or may not be informed by the non-zero structure of the provided matrix, and thus
				 * the provided matrix should possess the same non-zero structure as that of any matrix provided for setting
				 * values. This also enables the reuse of the same solver/matrix structure for multiple solves with different
				 * data if the previous data is cleared/overwritten.
				 *
				 * @tparam C The implementation class of the Sparse Matrix
				 * @tparam I The type of the indexing system
				 * @tparam T The data type of the matrix non-zero data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes setupMatrixA(cupcfd::data_structures::SparseMatrix<C, I, T>& matrix) = 0;

				/**
				 * Setup the internal data structures and sizes based on the non-zero structure of the provided sparse matrix.
				 * This should leave any vectors and or internal matrix structures ready for values to be set.
				 * Behaviour is undefined if the values are set at positions that were not treated as non-zero in the SparseMatrix.
				 *
				 * @param matrix The matrix containing the non-zero structure. In parallel environments, this may contain only a portion
				 * of the overall matrix, in which case the combined matrices across the ranks defines the non-zero structure of the total
				 * matrix. Some of the non-zero structure for this rank may or may not be defined by other ranks.
				 *
				 * @tparam C The implementation class of the Sparse Matrix
				 * @tparam I The type of the indexing system
				 * @tparam T The data type of the matrix non-zero data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes setup(cupcfd::data_structures::SparseMatrix<C, I, T>& matrix) = 0;

				/**
				 * Set every index of the vector X to a specific scalar value.
				 *
				 * Note: This is a collective operation in parallel setups
				 *
				 * @param scalar The scalar to set each index value to.
				 *
				 * @tparam C The implementation class of the Sparse Matrix
				 * @tparam I The type of the indexing system
				 * @tparam T The data type of the matrix non-zero data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_LINEARSOLVER_INVALID_VECTOR The X vector has not been setup
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes setValuesVectorX(T scalar) = 0;

				/**
				 * Set the specified row indexes in the vector X to the values stored in the scalars array.
				 *
				 * @param scalars The array of scalars to copy into the vector.
				 * @param nScalars The size of the scalars array
				 * @param indexes The array of global indexes, detailing which positions in the vector to update
				 * @param nIndexes The size of the indexes array
				 * @param indexBase The base of the indexing scheme used in indexes (e.g indexed from 0 or 1)
				 *
				 * @tparam C The implementation class of the Sparse Matrix
				 * @tparam I The type of the indexing system
				 * @tparam T The data type of the matrix non-zero data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_LINEARSOLVER_INVALID_VECTOR The X vector has not been setup
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes setValuesVectorX(T * scalars, I nScalars,
																	I * indexes, I nIndexes, I indexBase) = 0;

				/**
				 * Set every index of the vector B to a specific scalar value.
				 *
				 * Note: This is a collective operation.
				 *
				 * @param scalar The scalar to set each index value to.
				 *
				 * @tparam C The implementation class of the Sparse Matrix
				 * @tparam I The type of the indexing system
				 * @tparam T The data type of the matrix non-zero data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_LINEARSOLVER_INVALID_VECTOR The B vector has not been setup
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes setValuesVectorB(T scalar) = 0;

				/**
				 * Set the specified row indexes in the vector B to the values stored in the scalars array.
				 *
				 * @param scalars The array of scalars to copy into the vector.
				 * @param nScalars The size of the scalars array
				 * @param indexes The array of global indexes, detailing which positions in the vector to update
				 * @param nIndexes The size of the indexes array
				 * @param indexBase The base of the indexing scheme used in indexes (e.g indexed from 0 or 1)
				 *
				 * @tparam C The implementation class of the Sparse Matrix
				 * @tparam I The type of the indexing system
				 * @tparam T The data type of the matrix non-zero data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_LINEARSOLVER_INVALID_VECTOR The B vector has not been setup
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes setValuesVectorB(T * scalars, I nScalars,
																	I * indexes, I nIndexes, I indexBase) = 0;

				/**
				 * Set values in the internal matrix structure for the linear solver, sourced from the provided matrix.
				 * In parallel setups, this may mean that different positions will be set by different ranks, each holding a portion
				 * of the overall matrix.
				 *
				 * Behaviour is undefined if multiple ranks each try to set the same matrix position.
				 *
				 * @param matrix The matrix where values for the linear solver are sourced and set in matrix A.
				 *
				 * @tparam C The implementation class of the Sparse Matrix
				 * @tparam I The type of the indexing system
				 * @tparam T The data type of the matrix non-zero data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes setValuesMatrixA(cupcfd::data_structures::SparseMatrix<C, I, T>& matrix) = 0;

				/**
				 * Retrieve all values from the vector X, create a suitably sized array and store the results in that array.
				 *
				 * Note: In parallel setups or large problems this could lead to the creation of a very large array. Use with caution.
				 *
				 * @param result A pointer to the location where the newly created array pointer will be stored
				 * @param nResult A pointer to the location where the size of the newly created array will be stored
				 *
				 * @tparam C The implementation class of the Sparse Matrix
				 * @tparam I The type of the indexing system
				 * @tparam T The data type of the matrix non-zero data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes getValuesVectorX(T ** result, I * nResult) = 0;

				/**
				 * Retrieve values from the Vector X at the specified indexes, using the provided index base.
				 * Results will be stored in a suitably sized array created by the function, which will be
				 * use a base index of zero (offset from provided indexes = index - indexBase).
				 *
				 * @param result A pointer to the location where the newly created array pointer will be stored
				 * @param nResult A pointer to the location where the size of the newly created array will be stored
				 * @param indexes A pointer to an array of the vector indexes to retrieve the values of
				 * @param nIndexes The size of the indexes array, in the number of elements of type int
				 * @param indexBase The base used for the indexes array. E.g. if an index if 10 is provided, and baseIndex
				 * is 2, then the index is 8 in a zero-based index.
				 *
				 * @tparam C The implementation class of the Sparse Matrix
				 * @tparam I The type of the indexing system
				 * @tparam T The data type of the matrix non-zero data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes  getValuesVectorX(T ** result, I * nResult,
															 	 	 I * indexes, I nIndexes, I indexBase) = 0;

				/**
				 * Retrieve all values from the vector B, create a suitably sized array and store the results in that array.
				 *
				 * Note: In parallel setups or large problems this could lead to the creation of a very large array. Use with caution.
				 *
				 * @param result A pointer to the location where the newly created array pointer will be stored
				 * @param nResult A pointer to the location where the size of the newly created array will be stored
				 *
				 * @tparam C The implementation class of the Sparse Matrix
				 * @tparam I The type of the indexing system
				 * @tparam T The data type of the matrix non-zero data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes getValuesVectorB(T ** result, I * nResult) = 0;

				/**
				 * Retrieve values from the Vector B at the specified indexes, using the provided index base.
				 * Results will be stored in a suitably sized array created by the function, which will be
				 * use a base index of zero (offset from provided indexes = index - indexBase).
				 *
				 * @param result A pointer to the location where the newly created array pointer will be stored
				 * @param nResult A pointer to the location where the size of the newly created array will be stored
				 * @param indexes A pointer to an array of the vector indexes to retrieve the values of
				 * @param nIndexes The size of the indexes array, in the number of elements of type int
				 * @param indexBase The base used for the indexes array. E.g. if an index if 10 is provided, and baseIndex
				 * is 2, then the index is 8 in a zero-based index.
				 *
				 * @tparam C The implementation class of the Sparse Matrix
				 * @tparam I The type of the indexing system
				 * @tparam T The data type of the matrix non-zero data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes getValuesVectorB(T ** result, I * nResult,
																	I * indexes, I nIndexes, I indexBase) = 0;

				/**
				 * For each position registered as non-zero in the matrix object, retrieve its value from the matrix A in the
				 * linear solver object and overwrite the value inside the referenced matrix object.
				 *
				 * @param matrix The matrix object to copy values into - the positioned copied are based on this matrix's
				 * non-zero structure.
				 *
				 * @tparam C The implementation class of the Sparse Matrix
				 * @tparam I The type of the indexing system
				 * @tparam T The data type of the matrix non-zero data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes getValuesMatrixA(cupcfd::data_structures::SparseMatrix<C, I, T>& matrix) = 0;

				/**
				 * Reset all values of Vector X to 0.0
				 *
				 * @tparam C The implementation class of the Sparse Matrix
				 * @tparam I The type of the indexing system
				 * @tparam T The data type of the matrix non-zero data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_LINEARSOLVER_INVALID_VECTOR The Vector X has not been setup
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes clearVectorX() = 0;

				/**
				 * Reset all values of Vector B to 0.0
				 *
				 * @tparam C The implementation class of the Sparse Matrix
				 * @tparam I The type of the indexing system
				 * @tparam T The data type of the matrix non-zero data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_LINEARSOLVER_INVALID_VECTOR The vector X has not been setup
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes clearVectorB() = 0;

				/**
				 * Reset all non-zero values of Matrix A to 0.0
				 *
				 * @tparam C The implementation class of the Sparse Matrix
				 * @tparam I The type of the indexing system
				 * @tparam T The data type of the matrix non-zero data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes clearMatrixA() = 0;

				/**
				 * Run the linear solver using the current values set in the object for the
				 * Matrix A and Vector B. The contents of vector X will be overwritten with the result.
				 *
				 * @tparam C The implementation class of the Sparse Matrix
				 * @tparam I The type of the indexing system
				 * @tparam T The data type of the matrix non-zero data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes solve() = 0;
		};
	}
}

// Include Header Level Definitions
#include "LinearSolverInterface.ipp"

#endif
