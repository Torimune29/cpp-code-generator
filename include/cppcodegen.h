#pragma once

namespace cppcodegen {
class Dummy {
 public:
  /**
   * Default constructor for Dummy (does nothing).
   */
  Dummy() = default;
  /**
   * Returns a bool.
   * @return Always True.
   */
  static bool DoSomething() { return true; }
};

}  // namespace cppcodegen
