/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Description
 *
 * Contains the implementations of the search kernels for utility functions at a header level.
 */

#ifndef CUPCFD_UTILITY_SEARCH_KERNELS_TPP_H
#define CUPCFD_UTILITY_SEARCH_KERNELS_TPP_H

#include <cmath>

namespace cupcfd
{
	namespace utility
	{
		namespace kernels
		{
			template <class I, class T>
 			I linearSearch(T * source, I nEle, T target)
 			{
 				for(I i = 0; i < nEle; i++)
 				{
 					if(source[i] == target)
 					{
 						return i;
 					}
 				}

 				// Target not found. Return -1 to signify no valid index.
 				return -1;
 			}

 			template <class I, class T>
 			I binarySearch(T * source, I nEle, T target)
 			{
				I left = 0;
				I right = nEle - 1;
				I pivot;

				while(left <= right)
				{
					pivot = floor(left + right)/2.0;
					if(source[pivot] < target)
					{
						left = pivot + 1;
					}
					else if(source[pivot] > target)
					{
						right = pivot - 1;
					}
					else
					{
						return pivot;
					}
				}
				return -1;
 			}
		} // namespace kernels
	} // namespace search
} // namespace cupcfd

#endif
