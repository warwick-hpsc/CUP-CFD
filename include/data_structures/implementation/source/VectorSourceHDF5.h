/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the VectorSourceHDF5 class
 */

#ifndef CUPCFD_DATA_STRUCTURES_VECTOR_SOURCE_HDF5_INCLUDE_H
#define CUPCFD_DATA_STRUCTURES_VECTOR_SOURCE_HDF5_INCLUDE_H

#include "VectorSource.h"

// Library Functions/Objects
#include <string>

// Error Codes
#include "Error.h"

namespace cupcfd
{
	namespace data_structures
	{

		/**
		 * Load vector data from a HDF5 source.
		 *
		 * @tparam I The type of the indexing scheme
		 * @tparam T The type of the vector data
		 */
		template <class I, class T>
		class VectorSourceHDF5 : public VectorSource<I,T>
		{
			public:
				// === Members ===

				/** File where the data is stored. **/
				std::string fileName;

				// === Constructor ===

				/**
				 * Constructor.
				 *
				 * Setup class for reading data from the file at the provided location
				 *
				 * @param fileName Location
				 */
				VectorSourceHDF5(std::string fileName);

				/**
				 * Deconstructor.
				 */
				~VectorSourceHDF5();

				// === Concrete Methods ===

				// === Inherited Overloads ===

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getSize(I * vecSize);
				
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getData(T * data, I nData, I * indexes, I nIndexes, I indexBase);

				// === Pure Virtual Methods ===
		};
	}
}

// Include Header Level Definitions
#include "VectorSourceHDF5.ipp"

#endif
