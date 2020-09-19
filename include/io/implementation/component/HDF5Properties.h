/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 */

#ifndef CUPCFD_IO_HDF5_PROPERTIES_INCLUDE_H
#define CUPCFD_IO_HDF5_PROPERTIES_INCLUDE_H

#include "hdf5.h"
#include <string>
#include <vector>

#include "HDF5Access.h"
#include "Error.h"

namespace cupcfd
{
	namespace io
	{
		namespace hdf5
		{
			class HDF5Access;

			class HDF5Properties
			{
				public:
					int ndim;								// Number of dimensions
					std::vector<unsigned long long> dim;	// Dimension Sizes of total dataset/attribute
					// int type;	// Type Checking of what is stored in the file (enum?)

					unsigned long long nidx;
					// Store indexes in serialised form
					std::vector<unsigned long long> idx;
					hid_t memspaceID;		// Active Memory Space

					HDF5Properties(HDF5Access& access);
					virtual ~HDF5Properties();

					/**
					 *
					 */
					cupcfd::error::eCodes addIndex(unsigned long long x);

					/**
					 *
					 */
					cupcfd::error::eCodes addIndex(unsigned long long x,
								  unsigned long long y);

					/**
					 *
					 */
					cupcfd::error::eCodes addIndex(unsigned long long x,
								  unsigned long long y,
								  unsigned long long z);

					/**
					 *
					 */
					cupcfd::error::eCodes addIndex(unsigned long long x,
								  unsigned long long y,
								  unsigned long long z,
								  unsigned long long u);

					/**
					 *
					 */
					cupcfd::error::eCodes addIndex(unsigned long long x,
								  unsigned long long y,
								  unsigned long long z,
								  unsigned long long u,
								  unsigned long long v);

					/**
					 *
					 */
					cupcfd::error::eCodes addIndex(unsigned long long x,
								  unsigned long long y,
								  unsigned long long z,
								  unsigned long long u,
								  unsigned long long v,
								  unsigned long long w);
			};
		}
	}
}

#endif
