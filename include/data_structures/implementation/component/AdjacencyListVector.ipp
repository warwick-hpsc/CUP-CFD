/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains header level definitions for the AdjacencyListVector class
 */

#ifndef CUPCFD_DATA_STRUCTURES_ADJACENCY_LIST_VECTOR_IPP_H
#define CUPCFD_DATA_STRUCTURES_ADJACENCY_LIST_VECTOR_IPP_H

namespace cupcfd
{
	namespace data_structures
	{
		template <class I, class T>
		template <class S>
		void AdjacencyListVector<I,T>::operator=(AdjacencyList<S,I,T>& source)
		{
			// Forward operation to base class
			AdjacencyList<AdjacencyListVector<I,T>, I,T>::operator=(source);
		}
	}
}

#endif
