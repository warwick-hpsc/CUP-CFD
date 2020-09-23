/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * This file contains the declarations for the drivers of the utility sort operations
 */

#ifndef CUPCFD_UTILITY_SORT_DRIVERS_INCLUDE_H
#define CUPCFD_UTILITY_SORT_DRIVERS_INCLUDE_H

#include "Error.h"

namespace cupcfd
{
	namespace utility
	{
		namespace drivers
		{
			// ToDo: Interface for generic sort here

			// ToDo: Interface for generic stable sort here

			// ToDo: Interface for generic sort with indexes here

			// ToDo: Interface for generic stable sort with indexes here

			// Merge Sort (Destructive)
			/**
			 * Destructive Merge-Sort.
			 * Sorts the source array in-place recursively so that it is sorted in ascending order
			 * based on the < and == operators of type T.
			 *
			 * @param source The source data array
			 * @param nele The size of source as the number of elements of type T
			 *
			 * @tparam I The type of the indexing scheme (integer based)
			 * @tparam T The type of the stored array data
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
			 */
			template <class I, class T>
			cupcfd::error::eCodes merge_sort(T * source, I nele);

			// Merge Sort (Non-Destructive, result in dest)
			/**
			 * Merge Sort - Non-Destructive
			 * Sorts the source array using merge-sort but stores the result in a separate array
			 * corresponding to the sorted order of the elements in source in ascending order
			 * based on the < and == operators of type T.
			 *
			 * @param source The source data array
			 * @param dest The destination data array to place the sorted results
			 * @param nEle The size of source/dest as the number of elements of type T
			 *
			 * @tparam I The type of the indexing scheme (integer based)
			 * @tparam T The type of the stored array data
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
			 */
			template <class I, class T>
			cupcfd::error::eCodes merge_sort(T * source,  T * dest, I nele);

			/**
			 * Merge Sort - Non-Destructive
			 * Sorts the source array using merge-sort but stores the result as an array of indexes
			 * corresponding to the sorted order of the elements in source in ascending order, the indexes
			 * representing their original position in source.
			 * based on the < and == operators of type T.
			 *
			 * @param source The source data array
			 * @param nSource The size of source as the number of elements of type T
			 * @param indexes The index array the result is written to by the function
			 *
			 * @tparam I The type of the indexing scheme (integer based)
			 * @tparam T The type of the stored array data
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
			 */
			template <class I, class T>
			cupcfd::error::eCodes merge_sort_index(T * source,  I nSource, I * indexes);

			/**
			 * Determine whether an array is sorted
			 *
			 * @param source Array to test
			 * @param nEle Size of source in the number of elements of type T
			 * @param result A pointer to the location where the result will be stored, true if it
			 * is sorted, false if it is not.
			 *
			 * @tparam I The type of the indexing scheme (integer based)
			 * @tparam T The type of the stored array data
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
			 * @retval cupcfd::error::E_ARRAY_SIZE_UNDERSIZED The number of reported elements in nEle is too small
			 * (i.e. < 0)
			 */
			template <class I, class T>
			cupcfd::error::eCodes is_sorted(T * source, I nEle, bool * result);

			/**
			 * Reorder an array: The element at source[i] is replaced by the value at source[indexes[i]]
			 * i.e. source[i] = source[indexes[i]]
			 * E.g:
			 * source = {10, 20, 30, 40, 50}
			 * indexes = {4, 3, 0, 2, 1}
			 * source after call = {50, 40, 10, 30, 20}
			 *
			 * @param source The array of data elements to be rearranged
			 * @param nEleSource The size of source as the number of elements of type T
			 * @param indexes The array of indexes to reorder by
			 * @param nEleIndexes The size of indexes as the number of elements of type T. Should be the same
			 * as nEleSource
			 *
			 * @tparam I The type of the indexing scheme (integer based)
			 * @tparam T The type of the stored array data
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
			 */
			template <class I, class T>
			cupcfd::error::eCodes sourceIndexReorder(T * source, I nEleSource, I * indexes, I nEleIndexes);

			/**
			 * Reorder an array: The element at source[i] is moved to source[indexes[i]]
			 * i.e. source[indexes[i]] = source[i]
			 * E.g:
			 * source = {10, 20, 30, 40, 50}
			 * indexes = {4, 3, 0, 2, 1}
			 * source after call = {30, 50, 40, 20, 10}
			 *
			 * @tparam I The type of the indexing scheme (integer based)
			 * @tparam T The type of the stored array data
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
			 */
			template <class I, class T>
			cupcfd::error::eCodes destIndexReorder(T * source, I nEleSource, I * indexes, I nEleIndexes);
		} // namespace drivers
	} // namespace utility
} // namespace cupcfd

// Definitions for the declarations
#include "SortDrivers.ipp"

#endif
