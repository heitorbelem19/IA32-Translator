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
  void check_ADD(std::deque<std::string> tokens);
  void check_SUB(std::deque<std::string> tokens);
  
public:
  translator();
  ~translator();
  void translate(std::vector<std::string> &uploaded_file);
};


#endif //MONTADOR_TRANSLATOR_HPP