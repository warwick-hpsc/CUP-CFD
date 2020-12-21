/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the ExchangePattern class.
 */

#ifndef CUPCFD_COMM_EXCHANGE_PATTERN_INCLUDE_H
#define CUPCFD_COMM_EXCHANGE_PATTERN_INCLUDE_H

#include "Communicator.h"
#include "mpi.h"
#include <unordered_map>
#include "Error.h"

// ToDo: Note - Would like to extend the template to so that the index type
// is also templated. However, MPI functions take integers, so extending to longs
// would need to be investigated....

namespace cupcfd
{
	namespace comm
	{
		/**
		 * The ExchangePattern class is responsible for setting and recording information
		 * related to the repeated use of the same exchange pattern.
		 * If a one-off is required, directly use of an comm interface operation may be preferable.
		 *
		 * The ExchangePattern class operates on the basis of a process knowing:
		 * (a) Which data elements need to be sent from an array by local index, and to which process
		 * (b) Knowing the unique global index for every element in an array (including those which are
		 * not sent)
		 *
		 * This gives the exchange pattern sufficient information to send, but not to recv.
		 *
		 * From this, this class will determine the necessary information for an exchange, by determining
		 * (a) Which ranks this process should expect to *recv* from
		 * (b) Knowing which elements the process will receive by global ID, and in what order
		 * (c) Mapping these global elements to their local indexes in the array.
		 *
		 * This class does this at initialisation by every process informing the processes they wish
		 * to send to what elements, by global ID, they will be sending. The receiving processes
		 * can then retain this information and use it, along with their knowledge of the
		 * global -> local mapping on their process, to determine where received data should be unpacked to
		 * in a provided data array.
		 *
		 * This relies on a consistent global indexing scheme that does not change for a local array.
		 * If the indexing scheme changes, then a new ExchangePattern would need to be built.
		 * Also only any one process can be responsible for *sending* a specific global ID.
		 * Other ranks can have the global id assigned to a space for receiving data, but cannot send it
		 * (else it is unclear which rank takes priority for sending it). If such a situation occurs,
		 * then there should be two distinct global IDs.
		 *
		 * It also has some caveats in that the allocation of global index mappings on each
		 * rank must be correct.
		 * If Rank 1 wants to communicate Global Index '5' to Rank 0, and Rank 0 has no mapping for a
		 * Global Index '5' to a local index, then an error will occur since it does not know where to
		 * store the received data.
		 *
		 * It also presumes that no process sends duplicate global indexes - if this is the case
		 * then the last received will overwrite others received at the unpack stage.
		 *
		 * ToDo: Error checks need to be added for these scenarios.
		 *
		 * Example of Usage:
		 *
		 * Given the following setup:
		 *
		 * Rank 0: Array 'a'
		 * 	Local Index  [0, 1, 2, 3, 4]
		 * 	mapLocalToExchangeIDX (Global Index) [9, 3, 6, 2, 1]
		 * 	exchangeIDXSend (Array of local indexes to send): [0, 3, 4]
		 * 	tRanks : [1, 1, 1]
		 *
		 * 	This indicates that process 0 wishes to send local indexes 0, 3, 4,
		 * 	(that have global indexes of 9, 2 and 1) to rank 1 for all three.
		 * 	Rank 1 must therefore have space allocated for global indexes 9, 2 and 1.
		 *
		 * Rank 1: Array 'b'
		 * 	Local Index [0, 1, 2, 3]
		 * 	mapLocalToExchangeIDX (Global Index) [3, 1, 6, 9, 2]
		 * 	exchangeIDXSend (Array of local indexes to send): [0, 2]
		 * 	tRanks : [0, 0]
		 *
		 *  This indicates that process 1 wishes to send local indexes 0 and 2,
		 *  that have global indexes of 3 and 6 respectively, to rank 0 for both.
		 *
		 *  From this the pattern will store that rank 0 should expect to receive
		 *  two elements from Rank 1, and rank 1 should expected to receive 3 elements from rank 0.
		 *  It was also store map where the expected received data should be placed when unpacking the recv buffer.
		 *
		 *  Once constructed, this should permit a simple usage of:
		 *  packBuffer (Copy correct positions from an array to the send buffer)
		 *  Exchange (Start and Stop to guarantee data has been received)
		 *  unpackBuffer (To overwrite the correct positions of an array from the recv buffer)
		 *
		 *  When packing and unpacking, the data array must be equal to or greater in the size that the
		 *  mapLocalToExchangeIDX array (else there would be insufficient local indexes to map)
		 *
		 */
		template <class T>
		class ExchangePattern
		{
			public:
				// === Variables ===

				/** Communicator of ranks provided at initialisation **/
				cupcfd::comm::Communicator comm;

				/** Local ID to Global Exchange ID Mapping **/
				std::unordered_map<int, int> localToExchange;

				/** Exchange ID to Local ID Mapping **/
				std::unordered_map<int, int> exchangeToLocal;

				// === Send CSR Data ===

				/** CSR Data - Distinct Processes we are sending to **/
				int * sProc;

				/** CSR Data - Number of distinct processes we are sending to **/
				int nSProc;

				/** CSR Data - Exchange Index Group Lookup for a sending process (Process sProc[i] Indexes = sXAdj[i] -> (sXAdj[i+1]-1) in sAdjncy **/
				int * sXAdj;

				/** CSR Data - Size of nSXAdj in elements (should be number of destination processes + 1) **/
				int nSXAdj;

				/** CSR Data - Exchange Indexes we are sending, grouped by destination process **/
				int * sAdjncy;

				/** CSR Data - Size of sAdjncy (should be number of sending elements) **/
				int nSAdjncy;

				/** CSR Data - Distinct Processes we are receiving from **/
				int * rProc;

				/** CSR Data - Number of distinct processes we are receiving from **/
				int nRProc;

				/** CSR Data - Exchange Index Group Lookup for receiving from a process (Process rProc[i] Indexes = rXAdj[i] -> (rXAdj[i+1]-1) in rAdjncy **/
				int * rXAdj;

				/** CSR Data - Size of nRXAdj in elements (should be number of receiving processes + 1) **/
				int nRXAdj;

				/** CSR Data - Exchange Indexes we are receiving, grouped by source process **/
				int * rAdjncy;

				/** CSR Data - Size of sAdjncy (should be number of receiving elements) **/
				int nRAdjncy;

				// === Constructors/Deconstructors ===

				/**
				 * Constructor.
				 * Sets up initial arrays to be nullptrs and sizes to 0.
				 */
				ExchangePattern();

				/**
				 * Deconstructor
				 */
				virtual ~ExchangePattern();

				// === Other Methods ===

				/**
				 * Initialise by registering an exchange pattern (see class documentation).
				 *
				 * This registers sufficient information to conduct an exchange - i.e.
				 * (a) An array of global 'exchange' indexes possessing a local index that is
				 * their position in the array. This information is necessary for the unpacking
				 * stage on both this and other processes.
				 * (b) An array of global 'exchange' indexes that will be sent in an exchange
				 * (c) An array of corresponding ranks indicating where the values at indexes i in
				 * (b) will be sent to.
				 *
				 * @param pattern the object to be initialised
				 * @param comm The communicator of all participting ranks in an exchange
				 * @param mapLocalToExchangeIDX An array of global 'exchange' indexes that map to the
				 * local indexes they are placed at in this array.
				 * @param nMapLocalToExchangeIDX The size of the mapLocalToExchangeIDX array
				 * @param exchangeIDXSend An array of global exchange indexes indicating which data elements are to be sent
				 * @param nExchangeIDXSend The size of the exchangeIDXSend array
				 * @param tRanks The target ranks for the elements in exchangeIDXSend to be sent to, paired by index
				 * @param nTRanks The size of the tRanks array
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes init(cupcfd::comm::Communicator& comm,
												int * mapLocalToExchangeIDX, int nMapLocalToExchangeIDX,
												int * exchangeIDXSend, int nExchangeIDXSend,
												int * tRanks, int nTRanks);

				// ToDo: These need more error code validations

				/**
				 * Pack the send buffer with data to be exchanged from the provided
				 * data array, using the predefined exchange pattern.
				 *
				 * Data elements are selected from the array based on the configuration
				 * of the ExchangePattern at construction. The local indexes used in the
				 * constructor (exchangeIDXSend) are the indexes used to determine which
				 * values are extracted and packed into the send buffer.
				 *
				 * E.g. If exchangeIDXSend in the constructor was [0, 5, 7, 1], then
				 * given the data array [a, b, c, d, e, f, g, h], the send buffer would contain
				 * [a, f, h, b]
				 *
				 * @param data The source data array to get values from for the send buffer.
				 * The elements used will be based on the mappings specified at initialisation.
				 * @param nData The size of the data array - must be greater than the
				 * number of stored mappings in the exchange pattern.
				 *
				 * @tparam T The datatype of the data to be communicated.
				 * Can be a supported primitive type or a CustomMPIType.
				 *
				 * @return Nothing
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes packSendBuffer(T * data, int nData) = 0;

				/**
				 * Unpack the recv buffer into the provided data array,
				 * using the predefined exchange pattern.
				 *
				 * Data elements are extracted from the recv buffer and placed into particular indexes
				 * in the data array.
				 *
				 * The indexes that these are placed at is determined at constructor time. Most notably,
				 * the constructor does not specify information about receiving data, only what data is being
				 * sent and the global indexes associated with the local indexes of the data array.
				 * The indexes that are updated in 'data' are determined based on what other processes
				 * have indicated they are sending - i.e. the pattern stores information about what
				 * data is being received as a global index, and these global indexes are translated to a local
				 * index and replaced here.
				 *
				 * @param data The data array to update with values from the recv buffer.
				 * The elements updated will be based on the mappings specified at initialisation and which
				 * global indexes the process has received data for.
				 * @param nData The size of the data array - must be greater than the
				 * number of stored mappings in the exchange pattern.
				 *
				 * @tparam T The datatype of the data to be communicated.
				 * Can be a supported primitive type or a CustomMPIType.
				 *
				 * @return Nothing
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes unpackRecvBuffer(T * data, int nData) = 0;

				/**
				 * Begin a data exchange using the stored pattern.
				 *
				 * The provided data array acts as the source for the send buffer.
				 * Not all data may be used, depending on which elements are to be sent.
				 *
				 * The stored global indexes for sends are mapped to the local indexes based on the pattern
				 * mapping, which in turn map to the data elements in the sourceData at those indexes,
				 * and thus sourceData must be as large as the largest local index to be sent on this process.
				 *
				 * This function starts the exchange. It should copy the relevant data to a packed buffer,
				 * and thus it is safe to reuse the sourceData array after this function is complete.
				 *
				 * @param sourceData The data array for retrieving data to be sent/storing received data.
				 * @param nData The size of the sourceData array, in the number of elements of type T.
				 *
				 * @tparam T The datatype of the data to be communicated.
				 * Can be a supported primitive type or a CustomMPIType.
				 *
				 * @return Nothing
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes exchangeStart(T * sourceData, int nData) = 0;

				// ToDo:
				// Error Codes - Exchange not active

				/**
				 * Ends an active data exchange using the stored pattern.
				 *
				 * The provided data array acts as the destination for data from the recv buffer.
				 * Only indexes with received data will be overwritten in sinkData, the rest will remain unchanged.
				 *
				 * The stored global indexes for recieves are mapped to the local indexes based on the pattern
				 * mapping, which in turn map to the data elements in sinkData at those indexes,
				 * and thus sinkData must be as large as the largest local index to be updated on this process.
				 *
				 * It is safe to reuse the sinkData array after this function is complete.
				 *
				 * @param sourceData The data array for retrieving data to be sent/storing received data.
				 * @param nData The size of the sourceData array, in the number of elements of type T.
				 *
				 * @tparam T The datatype of the data to be communicated.
				 * Can be a supported primitive type or a CustomMPIType.
				 *
				 * @return Nothing
				 */
				__attribute__((warn_unused_result))
				virtual cupcfd::error::eCodes exchangeStop(T * sinkData, int nData) = 0;
		};
	}
}

// Include Header Level Definitions
#include "ExchangePattern.ipp"

#endif
