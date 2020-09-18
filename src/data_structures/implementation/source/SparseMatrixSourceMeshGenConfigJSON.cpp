/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the SparseMatrixSourceMeshGenConfigJSON class
 */

#include "SparseMatrixSourceMeshGenConfigJSON.h"
#include <fstream>

namespace cupcfd
{
	namespace data_structures
	{
		template <class I, class T>
		SparseMatrixSourceMeshGenConfigJSON<I,T>::SparseMatrixSourceMeshGenConfigJSON(std::string configFilePath, std::string * topLevel, int nTopLevel)
		{
			// Copy the top level strings
			for(int i = 0; i < nTopLevel; i++)
			{
				this->topLevel.push_back(topLevel[i]);
			}

			this->topLevel.push_back("SparseMatrixMeshGen");

			std::ifstream source(configFilePath, std::ifstream::binary);
			source >> this->configData;

			for(int i = 0; i < this->topLevel.size(); i++)
			{
				this->configData = this->configData[this->topLevel[i]];
			}
		}

		template <class I, class T>
		SparseMatrixSourceMeshGenConfigJSON<I,T>::SparseMatrixSourceMeshGenConfigJSON(const SparseMatrixSourceMeshGenConfigJSON<I,T>& source)
		{
			*this = source;
		}

		template <class I, class T>
		SparseMatrixSourceMeshGenConfigJSON<I,T>::~SparseMatrixSourceMeshGenConfigJSON()
		{

		}

		template <class I, class T>
		void SparseMatrixSourceMeshGenConfigJSON<I,T>::operator=(const SparseMatrixSourceMeshGenConfigJSON<I,T>& source)
		{
			this->topLevel = source.topLevel;
			this->configData = source.configData;
		}

		template <class I, class T>
		SparseMatrixSourceMeshGenConfigJSON<I,T> * SparseMatrixSourceMeshGenConfigJSON<I,T>::clone()
		{
			return new SparseMatrixSourceMeshGenConfigJSON(*this);
		}

		template <class I, class T>
		cupcfd::error::eCodes SparseMatrixSourceMeshGenConfigJSON<I,T>::buildSparseMatrixSourceConfig(SparseMatrixSourceConfig<I,T> ** matrixSourceConfig)
		{
			// ToDo
			return cupcfd::error::E_ERROR;
		}
	}
}

// Explicit Instantiation
template class cupcfd::data_structures::SparseMatrixSourceMeshGenConfigJSON<int, float>;
template class cupcfd::data_structures::SparseMatrixSourceMeshGenConfigJSON<int, double>;
