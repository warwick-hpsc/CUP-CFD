/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains driver declarations for search operations.
 * These operations could likely be updated to just redirect to C++
 * search functionality.
 *
 */

#ifndef CUPCFD_UTILITY_SEARCH_DRIVERS_INCLUDE_H
#define CUPCFD_UTILITY_SEARCH_DRIVERS_INCLUDE_H

#include "Error.h"

namespace cupcfd
{
	namespace utility
 	{
 		namespace drivers
 		{
 			/**
 			 * Performs a linear search for an element within an array.
 			 * This driver both runs the kernel and provides error checking.
 			 *
 			 * @param source The source array to search through
			 * @param nEle The number of elements to search through the array.
			 * This should be no greater than the number of elements in the array.
 			 * @param target The element to search for
 			 * @param index The pointer target that will be updated  by the
 			 * method to the index where the first instance of the element is found
 			 * in the array, if found. If not found, index is undefined.
 			 *
		 	 * @tparam I The type of the indexing scheme
			 * @tparam T The type of the stored array data
 			 *
			 * @return An error status indicating the success or failure of the operation
 			 * @retval cupcfd::error::E_SEARCH_SUCCESS Value was found successfully.
 			 * @retval cupcfd::error::E_SEARCH_NOT_FOUND Value was not found.
 			 */
 			template <class I, class T>
 			__attribute__((warn_unused_result))
 			cupcfd::error::eCodes linearSearch(T * source, I nEle, T target, I * index);

 			/**
 			 * Performs a linear search for an element within an array.
 			 * This driver both runs the kernel and provides error checking.
 			 *
 			 * @param source The source array to search through
			 * @param nEle The number of elements to search through the array.
			 * 			   This should be no greater than the number of elements
			 * 			   in the array.
 			 * @param target The element to search for
 			 * @param found True if element is found, else fails
 			 *
		 	 * @tparam I The type of the indexing scheme
			 * @tparam T The type of the stored array data
 			 *
			 * @return An error status indicating the success or failure of the operation
 			 * @retval cupcfd::error::E_SEARCH_SUCCESS Value was found successfully.
 			 * @retval cupcfd::error::E_SEARCH_NOT_FOUND Value was not found.
 			 */
  			template <class I, class T>
 			__attribute__((warn_unused_result))
  			cupcfd::error::eCodes linearSearch(T * source, I nEle, T target, bool * found);

 			/**
 			 * Performs a binary search for an element within an array.
 			 * This driver both runs the kernel and provides error checking.
 			 *
 			 * @param source The source array to search through
			 * @param nEle The number of elements to search through the array.
			 * 			   This should be no greater than the number of elements
			 * 			   in the array.
 			 * @param target The element to search for
 			 * @param index The pointer target that will be updated  by the
 			 * method to the index where the first instance of the element is found
 			 * in the array, if found. If not found, index is undefined.
 			 *
		 	 * @tparam I The type of the indexing scheme
			 * @tparam T The type of the stored array data
 			 *
			 * @return An error status indicating the success or failure of the operation
 			 * @retval cupcfd::error::E_SEARCH_SUCCESS Value was found successfully.
 			 * @retval cupcfd::error::E_SEARCH_UNSORTED 'Source' is not sorted
 			 * @retval cupcfd::error::E_SEARCH_SORT_CHECK_FAILURE Error occurred checking if source is sorted
 			 * @retval cupcfd::error::E_SEARCH_NOT_FOUND Value was not found.
 			 */
  			template <class I, class T>
 			__attribute__((warn_unused_result))
  			cupcfd::error::eCodes binarySearch(T * source, I nEle, T target, I * index);

 			/**
 			 * Performs a binary search for an element within an array.
 			 * This driver both runs the kernel and provides error checking.
 			 *
 			 * @param source The source array to search through
			 * @param nEle The number of elements to search through the array.
			 * 			   This should be no greater than the number of elements
			 * 			   in the array.
 			 * @param target The element to search for
 			 * @param found True if element is found, else fails
 			 *
		 	 * @tparam I The type of the indexing scheme
			 * @tparam T The type of the stored array data
 			 *
			 * @return An error status indicating the success or failure of the operation
 			 * @retval cupcfd::error::E_SEARCH_SUCCESS Value was found successfully.
 			 * @retval cupcfd::error::E_SEARCH_UNSORTED 'Source' is not sorted
 			 * @retval cupcfd::error::E_SEARCH_SORT_CHECK_FAILURE Error occurred checking if source is sorted
 			 * @retval cupcfd::error::E_SEARCH_NOT_FOUND Value was not found.
 			 */
 			// template <class I, class T>
 			// __attribute__((warn_unused_result))
 			// cupcfd::error::eCodes binarySearch(T * source, I nEle, T target, bool * found);
 		} // namespace drivers
 	} // namespace utility
} // namespace cupcfd

// Definitions for the declarations
#include "SearchDrivers.ipp"

#endif
