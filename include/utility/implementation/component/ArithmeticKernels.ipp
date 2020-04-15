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

#ifndef CUPCFD_ZERO_COMP_TOL_F
#define CUPCFD_ZERO_COMP_TOL_F 1.0E-8f
#endif

#ifndef CUPCFD_ZERO_COMP_TOL_D
#define CUPCFD_ZERO_COMP_TOL_D 1.0E-8
#endif

namespace cupcfd
{
	namespace utility
	{
		namespace arithmetic
		{
			namespace kernels
			{
				/** Tolerance range +/- from 0 for floats **/
				const static float tol_f = CUPCFD_ZERO_COMP_TOL_F;

				/** Tolerance range +/- from 0 for doubles **/
				const static double tol_d = CUPCFD_ZERO_COMP_TOL_D;

				inline bool isEqual(int val, int target)
				{
					if(val == target)
					{
						return true;
					}

					return false;
				}

				// inline bool isEqual(float val, float target)
				// {
				// 	// C++ also uses abs as function name for floats, doubles
				// 	if(fabsf(val - target) < CUPCFD_ZERO_COMP_TOL_F)
				// 	{
				// 		return true;
				// 	}

				// 	return false;
				// }

				inline bool isEqual(float val, float target, float tolerance)
				{
					// C++ also uses abs as function name for floats, doubles
					if(fabsf(val - target) < tolerance)
					{
						return true;
					}

					return false;
				}

				inline bool isEqual(double val, double target)
				{
					// C++ also uses abs as function name for floats, doubles
					if(fabs(val - target) < CUPCFD_ZERO_COMP_TOL_D)
					{
						return true;
					}

					return false;
				}

				inline bool isEqual(double val, double target, double tolerance)
				{
					// C++ also uses abs as function name for floats, doubles
					if(fabs(val - target) < tolerance)
					{
						return true;
					}

					return false;
				}

				inline double sqrtWr(double val)
				{
					return sqrt(val);
				}

				inline float sqrtWr(float val)
				{
					return sqrtf(val);
				}

				template <class T>
				inline int sgn(T v)
				{
					// Based on: https://helloacm.com/how-to-implement-the-sgn-function-in-c
					// Ideally would like to do without the branch this will introduce,
					// but a tolerance check is v. likely needed around zero for floating point
					if(isEqual(v, (T) 0))
					{
						return 0;
					}

					return (((T) v) > (T) 0) - (((T) v) < (T) 0);
				}
			}
		}
	}
}

#endif
