
#ifndef READER_HPP
#define READER_HPP

struct MalType;

class Reader {


  std::deque<std::string> tokens;


  void read_str(std::string input) {
    input.c_str();
    tokenize();
  }

  void tokenize(char* input, size_t len) {

  }

  MalType read_form() {

  }

}


#endif
