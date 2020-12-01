# Trabalho 2 - Tradutor Assembly Inventado para IA-32
## Universidade de Brasília - UnB
---
## Autor
### Nome: Heitor de Lima Belém
### Matrícula: 16/0123950

## Instruções para compilação
1. Verifique se, na pasta **translator_sb02/src**, existe a pasta **obj**:
  - Se não existir crie essa pasta (**obj**) dentro de **translator_sb02/src/** e vá para o passo _2_.
  - Se existir, vá para o passo _2_.
2. No terminal, dentro da pasta **translator_sb02/src** execute o comando:
    ```
      make
    ```
3. Agora, ainda na mesma pasta do passo anterior, execute o programa:
    ```
      ./tradutor <nome_do_arquivo>.asm
    ```
  **OBS**: o arquivo *.asm* deve estar na pasta **translator_sb02/src**
  
  ### Exemplo de Pre processamento:
  
  ```bash
    ./tradutor triangulo.asm

    Arquivo de saída: triangulo.s
  ```
# Observações Importantes sobre o Trabalho
## Trabalho feito no Linux usando MakeFile.
## Os arquivos de saída são armazenados na pasta translator_sb02/src.
## Não consegui fazer a instrução de IO para números negativos funcionar.


    