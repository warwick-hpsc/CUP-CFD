/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the SparseMatrixSourceFileConfig class
 */

#include "SparseMatrixSourceFileConfig.h"
#include "SparseMatrixSourceHDF5.h"

namespace cupcfd
{
	namespace data_structures
	{
		template <class I, class T>
		SparseMatrixSourceFileConfig<I,T>::SparseMatrixSourceFileConfig(std::string& filePath, SparseMatrixFileFormat format)
		: SparseMatrixSourceConfig<I,T>(),
		  filePath(filePath),
		  format(format)
		{
			// Nothing to do beyond intialiser list currently.
		}

		template <class I, class T>
		SparseMatrixSourceFileConfig<I,T>::SparseMatrixSourceFileConfig(SparseMatrixSourceFileConfig<I,T>& source)
		{
			*this = source;
		}

		template <class I, class T>
		SparseMatrixSourceFileConfig<I,T>::~SparseMatrixSourceFileConfig()
		{
			// Nothing to do beyond intialiser list currently.
		}

		template <class I, class T>
		void SparseMatrixSourceFileConfig<I,T>::operator=(SparseMatrixSourceFileConfig<I,T>& source)
		{
			this->filePath = source.filePath;
			this->format = source.format;
		}

		template <class I, class T>
		SparseMatrixSourceFileConfig<I,T> * SparseMatrixSourceFileConfig<I,T>::clone()
		{
			return new SparseMatrixSourceFileConfig<I,T>(*this);
		}

		template <class I, class T>
		cupcfd::error::eCodes SparseMatrixSourceFileConfig<I,T>::buildSparseMatrixSource(SparseMatrixSource<I,T> ** matrixSource)
		{
			if(this->format == SPARSE_MATRIX_FILE_FORMAT_HDF5)
			{
				*matrixSource = new SparseMatrixSourceHDF5<I,T>(this->filePath);
				return cupcfd::error::E_SUCCESS;
			}

			// Shouldn't reach this part if we have a valid format
			return cupcfd::error::E_ERROR;
		}
	}
}

// Explicit Instantiation
using namespace cupcfd::data_structures;

template class cupcfd::data_structures::SparseMatrixSourceFileConfig<int, float>;
template class cupcfd::data_structures::SparseMatrixSourceFileConfig<int, double>;
