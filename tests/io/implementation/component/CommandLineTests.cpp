/*
 * @file
 * @author University of Warwick
 * @version 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 * Tests for the CommandLine object, responsible for
 * parsing input on the command line
 *
 */

#define BOOST_TEST_MODULE CommandLine
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <stdexcept>
#include "CommandLine.h"
#include <string>

using namespace cupcfd::structure;

// === Constructor/Deconstructor ===
// Test 1: Check the constructor runs without crashing
BOOST_AUTO_TEST_CASE(constructor_test1)
{
    // Rather than providing command line args to the test binary,
    // we will hardcode argc and argv inside the tests

    int argc = 1;
    char argv[1][10] = {{"binName"}};
    CommandLine opts(argc, (char**) argv);
}

// === parse ===
// Test 1: Parse is triggered by the constructor:
// Check the internal contents
// Test 1:
BOOST_AUTO_TEST_CASE(parse_test1)
{
	cupcfd::error::eCodes status;

    int argc = 3;

    char ** argv = (char **) malloc(sizeof(char *) * 3);
    for(int i = 0; i < 3; i++)
    {
    	argv[i] = (char *) malloc(sizeof(char) * 10);
    }
    argv[0] = "binName";
    argv[1] = "--test";
    argv[2] = "4";

    CommandLine opts(argc, argv);
    BOOST_CHECK_EQUAL(opts.nOptions, 1);
    BOOST_CHECK_EQUAL(opts.optionsToArgs.size(), 1);
}

// === getStringArg ===
BOOST_AUTO_TEST_CASE(getStringArg_test1)
{
	cupcfd::error::eCodes status;

    int argc = 3;

    char ** argv = (char **) malloc(sizeof(char *) * 3);
    for(int i = 0; i < 3; i++)
    {
    	argv[i] = (char *) malloc(sizeof(char) * 10);
    }
    argv[0] = "binName";
    argv[1] = "--test";
    argv[2] = "abcde";

    CommandLine opts(argc, argv);

    std::string result;
    status = opts.getStringArg("test", 0, &result);
    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
    BOOST_CHECK_EQUAL(result, "abcde");
}

BOOST_AUTO_TEST_CASE(getStringArg_test2)
{
	cupcfd::error::eCodes status;

    int argc = 4;

    char ** argv = (char **) malloc(sizeof(char *) * 4);
    for(int i = 0; i < 4; i++)
    {
    	argv[i] = (char *) malloc(sizeof(char) * 10);
    }
    argv[0] = "binName";
    argv[1] = "--test";
    argv[2] = "abcde";
    argv[3] = "fghijk";

    CommandLine opts(argc, argv);

    std::string result;
    status = opts.getStringArg("test", 1, &result);
    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
    BOOST_CHECK_EQUAL(result, "fghijk");
}

BOOST_AUTO_TEST_CASE(getStringArg_test3)
{
	cupcfd::error::eCodes status;

    int argc = 4;

    char ** argv = (char **) malloc(sizeof(char *) * 4);
    for(int i = 0; i < 4; i++)
    {
    	argv[i] = (char *) malloc(sizeof(char) * 10);
    }
    argv[0] = "binName";
    argv[1] = "--test";
    argv[2] = "abcde";
    argv[3] = "fghijk";

    CommandLine opts(argc, argv);

    std::string result;
    status = opts.getStringArg("missing", 0, &result);
    BOOST_CHECK_EQUAL(status, cupcfd::error::E_CMDLINE_ERR_ARG_MISSING_FLAG);
}

BOOST_AUTO_TEST_CASE(getStringArg_test4)
{
	cupcfd::error::eCodes status;

    int argc = 4;

    char ** argv = (char **) malloc(sizeof(char *) * 4);
    for(int i = 0; i < 4; i++)
    {
    	argv[i] = (char *) malloc(sizeof(char) * 10);
    }
    argv[0] = "binName";
    argv[1] = "--test";
    argv[2] = "abcde";
    argv[3] = "fghijk";

    CommandLine opts(argc, argv);

    std::string result;
    status = opts.getStringArg("test", 3, &result);
    BOOST_CHECK_EQUAL(status, cupcfd::error::E_CMDLINE_ERR_ARG_INVALID_ARG);
}

// === getBoolArg ===
// Test 1: Retrieve a boolean argument from a flag with only one argument, true
BOOST_AUTO_TEST_CASE(getBoolArg_test1)
{
	cupcfd::error::eCodes status;

    int argc = 3;

    char ** argv = (char **) malloc(sizeof(char *) * 3);
    for(int i = 0; i < 3; i++)
    {
    	argv[i] = (char *) malloc(sizeof(char) * 10);
    }
    argv[0] = "binName";
    argv[1] = "--test";
    argv[2] = "true";

    CommandLine opts(argc, argv);

    bool result;
    status = opts.getBoolArg("test", 0, &result);
    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
    BOOST_CHECK_EQUAL(result, true);
}

// Test 2: Retrieve a boolean argument from a flag with only one argument, false
BOOST_AUTO_TEST_CASE(getBoolArg_test2)
{
	cupcfd::error::eCodes status;

    int argc = 3;

    char ** argv = (char **) malloc(sizeof(char *) * 3);
    for(int i = 0; i < 3; i++)
    {
    	argv[i] = (char *) malloc(sizeof(char) * 10);
    }
    argv[0] = "binName";
    argv[1] = "--test";
    argv[2] = "false";

    CommandLine opts(argc, argv);

    bool result;
    status = opts.getBoolArg("test", 0, &result);
    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
    BOOST_CHECK_EQUAL(result, false);
}

// Test 2: Retrieve a boolean argument from a flag with only one argument, 1
BOOST_AUTO_TEST_CASE(getBoolArg_test3)
{
	cupcfd::error::eCodes status;

    int argc = 3;

    char ** argv = (char **) malloc(sizeof(char *) * 3);
    for(int i = 0; i < 3; i++)
    {
    	argv[i] = (char *) malloc(sizeof(char) * 10);
    }
    argv[0] = "binName";
    argv[1] = "--test";
    argv[2] = "1";

    CommandLine opts(argc, argv);

    bool result;
    status = opts.getBoolArg("test", 0, &result);
    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
    BOOST_CHECK_EQUAL(result, true);
}

// Test 2: Retrieve a boolean argument from a flag with only one argument, 0
BOOST_AUTO_TEST_CASE(getBoolArg_test4)
{
	cupcfd::error::eCodes status;

    int argc = 3;

    char ** argv = (char **) malloc(sizeof(char *) * 3);
    for(int i = 0; i < 3; i++)
    {
    	argv[i] = (char *) malloc(sizeof(char) * 10);
    }
    argv[0] = "binName";
    argv[1] = "--test";
    argv[2] = "0";

    CommandLine opts(argc, argv);

    bool result;
    status = opts.getBoolArg("test", 0, &result);
    BOOST_CHECK_EQUAL(status, cupcfd::error::E_SUCCESS);
    BOOST_CHECK_EQUAL(result, false);
}

// Test 2: Retrieve a boolean argument from a flag with only one argument, non-boolean
BOOST_AUTO_TEST_CASE(getBoolArg_test5)
{
	cupcfd::error::eCodes status;

    int argc = 3;

    char ** argv = (char **) malloc(sizeof(char *) * 3);
    for(int i = 0; i < 3; i++)
    {
    	argv[i] = (char *) malloc(sizeof(char) * 10);
    }
    argv[0] = "binName";
    argv[1] = "--test";
    argv[2] = "abc";

    CommandLine opts(argc, argv);

    bool result;
    status = opts.getBoolArg("test", 0, &result);
    BOOST_CHECK_EQUAL(status, cupcfd::error::E_CMDLINE_ERR_ARG_INVALID_TYPE);
}


// === getIntArg ===


// === getDoubleArg ===

