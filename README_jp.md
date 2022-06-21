
# cppcodegen

ヘッダオンリーのC++11用コードスニペットジェネレータ

![GitHub Workflow Status](https://img.shields.io/github/workflow/status/Torimune29/cppcodegen/CI)
[![codecov](https://codecov.io/gh/Torimune29/cppcodegen/branch/main/graph/badge.svg)](https://codecov.io/gh/Torimune29/cppcodegen)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/4ab150dd86c44db9ba17df846aa309a3)](https://www.codacy.com/gh/Torimune29/cppcodegen/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Torimune29/cppcodegen&amp;utm_campaign=Badge_Grade)
![CodeQL](https://github.com/Torimune29/cppcodegen/workflows/CodeQL/badge.svg)
<br>[Github Repo](https://github.com/Torimune29/cppcodegen)

## 特徴

- ヘッダオンリー
- C++11標準ライブラリにのみ依存
- スニペット対応
  - 任意の行 スニペット
    - マクロ
    - インクルード
  - 任意のコードブロック
    - 名前空間
    - 関数定義
  - 任意のクラス
- 各スニペットの手動・インデント
- 各スニペット追加時の自動インデント
  - ブロックにスニペット・ブロック・クラスを追加したとき
  - クラスにスニペット・ブロック・クラスを追加したとき

## 例

詳細はこちらを参照：[unit tests](https://github.com/Torimune29/cppcodegen/blob/main/tests/unit_tests_cppcodegen.cpp)

## 使い方

## インストール方法

include/cppcodegen.h をダウンロード・インクルードするだけ

## 使用方法

### スニペット

```cpp
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
 ```

### ブロックとクラス

```cpp
  cppcodegen::Snippet file_pre_main(cppcodegen::line_t);  // line as file pre main
  cppcodegen::Block s_function(cppcodegen::definition_t,
                               "void Print(const std::string &str)");  // block as function declaration/definition
  s_function << "str_= str;"
             << "std::cout << str_ << std::endl";  // add function body

  cppcodegen::Class s_class("Test");               // Class
  s_class << cppcodegen::AccessSpecifier::kPublic << "Test() = default;"
          << s_function;                                                    // add public member function and snippet
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

```

### インデント制御

```cpp
  cppcodegen::Block main_context(cppcodegen::definition_t,
                                 "int main(int argc, char**argv[])");  // main context codeblock
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
      cppcodegen::definition_t, "int main(int argc, char**argv[])",
      cppcodegen::Indent(2, 4));  // this is indent level 1 (default: 0), indent 4 spaces (default: 2)
  std::cout << main_context_more_indent.Out() << "------" << std::endl;
  /* std out is ...
        int main(int argc, char** argv[]) {
        }
  */
```
