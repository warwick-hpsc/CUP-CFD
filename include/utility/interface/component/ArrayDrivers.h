/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains Declarations for drivers of array operations
 */

#ifndef CUPCFD_UTILITY_ARRAY_DRIVERS_INCLUDE_H
#define CUPCFD_UTILITY_ARRAY_DRIVERS_INCLUDE_H

#include "Error.h"

namespace cupcfd
{
	namespace utility
	{
		namespace drivers
		{
			/**
			 * Array copy: copy elements from src to dst.
			 *
			 * @param src The source array to copy from
			 * @param nSrc Number of elements in the src array/number of elements copied
			 * @param dst The destination array to copy to.
			 * @param nDst The number of elements in the destination array
			 *
		 	 * @tparam I The type of the indexing scheme
			 * @tparam T The type of the stored array data
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
			 * @retval cupcfd::error::E_ARRAY_SIZE_UNDERSIZED The destination is too small to copy the source array
			 */
			template <class I, class T>
			cupcfd::error::eCodes copy(T * src, I nSrc, T * dst, I nDst);

			/**
			 * Fill an array with zeroes
			 *
			 * @param source The array to zero.
			 * @param nEle The size of the array/number of elements to zero.
			 *
		 	 * @tparam I The type of the indexing scheme
			 * @tparam T The type of the stored array data
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
			 */
			template <class I, class T>
			cupcfd::error::eCodes zero(T * source, I nEle);

			/**
			 * Return the number of elements in the array that are unique (i.e. there exist no duplicates)
			 *
			 * @param source The array containing the elements for testing.
			 * @param nSourceEle The size of the source array, as the number of elements of type T
			 *
		 	 * @tparam I The type of the indexing scheme
			 * @tparam T The type of the stored array data
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
			 */
			template <class I, class T>
			cupcfd::error::eCodes uniqueCount(T * source, I nSourceEle, I * count);

			/**
			 * Retrieve the elements that are unique in an array (i.e. there exist no duplicates)
			 *
			 * @param source The array of elements to be processed.
			 * @param nSourceEle The size of source, as the number of elements of type T
			 * @param dest A pointer to a location where the pointer to a new array containing the unique elements will be stored.
			 * @param nDestEle A pointer to a location where the size of dest is stored, as the number of elements of type T
			 *
		 	 * @tparam I The type of the indexing scheme
			 * @tparam T The type of the stored array data
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
			 */
			template <class I, class T>
			// cupcfd::error::eCodes uniqueArray(T * source, I nSourceEle, T ** dest, I * nDestEle);
			cupcfd::error::eCodes uniqueArray(T * source, I nSourceEle, T ** dest);

			/**
			 * Retrieve the elements that are unique in an array (i.e. there exist no duplicates)
			 *
			 * @param source The array of elements to be processed.
			 * @param nSourceEle The size of source, as the number of elements of type T
			 * @param dest The array where the results array will be stored
			 * @param nDestEle The size of dest, as the number of elements of type T
			 *
		 	 * @tparam I The type of the indexing scheme
			 * @tparam T The type of the stored array data
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
			*/
			template <class I, class T>
			cupcfd::error::eCodes uniqueArray(T * source, I nSourceEle, T * dest, I nDestEle);


			/**
			 * Add two arrays: at each index an element from each array is summed and stored in source1.
			 *
			 * @param source1 An array to add values from. Also will be overwritten to store the result.
			 * @param nSourceEle The size of source1, as the number of elements of type T
			 * @param source2 A second array to add values from.
			 * @param nSource2Ele The size of source2, as the number of elements of type T
			 *
		 	 * @tparam I The type of the indexing scheme
			 * @tparam T The type of the stored array data
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
			 */
			// template <class I, class T>
			// cupcfd::error::eCodes add(T * source1, I nSource1Ele, T * source2, I nSource2Ele);

			/**
			 * Add two arrays: at each index an element from each array is summed.
			 *
			 * @param source1 An array to add values from. Also will be overwritten to store the result.
			 * @param nSourceEle The size of source1, as the number of elements of type T
			 * @param source2 A second array to add values from.
			 * @param nSource2Ele The size of source2, as the number of elements of type T
			 * @param dest The array where the result of the add will be stored.
			 * @param nDestEle The size of dest, as the number of elements of type T. This should be the same
			 * as the size of source1 and source2
			 *
		 	 * @tparam I The type of the indexing scheme
			 * @tparam T The type of the stored array data
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
			 */
			// template <class I, class T>
			// cupcfd::error::eCodes add(T * source1, I nSource1Ele,T * source2, I nSource2Ele, T * dest, I nDestEle);

			/**
			 * Calculate the number of distinct elements in an array (i.e. count the first instance, but ignore duplicates)
			 * @param source An array of elements to count the number of distinct elements in.
			 * @param nEle The size of source, as the number of elements of type T
			 * @param count A pointer to the location where the distinct count will be stored.
			 *
		 	 * @tparam I The type of the indexing scheme
			 * @tparam T The type of the stored array data
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
			*/
			template <class I, class T>
			cupcfd::error::eCodes distinctCount(T * source, I nEle, I * count);

			/**
			 * Build an array of the distinct elements in a source array (i.e. count the first instance of an element, but not duplicates).
			 *
			 * @param source An array of elements to find the distinct elements in.
			 * @param nEleSource The size of source, as the number of elements of type T
			 * @param dst The array where a copy of each distinct element will be stored.
			 * @param nEleDst The size of dst, as the number of elements of type T. Must be equal to or larger than the number of distinct elements.
			 *
		 	 * @tparam I The type of the indexing scheme
			 * @tparam T The type of the stored array data
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
			 */
			template <class I, class T>
			cupcfd::error::eCodes distinctArray(T * source, I nEleSource, T * dst, I nEleDst);

			/**
			 * Build an array of the distinct elements in a source array (i.e. count the first instance of an element, but not duplicates).
			 *
			 * @param source An array of elements to find the distinct elements in.
			 * @param nEleSource The size of source, as the number of elements of type T
			 * @param dst A pointer to the location where a pointer to a new array will be stored, and a copy of each distinct element will be stored.
			 * @param nEleDst A pointer where the size of the pointer created at dst will be stored.
			 *
		 	 * @tparam I The type of the indexing scheme
			 * @tparam T The type of the stored array data
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
			*/
			template <class I, class T>
			cupcfd::error::eCodes distinctArray(T * source, I nEleSource, T ** dst, I * nEleDst);

			/**
			 * Build an array of the distinct elements in a source array (i.e. count the first instance of an element, but not duplicates), and
			 * also store the number of instances of each element in the source array in a second result array at the corresponding index.
			 *
			 * @param source An array of elements to find the distinct elements in.
			 * @param nEleSource The size of source, as the number of elements of type T
			 * @param dst The array where a copy of each distinct element will be stored.
			 * @param nEleDst The size of dst, as the number of elements of type T. Must be equal to or larger than the number of distinct elements.
			 * @param dupCount The array where the number of instances of the element at dst[i] is stored at dupCount[i].
			 * @param The size of dupCount, as the number of elements of type T
			*
		 	 * @tparam I The type of the indexing scheme
			 * @tparam T The type of the stored array data
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
			*/
			template <class I, class T>
			cupcfd::error::eCodes distinctArray(T * source, I nEleSource, T * dst, I nEleDst, I * dupCount);

			/**
			 * Build an array of the distinct elements in a source array (i.e. count the first instance of an element, but not duplicates), and
			 * also store the number of instances of each element in the source array in a second result array at the corresponding index.
			 *
			 * @param source An array of elements to find the distinct elements in.
			 * @param nEleSource The size of source, as the number of elements of type T
			 * @param dst A pointer to the location where a pointer to a new array will be stored, and a copy of each distinct element will be stored.
			 * @param nEleDst A pointer where the size of the pointer created at dst will be stored, as the number of elements of type T
			 * @param dupCount A pointer to the location where an array will be created,
			 * containing the number of instances of the element at dst[i] is stored at dupCount[i].
			 * @param A pointer to the location where the size of dupCount will be stored, as the number of elements of type T
			*
		 	 * @tparam I The type of the indexing scheme
			 * @tparam T The type of the stored array data
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
			*/
			template <class I, class T>
			cupcfd::error::eCodes distinctArray(T * source, I nEleSource, T ** dst, I * nEleDst, I ** dupCount);

			/**
			 * Computes the number of elements that appear in source1, but not in source 2, aka a count of a set minus (A - B).
			 * This includes duplicates.
			 *
			 * @param source1 A in the set minus (A - B)
			 * @param nSource1 The size of source1, as the number of elements of type T
			 * @param source2 B in the set minus (A - B)
			 * @param nSource2 The size of source2, as the number of elements of type T
			 * @param count A pointer to the location where the number of elements in source1 but not source2 (including duplicates) is stored
			 *
			 * @tparam I The type of the indexing scheme
			 * @tparam T The type of the stored array data
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
			 */
			template <class I, class T>
			cupcfd::error::eCodes minusCount(T * source1, I nSource1, T * source2, I nSource2, I * count);

			/**
			 * Computes the number of elements that appear in source1, but not in source 2, aka a count of a set minus (A - B).
			 * This does not count duplicates.
			 *
			 * @param source1 A in the set minus (A - B)
			 * @param nSource1 The size of source1, as the number of elements of type T
			 * @param source2 B in the set minus (A - B)
			 * @param nSource2 The size of source2, as the number of elements of type T
			 * @param count A pointer to the location where the number of elements in source1 but not source2 (including duplicates) is stored
			 *
			 * @tparam I The type of the indexing scheme
			 * @tparam T The type of the stored array data
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
			 */
			template <class I, class T>
			cupcfd::error::eCodes minusDistinctCount(T * source1, I nSource1, T * source2, I nSource2, I * count);

			/**
			 * Computes the array of elements that appear in source1, but not in source 2, aka a set minus (A - B).
			 *
			 * @param source1 A in the set minus (A - B)
			 * @param nSource1 The number of elements in source1
			 * @param source2 B in the set minus (A - B)
			 * @param nSource2 The number of elements in source2
			 * @param result A pointer to the location where a pointer for a newly created array will be stored. The array
			 * will contain any element that appears in source1 but not source2, including duplicates
			 * @param nResult A pointer to the location where the size of the array created at the location at by result is stored,
			 * as the number of elements of type T
			 *
			 * @tparam I The type of the indexing scheme
			 * @tparam T The type of the stored array data
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
			 */
			// template <class I, class T>
			// cupcfd::error::eCodes minusArray(T * source1, I nSource1, T * source2, I nSource2, T ** result, I * nResult);

			/**
			 * Computes the number of elements that is in the intersection of two arrays - i.e. those elements that are in
			 * present in both arrays. This function includes duplicates in that count.
			 *
			 * @param source1 The first array of elements in the intersection
			 * @param nSource1 The size of source1 in the number of elements of type T - should be equal to nSource2
			 * @param source2 The second array of elements in the intersection
			 * @param nSource2 The size of source2 in the number of elements of type T - should be equal to nSource1
			 * @param count A pointer to the location where the number of elements in source1 but not source2 (including duplicates) is stored
			 *
			 * @tparam I The type of the indexing scheme
			 * @tparam T The type of the stored array data
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
			 */
			template <class I, class T>
			cupcfd::error::eCodes intersectCount(T * source1, I nSource1, T * source2, I nSource2, I * count);

			/**
			 * Computes the array of elements that is in the intersection of two arrays - i.e. those elements that are in
			 * present in both arrays. This function includes duplicates in the array construction.
			 *
			 * @param source1 The first array of elements in the intersection
			 * @param nSource1 The size of source1 in the number of elements of type T - should be equal to nSource2
			 * @param source2 The second array of elements in the intersection
			 * @param nSource2 The size of source2 in the number of elements of type T - should be equal to nSource1
			 * @param result A pointer to the location where the result array will be written
		 	 * @param nResult The size of the result array in the number of elements of type T. This should be equal to or
		 	 * greater than nSource1/nSource2
			 *
			 * @tparam I The type of the indexing scheme
			 * @tparam T The type of the stored array data
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
			 */
			template <class I, class T>
			cupcfd::error::eCodes intersectArray(T * source1, I nSource1, T * source2, I nSource2, T ** result, I * nResult);

		} // namespace drivers
	} // namespace array
} // namespace cupcfd

// Definitions for the declarations
#include "ArrayDrivers.ipp"

#endif
