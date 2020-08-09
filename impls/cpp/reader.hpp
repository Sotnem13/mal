
#ifndef READER_HPP
#define READER_HPP

#include <deque>

#include "types.hpp"

constexpr bool match(char c, const char* i) {
  while(*i) {
    if (*i == c) return true;
    ++i;
  }
  return false;
}
// constexpr

bool is_number(const std::string& input) {
  // std::cout << "is_number: " << input << std::endl;
  // if (!match(input[0], "+-")) return false;
  auto s = 0;
  if (input[0] == '-') ++s;
  auto res = false;
  for (size_t i = s; i < input.size(); i++) {
    auto c = input[i];
    res = true;
    if (!match(c, "0123456789")) return false;
    // if (c == '.' && dot) dot = true
  }
  return res;
}

MalType::Number to_number(const std::string& input) {
  return std::atol(input.c_str());
}


struct Tokenizer {

  Tokenizer() = default;
  Tokenizer(std::string s) : input(std::move(s)) {}


  void read_str(std::string s) {
    input = std::move(s);

    cp = 0;
    while (is_space(input[cp])) cp++;
    if (input[cp] == ';') while (input[cp++] != '\n' && !empty()) cp++;
  }

  void pop_front() {
    front_ = "";
  }
  std::string front() {
    if (!front_.empty()) return front_;

    if (input[cp] == '"') {
      string();
    } else if (input[cp] == '~' && input[cp+1] == '@') {
        front_ = "~@";
        cp += 2;
    } else if (match(input[cp], "()[]{}`'~^@")) {
        front_ = input[cp];
        cp++;
    } else {
      symbols();
    }

    while (is_space(input[cp])) cp++;
    if (input[cp] == ';') while (input[cp++] != '\n' && !empty());

    return front_;
  }


  void symbols() {
    auto b = cp;
    while (true) {
      // if (empty()) throw std::logic_error("tokenizer: eof");
      if (empty()) break;
      auto c = input[cp];
      if (match(c," \",\n\t()[]{}")) break;
      cp++;
    }
    front_ = std::string(input.begin() + b, input.begin() + cp);
  }

  void string() {
    auto b = cp++;
    auto skip = false;
    while (true) {
      if (empty()) throw std::logic_error("tokenizer: eof");
      auto c = input[cp];
      cp++;
      if (!skip && c == '"') break;
      skip = !skip && c == '\\';
    }
    front_ = std::string(input.begin() + b, input.begin() + cp);
  }

  bool empty() {
    return front_.size() == 0 && cp >= input.size();
  }

  static bool is_space(char c) {
    return match(c, " \t\n,");
  }

  size_t cp = 0;
  std::string input;
  std::string front_;

};
struct Reader {




  Tokenizer tokens;
  // std::deque<std::string> tokens;


  void read_str(std::string input) {
    tokens.read_str(input);
  }
  MalType read_form() {
    if (tokens.empty()) throw std::runtime_error("read_form: tokens empty");
    auto t = tokens.front();

    if (t == ListMeta::begin) {
      return read_list<ListMeta>();
    }
    if (t == ArrayMeta::begin) {
      return read_list<ArrayMeta>();
    }
    if (t == "{") {
      return read_map();
    }
    if (t == "'") {
      tokens.pop_front();
      MalType::List l;
      l.push_back(MalType::Symbol("quote"));
      l.push_back(read_form());
      return l;
    }
    if (t == "`") {
      tokens.pop_front();
      MalType::List l;
      l.push_back(MalType::Symbol("quasiquote"));

      l.push_back(read_form());
      return l;
    }
    if (t == "~") {
      tokens.pop_front();
      MalType::List l;
      l.push_back(MalType::Symbol("unquote"));

      l.push_back(read_form());
      return l;
    }
    if (t == "~@") {
      tokens.pop_front();
      MalType::List l;
      l.push_back(MalType::Symbol("splice-unquote"));

      l.push_back(read_form());
      return l;
    }
    if (t == "@") {
      tokens.pop_front();
      MalType::List l;
      l.push_back(MalType::Symbol("deref"));
      l.push_back(read_form());
      return l;
    }
    if (t == "^") {
      tokens.pop_front();
      MalType::List l;
      l.push_back(MalType::Symbol("with-meta"));
      auto arg1 = read_form();
      auto arg2 = read_form();
      l.push_back(arg2);
      l.push_back(arg1);
      return l;
    }

    return read_atom();
  }

  template<typename T>
  MalType read_list() {
    if (tokens.empty()) throw std::runtime_error("read_list: tokens empty");
    auto t = tokens.front();
    if (t != T::begin) throw std::runtime_error("read_list: expected " + std::string(T::begin));
    tokens.pop_front();
//
    typename T::Type data;

    while (true) {
      if (tokens.empty()) throw std::runtime_error("read_list: expected " + std::string(T::end));
      t = tokens.front();
      if (t == T::end) {
        tokens.pop_front();
        break;
      }
      data.push_back(read_form());
    }

    return {data};
  }

  MalType read_map() {
    if (tokens.empty()) throw std::runtime_error("read_list: tokens empty");
    auto t = tokens.front();
    if (t != "{") throw std::runtime_error("read_list: expected {");
    tokens.pop_front();
//
    MalType::Map data;

    while (true) {
      if (tokens.empty()) throw std::runtime_error("read_list: expected }");
      t = tokens.front();
      if (t == "}") {
        tokens.pop_front();
        break;
      }
      auto key = read_form();
      if (tokens.empty()) throw std::runtime_error("read_list: expected map value");
      auto value = read_form();

      if (key.is_string()) {
        data["s_" + key.string()] = value;
      } else if (key.is_keyword()) {
        data["k_" + key.keyword()] = value;
      } else {
        throw std::runtime_error("read_list: map key must be string or keyword");
      }

    }

    return {data};
  }

  MalType read_atom() {
    if (tokens.empty()) throw std::runtime_error("read_atom: tokens empty");

    auto t = tokens.front();
    tokens.pop_front();

    if (is_number(t)) {
      const auto n = to_number(t);
      // std::cout << "number: " << t << " " << n << std::endl;
      return n;
    }

    if (t == "true") {
      return true;
    }
    if (t == "false") {
      return false;
    }
    if (t == "nil") {
      return MalType::Nil();
    }
    // std::cout << "token: " << t << std::endl;

    if (t[0] == '"') {
      MalType::String s;
      s.data = t;
      return s;
    }

    if (t[0] == ':') {
      MalType::Keyword s;
      s.data = t;
      return s;
    }

    MalType::Symbol s;
    s.data = t;
    return s;
  }

};


#endif
