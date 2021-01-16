/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the header level definitions for the Communicator class
 */

#ifndef CUPCFD_COMM_COMMUNICATOR_IPP_H
#define CUPCFD_COMM_COMMUNICATOR_IPP_H

#include <mpi.h>
#include <iostream>

namespace cupcfd
{
	namespace comm
	{
		inline void Communicator::operator=(Communicator& source) {		
			int err;
			// Free this value if it is currently not initialised
			// (e.g. during a clone operation)
			if(this->comm != MPI_COMM_NULL) {
			// if(this->comm != MPI_COMM_NULL && this->comm != MPI_COMM_SELF && this->comm != MPI_COMM_WORLD) {
				err = 
				MPI_Comm_free(&(this->comm));
				if(err != MPI_SUCCESS) {
					std::cout << "Warning: Error Duplicating MPIComm: MPI_Comm_free() failed" << std::endl;
				}
			}
			// Create a Duplicate Communicator
			err = MPI_Comm_dup(source.comm, &(this->comm));
			if(err != MPI_SUCCESS) {
				std::cout << "Warning: Error Duplicating MPIComm: MPI_Comm_dup() failed" << std::endl;
			}
			// this->comm = source.comm;
			
			// Copy Values
			this->rank = source.rank;
			this->size = source.size;
			this->root = source.root;
			this->root_rank = source.root_rank;
		}
	} // namespace comm
} // namespace cupcfd

#endif
