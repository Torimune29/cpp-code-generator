#include <gtest/gtest.h>

#include "cppcodegen.h"

// Tests that don't naturally fit in the headers/.cpp files directly
// can be placed in a tests/*.cpp file. Integration tests are a good example.

TEST(cppcodegenTest, LineSnippet) {
  const std::string line_input = "#include <iostream>";
  const std::string line_input_2 = "#include <string>";
  const std::string line_output_expected = "#include <iostream>\n";
  const std::string line_2_output_expected = "#include <string>\n#include <iostream>\n";

  cppcodegen::Line line;
  cppcodegen::Line line_2;
  line.Add(cppcodegen::line_type_t, line_input);
  line_2.Add(cppcodegen::line_type_t, line_input_2);
  line_2.Add(line);

  EXPECT_EQ(line.Out(), line_output_expected);
  EXPECT_EQ(line_2.Out(), line_2_output_expected);
}

TEST(cppcodegenTest, LineSnippetWithIndent) {
  const std::string line_input = "TESTMACRO";
  const std::string line_input_2 = "TESTMACRO2";
  const std::string line_output_expected = "TESTMACRO\n";
  const std::string line_2_output_expected = "    TESTMACRO2\n    TESTMACRO\n";

  cppcodegen::Line line;
  cppcodegen::Line line_2(cppcodegen::Indent(0, 4));
  line.Add(cppcodegen::line_type_t, line_input);
  line_2.Add(cppcodegen::line_type_t, line_input_2);
  line_2.Add(line);

  EXPECT_EQ(line.Out(), line_output_expected);
  EXPECT_EQ(line_2.Out(), line_2_output_expected);
}
