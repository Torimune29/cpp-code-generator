#pragma once

namespace cppcodegen {

typedef struct LineType {
  explicit LineType() = default;
} LineType;
constexpr LineType line_t{};

typedef struct SystemIncludeType {
  explicit SystemIncludeType() = default;
} SystemIncludeType;
constexpr SystemIncludeType system_include_t{};

typedef struct LocalIncludeType {
  explicit LocalIncludeType() = default;
} LocalIncludeType;
constexpr LocalIncludeType local_include_t{};

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
} Indent;

template <typename Type>
class Snippet {
 public:
  Snippet(LineType type, const Indent &indent = Indent(0, 0))
      : indent_(indent), header_(), footer_("\n"), type_(type) {}
  Snippet(SystemIncludeType type, const Indent &indent = Indent(0, 0))
      : indent_(indent), header_("#include <"), footer_(">\n"), type_(type) {}
  Snippet(LocalIncludeType type, const std::string base_dir_path, const Indent &indent = Indent(0, 0))
      : indent_(indent), header_("#include \"" + base_dir_path), footer_("\"\n"), type_(type) {}
  virtual ~Snippet() = default;

  template <typename... Args>
  void Add(Args &&...args) {
    snippet_ += Indenting();
    add(std::forward<Args>(args)...);
    return;
  }

  /**
   * @brief Out with own indent
   *
   * @return std::string
   */
  std::string Out() const noexcept { return snippet_; }

 private:
  std::string snippet_;
  const Indent indent_;
  std::string header_;
  std::string footer_;
  Type type_;

  template <typename AllType>
  void add(const Snippet<AllType> &snippet) noexcept {
    snippet_ += snippet.Out();
    return;
  }
  void add(const std::string &line) noexcept {
    snippet_ += header_ + line + footer_;
    return;
  }

  std::string Indenting() {
    std::string total_indent;
    std::size_t level = 0;
    while (level <= indent_.level_) {
      total_indent += indent_.indent_string_;
      level++;
    }
    return total_indent;
  }
};

using Line = Snippet<LineType>;
using SystemInclude = Snippet<SystemIncludeType>;
using LocalInclude = Snippet<LocalIncludeType>;

}  // namespace cppcodegen
