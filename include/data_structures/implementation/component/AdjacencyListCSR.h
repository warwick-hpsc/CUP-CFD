/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the AdjacencyListCSR class
 */

#ifndef CUPCFD_DATA_STRUCTURES_ADJACENCY_LIST_CSR_INCLUDE_H
#define CUPCFD_DATA_STRUCTURES_ADJACENCY_LIST_CSR_INCLUDE_H

#include <vector>
#include <map>
#include <cstdlib>
#include <unordered_map>

#include "AdjacencyListVector.h"
#include "AdjacencyList.h"
#include "Error.h"

namespace cupcfd
{
	namespace data_structures
	{
		template<class I, class T>
		class AdjacencyListCSR : public AdjacencyList<AdjacencyListCSR<I,T>, I,T>
		{
			public:
				// === Members ===
				// Data Storage (CSR)

				/**
				 * xadj component of CSR - index into adjncy for neighbouring nodes by local index
				 **/
				std::vector<I> xadj;

				/**
				 * Array of neighbouring nodes, grouped by node, indexed lookup via xadj
				 **/
				std::vector<I> adjncy;

				// === Constructors/Deconstructors ===

				/**
				 * Default constructor - Sets up internal data structures and sets sizes to 0
				 */
				AdjacencyListCSR();

				/**
				 * Deconstructor
				 */
				virtual ~AdjacencyListCSR();

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
	} // namespace adjacency_list
} // namespace cupcfd

// Include Header Level Definitions
#include "AdjacencyListCSR.ipp"

#endif
