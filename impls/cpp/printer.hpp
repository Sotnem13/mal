#ifndef PRINTER_HPP
#define PRINTER_HPP

#include "types.hpp"

namespace printer {



}

std::string pr_str(const MalType& d);
std::string pr_str(const MalType::Number& d);
std::string pr_str(const MalType::List& d);


std::string pr_str(const MalType::Number& number) {
  // std::string res;
  return std::to_string(number);
  // return res;
}

std::string pr_str(const MalType::String& s) {
  return '"' + s.data + '"';
}

std::string pr_str(const MalType::Symbol& s) {
  return s.data;
}
std::string pr_str(const MalType::Keyword& s) {
  return ':' + s.data;
}

// std::string pr_str(const MalType::List& list) {
//   std::string res = "(";
//   bool first = true;
//   for (const auto& d : list) {
//     if (!first) res += ' ';
//     else first = false;
//     res += pr_str(d);
//   }
//   res += ')';
//   return res;
// }

template <typename T>
std::string pr_str(const typename T::Type& list) {
  std::string res = T::begin;
  bool first = true;
  for (const auto& d : list) {
    if (!first) res += ' ';
    else first = false;
    res += pr_str(d);
  }
  res += T::end;
  return res;
}

std::string pr_str(const MalType::Map& map) {
  std::string res = "{";
  bool first = true;
  for (const auto& d : map) {
    if (!first) res += ' ';
    else first = false;

    auto k = d.first;
    auto s = MalType::mapkey(k);
    const auto& v = std::any_cast<const MalType&>(d.second);
    res += pr_str(s);
    res += ' ';
    res += pr_str(v);

  }
  res += '}';
  return res;
}

std::string pr_str(const MalType::Bool& b) {
  if (b) return "true";
  return "false";
}
std::string pr_str(const MalType::Nil&) {
  return "nil";
}

std::string pr_str(const MalType& d) {

  auto& data = d.data;

  if (d.is_symbol()) {
    return d.symbol();
  }
  if (d.is_string()) {
    return d.string();
  }

  if (d.is_keyword()) {
    return d.keyword();
  }

  if (d.is<MalType::Number>()) {
    return pr_str(d.number());
  }
  if (d.is<MalType::List>()) {
    return pr_str<ListMeta>(d.list());
  }
  if (d.is_vector()) {
    return pr_str<ArrayMeta>(d.vector());
  }
  if (d.is<MalType::Map>()) {
    return pr_str(d.map());
  }
  if (d.is<MalType::Bool>()) {
    return pr_str(d.as_bool());
  }
  if (d.is<MalType::Nil>()) {
    return "nil";
  }
  if (d.is_function()) {
    return "function";
  }

  return "error";
}

#endif
