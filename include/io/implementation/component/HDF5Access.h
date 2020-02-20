/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Class Declarations for HDF5Access class.
 */

#ifndef CUPCFD_IO_HDF5_ACCESS_INCLUDE_H
#define CUPCFD_IO_HDF5_ACCESS_INCLUDE_H

#include "hdf5.h"
#include <string>
#include <vector>

#include "HDF5Record.h"
#include "HDF5Properties.h"
#include "Error.h"

namespace cupcfd
{
	namespace io
	{
		namespace hdf5
		{
			// ToDo: The HDF5 Interface could do with some restructuring - we don't really want
			// these circular dependencies
			class HDF5Record;
			class HDF5Properties;

			/**
			 * HDF5Access Class.
			 * This is the class responsible for tracking current access to a HDF5 file,
			 * open/active file, group, datasets etc.
			 * It also provides the access functions for retrieving data.
			 * Currently only handles reading data, not writing data.
			 */
			class HDF5Access
			{
				public:
					// Accessor Variables

					/** Active HDF5 File ID **/
					hid_t fileID;

					/** Active HDF5 Group ID **/
					hid_t groupID;

					/**  Active HDF5 Data Set ID **/
					hid_t datasetID;

					/** Active HDF5 Data Space ID **/
					hid_t dataspaceID;

					/** Active Memory Space (e.g. for index lookups) **/
					hid_t memspaceID;

					/** Active Attribute ID **/
					hid_t attrID;

					/** Name of open file **/
					std::string fileName;

					/**
					 * Data record for HDF5 Data being accessed from file.
					 * Contains record name, group name etc.
					 **/
					HDF5Record& record;

					// === Constructors/Deconstructors ===

					/**
					 * Constructor: Tracks access to specified file
					 * for specified data record.
					 *
					 * @param fileName File to access data from
					 * @param record Data record to access from file.
					 */
					HDF5Access(std::string fileName, HDF5Record& record);

					/**
					 * Deconstructor:
					 * Cleans up any open accessors, records etc.
					 */
					virtual ~HDF5Access();

					/**
					 * Open the HDF5 file tracked by this object.
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Operation was successful
					 */
					cupcfd::error::eCodes openFile();

					/**
					 * Close the HDF5 file tracked by this object.
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Operation was successful
					 */
					cupcfd::error::eCodes closeFile();

					// ToDo: These readData functions should also take in the size of the array,
					// so we can error track whether the arrays are large enough to hold the
					// requested data.

					/**
					 * Read all data stored in the tracked HDF5Record into an array - integer variant.
					 *
					 * Records that are 2D or greater will be flattened into a 1D array.
					 *
					 * @param sink The data array to store the data in.
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Operation was successful
					 */
					cupcfd::error::eCodes readData(int * sink);

					/**
					 * Read all data stored in the tracked HDF5Record into an array - float variant.
					 *
					 * Records that are 2D or greater will be flattened into a 1D array.
					 *
					 * @param sink The data array to store the data in.
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Operation was successful
					 */
					cupcfd::error::eCodes readData(float * sink);

					/**
					 * Read all data stored in the tracked HDF5Record into an array - double variant.
					 *
					 * Records that are 2D or greater will be flattened into a 1D array.
					 *
					 * @param sink The data array to store the data in.
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Operation was successful
					 */
					cupcfd::error::eCodes readData(double * sink);

					/**
					 * Read all data stored in the tracked HDF5Record into an array - integer variant.
					 *
					 * Records that are 2D or greater will be flattened into a 1D array.
					 *
					 * This function restricts the data retrieved from the record to those indexes
					 * specified in the properties object.
					 *
					 * @param sink The data array to store the data in.
					 * @param properties A properties object for the record, specifying which
					 * indexes to load
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Operation was successful
					 */
					cupcfd::error::eCodes readData(int * sink, HDF5Properties& properties);

					/**
					 * Read all data stored in the tracked HDF5Record into an array - float variant.
					 *
					 * Records that are 2D or greater will be flattened into a 1D array.
					 *
					 * This function restricts the data retrieved from the record to those indexes
					 * specified in the properties object.
					 *
					 * @param sink The data array to store the data in.
					 * @param properties A properties object for the record, specifying which
					 * indexes to load
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Operation was successful
					 */
					cupcfd::error::eCodes readData(float * sink, HDF5Properties& properties);

					/**
					 * Read all data stored in the tracked HDF5Record into an array - double variant.
					 *
					 * Records that are 2D or greater will be flattened into a 1D array.
					 *
					 * This function restricts the data retrieved from the record to those indexes
					 * specified in the properties object.
					 *
					 * @param sink The data array to store the data in.
					 * @param properties A properties object for the record, specifying which
					 * indexes to load
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Operation was successful
					 */
					cupcfd::error::eCodes readData(double * sink, HDF5Properties& properties);

					/**
					 * Read all data stored in the tracked HDF5Record into an array - integer variant.
					 *
					 * Records that are 2D or greater will be flattened into a 1D array.
					 *
					 * This function restricts the data retrieved from the record to those indexes
					 * specified in the properties object.
					 *
					 * This function also creates an appropriately sized array to store the data.
					 *
					 * @param sink A pointer to the location where a data array with the stored
					 * data will be created.
					 * @param properties A properties object for the record, specifying which
					 * indexes to load
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Operation was successful
					 */
					cupcfd::error::eCodes readData(int ** sink, HDF5Properties& properties);

					/**
					 * Read all data stored in the tracked HDF5Record into an array - float variant.
					 *
					 * Records that are 2D or greater will be flattened into a 1D array.
					 *
					 * This function restricts the data retrieved from the record to those indexes
					 * specified in the properties object.
					 *
					 * This function also creates an appropriately sized array to store the data.
					 *
					 * @param sink A pointer to the location where a data array with the stored
					 * data will be created.
					 * @param properties A properties object for the record, specifying which
					 * indexes to load
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Operation was successful
					 */
					cupcfd::error::eCodes readData(float ** sink, HDF5Properties& properties);

					/**
					 * Read all data stored in the tracked HDF5Record into an array - double variant.
					 *
					 * Records that are 2D or greater will be flattened into a 1D array.
					 *
					 * This function restricts the data retrieved from the record to those indexes
					 * specified in the properties object.
					 *
					 * This function also creates an appropriately sized array to store the data.
					 *
					 * @param sink A pointer to the location where a data array with the stored
					 * data will be created.
					 * @param properties A properties object for the record, specifying which
					 * indexes to load
					 *
					 * @return An error status indicating the success or failure of the operation
					 * @retval cupcfd::error::E_SUCCESS Operation was successful
					 */
					cupcfd::error::eCodes readData(double ** sink, HDF5Properties& properties);
			};
		}
	}
}

#endif
