/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the VectorSourceFileConfigJSON class
 */

#include "VectorSourceFileConfigJSON.h"
#include <fstream>

namespace cupcfd
{
	namespace data_structures
	{
		template <class I, class T>
		VectorSourceFileConfigJSON<I,T>::VectorSourceFileConfigJSON(Json::Value& parseJSON)
		: VectorSourceConfigSource<I,T>()
		{
			this->configData = parseJSON;
		}

		template <class I, class T>
		VectorSourceFileConfigJSON<I,T>::VectorSourceFileConfigJSON(const VectorSourceFileConfigJSON<I,T>& source)
		: VectorSourceConfigSource<I,T>()
		{
			*this = source;
		}

		template <class I, class T>
		VectorSourceFileConfigJSON<I,T>::~VectorSourceFileConfigJSON() {

		}

		template <class I, class T>
		void VectorSourceFileConfigJSON<I,T>::operator=(const VectorSourceFileConfigJSON<I,T>& source) {
			this->configData = source.configData;
		}

		template <class I, class T>
		VectorSourceFileConfigJSON<I,T> * VectorSourceFileConfigJSON<I,T>::clone() {
			return new VectorSourceFileConfigJSON(*this);
		}

		template <class I, class T>
		cupcfd::error::eCodes VectorSourceFileConfigJSON<I,T>::buildVectorSourceConfig(VectorSourceConfig<I,T> ** vectorSourceConfig) {
			cupcfd::error::eCodes status;
			std::string filePath;
			VectorFileFormat fileFormat;

			// Get the File Path
			status = this->getFilePath(filePath);
			CHECK_ECODE(status)

			// Get the File Format
			status = this->getFileFormat(&fileFormat);
			CHECK_ECODE(status)

			// Build the object
			*vectorSourceConfig = new VectorSourceFileConfig<I,T>(filePath, fileFormat);

			return cupcfd::error::E_SUCCESS;

		}

		template <class I, class T>
		cupcfd::error::eCodes VectorSourceFileConfigJSON<I,T>::getFilePath(std::string& filePath) {
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
		cupcfd::error::eCodes VectorSourceFileConfigJSON<I,T>::getFileFormat(VectorFileFormat * format) {
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
					*format = VECTOR_FILE_FORMAT_HDF5;
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
template class cupcfd::data_structures::VectorSourceFileConfigJSON<int, float>;
template class cupcfd::data_structures::VectorSourceFileConfigJSON<int, double>;
