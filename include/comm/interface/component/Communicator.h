/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the declarations for the Communicator class
 */

#ifndef CUPCFD_COMM_COMMUNICATOR_INCLUDE_H
#define CUPCFD_COMM_COMMUNICATOR_INCLUDE_H

#include <mpi.h>

namespace cupcfd
{
	namespace comm
	{
		/**
		 * Class that stores basic information about the process
		 * and it's state within a communicator for parallel communications
		 * (e.g. size, rank ID etc).
		 *
		 * MPI Communicators are duplicated - they do not directly use the original.
		 *
		 * ToDo: This is an 'interface' communicator, however it currently has MPI
		 * directly in it. However, since we currently only have MPI implemented
		 * this should be fine for now.
		 */
		class Communicator
		{
			public:

				/** Rank in Communicator **/
				int rank;

				/** Size of Communicator **/
				int size;

				/** Store whether this rank is the root rank of the communicator **/
				bool root;

				/** Store which rank is the root rank of the communicator **/
				int root_rank;

				/** Store the MPI Communicator ID **/
				MPI_Comm comm;

				/**
				 * Sets up a Communicator that uses MPI_SELF with a size of 1.
				 */
				Communicator();

				/**
				 * Sets up a communicator that copies the values of the provided
				 * communicator.
				 *
				 * Any MPI_Comms are duplicated.
				 */
				Communicator(Communicator& comm);

				/**
				 * Sets up a Communicator that uses a duplicate of the provided
				 * MPI Communicator, as well as storing this ranks properties.
				 *
				 * @param mpiComm The MPI Communicator to duplicate.
				 */
				Communicator(MPI_Comm mpiComm);

				/**
				 * Deconstructor:
				 * Frees the internally stored duplicate MPI Communicator.
				 */
				~Communicator();

				/**
				 * Copy the communicator properties from the source to this communicator,
				 * duplicating the MPI communicator in source.
				 *
				 * @param source The object to copy the values from
				 *
				 * @return Returns nothing
				 */
				inline void operator=(Communicator& source);

				/**
				 * Clones the current object to create a new object
				 */
				__attribute__((warn_unused_result))
				Communicator * clone();

		};
	} // namespace comm
} // namespace cupcfd

// Include Header Level Definitions
#include "Communicator.ipp"

#endif
