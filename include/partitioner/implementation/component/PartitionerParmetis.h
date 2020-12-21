/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Description
 *
 */

#ifndef CUPCFD_PARTITIONER_PARMETIS_INCLUDE_H
#define CUPCFD_PARTITIONER_PARMETIS_INCLUDE_H

#include "PartitionerInterface.h"

#include "parmetis.h"

namespace cupcfd
{
	namespace partitioner
	{
		/**
		 * Partitioner interface for using the ParMETIS partitioner library
		 *
		 * @tparam I The type of the indexing scheme
		 * @tparam T The type of the stored array data
		 */
		template <class I, class T>
		class PartitionerParmetis : public PartitionerInterface<I, T>
		{
			public:
				// =================
				// === Variables ===
				// =================

				// ToDo: Fix/error check use of idx_t - will possibly break if longs used instead for I?

				/** Index Numbering Scheme - 0 for base-0 **/
				idx_t numflag;

				/** parmetis Options - Stores minimal set for now **/
				idx_t options[1];

				/** Number of edges that are cut by partitioning (set by partkway) **/
				idx_t edgecut;

				/** Fraction of vertex weights for each subdomain **/
				idx_t nCon;

				// === Parallel Attributes ===

				// === Weight Arrays ===

				/** Weight active flag **/
				idx_t wgtflag;

				/** Vertex Weights **/
				idx_t * vwgt;

				/** Size of vwgt in number of elements of type idx_t **/
				I nVwgt;

				/** Edge Weights **/
				idx_t * adjwgt;

				/** Size of adjwgt in number of elements of type idx_t **/
				I nAdjwgt;

				/** Imbalance tolerance for vertex weights across sub-domains **/
				real_t * ubvec;

				/** Size of ubvec in number of elements of type real_t **/
				I nUbvec;

				/** Fraction of vertex weight distributed across each subdomain **/
				real_t * tpwgts;

				/** Size of nTpwgts in number of elements of type real_t **/
				I nTpwgts;

				// === Partitioner Work Arrays ===

				/** xadj CSR array as used by parmetis **/
				idx_t * xadj;

				/** Size of xadj in number of elements of type idx_t **/
				I nXAdj;

				/** adjncy CSR array as used by parmetis **/
				idx_t * adjncy;

				/** Size of adjncy in number of elements of type idx_t **/
				I nAdjncy;

				/** vtxdist array as used by parmetis **/
				idx_t * vtxdist;

				/** Size of vtxdist in number of elements of type idx_t **/
				I nVtxdist;

				// =================
				// ==== Methods ====
				// =================

				// Constructor/Deconstructors
				/**
				 * Default constructor. Creates the object and sets all arrays to nullptr,
				 * values to 0. Will require the initialise of all arrays before it can be
				 * used for partitioning.
				 */
				PartitionerParmetis(cupcfd::comm::Communicator& workComm);

				/**
				 * Create a partitioner object and setup the internal data structures
				 * to be ready for running the partitioner.
				 *
				 * This constructor sources the data it needs to partition from the provided graph.
				 * All process rank members of the graph's communicator must participate in the
				 * construction of this Partitioner, as well as the partitioning later (since they hold
				 * data).
				 *
				 * @param sourceGraph The graph to partition
				 * @param nParts The number of parts to partition the graph into
				 * @param nCon The value to set nCon to for Parmetis
				 */
				PartitionerParmetis(cupcfd::data_structures::DistributedAdjacencyList<I,T>& sourceGraph, int nParts, int nCon);

				/**
				 * Deconstructor for PartitionerParmetis
				 */
				~PartitionerParmetis();

				// === Concrete Methods ===

				/**
				 * Reset any data storage used as either temporary or permanent space for partitioning work.
				 * This will leave the data storage unallocated and set to nullptr.
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored array data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes resetWorkArrays();

				/**
				 * This method takes a distributed graph and stores the node/edge data stored on this process in a manner
				 * suitable for use by the partitioner, creating, structuring and storing it in suitable members
				 * of this object. Depending on the use of a serial or parallel partitioner, this may involve redistribution
				 * of data across the ranks for partitioning. Ideally, this graph should have a balanced spread of data
				 * across the MPI process ranks.
				 *
				 * This will reset any existing internal data arrays - data stored within them will be lost.
				 *
				 * @param distGraph The graph acting as a data source
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored array data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_DISTGRAPH_UNFINALIZED Error: The distributed graph is unfinalised
				 * @retval cupcfd::error::E_DISTGRAPH_NO_LOCAL_NODES Error: The distributed graph has no local nodes on this process
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes setWorkArrays(cupcfd::data_structures::DistributedAdjacencyList<I, T>& distGraph);


				/**
				 * Reset any data storage used to store edge/vertex weights for parmetis input.
				 * This will leave the data storage unallocated and set to nullptr, with sizes of 0.
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes resetVertexEdgeWeights();

				/**
				 * Reset any data storage used to store subdomain weights for parmetis input.
				 * This will leave the data storage unallocated and set to nullptr, with sizes of 0.
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes resetSubdomainWeights();

				/**
				 * Reset any data storage used to store vertex imbalance weights for parmetis input.
				 * This will leave the data storage unallocated and set to nullptr, with sizes of 0.
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes resetVertexImbalanceWeights();

				/**
				 * Set the ncon values for parmetis
				 * This will also reset the subdomain weight arrays and vertex imbalance
				 * weights arrays due to the change in size necessitating a later reallocation.
				 *
				 * @param nCon The number value of nCon
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes setNCon(I nCon);

				/**
				 * Set the weight arrays. This currently does not do anything, and required
				 * weight arrays are set to defaults via other functions where appropriate.
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes setWeightArrays();

				/**
				 * Set the subdomain weight arrays. This allocates memory based on the values
				 * of nParts and nCon in the object, and sets the values to a default of
				 * 1.0/nParts.
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_PARMETIS_INVALID_NCON Error: The object has an ncon value of less than 1
				 * @retval cupcfd::error::E_PARMETIS_INVALID_NPARTS Error: The object has an nParts value of less than 1
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes setSubdomainWeightArrays();

				/**
				 * Set the vertex imbalance weight arrays. This allocates memory based on the values
				 * of nCon in the object, and sets the values to a default of 1.05 at every index.
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_PARMETIS_INVALID_NCON Error: The object has an ncon value of less than 1
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes setVertexImbalanceWeightArrays();



				// === Inherited Overloaded Methods ===

				/**
				 * Set the number of partitions the graph space should be split into.
				 * This will also reset the subdomain weight arrays due to the change in size.
				 *
				 * @param nParts The number of desired partitions for the partitioner results
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes setNParts(I nParts);

				/**
				 * Call the parmetis library to partition in a distributed manner,
				 * using the data and parameters stored in the PartitionerParmetis object.
				 * Note: This will overwrite any previously stored partitioning results
				 * Note: This is a blocking call - all members of the work communicator must call it.
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_PARMETIS_INVALID_WORK_ARRAY Error: One or more of the work arrays are unset
				 * @retval cupcfd::error::E_PARMETIS_INVALID_SUBDOMAIN_WEIGHT_ARRAYS Error: One or more of the subdomain weight arrays are unset
				 * @retval cupcfd::error::E_PARMETIS_INVALID_VERTEX_IMBALANCE_WEIGHT_ARRAYS Error: One or more of the vertex imbalance weight arrays are unset
				 * @retval cupcfd::error::E_PARMETIS_INVALID_NCON Error: The object has an ncon value of less than 1
				 * @retval cupcfd::error::E_PARMETIS_INVALID_NPARTS Error: The value of nParts is 1 or less
				 * @retval cupcfd::error::E_PARMETIS_LIBRARY_ERROR Error: An unexpected error was encountered from the parmetis library
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes partition();

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes initialise(cupcfd::data_structures::DistributedAdjacencyList<I, T>& graph, I nParts);

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes reset();

				// === Concrete Methods ===

			protected:
				// === Variables ===

				// === Methods ===
				// Inherited Overrides
		};
	}
}

// Include Header Level Definitions
#include "PartitionerParmetis.ipp"

#endif
