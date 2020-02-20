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

#include "HDF5Record.h"
#include <stdexcept>

using namespace cupcfd::io::hdf5;

namespace cupcfd
{
	namespace io
	{
		namespace hdf5
		{
			HDF5Record::HDF5Record(std::string groupName, std::string dataName, bool attr)
			{
				this->groupName = groupName;
				this->dataName = dataName;
				this->attr = attr;
			}

			HDF5Record::HDF5Record(std::string dataName, bool attr)
			{
				this->dataName = dataName;
				this->attr = attr;
			}

			HDF5Record::~HDF5Record()
			{

			}

			cupcfd::error::eCodes HDF5Record::openGroup(HDF5Access& access)
			{
				hid_t groupID;

				// Close if already open
				// ToDo - Should close other containers below this
				if(access.groupID > 0)
				{
					closeGroup(access);
				}

				// HDF5 C Interface - Open Group
				if(this->groupName != "")
				{
					// Store it in a separate variable initially in case there is an error
					groupID = H5Gopen(access.fileID, this->groupName.c_str(), H5P_DEFAULT);
				}

				if(groupID < 0)
				{
					throw( std::invalid_argument("HDF5Interface: openGroup: Unable to Open Group " + this->groupName));
				}

				access.groupID = groupID;

				return cupcfd::error::E_SUCCESS;
			}

			cupcfd::error::eCodes HDF5Record::closeGroup(HDF5Access& access)
			{
				hid_t err;

				if(access.groupID < 0)
				{
					throw( std::invalid_argument("HDF5Interface: closeGroup: Invalid groupID - must be greater than zero."));
				}

				// HDF5 C Interface - Close Group
				if(access.groupID > 0)
				{
					err = H5Gclose(access.groupID);
				}

				if(err < 0)
				{
					// Error
					throw( std::invalid_argument("HDF5Interface: closeGroup: HDF5 unable to close group with ID " + std::to_string(access.groupID)));
				}

				return cupcfd::error::E_SUCCESS;
			}

			cupcfd::error::eCodes HDF5Record::openDataSet(HDF5Access& access)
			{
				hid_t datasetID;
				hid_t pathID;

				if(this->attr == false)
				{
					// Close if already open
					// ToDo - Should close other containers below this
					if(access.datasetID > 0)
					{
						closeDataSet(access);
					}

					if(access.groupID > 0)
					{
						pathID = access.groupID;
					}
					else if(access.fileID > 0)
					{
						pathID = access.fileID;
					}
					else
					{
						// Error
						int a = 1;
					}

					datasetID = H5Dopen(pathID, this->dataName.c_str(), H5P_DEFAULT);

					if(datasetID < 0)
					{
						throw(std::invalid_argument("HDF5Interface: openDataSet: Unable to Open Data Set " + this->dataName));
					}
					else
					{
						access.datasetID = datasetID;
					}
				}

				return cupcfd::error::E_SUCCESS;
			}

			cupcfd::error::eCodes HDF5Record::closeDataSet(HDF5Access& access)
			{
				hid_t err;

				if(access.datasetID > 0)
				{
					err = H5Dclose(access.datasetID);
				}

				if(err < 0)
				{
					// Error
					int a = 1;
				}

				return cupcfd::error::E_SUCCESS;
			}

			cupcfd::error::eCodes HDF5Record::openDataSpace(HDF5Access& access)
			{
				hid_t dataspaceID;

				if(this->attr == false)
				{
					// Opening a Dataspace for a Dataset
					if(access.datasetID > 0)
					{
						dataspaceID = H5Dget_space(access.datasetID);
					}
				}
				else
				{
					if(access.attrID > 0)
					{
						dataspaceID = H5Aget_space(access.attrID);
					}
				}

				if(dataspaceID < 0)
				{
					// Error
					int a = 1;
				}
				else
				{
					access.dataspaceID = dataspaceID;
				}

				return cupcfd::error::E_SUCCESS;
			}

			cupcfd::error::eCodes HDF5Record::closeDataSpace(HDF5Access& access)
			{
				hid_t err;

				if(access.dataspaceID > 0)
				{
					err = H5Sclose(access.dataspaceID);
				}

				if(err < 0)
				{
					// Error
					int a = 1;
				}

				return cupcfd::error::E_SUCCESS;
			}

			cupcfd::error::eCodes HDF5Record::openAttribute(HDF5Access& access)
			{
				hid_t attrID;
				hid_t pathID;

				if(this->attr == true)
				{
					if(access.groupID > 0)
					{
						pathID = access.groupID;
					}
					else if(access.fileID > 0)
					{
						pathID = access.fileID;
					}
					else
					{
						// Error
						int a = 1;
					}

					attrID = H5Aopen(pathID, this->dataName.c_str(), H5P_DEFAULT);

					if(attrID < 0)
					{
						// Error
						int a = 1;
					}
					else
					{
						access.attrID = attrID;
					}
				}

				return cupcfd::error::E_SUCCESS;
			}

			cupcfd::error::eCodes HDF5Record::closeAttribute(HDF5Access& access)
			{
				hid_t err;

				if(access.attrID > 0)
				{
					err = H5Aclose(access.attrID);
				}

				if(err < 0)
				{
					// Error
					int a = 1;
				}

				return cupcfd::error::E_SUCCESS;
			}
		}
	}
}
