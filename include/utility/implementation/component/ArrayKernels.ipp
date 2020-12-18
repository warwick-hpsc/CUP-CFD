/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the declarations for various kernel operations that manipulate 
 * or evaluate the contents of arrays.
 */

#ifndef CUPCFD_UTILITY_ARRAY_KERNELS_TPP_H
#define CUPCFD_UTILITY_ARRAY_KERNELS_TPP_H

// C/C++ Lib Headers
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <random>

// CupCfd Headers
#include "SearchKernels.h"

namespace cupcfd
{
	namespace utility
	{
		namespace kernels
		{
			// template <class I, class T>
			// void add(T * source1, T * source2, T * dest, I nEle)
			// {
			// 	for(I i = 0; i < nEle; i++)
			// 	{
			// 		dest[i] = source1[i] + source2[i];
			// 	}
			// }

			// Assume array is sorted and size 1 or greater
			template <class I, class T>
			I uniqueCount(T * source, I nEle) {
				I count = 0; // Overall count of unique elements
				I curCount; // Count how many instances of current element

				// Set current counter to 1, since we are skipping over first element
				curCount = 1;

				for(I i = 1; i < nEle; i++) {
					if(source[i-1] == source[i]) {
						curCount++;
					}
					else {
						// We have changed elements, check if previous element was unique
						if(curCount == 1) {
							count++;
						}

						// Reset counter for new element
						curCount = 1;
					}
				}

				// Check for final element in array
				if(curCount == 1) {
					// The last element was unique since curCount was reset to 1, so increase the overall count
					count++;
				}

				return count;
			}

			// Will return unique elements in a sorted order
			template <class I, class T>
			void uniqueArray(T * source, T * dest, I nEle) {
				I curCount;				// Count how many instances of current element
				I ptr;					// Track current target location in dest array

				curCount = 1;				// Set current counter to 1, since we are skipping over first element
				ptr = 0;					// Set start point for next location in dest to copy to

				for(I i = 1; i < nEle; i++) {
					if(source[i-1] == source[i]) {
						curCount++;	// Increase count of current element
					}
					else {
						// We have changed elements, check if previous element was only encountered once
						if(curCount == 1) {
							dest[ptr] = source[i-1];	// Copy previous element across as unique
							ptr++;				// Advance target copy location
						}

						curCount = 1;				// Reset counter for new element
					}
				}

				// Check for final element in array
				if(curCount == 1) {
					dest[ptr] = source[nEle-1];	// The last element was unique since curCount was reset to 1
				}
			}

			template <class I, class T>
			I distinctCount(T * source, I nEle) {
				// Stores current partial count of how many different values we have encountered
				I count;

				// Could use array::drivers::count, but since that is O(n) this would effectively lead to an O(n^2)
				// if done for every element.

				// Instead, we take advantage of the sorted property to know that when an element changes between
				// indexes, we are encountering a brand new element.

				// Assuming a minimum size of at least 1
				count = 1;

				for(I i=1; i < nEle; i++) {
					if(source[i] != source[i-1]) {
						// Value has changed between indexes
						count++;
					}
				}

				// Finished traversing the array - return the final count
				return count;
			}

			template <class I, class T>
			void distinctArray(T * source, T * dst, I nEle) {
				// Return if size is 0 or less
				if(nEle <= 0) {
					return;
				}

				// Assuming a minimum size of 1
				I ptr = 0;
				dst[0] = source[0];

				for(I i = 1; i < nEle; i++) {
					if(source[i] != source[i-1]) {
						ptr++;
						dst[ptr] = source[i];
					}
				}
			}

			template <class I, class T>
			void distinctArray(T * source, T * dst, I * dupCount, I nEle) {
				// Assuming a minimum size of 1
				I ptr = 0;
				I curCount = 1;
				dst[0] = source[0];

				for(I i = 1; i < nEle; i++) {
					if(source[i] != source[i-1]) {
						// Store count of previous element, and reset counter
						dupCount[ptr] = curCount;
						curCount = 1;

						// Update index pointer to next location
						ptr++;
						// Store copy of newly encountered element
						dst[ptr] = source[i];
					}
					else {
						// Duplicate of previous element, increase counter
						curCount++;
					}
				}

				// Last enocuntered element will be in the dst array, but the count will not be stored
				// (if the last index is unique, then the count for the previous element was stored. If it was
				// the same then the counter was just incremented.)
				// Ensure it is stored here (count should still be preserved from last loop)
				dupCount[ptr] = curCount;
			}

			// ToDo: This could likely be more efficient (.e.g use a hash to test for existance?)
			// This Kernel expects sorted arrays
			template <class I, class T>
			I minusCount(T * source1, I nSource1, T * source2, I nSource2) {
				I count = 0;
				cupcfd::error::eCodes err;

				for(I i = 0; i < nSource1; i++) {
					// Will use the kernel to skip the is_sorted error check in the driver since we already expect
					// it to be sorted.
					I index;
					err = cupcfd::utility::kernels::binarySearch(source2, nSource2, source1[i], &index);
					if (err == cupcfd::error::E_SEARCH_NOT_FOUND) {
						count++;
					}
				}

				return count;
			}

			// template <class I, class T>
			// void minusArray(T * source1, I nSource1, T * source2, I nSource2, T * result, I nResult)
			// {
			// 	I ptr = 0;

			// 	for(I i = 0; i < nSource1; i++)
			// 	{
			// 		// Will use the kernel to skip the is_sorted error check in the driver since we already expect
			// 		// it to be sorted.
			// 		I index = cupcfd::utility::kernels::binarySearch(source2, nSource2, source1[i]);
			// 		if(index < 0)
			// 		{
			// 			result[ptr] = source1[i];
			// 			ptr++;
			// 		}
			// 	}
			// }

			template <class I, class T>
			I intersectCount(T * source1, I nSource1, T * source2, I nSource2) {
				// Loop over whichever array has more elements - this way we cannot miss elements from finishing early
				// and elements not in array1 cannot possibly be in the intersect array, even if in array2
				cupcfd::error::eCodes err;

				T * sourcePtr;
				T * searchPtr;
				I sourceCap;
				I searchCap;

				I count = 0;

				if(nSource1 >= nSource2) {
					sourcePtr = source1;
					sourceCap = nSource1;
					searchPtr = source2;
					searchCap = nSource2;
				}
				else {
					sourcePtr = source2;
					sourceCap = nSource2;
					searchPtr = source1;
					searchCap = nSource1;
				}

				// This kernel assumes that the arrays are sorted and contain only distinct elements
				for(I i = 0; i < sourceCap; i++) {
					// We already expect the kernel to be sorted so we will skip the driver checks and
					// directly use the binary search kernel
					I index;
					err = cupcfd::utility::kernels::binarySearch(searchPtr, searchCap, sourcePtr[i], &index);
					if (err == cupcfd::error::E_SUCCESS) {
						// Element from source was found in the search array, so is present in both.
						// Increment counter
						count++;
					}
				}

				return count;
			}

			template <class I, class T>
			cupcfd::error::eCodes intersectArray(T * source1, I nSource1, T * source2, I nSource2, T * result, I nResult) {
				// Loop over whichever array has more elements - this way we cannot miss elements from finishing early
				// and elements not in array1 cannot possibly be in the intersect array, even if in array2
				cupcfd::error::eCodes err;

				T * sourcePtr;
				T * searchPtr;
				I sourceCap;
				I searchCap;

				I ptr = 0;

				if(nSource1 >= nSource2) {
					sourcePtr = source1;
					sourceCap = nSource1;
					searchPtr = source2;
					searchCap = nSource2;
				}
				else {
					sourcePtr = source2;
					sourceCap = nSource2;
					searchPtr = source1;
					searchCap = nSource1;
				}

				// This kernel assumes that the arrays are sorted
				for(I i = 0; i < sourceCap; i++) {
					// We already expect the kernel to be sorted so we will skip the driver checks and
					// directly use the binary search kernel
					I index;
					err = cupcfd::utility::kernels::binarySearch(searchPtr, searchCap, sourcePtr[i], &index);
					if (err == cupcfd::error::E_SUCCESS) {
						// Element from source was found in the search array, so is present in both.
						// Add to results
						if (ptr >= nResult) {
							DEBUGGABLE_ERROR; return cupcfd::error::E_INVALID_INDEX;
						}
						result[ptr] = sourcePtr[i];
						ptr++;
					}
				}

				return cupcfd::error::E_SUCCESS;
			}


			template <class I, class T>
			void randomUniform(T * dest, I nDest, T lower_bound, T upper_bound) {
				std::uniform_real_distribution<double> unif(lower_bound, upper_bound);
				std::random_device seedSource;
				std::default_random_engine eng(seedSource());

				for(I i = 0; i < nDest; i++) {
					dest[i] = unif(eng);
				}
			}

		} // namespace kernels
	} // array
} // cupcfd

#endif
