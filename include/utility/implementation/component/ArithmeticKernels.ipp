/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 * 
 * Contains the templated and non-templated definitions at a header
 * level for kernels that performn arithmetic comparisons or computation.
 */

#ifndef CUPCFD_UTILITY_ARITHMETIC_TPP_H
#define CUPCFD_UTILITY_ARITHMETIC_TPP_H

#include <cmath>

namespace cupcfd
{
	namespace utility
	{
		namespace arithmetic
		{
			namespace kernels
			{
				inline bool isEqual(int val, int target) {
					if(val == target) {
						return true;
					}

					return false;
				}

				inline bool isEqual(float val, float target) {
					return val == target;
				}

				// inline bool isEqual(float val, float target, float tolerance)
				// {
				// 	return val == target;
				// }

				inline bool isEqual(double val, double target) {
					return val == target;
				}

				// inline bool isEqual(double val, double target, double tolerance)
				// {
				// 	return val == target;
				// }

				inline double sqrtWr(double val) {
					return sqrt(val);
				}

				inline float sqrtWr(float val) {
					return sqrtf(val);
				}

				template <class T>
				inline int sgn(T v) {
					// Based on: https://helloacm.com/how-to-implement-the-sgn-function-in-c
					// Ideally would like to do without the branch this will introduce,
					// but a tolerance check is v. likely needed around zero for floating point
					if(isEqual(v, (T) 0)) {
						return 0;
					}

					return (((T) v) > (T) 0) - (((T) v) < (T) 0);
				}
			}
		}
	}
}

#endif
