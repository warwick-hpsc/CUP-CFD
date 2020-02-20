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

#define BOOST_TEST_MODULE HDF5Interface
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>
#include <string>
#include <iostream>

#include "HDF5Access.h"
#include "HDF5Properties.h"
#include "HDF5Record.h"

// Note: Three separate classes are tested here, since the current design has a bit
// of an interdependence between them. It would be good to resolve this at some point

using namespace cupcfd::io::hdf5;

// === Constructors ===
// Test 1: Test HDF5Record Constructor
BOOST_AUTO_TEST_CASE(constructor_test1)
{

}

// Test 2: Test HDF5Access Constructor
BOOST_AUTO_TEST_CASE(constructor_test2)
{

}

// Test 2: Test HDF5Properties Constructor
BOOST_AUTO_TEST_CASE(constructor_test3)
{

}

// === Deconstructors ===
// Test 1: Test that all HDF5 open objects are closed on exit

// == HDF5Access - OpenFile ===
// Test 1: Test an existing file opens without error
BOOST_AUTO_TEST_CASE(openFile_test1)
{
	HDF5Record record("/TestGroup1", "TestDataSetInt1D", false);
	HDF5Access access("../tests/io/data/TestReadSource.h5", record);

	access.openFile();
}

// Test 2: Test an missing file raises an error code
BOOST_AUTO_TEST_CASE(openFile_test2)
{

}

// Test 3: Test that if a file is already open an error is raised
BOOST_AUTO_TEST_CASE(openFile_test3)
{

}

// == HDF5Access - CloseFile ===
// Test 1: Test an open file closes without error
BOOST_AUTO_TEST_CASE(closeFile_test1)
{

}

// Test 2: Test closing without opening a file raises an error
BOOST_AUTO_TEST_CASE(closeFile_test2)
{

}

// === HDF5Record - openGroup ===
// Test 1: Test an existing group can be opened without error
BOOST_AUTO_TEST_CASE(openGroup_test1)
{

}

// Test 2: Test a group under another group can be opened without error
BOOST_AUTO_TEST_CASE(openGroup_test2)
{

}

// Test 3: Test that a non-existant group raises an error
BOOST_AUTO_TEST_CASE(openGroup_test3)
{

}

// Test 4: Test that if a group is already open an error is raised
BOOST_AUTO_TEST_CASE(openGroup_test4)
{

}

// === HDF5Record - closeGroup ===
// Test 1: Test that a open group is successfully closed
BOOST_AUTO_TEST_CASE(closeGroup_test1)
{

}

// Test 2: Test that if a group is not open a suitable error is raised
BOOST_AUTO_TEST_CASE(closeGroup_test2)
{

}

// === HDF5Record - openDataSet ===
// Test 1: Test that a groups dataset can be opened without error
BOOST_AUTO_TEST_CASE(openDataSet_test1)
{

}

// Test 2: Test that if a group is not open an error is raised
BOOST_AUTO_TEST_CASE(openDataSet_test2)
{

}

// === HDF5Record - closeDataSet ===
// Test 1: Test that an open dataset can be closed without error
BOOST_AUTO_TEST_CASE(closeDataSet_test1)
{

}

// Test 2: Test that if a dataset is not open an error is raised
BOOST_AUTO_TEST_CASE(closeDataSet_test2)
{

}

// === HDF5Record - openDataSpace ===
// Test 1: Test that a dataset's space can be opened without error
BOOST_AUTO_TEST_CASE(openDataSpace_test1)
{

}

// Test 2: Test that if a dataspace is not open an error is raised
BOOST_AUTO_TEST_CASE(openDataSpace_test2)
{

}




// === HDF5Record - closeDataSpace ===

// === HDF5Record - openAttribute ===
// Test 1: Test that an non-existant attribute raises an error

// Test 2: Test that opening a single int attribute is successful

// Test 3: Test that opening a single float attribute is successful

// Test 4: Test that opening a single double attribute is successful


// === HDF5Record - closeAttribute ===

// === HDF5Properties - addIndex (Various) ===


// == HDF5Access - readData int* sink ===
// Tests reading from various data sources/types into a integer array
// May cause truncation (but otherwise from correct indexes)

// Test 1: Read entire 1D integer array from a 1D integer dataset
BOOST_AUTO_TEST_CASE(readData_full_test1)
{
	HDF5Record record("/TestGroup1", "TestDataSetInt1D", false);
	HDF5Access access("../tests/io/data/TestReadSource.h5", record);

	int result[20];

	access.readData(result);

	int cmp[20] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 ,13, 14, 15, 16, 17, 18, 19};

	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 20, cmp, cmp + 20);
}


// Test 2: Read entire 2D integer array from a 2D integer dataset
BOOST_AUTO_TEST_CASE(readData_full_test2)
{
	HDF5Record record("/TestGroup1", "TestDataSetInt2D", false);
	HDF5Access access("../tests/io/data/TestReadSource.h5", record);

	int result[80];

	access.readData(result);

	int cmp[80] = {100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
				   111, 112, 113, 114, 115, 116, 117, 118, 119, 120,
				   121, 122, 123, 124, 125, 126, 127, 128, 129, 130,
				   131, 132, 133, 134, 135, 136, 137, 138, 139, 140,
				   141, 142, 143, 144, 145, 146, 147, 148, 149, 150,
				   151, 152, 153, 154, 155, 156, 157, 158, 159, 160,
				   161, 162, 163, 164, 165, 166, 167, 168, 169, 170,
				   171, 172, 173, 174, 175, 176, 177, 178, 179};

	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 80, cmp, cmp + 80);
}


// Test 3: Read entire 1D integer array from a 1D float dataset (while identifying loss of precision)
BOOST_AUTO_TEST_CASE(readData_full_test3)
{
	HDF5Record record("/TestGroup1", "TestDataSetFloat1D", false);
	HDF5Access access("../tests/io/data/TestReadSource.h5", record);

	int result[15];

	access.readData(result);

	// Data will be truncated
	int cmp[15] = {200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214};

	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 15, cmp, cmp + 15);
}


// Test 4: Read entire 2D integer array from a 2D float dataset (while identifying loss of precision)
BOOST_AUTO_TEST_CASE(readData_full_test4)
{
	HDF5Record record("/TestGroup1", "TestDataSetFloat2D", false);
	HDF5Access access("../tests/io/data/TestReadSource.h5", record);

	int result[24];

	access.readData(result);

	// Data will be truncated
	int cmp[24] = {301, 302, 303, 304, 305, 306, 307, 308, 309, 310,311, 312,
				   313, 314, 315, 316, 317, 318, 319, 320, 321, 322, 323, 324};

	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 24, cmp, cmp + 24);
}


// Test 5: Read entire 1D integer array from a 1D double dataset (while identifying loss of precision)
BOOST_AUTO_TEST_CASE(readData_full_test5)
{
	HDF5Record record("/TestGroup1", "TestDataSetDouble1D", false);
	HDF5Access access("../tests/io/data/TestReadSource.h5", record);

	int result[11];

	access.readData(result);

	// Data will be truncated
	int cmp[11] = {400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410};

	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 11, cmp, cmp + 11);
}

// Test 6: Read entire 2D integer array from a 2D double dataset (while identifying loss of precision)
BOOST_AUTO_TEST_CASE(readData_full_test6)
{
	HDF5Record record("/TestGroup1", "TestDataSetDouble2D", false);
	HDF5Access access("../tests/io/data/TestReadSource.h5", record);

	int result[14];

	access.readData(result);

	// Data will be truncated
	int cmp[14] = {500, 501, 502, 503, 504, 505, 506, 507, 508, 509, 510, 511, 512, 513};

	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 14, cmp, cmp + 14);
}


// == HDF5Access - readData float* sink ===
// Tests reading from various data sources/types into a float array
// May cause truncation (but otherwise from correct indexes)

// Test 7: Read 1D float array from entire dataset in a 1D integer dataset

// Test 8: Read 2D float array from  entire dataset in a 2D integer dataset

// Test 9: Read 1D float array from  entire dataset in a 1D float dataset (while identifying loss of precision)

// Test 10: Read 2D float array from  entire dataset in a 2D float dataset (while identifying loss of precision)

// Test 11: Read 1D float array from  entire dataset in a 1D double dataset (while identifying loss of precision)

// Test 12: Read 2D float array from  entire dataset in a 2D double dataset (while identifying loss of precision)


// === HDF5Access - readData double* sink ===
// Tests reading from various data sources/types into a double array
// May cause truncation (but otherwise from correct indexes)


// Test 13: Read 1D double array from indexes in a 1D integer dataset

// Test 14: Read 2D double array from indexes in a 2D integer dataset

// Test 15: Read 1D double array from indexes in a 1D float dataset (while identifying loss of precision)

// Test 16: Read 2D double array from indexes in a 2D float dataset (while identifying loss of precision)

// Test 17: Read 1D double array from indexes in a 1D double dataset (while identifying loss of precision)

// Test 18: Read 2D double array from indexes in a 2D double dataset (while identifying loss of precision)
BOOST_AUTO_TEST_CASE(readData_full_test18)
{
	HDF5Record record("/TestGroup1", "TestDataSetDouble2D", false);
	HDF5Access access("../tests/io/data/TestReadSource.h5", record);

	double result[14];

	access.readData(result);

	// Data will be truncated
	double cmp[14] = {500.32, 501.523, 502.626, 503.6236, 504.10562, 505.3215,
					  506.9834, 507.6783, 508.2316, 509.32161, 510.624562,
					  511.6134, 512.671, 513.1613};

	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 14, cmp, cmp + 14);
}

// === HDF5Access - readData int* sink + properties ===

// Test 1: Read 1D integer array from indexes in a 1D integer dataset
BOOST_AUTO_TEST_CASE(readData_indexed_test1)
{
	HDF5Record record("/TestGroup1", "TestDataSetInt1D", false);
	HDF5Access access("../tests/io/data/TestReadSource.h5", record);
	HDF5Properties properties(access);

	unsigned long long indexes[6] = {0, 4, 9, 13, 17, 19};

	for(int i = 0; i < 6; i++)
	{
		properties.addIndex(indexes[i]);
	}

	int result[6];

	access.readData(result, properties);

	int cmp[6] = {0, 4, 9, 13, 17, 19};

	BOOST_CHECK_EQUAL_COLLECTIONS(result, result + 6, cmp, cmp + 6);
}

// Test 2: Read 2D integer array from indexes in a 2D integer dataset
BOOST_AUTO_TEST_CASE(readData_indexed_test2)
{

}

// Test 3: Read 1D integer array from indexes in a 1D float dataset (while identifying loss of precision)

// Test 4: Read 2D integer array from indexes in a 2D float dataset (while identifying loss of precision)

// Test 5: Read 1D integer array from indexes in a 1D double dataset (while identifying loss of precision)

// Test 6: Read 2D integer array from indexes in a 2D double dataset (while identifying loss of precision)

// === HDF5Access - readData float* sink + properties ===

// Test 7: Read 1D float array from indexes in a 1D integer dataset

// Test 8: Read 2D float array from indexes in a 2D integer dataset

// Test 9: Read 1D float array from indexes in a 1D float dataset (while identifying loss of precision)

// Test 10: Read 2D float array from indexes in a 2D float dataset (while identifying loss of precision)

// Test 11: Read 1D float array from indexes in a 1D double dataset (while identifying loss of precision)

// Test 12: Read 2D float array from indexes in a 2D double dataset (while identifying loss of precision)


// === HDF5Access - readData double* sink + properties ===

// Test 13: Read 1D double array from indexes in a 1D integer dataset

// Test 14: Read 2D double array from indexes in a 2D integer dataset

// Test 15: Read 1D double array from indexes in a 1D float dataset (while identifying loss of precision)

// Test 16: Read 2D double array from indexes in a 2D float dataset (while identifying loss of precision)

// Test 17: Read 1D double array from indexes in a 1D double dataset (while identifying loss of precision)

// Test 18: Read 2D double array from indexes in a 2D double dataset (while identifying loss of precision)


// === HDF5Access - readData int** sink + properties ===
// Test reading from various data sources/types into a integer array
// using indexes to access specific locations in the record.
// May cause truncation (but otherwise from correct indexes)
// Also creates the array.

// Test 1: Read 1D integer array from indexes in a 1D integer dataset

// Test 2: Read 2D integer array from indexes in a 2D integer dataset

// Test 3: Read 1D integer array from indexes in a 1D float dataset (while identifying loss of precision)

// Test 4: Read 2D integer array from indexes in a 2D float dataset (while identifying loss of precision)

// Test 5: Read 1D integer array from indexes in a 1D double dataset (while identifying loss of precision)

// Test 6: Read 2D integer array from indexes in a 2D double dataset (while identifying loss of precision)

// === HDF5Access - readData float** sink + properties ===

// Test 7: Read 1D float array from indexes in a 1D integer dataset

// Test 8: Read 2D float array from indexes in a 2D integer dataset

// Test 9: Read 1D float array from indexes in a 1D float dataset (while identifying loss of precision)

// Test 10: Read 2D float array from indexes in a 2D float dataset (while identifying loss of precision)

// Test 11: Read 1D float array from indexes in a 1D double dataset (while identifying loss of precision)

// Test 12: Read 2D float array from indexes in a 2D double dataset (while identifying loss of precision)

// === HDF5Access - readData double** sink + properties ===

// Test 13: Read 1D double array from indexes in a 1D integer dataset

// Test 14: Read 2D double array from indexes in a 2D integer dataset

// Test 15: Read 1D double array from indexes in a 1D float dataset (while identifying loss of precision)

// Test 16: Read 2D double array from indexes in a 2D float dataset (while identifying loss of precision)

// Test 17: Read 1D double array from indexes in a 1D double dataset (while identifying loss of precision)

// Test 18: Read 2D double array from indexes in a 2D double dataset (while identifying loss of precision)
