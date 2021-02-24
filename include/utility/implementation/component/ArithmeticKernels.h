/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains doxygen and declarations for kernel operations
 * pertaining to arithmetic comparisons or computation
 */

#ifndef CUPCFD_UTILITY_ARITHMETIC_INCLUDE_H
#define CUPCFD_UTILITY_ARITHMETIC_INCLUDE_H

namespace cupcfd
{
	namespace utility
	{
		namespace arithmetic
		{
			namespace kernels
			{
				/**
				 * Compare whether two integers are equivalent
				 * Primarily of use as a generic wrapper for templated functions where
				 * it is not known that the type is an integer.
				 *
				 * @param val The first value to compare
				 * @param target The second value to compare
				 *
				 * @return A boolean indicating whether the values are equivalent.
				 * @retval true The values are considered equivalent.
				 * @retval false The values are not considered equivalent.
				 */
				__attribute__((warn_unused_result))
				inline bool isEqual(int val, int target);

				/**
				 * Compare whether two floats are equivalent within a tolerance.
				 * The tolerance is fixed at runtime.
				 *
				 * @param val The first value to compare
				 * @param target The second value to compare
				 *
				 * @return A boolean indicating whether the values are within
				 * a tolerance of one another and considered equivalent.
				 * @retval true The values are considered equivalent.
				 * @retval false The values are not considered equivalent.
				 */
				__attribute__((warn_unused_result))
				inline bool isEqual(float val, float target);

				/*
				 * Compare whether two floats are equivalent within a tolerance.
				 *
				 * @param val The first value to compare
				 * @param target The second value to compare
				 * @param tolerance The tolerance to use for the comparison
				 *
				 * @return A boolean indicating whether the values are within
				 * a tolerance of one another and considered equivalent.
				 * @retval true The values are considered equivalent.
				 * @retval false The values are not considered equivalent.
				 */
				__attribute__((warn_unused_result))
				inline bool isEqual(float val, float target, float tolerance);

				/*
				 * Compare whether two doubles are equivalent within a tolerance.
				 * The tolerance is fixed at runtime.
				 *
				 * @param val The first value to compare
				 * @param target The second value to compare
				 *
				 * @return A boolean indicating whether the values are within
				 * a tolerance of one another and considered equivalent.
				 * @retval true The values are considered equivalent.
				 * @retval false The values are not considered equivalent.
				 */
				__attribute__((warn_unused_result))
				inline bool isEqual(double val, double target);

				/*
				 * Compare whether two doubles are equivalent within a tolerance.
				 *
				 * @param val The first value to compare
				 * @param target The second value to compare
				 * @param tolerance The tolerance to use for the comparison
				 *
				 * @return A boolean indicating whether the values are within
				 * a tolerance of one another and considered equivalent.
				 * @retval true The values are considered equivalent.
				 * @retval false The values are not considered equivalent.
				 */
				__attribute__((warn_unused_result))
				inline bool isEqual(double val, double target, double tolerance);

				/**
				 * Wrapper for square root - used to distinguish different square root calls by
				 * specialisation on type
				 *
				 * @param val Value to compute the square root of
				 *
				 * @return The square root
				 */
				__attribute__((warn_unused_result))
				inline double sqrtWr(double val);

				/**
				 * Wrapper for square root - used to distinguish different square root calls by
				 * specialisation on type
				 *
				 * @param val Value to compute the square root of
				 *
				 * @return The square root
				 */
				__attribute__((warn_unused_result))
				inline float sqrtWr(float val);

				/**
				 * Determine the sign of the provided value.
				 *
				 * @tparam Type of value to test sign of
				 *
				 * @return A number indicating whether the sign is positive, negative or zero
				 * @retval 0 The value of v is zero
				 * @retval 1 The sign of v is positive
				 * @retval -1 The sign of v is negative
				 */
				template <class T>
				__attribute__((warn_unused_result))
				inline int sgn(T v);
			}
		}
	}
}

// Definitions for the declarations
#include "ArithmeticKernels.ipp"

#endif
