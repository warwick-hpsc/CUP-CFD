/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains header level definitions for the AdjacencyListCSR class
 */

#ifndef CUPCFD_DATA_STRUCTURES_ADJACENCY_LIST_CSR_IPP_H
#define CUPCFD_DATA_STRUCTURES_ADJACENCY_LIST_CSR_IPP_H

namespace cupcfd
{
	namespace data_structures
	{
		template <class I, class T>
		template <class S>
		void AdjacencyListCSR<I,T>::operator=(AdjacencyList<S,I,T>& source)
		{
			// Forward operation to base class
			AdjacencyList<AdjacencyListCSR<I,T>, I,T>::operator=(source);
		}
	}
}

#endif
