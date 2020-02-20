/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the SparseMatrixSourceFileConfig class
 */

#ifndef CUPCFD_DATA_STRUCTURES_SPARSE_MATRIX_SOURCE_FILE_CONFIG_INCLUDE_H
#define CUPCFD_DATA_STRUCTURES_SPARSE_MATRIX_SOURCE_FILE_CONFIG_INCLUDE_H

// Library Functions/Classes
#include <string>

// Parent Class
#include "SparseMatrixSourceConfig.h"

// Error Codes
#include "Error.h"

// Sparse Matrix Source
#include "SparseMatrixSource.h"

namespace cupcfd
{
	namespace data_structures
	{
		enum SparseMatrixFileFormat
		{
			SPARSE_MATRIX_FILE_FORMAT_HDF5
		};

		/**
		 * Specify configuration options required for building mesh data sources from a file.
		 */
		template <class I, class T>
		class SparseMatrixSourceFileConfig : public SparseMatrixSourceConfig<I,T>
		{
			public:
				// === Members ===

				/** Path to the file where the sparse matrix data is stored **/
				std::string filePath;

				/** Format of the Sparse Matrix file **/
				SparseMatrixFileFormat format;

				// === Constructor ===

				/**
				 * Constructor:
				 * Set values to the parameters provided
				 *
				 * @param filePath The file path to where the data source is located
				 * @param format The expected file format of the data source
				 */
				SparseMatrixSourceFileConfig(std::string& filePath, SparseMatrixFileFormat format);

				/**
				 * Constructor:
				 * Copy values from source into a new object.
				 *
				 * @param source The source object to copy the values from
				 */
				SparseMatrixSourceFileConfig(SparseMatrixSourceFileConfig<I,T>& source);

				/**
				 * Deconstructor.
				 */
				virtual ~SparseMatrixSourceFileConfig();

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
				inline SparseMatrixFileFormat getFormat();

				/**
				 * Set the file format of the matrix data.
				 *
				 * @param format The file format of the matrix data.
				 *
				 * @return Nothing
				 */
				inline void setFormat(SparseMatrixFileFormat format);

				// === Overridden Inherited Methods ===

				void operator=(SparseMatrixSourceFileConfig<I,T>& source);
				SparseMatrixSourceFileConfig<I,T> * clone();
				cupcfd::error::eCodes buildSparseMatrixSource(SparseMatrixSource<I,T> ** matrixSource);
		};
	}
}

// Include Header Level Definitions
#include "SparseMatrixSourceFileConfig.ipp"

#endif
