/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * This file contains the declarations for the CupCfd class
 */

#ifndef CUPCFD_RUN_STORE_INCLUDE_H
#define CUPCFD_RUN_STORE_INCLUDE_H

#include <string>
#include <vector>
#include <memory>

#include "Benchmark.h"

namespace cupcfd
{
	/**
	 * @tparam M Mesh Specialisation Class
	 * @tparam I Datatype for indexing
	 * @tparam T Datatype for computation/mesh geometry
	 * @tparam L Label type for mesh
	 */
	template <class M, class I, class T, class L>
	class CupCfd
	{
		public:
			// === Members ===

			/** Pointer to Mesh to use **/
			std::shared_ptr<M> meshPtr;

			/** Benchmarks to Run **/
			std::vector<cupcfd::benchmark::Benchmark<I,T> *> benchmarks;

			// === Constructors/Deconstructors ===

			/**
			 * Constructors.
			 *
			 * Setup any benchmarks, and store the mesh and system state for
			 * use in said benchmarks.
			 *
			 * The configuration of these is determined by the contents of the JSON file.
			 * Since the type of mesh is determined by CRTP, it canno be setup dynamically and
			 * thus the mesh object is setup separately from this class.
			 *
			 * @param jsonFilePath Path to JSON Configuration File
			 */
			CupCfd(std::string jsonFilePath, std::shared_ptr<M> meshPtr);

			/**
			 *
			 */
			~CupCfd();
	};
}

// Include Header Level Definitions
#include "CupCfd.ipp"

#endif
