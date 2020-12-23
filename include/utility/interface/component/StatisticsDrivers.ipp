/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 * Contains the definitions for the drivers of the 
 * statistic utility functions at a header level.
 */

#include <cstdlib>

#ifndef CUPCFD_UTILITY_STATISTICS_DRIVERS_TPP_H
#define CUPCFD_UTILITY_STATISTICS_DRIVERS_TPP_H

#include "StatisticsKernels.h"

namespace cupcfd
{
	namespace utility
	{
		namespace drivers
		{
			template <class I, class T>
			void sum(T * source, I nEle, T * result) {
				if(nEle < 1) {
					*result = 0;
					return;
				}
				*result = kernels::sum(source, nEle);
			}

			template <class I, class T>
			cupcfd::error::eCodes mean(T * source, I nEle, double * result) {
				T sum = cupcfd::utility::kernels::sum(source, nEle);

				*result = ((double) sum) / nEle;

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes median(T * source, I nEle, T * result) {
				*result = cupcfd::utility::kernels::median(source, nEle);

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes mode(T * source, I nEle, T * result) {
				*result = cupcfd::utility::kernels::mode(source, nEle);

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes stDev(T * source, I nEle, double * result) {
				*result = cupcfd::utility::kernels::stDev(source, nEle);

				return cupcfd::error::E_SUCCESS;
			}

			template <class I, class T>
			cupcfd::error::eCodes count(T * source, I nEle, T target, I * result) {
				*result = cupcfd::utility::kernels::count(source, nEle, target);

				return cupcfd::error::E_SUCCESS;
			}
		}
	}
}

#endif
