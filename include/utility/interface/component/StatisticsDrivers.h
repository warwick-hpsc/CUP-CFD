/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the declarations for the drivers of
 * the statistic utility functions.
 */

#ifndef CUPCFD_UTILITY_STATISTICS_DRIVERS_INCLUDE_H
#define CUPCFD_UTILITY_STATISTICS_DRIVERS_INCLUDE_H

#include "Error.h"

namespace cupcfd
{
	namespace utility
	{
		namespace drivers
		{
			/**
			 * Compute the sum of the values in an array
			 *
			 * @param source The source data array
			 * @param nEle The size of the source array, as the number of elements of type T
			 * @param result A pointer to where the result will be stored
			 *
			 * @tparam I The type of the indexing scheme (integer based)
			 * @tparam T The type of the stored array data
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
			 */
			template <class I, class T>
			void sum(T * source, I nEle, T * result);

			/**
			 * Compute the mean of the values in an array
			 *
			 * @param source The source data array
			 * @param nEle The size of the source array, as the number of elements of type T
			 * @param result A pointer to the location where the mean will be stored
			 *
			 * @tparam I The type of the indexing scheme (integer based)
			 * @tparam T The type of the stored array data
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
			 */
			template <class I, class T>
			__attribute__((warn_unused_result))
			cupcfd::error::eCodes mean(T * source, I nEle, double * result);

			/**
			 * Compute the median of the values in an array
			 *
			 * @param source The source data array
			 * @param nEle The size of the source array, as the number of elements of type T
			 * @param result A pointer to where the result will be stored
			 *
			 * @tparam I The type of the indexing scheme (integer based)
			 * @tparam T The type of the stored array data
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
			 */
			template <class I, class T>
			__attribute__((warn_unused_result))
			cupcfd::error::eCodes median(T * source, I nEle, T * result);

			/**
			 * Compute the mode of the values in an array
			 *
			 * @param source The source data array
			 * @param nEle The size of the source array, as the number of elements of type T
			 * @param result A pointer to where the result will be stored
			 *
			 * @tparam I The type of the indexing scheme (integer based)
			 * @tparam T The type of the stored array data
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
			 */
			template <class I, class T>
			__attribute__((warn_unused_result))
			cupcfd::error::eCodes mode(T * source, I nEle, T * result);

			/**
			 * Compute the standard deviation of the values in an array
			 *
			 * @param source The source data array
			 * @param nEle The size of the source array, as the number of elements of type T
			 * @param result A pointer to where the result will be stored
			 *
			 * @tparam I The type of the indexing scheme (integer based)
			 * @tparam T The type of the stored array data
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
			 */
			template <class I, class T>
			__attribute__((warn_unused_result))
			cupcfd::error::eCodes stDev(T * source, I nEle, double * result);

			/**
			 * Count the number of instances of a particular element in an array - requires
			 * the == operator for type T
			 *
			 * @param source The source data array
			 * @param nEle The size of the source array, as the number of elements of type T
			 * @param target The element to search for
			 * @param result A pointer to where the result will be stored
			 *
			 * @tparam I The type of the indexing scheme (integer based)
			 * @tparam T The type of the stored array data
			 *
			 * @return An error status indicating the success or failure of the operation
			 * @retval cupcfd::error::E_ARRAY_SUCCESS Success
			 */
			template <class I, class T>
			__attribute__((warn_unused_result))
			cupcfd::error::eCodes count(T * source, I nEle, T target, I * result);
		} // namespace drivers
	} // namespace utility
} // namespace cupcfd

// Definitions for the declarations
#include "StatisticsDrivers.ipp"

#endif
