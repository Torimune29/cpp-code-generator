
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

### スニペット

```cpp
#include "cppcodegen.h"

cppcodegen::Snippet snippet_include(cppcodegen::system_include_t);  // include

cppcodegen::Snippet line(cppcodegen::line_t);
```
