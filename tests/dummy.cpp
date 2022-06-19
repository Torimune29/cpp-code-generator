#include <gtest/gtest.h>

#include "cppcodegen.h"

// Tests that don't naturally fit in the headers/.cpp files directly
// can be placed in a tests/*.cpp file. Integration tests are a good example.

TEST(ComplicatedIntegrationTestsCouldBeHere, dummy) {
  cppcodegen::Dummy instance;
  EXPECT_TRUE(instance.DoSomething());
}
