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
			cupcfd::error::eCodes linearSearch(T * source, I nEle, T target, bool * found)
 			{
				I index;
				cupcfd::error::eCodes status;
 				*found = false;

				status = drivers::linearSearch(source, nEle, target, &index);

				if(status == cupcfd::error::E_SEARCH_SUCCESS)
				{
					// A valid index was found
					*found = true;
				}

				return status;
 			}

		 	template <class I, class T>
		 	cupcfd::error::eCodes linearSearch(T * source, I nEle, T target, I * index)
 			{
 				*index = cupcfd::utility::kernels::linearSearch(source, nEle, target);

				if(*index == -1)
				{
					return cupcfd::error::E_SEARCH_NOT_FOUND;
				}
				else
 				{
					return cupcfd::error::E_SEARCH_SUCCESS;
 				}
 			}

 			template <class I, class T>
 			cupcfd::error::eCodes binarySearch(T * source, I nEle, T target, bool * found)
 			{
 				I index;
 				cupcfd::error::eCodes status;

 				*found = false;
 				status = drivers::binarySearch(source, nEle, target, &index);

 				if(status == cupcfd::error::E_SEARCH_SUCCESS)
 				{
 					// A valid index was found, so it does exist in the array.
 					*found = true;
 				}

 				// Pass up the error code from the index search.
 				return status;
 			}

 			template <class I, class T>
 			cupcfd::error::eCodes binarySearch(T * source, I nEle, T target, I * index)
 			{
 				I isSortedStatus;
 			 	bool sortCheck;

 				// ================================================================
 				// (1) Array must be sorted to use binarySearch kernel.
 				//     Check status, and return error without searching if unsorted
 				// ================================================================

 			 	isSortedStatus = cupcfd::utility::drivers::is_sorted(source, nEle, &sortCheck);

 				// Error check on is_sorted
 				if(isSortedStatus == -1)
 				{
 					// Is not sorted error
 					return cupcfd::error::E_SEARCH_SORT_CHECK_FAILURE;
 				}

 				// Check whether array was sorted
 				if(sortCheck == false)
 				{
 					return cupcfd::error::E_SEARCH_UNSORTED;
 				}

 				// ================================================================
 				// (2) Array is sorted. Run binarySearch kernel.
 				// ================================================================
 				*index = cupcfd::utility::kernels::binarySearch(source, nEle, target);

 				 // ===============================================================
 				// (3) Check whether value was found in binarySearch kernel.
 				// ================================================================

 				if(*index == -1)
 				{
					return cupcfd::error::E_SEARCH_NOT_FOUND;
 				}
 				else
 				{
					return cupcfd::error::E_SEARCH_SUCCESS;
 				}
 			}

		} // namespace drivers
	} // namespace utility
} // namespace cupcfd

#endif
