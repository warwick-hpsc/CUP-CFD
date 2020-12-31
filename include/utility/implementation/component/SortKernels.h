/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the declarations for the kernels of the utility sort operations.
 * Note: These could ptoentially be replaced by in-built C++ equivalents.
 */

#ifndef CUPCFD_UTILITY_SORT_KERNELS_INCLUDE_H
#define CUPCFD_UTILITY_SORT_KERNELS_INCLUDE_H

namespace cupcfd
{
	namespace utility
	{
		namespace kernels
		{
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
			 * @return Monitoring value for identifying when recursion is complete
			 */
			template <class I, class T>
			__attribute__((warn_unused_result))
			int merge_sort(T * source, I nele);


			/**
			 * Merge Sort - Destructive
			 * Sorts the source array using merge-sort but also stores an array of indexes that
			 * reference the indexes of their original positions in source source in ascending order
			 * based on the < and == operators of type T.
			 *
			 * The value in indexes[i] refers to the original index position of the element that is
			 * in source[i] after the sort
			 *
			 * @param source The source data array
			 * @param nSource The size of source as the number of elements of type T
			 * @param indexes The index array the result is written to by the function
			 *
			 * @tparam I The type of the indexing scheme (integer based)
			 * @tparam T The type of the stored array data
			 *
			 * @return Monitoring value for identifying when recursion is complete
			 */
			template <class I, class T>
			__attribute__((warn_unused_result))
			int merge_sort(T * source,  I nSource, I * indexes, I nIndexes);

			/**
			 * Evaluate whether an array is sorted in ascending order based on the < and == operators of type T.
			 *
			 * @param source The source data array
			 * @param nEle The size of source as the number of elements of type T
			 *
			 * @tparam I The type of the indexing scheme (integer based)
			 * @tparam T The type of the stored array data
			 *
			 * @return A boolean indicating whether the array is sorted
			 * @retval true The array is sorted
			 * @retval false The array is not sorted
			 */
			template <class I, class T>
			__attribute__((warn_unused_result))
			bool is_sorted(T * source, I nEle);

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
			 * @return Returns nothing
			 */
			template <class I, class T>
			__attribute__((warn_unused_result))
			cupcfd::error::eCodes sourceIndexReorder(T * source, I nEleSource, I * indexes, I nEleIndexes);

			/**
			 * Reorder an array: The element at source[i] is moved to source[indexes[i]]
			 * i.e. source[indexes[i]] = source[i]
			 * E.g:
			 * source = {10, 20, 30, 40, 50}
			 * indexes = {4, 3, 0, 2, 1}
			 * source after call = {30, 50, 40, 20, 10}
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
			 * @return Returns nothing
			 */
			template <class I, class T>
			__attribute__((warn_unused_result))
			cupcfd::error::eCodes destIndexReorder(T * source, I nEleSource, I * indexes, I nEleIndexes);
		} // namespace kernels
	} // namespace utility
} // namespace cupcfd

// Definitions for the declarations
#include "SortKernels.ipp"

#endif
