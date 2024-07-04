# Trabalho Prático de Linguagens Formais

### Integrantes: **Thiago Bortoluzzi e Rafael Nogueira**

Compilação do programa:
```bash
g++ -o analisador_lexico main.cpp ./src/parser.cpp ./src/tokenizer.cpp
```

Execução do programa:
```bash
./analisador_lexico
```

Ao executar o programa, automaticamente será realizado o teste de parsing dos arquivos de exemplo localizados no diretório './exemplos/'. Para realizar o teste de parsing de um arquivo específico, basta passar o caminho do arquivo como argumento ao executar o programa. Exemplo:
```bash
./analisador_lexico ./exemplos/exemplo1.txt
```