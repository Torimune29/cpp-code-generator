#pragma once
#include <iostream>
#include <sstream>
#include <vector>

namespace cppcodegen {

const std::size_t kDefaultIndentSize = 2;

typedef struct LineType {
  explicit LineType() = default;
} LineType;
typedef struct SystemIncludeType {
  explicit SystemIncludeType() = default;
} SystemIncludeType;
typedef struct LocalIncludeType {
  explicit LocalIncludeType() = default;
} LocalIncludeType;
typedef struct DefinitionType {
  explicit DefinitionType() = default;
} DefinitionType;

constexpr LineType line_t{};
constexpr SystemIncludeType system_include_t{};
constexpr LocalIncludeType local_include_t{};
constexpr DefinitionType definition_t{};

enum class SnippetType { kLine, kSystemInclude, kLocalInclude, kDefinition };

/**
 * @brief Indent information holder
 *
 */
typedef struct Indent {
  Indent(std::size_t level, std::size_t size, char character = ' ')
      : level_(level), size_(size), character_(character) {
    std::size_t index = 0;
    while (index < size_) {
      indent_string_ += character_;
      index++;
    }
  }
  std::size_t level_;
  std::size_t size_;
  char character_;
  std::string indent_string_;

  std::string Indenting() const noexcept {
    std::string total_indent;
    std::size_t level = 0;
    while (level < level_) {
      total_indent += indent_string_;
      level++;
    }
    return total_indent;
  }
} Indent;

/**
 * @brief Snippet
 *
 */
class Snippet {
 public:
  /**
   * @brief Construct a new Snippet object as Line
   *
   * @param indent
   */
  Snippet(LineType, const Indent &indent = Indent(0, kDefaultIndentSize))
      : indent_(indent), header_(), footer_(), type_(SnippetType::kLine) {}
  /**
   * @brief Construct a new Snippet object as system include
   *
   * @param indent
   */
  Snippet(SystemIncludeType, const Indent &indent = Indent(0, kDefaultIndentSize))
      : indent_(indent), header_("#include <"), footer_(">"), type_(SnippetType::kSystemInclude) {}
  /**
   * @brief Construct a new Snippet object as local include
   *
   * @param base_dir_path include relative base path
   * @param indent
   */
  Snippet(LocalIncludeType, const std::string base_dir_path, const Indent &indent = Indent(0, kDefaultIndentSize))
      : indent_(indent), header_("#include \"" + base_dir_path), footer_("\""), type_(SnippetType::kLocalInclude) {}
  ~Snippet() = default;

  /**
   * @brief Out with own indent
   *
   * @return std::string
   */
  std::string Out() const noexcept {
    std::string snippet;
    for (const auto &line : lines_) {
      snippet += indent_.Indenting() + line + "\n";
    }
    return snippet;
  }

  SnippetType Type() const noexcept { return type_; }

  void Add(const std::string &line) noexcept {
    lines_.emplace_back(header_ + line + footer_);
    return;
  }

  /**
   * @brief Add from other snippet
   *
   * @param snippet
   * @details
   * add without header and footer.
   */
  void Add(const Snippet &snippet) noexcept {
    std::stringstream line_stream(snippet.Out());
    std::string line;
    while (std::getline(line_stream, line)) {
      lines_.emplace_back(line);
    }
    return;
  }

  void Add(const std::vector<std::string> &lines) noexcept {
    for (const auto &line : lines) {
      Add(line);
    }
    return;
  }

  void IncrementIndent(std::size_t size = 1) noexcept {
    indent_.level_ += size;
    return;
  }

 private:
  Indent indent_;
  std::string header_;
  std::string footer_;
  SnippetType type_;
  std::vector<std::string> lines_;
};

/**
 * @brief Codeblock
 *
 */
class Block {
 public:
  /**
   * @brief Construct a new Block object as definition
   *
   * @param indent
   */
  Block(DefinitionType, const Indent &indent = Indent(0, kDefaultIndentSize))
      : indent_(indent), header_("{\n"), footer_("}\n"), type_(SnippetType::kDefinition) {}
  ~Block() = default;

  /**
   * @brief Out with own indent
   *
   * @return std::string
   */
  std::string Out() const noexcept {
    std::string block = indent_.Indenting() + header_;
    for (const auto &snippet : snippets_) {
      block += snippet.Out();
    }
    block += indent_.Indenting() + footer_;
    return block;
  }

  SnippetType Type() const noexcept { return type_; }

  void Add(const std::string &line) noexcept {
    Snippet snippet(line_t, Indent(indent_.level_ + 1, indent_.size_));
    snippet.Add(line);
    snippets_.emplace_back(std::move(snippet));
    return;
  }

  void Add(const std::vector<std::string> &lines) noexcept {
    for (const auto &line : lines) {
      Add(line);
    }
    return;
  }

  void Add(const Snippet &snippet) noexcept {
    Snippet snippet_copy(line_t, Indent(indent_.level_ + 1, indent_.size_));
    snippet_copy.Add(snippet);
    snippets_.emplace_back(std::move(snippet_copy));
    return;
  }

  void Add(const Block &block) noexcept {
    auto block_copy = block;
    std::stringstream line_stream(block_copy.Out());
    std::string line;
    while (std::getline(line_stream, line)) {
      Add(line);
    }
    return;
  }

  void IncrementIndent(std::size_t size = 1) noexcept {
    indent_.level_ += size;
    for (auto &&snippet : snippets_) {
      snippet.IncrementIndent(size);
    }
    return;
  }

 private:
  Indent indent_;
  std::string header_;
  std::string footer_;
  SnippetType type_;
  std::vector<Snippet> snippets_;
};

}  // namespace cppcodegen
