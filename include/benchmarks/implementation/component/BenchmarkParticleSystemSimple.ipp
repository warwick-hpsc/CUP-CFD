/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Header level definitions for the BenchmarkParticleSystem class.
 */

#ifndef CUPCFD_BENCHMARK_BENCHMARK_PARTICLESYSTEM_SIMPLE_IPP_H
#define CUPCFD_BENCHMARK_BENCHMARK_PARTICLESYSTEM_SIMPLE_IPP_H

#include "tt_interface_c.h"
#include "mpi.h"

namespace cupcfd
{
	namespace benchmark
	{
		template <class M, class I, class T, class L>
		BenchmarkParticleSystemSimple<M,I,T,L>::BenchmarkParticleSystemSimple(std::string benchmarkName, I repetitions,
																		I nTimesteps,
																		cupcfd::distributions::Distribution<I,T>& dtDist,
																		std::shared_ptr<cupcfd::particles::ParticleSystemSimple<M,I,T,L>> particleSystemPtr)
		: Benchmark<I,T>(benchmarkName, repetitions),
		  particleSystemPtr(particleSystemPtr),
		  nTimesteps(nTimesteps)
		{
			this->dtDist = dtDist.clone();
		}

		template <class M, class I, class T, class L>
		BenchmarkParticleSystemSimple<M,I,T,L>::~BenchmarkParticleSystemSimple() {
			delete this->dtDist;
		}

		template <class M, class I, class T, class L>
		void BenchmarkParticleSystemSimple<M,I,T,L>::setupBenchmark() {
			// Nothing to do currently
		}

		template <class M, class I, class T, class L>
		void BenchmarkParticleSystemSimple<M,I,T,L>::recordParameters() {
			// Nothing to do currently
		}

		template <class M, class I, class T, class L>
		cupcfd::error::eCodes BenchmarkParticleSystemSimple<M,I,T,L>::runBenchmark(MPI_Fint custom, int instance_number, struct unit units[], struct locators relative_positions[]) {
			// ToDo: Increasing number of repetitions is currently just
			// a multiplier for the number of timesteps.
			// Need to add a means to reset the Particle System!
					
			cupcfd::error::eCodes status;
			int worldrank;
			MPI_Comm_rank(MPI_COMM_WORLD, &worldrank); 
		
			int cup_unit_num = relative_positions[worldrank].placelocator;
			int unit_count = 0;
			int cup_count = 1;//since units start from 1
			bool found = false;
			while(!found){
				if(units[unit_count].type == 'P' && cup_unit_num == cup_count){
					found=true;
				}else {
					if(units[unit_count].type != 'C'){
						cup_count++;
					}
					unit_count++;
				}
			}

			int coupler_rank = units[unit_count].coupler_ranks[0][0];

			int newrank;
			int internal_rank;
			MPI_Comm mgcfd_comm = MPI_Comm_f2c(custom);
			MPI_Comm_rank(mgcfd_comm, &internal_rank);
			fflush(stdout);
			printf("I'm from CUP, my rank is %d. It would be great if my rank is equal to %d\n", internal_rank, MPI_ROOT);
			if(internal_rank == 0){
				fflush(stdout);
				printf("I am handing comms with coupler unit");
				fflush(stdout);
				MPI_Send(&worldrank, 1, MPI_INT, coupler_rank, 0, MPI_COMM_WORLD);
				printf("I have sent to coupler unit");
				fflush(stdout);
				MPI_Recv(&newrank, 1, MPI_INT, coupler_rank, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				fflush(stdout);
				printf("I have recieved from coupler unit");
			}

			this->startBenchmarkBlock(this->benchmarkName);
			TreeTimerLogParameterInt("Repetitions", this->repetitions);
			this->recordParameters();
			int testvar = coupler_rank;
			for(I i = 0; i < this->repetitions; i++) {
				for(I j = 0; j < this->nTimesteps; j++) {
					// Generate time for next timestep
					T timestep;
					this->dtDist->getValues(&timestep, 1);

					// Advance Particle System by one timestep
					this->startBenchmarkBlock("UpdateParticleTimestep");
					status = particleSystemPtr->updateSystem(timestep, testvar);
					if (status != cupcfd::error::E_SUCCESS) {
						std::cout << "ERROR: updateSystem() failed" << std::endl;
						MPI_Abort(MPI_COMM_WORLD, status);
					}
					this->stopBenchmarkBlock("UpdateParticleTimestep");
				}
			}

			this->stopBenchmarkBlock(this->benchmarkName);

			return cupcfd::error::E_SUCCESS;
		}
	}
}

#endif
