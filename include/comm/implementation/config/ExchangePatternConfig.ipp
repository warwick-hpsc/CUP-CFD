/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains header level definitions for the ExchangePatternConfig class
 */

#ifndef CUPCFD_COMM_EXCHANGEPATTERN_CONFIG_IPP_H
#define CUPCFD_COMM_EXCHANGEPATTERN_CONFIG_IPP_H

#include "ExchangePatternOneSidedNonBlocking.h"
#include "ExchangePatternTwoSidedNonBlocking.h"

#include "ArrayDrivers.h"

namespace cupcfd
{
	namespace comm
	{
		inline ExchangeMethod ExchangePatternConfig::getExchangeMethod()
		{
			return this->method;
		}	
		
		inline void ExchangePatternConfig::setExchangeMethod(ExchangeMethod method)
		{
			this->method = method;
		}
		
		inline void ExchangePatternConfig::operator=(const ExchangePatternConfig& source)
		{
			this->method = source.method;
		}
		
		inline ExchangePatternConfig * ExchangePatternConfig::clone()
		{
			return new ExchangePatternConfig(*this);
		}
		
		template <class I, class T, class N>
		cupcfd::error::eCodes ExchangePatternConfig::buildExchangePattern(ExchangePattern<T> ** pattern, 
																			   cupcfd::data_structures::DistributedAdjacencyList<I, N>& graph)
		{
			// Create a pattern of an appropriate type.
			ExchangeMethod method = this->getExchangeMethod();

			if(method == EXCHANGE_NONBLOCKING_ONE_SIDED)
			{
				*pattern = new ExchangePatternOneSidedNonBlocking<T>();
			}
			else if(method == EXCHANGE_NONBLOCKING_TWO_SIDED)
			{
				*pattern = new ExchangePatternTwoSidedNonBlocking<T>();
			}

			// Items needed to initialise the exchange pattern
			// (a) Communicator (taken from graph)
			// (b) Mapping of Local to Exchange IDs - this will be the global ID mapping for any local or ghost cells
			// store on this process.
			// This is important since the mapping order must be the same as for any data arrays, (i.e. cell '5' has
			// to be in the same place in this mapping and in the data array) so it needs to be consistent.
			// (c) Number of elements in mapping array - see above
			// (d)/(e)Pairwise linked array that contain a data element to send, by global id, and the process it is going to
			// This is a pair of a ghost nodes global id and the process that owns that ghost node locally.

			// Mapping
			I nMapLocalToExchangeIDX = graph.connGraph.nNodes;
			I * mapLocalToExchangeIDX = (I *) malloc(sizeof(I) * nMapLocalToExchangeIDX);


			for(I i = 0; i < nMapLocalToExchangeIDX; i++)
			{
				I localID = i;
				I globalID = graph.nodeToGlobal[graph.connGraph.IDXToNode[localID]];
				mapLocalToExchangeIDX[i] = globalID;
			}

			// Compile arrays of the data elements we are going to send, and to which processes.
			// This is basically the list of ghost cells and their target processes

			// ToDo: For now we can reuse the send/recv CSRs that the adjacency list builds
			// May wish to move that data/computation out to this drivers, since it is only needed for
			// exchange patterns, so it could be wrapped into an exchange pattern object in graph instead.

			I nTRanks = graph.sendGlobalIDsAdjncy.size();
			I * tRanks = (I *) malloc(sizeof(I) * nTRanks);

			I iLimit;
			iLimit = cupcfd::utility::drivers::safeConvertSizeT<I>(graph.sendGlobalIDsXAdj.size());
			for (I i = 0; i < iLimit; i++)
			{
				for(I j = 0; j < graph.sendGlobalIDsXAdj[i+1] - graph.sendGlobalIDsXAdj[i]; j++)
				{
					tRanks[graph.sendGlobalIDsXAdj[i] + j] = graph.sendRank[i];
				}
			}


			(*pattern)->init(*(graph.comm),
						 mapLocalToExchangeIDX, nMapLocalToExchangeIDX,
						 &(graph.sendGlobalIDsAdjncy[0]), graph.sendGlobalIDsAdjncy.size(),
						 tRanks, nTRanks);

			free(mapLocalToExchangeIDX);
			free(tRanks);

			return cupcfd::error::E_SUCCESS;
		}
	}
}

#endif
