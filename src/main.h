#pragma once

#include <boost/program_options.hpp>

class Configuration;

boost::program_options::variables_map setupCommandLine(int argc, const char* argv[], boost::program_options::options_description& desc);
void mainLoop();
void init(Configuration* parameter);
void printHelp(boost::program_options::options_description& desc);
