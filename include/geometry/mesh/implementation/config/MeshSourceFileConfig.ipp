/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Definition of some functions of the MeshSourceFileConfig class at a header level
 */

#ifndef CUPCFD_GEOMETRY_MESH_SOURCE_FILE_CONFIG_IPP_H
#define CUPCFD_GEOMETRY_MESH_SOURCE_FILE_CONFIG_IPP_H

// C++ Library
#include <fstream>

#include "MeshHDF5Source.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			// === Concrete Methods ===

			template <class I, class T>
			inline MeshFileFormat MeshSourceFileConfig<I,T>::getFileFormat()
			{
				return this->fileFormat;
			}

			template <class I, class T>
			inline void MeshSourceFileConfig<I,T>::setFileFormat(MeshFileFormat format)
			{
				this->fileFormat = format;
			}

			template <class I, class T>
			inline std::string MeshSourceFileConfig<I,T>::getSourceFilePath()
			{
				return this->sourceFilePath;
			}

			template <class I, class T>
			inline void MeshSourceFileConfig<I,T>::setSourceFilePath(std::string sourceFilePath)
			{
				this->sourceFilePath = sourceFilePath;
			}
						
			// === Overloaded Inherited Methods ===
			
			template <class I, class T>
			inline void MeshSourceFileConfig<I,T>::operator=(const MeshSourceFileConfig<I,T>& source)
			{
				// Derived Members Copy
				this->setFileFormat(source.fileFormat);
				this->setSourceFilePath(source.sourceFilePath);
			}
			
			template <class I, class T>
			cupcfd::error::eCodes MeshSourceFileConfig<I,T>::buildMeshSource(MeshSource<I,T,I> ** source)
			{
				std::string filePath;
				cupcfd::geometry::mesh::MeshFileFormat fileFormat;

				// === Fetch Options ===

				// (1) Mesh Source File Location
				filePath = this->getSourceFilePath();

				// Check File is Accessible
				std::ifstream accessTest(filePath);
				if(!accessTest.good())
				{
					// File isn't open, so shouldn't need to close?
					return cupcfd::error::E_FILE_MISSING;
				}

				// (2) Mesh Source File Format
				fileFormat = this->getFileFormat();

				// Construct Mesh File Object
				// It is expected that the file format specified in config should be valid
				// via the use of the constructor (though we'll put a error catch just in case)
				if(fileFormat == cupcfd::geometry::mesh::MESH_FILE_FORMAT_HDF5)
				{
					*source = new cupcfd::geometry::mesh::MeshHDF5Source<I,T>(filePath);
				}
				else
				{
					// Unexpected File Format
					return cupcfd::error::E_INVALID_FILE_FORMAT;
				}

				// Done!

				return cupcfd::error::E_SUCCESS;
			}
		}
	}
}

#endif
