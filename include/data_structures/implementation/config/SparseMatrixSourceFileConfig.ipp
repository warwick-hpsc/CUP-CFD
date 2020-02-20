/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains header level definitions for the SparseMatrixSourceFileConfig class
 */
 
#ifndef CUPCFD_DATA_STRUCTURES_SPARSE_MATRIX_SOURCE_FILE_CONFIG_IPP_H
#define CUPCFD_DATA_STRUCTURES_SPARSE_MATRIX_SOURCE_FILE_CONFIG_IPP_H

namespace cupcfd
{
	namespace data_structures
	{
		template <class I, class T>
		inline std::string SparseMatrixSourceFileConfig<I,T>::getFilePath()
		{
			return this->filePath;
		}
		
		template <class I, class T>
		inline void SparseMatrixSourceFileConfig<I,T>::setFilePath(std::string filePath)
		{
			this->filePath = filePath;
		}
		
		template <class I, class T>	
		inline SparseMatrixFileFormat SparseMatrixSourceFileConfig<I,T>::getFormat()
		{
			return this->format;
		}
		
		template <class I, class T>	
		inline void SparseMatrixSourceFileConfig<I,T>::setFormat(SparseMatrixFileFormat format)
		{
			this->format = format;
		}
	}
}

#endif
