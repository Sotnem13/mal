#include <string>
#include <iostream>

std::string read(std::string read) {
  return read;
}

std::string eval(std::string input) {
  return input;
}

std::string print(std::string input) {
  return input;
}

std::string repl(std::string input) {
  return print(eval(read(std::move(input))));
}

int main() {

  while (true) {
      std::cout << "user> ";
      std::string input;
      if (!std::getline(std::cin, input)) break;
      std::cout << repl(input) << std::endl;
  }
}
