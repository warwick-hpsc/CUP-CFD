/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Definitions for the MeshSourceFileConfig class
 */

#include "MeshSourceFileConfig.h"

namespace cupcfd
{
	namespace geometry
	{
		namespace mesh
		{
			template <class I, class T>
			MeshSourceFileConfig<I,T>::MeshSourceFileConfig(MeshFileFormat fileFormat, std::string sourceFilePath)
			: MeshSourceConfig<I,T,I>(),
			  fileFormat(fileFormat),
			  sourceFilePath(sourceFilePath)
			{
				// Nothing else to do
			}

			template <class I, class T>
			MeshSourceFileConfig<I,T>::MeshSourceFileConfig(MeshSourceFileConfig<I,T>& source)
			: MeshSourceConfig<I,T,I>()
			{
				// Constructor required a value above, but it will get overwritten again shortly with
				// same value.
				*this = source;
			}

			template <class I, class T>
			MeshSourceFileConfig<I,T>::~MeshSourceFileConfig()
			{
				// Nothing to clear in derived components.
			}

			template <class I, class T>
			MeshSourceFileConfig<I,T> * MeshSourceFileConfig<I,T>::clone()
			{
				return new MeshSourceFileConfig(*this);
			}
		}
	}
}

// Explicit Instantiation
template class cupcfd::geometry::mesh::MeshSourceFileConfig<int, float>;
template class cupcfd::geometry::mesh::MeshSourceFileConfig<int, double>;
