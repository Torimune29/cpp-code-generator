// Executables must have the following defined if the library contains
// doctest definitions. For builds with this disabled, e.g. code shipped to
// users, this can be left out.
#include <cstdlib>
#include <iostream>
#include <string>

#include "CLI/CLI.hpp"
#include "ProjectInformation.h"
#include "example.h"

/*
 * Simple main program that demontrates how access
 * CMake definitions (here the version number) from source code.
 */
int main(int argc, char *argv[]) {
  std::cout << PROJECT_NAME_VERSION << std::endl;

  CLI::App app{"Sample App description"};
  std::string filename = "../LICENSE";
  app.add_option("-f,--file", filename, "Path");
  CLI11_PARSE(app, argc, argv)
  // Bring in the dummy class from the example source,
  // just to show that it is accessible from main.cpp.
  return Dummy::DoSomething() ? 0 : -1;
}
