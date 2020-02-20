/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Class Definitions for the MeshConfig class at a non-header level.
 */

#include "MeshConfig.h"
#include "PartitionerConfig.h"
#include "ArrayDrivers.h"
#include "StatisticsDrivers.h"
#include "EuclideanPoint.h"
#include "EuclideanVector.h"

#include "CupCfdAoSMesh.h"

#include <iostream>

namespace euc = cupcfd::geometry::euclidean;

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			// === Constructors/Deconstructors ===

			template <class I, class T, class L>
			MeshConfig<I,T,L>::MeshConfig(cupcfd::partitioner::PartitionerConfig<I,I>& partConfig,
										MeshSourceConfig<I,T,L>& meshSourceConfig)
			{
				// Clone so we maintain the polymorphic type
				this->partConfig = partConfig.clone();
				this->meshSourceConfig = meshSourceConfig.clone();
			}

			template <class I, class T, class L>
			MeshConfig<I,T,L>::MeshConfig(MeshConfig<I,T,L>& source)
			:partConfig(nullptr),
			 meshSourceConfig(nullptr)
			{
				*this = source;
			}

			template <class I, class T, class L>
			MeshConfig<I,T,L>::~MeshConfig()
			{
				delete this->partConfig;
				delete this->meshSourceConfig;
			}

			template <class I, class T, class L>
			MeshConfig<I,T,L> * MeshConfig<I,T,L>::clone()
			{
				return new MeshConfig(*this);
			}
		}
	}
}

// Explicit Instantiation
template class cupcfd::geometry::mesh::MeshConfig<int, float, int>;
template class cupcfd::geometry::mesh::MeshConfig<int, double, int>;
