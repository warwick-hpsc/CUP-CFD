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
			cupcfd::error::eCodes merge_sort(T * source, I nele) {
				// ToDo: Error checks before and after
				kernels::merge_sort(source, nele);
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes merge_sort(T * source,  T * dest, I nele) {
				// ToDo: Error checks before and after

				// Copy the source array to the destination array
				cupcfd::utility::drivers::copy(source, nele, dest, nele);

				// Sort the copied array
				kernels::merge_sort(dest, nele);
				return cupcfd::error::E_SUCCESS;
			}

			// Merge Sort (Destructive) - Also returns original indexes in sorted fashion
			template <class I, class T>
			cupcfd::error::eCodes merge_sort_index(T * source,  I nSource, I * indexes, I nIndexes) {
				// ToDo: Error checks before and after

				// Populate the index array with indexes
				for(I i = 0; i < nSource; i++) {
					indexes[i] = i;
				}

				kernels::merge_sort(source, nSource, indexes, nIndexes);
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes index_sort(T * source, I nele, I * indexes, I nidx) {
				// ToDo: Error checks before and after

				index_sort(source, nele, indexes, nidx);
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

				kernels::sourceIndexReorder(source, nEleSource, indexes, nEleIndexes);
				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes destIndexReorder(T * source, I nEleSource, I * indexes, I nEleIndexes) {
				// ToDo: Error checks before and after

				kernels::destIndexReorder(source, nEleSource, indexes, nEleIndexes);
				return cupcfd::error::E_SUCCESS;
			}

		} // namespace drivers
	} // namespace utility
} // namespace cupcfd

#endif
