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

#include "CommandLine.h"
#include <string>
#include <cstring>
#include <iostream>
#include "Error.h"
#include "ArrayDrivers.h"

namespace cupcfd
{
	namespace structure
	{
		CommandLine::CommandLine(int argc, char ** argv)
		{
			this->parse(argc, argv);
		}

		CommandLine::~CommandLine()
		{
			// Nothing to do currently
		}

		void CommandLine::parse(int argc, char ** argv)
		{
			std::string curOpt = "";

			// Skip argv[0]
			for(int i = 1; i < argc; i++)
			{
				// ToDo - Handle case of just a single dash or double dash (i.e. no letters/numbers)

				// Check for a flag option
				// Accept either a single or double dash
				bool singleDash = strlen(argv[i]) > 0 && argv[i][0] == '-';
				bool doubleDash = strlen(argv[i]) > 1 && argv[i][0] == '-' && argv[i][1] == '-';

				if(doubleDash)
				{
					curOpt = std::string(argv[i]);
					curOpt.erase(0, 2);
					this->optionsToArgs[curOpt] = std::vector<std::string>();
					this->nOptions = this->nOptions + 1;
				}
				else if(singleDash)
				{
					curOpt = std::string(argv[i]);
					curOpt.erase(0, 1);
					this->optionsToArgs[curOpt] = std::vector<std::string>();
					this->nOptions = this->nOptions + 1;
				}
				else
				{
					if(curOpt != "")
					{
						// Argument to current option
						this->optionsToArgs[curOpt].push_back(argv[i]);
					}
					else
					{
						// Error - We have an argument without a option flag
						// ToDo: Handle appropriately - this is a warning, not a terminal error
						std::cout << "Warning: Argument " << std::string(argv[i]) << " found without a preceeding flag\n";
					}
				}
			}
		}

		cupcfd::error::eCodes CommandLine::getBoolArg(std::string flag, int arg, bool * result)
		{
			std::string argString;
			cupcfd::error::eCodes err = this->getStringArg(flag, arg, &argString);

			if(err != cupcfd::error::E_SUCCESS)
			{
				return err;
			}

			if(argString == "0")
			{
				*result = false;
			}
			else if(argString == "1")
			{
				*result = true;
			}
			else if(argString == "false")
			{
				*result = false;
			}
			else if(argString == "true")
			{
				*result = true;
			}
			else
			{
				// Error - arg is unsuitable for bool conversion
				DEBUGGABLE_ERROR; return cupcfd::error::E_CMDLINE_ERR_ARG_INVALID_TYPE;
			}

			return cupcfd::error::E_SUCCESS;
		}

		cupcfd::error::eCodes CommandLine::getIntArg(std::string flag, int arg, int * result)
		{
			std::string argString;
			cupcfd::error::eCodes err = this->getStringArg(flag, arg, &argString);

			if(err != cupcfd::error::E_SUCCESS)
			{
				return err;
			}

			try
			{
				*result = std::stoi(argString, NULL);
			}
			catch(std::invalid_argument e)
			{
				DEBUGGABLE_ERROR; return cupcfd::error::E_CMDLINE_ERR_ARG_INVALID_TYPE;
			}

			return cupcfd::error::E_SUCCESS;
		}

		cupcfd::error::eCodes CommandLine::getDoubleArg(std::string flag, int arg, double * result)
		{
			std::string argString;
			cupcfd::error::eCodes err = this->getStringArg(flag, arg, &argString);

			if(err != cupcfd::error::E_SUCCESS)
			{
				return err;
			}

			try
			{
				*result = std::stod(argString, NULL);
			}
			catch(std::invalid_argument e)
			{
				DEBUGGABLE_ERROR; return cupcfd::error::E_CMDLINE_ERR_ARG_INVALID_TYPE;
			}

			return cupcfd::error::E_SUCCESS;
		}

		cupcfd::error::eCodes CommandLine::getStringArg(std::string flag, int arg, std::string * result)
		{
			// Error Check: Does the flag exist?
			std::unordered_map<std::string, std::vector<std::string>>::const_iterator search = this->optionsToArgs.find(flag);
			if(search == this->optionsToArgs.end())
			{
				// Error - Not Found
				*result = "";
				DEBUGGABLE_ERROR; return cupcfd::error::E_CMDLINE_ERR_ARG_MISSING_FLAG;
			}

			// Error Check: Does the flag have sufficient arguments for index arg
			int size = cupcfd::utility::drivers::safeConvertSizeT<int>(this->optionsToArgs[flag].size());
			if(arg >= size) {
				// Error - arg value too high
				*result = "";
				DEBUGGABLE_ERROR; return cupcfd::error::E_CMDLINE_ERR_ARG_INVALID_ARG;
			}

			// Try to convert value
			std::string argString = this->optionsToArgs[flag][arg];
			*result = argString;

			return cupcfd::error::E_SUCCESS;
		}
	}
}
