#pragma once

#include <boost/program_options.hpp>

class InitParameter;

boost::program_options::variables_map setupCommandLine(int argc, char* argv[], boost::program_options::options_description& desc);
void mainLoop();
void init(InitParameter* parameter);
void printHelp(boost::program_options::options_description& desc);
