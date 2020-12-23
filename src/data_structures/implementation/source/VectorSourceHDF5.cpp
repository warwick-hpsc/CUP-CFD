/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the VectorSourceHDF5 class
 */

// Class Declarations
#include "VectorSourceHDF5.h"

// C++ Library Function/Objects
#include <vector>

// HDF5 I/O Access
#include "HDF5Record.h"
#include "HDF5Access.h"

#include <iostream>

namespace cupcfd
{
	namespace data_structures
	{
		// This HDF5 file is formatted as follows:
		// Attributes:
		// Group | Name | Description
		// "/"	 | "size" | The number of vector values stored in this file

		// DataSets:
		// Group | Name | Description
		// "/"	 | "vector" | The array of vector values

		template <class I, class T>
		VectorSourceHDF5<I, T>::VectorSourceHDF5(std::string fileName)
		: VectorSource<I,T>(),
		  fileName(fileName)
		{

		}

		template <class I, class T>
		VectorSourceHDF5<I, T>::~VectorSourceHDF5() {
			// Currently Nothing to Clean Up.
		}

		template <class I, class T>
		cupcfd::error::eCodes VectorSourceHDF5<I,T>::getSize(I * vecSize) {
			cupcfd::error::eCodes status;

			// This format will store it's attributes at the root level
			cupcfd::io::hdf5::HDF5Record record("/", "size",true);
			cupcfd::io::hdf5::HDF5Access access(this->fileName, record);
			status = access.readData(vecSize);
			CHECK_ECODE(status)

			return cupcfd::error::E_SUCCESS;
		}

		template <class I, class T>
		cupcfd::error::eCodes VectorSourceHDF5<I,T>::getData(T * data, I nData, I * indexes, I nIndexes, I indexBase) {
			cupcfd::error::eCodes status;

			if(nData != nIndexes) {
				return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
			}

			// Setup Access
			cupcfd::io::hdf5::HDF5Record record("/", "vector" , false);
			cupcfd::io::hdf5::HDF5Access access(this->fileName, record);
			cupcfd::io::hdf5::HDF5Properties properties(access);

			// Compute the sizes from the range difference
			for(I pos = 0; pos < nIndexes; pos++) {
				status = properties.addIndex(indexes[pos] - indexBase);
				CHECK_ECODE(status)
			}

			// Read Data
			status = access.readData(data, properties);
			CHECK_ECODE(status)

			return cupcfd::error::E_SUCCESS;
		}
	}
}

// Explicit Instantiation
template class cupcfd::data_structures::VectorSourceHDF5<int, float>;
template class cupcfd::data_structures::VectorSourceHDF5<int, double>;

