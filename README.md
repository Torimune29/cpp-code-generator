
# cppcodegen

![GitHub Workflow Status](https://img.shields.io/github/workflow/status/Torimune29/cppcodegen/CI)
[![codecov](https://codecov.io/gh/Torimune29/cppcodegen/branch/main/graph/badge.svg)](https://codecov.io/gh/Torimune29/cppcodegen)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/4ab150dd86c44db9ba17df846aa309a3)](https://www.codacy.com/gh/Torimune29/cppcodegen/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Torimune29/cppcodegen&amp;utm_campaign=Badge_Grade)
![CodeQL](https://github.com/Torimune29/cppcodegen/workflows/CodeQL/badge.svg)
<br>[Github Repo](https://github.com/Torimune29/cppcodegen)

## Features(WIP)

- header-only cpp code snippet generating
  - separating 3 parts
    - header
      - includes
    - body
      - code blocks
        - scopes (like global, anonymous, naming or class)
          - auto indent
          - auto close braces
    - footer
      - include guards
  - freestyle line writing (for macro) on any parts
