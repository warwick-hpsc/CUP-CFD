/**
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

#ifndef CUPCFD_UTILITY_ARRAY_KERNELS_INCLUDE_H
#define CUPCFD_UTILITY_ARRAY_KERNELS_INCLUDE_H

namespace cupcfd
{
	namespace utility
	{
		namespace kernels
		{

		   /**
			* Add the elements at position i of each array and store the result at position i in a third array.
			*
			* @param source1 An array to add values from. Also will be overwritten to store the result.
			* @param source2 A second array to add values from.
			* @param dest The array to store the results in.
			* @param nEle The number of elements to process.
			*
			* @tparam I The type of the indexing scheme
			* @tparam T The type of the stored array data
			*
			* @return Returns nothing
			*/
			// template <class I, class T>
			// void add(T * source1, T * source2, T * dest, I nEle);

			/**
			* Return the number of elements in the array that are unique (i.e. there exist no duplicates)
			*
			* @param source The array contaning the elements for testing. The array must be sorted.
			* @param nEle The number of elements to process
			*
			* @tparam I The type of the indexing scheme
			* @tparam T The type of the stored array data
			*
			* @return The number of unique elements (i.e. with no duplicates) in the array.
			*/
			template <class I, class T>
			I uniqueCount(T * source, I nEle);

			/**
			* Retrieve the elements that are unique in an array (i.e. there exist no duplicates)
			*
			* @param source The array of elements to be processed.
			* @param dest The array where  a copy of the unique elements will be stored.
			* @param nEle The number of elements to process.
			*
			* @tparam I The type of the indexing scheme
			* @tparam T The type of the stored array data
			*
			* @return Returns nothing
			*/
			template <class I, class T>
			void uniqueArray(T * source, T * dest, I nEle);

			/**
			* Calculate the number of distinct elements in an array (i.e. count the first instance, but ignore duplicates)
			* @param source An array of elements to count the number of distinct elements in. It must be sorted.
			* @param nEle The number of elements to process.
			*
			* @tparam I The type of the indexing scheme
			* @tparam T The type of the stored array data
			*
			* @return The number of distinct elements (i.e. counts the first instance, but not duplicates)
			*/
			template <class I, class T>
			I distinctCount(T * source, I nEle);

			/**
			* Build an array of distinct elements from the source array (i.e. count the first instance of an element, but not duplicates), and
			* also store the number of instances of each element in the source array.
			*
			* @param source An array of elements to find the distinct elements in. This must be sorted.
			* @param dst The array where a copy of each distinct element will be stored. It will be in a sorted order.
			* @param nEle The number of elements to process in source.
			*
			* @tparam I The type of the indexing scheme
			* @tparam T The type of the stored array data
			*
			* @return Returns nothing
			*/
			template <class I, class T>
			void distinctArray(T * source, T * dst, I nEle);

			/**
			* Processes an array, and returns (a) an array consisting of the distinct members of the array, and (b)
			* a count of how many instances of each member there were in the corresponding index of a second array.
			* @param source The array containing the elements to be processed. This must be sorted.
			* @param dst The array that will hold the distinct elements of source. It will be in a sorted order.
			* @param dupCount An array that will be updated to hold how many instances of each element were encountered.
							  dupCount[i] will hold the number of elements for the member in dst[i].
			* @param nEle The number of elements to process in source.
			*
			* @tparam I The type of the indexing scheme
			* @tparam T The type of the stored array data
			*
			* @return Returns nothing
			*/
			template <class I, class T>
			void distinctArray(T * source, T * dst, I * dupCount, I nEle);

			/**
			* Computes the number of elements that appear in source1, but not in source 2, aka a count of a set minus (A - B)
			*
			* @param source1 A in the set minus (A - B)
			* @param nSource1 The number of elements in source1
			* @param source2 B in the set minus (A - B)
			* @param nSource2 The number of elements in source2
			*
			* @tparam I The type of the indexing scheme
			* @tparam T The type of the stored array data
			*
			* @return The number of elements that appear in source1, but not in source 2, including duplicates
			*/
			template <class I, class T>
			I minusCount(T * source1, I nSource1, T * source2, I nSource2);

			/**
			* Computes the array that is array1 minus array2 (set operation) - i.e. those elements that are in array1 but not array 2.
			* This method requires array1 and array2 to be sorted at time of input.
			*
			* @param source1 The first array in the set minus - i.e. 'A'
			* @param nSource1 The size of source1 in the number of elements of type T - should be equal to nSource2
			* @param source2 The second array in the set minus - i.e. 'B'
			* @param nSource2 The size of source2 in the number of elements of type T - should be equal to nSource1
			* @param result A pointer to the location where the result array will be written
			* @param nResult The size of the result array in the number of elements of type T. This should be equal to or
			* greater than the size of source1/source2
			*
			* @tparam I The type of the indexing scheme
			* @tparam T The type of the stored array data
			*
			* @return Returns nothing
			*/
			// template <class I, class T>
			// void minusArray(T * source1, I nSource1, T * source2, I nSource2, T * result, I nResult);


			/**
			 * Computes the number of elements that is in the intersection of two arrays - i.e. those elements that are in
			 * present in both arrays. This function includes duplicates in that count.
			 *
			 * @param source1 The first array of elements in the intersection
			 * @param nSource1 The size of source1 in the number of elements of type T - should be equal to nSource2
			 * @param source2 The second array of elements in the intersection
			 * @param nSource2 The size of source2 in the number of elements of type T - should be equal to nSource1
			 *
			 * @tparam I The type of the indexing scheme
			 * @tparam T The type of the stored array data
			 *
			 * @return The number of elements that appear in source1 and in source 2, including duplicates
			 */
			template <class I, class T>
			I intersectCount(T * source1, I nSource1, T * source2, I nSource2);

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
			 * @return Returns nothing
			 */
			template <class I, class T>
			__attribute__((warn_unused_result))
			cupcfd::error::eCodes intersectArray(T * source1, I nSource1, T * source2, I nSource2, T * result, I nResult);

			/**
			 * Populates an array with random numbers, generated between lower_bound and upper_bound with
			 * a uniform distribution
			 *
			 * @param dest The array to populate with random data - this will be modified by this function
			 * @param nDest The size of dest in the number of elements of type T
			 * @param lower_bound The number to use for the lower bound of the number generation
			 * @param upper_bound The number to use for the upper bound of the number generation
			 *
			 * @tparam I The type of the indexing scheme
			 * @tparam T The type of the stored array data
			 *
			 * @return Returns nothing
			 */
			template <class I, class T>
			void randomUniform(T * dest, I nDest, T lower_bound, T upper_bound);

		} // namespace kernels
	} // namespace utility
} // namespace cupcfd

// Definitions for the declarations
#include "ArrayKernels.ipp"

#endif
