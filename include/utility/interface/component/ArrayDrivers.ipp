/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the definitions of routines
 * for utility operations that evaluate or manipulate arrays
 * at a header level
 */

#ifndef CUPCFD_UTILITY_ARRAY_DRIVERS_TPP_H
#define CUPCFD_UTILITY_ARRAY_DRIVERS_TPP_H

#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <typeinfo>

#include "Error.h"
#include "ArrayKernels.h"
#include "SortDrivers.h"

namespace cupcfd
{
	namespace utility
	{
		namespace drivers
		{
			// Currently a wrapper for memcpy. Exists primarily so that copy calls
			// could be switched out for another approach at a later time.
			template <class I, class T>
			cupcfd::error::eCodes copy(T * src, I nSrc, T * dst, I nDst) {
				// Error Check: Check dst is large enough to hold src.
				if(nDst < nSrc) {
					return cupcfd::error::E_ARRAY_SIZE_UNDERSIZED;
				}

				int byteSize = sizeof(T);
				memcpy((void*)dst, (void*)src, byteSize * nSrc);
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			T* duplicate(T * src, I nSrc) {
				if (nSrc == I(0)) {
					return NULL;
				}

				int byteSize = sizeof(T);
				T* dst = (T*)malloc(byteSize * nSrc);
				memcpy((void*)dst, (void*)src, byteSize * nSrc);
				return dst;
			}

			template <class I, class T>
			cupcfd::error::eCodes zero(T * source, I nEle) {
				// Typecast 0 to appropriate datatype
				memset(source, (T) 0, nEle * sizeof(T));

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes uniqueCount(T * source, I nSourceEle, I * count) {
				cupcfd::error::eCodes status;
				bool isSorted;
				isSorted = cupcfd::utility::drivers::is_sorted(source, nSourceEle);

				// Error Check on is_sorted

				if(!isSorted) {
					// Non-Destructive Sort
					T * scratch = (T *) malloc(sizeof(T) * nSourceEle);
					status = cupcfd::utility::drivers::merge_sort(source, scratch, nSourceEle);
					CHECK_ECODE(status)
					*count = cupcfd::utility::kernels::uniqueCount(scratch, nSourceEle);
					free(scratch);
				}
				else {
					*count = cupcfd::utility::kernels::uniqueCount(source, nSourceEle);
				}
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes uniqueArray(T * source, I nSourceEle, T ** dest, I * nDestEle) {
				cupcfd::error::eCodes status;

				// Determine number of unique elements
				status = drivers::uniqueCount(source, nSourceEle, nDestEle);
				CHECK_ECODE(status)

				// Allocate the results array
				*dest = (T *) malloc(sizeof(T) * *nDestEle);

				// Pass work along to driver that performs the same function as this, but with the results array set up.
				status = uniqueArray(source, nSourceEle, *dest, *nDestEle);
				return status;
			}


			template <class I, class T>
			cupcfd::error::eCodes uniqueArray(T * source, I nSourceEle, T * dest, I nDestEle) {
				if (nSourceEle != nDestEle) {
					return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
				}
				cupcfd::error::eCodes status;

				bool isSorted;
				isSorted = cupcfd::utility::drivers::is_sorted(source, nSourceEle);
				// Error Check on is_sorted

				if(!isSorted) {
					// Non-Destructive Sort
					T * scratch = (T *) malloc(sizeof(T) * nSourceEle);
					status = cupcfd::utility::drivers::merge_sort(source, scratch, nSourceEle);
					CHECK_ECODE(status)
					kernels::uniqueArray(scratch, dest, nSourceEle);
					free(scratch);
					return cupcfd::error::E_SUCCESS;;
				}
				else {
					kernels::uniqueArray(source, dest, nSourceEle);
					return cupcfd::error::E_SUCCESS;
				}
			}


			template <class I, class T>
			cupcfd::error::eCodes add(T * source1, I nSource1Ele, T * source2, I nSource2Ele) {
				if (nSource1Ele != nSource2Ele) {
					return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
				}

				kernels::add(source1, source2, source1, nSource1Ele);
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes add(T * source1, I nSource1Ele, T * source2, I nSource2Ele, T * dest, I nDestEle) {
				if (nSource1Ele != nSource2Ele) {
					return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
				}
				if (nSource1Ele != nDestEle) {
					return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
				}

				kernels::add(source1, source2, dest, nSource1Ele);
				return cupcfd::error::E_SUCCESS;
			}

            template <class I, class T>
            cupcfd::error::eCodes distinctCount(T * source, I nEle, I * count) {
            	cupcfd::error::eCodes status;

				bool sorted;
				T * arrPtr;

				// ToDo: Negative nEle Error Check

				// If nEle is zero, count will be zero
				if(nEle == 0) {
					*count = 0;
					return cupcfd::error::E_SUCCESS;
				}

				// (1) Ensure array is sorted
				sorted = cupcfd::utility::drivers::is_sorted(source, nEle);

				// (2) If not sorted, do a non-destructive sort to get a sorted copy.
				if(!sorted) {
					arrPtr = (T *) malloc(sizeof(T) * nEle);
					status = cupcfd::utility::drivers::merge_sort(source, arrPtr, nEle);
					CHECK_ECODE(status)
				}
				else {
					arrPtr = source;
				}

				// (3) Call the kernel on a sorted array.
				*count = kernels::distinctCount(arrPtr, nEle);

				// Cleanup
				if(arrPtr != source) {
					free(arrPtr);
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes distinctArray(T * source, I nEleSource, T * dst, I nEleDst) {
				cupcfd::error::eCodes status;
				bool sorted;
				T * arrPtr;

				if(nEleSource <= 0) {
					// ToDo: This should probably be an warning error code
					// It's not incorrect to return if there's nothing to do, but
					// that status seems unusual...
					return cupcfd::error::E_SUCCESS;
				}

				// (1) Ensure array is sorted
				sorted = cupcfd::utility::drivers::is_sorted(source, nEleSource);

				// (2) If not sorted, do a non-destructive sort to get a sorted copy.
				if(!sorted) {
					arrPtr = (T *) malloc(sizeof(T) * nEleSource);
					status = cupcfd::utility::drivers::merge_sort(source, arrPtr, nEleSource);
					CHECK_ECODE(status)
				}
				else {
					arrPtr = source;
				}

				// Error Check: Ensure dst is correct size
				I nEleDst2;
				status = drivers::distinctCount(source, nEleSource, &nEleDst2);
				CHECK_ECODE(status)
				if (nEleDst2 != nEleDst) {
					return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
				}

				// (3) Call the kernel on a sorted array.
				kernels::distinctArray(arrPtr, dst, nEleSource);

				// Result should now be in dst - cleanup
				if(arrPtr != source) {
					free(arrPtr);
				}

				return cupcfd::error::E_SUCCESS;
			}

            template <class I, class T>
            cupcfd::error::eCodes distinctArray(T * source, I nEleSource, T ** dst, I * nEleDst) {
            	cupcfd::error::eCodes status;

				// Determine number of distinct elements
				status = drivers::distinctCount(source, nEleSource, nEleDst);
				CHECK_ECODE(status)

				// Allocate the results array
				*dst = (T *) malloc(sizeof(T) * *nEleDst);

				// Pass work along to driver that performs the same functionas this, but with the results array set up.
				status = drivers::distinctArray(source, nEleSource, *dst, *nEleDst);
				CHECK_ECODE(status)

				return status;
			}

			template <class I, class T>
			cupcfd::error::eCodes distinctArray(T * source, I nEleSource, T * dst, I nEleDst, I * dupCount, I nEleDupCount) {
				cupcfd::error::eCodes status;

				bool sorted;
				T * sourceSorted;

				// (1) Ensure array is sorted
				sorted = cupcfd::utility::drivers::is_sorted(source, nEleSource);

				// (2) If not sorted, do a non-destructive sort to get a sorted copy.
				if(!sorted) {
					sourceSorted = (T *) malloc(sizeof(T) * nEleSource);
					status = cupcfd::utility::drivers::merge_sort(source, sourceSorted, nEleSource);
					CHECK_ECODE(status)
				}
				else {
					sourceSorted = source;
				}

				// Error Check: Ensure dst is correct size
				I nEleDst2;
				status = drivers::distinctCount(source, nEleSource, &nEleDst2);
				CHECK_ECODE(status)
				if (nEleDst2 != nEleDst) {
					return cupcfd::error::E_ARRAY_INCORRECT_SIZE;
				}
				if (nEleDupCount != nEleDst) {
					return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
				}

				// (3) Call the kernel on a sorted array.
				kernels::distinctArray(sourceSorted, dst, dupCount, nEleSource);

				// Result should now be in dst - cleanup
				if(sourceSorted != source) {
					free(sourceSorted);
				}

				return cupcfd::error::E_SUCCESS;
			}

            template <class I, class T>
            cupcfd::error::eCodes distinctArray(T * source, I nEleSource, T ** dst, I * nEleDst, I ** dupCount, I * nEleDupCount) {
            	cupcfd::error::eCodes status;

				// Determine number of distinct elements
				status =drivers::distinctCount(source, nEleSource, nEleDst);
				CHECK_ECODE(status)

				// Allocate the results array
				*dst = (T *) malloc(sizeof(T) * *nEleDst);

				// Allocate the count array
				*nEleDupCount = *nEleDst;
				*dupCount = (I *) malloc(sizeof(I) * *nEleDupCount);

				// Pass work along to driver that performs the same functionas this, but with the results array set up.
				status = drivers::distinctArray(source, nEleSource, *dst, *nEleDst, *dupCount, *nEleDupCount);
				CHECK_ECODE(status)

				// Responsibility for freeing the dst and dupCount arrays is left to the caller, since they contain the results.
				return cupcfd::error::E_SUCCESS;
			}

            template <class I, class T>
            // cupcfd::error::eCodes minusCount(T * source1, I nSource1, T * source2, I nSource2, I * count) {
			void minusCount(T * source1, I nSource1, T * source2, I nSource2, I * count) {
				// If not sorted, make a copy and use that
				bool source1Sorted;
				bool source2Sorted;
				*count = 0;
				cupcfd::error::eCodes status;

				source1Sorted = cupcfd::utility::drivers::is_sorted(source1, nSource1);
				source2Sorted = cupcfd::utility::drivers::is_sorted(source2, nSource2);

				T * source1Ptr;
				T * source2Ptr;

				if(!source1Sorted) {
					source1Ptr = cupcfd::utility::drivers::duplicate(source1, nSource1);
					status = cupcfd::utility::drivers::merge_sort(source1Ptr, nSource1);
					CHECK_ECODE(status)
				}
				else {
					source1Ptr = source1;
				}

				if(!source2Sorted) {
					source2Ptr = cupcfd::utility::drivers::duplicate(source2, nSource2);
					status = cupcfd::utility::drivers::merge_sort(source2Ptr, nSource2);
					CHECK_ECODE(status)
				}
				else {
					source2Ptr = source2;
				}

				// Call the diff kernel now we are sure we have two sorted arrays.
				*count = cupcfd::utility::kernels::minusCount(source1Ptr, nSource1, source2Ptr, nSource2);

				// Cleanup if temporary arrays were allocated
				if(source1Ptr != source1) {
					free(source1Ptr);
				}

				if(source2Ptr != source2) {
					free(source2Ptr);
				}

				// return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes minusDistinctCount(T * source1, I nSource1, T * source2, I nSource2, I * count) {
				cupcfd::error::eCodes status;

				T * source1Distinct;
				I nSource1Distinct;

				T * source2Distinct;
				I nSource2Distinct;

				*count = 0;

				// Get an array of distinct values for source1.
				// The driver should be non-destructive and return it sorted, so we can reuse it later.
				status = distinctArray(source1,nSource1, &source1Distinct, &nSource1Distinct);
				CHECK_ECODE(status)
				status = distinctArray(source2,nSource2, &source2Distinct, &nSource2Distinct);
				CHECK_ECODE(status)

				// Call the diff kernel now we are sure we have two sorted arrays.
				*count = cupcfd::utility::kernels::minusCount(source1Distinct, nSource1Distinct, source2Distinct, nSource2Distinct);

				free(source1Distinct);
				free(source2Distinct);

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes minusArray(T * source1, I nSource1, T * source2, I nSource2, T ** result, I * nResult) {
				bool sorted1;
				bool sorted2;
				T * source1Ptr = nullptr;
				T * source2Ptr = nullptr;
				cupcfd::error::eCodes status;

				// Error Check: result should be a nullptr (else we risk a memory leak by overwriting an active
				// pointer memory allocation).

				// Kernel requires sorted arrays. Check if source1 is sorted.
				sorted1 = cupcfd::utility::drivers::is_sorted(source1, nSource1);

				if(!sorted1) {
					source1Ptr = (T *) malloc(sizeof(T) * nSource1);
					status = cupcfd::utility::drivers::copy(source1, nSource1, source1Ptr, nSource1);
					CHECK_ECODE(status)
					status = cupcfd::utility::drivers::merge_sort(source1Ptr, nSource1);
					CHECK_ECODE(status)
				} else {
					source1Ptr = source1;
				}

				// Kernel requires sorted arrays. Check if source2 is sorted.
				sorted2 = cupcfd::utility::drivers::is_sorted(source2, nSource2);
				if(!sorted2) {
					source2Ptr = (T *) malloc(sizeof(T) * nSource2);
					status = cupcfd::utility::drivers::copy(source2, nSource2, source2Ptr, nSource2);
					CHECK_ECODE(status)
					status = cupcfd::utility::drivers::merge_sort(source2Ptr, nSource2);
					CHECK_ECODE(status)
				} else {
					source2Ptr = source2;
				}

				// Need to allocate space for the result array, requiring us to know how many elements there
				// are in the distinct array.
				minusCount(source1Ptr, nSource1, source2Ptr, nSource2, nResult);

				*result = (T* ) malloc(sizeof(T) * *nResult);

				// Compute the set minus array.
				status = kernels::minusArray(source1Ptr, nSource1, source2Ptr, nSource2, *result, *nResult);
				CHECK_ECODE(status)

				// Cleanup
				// Result is not freed here, it is used to pass the results back to the caller
				if(source1Ptr != source1) {
					free(source1Ptr);
				}
				if(source2Ptr != source2) {
					free(source2Ptr);
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes intersectCount(T * source1, I nSource1, T * source2, I nSource2, I * count) {
				cupcfd::error::eCodes status;

				// If not sorted, make a copy and use that
				bool source1Sorted;
				bool source2Sorted;
				*count = 0;

				source1Sorted = cupcfd::utility::drivers::is_sorted(source1, nSource1);
				source2Sorted = cupcfd::utility::drivers::is_sorted(source2, nSource2);

				T * source1Ptr;
				I nSource1Ptr;

				T * source2Ptr;
				I nSource2Ptr;

				// Kernels requires sorted and distinct arrays (since it's technically a set operation)
				// distinctArray will not only sure it only has unique elements, it will also return an array
				// that is already sorted.
				status = distinctArray(source1, nSource1, &source1Ptr, &nSource1Ptr);
				CHECK_ECODE(status)
				status = distinctArray(source2, nSource2, &source2Ptr, &nSource2Ptr);
				CHECK_ECODE(status)

				// Call the diff kernel now we are sure we have two sorted arrays.
				*count = cupcfd::utility::kernels::intersectCount(source1Ptr, nSource1, source2Ptr, nSource2);

				// Cleanup temporary arrays
				free(source1Ptr);
				free(source2Ptr);

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes intersectArray(T * source1, I nSource1, T * source2, I nSource2, T ** result, I * nResult) {
				cupcfd::error::eCodes status;

				bool sorted1;
				bool sorted2;
				T * source1Ptr = nullptr;
				T * source2Ptr = nullptr;

				// Error Check: result should be a nullptr (else we risk a memory leak by overwriting an active
				// pointer memory allocation).
				if (*result != nullptr) {
					return cupcfd::error::E_NOT_NULL_PTR;
				}

				// Kernel requires sorted arrays. Check if source1 is sorted.
				sorted1 = cupcfd::utility::drivers::is_sorted(source1, nSource1);

				if(!sorted1) {
					source1Ptr = cupcfd::utility::drivers::duplicate(source1, nSource1);
					status = cupcfd::utility::drivers::merge_sort(source1Ptr, nSource1);
					CHECK_ECODE(status)
				}
				else {
					source1Ptr = source1;
				}

				// Kernel requires sorted arrays. Check if source2 is sorted.
				sorted2 = cupcfd::utility::drivers::is_sorted(source2, nSource2);
				if(!sorted2) {
					source2Ptr = cupcfd::utility::drivers::duplicate(source2, nSource2);
					status = cupcfd::utility::drivers::merge_sort(source2Ptr, nSource2);
					CHECK_ECODE(status)
				}
				else {
					source2Ptr = source2;
				}

				// Need to allocate space for the result array, requiring us to know how many elements there
				// are in the distinct array.
				status = intersectCount(source1Ptr, nSource1, source2Ptr, nSource2, nResult);
				CHECK_ECODE(status)

				*result = (T* ) malloc(sizeof(T) * *nResult);

				// Compute the set minus array.
				status = kernels::intersectArray(source1Ptr, nSource1, source2Ptr, nSource2, *result, *nResult);
				CHECK_ECODE(status)

				// Cleanup
				// Result is not freed here, it is used to pass the results back to the caller
				if(source1Ptr != source1) {
					free(source1Ptr);
				}

				if(source2Ptr != source2) {
					free(source2Ptr);
				}

				return cupcfd::error::E_SUCCESS;
			}

			template <class I>
			I safeConvertSizeT(std::size_t s) {
				#ifdef DEBUG
					// Need to cast size_t to I. First, check that s is representatable in type I
					if (s > (std::size_t)std::numeric_limits<I>::max()) {
						std::string msg("boundaries size ");
						msg += s;
						msg += std::string(" exceeds max value of type ");
						msg += typeid(I).name();
						throw(std::runtime_error(msg));
					}
				#endif
				return (I)s;
			}

		} // namespace drivers
	} // utility
} // cupcfd

#endif
