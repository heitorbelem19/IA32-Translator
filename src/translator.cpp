#include "translator.hpp"

translator::translator(){
  this->instructions_reg = std::regex("^(?:([A-Za-z_]\\w*): )?([A-Za-z]+)(?: ([A-Za-z_]\\w*))?(?: ([+-]) )?(?: ?([-\\d]+))?(?:,)?(?: ([A-Za-z_]\\w*))?(?: ([+-]) )?(?: ?([-\\d]+))?$");
}
translator::~translator(){}

bool translator::check_operator(std::string operacao){
  return operacao == "+" || operacao == "-";
}

void translator::check_ADD(std::deque<std::string> tokens){
  if(!tokens[6].empty() || !tokens[7].empty() || !tokens[8].empty()){
    std::cout << "ERRO: instrucao " << tokens[2] << " invalida\n";
  }
  else{
    if(tokens[4].empty()){
      this->section_text.emplace_back("add dword[" + tokens[3] + "], eax\n");
    }
    else{
      std::cout << "VERIFICAR SE O ADD É UMA OPERAÇÃO VÁLIDA SOBRE UM ARRAY\n";
      // se for, trata o offset e o operador 1, se não for, dá um erro de operação inválida
    }
  }
}

void translator::translate(std::vector<std::string> &uploaded_file){
  std::smatch matches;
  for(int i=0; i<uploaded_file.size(); i++){
    if(std::regex_search(uploaded_file[i], matches, this->instructions_reg)){
      std::deque<std::string> tokens;
      /*
        Campos separados para cada item no array "tokens" Todos os campos são opcionais, a não ser o tokens[2]
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
          std::cout << "SUB\n";
        }
        else if(tokens[2] == "MUL"){
          std::cout << "MUL\n";
        }
        else if(tokens[2] == "DIV"){
          std::cout << "DIV\n";
        }
        else if(tokens[2] == "COPY"){
          std::cout << "COPY\n";
        }
        else if(tokens[2] == "JMP"){
          std::cout << "JMP\n";
        }
        else if(tokens[2] == "JPMZ"){
          std::cout << "JMPZ\n";
        }
        else if(tokens[2] == "JPMP"){
          std::cout << "JMPP\n";
        }
        else if(tokens[2] == "JMPN"){
          std::cout << "JMPN\n";
        }
        else if(tokens[2] == "LOAD"){
          std::cout << "LOAD\n";
        }
        else if(tokens[2] == "STORE"){
          std::cout << "STORE\n";
        }
        else if(tokens[2] == "CONST"){
          std::cout << "CONST\n";
        }
        else if(tokens[2] == "SPACE"){
          std::cout << "SPACE\n";
        }
        else if(tokens[2] == "STOP"){
          std::cout << "STOP\n";
        }
      }
      catch(std::exception exc){
        printf("Exception '%s' reported at line %d\n", exc.what(), i+1);
      }
    }
  }

}