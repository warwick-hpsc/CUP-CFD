/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains definitions for the ParticleEmitterSimpleConfigJSON class.
 */

#include "ParticleEmitterSimpleConfigJSON.h"

namespace cupcfd
{
	namespace particles
	{

	}
}

// Explicit Instantiation
using namespace cupcfd::particles;

template class cupcfd::particles::ParticleEmitterSimpleConfigJSON<int, float>;
template class cupcfd::particles::ParticleEmitterSimpleConfigJSON<int, double>;

