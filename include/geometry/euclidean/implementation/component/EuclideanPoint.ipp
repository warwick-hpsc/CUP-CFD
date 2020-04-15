/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * This file contains header level definitions for the N-dimensional EuclideanPoint class
 */

#ifndef CUPCFD_GEOMETRY_EUCLIDEAN_POINT_IPP_H
#define CUPCFD_GEOMETRY_EUCLIDEAN_POINT_IPP_H

#include "EuclideanVector.h"

#include <iostream>

namespace cupcfd
{
	namespace geometry
	{
		namespace euclidean
		{
			template <class T, unsigned int N>
			EuclideanPoint<T,N>::EuclideanPoint()
			{
				for(int i = 0; i < N; i++)
				{
					this->cmp[i] = T(0);
				}
			}

			template <class T, unsigned int N>
			EuclideanPoint<T,N>::EuclideanPoint(const EuclideanPoint<T,N> &point)
			{
				for(int i = 0; i < N; i++)
				{
					this->cmp[i] = point.cmp[i];
				}
			}
			
			template <class T, unsigned int N>
			inline void EuclideanPoint<T,N>::operator=(const EuclideanPoint<T,N>& source)
			{
				for(int i = 0; i < N; i++)
				{
					this->cmp[i] = source.cmp[i];
				}
			}
			
			template <class T, unsigned int N>
			inline void EuclideanPoint<T,N>::operator=(const T scalar)
			{
				for(int i = 0; i < N; i++)
				{
					this->cmp[i] = scalar;
				}
			}
						
			template <class T, unsigned int N>
			inline cupcfd::error::eCodes EuclideanPoint<T,N>::getMPIType(MPI_Datatype * dType)
			{
				if(!(this->isRegistered()))
				{
					return cupcfd::error::E_MPI_DATATYPE_UNREGISTERED;
				}
			
				*dType = EuclideanPoint<T,N>::mpiType;

				return cupcfd::error::E_SUCCESS;
			}
			
			template <class T, unsigned int N>
			inline MPI_Datatype EuclideanPoint<T,N>::getMPIType()
			{
				return EuclideanPoint<T,N>::mpiType;
			}
			
			template <class T, unsigned int N>
			inline bool EuclideanPoint<T,N>::isRegistered()
			{
				return EuclideanPoint<T,N>::mpiDataTypeReg;
			}
			
			template <class T, unsigned int N>
			cupcfd::error::eCodes EuclideanPoint<T,N>::registerMPIType()
			{
				// Error Check - Only Register if currently unregistered
				if(this->isRegistered())
				{
					return cupcfd::error::E_MPI_DATATYPE_REGISTERED;
				}

				int mpiErr;

				// Only need one block since all of same type
				int count = 1;

				// One block on contigious memory for the component array, N elements for an N length vector
				int blocklengths[1] = {N};
				MPI_Aint displ[1];
				MPI_Datatype structTypes[1];

				// Get the mpi type of a euclidean vector component - all components should share the same type
				MPI_Datatype componentType;
				cupcfd::comm::mpi::getMPIType(this->cmp[0], &componentType);
				structTypes[0] = componentType;

				displ[0] = (MPI_Aint) offsetof(class EuclideanPoint, cmp);

				MPI_Datatype vecType;
				MPI_Datatype vecTypeResized;

				mpiErr = MPI_Type_create_struct(1, blocklengths, displ, structTypes, &vecType);

				if(mpiErr != MPI_SUCCESS)
				{
					return cupcfd::error::E_MPI_ERR;
				}

				mpiErr = MPI_Type_commit(&vecType);

				if(mpiErr != MPI_SUCCESS)
				{
					return cupcfd::error::E_MPI_ERR;
				}

				mpiErr = MPI_Type_create_resized(vecType, displ[0], (MPI_Aint) sizeof(class EuclideanPoint<T,N>), &vecTypeResized);
				if(mpiErr != MPI_SUCCESS)
				{
					return cupcfd::error::E_MPI_ERR;
				}

				mpiErr = MPI_Type_commit(&vecTypeResized);
				if(mpiErr != MPI_SUCCESS)
				{
					return cupcfd::error::E_MPI_ERR;
				}

				mpiErr = MPI_Type_commit(&vecTypeResized);
				if(mpiErr != MPI_SUCCESS)
				{
					return cupcfd::error::E_MPI_ERR;
				}

				// Store statically so the get method can retrieve it later
				EuclideanPoint<T,N>::mpiType = vecTypeResized;

				// Cleanup - Don't need the unresized type
				mpiErr = MPI_Type_free(&vecType);
				if(mpiErr != MPI_SUCCESS)
				{
					return cupcfd::error::E_MPI_ERR;
				}

				EuclideanPoint<T,N>::mpiDataTypeReg = true;

				return cupcfd::error::E_SUCCESS;
			}

			template <class T, unsigned int N>
			cupcfd::error::eCodes EuclideanPoint<T,N>::deregisterMPIType()
			{
				int mpiErr;

				// Error Check - Only Deregister if currently registered
				if(!this->isRegistered())
				{
					return cupcfd::error::E_MPI_DATATYPE_UNREGISTERED;
				}

				mpiErr = MPI_Type_free(&(EuclideanPoint<T,N>::mpiType));
				if(mpiErr != MPI_SUCCESS)
				{
					return cupcfd::error::E_MPI_ERR;
				}

				EuclideanPoint<T,N>::mpiDataTypeReg = false;
				return cupcfd::error::E_SUCCESS;
			}		
			
			template <class T, unsigned int N>
			inline void EuclideanPoint<T,N>::print()
			{
				for(int i = 0; i < N; i++)
				{
					std::cout << "P_" << i << ": " << this->cmp[i] << ", ";
				}
			}
		} // namespace euclidean
	} // namespace geometry
} // namespace cupcfd

#endif
