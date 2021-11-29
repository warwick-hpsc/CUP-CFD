/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * This class contains the implementation of the EuclideanVector class.
 */

#include "EuclideanVector.h"
#include "ArithmeticKernels.h"

#include <cmath>

namespace arth = cupcfd::utility::arithmetic::kernels;

namespace cupcfd
{
	namespace geometry
	{
		namespace euclidean
		{
			// Initialise Static Variables
			template <class T, unsigned int N>
			bool EuclideanVector<T,N>::mpiDataTypeReg = false;

			// === Members ===

			template <class T, unsigned int N>
			MPI_Datatype EuclideanVector<T,N>::mpiType = reinterpret_cast<MPI_Datatype>(-1);;

			// === Constructors/Deconstructors ===
			template <class T, unsigned int N>
			EuclideanVector<T,N>::EuclideanVector()
			: CustomMPIType()
			{
				for(uint i = 0; i < N; i++)
				{
					this->cmp[i] = T(0);
				}
			}

			template <class T, unsigned int N>
			EuclideanVector<T,N>::EuclideanVector(T scalar)
			: CustomMPIType()
			{
				for(uint i = 0; i < N; i++)
				{
					this->cmp[i] = scalar;
				}
			}

			template <class T, unsigned int N>
			EuclideanVector<T,N>::EuclideanVector(const EuclideanVector& v)
			: CustomMPIType()
			{
				for(uint i = 0; i < N; i++)
				{
					this->cmp[i] = v.cmp[i];
				}
			}

			template <class T, unsigned int N>
			template <class...Args>
			EuclideanVector<T,N>::EuclideanVector(Args...src)
			: CustomMPIType(),
			  cmp{ (src)... }
			{
				static_assert(sizeof...(Args) == N, "EuclideanVector constructor dimension does not match number of parameters");
			}

			template <class T, unsigned int N>
			EuclideanVector<T,N>::~EuclideanVector() {

			}

			// === Concrete Methods ===

			template <class T, unsigned int N>
			T EuclideanVector<T,N>::dotProduct(const EuclideanVector<T,N>& vec) const {
				// https://en.wikipedia.org/wiki/Dot_product
				T dotP = 0.0;
				for(uint i = 0; i < N; i++) {
					dotP += (this->cmp[i] * vec.cmp[i]);
				}
				return dotP;
			}

			template <class T, unsigned int N>
			double EuclideanVector<T,N>::length() {
				// https://en.wikipedia.org/wiki/Euclidean_vector#length
				// Use square root of dot-product with itself
				T dotP = this->dotProduct(*this);
				double length = arth::sqrtWr(dotP);
				return length;
			}

			template <class T, unsigned int N>
			void EuclideanVector<T,N>::normalise() {
				T scalar = this->length();
				if (scalar != T(0)) {
					scalar = T(1.0) / scalar;
					for(uint i = 0; i < N; i++) {
						this->cmp[i] *= scalar;
					}
				}
			}

			template <class T, unsigned int N>
			void EuclideanVector<T,N>::normalise(EuclideanVector<T,N>& result) {
				// T scalar;
				// this->length(&scalar);
				T scalar = this->length();
				scalar = T(1.0) / scalar;
				for(uint i = 0; i < N; i++) {
					result.cmp[i] = this->cmp[i] * T(scalar);
				}
				// return cupcfd::error::E_SUCCESS;
			}

			template <class T, unsigned int N>
			cupcfd::error::eCodes EuclideanVector<T,N>::registerMPIType() {
				// Error Check - Only Register if currently unregistered
				if(this->isRegistered()) {
					return cupcfd::error::E_MPI_DATATYPE_REGISTERED;
				}

				int mpiErr;

				// Only need one block since all of same type

				// One block on contigious memory for the component array, N elements for an N length vector
				int blocklengths[1] = {N};
				MPI_Aint displ[1];
				MPI_Datatype structTypes[1];

				// Get the mpi type of a euclidean vector component - all components should share the same type
				MPI_Datatype componentType;
				cupcfd::comm::mpi::getMPIType(this->cmp[0], &componentType);
				structTypes[0] = componentType;

				// displ[0] = (MPI_Aint) offsetof(class EuclideanVector, cmp);
				displ[0] = (MPI_Aint) ( (char*)&(this->cmp) - (char*)this );

				MPI_Datatype vecType;
				MPI_Datatype vecTypeResized;

				mpiErr = MPI_Type_create_struct(1, blocklengths, displ, structTypes, &vecType);

				if(mpiErr != MPI_SUCCESS) {
					return cupcfd::error::E_MPI_ERR;
				}

				mpiErr = MPI_Type_commit(&vecType);

				if(mpiErr != MPI_SUCCESS) {
					return cupcfd::error::E_MPI_ERR;
				}

				mpiErr = MPI_Type_create_resized(vecType, displ[0], (MPI_Aint) sizeof(class EuclideanVector<T,N>), &vecTypeResized);
				if(mpiErr != MPI_SUCCESS) {
					return cupcfd::error::E_MPI_ERR;
				}

				mpiErr = MPI_Type_commit(&vecTypeResized);
				if(mpiErr != MPI_SUCCESS) {
					return cupcfd::error::E_MPI_ERR;
				}

				mpiErr = MPI_Type_commit(&vecTypeResized);
				if(mpiErr != MPI_SUCCESS) {
					return cupcfd::error::E_MPI_ERR;
				}

				// Store statically so the get method can retrieve it later
				EuclideanVector<T,N>::mpiType = vecTypeResized;

				// Cleanup - Don't need the unresized type
				mpiErr = MPI_Type_free(&vecType);
				if(mpiErr != MPI_SUCCESS) {
					return cupcfd::error::E_MPI_ERR;
				}

				EuclideanVector<T,N>::mpiDataTypeReg = true;

				return cupcfd::error::E_SUCCESS;
			}

			template <class T, unsigned int N>
			cupcfd::error::eCodes EuclideanVector<T,N>::deregisterMPIType() {
				int mpiErr;

				// Error Check - Only Deregister if currently registered
				if(!this->isRegistered()) {
					return cupcfd::error::E_MPI_DATATYPE_UNREGISTERED;
				}

				mpiErr = MPI_Type_free(&(EuclideanVector<T,N>::mpiType));
				if(mpiErr != MPI_SUCCESS) {
					return cupcfd::error::E_MPI_ERR;
				}

				EuclideanVector<T,N>::mpiDataTypeReg = false;
				return cupcfd::error::E_SUCCESS;
			}

			// Explicit Instantiation
			// ToDo: This is kind of brittle for template usage
			// We could probably move everything to the header and avoid explicit instantiation entirely....
			template class EuclideanVector<float, 2>;
			template EuclideanVector<float, 2>::EuclideanVector(float x, float y);

			template class EuclideanVector<float, 3>;
			template EuclideanVector<float, 3>::EuclideanVector(float x, float y, float z);

			template class EuclideanVector<double, 2>;
			template EuclideanVector<double, 2>::EuclideanVector(double x, double y);

			template class EuclideanVector<double, 3>;
			template EuclideanVector<double, 3>::EuclideanVector(double x, double y, double z);
		}
	}
}
