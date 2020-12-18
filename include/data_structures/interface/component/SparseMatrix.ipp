/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains the header level definitions for the SparseMatrix class
 *
 */

#ifndef CUPCFD_DATA_STRUCTURES_SPARSEMATRIX_IPP_H
#define CUPCFD_DATA_STRUCTURES_SPARSEMATRIX_IPP_H

namespace cupcfd
{
	namespace data_structures
	{
		// === Constructors/Deconstructors ===

		template <class C, class I, class T>
		SparseMatrix<C,I,T>::SparseMatrix()
		: m(1), n(1), baseIndex(0), nnz(0)
		{
			// Nothing else to add
		}

		template <class C, class I, class T>
		SparseMatrix<C,I,T>::SparseMatrix(I m, I n)
		: m(m), n(n), baseIndex(0), nnz(0)
		{
			// Nothing else to add
		}

		template <class C, class I, class T>
		SparseMatrix<C,I,T>::SparseMatrix(I m, I n, I baseIndex)
		: m(m), n(n), baseIndex(baseIndex), nnz(0)
		{
			// Nothing else to add
		}

		template <class C, class I, class T>
		SparseMatrix<C,I,T>::~SparseMatrix()
		{
			// Nothing to do for now
		}

		// === Concrete Methods ===

		template <class C, class I, class T>
		inline cupcfd::error::eCodes SparseMatrix<C,I,T>::resize(I rows, I columns)
		{
			return static_cast<C*>(this)->resize(rows, columns);
		}
		
		template <class C, class I, class T>
		inline cupcfd::error::eCodes SparseMatrix<C,I,T>::clear()
		{
			return static_cast<C*>(this)->clear();
		}
		
		template <class C, class I, class T>
		inline cupcfd::error::eCodes SparseMatrix<C,I,T>::setElement(I row, I col, T val)
		{
			return static_cast<C*>(this)->setElement(row, col, val);
		}
		
		template <class C, class I, class T>
		inline cupcfd::error::eCodes SparseMatrix<C,I,T>::getElement(I row, I col, T * val)
		{
			return static_cast<C*>(this)->getElement(row, col, val);
		}

		template <class C, class I, class T>
		inline cupcfd::error::eCodes SparseMatrix<C,I,T>::getNonZeroRowIndexes(I ** rowIndexes, I * nRowIndexes)
		{
			return static_cast<C*>(this)->getNonZeroRowIndexes(rowIndexes, nRowIndexes);
		}

		template <class C, class I, class T>
		inline cupcfd::error::eCodes SparseMatrix<C,I,T>::getRowColumnIndexes(I rowIndex, I ** columnIndexes, I * nColumnIndexes)
		{
			return static_cast<C*>(this)->getRowColumnIndexes(rowIndex, columnIndexes, nColumnIndexes);
		}

		template <class C, class I, class T>
		inline cupcfd::error::eCodes SparseMatrix<C,I,T>::getRowNNZValues(I rowIndex, T ** nnzValues, I * nNNZValues)
		{
			return static_cast<C*>(this)->getRowNNZValues(rowIndex, nnzValues, nNNZValues);
		}
	}
}

#endif
