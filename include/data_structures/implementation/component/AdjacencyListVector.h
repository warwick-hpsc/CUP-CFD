/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the AdjacencyListVector Class
 */

#ifndef CUPCFD_DATA_STRUCTURES_ADJACENCY_LIST_VECTOR_INCLUDE_H
#define CUPCFD_DATA_STRUCTURES_ADJACENCY_LIST_VECTOR_INCLUDE_H

#include <vector>
#include <map>
#include <unordered_map>

#include "AdjacencyList.h"

namespace cupcfd
{
	namespace data_structures
	{
		/**
		 * This class stores adjacency list data using vectors.
		 * Specifically, every node has a vector bucket associated
		 * with it that contains the identifiers of the adjacent nodes.
		 *
		 * @tparam I The type of the indexing scheme
		 * @tparam T The type of the stored node data
		 */
		template<class I, class T>
		class AdjacencyListVector : public AdjacencyList<AdjacencyListVector<I,T>, I, T>
		{
			public:
				// === Members ===
				// Each index of adjacencies maps to an internal index for a node.
				// This contains another vector, which is the list of internal node indexes
				// that are adjacent to that node.
				// This can then be used to lookup the node identifier from the maps defined above.

				/**
				 * All node buckets, indexed by the unique internal node identifier/index
				 * assigned when a node is added.
				 **/
				std::vector<std::vector<I>> adjacencies;

				// === Constructors/Deconstructors ===

				/**
				 * Default constructor:
				 * Sets up internal data structures and sets sizes to 0
				 */
				AdjacencyListVector();

				/**
				 * Deconstructor:
				 * Cleans up any internal dynamically allocated structures.
				 */
				virtual ~AdjacencyListVector();

				// === Concrete Methods ===

				/**
				 * Copy the edge and node data from source to this adjacency list
				 *
				 * @param source The adjacency list to copy from
				 *
				 * @tparam C The implementing class of the *destination* (this) adjacency list.
				 * @tparam S The implementing class of the *source* adjacency list (may differ from this
				 * adjacency list)
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the stored node data
				 *
				 * @return Nothing
				 */
				template <class S>
				void operator=(AdjacencyList<S,I,T>& source);

				// === CRTP Methods ===

				cupcfd::error::eCodes reset();

				cupcfd::error::eCodes addNode(T node);

				cupcfd::error::eCodes existsNode(T node, bool * exists);

				cupcfd::error::eCodes addEdge(T node, T adjNode);

				cupcfd::error::eCodes existsEdge(T srcNode, T dstNode, bool * exists);

				cupcfd::error::eCodes getAdjacentNodeCount(T node, I * count);

				cupcfd::error::eCodes getAdjacentNodes(T node, T * adjNodes, I nAdjNodes);
		};
	} // namespace data_structures
} // namespace cupcfd

// Include Header Level Definitions
#include "AdjacencyListVector.ipp"

#endif
