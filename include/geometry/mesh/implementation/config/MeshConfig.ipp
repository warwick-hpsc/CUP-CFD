/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Select Class Definitions for the MeshConfig class at a header level.
 */

#ifndef CUPCFD_CONFIG_MESH_CONFIG_IPP_H
#define CUPCFD_CONFIG_MESH_CONFIG_IPP_H

#include <iostream>

namespace euc = cupcfd::geometry::euclidean;

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			template <class I, class T, class L>
			inline cupcfd::partitioner::PartitionerConfig<I,I> * MeshConfig<I,T,L>::getPartitionerConfig()
			{
				return this->partConfig->clone();
			}

			template <class I, class T, class L>
			inline void MeshConfig<I,T,L>::setPartitionerConfig(cupcfd::partitioner::PartitionerConfig<I,I>& partConfig)
			{
				// Cleanup old, clone new.
				if(this->partConfig != nullptr)
				{
					delete this->partConfig;
				}
					
				this->partConfig = partConfig.clone();
			}

			template <class I, class T, class L>
			inline MeshSourceConfig<I,T,L> * MeshConfig<I,T,L>::getMeshSourceConfig()
			{
				return this->meshSourceConfig->clone();
			}

			template <class I, class T, class L>
			inline void MeshConfig<I,T,L>::setMeshSourceConfig(MeshSourceConfig<I,T,L>& meshSourceConfig)
			{
				if(this->meshSourceConfig != nullptr)
				{
					delete this->meshSourceConfig;
				}

				this->meshSourceConfig = meshSourceConfig.clone();
			}
					
			template <class I, class T, class L>
			inline void MeshConfig<I,T,L>::operator=(const MeshConfig<I,T,L>& source)
			{				
				this->setPartitionerConfig(*(source.partConfig));
				this->setMeshSourceConfig(*(source.meshSourceConfig));
			}
			
			// ToDo: Might wish to consider splitting this up and putting parts of it in MeshSource so that a
			// Mesh could be built directly without creating a MeshConfig object first.
			// This functions for now however.
			// ToDo: This should be a more generic cupcfd::geometry::mesh::UnstructuredMeshInterface<M,I,T,L>& mesh rather
			// than type M to be more explicit about its format, but getting a cannot deduce template argument error.

			template <class I, class T, class L>
			template <class M>
			cupcfd::error::eCodes MeshConfig<I,T,L>::buildUnstructuredMesh(M ** mesh,
																			  cupcfd::comm::Communicator& comm)
			{
				cupcfd::error::eCodes status;

				// ==========================================================
				// (1) Setup Stage: Identify which cells this process 'owns'
				// ==========================================================


				// (1) Build the mesh source
				MeshSource<I,T,L> * source;
				status = this->meshSourceConfig->buildMeshSource(&source);
				if(status != cupcfd::error::E_SUCCESS)
				{
					return status;
				}

				// (2) Build a naive connectivity graph
				cupcfd::data_structures::DistributedAdjacencyList<I,I> * naiveConnGraph;
				status = source->buildDistributedAdjacencyList(&naiveConnGraph, comm);
				if(status != cupcfd::error::E_SUCCESS)
				{
					return status;
				}
				
				// (3) Use the partitioner config to build a partitioner
				cupcfd::partitioner::PartitionerInterface<I,I> * partitioner;
				status = this->partConfig->buildPartitioner(&partitioner, *naiveConnGraph);
				if(status != cupcfd::error::E_SUCCESS)
				{
					return status;
				}
				
				// (4) Run the partitioner and store the results
				//partitioner->initialise(*naiveConnGraph, comm.size);
				partitioner->partition();

				// I * assignedCellLabels;
				I * assignedCellLabels = NULL;
				I nAssignedCellLabels;

				// ToDo: The use of comm inside the partitioner for this alongside the nparts
				// above needs some tidying up....
				partitioner->assignRankNodes(&assignedCellLabels, &nAssignedCellLabels);

				// (5) Create the Mesh using the MeshSource and the assigned labels
				// Create the Mesh Object based on the template type M
				// This should inherit from UnstructuredMeshInterface so the type constraint is satisfied
				*mesh = new M(comm);
				(*mesh)->addData(*source, assignedCellLabels, nAssignedCellLabels);
				(*mesh)->finalize();
				
				// Cleanup
				delete naiveConnGraph;
				delete partitioner;
				delete source;
				free(assignedCellLabels);

				return cupcfd::error::E_SUCCESS;
			}
		}
	}
}

#endif
