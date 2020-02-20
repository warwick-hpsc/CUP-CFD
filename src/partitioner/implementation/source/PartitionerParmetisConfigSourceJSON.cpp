/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Class Definition for the PartitionerParmetisConfigSourceJSON class.
 */

// Header for this class
#include "PartitionerParmetisConfigSourceJSON.h"
#include "PartitionerParmetisConfig.h"

// File access for reading into JSON structures
#include <fstream>

namespace cupcfd
{
	namespace partitioner
	{
		// === Constructors/Deconstructors ===

		template <class I, class T>
		PartitionerParmetisConfigSourceJSON<I,T>::PartitionerParmetisConfigSourceJSON(Json::Value& parseJSON)
		{
			this->configData = parseJSON;
		}

		template <class I, class T>
		PartitionerParmetisConfigSourceJSON<I,T>::~PartitionerParmetisConfigSourceJSON()
		{
			// Nothing to do currently
		}

		// === Concrete Methods ===

		template <class I, class T>
		cupcfd::error::eCodes PartitionerParmetisConfigSourceJSON<I,T>::buildPartitionerConfig(PartitionerConfig<I,T> ** config)
		{
			// Since we don't currently load any other options, we can only match on the top level name
			// This can be included as a workaround for now in lieu of missing options checks to verify if a valid top level
			// name was found (and thus a correct configuration match)
			if(this->configData == Json::Value::null)
			{
				return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
			}

			*config = new PartitionerParmetisConfig<I,T>();
			return cupcfd::error::E_SUCCESS;
		}
	}
}

// Explicit Instantiation
template class cupcfd::partitioner::PartitionerParmetisConfigSourceJSON<int,int>;
