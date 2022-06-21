#pragma once
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

namespace cppcodegen {

const std::size_t kDefaultIndentSize = 2;

// previous declaration

typedef struct LineType {
  explicit LineType() = default;
} LineType;
typedef struct SystemIncludeType {
  explicit SystemIncludeType() = default;
} SystemIncludeType;
typedef struct LocalIncludeType {
  explicit LocalIncludeType() = default;
} LocalIncludeType;
typedef struct CodeBlockType {
  explicit CodeBlockType() = default;
} CodeBlockType;
typedef struct DefinitionType {
  explicit DefinitionType() = default;
} DefinitionType;
typedef struct NamespaceType {
  explicit NamespaceType() = default;
} NamespaceType;
typedef struct ClassType {
  explicit ClassType() = default;
} ClassType;
typedef struct StructType {
  explicit StructType() = default;
} StructType;

constexpr LineType line_t{};
constexpr SystemIncludeType system_include_t{};
constexpr LocalIncludeType local_include_t{};
constexpr CodeBlockType code_block_t{};
constexpr DefinitionType definition_t{};
constexpr NamespaceType namespace_t{};
constexpr ClassType class_t{};
constexpr StructType struct_t{};

enum class Type { kLine, kSystemInclude, kLocalInclude, kCodeBlock, kDefinition, kNamespace, kClass, kStruct };
enum class AccessSpecifier { kPublic, kProtected, kPrivate };

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
  ~Indent() = default;
  Indent(const Indent &) = default;
  Indent &operator=(const Indent &) = default;
  Indent(Indent &&) = default;
  Indent &operator=(Indent &&) = default;

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
   * @brief Construct a new Snippet object as default : Line
   *
   * @param indent
   */
  Snippet(const Indent &indent = Indent(0, kDefaultIndentSize)) : Snippet(LineType(), indent) {
  }
  /**
   * @brief Construct a new Snippet object as Line
   *
   * @param indent
   */
  Snippet(LineType, const Indent &indent = Indent(0, kDefaultIndentSize))
      : indent_(indent), header_(), footer_(), type_(Type::kLine) {
  }
  /**
   * @brief Construct a new Snippet object as system include
   *
   * @param indent
   */
  Snippet(SystemIncludeType, const Indent &indent = Indent(0, kDefaultIndentSize))
      : indent_(indent), header_("#include <"), footer_(">"), type_(Type::kSystemInclude) {
  }
  /**
   * @brief Construct a new Snippet object as local include
   *
   * @param base_dir_path include relative base path
   * @param indent
   */
  Snippet(LocalIncludeType, const std::string base_dir_path, const Indent &indent = Indent(0, kDefaultIndentSize))
      : indent_(indent), header_("#include \"" + base_dir_path), footer_("\""), type_(Type::kLocalInclude) {
  }
  ~Snippet() = default;
  Snippet(const Snippet &) = default;
  Snippet &operator=(const Snippet &) = default;
  Snippet(Snippet &&) = default;
  Snippet &operator=(Snippet &&) = default;

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

  Type GetType() const noexcept {
    return type_;
  }

  void Add(const std::string &line) noexcept {
    lines_.emplace_back(header_ + line + footer_);
    return;
  }

  /**
   * @brief Add any type snippet as lines
   *
   * @tparam T
   * @param any
   */
  template <typename T>
  void Add(const T &any) noexcept {
    std::stringstream line_stream(any.Out());
    std::string line;
    while (std::getline(line_stream, line)) {
      lines_.emplace_back(line);
    }
    return;
  }
  void Add(const char characters[]) noexcept {
    Add(std::string(characters));
    return;
  }

  void Add(const std::vector<std::string> &lines) noexcept {
    for (const auto &line : lines) {
      Add(line);
    }
    return;
  }

  void IncrementIndent(std::size_t level = 1) noexcept {
    indent_.level_ += level;
    return;
  }

 private:
  Indent indent_;
  std::string header_;
  std::string footer_;
  Type type_;
  std::vector<std::string> lines_;
};

/**
 * @brief Codeblock
 *
 */
class Block {
 public:
  /**
   * @brief Construct a new Block object as default : codeblock
   *
   * @param indent
   */
  Block(const Indent &indent = Indent(0, kDefaultIndentSize)) : Block(CodeBlockType(), indent) {
  }
  /**
   * @brief Construct a new Block object as codeblock
   *
   * @param indent
   */
  Block(CodeBlockType, const Indent &indent = Indent(0, kDefaultIndentSize))
      : indent_(indent), header_("{\n"), footer_("}\n"), type_(Type::kCodeBlock) {
  }
  /**
   * @brief Construct a new Block object as definition
   *
   * @param indent
   */
  Block(DefinitionType, const std::string &declaration, const Indent &indent = Indent(0, kDefaultIndentSize))
      : indent_(indent), header_(declaration + " {\n"), footer_("}\n"), type_(Type::kDefinition) {
  }
  /**
   * @brief Construct a new Block object as namespace
   *
   * @param name
   * @param indent
   */
  Block(NamespaceType, const std::string &name, const Indent &indent = Indent(0, kDefaultIndentSize))
      : indent_(indent), header_("namespace " + name + " {\n"), footer_("}\n"), type_(Type::kNamespace) {
  }
  ~Block() = default;
  Block(const Block &) = default;
  Block &operator=(const Block &) = default;
  Block(Block &&) = default;
  Block &operator=(Block &&) = default;

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

  Type GetType() const noexcept {
    return type_;
  }

  void Add(const std::vector<std::string> &lines) noexcept {
    for (const auto &line : lines) {
      Add(line);
    }
    return;
  }

  template <typename T>
  void Add(const T &any) noexcept {
    Snippet snippet_copy(line_t, Indent(indent_.level_ + 1, indent_.size_));
    snippet_copy.Add(any);
    snippets_.emplace_back(std::move(snippet_copy));
    return;
  }

  void IncrementIndent(std::size_t level = 1) noexcept {
    indent_.level_ += level;
    for (auto &&snippet : snippets_) {
      snippet.IncrementIndent(level);
    }
    return;
  }

 private:
  Indent indent_;
  std::string header_;
  std::string footer_;
  Type type_;
  std::vector<Snippet> snippets_;
};

/**
 * @brief Class and Struct
 *
 */
class Class {
 public:
  /**
   * @brief Construct a new Class object default : class
   *
   * @param name
   * @param indent
   */
  Class(const std::string &name, const Indent &indent = Indent(0, kDefaultIndentSize))
      : Class(ClassType(), name, indent) {
  }
  /**
   * @brief Construct a new Class object as class
   *
   * @param name
   * @param indent
   * @details
   * default access specifier is private.
   */
  Class(ClassType, const std::string &name, const Indent &indent = Indent(0, kDefaultIndentSize))
      : indent_(indent),
        name_(name),
        header_(" {\n"),
        footer_("};\n"),
        type_(Type::kClass),
        snippets_({{AccessSpecifier::kPrivate, {}}, {AccessSpecifier::kPublic, {}}, {AccessSpecifier::kProtected, {}}}),
        now_specifier_(AccessSpecifier::kPrivate) {
  }
  /**
   * @brief Construct a new Class object as class with inheritances
   *
   * @param name
   * @param inheritances inheritance access specifiers and class names
   * @param indent
   */
  Class(ClassType, const std::string &name, const std::vector<std::pair<AccessSpecifier, std::string>> inheritances,
        const Indent &indent = Indent(0, kDefaultIndentSize))
      : indent_(indent),
        name_(name),
        header_(" {\n"),
        footer_("};\n"),
        type_(Type::kClass),
        snippets_({{AccessSpecifier::kPrivate, {}}, {AccessSpecifier::kPublic, {}}, {AccessSpecifier::kProtected, {}}}),
        now_specifier_(AccessSpecifier::kPrivate) {
    for (const auto &inheritance : inheritances) {
      AddInheritance(inheritance.second, inheritance.first);
    }
  }
  /**
   * @brief Construct a new Class object as struct
   *
   * @param name
   * @param indent
   * @details
   * default access specifier is public.
   */
  Class(StructType, const std::string &name, const Indent &indent = Indent(0, kDefaultIndentSize))
      : indent_(indent),
        name_(name),
        header_(" {\n"),
        footer_("};\n"),
        type_(Type::kStruct),
        snippets_({{AccessSpecifier::kPrivate, {}}, {AccessSpecifier::kPublic, {}}, {AccessSpecifier::kProtected, {}}}),
        now_specifier_(AccessSpecifier::kPublic) {
  }
  ~Class() = default;
  Class(const Class &) = default;
  Class &operator=(const Class &) = default;
  Class(Class &&) = default;
  Class &operator=(Class &&) = default;

  /**
   * @brief Out with own indent
   *
   * @return std::string
   */
  std::string Out() const noexcept {
    std::string block = indent_.Indenting() + "class " + name_ + header_;
    if (!snippets_.at(AccessSpecifier::kPublic).empty()) {
      block += indent_.Indenting() + " public:\n";
      for (const auto &snippet : snippets_.at(AccessSpecifier::kPublic)) {
        block += snippet.Out();
      }
    }
    if (!snippets_.at(AccessSpecifier::kProtected).empty()) {
      block += indent_.Indenting() + " protected:\n";
      for (const auto &snippet : snippets_.at(AccessSpecifier::kProtected)) {
        block += snippet.Out();
      }
    }
    if (!snippets_.at(AccessSpecifier::kPrivate).empty()) {
      block += indent_.Indenting() + " private:\n";
      for (const auto &snippet : snippets_.at(AccessSpecifier::kPrivate)) {
        block += snippet.Out();
      }
    }
    block += indent_.Indenting() + footer_;
    return block;
  }

  Type GetType() const noexcept {
    return type_;
  }

  void Add(const std::vector<std::string> &lines) noexcept {
    for (const auto &line : lines) {
      Add(line);
    }
    return;
  }

  template <typename T>
  void Add(const T &any) noexcept {
    Snippet snippet_copy(line_t, Indent(indent_.level_ + 1, indent_.size_));
    snippet_copy.Add(any);
    snippets_[now_specifier_].emplace_back(std::move(snippet_copy));
    return;
  }

  void IncrementIndent(std::size_t level = 1) noexcept {
    indent_.level_ += level;
    for (auto &&each_snippets : snippets_) {
      for (auto &&snippet : each_snippets.second) {
        snippet.IncrementIndent(level);
      }
    }
    return;
  }

  void AddInheritance(const std::string &name, AccessSpecifier access_specifier = AccessSpecifier::kPublic) noexcept {
    std::string access_specifier_str;
    if (access_specifier == AccessSpecifier::kPublic) {
      access_specifier_str = "public";
    } else if (access_specifier == AccessSpecifier::kProtected) {
      access_specifier_str = "protected";
    } else if (access_specifier == AccessSpecifier::kPrivate) {
      access_specifier_str = "private";
    }
    if (header_.size() < 3 || header_.substr(0, 3) != " : ") {
      header_.insert(0, " : ");
      header_.insert(header_.size() - 3, access_specifier_str + " " + name);
    } else {
      header_.insert(header_.size() - 3, ", " + access_specifier_str + " " + name);
    }
    return;
  }

  void SetAccessSpecifier(AccessSpecifier access_specifier) {
    now_specifier_ = access_specifier;
  }

 private:
  Indent indent_;
  std::string name_;
  std::string header_;
  std::string footer_;
  Type type_;
  std::unordered_map<AccessSpecifier, std::vector<Snippet>> snippets_;
  AccessSpecifier now_specifier_;
};

/**
 * @brief Stream operator for snippet
 *
 * @tparam T
 * @param value
 * @param another
 * @return Snippet&
 */
template <typename T>
Snippet &operator<<(Snippet &value, const T &another) {
  value.Add(another);
  return value;
}
/**
 * @brief Stream operator for block
 *
 * @tparam T
 * @param value
 * @param another
 * @return Block&
 */
template <typename T>
Block &operator<<(Block &value, const T &another) {
  value.Add(another);
  return value;
}
/**
 * @brief Stream operator for class
 *
 * @tparam T
 * @param value
 * @param another
 * @return Class&
 */
template <typename T>
Class &operator<<(Class &value, const T &another) {
  value.Add(another);
  return value;
}
/**
 * @brief Stream operator for class access specifier
 *
 * @param value
 * @param access_specifier
 * @return Class&
 */
Class &operator<<(Class &value, AccessSpecifier access_specifier) {
  value.SetAccessSpecifier(access_specifier);
  return value;
}

}  // namespace cppcodegen
