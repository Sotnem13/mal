#include <string>
#include <iostream>
#include "types.hpp"
#include "reader.hpp"
#include "printer.hpp"
#include "environment.hpp"


Reader reader;
Environment env;

MalType eval(MalType ast);
std::string print(const MalType& data);

MalType read(std::string read) {
  reader.read_str(std::move(read));
  return reader.read_form();
}

MalType eval_ast(MalType& ast) {
  if (ast.is_symbol()) return env.map[ast.symbol()];
  if (ast.is_list()) {
    auto& l = ast.list();
    MalType::List nl;
    for (auto& e : l) {
      nl.push_back(eval(e));
    }
    return nl;
  }
  if (ast.is_vector()) {
    auto& l = ast.vector();
    MalType::Vector nl;
    for (auto& e : l) {
      nl.push_back(eval(e));
    }
    return nl;
  }

  if (ast.is_map()) {
    auto& l = ast.map();
    MalType::Map nl;
    for (auto& e : l) {
      nl[e.first] = eval(std::any_cast<MalType&>(e.second));
      // nl.push_back(eval(e));
    }
    return nl;
  }
  return ast;
}

MalType apply_(MalType fn, MalType args) {
  // print(f);
  // print(f);
  // print(f);
  // print(f);
  // auto fn = env.map[MalType::mapkey(f)];
  return fn(args);
  // return f;
  // return f;
}

MalType eval(MalType ast) {
  // std::cout << "TTTT" << std::endl;

  if (!ast.is_list())  return eval_ast(ast);
  if (ast.is_nil() || ast.is_list() && ast.list().empty())  return ast;
  // std::cout << "TTTT" << std::endl;

  auto l1 = eval_ast(ast);
  if (l1.is_list()) {
    auto& l = l1.list();
    auto f = l.front();
    if (f.is_function()) {
      l.pop_front();
      return apply_(std::move(f), l);
    }
  }
  return l1;
}




std::string print(const MalType& data) {
  return pr_str(data);
}

std::string repl(std::string input) {
  return print(eval(read(std::move(input))));
}

MalType add_fn(MalType args_) {
  // print(args_);
  // std::cout << std::endl;
  // return args_;
  auto args = args_.list();
  return args[0].number() + args[1].number();
}

MalType sub_fn(MalType args_) {
  auto args = args_.list();
  return args[0].number() - args[1].number();
}
MalType div_fn(MalType args_) {
  auto args = args_.list();
  if (args[1].number() == 0) {
    throw std::runtime_error("Div to zero");
  }
  return args[0].number() / args[1].number();
}
MalType mul_fn(MalType args_) {
  auto args = args_.list();
  return args[0].number() * args[1].number();
}


int main() {
  auto &map = env.map;
  map["+"] = std::function(add_fn);
  map["-"] = std::function(sub_fn);
  map["/"] = std::function(div_fn);
  map["*"] = std::function(mul_fn);

  while (true) {
      std::cout << "user> ";
      std::string input;
      if (!std::getline(std::cin, input)) break;
      // try {
        std::cout << repl(input) << std::endl;
      // } catch(...) {
        // std::cout << "EOF" << std::endl;
      // }
  }
}
