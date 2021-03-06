/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the implementation for the kernels of the utility sort operations
 */

#ifndef CUPCFD_UTILITY_SORT_KERNELS_IPP_H
#define CUPCFD_UTILITY_SORT_KERNELS_IPP_H

#include <cstdlib>
#include <cstring>
#include "ArrayDrivers.h"

namespace cupcfd
{
	namespace utility
	{
		namespace kernels
		{
			template <class I, class T>
			int merge_sort(T * source, I nele) {
				T * left;
				T * right;
				I i, split, n_l, n_r, left_idx, right_idx;
				int returnValLeft, returnValRight;

				// Size < 0 : Error case
				if(nele < 0) {
					return -1;
				}

				// Size 0 or 1 : No splitting to be done
				if(nele <= 1) {
					return 0;
				}

				split = (nele + 1)/2;
				n_l = split;
				n_r = (nele - n_l);

				left = (T *) malloc(sizeof(T) * n_l);
				memcpy(left, source, n_l * sizeof(T));
				returnValLeft = merge_sort(left, n_l);

				right = (T *) malloc(sizeof(T) * n_r);
				memcpy(right, source + n_l, n_r * sizeof(T));
				returnValRight = merge_sort(right, n_r);

				left_idx = 0;
				right_idx = 0;

				for(i = 0; i < nele; i++) {
					// Left array is empty, copy from next value in right array
					if(left_idx >= n_l) {
						source[i] = right[right_idx];
						right_idx++;
						continue;
					}

					// Right array is empty, copy from next value in left array
					if(right_idx >= n_r) {
						source[i] = left[left_idx];
						left_idx++;
						continue;
					}

					// Copy from either left or right depending on which head of array
					// is smaller
					if(left[left_idx] <= right[right_idx]) {
						source[i] = left[left_idx];
						left_idx++;
					}
					else {
						source[i] = right[right_idx];
						right_idx++;
					}
				}

				free(left);
				free(right);

				if(returnValLeft < 0) {
					return returnValLeft;
				}

				if(returnValRight < 0) {
					return returnValRight;
				}

				return 0;
			}

			template <class I, class T>
			int merge_sort(T * source,  I nSource, I * indexes, I nIndexes) {
				if (nIndexes != nSource) {
					return -1;
				}

				T * left;
				T * right;
				I * leftIndexes;
				I * rightIndexes;

				I i, split, n_l, n_r, left_idx, right_idx;
				int returnValLeft, returnValRight;

				// Size < 0 : Error case
				if(nSource < 0) {
					return -1;
				}

				// Size 0 or 1 : No splitting to be done
				if(nSource <= 1) {
					return 0;
				}

				split = (nSource + 1)/2;
				n_l = split;
				n_r = (nSource - n_l);

				// Copy Left
				left = (T *) malloc(sizeof(T) * n_l);
				memcpy(left, source, n_l * sizeof(T));

				// Copy Left Indexes
				leftIndexes = (I *) malloc(sizeof(I) * n_l);
				memcpy(leftIndexes, indexes, n_l * sizeof(I));

				returnValLeft = merge_sort(left, n_l, leftIndexes, n_l);

				// Copy Right
				right = (T *) malloc(sizeof(T) * n_r);
				memcpy(right, source + n_l, n_r * sizeof(T));

				// Copy Right Indexes
				rightIndexes = (I *) malloc(sizeof(I) * n_r);
				memcpy(rightIndexes, indexes + n_l, n_r * sizeof(I));

				returnValRight = merge_sort(right, n_r, rightIndexes, n_r);

				left_idx = 0;
				right_idx = 0;

				for(i = 0; i < nSource; i++) {
					// Left array is empty, copy from next value in right array
					if(left_idx >= n_l) {
						source[i] = right[right_idx];
						indexes[i] = rightIndexes[right_idx];
						right_idx = right_idx + 1;
						continue;
					}

					// Right array is empty, copy from next value in left array
					if(right_idx >= n_r) {
						source[i] = left[left_idx];
						indexes[i] = leftIndexes[left_idx];
						left_idx = left_idx + 1;
						continue;
					}

					// Copy from either left or right depending on which head of array
					// is smaller
					if(left[left_idx] <= right[right_idx]) {
						source[i] = left[left_idx];
						indexes[i] = leftIndexes[left_idx];
						left_idx = left_idx + 1;
					}
					else {
						source[i] = right[right_idx];
						indexes[i] = rightIndexes[right_idx];
						right_idx = right_idx + 1;
					}
				}

				free(left);
				free(right);
				free(leftIndexes);
				free(rightIndexes);

				if(returnValLeft < 0) {
					return returnValLeft;
				}

				if(returnValRight < 0) {
					return returnValRight;
				}

				return 0;
			}

			template <class I, class T>
			bool is_sorted(T * source, I nEle) {
				for(I i = 1; i < nEle; i++) {
					if(source[i-1] > source[i]) {
						return false;
					}
				}

				return true;
			}

			template <class I, class T>
			cupcfd::error::eCodes sourceIndexReorder(T * source, I nEleSource, I * indexes, I nEleIndexes) {
				// Considered approach of swapping value into new location, and temporarily storing
				// unorder value in swapped location - however this breaks later reordering.
				// Instead, make a temporary array we can copy from..

				if (nEleSource != nEleIndexes) {
					return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}

				T* tmpCpy = cupcfd::utility::drivers::duplicate(source, nEleSource);

				for(I i=0; i < nEleIndexes; i++) {
					source[i] = tmpCpy[indexes[i]];
				}
				free(tmpCpy);

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes destIndexReorder(T * source, I nEleSource, I * indexes, I nEleIndexes) {
				cupcfd::error::eCodes status;
				
				if (nEleSource != nEleIndexes) {
					return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}
				
				T * tmpCpy = (T *) malloc(sizeof(T) * nEleSource);
				for(I i=0; i < nEleIndexes; i++) {
					tmpCpy[indexes[i]] = source[i];
				}

				status = cupcfd::utility::drivers::copy(tmpCpy, nEleSource, source, nEleSource);
				CHECK_ECODE(status)

				free(tmpCpy);

				return cupcfd::error::E_SUCCESS;
			}

		} // namespace kernels
	}
}

#endif
