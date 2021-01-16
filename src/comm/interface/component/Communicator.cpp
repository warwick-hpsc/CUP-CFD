/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the definitions for the Communicator class
 */

#include "Communicator.h"
#include "mpi.h"
#include <iostream>


namespace cupcfd
{
	namespace comm
	{
		Communicator::Communicator()
		: comm(MPI_COMM_NULL)
		{
			this->rank = 0;
			this->size = 1;
			this->root_rank = 0;
			this->root = true;

			// std::cout << "Communicator::Communicator(): attempting duplication of MPI_COMM_SELF" << std::endl;
			// std::cout << "Communicator::Communicator(): MPI_COMM_SELF=" << MPI_COMM_SELF << std::endl;
			// // Override the null comm
			// int err = MPI_Comm_dup(MPI_COMM_SELF, &(this->comm));
			// if(err != MPI_SUCCESS) {
			// 	std::cout << "Warning: Error Duplicating MPIComm in MPI Communicator Constructor" << std::endl;
			// }
			// std::cout << "Communicator::Communicator(): this->comm=" << this->comm << std::endl;
			// std::cout << "Communicator::Communicator(): duplication succceded" << std::endl;
			this->comm = MPI_COMM_SELF;
		}

		Communicator::Communicator(MPI_Comm mpiComm)
		: comm(MPI_COMM_NULL)
		{
			std::cout << "Communicator::Communicator(mpiComm): attempting duplication of mpiComm=" << mpiComm << std::endl;

			int err;

			// Duplicate the communicator
			// err = MPI_Comm_dup(mpiComm, &(this->comm));
			// if(err != MPI_SUCCESS) {
			// 	std::cout << "Warning: Error Duplicating MPIComm in MPI Communicator Constructor" << std::endl;
			// }
			// std::cout << "Communicator::Communicator(mpiComm): duplication succceded" << std::endl;
			// std::cout << "Communicator::Communicator(mpiComm): this->comm=" << this->comm << std::endl;
			this->comm = mpiComm;

			// Store basic properties
			err = MPI_Comm_rank(this->comm, &(this->rank));
			if(err != MPI_SUCCESS) {
				std::cout << "Warning: Error Duplicating MPIComm in MPI Communicator Constructor: MPI_Comm_rank() failed" << std::endl;
			}

			err = MPI_Comm_size(this->comm, &(this->size));
			if(err != MPI_SUCCESS) {
				std::cout << "Warning: Error Duplicating MPIComm in MPI Communicator Constructor: MPI_Comm_size() failed" << std::endl;
			}

			// Leave root rank as always zero for now
			this->root_rank = 0;

			// Determine if this process is root
			if(this->rank == this->root_rank) {
				this->root = true;
			}
			else
			{
				this->root = false;
			}
		}

		Communicator::Communicator(Communicator& comm)
		: comm(MPI_COMM_NULL)
		{
			// Use the copy operator
			std::cout << "Communicator::Communicator(comm): attempting copy of Communicator object (comm=" << comm.comm << ")" << std::endl;

			// *this = comm;
			this->rank = comm.rank;
			this->size = comm.size;
			this->root = comm.root;
			this->root_rank = comm.root_rank;

			// std::cout << "Communicator::Communicator(comm): comm.comm=" << comm.comm << std::endl;
			// int err = MPI_Comm_dup(comm.comm, &(this->comm));
			// if(err != MPI_SUCCESS) {
			// 	std::cout << "Warning: Error Duplicating MPIComm in MPI Communicator Constructor" << std::endl;
			// }
			// std::cout << "Communicator::Communicator(comm): this->comm=" << this->comm << std::endl;
			this->comm = comm.comm;

			std::cout << "Communicator::Communicator(comm): copy succeeded" << std::endl;
		}

		Communicator::~Communicator() {
			// === Cleanup ===

			// Stored MPI Comms are all duplicated from other MPI Communicators.
			// They should be released here else we will lose track of the duplicates.
			MPI_Comm_free(&(this->comm));
		}

		Communicator * Communicator::clone() {
			return new Communicator(*this);
		}

	} // namespace comm
} // namespace cupcfd
