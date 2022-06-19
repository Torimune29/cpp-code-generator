// Executables must have the following defined if the library contains
// doctest definitions. For builds with this disabled, e.g. code shipped to
// users, this can be left out.
#include <cstdlib>
#include <iostream>
#include <string>

#include "ProjectInformation.h"
#include "cppcodegen.h"

/**
 * @brief for header warning check only
 *
 */
int main(int /*argc*/, char** /*argv[]*/) {
  std::cout << PROJECT_NAME_VERSION << std::endl;
  cppcodegen::Snippet line(cppcodegen::line_t);

  return 0;
}
