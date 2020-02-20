/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the VectorSource class
 */

#ifndef CUPCFD_DATA_STRUCTURES_VECTOR_SOURCE_INCLUDE_H
#define CUPCFD_DATA_STRUCTURES_VECTOR_SOURCE_INCLUDE_H

#include "Error.h"

namespace cupcfd
{
	namespace data_structures
	{
		/**
		 * This class defines an interface for accessing vector data from some
		 * as yet undefined source (e.g. a file)
		 *
		 * @tparam I The type of the indexing scheme
		 * @tparam T The type of the stored node data
		 */
		template <class I, class T>
		class VectorSource
		{
			public:
				// === Members ===

				// === Constructor ===

				/**
				 * Constructor:
				 * Default - current sets nothing.
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 */
				VectorSource();

				/**
				 * Deconstructor
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 */
				virtual ~VectorSource();

				// === Concrete Methods ===

				// === Pure Virtual Methods ===

				/**
				 * Get the number of elements in the vector stored in the data source
				 *
				 * @param vecSize A pointer to the location updated with the number of elements
				 *
				 * @return An error code indicating the success or failure of the function
				 * @retval cupcfd::error::E_SUCCESS The function completed successfully
				 */
				virtual cupcfd::error::eCodes getSize(I * vecSize) = 0;

				/**
				 * Get the data elements at the indexes provided in the data source.
				 *
				 * @param data A pointer to an array that will be updated with the retrieved elements
				 * matching the same order as the indexes provided.
				 * @param nData The size of the data array in the number of elements of type T
				 * @param indexes An array of the indexes to retrieve data for from the data source
				 * @param nIndexes The size of the indexes array in the number of elements of type I
				 * @param indexBase The base index used for the indexes provided in the indexes array (e.g. 0 for zero-indexed etc).
				 * These will be shifted if the data source requires it.
				 */
				virtual cupcfd::error::eCodes getData(T * data, I nData, I * indexes, I nIndexes, I indexBase) = 0;
		};
	}
}

// Include Header Level Definitions
#include "SparseMatrixSource.ipp"

#endif
