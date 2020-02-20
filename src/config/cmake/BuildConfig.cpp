/*
#############################################################################
# License:
# File Name: Build.cpp
# File Description:
# Author: University of Warwick
# Comments:
#############################################################################
*/

#include "BuildConfig.h"
#include "cmake_settings.h"
#include <string>

Build::Build()
{
	versionMajor = CUPCFD_VERSION_MAJOR;
	versionMinor = CUPCFD_VERSION_MINOR;
}

int Build::getVersionMajor()
{
	return this->versionMajor;
}

int Build::getVersionMinor()
{
	return this->versionMinor;
}

std::string Build::getCXXFlags()
{
	return CXXFLAGS;
}

