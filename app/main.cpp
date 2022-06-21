// This is the test app for cppcodegen

#include <iostream>
#include <string>

#include "cppcodegen.h"

int main(int /*argc*/, char** /*argv[]*/) {
  // Basic Usage
  cppcodegen::Snippet file_header(cppcodegen::line_t);                   // line as file header
  cppcodegen::Snippet s_system_include(cppcodegen::system_include_t);    // system include directive
  cppcodegen::Snippet s_local_include(cppcodegen::local_include_t, "");  // local include directive

  file_header << "// This is the test app for cppcodegen";  // add line
  s_system_include << "iostream"
                   << "string";       // add system include header
  s_local_include << "cppcodegen.h";  // add local include header

  file_header << s_system_include << "" << s_local_include << "";

  std::cout << file_header.Out() << "------" << std::endl;
  /* std out is ...
  // This is the test app for cppcodegen
  #include <iostream>
  #include <string>

  #include "cppcodegen.h"

  */

  cppcodegen::Snippet file_pre_main(cppcodegen::line_t);  // line as file pre main
  cppcodegen::Block s_function(cppcodegen::definition_t,
                               "void Print(const std::string &str)");  // block as function declaration/definition
  s_function << "str_ = str;"
             << "std::cout << str_ << std::endl";  // add function body
  cppcodegen::Class s_class("Test");               // Class
  s_class << cppcodegen::AccessSpecifier::kPublic << "Test() = default;"
          << s_function;                                                    // add public member with function
  s_class << cppcodegen::AccessSpecifier::kPrivate << "std::string str_;";  // add private member

  cppcodegen::Block s_namespace(cppcodegen::namespace_t, "TestNamespace");  // block as namespace
  s_namespace << s_class;                                                   // add class into namespace
  file_pre_main << s_namespace << "";

  std::cout << file_pre_main.Out() << "------" << std::endl;
  /* std out is ...
  namespace TestNamespace {
    class Test {
     public:
      Test() = default;
      void Print(const std::string &str) {
        str_ = str;
        std::cout << str_ << std::endl
      }
     private:
      std::string str_;
    };
  }

  */

  cppcodegen::Block main_context(cppcodegen::definition_t,
                                 "int main(int argc, char** argv[])");  // main context codeblock
  main_context.IncrementIndent();                                       // add indent
  main_context << "return 0";
  std::cout << main_context.Out() << "------" << std::endl;
  /* std out is ...
    int main(int argc, char** argv[]) {
    return 0
    }
  */

  // Custom Indent
  // snippet, block, and class supports custom indent level and spaces
  cppcodegen::Block main_context_more_indent(
      cppcodegen::definition_t, "int main(int argc, char** argv[])",
      cppcodegen::Indent(2, 4));  // this is indent level 1 (default: 0), indent 4 spaces (default: 2)
  std::cout << main_context_more_indent.Out() << "------" << std::endl;
  /* std out is ...
          int main(int argc, char** argv[]) {
          }
  */
  return 0;
}
