/**
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 */

#ifndef __BUILD_H__
#define __BUILD_H__

#include <string>

class Build {
	private:
		int versionMajor;
		int versionMinor;

	public:
		Build();
		int getVersionMajor();
		int getVersionMinor();
		std::string getCXXFlags();
};

#endif
