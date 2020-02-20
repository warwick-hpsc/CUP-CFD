/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Definitions for the BenchmarkConfigParticleSystemSimpleJSON class.
 */

#include "BenchmarkConfigParticleSystemSimpleJSON.h"

#include "CupCfdAoSMesh.h"
#include "CupCfdSoAMesh.h"

#include "DistributionConfigSourceFixedJSON.h"
#include "DistributionConfigSourceNormalJSON.h"
#include "DistributionConfigSourceUniformJSON.h"
#include "ParticleSystemSimpleConfigJSON.h"

#include <fstream>

namespace cupcfd
{
	namespace benchmark
	{
		template <class M, class I, class T, class L>
		BenchmarkConfigParticleSystemSimpleJSON<M,I,T,L>::BenchmarkConfigParticleSystemSimpleJSON(Json::Value& parseJSON)
		{
			this->configData = parseJSON;
		}

		template <class M, class I, class T, class L>
		BenchmarkConfigParticleSystemSimpleJSON<M,I,T,L>::~BenchmarkConfigParticleSystemSimpleJSON()
		{

		}

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes BenchmarkConfigParticleSystemSimpleJSON<M,I,T,L>::getBenchmarkName(std::string& benchmarkName)
		{
			cupcfd::error::eCodes status;

			if(this->configData.isMember("BenchmarkName"))
			{
				const Json::Value dataSourceType = this->configData["BenchmarkName"];

				if(dataSourceType == Json::Value::null)
				{
					return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
				}
				else
				{
					benchmarkName = dataSourceType.asString();
					return cupcfd::error::E_SUCCESS;
				}

				// Found, but not a matching value
				return cupcfd::error::E_CONFIG_INVALID_VALUE;
			}

			return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
		}

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes BenchmarkConfigParticleSystemSimpleJSON<M,I,T,L>::getBenchmarkRepetitions(I * repetitions)
		{
			cupcfd::error::eCodes status;

			if(this->configData.isMember("Repetitions"))
			{
				const Json::Value dataSourceType = this->configData["Repetitions"];

				if(dataSourceType == Json::Value::null)
				{
					return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
				}
				else
				{
					*repetitions = dataSourceType.asLargestInt();
					return cupcfd::error::E_SUCCESS;
				}

				// Found, but not a matching value
				return cupcfd::error::E_CONFIG_INVALID_VALUE;
			}

			return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
		}

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes BenchmarkConfigParticleSystemSimpleJSON<M,I,T,L>::getNTimesteps(I * nTimesteps)
		{
			cupcfd::error::eCodes status;

			if(this->configData.isMember("NTimesteps"))
			{
				const Json::Value dataSourceType = this->configData["NTimesteps"];

				if(dataSourceType == Json::Value::null)
				{
					return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
				}
				else
				{
					*nTimesteps = dataSourceType.asLargestInt();
					return cupcfd::error::E_SUCCESS;
				}

				// Found, but not a matching value
				return cupcfd::error::E_CONFIG_INVALID_VALUE;
			}

			return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
		}

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes BenchmarkConfigParticleSystemSimpleJSON<M,I,T,L>::getDtDistributionConfig(cupcfd::distributions::DistributionConfig<I,T> ** dtDistConfig)
		{
			// Iterate through JSON formats for different potential distribution types
			// Test each to see if a distribution can be built - if it can a viable format has been found.

			// ToDo: Move this out to the DistributionConfig class or a generic method elsewhere (so when a new distribution type is added,
			// we only have to edit one method.)

			if(this->configData.isMember("DtDistribution"))
			{
				cupcfd::distributions::DistributionConfig<I,T> * testDist;
				cupcfd::error::eCodes status;

				if(this->configData["DtDistribution"].isMember("FixedDistribution"))
				{
					cupcfd::distributions::DistributionConfigSourceFixedJSON<I,T> test1(this->configData["DtDistribution"]["FixedDistribution"]);
					status = test1.buildDistributionConfig(&testDist);

					if(status == cupcfd::error::E_SUCCESS)
					{
						// Don't cleanup testDist since we point the result at the same location
						*dtDistConfig = testDist;
						return cupcfd::error::E_SUCCESS;
					}
					else
					{
						return status;
					}
				}

				if(this->configData["DtDistribution"].isMember("NormalDistribution"))
				{
					cupcfd::distributions::DistributionConfigSourceNormalJSON<I,T> test1(this->configData["DtDistribution"]["NormalDistribution"]);
					status = test1.buildDistributionConfig(&testDist);

					if(status == cupcfd::error::E_SUCCESS)
					{
						// Don't cleanup testDist since we point the result at the same location
						*dtDistConfig = testDist;
						return cupcfd::error::E_SUCCESS;
					}
					else
					{
						return status;
					}
				}

				if(this->configData["DtDistribution"].isMember("UniformDistribution"))
				{
					cupcfd::distributions::DistributionConfigSourceNormalJSON<I,T> test1(this->configData["DtDistribution"]["UniformDistribution"]);
					status = test1.buildDistributionConfig(&testDist);

					if(status == cupcfd::error::E_SUCCESS)
					{
						// Don't cleanup testDist since we point the result at the same location
						*dtDistConfig = testDist;
						return cupcfd::error::E_SUCCESS;
					}
					else
					{
						return status;
					}
				}

				// No viable JSON found for a distribution
				return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
			}

			// No DtDistribution field
			return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
		}

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes BenchmarkConfigParticleSystemSimpleJSON<M,I,T,L>::getParticleSystemConfig(cupcfd::particles::ParticleSystemSimpleConfig<M,I,T,L> ** particleSystemConfig)
		{
			cupcfd::error::eCodes status;

			// Search for a JSON  Config describing the corret Particle System type matching the template
			if(this->configData.isMember("ParticleSystemSimple"))
			{
				cupcfd::particles::ParticleSystemSimpleConfigJSON<M,I,T,L> particleSystemConfigJSON(this->configData["ParticleSystemSimple"]);
				cupcfd::particles::ParticleSystemConfig<cupcfd::particles::ParticleSystemSimple<M,I,T,L>, cupcfd::particles::ParticleEmitterSimple<I,T>, cupcfd::particles::ParticleSimple<I,T>, M,I,T,L> * tmp;

				status = particleSystemConfigJSON.buildParticleSystemConfig(&tmp);
				if(status != cupcfd::error::E_SUCCESS)
				{
					return status;
				}

				// ToDo: Take a look at templates again, don't really want to have to typecast this....
				*particleSystemConfig = static_cast<cupcfd::particles::ParticleSystemSimpleConfig<M,I,T,L> *>(tmp);

				return cupcfd::error::E_SUCCESS;
			}

			// No field found
			return cupcfd::error::E_CONFIG_OPT_NOT_FOUND;
		}

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes BenchmarkConfigParticleSystemSimpleJSON<M,I,T,L>::buildBenchmarkConfig(BenchmarkConfigParticleSystemSimple<M,I,T,L> ** config)
		{
			cupcfd::error::eCodes status;

			std::string benchmarkName;
			I repetitions;
			I nTimesteps;
			cupcfd::distributions::DistributionConfig<I,T> * dtDistConfig;
			cupcfd::particles::ParticleSystemSimpleConfig<M,I,T,L> * particleSystemConfig;

			status = this->getBenchmarkName(benchmarkName);
			if(status != cupcfd::error::E_SUCCESS)
			{
				return status;
			}

			status = this->getBenchmarkRepetitions(&repetitions);
			if(status != cupcfd::error::E_SUCCESS)
			{
				return status;
			}

			status = this->getNTimesteps(&nTimesteps);
			if(status != cupcfd::error::E_SUCCESS)
			{
				return status;
			}

			status = this->getDtDistributionConfig(&dtDistConfig);
			if(status != cupcfd::error::E_SUCCESS)
			{
				return status;
			}

			status = this->getParticleSystemConfig(&particleSystemConfig);
			if(status != cupcfd::error::E_SUCCESS)
			{
				return status;
			}

			*config = new BenchmarkConfigParticleSystemSimple<M,I,T,L>(benchmarkName, repetitions, nTimesteps, *dtDistConfig, *particleSystemConfig);

			delete dtDistConfig;
			delete particleSystemConfig;

			return cupcfd::error::E_SUCCESS;
		}
	}
}

// Explicit Instantiation
template class cupcfd::benchmark::BenchmarkConfigParticleSystemSimpleJSON<cupcfd::geometry::mesh::CupCfdAoSMesh<int,float,int>, int, float, int>;
template class cupcfd::benchmark::BenchmarkConfigParticleSystemSimpleJSON<cupcfd::geometry::mesh::CupCfdAoSMesh<int,double,int>, int, double, int>;

template class cupcfd::benchmark::BenchmarkConfigParticleSystemSimpleJSON<cupcfd::geometry::mesh::CupCfdSoAMesh<int,float,int>, int, float, int>;
template class cupcfd::benchmark::BenchmarkConfigParticleSystemSimpleJSON<cupcfd::geometry::mesh::CupCfdSoAMesh<int,double,int>, int, double, int>;
