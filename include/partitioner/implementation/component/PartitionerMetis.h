/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the PartitionerMetis class
 */

#ifndef CUPCFD_PARTITIONER_METIS_INCLUDE_H
#define CUPCFD_PARTITIONER_METIS_INCLUDE_H

#include "PartitionerInterface.h"
#include "DistributedAdjacencyList.h"
#include "AdjacencyListCSR.h"
#include "metis.h"

namespace cupcfd
{
	namespace partitioner
	{
		/**
		 * Partitioner interface for using the METIS partitioner library
		 *
		 * @tparam I The type of the indexing scheme
		 * @tparam T The type of the stored array data
		 */
		template <class I, class T>
		class PartitionerMetis : public PartitionerInterface<I,T>
		{
			public:
				// === Members ===

				// ToDo: Fix/error check use of idx_t - will possibly break if longs used instead for I?

				/** Index Numbering Scheme - 0 for base-0 **/
				idx_t numflag;

				/** Metis Options - Stores minimal set for now **/
				idx_t options[1];

				/** xadj CSR array as used by metis **/
				idx_t * xadj;

				/** Size of xadj in the number of elements of type T **/
				I nXAdj;

				/** adjncy CSR array as used by metis **/
				idx_t * adjncy;

				/** Size of adjncy in the number of elements of type T **/
				I nAdjncy;

				/** Fraction of vertex weights for each subdomain **/
				idx_t nCon;

				/** Returned by Metis PartKWay Function **/
				idx_t objval;

				// === Methods ===
				// Constructor/Deconstructors

				/**
				 * Create a default Partitioner object.
				 * Initially this leaves the many work arrays and result arrays unset.
				 * they can be set in one of two ways - directly, or via the use of a function
				 * like partition that will setup the various member values and arrays where needed.
				 *
				 * @param workComm Communicator of all processes creating a partitioning object that
				 * could potentially receive partition assignments.
				 * For METIS, only the root process will perform the partitioning work.
				 */
				PartitionerMetis(cupcfd::comm::Communicator workComm);

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
				 * @param nCon The value to set nCon to for Metis
				 */
				PartitionerMetis(cupcfd::data_structures::DistributedAdjacencyList<I,T>& sourceGraph, int nParts, int nCon);

				/**
				 * Deconstructor for a PartitionerMetis object.
				 * This will free up any memory being used to store result or node data.
				 */
				~PartitionerMetis();

				// === Metis Specific Concrete Methods ===

				/**
				 * Clear and unallocate the internal CSR Data stores.
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored array data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes resetWorkArrays();

				/**
				 * Set the internal CSR structured data stores needed for the METIS library.
				 * This will also initialise the node arrays.
				 *
				 * @param graph The graph to retrieve the connectivity data from.
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored array data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes setWorkArrays(cupcfd::data_structures::AdjacencyListCSR<I, T>& graph);

				/**
				 * Set the nCon member value of this object
				 *
				 * @param nCon The value to set the object nCon value to.
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored array data
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes setNCon(I nCon);

				// === Overriden Inherited Methods ===

				__attribute__((warn_unused_result))
				cupcfd::error::eCodes assignRankNodes(T** rankNodes, I * nNodes);
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes partition();
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes initialise(cupcfd::data_structures::DistributedAdjacencyList<I, T>& graph, I nParts);
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes reset();
		};
	}
}

// Include Header Level Definitions
#include "PartitionerMetis.ipp"

#endif
