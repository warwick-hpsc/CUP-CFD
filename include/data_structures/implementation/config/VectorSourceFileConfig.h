/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the VectorSourceFileConfig class
 */

#ifndef CUPCFD_DATA_STRUCTURES_VECTOR_SOURCE_FILE_CONFIG_INCLUDE_H
#define CUPCFD_DATA_STRUCTURES_VECTOR_SOURCE_FILE_CONFIG_INCLUDE_H

// Library Functions/Classes
#include <string>

// Parent Class
#include "VectorSourceConfig.h"

// Error Codes
#include "Error.h"

namespace cupcfd
{
	namespace data_structures
	{
		enum VectorFileFormat
		{
			VECTOR_FILE_FORMAT_HDF5
		};

		/**
		 * Specify configuration options required for building mesh data sources from a file.
		 */
		template <class I, class T>
		class VectorSourceFileConfig : public VectorSourceConfig<I,T>
		{
			public:
				// === Members ===

				/** Path to the file where the sparse matrix data is stored **/
				std::string filePath;

				/** Format of the Sparse Matrix file **/
				VectorFileFormat format;

				// === Constructor ===

				/**
				 * Constructor:
				 * Set values to the parameters provided
				 *
				 * @param filePath The file path to where the data source is located
				 * @param format The expected file format of the data source
				 */
				VectorSourceFileConfig(std::string& filePath, VectorFileFormat format);

				/**
				 * Constructor:
				 * Copy values from source into a new object.
				 *
				 * @param source The source object to copy the values from
				 */
				VectorSourceFileConfig(VectorSourceFileConfig<I,T>& source);

				/**
				 * Deconstructor.
				 */
				~VectorSourceFileConfig();

				// === Concrete Methods ===

				/**
				 * Get the file path where the matrix data is stored.
				 *
				 * @return The file path to the matrix data.
				 */
				inline std::string getFilePath();

				/**
				 * Set the file path where the matrix data is stored.
				 *
				 * @param filePath The file path to the matrix data
				 *
				 * @return Nothing
				 */
				inline void setFilePath(std::string filePath);

				/**
				 * Get the file format of the matrix data.
				 *
				 * @return The file format of the matrix data.
				 */
				inline VectorFileFormat getFormat();

				/**
				 * Set the file format of the matrix data.
				 *
				 * @param format The file format of the matrix data.
				 *
				 * @return Nothing
				 */
				inline void setFormat(VectorFileFormat format);

				// === Overridden Inherited Methods ===

				void operator=(VectorSourceFileConfig<I,T>& source);
				
				VectorSourceFileConfig<I,T> * clone();

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes buildVectorSource(VectorSource<I,T> ** vectorSource);
		};
	}
}

// Include Header Level Definitions
#include "VectorSourceFileConfig.ipp"

#endif
