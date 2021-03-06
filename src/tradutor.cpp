#include <bits/stdc++.h>
#include "io_manager.hpp"
#include "pre_processor.hpp"
#include "translator.hpp"

int main(int argc, char** argv){
  /*
    Carrega arquivo para a memória
    Salva na instância uploaded_file_stream;
  */
  std::regex remove_extension_reg("(.asm)");
  std::string output_file_name = argv[1];

  // Lê arquivo .asm
  io_manager file_input(argv[1]);
  std::vector<std::string> uploaded_file = file_input.get_uploaded_file();
  
  // faz pre processamento
  pre_processor pre_processamento;
  pre_processamento.process(uploaded_file);
  
  // cria arquivo .PRE
  std::ofstream pre_process_file;
  std::string file_out = std::regex_replace(output_file_name, remove_extension_reg, ".PRE");
  
  // escreve no arquivo .PRE
  pre_process_file.open(file_out);
  for(int i=0; i<uploaded_file.size(); i++){
    pre_process_file << uploaded_file[i] << '\n';
  }
  pre_process_file.close();

  // traduz do .PRE em assembly inventado para o .s em IA-32
  translator tradutor;
  tradutor.translate(uploaded_file, output_file_name);
  
  
  return 0;
}