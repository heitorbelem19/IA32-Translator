#include "translator.hpp"

translator::translator(){
  this->instructions_reg = std::regex("^(?:([A-Za-z_]\\w*): )?([A-Za-z]+)(?: ([A-Za-z_]\\w*))?(?: ([+-]) )?(?: ?([-\\d]+))?(?:,)?(?: ([A-Za-z_]\\w*))?(?: ([+-]) )?(?: ?([-\\d]+))?$");
}
translator::~translator(){}

bool translator::check_operator(std::string operacao){
  return operacao == "+" || operacao == "-";
}

bool translator::check_offset(std::string offset){
  try {
    std::stoul(offset, nullptr, 0);
    return true;
  }
  catch(std::exception &ex){
    return false;
  }
}

void translator::check_ADD(std::deque<std::string> tokens){
  if(!tokens[6].empty() || !tokens[7].empty() || !tokens[8].empty()){
    std::cout << "ERRO: instrucao " << tokens[2] << " invalida\n";
  }
  else{
    if(tokens[4].empty()){
      this->section_text.emplace_back("add eax, dword[" + tokens[3] + "]\n");
    }
    else{
      if(check_operator(tokens[4]) && check_offset(tokens[5])){
        this->section_text.emplace_back("add eax, dword[" + tokens[3] + tokens[4] + "4*" + tokens[5] + "]\n");
      }
      else{
        std::cout << ("ERRO: Operacao invalida na instrucao '%s'\n", tokens[0]);
      }
    }
  }
}

void translator::check_SUB(std::deque<std::string> tokens){
  if(!tokens[6].empty() || !tokens[7].empty() || !tokens[8].empty()){
    std::cout << "ERRO: instrucao " << tokens[2] << " invalida\n";
  }
  else{
    if(tokens[4].empty()){
      this->section_text.emplace_back("sub eax, dword[" + tokens[3] + "]\n");
    }
    else{
      if(check_operator(tokens[4]) && check_offset(tokens[5])){
        section_text.emplace_back("sub eax, dword[" + tokens[3] + tokens[4] + "4*" + tokens[5] + "]\n");
      }
      else{
        std::cout << ("ERRO: Operacao invalida na instrucao '%s'\n", tokens[0]);
      }
    }
  }
}

void translator::check_MUL(std::deque<std::string> tokens){

}

void translator::check_DIV(std::deque<std::string> tokens){

}

void translator::check_JMP(std::deque<std::string> tokens){
  this->section_text.emplace_back("jmp " + tokens[3] + "\n");
}

void translator::check_JMPZ(std::deque<std::string> tokens){
  this->section_text.emplace_back("cmp eax, 0\n");
  this->section_text.emplace_back("je " + tokens[3] + "\n");
}

void translator::check_JMPP(std::deque<std::string> tokens){
  this->section_text.emplace_back("cmp eax, 0\n");
  this->section_text.emplace_back("jg " + tokens[3] + "\n");
}

void translator::check_JMPN(std::deque<std::string> tokens){
  this->section_text.emplace_back("cmp eax, 0\n");
  this->section_text.emplace_back("jl " + tokens[3] + "\n");
}

void translator::check_COPY(std::deque<std::string> tokens){
  std::string src, dst;
  bool error = false;

  // std::cout << "1: " << tokens[1] << "\t" << "2: " << tokens[2] << "\t" << "3: " << tokens[3] << "\t" << "4: " << tokens[4] << "\n";
  // std::cout << "5: " << tokens[5] << "\t" << "6: " << tokens[6] << "\t" << "7: " << tokens[7] << "\t" << "8: " << tokens[8] << "\n";

  // Verificar se o operando SRC é um elemento de array ou não
  if(tokens[4].empty())
    src = tokens[3];
  else { 
    if(check_operator(tokens[4]) && check_offset(tokens[5]))
      src = tokens[3] + tokens[4] + "4*" + tokens[5];
    else
      error = true;
  }

  // Verificar se o operando DST é um elemento de array ou não
  if(tokens[7].empty())
    dst = tokens[6];
  else { 
    if(check_operator(tokens[7]) && check_offset(tokens[8]))
      dst = tokens[6] + tokens[7] + "4*" + tokens[8];
    else
      error = true;
  }

  // verificar se houve erro na instrução a ser traduzida
  if(!error){
    this->section_text.emplace_back("push eax\n");
    this->section_text.emplace_back("mov eax, dword[" + src + "]\n");
    this->section_text.emplace_back("mov dword[" + dst + "], eax\n");
    this->section_text.emplace_back("pop eax\n");
  }
  else 
    std::cout << "ERRO: instrucao " << tokens[2] << " invalida\n";
}

void translator::check_LOAD(std::deque<std::string> tokens){
  if(!tokens[6].empty() || !tokens[7].empty() || !tokens[8].empty()){
    std::cout << "ERRO: instrucao " << tokens[2] << " invalida\n"; 
  }
  else {
    if(tokens[4].empty()){
      this->section_text.emplace_back("mov eax, dword[" + tokens[3] + "]\n");
    }
    else {
      if(check_operator(tokens[4]) && check_offset(tokens[5])){
        this->section_text.emplace_back("mov eax, dword[" + tokens[3] + tokens[4] + "4*" + tokens[5] + "]\n");
      }
      else {
        std::cout << ("ERRO: Operacao invalida na instrucao '%s'\n", tokens[0]); 
      }
    }
  }
}

void translator::check_STORE(std::deque<std::string> tokens){
  if(!tokens[6].empty() || !tokens[7].empty() || !tokens[8].empty()){
    std::cout << "ERRO: instrucao " << tokens[2] << " invalida\n"; 
  }
  else {
    if(tokens[4].empty()) {
      this->section_text.emplace_back("mov dword[" + tokens[3] + "], eax\n");
    } 
    else {
      if (check_operator(tokens[4]) && check_offset(tokens[5])) {
        this->section_text.emplace_back("mov dword[" + tokens[3] + tokens[4] + "4*" + tokens[5] + "], eax\n");
      }
      else{
        std::cout << ("ERRO: Operacao invalida na instrucao '%s'\n", tokens[0]); 
      }
    }
  }
}

void translator::check_STOP(std::deque<std::string> tokens){
    this->section_text.emplace_back("mov eax, 1\n");
    this->section_text.emplace_back("mov ebx, 0\n");
    this->section_text.emplace_back("int 80h\n");
}

void translator::check_SPACE(std::deque<std::string> tokens){
  this->section_bss.emplace_back(tokens[1] + " resb " + (tokens[5].empty() ? "1" : tokens[5]) + "\n");
}

void translator::check_CONST(std::deque<std::string> tokens){
  this->section_data.emplace_back(tokens[1] + " db " + tokens[5] + "\n");
}

void translator::check_SECTION(std::deque<std::string> tokens){
  if(tokens[3] == "TEXT"){
    this->section_text.emplace_back("section .text\n");
    this->section_text.emplace_back("global _start\n");
    this->section_text.emplace_back("_start:\n"); 
  }
  else if(tokens[3] == "DATA"){
    this->section_data.emplace_back("section .data\n");
  }
  else if(tokens[3] == "BSS"){
    this->section_bss.emplace_back("section .bss\n");
  }
  else {
    std::cout << "ERRO: Secao Invalida\n";
  }
}

void translator::translate(std::vector<std::string> &uploaded_file){
  std::smatch matches;
  for(int i=0; i<uploaded_file.size(); i++){
    if(std::regex_search(uploaded_file[i], matches, this->instructions_reg)){
      std::deque<std::string> tokens;
      /*
        - Campos separados para cada item no array "tokens".
        - Todos os campos são opcionais, a não ser o tokens[2] 
        tokens[0]: Linha completa           tokens[5]: Offset_1                   
        tokens[1]: Rótulo                   tokens[6]: Operando_2
        tokens[2]: Instrução (OBR)          tokens[7]: Operador_2
        tokens[3]: Operando_1               tokens[8]: Offset_2
        tokens[4]: Operador_1               
      */
      tokens.clear();
      for(auto &match: matches) {
        tokens.push_back(match.str());
      }
      try{
        if(tokens[2] == "ADD"){
          if(!tokens[1].empty())
            this->section_text.emplace_back(tokens[1] + ": ");
          this->check_ADD(tokens);
        }
        else if(tokens[2] == "SUB"){
          if(!tokens[1].empty())
            this->section_text.emplace_back(tokens[1] + ": ");
          this->check_SUB(tokens);
        }
        else if(tokens[2] == "MUL"){
          if(!tokens[1].empty())
            this->section_text.emplace_back(tokens[1] + ": ");
          this->check_MUL(tokens);
        }
        else if(tokens[2] == "DIV"){
          if(!tokens[1].empty())
            this->section_text.emplace_back(tokens[1] + ": ");
          this->check_DIV(tokens);
        }
        else if(tokens[2] == "JMP"){
          if(!tokens[1].empty())
            this->section_text.emplace_back(tokens[1] + ": ");
          this->check_JMP(tokens);
        }
        else if(tokens[2] == "JMPZ"){
          if(!tokens[1].empty())
            this->section_text.emplace_back(tokens[1] + ": ");
          this->check_JMPZ(tokens);
        }
        else if(tokens[2] == "JMPP"){
          if(!tokens[1].empty())
            this->section_text.emplace_back(tokens[1] + ": ");
          this->check_JMPP(tokens);
        }
        else if(tokens[2] == "JMPN"){
          if(!tokens[1].empty())
            this->section_text.emplace_back(tokens[1] + ": ");
          this->check_JMPN(tokens);
        }
        else if(tokens[2] == "COPY"){
          if(!tokens[1].empty())
            this->section_text.emplace_back(tokens[1] + ": ");
          this->check_COPY(tokens);
        }
        else if(tokens[2] == "LOAD"){
          if(!tokens[1].empty())
            this->section_text.emplace_back(tokens[1] + ": ");
          this->check_LOAD(tokens);
        }
        else if(tokens[2] == "STORE"){
          if(!tokens[1].empty())
            this->section_text.emplace_back(tokens[1] + ": ");
          this->check_STORE(tokens);
        }
        else if(tokens[2] == "STOP"){
          if(!tokens[1].empty())
            this->section_text.emplace_back(tokens[1] + ": ");
          this->check_STOP(tokens);
        }
        else if(tokens[2] == "SPACE"){
          this->check_SPACE(tokens);
        }
        else if(tokens[2] == "CONST"){
          this->check_CONST(tokens);
        }
        else if(tokens[2] == "SECTION"){
          this->check_SECTION(tokens);
        }
      }
      catch(std::exception exc){
        printf("Exception '%s' reported at line %d\n", exc.what(), i+1);
      }
    }
  }

  for (int i = 0; i < this->section_text.size(); i++){
    std::cout << this->section_text[i];
  }
  for (int i = 0; i < this->section_bss.size(); i++){
    std::cout << this->section_bss[i];
  }
  for (int i = 0; i < this->section_data.size(); i++){
    std::cout << this->section_data[i];
  }

}