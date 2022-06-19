#pragma once

namespace cppcodegen {

typedef struct LineType {
  explicit LineType() = default;
} LineType;
constexpr LineType line_type_t{};

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
  explicit Snippet(const Indent &indent = Indent(0, 0)) : indent_(indent) {}
  virtual ~Snippet() = default;

  template <typename... Args>
  void Add(Args... args) {
    snippet_ += Indenting();
    add(args...);
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

  void add(const Snippet<Type> &snippet) noexcept {
    snippet_ += snippet.Out();
    return;
  }
  void add(LineType, const std::string &line) noexcept {
    snippet_ += line + "\n";
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

}  // namespace cppcodegen
