#ifndef TYPES_HPP
#define TYPES_HPP

#include <variant>
#include <vector>
#include <deque>
#include <unordered_map>
#include <memory>
#include <any>
#include <functional>


struct MalType {

  enum Type {
    SYMBOL,
    STRING,
    KEYWORD
  } type;

  template <Type T>
  struct Wrap {
    Wrap() = default;
    Wrap(std::string s) : data(std::move(s)) {};

    ~Wrap() = default;

    std::string data;
    constexpr static Type type = T;
  };

  // struct Symbol {
  //   std::string data;
  //   constexpr static Type = SYMBOL;
  // };



  using Number = long;
  using String  = Wrap<STRING>;
  using Symbol  = Wrap<SYMBOL>;
  using Keyword = Wrap<KEYWORD>;

  using Bool = bool;
  struct Nil {};

  using List   = std::deque<MalType>;
  using Vector  = std::vector<MalType>;
  using Map    = std::unordered_map<std::string, std::any>;

  using Fn1    = std::function<MalType(MalType)>;
  // using Fn2    = std::function<MalType(MalType)>;

  std::variant<std::string, List, Number, Map, Vector, Nil, Bool, Fn1> data;


  MalType() : MalType(Nil()) {};

  template <Type T>
  MalType(Wrap<T> s) : data(s.data), type(T) {};

  MalType(Number s) : data(s) {};
  MalType(Vector s) : data(s) {};
  MalType(List s) : data(s) {};
  MalType(Map s) : data(s) {};

  MalType(Nil s) : data(s) {};
  MalType(Bool s) : data(s) {};
  MalType(Fn1 s) : data(s) {};
  // MalType(Fn2 s) : data(s) {};


  MalType(MalType&&) = default;
  MalType(const MalType&) = default;


  ~MalType() = default;

  MalType& operator=(const MalType&) = default;

  MalType operator()(MalType& args) {
    if (!is_function()) throw std::logic_error("call value as fn");
    return function()(args);
    // return {};
  };


  // MalType& operator=() = default;


  // template<typename T>
  // MalType& operator=(T) {
  //   data =
  //   return *this;
  // }

  // bool is() const;
  //

  bool is_nil() const {
    return std::holds_alternative<Nil>(data);
  }

  bool is_function() const {
    return std::holds_alternative<Fn1>(data);
  }

  bool is_list() const {
    return std::holds_alternative<List>(data);
  }

  bool is_vector() const {
    return std::holds_alternative<Vector>(data);
  }

  bool is_map() const {
    return std::holds_alternative<Map>(data);
  }

  bool is_bool() const {
    return std::holds_alternative<Bool>(data);
  }

  bool is_number() const {
    return std::holds_alternative<Number>(data);
  }

  bool is_string() const {
    return type == STRING && std::holds_alternative<std::string>(data);
  }

  bool is_keyword() const {
    return type == KEYWORD && std::holds_alternative<std::string>(data);
  }
  bool is_symbol() const {
    return type == SYMBOL && std::holds_alternative<std::string>(data);
  }

  template<typename T>
  bool is() const {
    return std::holds_alternative<T>(data);
  }

  Bool as_bool() const {
    return std::get<Bool>(data);
  }

  const std::string& symbol() const {
    return std::get<std::string>(data);
  }
  const std::string& string() const {
    return std::get<std::string>(data);
  }
  const std::string& keyword() const {
    return std::get<std::string>(data);
  }

  const List& list() const {
    return std::get<List>(data);
  }
  const Number& number() const {
    return std::get<Number>(data);
  }

  const Map& map() const {
    return std::get<Map>(data);
  }

  const Vector& vector() const {
    return std::get<Vector>(data);
  }

  const Fn1& function() const {
    return std::get<Fn1>(data);
  }


  std::string& symbol() {
    return std::get<std::string>(data);
  }

  std::string& string() {
    return std::get<std::string>(data);
  }
  std::string& keyword() {
    return std::get<std::string>(data);
  }

  List& list() {
    return std::get<List>(data);
  }
  Number& number() {
    return std::get<Number>(data);
  }

  Map& map() {
    return std::get<Map>(data);
  }
  Vector& vector() {
    return std::get<Vector>(data);
  }

  Fn1& function() {
    return std::get<Fn1>(data);
  }

  // MalType& operator[](MalType& key) {
  //   if (!is<Map>()) throw std::logic_error("must be map");
  //   auto& m = map();
  //
  //   return m;
  // }

  // holds_alternative<MalType::Symbol>




  static MalType mapkey(const std::string& s) {
      if (is_string(s)) {
        MalType::String d;
        d.data = std::string(s.begin() + 2, s.end());
        return d;
      }

      if (is_symbol(s)) {
        MalType::Symbol d;
        d.data = std::string(s.begin() + 2, s.end());
        return d;
      }

      if (is_keyword(s)) {
        MalType::Keyword d;
        d.data = std::string(s.begin() + 2, s.end());
        return d;
      }
      throw std::logic_error("MapKey fail");
      return {};
  }

  static std::string mapkey(const MalType& s) {
      if (s.is_string()) {
        return "s_" + s.string();
      }
      if (s.is_keyword()) {
        return "k_" + s.keyword();
      }
      if (s.is_symbol()) {
        return "c_" + s.symbol();
      }

      if (s.is_nil()) {
        throw std::logic_error("MapKey fail: nil");
      }
      if (s.is_bool()) {
        throw std::logic_error("MapKey fail: bool");
      }

      //
      // if (is_symbol(s)) {
      //   MalType::Symbol d;
      //   d.data = std::string(s.begin() + 2, s.end());
      //   return d;
      // }
      //
      // if (is_keyword(s)) {
      //   MalType::Keyword d;
      //   d.data = std::string(s.begin() + 2, s.end());
      //   return d;
      // }
      throw std::logic_error("MapKey fail");
      return {};
  }

  static bool is_string(const std::string& s) {
    static auto s_ = "s_";
    for (size_t i = 0; i < 2; i++) {
      if (s[i] != s_[i]) return false;
    }
    return true;
  }

  static bool is_symbol(const std::string& s) {
    static auto s_ = "c_";
    for (size_t i = 0; i < 2; i++) {
      if (s[i] != s_[i]) return false;
    }
    return true;
  }

  static bool is_keyword(const std::string& s) {
    static auto s_ = "k_";
    for (size_t i = 0; i < 2; i++) {
      if (s[i] != s_[i]) return false;
    }
    return true;
  }

};


struct ListMeta {
  constexpr static auto begin = "(";
  constexpr static auto end   = ")";
  using Type = MalType::List;
};

struct ArrayMeta {
  constexpr static auto begin = "[";
  constexpr static auto end   = "]";
  using Type = MalType::Vector;
};


#endif
