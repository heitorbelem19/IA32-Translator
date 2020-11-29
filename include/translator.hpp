#ifndef MONTADOR_TRANSLATOR_HPP
#define MONTADOR_TRANSLATOR_HPP

#include <bits/stdc++.h>

class translator {
private:
  std::regex instructions_reg;
  std::deque<std::string> section_text;
  std::deque<std::string> section_bss;
  std::deque<std::string> section_data;

  bool readChar;
  bool writeChar;
  bool readString;
  bool writeString;
  bool readInteger;
  bool writeInteger;
  bool writeOverflow;

  bool check_operator(std::string operacao);
  bool check_offset(std::string offset);
  
  void check_ADD(std::deque<std::string> tokens);
  void check_SUB(std::deque<std::string> tokens);
  void check_MUL(std::deque<std::string> tokens);
  void check_DIV(std::deque<std::string> tokens);

  void check_JMP(std::deque<std::string> tokens);
  void check_JMPZ(std::deque<std::string> tokens);
  void check_JMPP(std::deque<std::string> tokens);
  void check_JMPN(std::deque<std::string> tokens);

  void check_COPY(std::deque<std::string> tokens);
  
  void check_LOAD(std::deque<std::string> tokens);
  void check_STORE(std::deque<std::string> tokens);

  void check_STOP(std::deque<std::string> tokens);

  void check_SPACE(std::deque<std::string> tokens);
  void check_CONST(std::deque<std::string> tokens);

  void check_SECTION(std::deque<std::string> tokens);

  void check_C_INPUT(std::deque<std::string> tokens);
  void check_S_INPUT(std::deque<std::string> tokens);
  void check_C_OUTPUT(std::deque<std::string> tokens);
  void check_S_OUTPUT(std::deque<std::string> tokens);
  void check_INPUT(std::deque<std::string> tokens);
  void check_OUTPUT(std::deque<std::string> tokens);

  void LerChar();
  void LerString();
  void EscreverChar();
  void EscreverString();
  void LerInteiro();
  void EscreverInteiro();
  void ResultOverflow();

public:
  translator();
  ~translator();
  void translate(std::vector<std::string> &uploaded_file, std::string file_name);
  void write_translation_result(std::string file_name);
};


#endif //MONTADOR_TRANSLATOR_HPP