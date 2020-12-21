/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Declaration of the MeshSourceFileConfig class
 */

#ifndef CUPCFD_GEOMETRY_MESH_SOURCE_FILE_CONFIG_INCLUDE_H
#define CUPCFD_GEOMETRY_MESH_SOURCE_FILE_CONFIG_INCLUDE_H

// C++ Library Includes
#include <string>

// MeshConfig Parent Class
#include "MeshSourceConfig.h"

// Error Codes
#include "Error.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			/** Enumerator of identifiers for potential file formats of the mesh **/
			enum MeshFileFormat
			{
				MESH_FILE_FORMAT_HDF5
			};

			/**
			 * This class defines the configuration for Mesh data that is read from a Mesh File
			 * of some format.
			 */
			// ToDo: The Label type probably shouldn't be hard-coded to I here. Correct for HDF5, could
			// be different for other formats.
			template <class I, class T>
			class MeshSourceFileConfig : public MeshSourceConfig<I,T,I>
			{
				public:
					// === Members ===

					/**  Store the format of the source mesh file **/
					MeshFileFormat fileFormat;

					/** Store the file path to the mesh file **/
					std::string sourceFilePath;

					// === Constructors/Deconstructors ===

					/**
					 * Constructor:
					 * Setup the configuration using the provided components.
					 *
					 * @param fileFormat The format of the file (e.g. specific HDF5 schema)
					 * @param sourceFilePath File path to where the mesh file is stored.
					 */
					MeshSourceFileConfig(MeshFileFormat fileFormat, std::string sourceFilePath);

					/**
					 * Constructor:
					 * Copy the configuration stored in another configuration.
					 *
					 * @param source The configuration to copy
					 */
					MeshSourceFileConfig(MeshSourceFileConfig<I,T>& source);

					/**
					 * Deconstructor.
					 * Cleans up the stored partitioner data.
					 */
					~MeshSourceFileConfig();

					// === Concrete Methods ===

					/**
					 * Get the file format specified by this configuration
					 *
					 * @return An enum identifier for the file format
					 */
					inline MeshFileFormat getFileFormat();

					/**
					 * Set the file format specified by this configuration
					 *
					 * @param format An enum identifier for the expected mesh file format.
					 *
					 * @return Nothing
					 */
					inline void setFileFormat(MeshFileFormat format);

					/**
					 * Get the file path to the mesh source specified by this configuration
					 *
					 * @return The file path to the mesh source file stored in this configuration
					 */
					inline std::string getSourceFilePath();

					/**
					 * Set the file path to the mesh source specified by this configuration
					 *
					 * @param sourceFilePath The source path to store in this configuration object
					 *
					 * @return Nothing
					 */
					inline void setSourceFilePath(std::string sourceFilePath);

					// === Overloaded Methods ===

					inline void operator=(const MeshSourceFileConfig<I,T>& source);
					MeshSourceFileConfig<I,T> * clone();
					__attribute__((warn_unused_result))
					cupcfd::error::eCodes buildMeshSource(MeshSource<I,T,I> ** source);
			};
		}
	}
}

// Include Header Level Definitions
#include "MeshSourceFileConfig.ipp"

#endif
