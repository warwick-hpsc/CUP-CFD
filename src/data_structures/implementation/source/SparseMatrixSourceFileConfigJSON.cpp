/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the SparseMatrixSourceFileConfigJSON class
 */

#include "SparseMatrixSourceFileConfigJSON.h"
#include <fstream>

namespace cupcfd
{
	namespace data_structures
	{
		template <class I, class T>
		SparseMatrixSourceFileConfigJSON<I,T>::SparseMatrixSourceFileConfigJSON(Json::Value& parseJSON)
		{
			this->configData = parseJSON;
		}

		template <class I, class T>
		SparseMatrixSourceFileConfigJSON<I,T>::SparseMatrixSourceFileConfigJSON(const SparseMatrixSourceFileConfigJSON<I,T>& source)
		{
			*this = source;
		}

		template <class I, class T>
		SparseMatrixSourceFileConfigJSON<I,T>::~SparseMatrixSourceFileConfigJSON()
		{

		}

		template <class I, class T>
		void SparseMatrixSourceFileConfigJSON<I,T>::operator=(const SparseMatrixSourceFileConfigJSON<I,T>& source) {
			this->configData = source.configData;
		}

		template <class I, class T>
		SparseMatrixSourceFileConfigJSON<I,T> * SparseMatrixSourceFileConfigJSON<I,T>::clone() {
			return new SparseMatrixSourceFileConfigJSON(*this);
		}

		template <class I, class T>
		cupcfd::error::eCodes SparseMatrixSourceFileConfigJSON<I,T>::buildSparseMatrixSourceConfig(SparseMatrixSourceConfig<I,T> ** matrixSourceConfig) {
			cupcfd::error::eCodes status;
			std::string filePath;
			SparseMatrixFileFormat fileFormat;

			// Get the File Path
			status = this->getFilePath(filePath);
			CHECK_ERROR_CODE(status)
			if(status != cupcfd::error::E_SUCCESS) {
				return status;
			}

			// Get the File Format
			status = this->getFileFormat(&fileFormat);
			CHECK_ERROR_CODE(status)
			if(status != cupcfd::error::E_SUCCESS) {
				return status;
			}

			// Build the object
			*matrixSourceConfig = new SparseMatrixSourceFileConfig<I,T>(filePath, fileFormat);

			return cupcfd::error::E_SUCCESS;

		}

		template <class I, class T>
		cupcfd::error::eCodes SparseMatrixSourceFileConfigJSON<I,T>::getFilePath(std::string& filePath) {
			Json::Value dataSourceType;

			// Access the correct field
			if(this->configData.isMember("FilePath")) {
				dataSourceType = this->configData["FilePath"];

				// Check the value and return the appropriate ID
				if(dataSourceType == Json::Value::null) {
					return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
				}
				else {
					filePath = dataSourceType.asString();
					return cupcfd::error::E_SUCCESS;
				}

				// Found, but not a matching value
				return cupcfd::error::E_CONFIG_INVALID_VALUE;
			}

			return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
		}

		template <class I, class T>
		cupcfd::error::eCodes SparseMatrixSourceFileConfigJSON<I,T>::getFileFormat(SparseMatrixFileFormat * format) {
			Json::Value dataSourceType;

			if(this->configData.isMember("FileFormat")) {
				// Access the correct field
				dataSourceType = this->configData["FileFormat"];

				// Check the value and return the appropriate ID
				if(dataSourceType == Json::Value::null) {
					return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
				}
				else if(dataSourceType == "HDF5") {
					// Note: This will result in loss of precision if storage type is less than a double
					*format = SPARSE_MATRIX_FILE_FORMAT_HDF5;
					return cupcfd::error::E_SUCCESS;
				}

				// Found, but not a matching value
				return cupcfd::error::E_CONFIG_INVALID_VALUE;
			}

			return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
		}
	}
}

// Explicit Instantiation
template class cupcfd::data_structures::SparseMatrixSourceFileConfigJSON<int, float>;
template class cupcfd::data_structures::SparseMatrixSourceFileConfigJSON<int, double>;
