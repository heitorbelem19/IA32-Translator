#include "translator.hpp"

translator::translator(){
  this->instructions_reg = std::regex("^(?:([A-Za-z_]\\w*): )?([A-Za-z_]+)(?: ([A-Za-z_]\\w*))?(?: ([+-]) )?(?: ?([-\\d]+))?(?:,)?(?: ([A-Za-z_]\\w*))?(?: ([+-]) )?(?: ?([-\\d]+))?$");
  this->readChar = false;
  this->writeChar = false;
  this->readString = false;
  this->writeString = false;
  this->readInteger = false;
  this->writeInteger = false;
  this->writeOverflow = false;
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
      this->section_text.emplace_back("\tadd eax, dword[" + tokens[3] + "]\n");
    }
    else{
      if(check_operator(tokens[4]) && check_offset(tokens[5])){
        this->section_text.emplace_back("\tadd eax, dword[" + tokens[3] + tokens[4] + "4*" + tokens[5] + "]\n");
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
      this->section_text.emplace_back("\tsub eax, dword[" + tokens[3] + "]\n");
    }
    else{
      if(check_operator(tokens[4]) && check_offset(tokens[5])){
        section_text.emplace_back("\tsub eax, dword[" + tokens[3] + tokens[4] + "4*" + tokens[5] + "]\n");
      }
      else{
        std::cout << ("ERRO: Operacao invalida na instrucao '%s'\n", tokens[0]);
      }
    }
  }
}

void translator::check_MUL(std::deque<std::string> tokens){
  if(!tokens[6].empty() || !tokens[7].empty() || !tokens[8].empty())
    std::cout << "ERRO: instrucao " << tokens[2] << " invalida\n";
  else{
    if(tokens[4].empty()) {
      this->section_text.emplace_back("\timul dword["+ tokens[3] + "]\n");
      // this->section_text.emplace_back("\tjbe ResultOverflow\n");
      // this->writeOverflow = true;
    } 
    else {
      if(check_operator(tokens[4]) && check_offset(tokens[5])){
        this->section_text.emplace_back("\timul dword[" + tokens[3] + tokens[4] + "4*" + tokens[5] + "]\n");
        // this->section_text.emplace_back("\tjbe ResultOverflow\n");
        // this->writeOverflow = true;
      }
      else
        std::cout << ("ERRO: Operacao invalida na instrucao '%s'\n", tokens[0]);
    }
  }
}

void translator::check_DIV(std::deque<std::string> tokens){
  if(!tokens[6].empty() || !tokens[7].empty() || !tokens[8].empty())
    std::cout << "ERRO: instrucao " << tokens[2] << " invalida\n";
  else{
    if(tokens[4].empty()) {
      this->section_text.emplace_back("\tcdq\n");
      this->section_text.emplace_back("\tidiv dword[" + tokens[3] + "]\n");
    } 
    else {
      if(check_operator(tokens[4]) && check_offset(tokens[5])){
        this->section_text.emplace_back("\tcdq\n");
        this->section_text.emplace_back("\tidiv dword[" + tokens[3] + tokens[4] + "4*" + tokens[5] + "]\n");
      }
      else
        std::cout << ("ERRO: Operacao invalida na instrucao '%s'\n", tokens[0]);
    }
  }
}

void translator::check_JMP(std::deque<std::string> tokens){
  this->section_text.emplace_back("\tjmp " + tokens[3] + "\n");
}

void translator::check_JMPZ(std::deque<std::string> tokens){
  this->section_text.emplace_back("\tcmp eax, 0\n");
  this->section_text.emplace_back("\tje " + tokens[3] + "\n");
}

void translator::check_JMPP(std::deque<std::string> tokens){
  this->section_text.emplace_back("\tcmp eax, 0\n");
  this->section_text.emplace_back("\tjg " + tokens[3] + "\n");
}

void translator::check_JMPN(std::deque<std::string> tokens){
  this->section_text.emplace_back("\tcmp eax, 0\n");
  this->section_text.emplace_back("\tjl " + tokens[3] + "\n");
}

void translator::check_COPY(std::deque<std::string> tokens){
  std::string src, dst;
  bool error = false;
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
    this->section_text.emplace_back("\tpush eax\n");
    this->section_text.emplace_back("\tmov eax, dword[" + src + "]\n");
    this->section_text.emplace_back("\tmov dword[" + dst + "], eax\n");
    this->section_text.emplace_back("\tpop eax\n");
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
      this->section_text.emplace_back("\tmov eax, dword[" + tokens[3] + "]\n");
    }
    else {
      if(check_operator(tokens[4]) && check_offset(tokens[5])){
        this->section_text.emplace_back("\tmov eax, dword[" + tokens[3] + tokens[4] + "4*" + tokens[5] + "]\n");
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
      this->section_text.emplace_back("\tmov dword[" + tokens[3] + "], eax\n");
    } 
    else {
      if (check_operator(tokens[4]) && check_offset(tokens[5])) {
        this->section_text.emplace_back("\tmov dword[" + tokens[3] + tokens[4] + "4*" + tokens[5] + "], eax\n");
      }
      else{
        std::cout << ("ERRO: Operacao invalida na instrucao '%s'\n", tokens[0]); 
      }
    }
  }
}

void translator::check_STOP(std::deque<std::string> tokens){
  this->section_text.emplace_back("\tmov eax, 1\n");
  this->section_text.emplace_back("\tmov ebx, 0\n");
  this->section_text.emplace_back("\tint 80h\n");
}

void translator::check_SPACE(std::deque<std::string> tokens){
  this->section_bss.emplace_back(tokens[1] + " resd " + (tokens[5].empty() ? "1" : tokens[5]) + "\n");
}

void translator::check_CONST(std::deque<std::string> tokens){
  this->section_data.emplace_back(tokens[1] + " dd " + tokens[5] + "\n");
}

void translator::check_C_INPUT(std::deque<std::string> tokens){
  this->section_text.emplace_back("\tpush " + tokens[3] + "\n");
  this->section_text.emplace_back("\tcall LerChar\n");
  this->section_text.emplace_back("\tadd esp, 4\n");
  this->readChar = true;
}

void translator::check_C_OUTPUT(std::deque<std::string> tokens){
  this->section_text.emplace_back("\tpush " + tokens[3] + "\n");
  this->section_text.emplace_back("\tcall EscreverChar\n");
  this->section_text.emplace_back("\tadd esp, 4\n");
  this->writeChar = true;
}

void translator::check_S_INPUT(std::deque<std::string> tokens){
  this->section_text.emplace_back("\tpush " + tokens[5] + "\n");
  this->section_text.emplace_back("\tpush " + tokens[6] + "\n");
  this->section_text.emplace_back("\tcall LerString\n");
  this->section_text.emplace_back("\tadd esp, 4\n");
  this->readChar = true;
  this->readString = true;
}

void translator::check_S_OUTPUT(std::deque<std::string> tokens){
  this->section_text.emplace_back("\tpush " + tokens[3] + "\n");
  this->section_text.emplace_back("\tcall EscreverString\n");
  this->section_text.emplace_back("\tadd esp, 4\n");
  this->writeChar = true;
  this->writeString = true;
}

void translator::check_INPUT(std::deque<std::string> tokens){
  this->section_text.emplace_back("\tpush " + tokens[3] + "\n");
  this->section_text.emplace_back("\tcall LerInteiro\n");
  this->section_text.emplace_back("\tadd esp, 4\n");
  this->readChar = true;
  this->readInteger = true;
}

// void translator::check_OUTPUT(std::deque<std::string> tokens){
//   this->section_text.emplace_back("\tpush dword[" + tokens[3] + "]\n");
//   this->section_text.emplace_back("\tcall EscreverInteiro\n");
//   this->section_text.emplace_back("\tadd esp, 4\n");
//   this->writeChar = true;
//   this->writeString = true;
//   this->writeInteger = true;
// }

void translator::LerChar(){
  this->section_text.emplace_back("LerChar:\n");
  this->section_text.emplace_back("\tenter 0, 0\n");
  this->section_text.emplace_back("\tpusha\n");
  this->section_text.emplace_back("\tmov eax, 3\n");
  this->section_text.emplace_back("\tmov ebx, 0\n");
  this->section_text.emplace_back("\tmov ecx, [EBP+8]\n");
  this->section_text.emplace_back("\tmov edx, 1\n");
  this->section_text.emplace_back("\tint 80h\n");
  this->section_text.emplace_back("\tpopa\n");
  this->section_text.emplace_back("\tleave\n");
  this->section_text.emplace_back("\tret\n");
}

void translator::LerString(){
  this->section_text.emplace_back("LerString: \n");
  this->section_text.emplace_back("\tenter 0, 0\n");
  this->section_text.emplace_back("\tpusha\n");
  this->section_text.emplace_back("\tmov eax, 0\n");
  this->section_text.emplace_back("leitura_string:\n");
  this->section_text.emplace_back("\tmov ecx, [EBP+8]\n");
  this->section_text.emplace_back("\tmov esi, [EBP+12]\n");
  this->section_text.emplace_back("\tmov ebx, eax\n");
  this->section_text.emplace_back("\tshl ebx, 2\n");
  this->section_text.emplace_back("\tadd ecx, ebx\n");
  this->section_text.emplace_back("\tpush ecx\n");
  this->section_text.emplace_back("\tcall LerChar\n");
  this->section_text.emplace_back("\tadd esp, 4\n");
  this->section_text.emplace_back("\tinc eax\n");
  // add agora 2-linhas
  this->section_text.emplace_back("\tcmp eax, esi\n");
  this->section_text.emplace_back("\tje return\n");
  this->section_text.emplace_back("\tcmp dword[ecx], 0xa\n");
  this->section_text.emplace_back("\tjne leitura_string\n");
  this->section_text.emplace_back("\tmov dword[ecx], 0\n");
  // add agora 1-linha
  this->section_text.emplace_back("\treturn:");
  this->section_text.emplace_back("\tpopa\n");
  this->section_text.emplace_back("\tadd esp, 4\n");
  this->section_text.emplace_back("\tleave\n");
  this->section_text.emplace_back("\tret\n");
}

void translator::EscreverChar(){
  this->section_text.emplace_back("EscreverChar:\n");
  this->section_text.emplace_back("\tenter 0, 0\n");
  this->section_text.emplace_back("\tpusha\n");
  this->section_text.emplace_back("\tmov eax, 4\n");
  this->section_text.emplace_back("\tmov ebx, 1\n");
  this->section_text.emplace_back("\tmov ecx, [EBP+8]\n");
  this->section_text.emplace_back("\tmov edx, 1\n");
  this->section_text.emplace_back("\tint 80h\n");
  this->section_text.emplace_back("\tpopa\n");
  this->section_text.emplace_back("\tleave\n");
  this->section_text.emplace_back("\tret\n");
}

void translator::EscreverString(){
  this->section_text.emplace_back("EscreverString:\n");
  this->section_text.emplace_back("\tenter 0, 0\n");
  this->section_text.emplace_back("\tpush eax\n");
  this->section_text.emplace_back("\tmov eax, [EBP+8]\n");
  this->section_text.emplace_back("escrita_string:\n");
  this->section_text.emplace_back("\tpush eax\n");
  this->section_text.emplace_back("\tcall EscreverChar\n");
  this->section_text.emplace_back("\tadd esp, 4\n");
  this->section_text.emplace_back("\tadd eax, 4\n");
  this->section_text.emplace_back("\tcmp dword[eax], 0\n");
  this->section_text.emplace_back("\tjnz escrita_string\n");
  this->section_text.emplace_back("\tpop eax\n");
  this->section_text.emplace_back("\tadd esp, 4\n");
  this->section_text.emplace_back("\tleave\n");
  this->section_text.emplace_back("\tret\n");
}

void translator::LerInteiro(){
  this->section_text.emplace_back("LerInteiro:\n");
  this->section_text.emplace_back("\tenter 5, 0\n");
  this->section_text.emplace_back("\tpusha\n");
  this->section_text.emplace_back("\tmov ebx, ebp\n");
  this->section_text.emplace_back("\tsub ebx, 1\n");
  this->section_text.emplace_back("\tmov dword[EBP-5], 0\n");
  this->section_text.emplace_back("leitura_inteiro:\n");
  this->section_text.emplace_back("\tpush ebx\n");
  this->section_text.emplace_back("\tcall LerChar\n");
  this->section_text.emplace_back("\tadd esp, 4\n");
  this->section_text.emplace_back("\tcmp byte[EBP-1], 0xa\n");
  this->section_text.emplace_back("\tje fim_inteiro\n");
  this->section_text.emplace_back("\tmov eax, [EBP-5]\n");
  this->section_text.emplace_back("\tmov edx, eax\n");
  this->section_text.emplace_back("\tshl eax, 2\n");
  this->section_text.emplace_back("\tadd eax, eax\n");
  this->section_text.emplace_back("\tadd eax, edx\n");
  this->section_text.emplace_back("\tadd eax, edx\n");
  this->section_text.emplace_back("\tmov [EBP-5], eax\n");
  this->section_text.emplace_back("\tmov eax, [EBP-1]\n");
  this->section_text.emplace_back("\tsub eax, 0x30\n");
  this->section_text.emplace_back("\tadd [EBP-5], eax\n");
  this->section_text.emplace_back("\tjmp leitura_inteiro\n");
  this->section_text.emplace_back("fim_inteiro:\n");
  this->section_text.emplace_back("\tmov eax, [EBP-5]\n");
  this->section_text.emplace_back("\tmov ebx, [EBP+8]\n");
	this->section_text.emplace_back("\tmov [ebx], eax\n");
  this->section_text.emplace_back("\tpopa\n");
  this->section_text.emplace_back("\tleave\n");
  this->section_text.emplace_back("\tret\n");
}

void translator::EscreverInteiro(){
  this->section_text.emplace_back("EscreverInteiro:\n");
  this->section_text.emplace_back("\tenter 0, 0\n");
  this->section_text.emplace_back("\tpusha\n");
  this->section_text.emplace_back("\tmov ebx, esp\n");
  this->section_text.emplace_back("\tmov eax, [EBP+8]\n");
  this->section_text.emplace_back("\tmov edx, 0\n");
  this->section_text.emplace_back("\tpush edx\n");
  this->section_text.emplace_back("\tmov ecx, 10\n");
  this->section_text.emplace_back("escrita_inteiro:\n");
  this->section_text.emplace_back("\tmov edx, 0\n");
  this->section_text.emplace_back("\tdiv ecx\n");
  this->section_text.emplace_back("\tadd edx, 0x30\n");
  this->section_text.emplace_back("\tpush edx\n");
  this->section_text.emplace_back("\tcmp eax, 0\n");
  this->section_text.emplace_back("\tjnz escrita_inteiro\n");
  this->section_text.emplace_back("\tpush esp\n");
  this->section_text.emplace_back("\tcall EscreverString\n");
  this->section_text.emplace_back("\tadd esp, 4\n");
  this->section_text.emplace_back("\tmov esp, ebx\n");
  this->section_text.emplace_back("\tpopa\n");
  this->section_text.emplace_back("\tleave\n");
  this->section_text.emplace_back("\tret\n");

}

void translator::ResultOverflow(){
  this->section_text.emplace_back("ResultOverflow:\n");
  this->section_text.emplace_back("\tpush OVERFLOW\n");
  this->section_text.emplace_back("\tcall EscreverString\n");
  this->section_text.emplace_back("\tadd esp, 4\n");
  this->section_text.emplace_back("\tmov eax, 1\n");
  this->section_text.emplace_back("\tmov ebx, 0\n");
  this->section_text.emplace_back("\tint 80h\n"); 
}

void translator::translate(std::vector<std::string> &uploaded_file, std::string file_name){
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
            this->section_text.emplace_back(tokens[1] + ":\n");
          this->check_ADD(tokens);
        }
        else if(tokens[2] == "SUB"){
          if(!tokens[1].empty())
            this->section_text.emplace_back(tokens[1] + ":\n");
          this->check_SUB(tokens);
        }
        else if(tokens[2] == "MUL"){
          if(!tokens[1].empty())
            this->section_text.emplace_back(tokens[1] + ":\n");
          this->check_MUL(tokens);
        }
        else if(tokens[2] == "DIV"){
          if(!tokens[1].empty())
            this->section_text.emplace_back(tokens[1] + ":\n");
          this->check_DIV(tokens);
        }
        else if(tokens[2] == "JMP"){
          if(!tokens[1].empty())
            this->section_text.emplace_back(tokens[1] + ":\n");
          this->check_JMP(tokens);
        }
        else if(tokens[2] == "JMPZ"){
          if(!tokens[1].empty())
            this->section_text.emplace_back(tokens[1] + ":\n");
          this->check_JMPZ(tokens);
        }
        else if(tokens[2] == "JMPP"){
          if(!tokens[1].empty())
            this->section_text.emplace_back(tokens[1] + ":\n");
          this->check_JMPP(tokens);
        }
        else if(tokens[2] == "JMPN"){
          if(!tokens[1].empty())
            this->section_text.emplace_back(tokens[1] + ":\n");
          this->check_JMPN(tokens);
        }
        else if(tokens[2] == "COPY"){
          if(!tokens[1].empty())
            this->section_text.emplace_back(tokens[1] + ":\n");
          this->check_COPY(tokens);
        }
        else if(tokens[2] == "LOAD"){
          if(!tokens[1].empty())
            this->section_text.emplace_back(tokens[1] + ":\n");
          this->check_LOAD(tokens);
        }
        else if(tokens[2] == "STORE"){
          if(!tokens[1].empty())
            this->section_text.emplace_back(tokens[1] + ":\n");
          this->check_STORE(tokens);
        }
        else if(tokens[2] == "STOP"){
          if(!tokens[1].empty())
            this->section_text.emplace_back(tokens[1] + ":\n");
          this->check_STOP(tokens);
        }
        else if(tokens[2] == "SPACE"){
          this->check_SPACE(tokens);
        }
        else if(tokens[2] == "CONST"){
          this->check_CONST(tokens);
        }
        else if(tokens[2] == "C_INPUT"){
          if(!tokens[1].empty())
            this->section_text.emplace_back(tokens[1] + ":\n"); 
          this->check_C_INPUT(tokens);
        }
        else if(tokens[2] == "C_OUTPUT"){
          if(!tokens[1].empty())
            this->section_text.emplace_back(tokens[1] + ":\n");
          this->check_C_OUTPUT(tokens);
        }
        else if(tokens[2] == "S_INPUT"){
          if(!tokens[1].empty())
            this->section_text.emplace_back(tokens[1] + ":\n");
          this->check_S_INPUT(tokens);
        }
        else if(tokens[2] == "S_OUTPUT"){
          if(!tokens[1].empty())
            this->section_text.emplace_back(tokens[1] + ":\n");
          this->check_S_OUTPUT(tokens);
        }
        else if(tokens[2] == "INPUT"){
          if(!tokens[1].empty())
            this->section_text.emplace_back(tokens[1] + ":\n");
          this->check_INPUT(tokens);
        }
        // else if(tokens[2] == "OUTPUT"){
        //   if(!tokens[1].empty())
        //     this->section_text.emplace_back(tokens[1] + ":\n");
        //   this->check_OUTPUT(tokens);
        // }
      }
      catch(std::exception exc){
        printf("Exceção gerada '%s'\tLinha %d\n", exc.what(), i+1);
      }
    }
  }
  this->write_translation_result(file_name);

}

void translator::write_translation_result(std::string file_name){
  std::ofstream final_file;
  std::regex remove_extension_reg("(.asm)");
  std::string file_out = std::regex_replace(file_name, remove_extension_reg, ".s");
  
  if(this->readChar) this->LerChar();
  if(this->readString) this->LerString();
  if(this->writeChar) this->EscreverChar();
  if(this->writeString) this->EscreverString();
  if(this->readInteger) this->LerInteiro();
  if(this->writeInteger) this->EscreverInteiro();
  if(this->writeOverflow) this->ResultOverflow();

  final_file.open(file_out);

  final_file << "section .text\n";
  final_file << "global _start\n";
  final_file << "_start:\n"; 

  for (int i = 0; i < this->section_text.size(); i++){
    final_file << this->section_text[i];
  }
  final_file << std::endl;

  final_file << "section .data\n";
  final_file << "OVERFLOW dd 'o', 'v', 'e', 'r', 'f', 'l', 'o', 'w'\n";
  for (int i = 0; i < this->section_data.size(); i++){
    final_file << this->section_data[i];
  }
  final_file << std::endl;

  final_file << "section .bss\n";
  for (int i = 0; i < this->section_bss.size(); i++){
    final_file << this->section_bss[i];
  }
  final_file.close();
}