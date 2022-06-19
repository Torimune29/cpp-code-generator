#include <gtest/gtest.h>

#include "cppcodegen.h"

// Tests that don't naturally fit in the headers/.cpp files directly
// can be placed in a tests/*.cpp file. Integration tests are a good example.

TEST(cppcodegenTest, LineSnippet) {
  const std::string line_input = "#include <iostream>";
  const std::string line_input_2 = "#include <string>";
  const std::string line_output_expected = "#include <iostream>\n";
  const std::string line_2_output_expected = "#include <string>\n#include <iostream>\n";

  cppcodegen::Line line(cppcodegen::line_t);
  cppcodegen::Line line_2(cppcodegen::line_t);
  line.Add(line_input);
  line_2.Add(line_input_2);
  line_2.Add(line);

  EXPECT_EQ(line.Out(), line_output_expected);
  EXPECT_EQ(line_2.Out(), line_2_output_expected);
}

TEST(cppcodegenTest, LineSnippetWithIndent) {
  const std::string line_input = "TESTMACRO";
  const std::string line_input_2 = "TESTMACRO2";
  const std::string line_output_expected = "TESTMACRO\n";
  const std::string line_2_output_expected = "    TESTMACRO2\n    TESTMACRO\n";

  cppcodegen::Line line(cppcodegen::line_t);
  cppcodegen::Line line_2(cppcodegen::line_t, cppcodegen::Indent(0, 4));
  line.Add(line_input);
  line_2.Add(line_input_2);
  line_2.Add(line);

  EXPECT_EQ(line.Out(), line_output_expected);
  EXPECT_EQ(line_2.Out(), line_2_output_expected);
}

TEST(cppcodegenTest, IncludeSnippet) {
  const std::string include_input_1_1 = "sys/types.h";
  const std::string include_input_1_2 = "stddef.h";
  const std::string include_input_2 = "test.h";
  const std::string include_input_2_base_dir = "../";
  const std::string include_1_output_expected = "#include <sys/types.h>\n#include <stddef.h>\n";
  const std::string include_2_output_expected = "#include \"../test.h\"\n#include <sys/types.h>\n#include <stddef.h>\n";

  cppcodegen::SystemInclude include_1(cppcodegen::system_include_t);
  cppcodegen::LocalInclude include_2(cppcodegen::local_include_t, include_input_2_base_dir);
  // line.Add(include_input_1_1);  // compile error
  include_1.Add(include_input_1_1);
  include_1.Add(include_input_1_2);
  include_2.Add(include_input_2);
  include_2.Add(include_1);

  EXPECT_EQ(include_1.Out(), include_1_output_expected);
  EXPECT_EQ(include_2.Out(), include_2_output_expected);
}
