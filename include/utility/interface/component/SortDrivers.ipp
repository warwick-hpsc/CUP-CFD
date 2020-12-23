/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * This file contains the implementation for the drivers of the utility sort operations
 */

#ifndef CUPCFD_UTILITY_SORT_DRIVERS_IPP_H
#define CUPCFD_UTILITY_SORT_DRIVERS_IPP_H

#include "SortKernels.h"
#include "ArrayDrivers.h"

namespace cupcfd
{
	namespace utility
	{
		namespace drivers
		{
			template <class I, class T>
			cupcfd::error::eCodes merge_sort(T * source, I nEle) {
				if(nEle < 0) {
					return cupcfd::error::E_ARRAY_SIZE_UNDERSIZED;
				}

				int err = kernels::merge_sort(source, nEle);
				if (err == -1) {
					return cupcfd::error::E_ERROR;
				}
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes merge_sort(T * source,  T * dest, I nEle) {
				if(nEle < 0) {
					return cupcfd::error::E_ARRAY_SIZE_UNDERSIZED;
				}
				cupcfd::error::eCodes status;

				// Copy the source array to the destination array
				status = cupcfd::utility::drivers::copy(source, nEle, dest, nEle);
				CHECK_ECODE(status)

				// Sort the copied array
				kernels::merge_sort(dest, nEle);
				return cupcfd::error::E_SUCCESS;
			}

			// Merge Sort (Destructive) - Also returns original indexes in sorted fashion
			template <class I, class T>
			cupcfd::error::eCodes merge_sort_index(T * source,  I nSource, I * indexes, I nIndexes) {
				if(nSource < 0) {
					return cupcfd::error::E_ARRAY_SIZE_UNDERSIZED;
				}
				if (nSource != nIndexes) {
					return cupcfd::error::E_ARRAY_SIZE_MISMATCH;
				}

				// Populate the index array with indexes
				for(I i = 0; i < nSource; i++) {
					indexes[i] = i;
				}

				int err = kernels::merge_sort(source, nSource, indexes, nIndexes);
				if (err == -1) {
					return cupcfd::error::E_ERROR;
				}
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes index_sort(T * source, I nEle, I * indexes, I nidx) {
				if(nEle < 0) {
					return cupcfd::error::E_ARRAY_SIZE_UNDERSIZED;
				}
				cupcfd::error::eCodes status;

				status = index_sort(source, nEle, indexes, nidx);
				CHECK_ECODE(status)
				return cupcfd::error::E_SUCCESS;
			}

			// Detected whether an array is sorted
			template <class I, class T>
			cupcfd::error::eCodes is_sorted(T * source, I nEle, bool * result) {
				// Size Error Check
				if(nEle < 0) {
					return cupcfd::error::E_ARRAY_SIZE_UNDERSIZED;
				}

				// ToDo: Could use inbuilt C++ is_sorted method instead
				*result = kernels::is_sorted(source, nEle);

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes sourceIndexReorder(T * source, I nEleSource, I * indexes, I nEleIndexes) {
				// ToDo: Error checks before and after

				cupcfd::error::eCodes status;
				status = kernels::sourceIndexReorder(source, nEleSource, indexes, nEleIndexes);
				CHECK_ECODE(status)
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes destIndexReorder(T * source, I nEleSource, I * indexes, I nEleIndexes) {
				// ToDo: Error checks before and after

				cupcfd::error::eCodes status;
				status = kernels::destIndexReorder(source, nEleSource, indexes, nEleIndexes);
				void sourceIndexReorder(T * source, I nEleSource, I * indexes, I nEleIndexes);
				return cupcfd::error::E_SUCCESS;
			}

		} // namespace drivers
	} // namespace utility
} // namespace cupcfd

#endif
