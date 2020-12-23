/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the ParticleSimpleSourceFileConfigJSON class
 */

#include "ParticleSimpleSourceFileConfigJSON.h"
#include <fstream>

namespace cupcfd
{
	namespace particles
	{
		template <class I, class T>
		ParticleSimpleSourceFileConfigJSON<I,T>::ParticleSimpleSourceFileConfigJSON(Json::Value& parseJSON)
		:ParticleSourceConfigSource<ParticleSimple<I,T>,I,T>()
		{
			this->configData = parseJSON;
		}

		template <class I, class T>
		ParticleSimpleSourceFileConfigJSON<I,T>::ParticleSimpleSourceFileConfigJSON(const ParticleSimpleSourceFileConfigJSON<I,T>& source)
		{
			*this = source;
		}

		template <class I, class T>
		ParticleSimpleSourceFileConfigJSON<I,T>::~ParticleSimpleSourceFileConfigJSON()
		{

		}

		template <class I, class T>
		void ParticleSimpleSourceFileConfigJSON<I,T>::operator=(const ParticleSimpleSourceFileConfigJSON<I,T>& source) {
			this->configData = source.configData;
		}

		template <class I, class T>
		ParticleSimpleSourceFileConfigJSON<I,T> * ParticleSimpleSourceFileConfigJSON<I,T>::clone() {
			return new ParticleSimpleSourceFileConfigJSON(*this);
		}

		template <class I, class T>
		cupcfd::error::eCodes ParticleSimpleSourceFileConfigJSON<I,T>::buildParticleSourceConfig(ParticleSourceConfig<ParticleSimple<I,T>,I,T> ** config) {
		// cupcfd::error::eCodes ParticleSimpleSourceFileConfigJSON<I,T>::buildParticleSourceConfig(ParticleSourceConfig<ParticleSimple<I,T>,I,T> ** config, int sourceId) {
			cupcfd::error::eCodes status;
			std::string filePath;
			ParticleSimpleFileFormat fileFormat;

			// Get the File Path
			status = this->getFilePath(filePath);
			CHECK_ECODE(status)

			// Get the File Format
			status = this->getFileFormat(&fileFormat);
			CHECK_ECODE(status)

			// Build the object
			*config = new ParticleSimpleSourceFileConfig<I,T>(filePath, fileFormat);
			// *config = new ParticleSimpleSourceFileConfig<I,T>(filePath, fileFormat, id);

			return cupcfd::error::E_SUCCESS;

		}

		template <class I, class T>
		cupcfd::error::eCodes ParticleSimpleSourceFileConfigJSON<I,T>::getFilePath(std::string& filePath) {
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
		cupcfd::error::eCodes ParticleSimpleSourceFileConfigJSON<I,T>::getFileFormat(ParticleSimpleFileFormat * format) {
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
					*format = PARTICLE_SIMPLE_FILE_FORMAT_HDF5;
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
template class cupcfd::particles::ParticleSimpleSourceFileConfigJSON<int, float>;
template class cupcfd::particles::ParticleSimpleSourceFileConfigJSON<int, double>;
