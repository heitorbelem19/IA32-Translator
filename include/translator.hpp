#ifndef MONTADOR_TRANSLATOR_HPP
#define MONTADOR_TRANSLATOR_HPP

#include <bits/stdc++.h>

class translator {
private:
  std::regex instructions_reg;
  std::deque<std::string> section_text;
  std::deque<std::string> section_bss;
  std::deque<std::string> section_data;

  bool check_operator(std::string operacao);
  bool check_offset(std::string offset);
  
  void check_ADD(std::deque<std::string> tokens);
  void check_SUB(std::deque<std::string> tokens);

  void check_JMP(std::deque<std::string> tokens);
  void check_JMPZ(std::deque<std::string> tokens);
  void check_JMPP(std::deque<std::string> tokens);
  void check_JMPN(std::deque<std::string> tokens);

  void check_COPY(std::deque<std::string> tokens);
  
  void check_LOAD(std::deque<std::string> tokens);
  void check_STORE(std::deque<std::string> tokens);

  void check_STOP(std::deque<std::string> tokens);

  
public:
  translator();
  ~translator();
  void translate(std::vector<std::string> &uploaded_file);
};


#endif //MONTADOR_TRANSLATOR_HPP