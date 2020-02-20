/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the MeshConfigSourceJSON class.
 */

#include "MeshConfigSourceJSON.h"
#include "PartitionerMetisConfigSourceJSON.h"
#include "PartitionerNaiveConfigSourceJSON.h"
#include "PartitionerParmetisConfigSourceJSON.h"
#include "MeshSourceFileConfigJSON.h"
#include "MeshSourceStructGenConfigJSON.h"

#include <fstream>

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			template <class I, class T, class L>
			MeshConfigSourceJSON<I,T,L>::MeshConfigSourceJSON(std::string configFilePath, std::string * topLevel, int nTopLevel)
			{
				// Copy the top level strings
				for(int i = 0; i < nTopLevel; i++)
				{
					this->topLevel.push_back(topLevel[i]);
				}

				this->topLevel.push_back("Mesh");

				std::ifstream source(configFilePath, std::ifstream::binary);
				source >> this->configData;

				for(int i = 0; i < this->topLevel.size(); i++)
				{
					this->configData = this->configData[this->topLevel[i]];
				}

				this->configFilePath = configFilePath;
			}

			template <class I, class T, class L>
			MeshConfigSourceJSON<I,T,L>::MeshConfigSourceJSON(MeshConfigSourceJSON<I,T,L>& source)
			{
				*this = source;
			}

			template <class I, class T, class L>
			MeshConfigSourceJSON<I,T,L>::~MeshConfigSourceJSON()
			{

			}

			template <class I, class T, class L>
			void MeshConfigSourceJSON<I,T,L>::operator=(MeshConfigSourceJSON<I,T,L>& source)
			{
				this->topLevel = source.topLevel;
				this->configData = source.configData;
				this->configFilePath = source.configFilePath;
			}

			template <class I, class T, class L>
			MeshConfigSourceJSON<I,T,L> * MeshConfigSourceJSON<I,T,L>::clone()
			{
				return new MeshConfigSourceJSON<I,T,L>(*this);
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshConfigSourceJSON<I,T,L>::getPartitionerConfig(cupcfd::partitioner::PartitionerConfig<I,I> ** partConfig)
			{
				cupcfd::error::eCodes status;

				// Determine and build the correct polymorphic PartitionerConfig Type
				// Search for one of the potential partitioner options under the "Partitioner" field

				if(this->configData.isMember("Partitioner"))
				{
					// Partitioner field found, check which (if any) partitioner is specified

					Json::Value partConfigData = this->configData;

					if(partConfigData["Partitioner"].isMember("NaivePartitioner"))
					{
						cupcfd::partitioner::PartitionerNaiveConfigSourceJSON<I,I> naiveConfigSource(partConfigData["Partitioner"]["NaivePartitioner"]);
						status = naiveConfigSource.buildPartitionerConfig(partConfig);

						// Return error or success depending on whether the object was built (e.g. missing options causes failure)
						return status;
					}
					else if(partConfigData["Partitioner"].isMember("MetisPartitioner"))
					{
						cupcfd::partitioner::PartitionerMetisConfigSourceJSON<I,I> metisConfigSource(partConfigData["Partitioner"]["MetisPartitioner"]);
						status = metisConfigSource.buildPartitionerConfig(partConfig);

						// Return error or success depending on whether the object was built (e.g. missing options causes failure)
						return status;
					}
					else if(partConfigData["Partitioner"].isMember("ParmetisPartitioner"))
					{
						cupcfd::partitioner::PartitionerParmetisConfigSourceJSON<I,I> parmetisConfigSource(partConfigData["Partitioner"]["ParmetisPartitioner"]);
						status = parmetisConfigSource.buildPartitionerConfig(partConfig);

						// Return error or success depending on whether the object was built (e.g. missing options causes failure)
						return status;
					}
					else
					{
						// No expected Partitioner Field found
						return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
					}
				}

				// Partitioner Field not found
				return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
			}

			template <class I, class T, class L>
			MeshSourceConfig<I,T,L> * MeshConfigSourceJSON<I,T,L>::getMeshSourceConfig()
			{
				// Determine and build the correct MeshSourceConfig type
				// MeshSource Config stored under "MeshSource" in the Mesh JSON

				cupcfd::error::eCodes status;

				std::vector<std::string> meshSourceTopLevels = this->topLevel;
				meshSourceTopLevels.push_back("MeshSource");

				MeshSourceFileConfigJSON<I,T> source1Config(this->configFilePath, &(meshSourceTopLevels[0]), meshSourceTopLevels.size());
				MeshSourceStructGenConfigJSON<I,T> source2Config(this->configFilePath, &(meshSourceTopLevels[0]), meshSourceTopLevels.size());

				MeshSourceConfig<I,T,L> * sourceConfig;

				// Test Mesh Source from a File
				status = source1Config.buildMeshSourceConfig(&sourceConfig);
				if(status == cupcfd::error::E_SUCCESS)
				{
					return sourceConfig;
				}

				// Test Mesh Source from Structured Generation
				status = source2Config.buildMeshSourceConfig(&sourceConfig);
				if(status == cupcfd::error::E_SUCCESS)
				{
					return sourceConfig;
				}
			}

			template <class I, class T, class L>
			cupcfd::error::eCodes MeshConfigSourceJSON<I,T,L>::buildMeshConfig(MeshConfig<I,T,L> ** config)
			{
				cupcfd::error::eCodes status;

				// Get the partitioner config
				cupcfd::partitioner::PartitionerConfig<I,I> * partConfig;

				status = this->getPartitionerConfig(&partConfig);
				if(status != cupcfd::error::E_SUCCESS)
				{
					return status;
				}

				MeshSourceConfig<I,T,L> * sourceConfig = this->getMeshSourceConfig();

				*config = new MeshConfig<I,T,L>(*partConfig, *sourceConfig);

				delete partConfig;
				delete sourceConfig;

				return cupcfd::error::E_SUCCESS;
			}
		}
	}
}

// Explicit Instantiation
template class cupcfd::geometry::mesh::MeshConfigSourceJSON<int, float, int>;
template class cupcfd::geometry::mesh::MeshConfigSourceJSON<int, double, int>;
