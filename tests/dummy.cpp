#include <gtest/gtest.h>

#include "cppcodegen.h"

// Tests that don't naturally fit in the headers/.cpp files directly
// can be placed in a tests/*.cpp file. Integration tests are a good example.

TEST(cppcodegenTest, LineSnippet) {
  const std::string line_input = "#include <iostream>";
  const std::string line_input_2 = "#include <string>";
  const std::string line_output_expected = "#include <iostream>\n";
  const std::string line_2_output_expected = "#include <string>\n#include <iostream>\n";

  cppcodegen::Snippet line(cppcodegen::line_t);
  cppcodegen::Snippet line_2(cppcodegen::line_t);
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

  cppcodegen::Snippet line(cppcodegen::line_t);
  cppcodegen::Snippet line_2(cppcodegen::line_t, cppcodegen::Indent(1, 4));
  line.Add(line_input);
  line_2.Add(line_input_2);
  line_2.Add(line);

  EXPECT_EQ(line.Out(), line_output_expected);
  EXPECT_EQ(line_2.Out(), line_2_output_expected);
}

TEST(cppcodegenTest, IncludeSnippetSystemLocalMix) {
  const std::string include_input_1_1 = "sys/types.h";
  const std::string include_input_1_2 = "stddef.h";
  const std::string include_input_2 = "test.h";
  const std::string include_input_2_base_dir = "../";
  const std::string include_1_output_expected = "#include <sys/types.h>\n#include <stddef.h>\n";
  const std::string include_2_output_expected = "#include \"../test.h\"\n#include <sys/types.h>\n#include <stddef.h>\n";

  cppcodegen::Snippet include_1(cppcodegen::system_include_t);
  cppcodegen::Snippet include_2(cppcodegen::local_include_t, include_input_2_base_dir);
  cppcodegen::Snippet include_1_2(cppcodegen::system_include_t);
  // line.Add(include_input_1_1);  // compile error
  include_1.Add(include_input_1_1);
  include_1.Add(include_input_1_2);
  include_2.Add(include_input_2);
  include_2.Add(include_1);
  include_1_2.Add({include_input_1_1, include_input_1_2});

  EXPECT_EQ(include_1.Out(), include_1_output_expected);
  EXPECT_EQ(include_2.Out(), include_2_output_expected);
  EXPECT_EQ(include_1.Out(), include_1_2.Out());
}

TEST(cppcodegenTest, SnippetIncrementIndent) {
  const std::string line_input = "TESTMACRO";
  const std::string line_input_2 = "TESTMACRO2";
  const std::string line_output_expected = "  TESTMACRO\n";
  const std::string line_2_output_expected = "            TESTMACRO2\n              TESTMACRO\n";

  cppcodegen::Snippet line(cppcodegen::line_t);
  cppcodegen::Snippet line_2(cppcodegen::line_t, cppcodegen::Indent(1, 4));
  line.Add(line_input);
  line.IncrementIndent();
  line_2.Add(line_input_2);
  line_2.Add(line);
  line_2.IncrementIndent(2);

  EXPECT_EQ(line.Out(), line_output_expected);
  EXPECT_EQ(line_2.Out(), line_2_output_expected);
}

TEST(cppcodegenTest, Block) {
  const std::string block_line_1 = "A *a = new A;";
  const std::string block_line_2 = "B b = a.foo();";
  const std::string block_expected = R"({
  A *a = new A;
  B b = a.foo();
}
)";
  const std::string block_expected_indented = R"(  {
    A *a = new A;
    B b = a.foo();
  }
)";
  cppcodegen::Block block(cppcodegen::code_block_t);
  block.Add(block_line_1);
  block.Add(block_line_2);

  EXPECT_EQ(block.Out(), block_expected);
  block.IncrementIndent();
  EXPECT_EQ(block.Out(), block_expected_indented);
}

TEST(cppcodegenTest, BlockIntoBlock) {
  const std::string block_line_1 = "A *a = new A;";
  const std::string block_line_2 = "B b = a.foo();";
  const std::string block_expected = R"({
  {
    A *a = new A;
    B b = a.foo();
  }
}
)";
  const std::string block_expected_indented = R"(  {
    {
      A *a = new A;
      B b = a.foo();
    }
  }
)";
  cppcodegen::Block block(cppcodegen::code_block_t);
  cppcodegen::Block block_2(cppcodegen::code_block_t);
  block.Add(block_line_1);
  block.Add(block_line_2);
  block_2.Add(block);

  EXPECT_EQ(block_2.Out(), block_expected);
  block_2.IncrementIndent();
  EXPECT_EQ(block_2.Out(), block_expected_indented);
}

TEST(cppcodegenTest, BlockAddSnippet) {
  const std::string snippet_line_1 = "A *a = new A;";
  const std::string snippet_line_2 = "B b = a.foo();";
  const std::string block_expected = R"({
  A *a = new A;
  B b = a.foo();
}
)";
  const std::string block_expected_indented = R"(  {
    A *a = new A;
    B b = a.foo();
  }
)";
  cppcodegen::Snippet line(cppcodegen::line_t);
  cppcodegen::Block block(cppcodegen::code_block_t);
  line.Add({snippet_line_1, snippet_line_2});
  block.Add(line);

  EXPECT_EQ(block.Out(), block_expected);
  block.IncrementIndent();
  EXPECT_EQ(block.Out(), block_expected_indented);
}

TEST(cppcodegenTest, BlockAsNamespace) {
  const std::string snippet_line_1 = "A Test();";
  const std::string snippet_line_2 = "void Foo(int a);";
  const std::string namespace_expected = R"(namespace Test {
  A Test();
  void Foo(int a);
}
)";
  const std::string namespace_expected_combined = R"(namespace Test {
  A Test();
  void Foo(int a);
  namespace TestNested {
    A Test();
  }
}
)";
  cppcodegen::Snippet line(cppcodegen::line_t);
  cppcodegen::Block block_namespace_1(cppcodegen::namespace_t, "Test");
  cppcodegen::Block block_namespace_2(cppcodegen::namespace_t, "TestNested");
  line.Add({snippet_line_1, snippet_line_2});
  block_namespace_1.Add(line);
  block_namespace_2.Add(snippet_line_1);

  EXPECT_EQ(block_namespace_1.Out(), namespace_expected);
  block_namespace_1.Add(block_namespace_2);
  EXPECT_EQ(block_namespace_1.Out(), namespace_expected_combined);
}

TEST(cppcodegenTest, BlockAsDefinition) {
  const std::string snippet_declaration = "A Test()";
  const std::string snippet_definition = "return A;";
  const std::string definition_expected = R"(A Test() {
  return A;
}
)";
  const std::string namespace_definition_expected = R"(namespace Test {
  A Test() {
    return A;
  }
}
)";
  cppcodegen::Snippet line(cppcodegen::line_t);
  cppcodegen::Block block_definition(cppcodegen::definition_t, snippet_declaration);
  cppcodegen::Block block_namespace(cppcodegen::namespace_t, "Test");
  line.Add(snippet_definition);
  block_definition.Add(line);
  block_namespace.Add(block_definition);

  EXPECT_EQ(block_definition.Out(), definition_expected);
  EXPECT_EQ(block_namespace.Out(), namespace_definition_expected);
}

TEST(cppcodegenTest, Class) {
  const std::string class_name = "TestClass";
  const std::string private_member = "char c;";
  const std::string protected_member = "int b;";
  const std::string public_member = "TestClass();";
  const std::string block_expected = R"(class TestClass {
 public:
  TestClass();
 protected:
  int b;
 private:
  char c;
}
)";
  const std::string block_expected_indented = R"(  class TestClass {
   public:
    TestClass();
   protected:
    int b;
   private:
    char c;
  }
)";
  cppcodegen::Class class_block("TestClass");
  class_block.Add(private_member, cppcodegen::AccessSpecifier::kPrivate);
  class_block.Add(protected_member, cppcodegen::AccessSpecifier::kProtected);
  class_block.Add(public_member, cppcodegen::AccessSpecifier::kPublic);

  EXPECT_EQ(class_block.Out(), block_expected);
  class_block.IncrementIndent();
  EXPECT_EQ(class_block.Out(), block_expected_indented);
}
