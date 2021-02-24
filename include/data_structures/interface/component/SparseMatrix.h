/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the SparseMatrix class
 */

#ifndef CUPCFD_DATA_STRUCTURES_SPARSEMATRIX_INCLUDE_H
#define CUPCFD_DATA_STRUCTURES_SPARSEMATRIX_INCLUDE_H

#include "Error.h"

namespace cupcfd
{
	namespace data_structures
	{
		/**
		 * Provides an interface for storing a Matrix in a Sparse Matrix Format for later
		 * use in components such as Linear Solvers.
		 *
		 * The design pattern for this uses CRTP, since it is desirable to avoid virtual overheads for
		 * getters and setters to permit a single interface access.
		 *
		 * @tparam C The type of the implementation class for the SparseMatrix.
		 * This must inherit from this class passing itself as template parameter C, as well as
		 * providing implementation for the CRTP methods to passthrough to.
		 * @tparam I The type of the indexing scheme
		 * @tparam T The type of the stored node data
		 */
		template <class C, class I, class T>
		class SparseMatrix
		{
			public:
				// === Variables ===
				/** Number of rows **/
				I m;

				/** Number of columns **/
				I n;

				/** Base (Minimum) Row/Column Index of the matrix indexing scheme **/
				I baseIndex;

				/** Number of non-zero values in the matrix **/
				I nnz;

				/**
				 * Create the default sparse matrix object.
				 * Defaults to a size of 1, 1 with a base index of 0
				 *
				 * @tparam C The type of the implementation class for the SparseMatrix.
				 * This must inherit from this class passing itself as template parameter C, as well as
				 * providing implementation for the CRTP methods to passthrough to.
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 */
				SparseMatrix();

				/**
				 * Create the a sparse matrix object of a specific size and
				 * a base index of 0
				 *
				 * @param m Number of rows
				 * @param n Number of columns
				 *
				 * @tparam C The type of the implementation class for the SparseMatrix.
				 * This must inherit from this class passing itself as template parameter C, as well as
				 * providing implementation for the CRTP methods to passthrough to.
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 */
				SparseMatrix(I m, I n);

				/**
				 * Create the a sparse matrix object of a specific size and
				 * base index
				 *
				 * @param m Number of rows
				 * @param n Number of columns
				 * @param baseIndex The base index to use for the matrix indexing scheme
				 *
				 * @tparam C The type of the implementation class for the SparseMatrix.
				 * This must inherit from this class passing itself as template parameter C, as well as
				 * providing implementation for the CRTP methods to passthrough to.
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 */
				SparseMatrix(I m, I n, I baseIndex);

				/**
				 * Destroy a sparse matrix object
				 *
				 * @tparam C The type of the implementation class for the SparseMatrix.
				 * This must inherit from this class passing itself as template parameter C, as well as
				 * providing implementation for the CRTP methods to passthrough to.
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 */
				~SparseMatrix();

				// === CRTP Methods ===

				/**
				 * Resize the matrix. This will also clear it.
				 *
				 * @param rows The number of rows the matrix will have after resizing (must be 1 or greater)
				 * @param columns The number of columns the matrix will have after resizing (must be 1 or greater)
				 *
				 * @tparam C The type of the implementation class for the SparseMatrix.
				 * This must inherit from this class passing itself as template parameter C, as well as
				 * providing implementation for the CRTP methods to passthrough to.
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_MATRIX_INVALID_ROW_SIZE An invalid row size was provided
				 * @retval cupcfd::error::E_MATRIX_INVALID_COL_SIZE An invalid column size was provided
				 */
				__attribute__((warn_unused_result))
				inline cupcfd::error::eCodes resize(I rows, I columns);

				/**
				 * Clear the matrix of any non-zero values
				 *
				 * @tparam C The type of the implementation class for the SparseMatrix.
				 * This must inherit from this class passing itself as template parameter C, as well as
				 * providing implementation for the CRTP methods to passthrough to.
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				inline cupcfd::error::eCodes clear();

				/**
				 * Set the element in the matrix at a specific index
				 *
				 * @param row The row index, starting from the matrix base index
				 * @param col The column index, starting from the matrix base index
				 *
				 * @tparam C The type of the implementation class for the SparseMatrix.
				 * This must inherit from this class passing itself as template parameter C, as well as
				 * providing implementation for the CRTP methods to passthrough to.
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_MATRIX_ROW_OOB The row index is outside the matrix index range
				 * @retval cupcfd::error::E_MATRIX_COL_OOB The column index is outside the matrix index range
				 */
				__attribute__((warn_unused_result))
				inline cupcfd::error::eCodes setElement(I row, I col, T val);

				/**
				 * Retrieve the element stored in the matrix at a specific index
				 *
				 * @param row The row index, starting from the matrix base index
				 * @param col The column index, starting from the matrix base index
				 * @param val A pointer to the location where the element value will be stored
				 *
				 * @tparam C The type of the implementation class for the SparseMatrix.
				 * This must inherit from this class passing itself as template parameter C, as well as
				 * providing implementation for the CRTP methods to passthrough to.
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_MATRIX_ROW_OOB The row index is outside the matrix index range
				 * @retval cupcfd::error::E_MATRIX_COL_OOB The column index is outside the matrix index range
				 */
				__attribute__((warn_unused_result))
				inline cupcfd::error::eCodes getElement(I row, I col, T * val);

				/**
				 * Retrieve the indexes of rows in the matrix that contain one or more non-zero values
				 *
				 * @param rowIndexes A pointer to the location where an array containing the row indexes will be stored.
				 * These should be sorted by row order.
				 * @param nRowIndexes A pointer to the location where the number of row indexes will be stored
				 *
				 * @tparam C The type of the implementation class for the SparseMatrix.
				 * This must inherit from this class passing itself as template parameter C, as well as
				 * providing implementation for the CRTP methods to passthrough to.
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				inline cupcfd::error::eCodes getNonZeroRowIndexes(I ** rowIndexes, I * nRowIndexes);

				/**
				 * Retrieve the matrix column indexes that have non-zero values set for the specified row.
				 * They are in sorted order - i.e. ascending column indexes
				 *
				 * @param rowIndex The index of the row to get the column indexes for
				 * @param columnIndexes A pointer to the location where an array containing the column indexes will be created.
				 * These should be sorted in column order.
				 * @param nColumnIndexes A pointer to the location where the number of column indexes retrieved will be stored
				 *
				 * @tparam C The type of the implementation class for the SparseMatrix.
				 * This must inherit from this class passing itself as template parameter C, as well as
				 * providing implementation for the CRTP methods to passthrough to.
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_MATRIX_ROW_OOB rowIndex is out of bounds for an acceptable row index (from the matrix base index)
				 */
				__attribute__((warn_unused_result))
				inline cupcfd::error::eCodes getRowColumnIndexes(I rowIndex, I ** columnIndexes, I * nColumnIndexes);

				/**
				 * Return the matrix non-zero values that have values set for the specified row
				 * They are in the order they appear in across the row - i.e. ascending column index.
				 *
				 * @param rowIndex The index of the row to get the non-zero values from
				 * @param nnzValues A pointer to the location where an array containing the non-zero values for the row will be created.
				 * These should be sorted by column order.
				 * @param nNNZValues A pointer to the location where the number of non-zero values retrieved will be stored
				 *
				 * @tparam C The type of the implementation class for the SparseMatrix.
				 * This must inherit from this class passing itself as template parameter C, as well as
				 * providing implementation for the CRTP methods to passthrough to.
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_MATRIX_ROW_OOB rowIndex is out of bounds for an acceptable row index (from the matrix base index)
				 */
				__attribute__((warn_unused_result))
				inline cupcfd::error::eCodes getRowNNZValues(I rowIndex, T ** nnzValues, I * nNNZValues);

				// === Concrete Methods ===

				/**
				 * Populate the matrix with random non-zero values uniformly between a minimum and a maximum.
				 * The non-zero structure is determined by the local and ghost component of a distributed graph.
				 * This function will destroy any existing data and resize the mesh to a size appropriate for the graph
				 *
				 * Note: For this function, the indexes used are the global indexes assigned in the graph as
				 * row and column indexes. This is necessary to ensure that (a) the rows are kept together per process,
				 * (b) no more than one rank will set a specific row and (c) to ensure that there exists a full diagonal
				 * running through the matrix across the various ranks
				 *
				 * Note: This method only allocates rows in the matrix for locally owned cells - it is presumed that
				 * the respective cell owner will be generating its own matrix with that row in.
				 *
				 * @param graph The distributed graph to take the non-zero structure from. This takes the form
				 * of row = cell, cols = neighbouring cells + diagonal.
				 * @param minVal The minimum value to use for the randomly generated values
				 * @param maxVal The maximum value to use for the randomly generated values
				 *
				 * @tparam C The type of the implementation class for the SparseMatrix.
				 * This must inherit from this class passing itself as template parameter C, as well as
				 * providing implementation for the CRTP methods to passthrough to.
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */

				//cupcfd::error::eCodes populateUniformRandomGlobalIndexing(cupcfd::adjacency_list::DistributedAdjacencyList<I, int>& graph,
				//												 T minVal,
				//												 T maxVal);

				/**
				 * Populate the matrix with random non-zero values uniformly between a minimum and a maximum.
				 * The non-zero structure is determined by the local and ghost component of a distributed graph.
				 * This function will destroy any existing data and resize the mesh to a size appropriate for the graph
				 *
				 * Note: For this function, the indexes used are the local indexes assigned in the graph as
				 * row and column indexes. This means this method is unsuitable to generate matrices used for a parallel
				 * operations such as a parallel linear solve
				 *
				 * Note: This method only allocates rows in the matrix for locally owned cells.
				 *
				 * @param graph The distributed graph to take the non-zero structure from. This takes the form
				 * of row = cell, cols = neighbouring cells + diagonal.
				 * @param minVal The minimum value to use for the randomly generated values
				 * @param maxVal The maximum value to use for the randomly generated values
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_DISTGRAPH_UNFINALIZED Error: The provided graph has not been finalised
				 */

				//cupcfd::error::eCodes populateUniformRandomLocalIndexing(cupcfd::adjacency_list::DistributedAdjacencyList<I, int>& graph,
				//												 T minVal,
				//												 T maxVal);

			protected:
		};
	}
}

// Include Header Level Definitions
#include "SparseMatrix.ipp"

#endif
