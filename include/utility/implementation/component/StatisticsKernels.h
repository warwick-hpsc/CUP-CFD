/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Declarations for the kernels of the statistic utility functions
 */

#ifndef CUPCFD_UTILITY_STATISTICS_KERNELS_INCLUDE_H
#define CUPCFD_UTILITY_STATISTICS_KERNELS_INCLUDE_H

namespace cupcfd
{
	namespace utility
	{
		namespace kernels
		{
			/**
			 * Compute the sum of the values in an array
			 *
			 * @param source The source data array
			 * @param nEle The size of the source array, as the number of elements of type T
			 *
			 * @tparam I The type of the indexing scheme (integer based)
			 * @tparam T The type of the stored array data
			 *
			 * @return The array sum
			 */
			template <class I, class T>
			__attribute__((warn_unused_result))
			T sum(T * source, I nEle);

			/**
			 * Compute the median of the values in an array
			 *
			 * @param source The source data array
			 * @param nEle The size of the source array, as the number of elements of type T
			 *
			 * @tparam I The type of the indexing scheme (integer based)
			 * @tparam T The type of the stored array data
			 *
			 * @return The array median
			 */
			template <class I, class T>
			__attribute__((warn_unused_result))
			T median(T * source, I nEle);

			/**
			 * Compute the mode of the values in an array
			 *
			 * @param source The source data array
			 * @param nEle The size of the source array, as the number of elements of type T
			 *
			 * @tparam I The type of the indexing scheme (integer based)
			 * @tparam T The type of the stored array data
			 *
			 * @return The array mode
			 */
			template <class I, class T>
			__attribute__((warn_unused_result))
			T mode(T * source, I nEle);

			/**
			 * Compute the standard deviation of the values in an array
			 *
			 * @param source The source data array
			 * @param nEle The size of the source array, as the number of elements of type T
			 *
			 * @tparam I The type of the indexing scheme (integer based)
			 * @tparam T The type of the stored array data
			 *
			 * @return The array standard deviation
			 */
			template <class I, class T>
			__attribute__((warn_unused_result))
			T stDev(T * source, I nEle);

			/**
			 * Count the number of instances of a particular element in an array - requires
			 * the == operator for type T
			 *
			 * @param source The source data array
			 * @param nEle The size of the source array, as the number of elements of type T
			 * @param target The element to search for
			 *
			 * @tparam I The type of the indexing scheme (integer based)
			 * @tparam T The type of the stored array data
			 *
			 * @return The number of instances found of target
			 */
			template <class I, class T>
			__attribute__((warn_unused_result))
			I count(T * source, I nEle, T target);
		} // namespace kernels
	} // namespace utility
} // namespace cupcfd

// Definitions for the declarations
#include "StatisticsKernels.ipp"

#endif
