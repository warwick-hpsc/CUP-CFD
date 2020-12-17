/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Class Definition for the PartitionerMetisConfigSourceJSON class.
 */

// Header for this class
#include "PartitionerMetisConfigSourceJSON.h"
#include "PartitionerMetisConfig.h"

// File access for reading into JSON structures
#include <fstream>

namespace cupcfd
{
	namespace partitioner
	{
		// === Constructors/Deconstructors ===

		template <class I, class T>
		PartitionerMetisConfigSourceJSON<I,T>::PartitionerMetisConfigSourceJSON(Json::Value& parseJSON)
		{
			this->configData = parseJSON;
		}


		template <class I, class T>
		PartitionerMetisConfigSourceJSON<I,T>::~PartitionerMetisConfigSourceJSON()
		{
			// Nothing to do currently
		}

		template <class I, class T>
		cupcfd::error::eCodes PartitionerMetisConfigSourceJSON<I,T>::buildPartitionerConfig(PartitionerConfig<I,T> ** config)
		{
			// Since we don't currently load any other options, we just check that at least a valid JSON record was passed.
			if(this->configData == Json::Value::null)
			{
				DEBUGGABLE_ERROR; return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
			}

			*config = new PartitionerMetisConfig<I,T>();
			return cupcfd::error::E_SUCCESS;
		}
	}
}

// Explicit Instantiation
template class cupcfd::partitioner::PartitionerMetisConfigSourceJSON<int,int>;
