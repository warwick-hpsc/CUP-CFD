/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Class Definitions for the LinearSolverConfigPETScJSON class.
 */

#include "LinearSolverConfigPETScJSON.h"

#include <fstream>

#include "SparseMatrixCSR.h"
#include "SparseMatrixCOO.h"

namespace cupcfd
{
	namespace linearsolvers
	{
		// === Constructors/Deconstructors ===

	template <class C, class I, class T>
		LinearSolverConfigPETScJSON<C,I,T>::LinearSolverConfigPETScJSON(Json::Value& parseJSON)
		:LinearSolverConfigSource<C,I,T>()
		{
			this->configData = parseJSON;
		}

		template <class C, class I, class T>
		LinearSolverConfigPETScJSON<C,I,T>::~LinearSolverConfigPETScJSON()
		{
			// Nothing to do currently
		}

		// === Concrete Methods ===

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverConfigPETScJSON<C,I,T>::getPETScAlgorithm(PETScAlgorithm * solverAlg) {
			Json::Value dataSourceType;

			if(this->configData.isMember("Algorithm")) {
				// Access the correct field
				dataSourceType = this->configData["Algorithm"];

				// Check the value and return the appropriate ID
				if(dataSourceType == Json::Value::null) {
					return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
				}
				else if(dataSourceType == "CommandLine") {
					*solverAlg = PETSC_KSP_CMDLINE;
					return cupcfd::error::E_SUCCESS;
				}
				else if(dataSourceType == "CGAMG") {
					*solverAlg = PETSC_KSP_CGAMG;
					return cupcfd::error::E_SUCCESS;
				}

				// Found, but not a matching value
				return cupcfd::error::E_CONFIG_INVALID_VALUE;
			}

			return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverConfigPETScJSON<C,I,T>::getETol(T * eTol) {
			Json::Value dataSourceType;

			if(this->configData.isMember("eTol")) {
				// Access the correct field
				dataSourceType = this->configData["eTol"];

				// Check the value and return the appropriate ID
				if(dataSourceType == Json::Value::null) {
					return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
				}
				else {
					*eTol = T(dataSourceType.asDouble());
					return cupcfd::error::E_SUCCESS;
				}

				// Found, but not a matching value
				return cupcfd::error::E_CONFIG_INVALID_VALUE;
			}

			return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverConfigPETScJSON<C,I,T>::getRTol(T * rTol) {
			Json::Value dataSourceType;

			if(this->configData.isMember("rTol")) {
				// Access the correct field
				dataSourceType = this->configData["rTol"];

				// Check the value and return the appropriate ID
				if(dataSourceType == Json::Value::null) {
					return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
				}
				else {
					*rTol = T(dataSourceType.asDouble());
					return cupcfd::error::E_SUCCESS;
				}

				// Found, but not a matching value
				return cupcfd::error::E_CONFIG_INVALID_VALUE;
			}

			return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
		}

		template <class C, class I, class T>
		cupcfd::error::eCodes LinearSolverConfigPETScJSON<C,I,T>::buildLinearSolverConfig(LinearSolverConfig<C,I,T> ** linearSolverConfig) {
			cupcfd::error::eCodes status;

			PETScAlgorithm solverAlg;
			T eTol, rTol;

			status = this->getPETScAlgorithm(&solverAlg);
			CHECK_ERROR_CODE(status)
			if(status != cupcfd::error::E_SUCCESS) return status;

			status = this->getETol(&eTol);
			CHECK_ERROR_CODE(status)
			if(status != cupcfd::error::E_SUCCESS) return status;

			status = this->getRTol(&rTol);
			CHECK_ERROR_CODE(status)
			if(status != cupcfd::error::E_SUCCESS) return status;

			*linearSolverConfig = new LinearSolverConfigPETSc<C,I,T>(solverAlg, eTol, rTol);

			return cupcfd::error::E_SUCCESS;
		}
	}
}

template class cupcfd::linearsolvers::LinearSolverConfigPETScJSON<cupcfd::data_structures::SparseMatrixCSR<int,float>, int, float>;
template class cupcfd::linearsolvers::LinearSolverConfigPETScJSON<cupcfd::data_structures::SparseMatrixCSR<int,double>, int, double>;

template class cupcfd::linearsolvers::LinearSolverConfigPETScJSON<cupcfd::data_structures::SparseMatrixCOO<int,float>, int, float>;
template class cupcfd::linearsolvers::LinearSolverConfigPETScJSON<cupcfd::data_structures::SparseMatrixCOO<int,double>, int, double>;
