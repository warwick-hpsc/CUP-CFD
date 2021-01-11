/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Description
 *
 * Contains the definitions of the templated drivers functions
 * for utility search operations at a header level.
 */

#ifndef CUPCFD_UTILITY_SEARCH_DRIVERS_TPP_H
#define CUPCFD_UTILITY_SEARCH_DRIVERS_TPP_H

#include <cmath>
#include "SearchKernels.h"
#include "SortDrivers.h"

namespace cupcfd
{
	namespace utility
	{
		namespace drivers
		{
		 	template <class I, class T>
		 	cupcfd::error::eCodes linearSearch(T * source, I nEle, T target, I * index) {
 				return cupcfd::utility::kernels::linearSearch(source, nEle, target, index);
 			}

 			template <class I, class T>
 			cupcfd::error::eCodes binarySearch(T * source, I nEle, T target, I * index) {
 			 	bool isSorted;

 				// ================================================================
 				// (1) Array must be sorted to use binarySearch kernel.
 				//     Check status, and return error without searching if unsorted
 				// ================================================================

 			 	// status = cupcfd::utility::drivers::is_sorted(source, nEle, &isSorted);
 			 	isSorted = cupcfd::utility::drivers::is_sorted(source, nEle);
 				if(!isSorted) {
 					return cupcfd::error::E_SEARCH_UNSORTED;
 				}

 				// ================================================================
 				// (2) Array is sorted. Run binarySearch kernel.
 				// ================================================================
 				// status = cupcfd::utility::kernels::binarySearch(source, nEle, target, index);
 				cupcfd::utility::kernels::binarySearch(source, nEle, target, index);

 				 // ===============================================================
 				// (3) Check whether value was found in binarySearch kernel.
 				// ================================================================

 				if(*index == -1) {
					return cupcfd::error::E_SEARCH_NOT_FOUND;
 				} else {
					return cupcfd::error::E_SEARCH_SUCCESS;
 				}
 			}

		} // namespace drivers
	} // namespace utility
} // namespace cupcfd

#endif
