#include <string>
#include <iostream>
#include "types.hpp"
#include "reader.hpp"
#include "printer.hpp"


Reader reader;

MalType read(std::string read) {
  reader.read_str(std::move(read));
  // reader.print_tokens();
  return reader.read_form();
}

MalType eval(MalType ast) {
  return ast;
}

std::string print(const MalType& data) {
  return pr_str(data);
}

std::string repl(std::string input) {
  return print(eval(read(std::move(input))));
}

int main() {

  while (true) {
      std::cout << "user> ";
      std::string input;
      if (!std::getline(std::cin, input)) break;
      try {
        std::cout << repl(input) << std::endl;
      } catch(...) {
        std::cout << "EOF" << std::endl;
      }
  }
}
