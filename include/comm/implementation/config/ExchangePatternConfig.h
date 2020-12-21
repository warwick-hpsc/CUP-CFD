/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Contains declarations for the ExchangePatternConfig class
 */

#ifndef CUPCFD_COMM_EXCHANGEPATTERN_CONFIG_INCLUDE_H
#define CUPCFD_COMM_EXCHANGEPATTERN_CONFIG_INCLUDE_H

#include "ExchangePattern.h"
#include "DistributedAdjacencyList.h"

namespace cupcfd
{
	namespace comm
	{
		/**
		 * An identifier for selecting an exchange method to use on
		 * with an exchange pattern.
		 */
		enum ExchangeMethod
		{
			EXCHANGE_NONBLOCKING_ONE_SIDED,
			EXCHANGE_NONBLOCKING_TWO_SIDED
		};


		/**
		 * Defines the configuration parameters for setting up
		 * an ExchangePattern object. This can be used for with a builder.
		 *
		 * This currently includes:
		 * (a) The Exchange Method.
		 */
		class ExchangePatternConfig
		{
			public:
				// === Members ===

				/** Identifier for which exchange method to use **/
				ExchangeMethod method;

				// === Constructors/Deconstructors ===

				/**
				 * Default Constructor.
				 * Uses following defaults:
				 * method = EXCHANGE_NONBLOCKING_TWO_SIDED
				 *
				 */
				ExchangePatternConfig();

				/**
				 * Constructor.
				 * Sets configuration to provided values.
				 *
				 * @param method The Exchange method to use with this pattern.
				 */
				ExchangePatternConfig(ExchangeMethod method);

				/**
				 * Constructor.
				 * Creates a configuration that has a copy of the values in
				 * the provided configuration.
				 *
				 * @param config The configuration to copy.
				 */
				ExchangePatternConfig(ExchangePatternConfig& config);

				/**
				 * Deconstructor:
				 * Cleanup any dynamically allocated memory.
				 */
				virtual ~ExchangePatternConfig();

				// === Concrete Methods ===

				/**
				 * Retrieve the identifier for which exchange method to use.
				 *
				 * @return The exchange method identifier.
				 */
				ExchangeMethod getExchangeMethod();

				/**
				 * Set the identifier for which exchange method to use.
				 *
				 * @param method The exchange method identifier.
				 *
				 * @return Nothing
				 */
				void setExchangeMethod(ExchangeMethod method);

				/**
				 * Deep copy from source to this config
				 *
				 * @param source The source configuration to copy from.
				 *
				 * @return Nothing.
				 */
				void operator=(const ExchangePatternConfig& source);

				/**
				 * Returns a pointer to a new cloned copy of this object.
				 *
				 * @return A pointer to a cloned copy of this object
				 */
				inline ExchangePatternConfig * clone();

				/**
				 * Builds an ExchangePattern object, and sets up the exchange pattern
				 * based on the exchange of ghost cell data between the distributed regions of the graph.
				 * Note: This requires the graph to be finalized, since it requires full knowledge of the allocation
				 * of local and ghost nodes.
				 *
				 * @param pattern A pointer to where the pointer for the newly created ExchangePattern will be stored.
				 * @param list The distributed graph to source the connectivity data from for ghost data.
				 *
				 * @tparam I The type of the indexing scheme
				 * @tparam T The type of the data to be exchanged.
				 * @tparam N The type of the nodes in the graph
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 */
				template <class I, class T, class N>
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes buildExchangePattern(ExchangePattern<T>** pattern, cupcfd::data_structures::DistributedAdjacencyList<I, N>& graph);

				// === Pure Virtual Methods ===
		};
	}
}

// Include Header Level Definitions
#include "ExchangePatternConfig.ipp"

#endif
