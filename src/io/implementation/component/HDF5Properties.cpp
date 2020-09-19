/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 */

#include "HDF5Properties.h"

using namespace cupcfd::io::hdf5;

namespace cupcfd
{
	namespace io
	{
		namespace hdf5
		{
			HDF5Properties::HDF5Properties(HDF5Access& access)
			{
				// hid_t hdf5_type;
				// H5T_class_t hdf5_type_class;
				// size_t hdf5_type_size;
				int ndims;

				// Get Type Object
				// if(access.record.attr == true)
				// {
				// 	hdf5_type = H5Aget_type(access.attrID);
				// }
				// else
				// {
				// 	hdf5_type = H5Dget_type(access.datasetID);
				// }

				// Retrieve DataType as stored in the HDF5 File.
				// hdf5_type_class = H5Tget_class(hdf5_type);

				// Get DataType Size as stored in the HDF5 File.
				// hdf5_type_size = H5Tget_size(hdf5_type);

				// Retrieve Number of Dimensions of the record in the current file
				ndims = H5Sget_simple_extent_ndims(access.dataspaceID);

				// Retrieve Size of Dimensions of the record in the current file
				hsize_t dims[ndims];
				hsize_t maxdims[ndims];

				H5Sget_simple_extent_dims(access.dataspaceID, dims, maxdims);

				// Store appropriately in the HDF5Properties Object for access
				this->ndim = ndims;
				for(int i=0; i<ndims; i++)
				{
					this->dim.push_back(dims[i]);
				}

				this->nidx = 0;
				this->memspaceID = H5S_ALL;
			}

			HDF5Properties::~HDF5Properties()
			{

			}

			cupcfd::error::eCodes HDF5Properties::addIndex(unsigned long long x)
			{
				this->idx.push_back(x);
				this->nidx = this->nidx + 1;

				return cupcfd::error::E_SUCCESS;
			}

			cupcfd::error::eCodes HDF5Properties::addIndex(unsigned long long x,
						  	  	  	  	  unsigned long long y)
			{
				this->idx.push_back(x);
				this->idx.push_back(y);
				this->nidx = this->nidx + 1;

				return cupcfd::error::E_SUCCESS;
			}

			cupcfd::error::eCodes HDF5Properties::addIndex(unsigned long long x,
										  unsigned long long y,
										  unsigned long long z)
			{
				this->idx.push_back(x);
				this->idx.push_back(y);
				this->idx.push_back(z);
				this->nidx = this->nidx + 1;

				return cupcfd::error::E_SUCCESS;
			}

			cupcfd::error::eCodes HDF5Properties::addIndex(unsigned long long x,
						  	  	  	  	  unsigned long long y,
										  unsigned long long z,
										  unsigned long long u)
			{
				this->idx.push_back(x);
				this->idx.push_back(y);
				this->idx.push_back(z);
				this->idx.push_back(u);
				this->nidx = this->nidx + 1;

				return cupcfd::error::E_SUCCESS;
			}

			cupcfd::error::eCodes HDF5Properties::addIndex(unsigned long long x,
						  	  	  	  	  unsigned long long y,
										  unsigned long long z,
										  unsigned long long u,
										  unsigned long long v)
			{
				this->idx.push_back(x);
				this->idx.push_back(y);
				this->idx.push_back(z);
				this->idx.push_back(u);
				this->idx.push_back(v);
				this->nidx = this->nidx + 1;

				return cupcfd::error::E_SUCCESS;
			}

			cupcfd::error::eCodes HDF5Properties::addIndex(unsigned long long x,
						  	  	  	  	  unsigned long long y,
										  unsigned long long z,
										  unsigned long long u,
										  unsigned long long v,
										  unsigned long long w)
			{
				this->idx.push_back(x);
				this->idx.push_back(y);
				this->idx.push_back(z);
				this->idx.push_back(u);
				this->idx.push_back(v);
				this->idx.push_back(w);
				this->nidx = this->nidx + 1;

				return cupcfd::error::E_SUCCESS;
			}

		}
	}
}
