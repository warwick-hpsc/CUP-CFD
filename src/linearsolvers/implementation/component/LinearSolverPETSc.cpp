/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * This file contains the implementation of concrete
 * functions for the LinearSolver class
 */

#include "LinearSolverPETSc.h"

// SparseMatrix Implementation Classes
#include "SparseMatrixCSR.h"
#include "SparseMatrixCOO.h"

// Gather Operations
#include "Gather.h"

#include "DistributedAdjacencyList.h"

#include "Reduce.h"
#include "ExchangePatternTwoSidedNonBlocking.h"

#include <cstdlib>
#include <iostream>

namespace cupcfd
{
	namespace linearsolvers
	{
		// === Constructors/Deconstructors ===

		template <class C, class I, class T>
		LinearSolverPETSc<C,I,T>::LinearSolverPETSc(cupcfd::comm::Communicator& comm, PETScAlgorithm algorithm, T rTol, T eTol, cupcfd::data_structures::SparseMatrix<C,I,T>& matrix)
		:LinearSolverInterface<C,I,T>(comm, matrix.m, matrix.n)
		{
			cupcfd::error::eCodes status;

			// Set Petsc Objects to PETSC_NULL so we can track whether they
			// are initialised or not.

			// These have to be done here rather than in the reset method,
			// since the reset methods check whether these are NULL, and if
			// left undefined they will be in an unknown state.
			this->a = PETSC_NULL;
			this->x = PETSC_NULL;
			this->b = PETSC_NULL;

			this->bRanges = nullptr;
			this->xRanges = nullptr;
			this->aRanges = nullptr;

			status = this->setupVectorX();
			HARD_CHECK_ECODE(status)
			status = this->setupVectorB();
			HARD_CHECK_ECODE(status)
			status = this->setupMatrixA(matrix);
			HARD_CHECK_ECODE(status)

			this->algSolver = new LinearSolverPETScAlgorithm(comm, algorithm, rTol, eTol);
		}

		template <class C, class I, class T>
		LinearSolverPETSc<C,I,T>::~LinearSolverPETSc()
		{
			delete this->algSolver;
		}

		// === Overloaded Inherited Methods ===

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverPETSc<C,I,T>::resetVectorX() {
			if(this->x != PETSC_NULL) {
				VecDestroy(&(this->x));
				this->x = PETSC_NULL;
			}

			if(this->xRanges != nullptr) {
				// Note: it seems like xRanges is poIing at a PETSc Iernal array
				// Once the vector is destroyed, it looses values, so presume
				// that PETSc has freed it, in which case we just set this to nullptr.
				this->xRanges = nullptr;
			}

			return cupcfd::error::E_SUCCESS;
		}


		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverPETSc<C,I,T>::resetVectorB() {
			if(this->b != PETSC_NULL) {
				VecDestroy(&(this->b));
				this->b = PETSC_NULL;
			}

			if(this->bRanges != nullptr) {
				// Note: it seems like xRanges is pointing at a PETSc Internal array
				// Once the vector is destroyed, it looses values, so presume
				// that PETSc has freed it, in which case we just set this to nullptr.
				this->bRanges = nullptr;
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverPETSc<C,I,T>::resetMatrixA() {
			if(this->a != PETSC_NULL) {
				MatDestroy(&(this->a));
				this->a = PETSC_NULL;
			}

			if(this->aRanges != nullptr) {
				// Note: Presuming PETSc frees this since it is a const so may be
				// poIing Iernally? For the vectors it seems to lose values
				// once the vector is destroyed, so likely same for Matrix
				// so we won't free here (assumign PETSc handles it)
				// otherwise this would be a memory leak.
				this->aRanges = nullptr;
			}

			return cupcfd::error::E_SUCCESS;
		}


		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverPETSc<C,I,T>::setValuesVectorX(T scalar) {
			// Error Check that the vector has been setup
			if(this->x == PETSC_NULL) {
				return cupcfd::error::E_LINEARSOLVER_INVALID_VECTOR;
			}

			// Set the value of the x vector in the PETSc object
			PetscScalar val = scalar;

			// In PETSc, this is logically collective (since it has to be updated on all ranks for parallel vectors)
			VecSet(this->x, val);

			// Begin/End Assembly to ensure the vector is updated. This makes this a collective operation.
			VecAssemblyBegin(this->x);
			VecAssemblyEnd(this->x);

			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverPETSc<C,I,T>::setValuesVectorX(T * scalars, I nScalars,
																I * indexes, I nIndexes, I indexBase) {
			// Error Check - Are nScalars and nIndexes the same
			if(nScalars != nIndexes) {
				return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
			}

			// Error Check - Has the Vector X been setup
			if(this->x == PETSC_NULL) {
				return cupcfd::error::E_LINEARSOLVER_INVALID_VECTOR;
			}

			// Error Check - Are all the indexes within the bounds of the vector scheme?
			I * shiftedIndexes = (I *) malloc(sizeof(I) * nIndexes);

			for(I i = 0; i < nIndexes; i++) {
				I indexOffset = (indexes[i] - indexBase);

				if((indexes[i] - indexBase) >= this->mGlobal) {
					// Cleanup
					free(shiftedIndexes);
					return cupcfd::error::E_INVALID_INDEX;
				}

				if((indexes[i] - indexBase) < 0) {
					// Cleanup
					free(shiftedIndexes);
					return cupcfd::error::E_INVALID_INDEX;
				}

				// Copy the index across to a zero-shifted version
				// ToDo: If the base is 0, we don't need to make a local copy.
				// However, we have to loop over the array anyway for the error check....
				shiftedIndexes[i] = indexOffset;
			}

			// Petsc always uses zero based indexes for it's index scheme.
			// We can correct for this by offsetting using the indexBase.

			if(indexBase == 0) {
				// Convert to PETScScalar Type (Means choice of datatype for class doesn't matter as much)
				PetscScalar * tmp = (PetscScalar *) malloc(sizeof(PetscScalar) * nScalars);
				for(I i = 0; i < nScalars; i++) {
					tmp[i] = PetscScalar(scalars[i]);
				}

				// We don't need to shift the indexes, we can use them directly
				VecSetValues(this->x, nIndexes, shiftedIndexes, tmp, INSERT_VALUES);

				// Need to reassemble vector due to updates
				VecAssemblyBegin(this->x);
				VecAssemblyEnd(this->x);

				free(tmp);
			}

			// Cleanup
			free(shiftedIndexes);

			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverPETSc<C,I,T>::setValuesVectorB(T scalar) {
			// Error Check that the vector has been setup
			if(this->b == PETSC_NULL) {
				return cupcfd::error::E_LINEARSOLVER_INVALID_VECTOR;
			}

			// Set the value of the x vector in the PETSc object
			PetscScalar val = scalar;

			// In PETSc, this is logically collective (since it has to be updated on all ranks for parallel vectors)
			VecSet(this->b, val);

			// Begin/End Assembly to ensure the vector is updated. This makes this a collective operation.
			VecAssemblyBegin(this->b);
			VecAssemblyEnd(this->b);

			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverPETSc<C,I,T>::setValuesVectorB(T * scalars, I nScalars,
																I * indexes, I nIndexes, I indexBase) {
			// Error Check - Are nScalars and nIndexes the same
			if(nScalars != nIndexes) {
				return cupcfd::error::E_ARRAY_MISMATCH_SIZE;
			}

			// Error Check - Check the value of mGlobal is greater than 0
			if(this->mGlobal <= 0) {
				return cupcfd::error::E_LINEARSOLVER_ROW_SIZE_UNSET;
			}

			// Error Check - Has the Vector X been setup
			if(this->b == PETSC_NULL) {
				return cupcfd::error::E_LINEARSOLVER_INVALID_VECTOR;
			}

			// Error Check - Are all the indexes within the bounds of the vector scheme?
			I * shiftedIndexes = (I *) malloc(sizeof(I) * nIndexes);

			for(I i = 0; i < nIndexes; i++) {
				I indexOffset = (indexes[i] - indexBase);

				if((indexes[i] - indexBase) >= this->mGlobal) {
					// Cleanup
					free(shiftedIndexes);
					return cupcfd::error::E_INVALID_INDEX;
				}

				if((indexes[i] - indexBase) < 0) {
					// Cleanup
					free(shiftedIndexes);
					return cupcfd::error::E_INVALID_INDEX;
				}

				// Copy the index across to a zero-shifted version
				// ToDo: If the base is 0, we don't need to make a local copy.
				// However, we have to loop over the array anyway for the error check....
				shiftedIndexes[i] = indexOffset;
			}

			// Petsc always uses zero based indexes for it's index scheme.
			// We can correct for this by offsetting using the indexBase.

			if(indexBase == 0) {
				PetscScalar * tmp = (PetscScalar *) malloc(sizeof(PetscScalar) * nScalars);
				for(I i = 0; i < nScalars; i++) {
					tmp[i] = PetscScalar(scalars[i]);
				}

				// We don't need to shift the indexes, we can use them directly
				VecSetValues(this->b, nIndexes, shiftedIndexes, tmp, INSERT_VALUES);

				// Need to reassemble vector due to updates
				VecAssemblyBegin(this->b);
				VecAssemblyEnd(this->b);

				free(tmp);
			}

			// Cleanup
			free(shiftedIndexes);

			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverPETSc<C,I,T>::clearVectorX() {
			cupcfd::error::eCodes status;

			// Since we use a template, typecast 0 to the relevant type
			status = setValuesVectorX((T) 0);
			CHECK_ECODE(status)

			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverPETSc<C,I,T>::clearVectorB() {
			cupcfd::error::eCodes status;

			// Since we use a template, typecase 0 to the relevant type
			status = setValuesVectorB((T) 0);
			CHECK_ECODE(status)

			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverPETSc<C,I,T>::reset() {
			cupcfd::error::eCodes status;

			// Reset Vector X
			status = this->resetVectorX();
			CHECK_ECODE(status)

			// Reset Vector B
			status = this->resetVectorB();
			CHECK_ECODE(status)

			// Reset Matrix A
			status = this->resetMatrixA();
			CHECK_ECODE(status)

			return cupcfd::error::E_SUCCESS;
		}

		// === Pure Virtual Inherited Methods ===
		// Empty Definitions included here to satisfy linker

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverPETSc<C,I,T>::setupVectorX() {
			PetscErrorCode err;

			// Reset any current vectors
			this->resetVectorX();

			if(this->comm.size == 1) {
				// === Comm Size is set to 1 - Serial Linear Solver ===

				// Setup a PETSc Sequential Vector
				err = VecCreateSeq(PETSC_COMM_SELF, this->mGlobal, &(this->x));
				if (err != 0) {
					return cupcfd::error::E_PETSC_ERROR;
				}
			}
			else if(this->comm.size > 1) {
				// Setup a PETSc Parallel (MPI) Vector
				err = VecCreateMPI(this->comm.comm, PETSC_DECIDE, this->mGlobal, &(this->x));
				if (err != 0) {
					return cupcfd::error::E_PETSC_ERROR;
				}
			}
			else {
				// Comm Size is less than 1 - Error
				return cupcfd::error::E_ERROR;
			}

			// Get the Local Ownership range of the Vector X
			// For serial, this will be of size 2
			// For parallel, number of ranks + 1
			err = VecGetOwnershipRanges(this->x, &(this->xRanges));
			if (err != 0) {
				return cupcfd::error::E_PETSC_ERROR;
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverPETSc<C,I,T>::setupVectorB() {
			PetscErrorCode err;

			if(this->comm.size == 1) {
				// === Comm Size is set to 1 - Serial Linear Solver ===

				// Setup a PETSc Sequential Vector
				err = VecCreateSeq(PETSC_COMM_SELF, this->mGlobal, &(this->b));
				if (err != 0) {
					return cupcfd::error::E_PETSC_ERROR;
				}
			}
			else if(this->comm.size > 1) {
				// Setup a PETSc Parallel (MPI) Vector
				err = VecCreateMPI(this->comm.comm, PETSC_DECIDE, this->mGlobal, &(this->b));
				if (err != 0) {
					return cupcfd::error::E_PETSC_ERROR;
				}
			}
			else {
				// Comm Size is less than 1 - Error
				return cupcfd::error::E_ERROR;
			}

			// Get the Local Ownership range of the Vector B
			// For serial, this will be of size 1 + 1
			err = VecGetOwnershipRanges(this->b, &(this->bRanges));
			if (err != 0) {
				return cupcfd::error::E_PETSC_ERROR;
			}

			// Should be the same as the X Vector (else we will have issues....)
			// Overwrite existing value in case it hasn't been done for X yet.
			this->mLocal = this->bRanges[this->comm.rank + 1] - this->bRanges[this->comm.rank];

			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverPETSc<C,I,T>::setupMatrixA(cupcfd::data_structures::SparseMatrix<C,I,T>& matrix) {
			PetscErrorCode err;

			// ToDo: Error Check - The Matrix Global Sizes must match the global sizes of the linear solver
			// (even if it doesn't store that much data)

			if(this->comm.size == 1) {
				// Retrieve the non-zero structure from the SparseMatrix object and pass through to
				// the PETSc matrix initialisation routine to help with preallocation.

				// First, get the indexes of rows with non-zero values
				int * nnzRows;
				int nNNZRows;
				err = matrix.getNonZeroRowIndexes(&nnzRows, &nNNZRows);
				if (err != 0) {
					return cupcfd::error::E_PETSC_ERROR;
				}

				// To do this, create an array to store the number of non-zeroes per row and initially set them all to zero
				// Since this is serial, create an entry for all rows
				PetscInt * nnz = (PetscInt *) malloc(sizeof(PetscInt) * this->mGlobal);

				// Zero the array
				for(int i = 0; i < this->mGlobal; i++) {
					nnz[i] = 0;
				}

				int nnzCount = 0;


				for(int i = 0; i < nNNZRows; i++) {
					// For each row, get the number of columns (what we're actually interested in)
					int * nnzCols;
					int nNNZCols;
					err = matrix.getRowColumnIndexes(nnzRows[i], &nnzCols, &nNNZCols);
					if (err != 0) {
						return cupcfd::error::E_PETSC_ERROR;
					}

					// Store the number of columns for the matching matrix rowIndex in nnzRows[i]
					// Since PETSc will be accessing this using base zero, offset the row index such
					// that it is in base zero
					nnz[nnzRows[i] - matrix.baseIndex] = nNNZCols;
					nnzCount = nnzCount + nNNZCols;

					// Cleanup
					free(nnzCols);
				}

				// === Get the non-zero column indexes for each row and store as CSR ===
				PetscInt * indices = (PetscInt *) malloc(sizeof(PetscInt) * nnzCount);

				int ptr = 0;
				for(int i = 0; i < nNNZRows; i++) {
					// For each row, get the column indexes
					int * nnzCols;
					int nNNZCols;
					err = matrix.getRowColumnIndexes(nnzRows[i], &nnzCols, &nNNZCols);
					if (err != 0) {
						return cupcfd::error::E_PETSC_ERROR;
					}

					for(int j = 0; j < nNNZCols; j++) {
						// Don't forget to remove offset for matrix indexing so it is base zero
						indices[ptr] = nnzCols[j] - matrix.baseIndex;
						ptr = ptr + 1;
					}

					// Cleanup
					free(nnzCols);
				}

				// Create the PETSc Matrix Object based on the input matrix object
				// nz is set to PETSC_DEFAULT, but it should ignore it if nnz is set
				//MatCreateSeqAIJ(PETSC_COMM_SELF, this->mLocal, this->nLocal, PETSC_DEFAULT, nnz, &(this->a));
				// Create the Matrix Object
				err = MatCreate(PETSC_COMM_SELF, &(this->a));
				if (err != 0) {
					return cupcfd::error::E_PETSC_ERROR;
				}

				// Set the Matrix Type to a Sequential Matrix
				err = MatSetType(this->a, MATSEQAIJ);
				if (err != 0) {
					return cupcfd::error::E_PETSC_ERROR;
				}

				// Set the global size of the matrix, leave the local size to PETSc
				// Since this is a sequential matrix, they should end up being one and the same anyway
				err = MatSetSizes(this->a, PETSC_DECIDE, PETSC_DECIDE, this->mGlobal, this->nGlobal);
				if (err != 0) {
					return cupcfd::error::E_PETSC_ERROR;
				}

				// Set the number of non-zero values per row for preallocation purposes
				err = MatSeqAIJSetPreallocation(this->a, -1, nnz);
				if (err != 0) {
					return cupcfd::error::E_PETSC_ERROR;
				}

				// Set the column indices of the non-zero location to improve preallocation
				err = MatSeqAIJSetColumnIndices(this->a, indices);
				if (err != 0) {
					return cupcfd::error::E_PETSC_ERROR;
				}

				// Since introducing new non-zero location is expensive, disable their inclusion
				// by ignoring the values and introducing a PETSc error
				// Ideally all reuse of the matrix should have the same non-zero structure - new
				// structures should create a different matrix to use.
				// ToDo: Do we want to have a PETSc error - if we can catch the error we can introduce
				// an error code....
				err = MatSetOption(this->a, MAT_NEW_NONZERO_LOCATIONS, PETSC_FALSE);
				if (err != 0) {
					return cupcfd::error::E_PETSC_ERROR;
				}
				err = MatSetOption(this->a, MAT_NEW_NONZERO_LOCATION_ERR, PETSC_TRUE);
				if (err != 0) {
					return cupcfd::error::E_PETSC_ERROR;
				}

				// Cleanup temporary storage
				free(nnz);
				free(indices);
			}
			else if(this->comm.size > 1) {
				// Retrieve the non-zero structure from the SparseMatrix object and pass through to
				// the PETSc matrix initialisation routine to help with preallocation.
				// ToDo: This functionality should probably be moved to the SparseMatrix object itself, since it will
				// probably be used more than once.

				// Get the indexes of rows with non-zero values
				int * nnzRows;
				int nNNZRows;
				// int nnzCount = 0;
				err = matrix.getNonZeroRowIndexes(&nnzRows, &nNNZRows);
				if (err != 0) {
					return cupcfd::error::E_PETSC_ERROR;
				}

				// Get the matrix local data sizes...
				// I mLocal = 0;

				// Diagonal Count
				int * d_nnz = (PetscInt *) malloc(sizeof(PetscInt) * this->mLocal);

				// Off-Diagonal Count
				int * o_nnz = (PetscInt *) malloc(sizeof(PetscInt) * this->mLocal);

				for(int i = 0; i < this->mLocal; i++) {
					d_nnz[i] = 0;
					o_nnz[i] = 0;
				}

				// We need to count the number of diagonal and off-diagonal nnz values
				// This is bet described at https://www.mcs.anl.gov/petsc/petsc-current/docs/manualpages/MatMPIAIJSetPreallocation
				// but the intent is basically to subdivide the matrix into submatrices. If a non-zero occurs in the sub-matrix that
				// sits on the diagonal of the global submatrices, then it is a diagonal nnz, else it is a off-diagonal non-zero value

				// IMPORTANT NOTE: This only works if we are assuming a couple of things, namely that the matrix rows are sequentially
				// grouped across the ranks (i.e. Rank 0 has 0-4, Rank 1 has 5-7, Rank 2 has 8-10 etc) and we enforce the allocation
				// of a local row size that is equivalent to this value. If not, we have a dilema since if we let PETSc decide the local
				// size, or if the local space we are allocated is greater than the rows we have, then we don't have the information
				// necessary to determine what the diagonal/non-diagonal components will look like

				// As such, this currently relies on the provided matrix following this pattern, but it is not enforced.

				/*
				for(int i = 0; i < nNNZRows; i++) {
					// For each row, get the column indexes and the number of columns (what we're actually interested in)
					int * nnzCols;
					int nNNZCols;
					matrix.getRowColumnIndexes(nnzRows[i], &nnzCols, &nNNZCols);
					nnzCount = nnzCount + nNNZCols;

					// Loop over the column indices of the non-zero values for this row.
					// Determine whether a column lies in the diagonal sub-matrix, or one of the off-diagonal matrices
					// The documentation suggests 3x3 matrices, so I presume that applies regardless of matrix size
					// To do this, we take the column ID, and if it lies

					int rowBaseZero = nnzRows[i] - matrix.baseIndex;

					for(int j = 0; j < nNNZCols; j++) {
						int colBaseZero = nnzCols[j] - matrix.baseIndex;

						// Integer division important here
						if((rowBaseZero/3) == (colBaseZero/3)) {
							// NZ lies in diagonal matrix, increase count for this row by 1
							d_nnz[i] = d_nnz[i] + 1;
						}
						else {
							// NZ lies in off-diagonal matrix, increase count for this row by 1
							o_nnz[i] = o_nnz[i] + 1;
						}
					}

					// Cleanup
					free(nnzCols);
				}
				 */

				// Create the PETSc Matrix Object based on the input matrix object
				// nz is set to PETSC_DEFAULT, but it should ignore it if nnz is set

				// Create the Matrix Object
				// Use the communicator assigned during setup
				err = MatCreate(this->comm.comm, &(this->a));
				if (err != 0) {
					return cupcfd::error::E_PETSC_ERROR;
				}

				// Set the Matrix Type to a Parallel MPI Matrix
				err = MatSetType(this->a, MATMPIAIJ);
				if (err != 0) {
					return cupcfd::error::E_PETSC_ERROR;
				}

				// Set the global size of the matrix, leave the local size to PETSc
				// Set the local rows to the rows of the matrix we have
				// Set the local columns to be equivalent to the rows we have (since this affects the x vector)
				err = MatSetSizes(this->a, PETSC_DECIDE, PETSC_DECIDE, this->mGlobal, this->nGlobal);
				if (err != 0) {
					return cupcfd::error::E_PETSC_ERROR;
				}

				// IMPORTANT TODO: Fix the preallocation, setting of local sizes

				// Set the number of non-zero values per row for preallocation purposes
				// For MPIAIJ we need to consider diagonal and off-diagonal values separately

				//MatMPIAIJSetPreallocation(this->a, NULL, d_nnz, NULL, o_nnz);
				// Ideally want to use Preallocation, but just do basic setup for now
				err = MatSetUp(this->a);
				if (err != 0) {
					return cupcfd::error::E_PETSC_ERROR;
				}

				// Since introducing new non-zero location is expensive, disable their inclusion
				// by ignoring the values and introducing a PETSc error
				// Ideally all reuse of the matrix should have the same non-zero structure - new
				// structures should create a different matrix to use.
				// ToDo: Do we want to have a PETSc error - if we can catch the error we can introduce
				// an error code....
				// Remove this for now
				//MatSetOption(this->a, MAT_NEW_NONZERO_LOCATIONS, PETSC_FALSE);
				//MatSetOption(this->a, MAT_NEW_NONZERO_LOCATION_ERR, PETSC_TRUE);

				// Cleanup temporary storage
				free(d_nnz);
				free(o_nnz);

				// Get Ownership Range for the Matrix
				//MatGetOwnershipRanges(this->a, &(this->aRanges));
			}
			else {
				// Comm Size is less than 1 - Error
				return cupcfd::error::E_ERROR;
			}

			// Get Ownership Range for the Matrix
			err = MatGetOwnershipRanges(this->a, &(this->aRanges));
			if (err != 0) {
				return cupcfd::error::E_PETSC_ERROR;
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverPETSc<C,I,T>::setup(cupcfd::data_structures::SparseMatrix<C,I,T>& matrix) {
			cupcfd::error::eCodes status;

			// Create the vector X and zero it
			status = setupVectorX();
			CHECK_ECODE(status)

			status = this->setValuesVectorX(0.0);
			CHECK_ECODE(status)

			// Create the vector B and zero it
			status = setupVectorB();
			CHECK_ECODE(status)

			status = this->setValuesVectorB(0.0);
			CHECK_ECODE(status)

			// Create the matrix A, using the input Matrix to define the non-zero positions
			status = setupMatrixA(matrix);
			CHECK_ECODE(status)

			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverPETSc<C,I,T>::setValuesMatrixA(cupcfd::data_structures::SparseMatrix<C,I,T>& matrix) {
			if(this->mGlobal <= 0) {
				return cupcfd::error::E_LINEARSOLVER_ROW_SIZE_UNSET;
			}

			if(this->nGlobal <= 0) {
				return cupcfd::error::E_LINEARSOLVER_COL_SIZE_UNSET;
			}

			// Error Check: Check that the PETSc matrix object has been setup
			if(this->a == PETSC_NULL) {
				return cupcfd::error::E_LINEARSOLVER_INVALID_MATRIX;
			}

			cupcfd::error::eCodes status;

			// Get an array of the unique row indexes - we will set the values for these rows one by one
			int * rowIndexes;
			int nRowIndexes;
			status = matrix.getNonZeroRowIndexes(&rowIndexes, &nRowIndexes);
			CHECK_ECODE(status)

			for(int i = 0; i < nRowIndexes; i++) {
				// Get the columns ids for the row
				int * columnIndexes;
				int nColumnIndexes;
				status = matrix.getRowColumnIndexes(rowIndexes[i], &columnIndexes, &nColumnIndexes);
				CHECK_ECODE(status)

				// Get the nnz values for the row
				T * nnzValues;
				PetscScalar * petscNNZValues;
				int nNNZValues;
				status = matrix.getRowNNZValues(rowIndexes[i], &nnzValues, &nNNZValues);
				CHECK_ECODE(status)

				// ToDo: Would like to omit this copy, and place data in array directly
				// Could create new instance of getRowNNZValues, but would have to include PETSc header
				petscNNZValues = (PetscScalar *) malloc(sizeof(PetscScalar) * nNNZValues);
				for(int j = 0; j < nNNZValues; j++) {
					petscNNZValues[j] = nnzValues[j];
				}

				// Set the values in the PETSc matrix for this row
				// ToDo: Is v. likely more efficient to do all rows at once -> build up a 2D array of values instead?
				MatSetValues(this->a, 1, &rowIndexes[i], nColumnIndexes, columnIndexes, petscNNZValues, INSERT_VALUES);

				free(columnIndexes);
				free(nnzValues);
			}

			free(rowIndexes);

			// ToDo: Do we want to use MAT_FINAL_ASSEMBLY here?
			// Presumably this will be more expensive
			// Doesn't seem to impact ability to add more values after matrix clear, so shouldn't be a problem
			// as long as this method is used to set all values from a rank at once for a solve?
			// Begin Assembly
			MatAssemblyBegin(this->a, MAT_FINAL_ASSEMBLY);

			// End Assembly
			MatAssemblyEnd(this->a, MAT_FINAL_ASSEMBLY);

			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverPETSc<C,I,T>::clearMatrixA() {
			return cupcfd::error::E_SUCCESS;
		}


		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverPETSc<C,I,T>::getValuesVectorX(T ** result, I * nResult) {
			cupcfd::error::eCodes status;

			// Check the global row count is greater than 0
			if(this->mGlobal <= 0) {
				return cupcfd::error::E_LINEARSOLVER_ROW_SIZE_UNSET;
			}

			// Check the Vector exists/has been setup
			if(this->x == PETSC_NULL) {
				return cupcfd::error::E_LINEARSOLVER_INVALID_VECTOR;
			}

			if(this->comm.size == 1) {
				// PETSc can only return values from vector on same processor
				// However, since this is serial, the process owns all indices in the vector
				// Thus, we can just generate a full indices set and return the vector contents in the array

				// Note: We need to convert types for PETSc - this may incur a loss of precision if the
				// template type does not match the PETSc linear solver datatype size.

				// Allocate the array to pass the results back in
				*nResult = this->mGlobal;
				*result = (T *) malloc(sizeof(T) * (*nResult));

				PetscReal * tmpResult = (PetscScalar *) malloc(sizeof(PetscScalar) * (*nResult));
				PetscInt * indices = (PetscInt *) malloc(sizeof(PetscInt) * (*nResult));

				// PETSc always uses 0-based indexes
				for(int i = 0; i < this->mGlobal; i++) {
					indices[i] = (PetscInt) i;
				}

				// Call PETSc to get data
				VecGetValues(this->x, *nResult, indices, tmpResult);

				// Copy data from tmp result to result for type conversion
				// ToDo: Possible loss of precision depending on PETSc Scalar size settings
				for(I i = 0; i < *nResult; i++) {
					(*result)[i] = T(tmpResult[i]);
				}

				// Cleanup
				free(indices);
				free(tmpResult);

			}
			else if(this->comm.size > 1) {
				// Expensive - Everyone gets a copy of the vector and distributed comms will require all-to-all
				// However, we know the range from the distributed PETSc objects, so we know how many elements we
				// need from each process

				// PETSc VecGetValues only works for Local Indexes - Therefore we must get the local values and
				// redistribute as necessary.
				I rangeSize = xRanges[this->comm.rank+1] - xRanges[this->comm.rank];

				PetscReal * tmpResult = (PetscScalar *) malloc(sizeof(PetscScalar) * rangeSize);
				PetscInt * indices = (PetscInt *) malloc(sizeof(PetscInt) * rangeSize);

				// PETSc always uses 0-based indexes
				// Get values for local indexes
				for(I i = 0; i < rangeSize; i++) {
					indices[i] = (PetscInt) xRanges[this->comm.rank] + i;
				}


				// Call PETSc to get local data
				VecGetValues(this->x, rangeSize, indices, tmpResult);

				// Copy data from tmp result to result for type conversion
				// ToDo: Possible loss of precision depending on PETSc Scalar size settings

				T * tmpBuffer = (T *) malloc(sizeof(T) * rangeSize);

				for(I i = 0; i < rangeSize; i++) {
					tmpBuffer[i] = T(tmpResult[i]);
				}


				// AllGather to Gather Data to each process
				// Need storage for recvCounts (althought not used)
				I * recvCounts = nullptr;
				I nRecvCounts;

				*result = nullptr;

				// This will place the final allgather directly into the results array
				status = AllGatherV(tmpBuffer, rangeSize, result, nResult, &recvCounts, &nRecvCounts, this->comm);
				CHECK_ECODE(status)

				// Cleanup
				free(indices);
				free(tmpResult);
				free(tmpBuffer);
				free(recvCounts);
			}
			else {
				return cupcfd::error::E_ERROR;
			}


			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverPETSc<C,I,T>::getValuesVectorX(T ** result, I * nResult,
																 I * indexes, I nIndexes, I indexBase) {
			cupcfd::error::eCodes status;

			// Check the global row count is greater than 0
			if(this->mGlobal <= 0) {
				return cupcfd::error::E_LINEARSOLVER_ROW_SIZE_UNSET;
			}

			// Check the Vector exists/has been setup
			if(this->x == PETSC_NULL) {
				return cupcfd::error::E_LINEARSOLVER_INVALID_VECTOR;
			}

			if(this->comm.size == 1) {
				// Serial - Just load specific indices from the Vector

				// Note: We need to convert types for PETSc - this may incur a loss of precision if the
				// template type does not match the PETSc linear solver datatype size.

				// Allocate the array to pass the results back in
				*nResult = nIndexes;
				*result = (T *) malloc(sizeof(T) * (*nResult));

				PetscReal * tmpResult = (PetscScalar *) malloc(sizeof(PetscScalar) * (*nResult));
				PetscInt * indices = (PetscInt *) malloc(sizeof(PetscInt) * (*nResult));

				// PETSc always uses 0-based indexes
				for(int i = 0; i < nIndexes; i++) {
					indices[i] = indexes[i] - indexBase;
				}

				// Call PETSc to get data
				VecGetValues(this->x, *nResult, indices, tmpResult);

				// Copy data from tmp result to result for type conversion
				// ToDo: Possible loss of precision depending on PETSc Scalar size settings
				for(I i = 0; i < *nResult; i++) {
					(*result)[i] = T(tmpResult[i]);
				}

				// Cleanup
				free(indices);
				free(tmpResult);
			}
			else if(this->comm.size > 1) {
				// (1) Validity Check - If all requested indices across processes are local, we don't need to do
				// any large scale comms beyond a notification

				I nOffNodeCount = 0;
				I nTotalOffNodeCount = 0;

				for(I i = 0; i < nIndexes; i++) {
					I baseZeroIndex = indexes[i] - indexBase;

					if((baseZeroIndex < this->xRanges[this->comm.rank]) || (baseZeroIndex >= this->xRanges[this->comm.rank + 1])) {
						nOffNodeCount = nOffNodeCount + 1;
					}
				}

				cupcfd::comm::allReduceAdd(&nOffNodeCount, 1, &nTotalOffNodeCount, 1, this->comm);

				if(nTotalOffNodeCount == 0) {
					// All indices are local across all processes, so no need for futher comms
					*nResult = nIndexes;
					*result = (T *) malloc(sizeof(T) * (*nResult));

					PetscReal * tmpResult = (PetscScalar *) malloc(sizeof(PetscScalar) * (*nResult));
					PetscInt * indices = (PetscInt *) malloc(sizeof(PetscInt) * (*nResult));

					// PETSc always uses 0-based indexes
					for(int i = 0; i < nIndexes; i++) {
						indices[i] = indexes[i] - indexBase;
					}

					// Call PETSc to get data
					VecGetValues(this->x, *nResult, indices, tmpResult);

					// Copy data from tmp result to result for type conversion
					// ToDo: Possible loss of precision depending on PETSc Scalar size settings
					for(I i = 0; i < *nResult; i++) {
						(*result)[i] = T(tmpResult[i]);
					}

					// Cleanup
					free(indices);
					free(tmpResult);
				}
				else {
					// (2) If there is a off-node requested indice globally then we will need to perform an exchange of some sort.
					// Build a temporary distributed graph so that information is gathered about who 'owns' which indexes.
					// We can then use this to build an exchange pattern
					// ToDo: This process will involve All-To-Alls which are frankly too expensive for repeated construction like this.
					// Ideally an exchange pattern for linear solvers should be built and reused as a one-off ahead of time.
					// However this function takes an arbitrary index request so to accomodate any request requires a more robust
					// (but expensive) solution. We can add another function that is used for repeated patterns.


					cupcfd::data_structures::DistributedAdjacencyList<I,I> graph(this->comm);
					I rangeSize = this->xRanges[this->comm.rank + 1] - this->xRanges[this->comm.rank];

					PetscReal * tmpResult = (PetscScalar *) malloc(sizeof(PetscScalar) * rangeSize);
					PetscInt * indices = (PetscInt *) malloc(sizeof(PetscInt) * rangeSize);

					// Setup the PETSc indices to read all local data
					for(I i = this->xRanges[this->comm.rank]; i < this->xRanges[this->comm.rank + 1]; i++) {
						indices[i - this->xRanges[this->comm.rank]] = i;

						// Need to add all of the local indices as local nodes in case another node requests them
						status = graph.addLocalNode(i);
						CHECK_ECODE(status)
					}

					// Setup the Ghost Nodes of the Graph
					for(I i = 0; i < nIndexes;i++) {
						I baseZeroIndex = indexes[i] - indexBase;

						if((baseZeroIndex < this->xRanges[this->comm.rank]) || (baseZeroIndex >= this->xRanges[this->comm.rank + 1])) {
							// Add any row indices that are non-local requested indices as ghost nodes
							status = graph.addGhostNode(baseZeroIndex);
							CHECK_ECODE(status)
						}
					}

					// Finalize
					status = graph.finalize();
					CHECK_ECODE(status)

					// Build exchange pattern
					cupcfd::comm::ExchangePatternTwoSidedNonBlocking<T> * pattern;
					status = graph.buildExchangePattern(&pattern);
					CHECK_ECODE(status)

					// Setup data buffers and global indexes
					I nDataBuffer = graph.connGraph.nNodes;
					T * dataBuffer = (T *) malloc(sizeof(T) * nDataBuffer);

					// Read in Local Data
					VecGetValues(this->x, rangeSize, indices, tmpResult);

					// Loop over the Local/Global IDs of the exchange pattern, and copy to the data buffer if it a local node
					// since it is expected to be sent (we know that local nodes are those nodes (PETSc indices) in the local range)
					for(I i = 0; i < nDataBuffer; i++) {
						// Copy Data to correct positions in the data buffer for the exchange
						// Ordering of data in the exchange buffer is the same order as that of the localIDs in the graph
						// I.e. The node at localID 0 in the graph stores its data at index 0 in the data buffer

						// Looping over Graph Local Indexes
						// (1) Get Node (PETScIndex) at that local index
						I node;
						status = graph.connGraph.getLocalIndexNode(i, &node);
						CHECK_ECODE(status)

						// Is this a Node/PETScIndex that we own/is local
						if((node >= this->xRanges[this->comm.rank]) && (node < this->xRanges[this->comm.rank + 1])) {
							// It is, so find and copy its data to the buffer
							// Data is stored in the array read from the PETSc object, but offset from the index by the range minimum
							I lookupID = node - this->xRanges[this->comm.rank];
							dataBuffer[i] = T(tmpResult[lookupID]);
						}
						else {
							// It is not, so the data will be overwritten in the exchange. Set it to 0 for now
							dataBuffer[i] = 0.0;
						}
					}

					// Perform the data exchange
					pattern->exchangeStart(dataBuffer, nDataBuffer);
					pattern->exchangeStop(dataBuffer, nDataBuffer);

					// Copy data from dataBuffer to result
					*nResult = nIndexes;
					*result = (T *) malloc(sizeof(T) * (*nResult));

					for(I i = 0; i < nIndexes; i++) {
						// Find the position of the current index in the dataBuffer
						// Can look it up via the graph (node and base zero PETSc index are same here)
						I baseZeroIndex = indexes[i] - indexBase;

						I localID;
						status = graph.connGraph.getNodeLocalIndex(baseZeroIndex, &localID);
						CHECK_ECODE(status)

						// This localID is the position of the data i the dataBuffer, copy across
						(*result)[i] = dataBuffer[localID];
					}

					// Cleanup
					free(indices);
					free(tmpResult);
					free(dataBuffer);
				}
			}
			else {
				return cupcfd::error::E_ERROR;
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverPETSc<C,I,T>::getValuesVectorB(T ** result, I * nResult) {
			cupcfd::error::eCodes status;

			// Check the global row count is greater than 0
			if(this->mGlobal <= 0) {
				return cupcfd::error::E_LINEARSOLVER_ROW_SIZE_UNSET;
			}

			// Check the Vector exists/has been setup
			if(this->b == PETSC_NULL) {
				return cupcfd::error::E_LINEARSOLVER_INVALID_VECTOR;
			}

			if(this->comm.size == 1) {
				// PETSc can only return values from vector on same processor
				// However, since this is serial, the process owns all indices in the vector
				// Thus, we can just generate a full indices set and return the vector contents in the array

				// Note: We need to convert types for PETSc - this may incur a loss of precision if the
				// template type does not match the PETSc linear solver datatype size.

				// Allocate the array to pass the results back in
				*nResult = this->mGlobal;
				*result = (T *) malloc(sizeof(T) * (*nResult));

				PetscReal * tmpResult = (PetscScalar *) malloc(sizeof(PetscScalar) * (*nResult));
				PetscInt * indices = (PetscInt *) malloc(sizeof(PetscInt) * (*nResult));

				// PETSc always uses 0-based indexes
				for(int i = 0; i < this->mGlobal; i++) {
					indices[i] = (PetscInt) i;
				}

				// Call PETSc to get data
				VecGetValues(this->b, *nResult, indices, tmpResult);

				// Copy data from tmp result to result for type conversion
				// ToDo: Possible loss of precision depending on PETSc Scalar size settings
				for(I i = 0; i < *nResult; i++) {
					(*result)[i] = T(tmpResult[i]);
				}

				// Cleanup
				free(indices);
				free(tmpResult);

			}
			else if(this->comm.size > 1) {
				// Expensive - Everyone gets a copy of the vector and distributed comms will require all-to-all
				// However, we know the range from the distributed PETSc objects, so we know how many elements we
				// need from each process

				// PETSc VecGetValues only works for Local Indexes - Therefore we must get the local values and
				// redistribute as necessary.
				I rangeSize = bRanges[this->comm.rank+1] - bRanges[this->comm.rank];

				PetscReal * tmpResult = (PetscScalar *) malloc(sizeof(PetscScalar) * rangeSize);
				PetscInt * indices = (PetscInt *) malloc(sizeof(PetscInt) * rangeSize);

				// PETSc always uses 0-based indexes
				// Get values for local indexes
				for(I i = 0; i < rangeSize; i++) {
					indices[i] = (PetscInt) bRanges[this->comm.rank] + i;
				}


				// Call PETSc to get local data
				VecGetValues(this->b, rangeSize, indices, tmpResult);

				// Copy data from tmp result to result for type conversion
				// ToDo: Possible loss of precision depending on PETSc Scalar size settings

				T * tmpBuffer = (T *) malloc(sizeof(T) * rangeSize);

				for(I i = 0; i < rangeSize; i++) {
					tmpBuffer[i] = T(tmpResult[i]);
				}


				// AllGather to Gather Data to each process
				// Need storage for recvCounts (althought not used)
				I * recvCounts = nullptr;
				I nRecvCounts;

				*result = nullptr;

				// This will place the final allgather directly into the results array
				status = AllGatherV(tmpBuffer, rangeSize, result, nResult, &recvCounts, &nRecvCounts, this->comm);
				CHECK_ECODE(status)

				// Cleanup
				free(indices);
				free(tmpResult);
				free(tmpBuffer);
				free(recvCounts);
			}
			else {
				return cupcfd::error::E_ERROR;
			}


			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverPETSc<C,I,T>::getValuesVectorB(T ** result, I * nResult,
																I * indexes, I nIndexes, I indexBase) {
			cupcfd::error::eCodes status;

			// Check the global row count is greater than 0
			if(this->mGlobal <= 0) {
				return cupcfd::error::E_LINEARSOLVER_ROW_SIZE_UNSET;
			}

			// Check the Vector exists/has been setup
			if(this->b == PETSC_NULL) {
				return cupcfd::error::E_LINEARSOLVER_INVALID_VECTOR;
			}

			if(this->comm.size == 1) {
				// Serial - Just load specific indices from the Vector

				// Note: We need to convert types for PETSc - this may incur a loss of precision if the
				// template type does not match the PETSc linear solver datatype size.

				// Allocate the array to pass the results back in
				*nResult = nIndexes;
				*result = (T *) malloc(sizeof(T) * (*nResult));

				PetscReal * tmpResult = (PetscScalar *) malloc(sizeof(PetscScalar) * (*nResult));
				PetscInt * indices = (PetscInt *) malloc(sizeof(PetscInt) * (*nResult));

				// PETSc always uses 0-based indexes
				for(int i = 0; i < nIndexes; i++) {
					indices[i] = indexes[i] - indexBase;
				}

				// Call PETSc to get data
				VecGetValues(this->b, *nResult, indices, tmpResult);

				// Copy data from tmp result to result for type conversion
				// ToDo: Possible loss of precision depending on PETSc Scalar size settings
				for(I i = 0; i < *nResult; i++) {
					(*result)[i] = T(tmpResult[i]);
				}

				// Cleanup
				free(indices);
				free(tmpResult);
			}
			else if(this->comm.size > 1) {
				// (1) Validity Check - If all requested indices across processes are local, we don't need to do
				// any large scale comms beyond a notification

				I nOffNodeCount = 0;
				I nTotalOffNodeCount = 0;

				for(I i = 0; i < nIndexes; i++) {
					I baseZeroIndex = indexes[i] - indexBase;

					if((baseZeroIndex < this->bRanges[this->comm.rank]) || (baseZeroIndex >= this->bRanges[this->comm.rank + 1])) {
						nOffNodeCount = nOffNodeCount + 1;
					}
				}

				cupcfd::comm::allReduceAdd(&nOffNodeCount, 1, &nTotalOffNodeCount, 1, this->comm);

				if(nTotalOffNodeCount == 0) {
					// All indices are local across all processes, so no need for futher comms
					*nResult = nIndexes;
					*result = (T *) malloc(sizeof(T) * (*nResult));

					PetscReal * tmpResult = (PetscScalar *) malloc(sizeof(PetscScalar) * (*nResult));
					PetscInt * indices = (PetscInt *) malloc(sizeof(PetscInt) * (*nResult));

					// PETSc always uses 0-based indexes
					for(int i = 0; i < nIndexes; i++) {
						indices[i] = indexes[i] - indexBase;
					}

					// Call PETSc to get data
					VecGetValues(this->b, *nResult, indices, tmpResult);

					// Copy data from tmp result to result for type conversion
					// ToDo: Possible loss of precision depending on PETSc Scalar size settings
					for(I i = 0; i < *nResult; i++) {
						(*result)[i] = T(tmpResult[i]);
					}

					// Cleanup
					free(indices);
					free(tmpResult);
				}
				else {
					// (2) If there is a off-node requested indice globally then we will need to perform an exchange of some sort.
					// Build a temporary distributed graph so that information is gathered about who 'owns' which indexes.
					// We can then use this to build an exchange pattern
					// ToDo: This process will involve All-To-Alls which are frankly too expensive for repeated construction like this.
					// Ideally an exchange pattern for linear solvers should be built and reused as a one-off ahead of time.
					// However this function takes an arbitrary index request so to accomodate any request requires a more robust
					// (but expensive) solution. We can add another function that is used for repeated patterns.


					cupcfd::data_structures::DistributedAdjacencyList<I,I> graph(this->comm);
					I rangeSize = this->bRanges[this->comm.rank + 1] - this->bRanges[this->comm.rank];

					PetscReal * tmpResult = (PetscScalar *) malloc(sizeof(PetscScalar) * rangeSize);
					PetscInt * indices = (PetscInt *) malloc(sizeof(PetscInt) * rangeSize);

					// Setup the PETSc indices to read all local data
					for(I i = this->bRanges[this->comm.rank]; i < this->bRanges[this->comm.rank + 1]; i++) {
						indices[i - this->bRanges[this->comm.rank]] = i;

						// Need to add all of the local indices as local nodes in case another node requests them
						status = graph.addLocalNode(i);
						CHECK_ECODE(status)
					}

					// Setup the Ghost Nodes of the Graph
					for(I i = 0; i < nIndexes;i++) {
						I baseZeroIndex = indexes[i] - indexBase;

						if((baseZeroIndex < this->bRanges[this->comm.rank]) || (baseZeroIndex >= this->bRanges[this->comm.rank + 1])) {
							// Add any row indices that are non-local requested indices as ghost nodes
							status = graph.addGhostNode(baseZeroIndex);
							CHECK_ECODE(status)
						}
					}

					// Finalize
					status = graph.finalize();
					CHECK_ECODE(status)

					// Build exchange pattern
					cupcfd::comm::ExchangePatternTwoSidedNonBlocking<T> * pattern;
					status = graph.buildExchangePattern(&pattern);
					CHECK_ECODE(status)

					// Setup data buffers and global indexes
					I nDataBuffer = graph.connGraph.nNodes;
					T * dataBuffer = (T *) malloc(sizeof(T) * nDataBuffer);

					// Read in Local Data
					VecGetValues(this->b, rangeSize, indices, tmpResult);

					// Loop over the Local/Global IDs of the exchange pattern, and copy to the data buffer if it a local node
					// since it is expected to be sent (we know that local nodes are those nodes (PETSc indices) in the local range)
					for(I i = 0; i < nDataBuffer; i++) {
						// Copy Data to correct positions in the data buffer for the exchange
						// Ordering of data in the exchange buffer is the same order as that of the localIDs in the graph
						// I.e. The node at localID 0 in the graph stores its data at index 0 in the data buffer

						// Looping over Graph Local Indexes
						// (1) Get Node (PETScIndex) at that local index
						I node;
						status = graph.connGraph.getLocalIndexNode(i, &node);
						CHECK_ECODE(status)

						// Is this a Node/PETScIndex that we own/is local
						if((node >= this->bRanges[this->comm.rank]) && (node < this->bRanges[this->comm.rank + 1])) {
							// It is, so find and copy its data to the buffer
							// Data is stored in the array read from the PETSc object, but offset from the index by the range minimum
							I lookupID = node - this->bRanges[this->comm.rank];
							dataBuffer[i] = T(tmpResult[lookupID]);
						}
						else {
							// It is not, so the data will be overwritten in the exchange. Set it to 0 for now
							dataBuffer[i] = 0.0;
						}
					}

					// Perform the data exchange
					pattern->exchangeStart(dataBuffer, nDataBuffer);
					pattern->exchangeStop(dataBuffer, nDataBuffer);

					// Copy data from dataBuffer to result
					*nResult = nIndexes;
					*result = (T *) malloc(sizeof(T) * (*nResult));

					for(I i = 0; i < nIndexes; i++) {
						// Find the position of the current index in the dataBuffer
						// Can look it up via the graph (node and base zero PETSc index are same here)
						I baseZeroIndex = indexes[i] - indexBase;

						I localID;
						status = graph.connGraph.getNodeLocalIndex(baseZeroIndex, &localID);
						CHECK_ECODE(status)

						// This localID is the position of the data i the dataBuffer, copy across
						(*result)[i] = dataBuffer[localID];
					}

					// Cleanup
					free(indices);
					free(tmpResult);
					free(dataBuffer);
				}
			}
			else {
				return cupcfd::error::E_ERROR;
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverPETSc<C,I,T>::getValuesMatrixA(cupcfd::data_structures::SparseMatrix<C,I,T>& matrix) {
			cupcfd::error::eCodes status;

			if(this->comm.size == 1) {
				// Error Check: Check Petsc Matrix is setup
				// Error Check: Check that the PETSc matrix object has been setup
				if(this->a == PETSC_NULL) {
					return cupcfd::error::E_LINEARSOLVER_INVALID_MATRIX;
				}

				// ToDo: Error Check: Check the bounds of the matrix object are within the index bounds of the PETSc matrix's global indices.

				// MatGetValues is not collective, it can only get values from the local block.
				// However, since this is a serial version, we know that all values are local, so we do not have to check for this

				int nRows;
				int * rows;

				int nCols;
				int * cols;

				// ToDo: Will do row by row for now, but would be more efficient to do in one go
				status = matrix.getNonZeroRowIndexes(&rows, &nRows);
				CHECK_ECODE(status)

				for(int i = 0; i < nRows; i++) {
					// Correct any row offsets for PETSc so it is base 0
					PetscInt rowIndex = rows[i] - matrix.baseIndex;

					status = matrix.getRowColumnIndexes(rowIndex, &cols, &nCols);
					CHECK_ECODE(status)
					PetscScalar * vals = (PetscScalar *) malloc(sizeof(PetscScalar) * nCols);
					MatGetValues(this->a, 1, &rowIndex, nCols, cols, vals);

					for(int j = 0; j < nCols; j++) {
						// Copy value over to the matrix object
						status = matrix.setElement(rowIndex, cols[j], vals[j]);
						CHECK_ECODE(status)
					}
					free(cols);
				}

				free(rows);
			}
			else if(this->comm.size > 1) {

			}
			else {
				return cupcfd::error::E_ERROR;
			}

			return cupcfd::error::E_SUCCESS;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverPETSc<C,I,T>::solve() {
			this->algSolver->solve(&a, &b, &x);

			return cupcfd::error::E_SUCCESS;
		}
	}
}

// Explicit Instantiation
template class cupcfd::linearsolvers::LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCSR<int, float>, int, float>;
template class cupcfd::linearsolvers::LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCOO<int, float>, int, float>;

template class cupcfd::linearsolvers::LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCSR<int, double>, int, double>;
template class cupcfd::linearsolvers::LinearSolverPETSc<cupcfd::data_structures::SparseMatrixCOO<int, double>, int, double>;
