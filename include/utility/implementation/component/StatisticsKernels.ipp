/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains implementations of utility statistical functions
 */

#ifndef CUPCFD_UTILITY_STATISTICS_KERNELS_TPP_H
#define CUPCFD_UTILITY_STATISTICS_KERNELS_TPP_H

#include <cstdlib>

#include "StatisticsKernels.h"

namespace cupcfd
{
	namespace utility
	{
		namespace kernels
		{
			template <class I, class T>
			T sum(T * source, I nEle) {
				T result = (T) 0;
				for(I i = 0; i < nEle; i++) {
					result += source[i];
				}

				return result;
			}

			// template <class I, class T>
			// T median(T * source, I nEle)
			// {
			// 	// ToDo: Fill in Kernel
			// 	T result = (T) 0;

			// 	return result;
			// }

			// template <class I, class T>
			// T mode(T * source, I nEle)
			// {
			// 	// ToDo: Fill in Kernel
			// 	T result = (T) 0;

			// 	return result;
			// }

			// template <class I, class T>
			// T stDev(T * source, I nEle)
			// {
			// 	// ToDo: Fill in Kernel
			// 	T result = (T) 0;

			// 	return result;
			// }

			template <class I, class T>
			I count(T * source, I nEle, T target) {
				I count = 0;

				// Scan through array, add to counter for each instance of target found
				for(I i = 0; i < nEle; i++) {
					if(source[i] == target) {
						count = count + 1;
					}
				}

				return count;
			}
		} // namespace kernels
	} // namespace utility
} // namespace cupcfd

#endif
