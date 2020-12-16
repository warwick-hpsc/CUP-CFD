/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the MeshSourceFileConfigJSON class.
 */

#include "MeshSourceFileConfigJSON.h"
#include <fstream>
#include "MeshSourceFileConfig.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			template <class I, class T>
			MeshSourceFileConfigJSON<I,T>::MeshSourceFileConfigJSON(std::string configFilePath, std::string * topLevel, int nTopLevel) {
				// Copy the top level strings
				for(int i = 0; i < nTopLevel; i++) {
					this->topLevel.push_back(topLevel[i]);
				}
				this->topLevel.push_back("MeshSourceFile");

				std::ifstream source(configFilePath, std::ifstream::binary);
				source >> this->configData;
				int iLimit;
				iLimit = cupcfd::utility::drivers::safeConvertSizeT<int>(this->topLevel.size());
				for(int i = 0; i < iLimit; i++) {
					this->configData = this->configData[this->topLevel[i]];
				}
			}

			template <class I, class T>
			MeshSourceFileConfigJSON<I,T>::MeshSourceFileConfigJSON(MeshSourceFileConfigJSON<I,T>& source)
			{
				*this = source;
			}

			template <class I, class T>
			MeshSourceFileConfigJSON<I,T>::~MeshSourceFileConfigJSON()
			{

			}

			template <class I, class T>
			void MeshSourceFileConfigJSON<I,T>::operator=(MeshSourceFileConfigJSON<I,T>& source)
			{
				this->topLevel = source.topLevel;
				this->configData = source.configData;
			}

			template <class I, class T>
			MeshSourceFileConfigJSON<I,T> * MeshSourceFileConfigJSON<I,T>::clone()
			{
				return new MeshSourceFileConfigJSON<I,T>(*this);
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshSourceFileConfigJSON<I,T>::getFileFormat(MeshFileFormat * fileFormat)
			{
				const Json::Value dataSourceType = this->configData["FileFormat"];

				if(dataSourceType == Json::Value::null)
				{
					return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
				}
				else if(dataSourceType == "HDF5")
				{
					*fileFormat = MESH_FILE_FORMAT_HDF5;
					return cupcfd::error::E_SUCCESS;
				}

				// Found, but not a matching value
				return cupcfd::error::E_CONFIG_INVALID_VALUE;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshSourceFileConfigJSON<I,T>::getFilePath(std::string& sourceFilePath)
			{
				const Json::Value dataSourceType = this->configData["FilePath"];

				if(dataSourceType == Json::Value::null)
				{
					return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
				}
				else
				{
					sourceFilePath = dataSourceType.asString();
					return cupcfd::error::E_SUCCESS;
				}

				// Found, but not a matching value
				return cupcfd::error::E_CONFIG_INVALID_VALUE;
			}

			template <class I, class T>
			cupcfd::error::eCodes MeshSourceFileConfigJSON<I,T>::buildMeshSourceConfig(MeshSourceConfig<I,T,I> ** meshSourceConfig)
			{
				cupcfd::error::eCodes status;

				MeshFileFormat fileFormat;
				std::string sourceFilePath;

				status = this->getFileFormat(&fileFormat);
				if(status != cupcfd::error::E_SUCCESS)
				{
					return status;
				}

				status = this->getFilePath(sourceFilePath);
				if(status != cupcfd::error::E_SUCCESS)
				{
					return status;
				}

				*meshSourceConfig = new MeshSourceFileConfig<I,T>(fileFormat, sourceFilePath);

				return cupcfd::error::E_SUCCESS;
			}
		}
	}
}

// Explicit Instantiation
template class cupcfd::geometry::mesh::MeshSourceFileConfigJSON<int, float>;
template class cupcfd::geometry::mesh::MeshSourceFileConfigJSON<int, double>;
