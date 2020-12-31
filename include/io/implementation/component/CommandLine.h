/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 */

#ifndef CUPCFD_STRUCTURE_COMMAND_LINE_INCLUDE_H
#define CUPCFD_STRUCTURE_COMMAND_LINE_INCLUDE_H

#include <string>
#include <vector>
#include <unordered_map>
#include "Error.h"

namespace cupcfd
{
	namespace structure
	{
		/**
		 * Parse the command line and stores the options and values provided.
		 * This is separated into two components, flags and values.
		 * Flags are identified by any string on the command line proceeded by a
		 * '-' or '--'.
		 * Values are any string that is not proceeded by a dash, and become
		 * associated with the last seen flag that proceeded it on the command line.
		 *
		 * This class can be used to lookup values associated with flags, selected
		 * by the key of flag and position after  flag (zero-indexed).
		 * This class can also take care of type conversion if the appropriate get
		 * method is used.
		 */
		class CommandLine
		{
			public:

				// ===Members ===

				int nOptions;
				std::unordered_map<std::string, std::vector<std::string>> optionsToArgs;

				// Constructors/Deconstructors

				/**
				 *
				 */
				CommandLine(int argc, char ** argv);

				/**
				 *
				 */
				~CommandLine();

				// === Concrete Methods ===

				/**
				 * Parse the command line inputs to the program and store the options and values.
				 *
				 * @param argc The number of arguments on the command line
				 * @param argv The array of space separated strings from the command line.
				 *
				 * @return Nothing
				 */
				void parse(int argc, char ** argv);

				/**
				 * Get the argument at position 'arg' for the flag name provided.
				 * Convert it to a bool (or return an error if an unsuitable flag was provided)
				 *
				 * @param opts The object containing the flags + args
				 * @param flag The name of the flag the args are associated with
				 * @param arg The position of the arg in the arg list of the flag (zero-indexed)
				 * @param result Location to store result in
				 *
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getBoolArg(std::string flag, int arg, bool * result);

				/**
				 * Get the argument at position 'arg' for the flag name provided.
				 * Convert it to a bool (or return an error if an unsuitable flag was provided)
				 *
				 * @param opts The object containing the flags + args
				 * @param flag The name of the flag the args are associated with
				 * @param arg The position of the arg in the arg list of the flag (zero-indexed)
				 * @param result Location to store result in
				 *
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getIntArg(std::string flag, int arg, int * result);

				/**
				 * Get the argument at position 'arg' for the flag name provided.
				 * Convert it to a bool (or return an error if an unsuitable flag was provided)
				 *
				 * @param opts The object containing the flags + args
				 * @param flag The name of the flag the args are associated with
				 * @param arg The position of the arg in the arg list of the flag (zero-indexed)
				 * @param result Location to store result in
				 *
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getDoubleArg(std::string flag, int arg, double * result);

				/**
				 * Get the argument at position 'arg' for the flag name provided.
				 * Arg is zero-indexed.
				 * Convert it to a bool (or return an error if an unsuitable flag was provided)
				 *
				 * @param opts The object containing the flags + args
				 * @param flag The name of the flag the args are associated with
				 * @param arg The position of the arg in the arg list of the flag (zero-indexed)
				 * @param result Location to store result in
				 *
				 * @return An error status indicating the success or failure of the operation
				 * @retval cupcfd::error::E_SUCCESS The method completed successfully
				 * @retval cupcfd::error::E_CMDLINE_ERR_ARG_MISSING_FLAG The flag does not exist in
				 * the provided command line options
				 * @retval cupcfd::error::E_CMDLINE_ERR_ARG_INVALID_ARG The provided arg index is
				 * invalid - it is either negative or there are insufficient arguments to the requested
				 * flag
				 */
				__attribute__((warn_unused_result))
				cupcfd::error::eCodes getStringArg(std::string flag, int arg, std::string * result);
		};
	}
}

#endif
