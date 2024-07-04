# Trabalho Prático de Linguagens Formais

### Integrantes: **Thiago Bortoluzzi e Rafael Nogueira**

## Especificação Lexical da Linguagem

Esta especificação lexical descreve os tokens, seus tipos e subtipos para uma linguagem de programação baseada no código e nos comentários fornecidos.

### Tipos de Tokens

Os tokens são classificados em diferentes tipos e subtipos. Aqui estão os tipos de tokens aceitos:

1. **Keyword (Palavra-chave)**
   - **Subtipos:**
     - **DATA_TYPE (Tipo de Dados):** `int`, `float`, `char`, `double`, `void`
     - **CONTROL (Controle de Fluxo):** `if`, `else`, `for`, `while`, `do`
     - **RETURN (Retorno):** `return`

2. **Separator (Separador)**
   - **Subtipos:**
     - **OPEN_PARENTHESIS (Parêntese Aberto):** `(`
     - **CLOSE_PARENTHESIS (Parêntese Fechado):** `)`
     - **OPEN_BRACES (Chave Aberta):** `{`
     - **CLOSE_BRACES (Chave Fechada):** `}`
     - **OPEN_BRACKETS (Colchete Aberto):** `[`
     - **CLOSE_BRACKETS (Colchete Fechado):** `]`
     - **SEMICOLON (Ponto e Vírgula):** `;`
     - **COMMA (Vírgula):** `,`

3. **Operator (Operador)**
   - **Subtipos:**
     - **ARITHMETIC (Aritmético):** `+`, `-`, `*`, `/`, `%`
     - **RELATIONAL (Relacional):** `==`, `!=`, `>`, `<`, `>=`, `<=`
     - **LOGICAL (Lógico):** `&&`, `||`, `&`, `|`, `^`, `<<`, `>>`
     - **ASSIGNMENT (Atribuição):** `=`
     - **ARITHMETIC_ASSIGNMENT (Atribuição Aritmética):** `+=`, `-=`, `*=`, `/=`, `%=`, `&=`, `|=`, `^=`, `<<=`, `>>=`
     - **ARITHMETIC_UNARY (Unário Aritmético):** `++`, `--`
     - **UNARY (Unário):** `~`, `!`, `&`, `*`

4. **Identifier (Identificador)**
   - **Padrão de Formação:** letra(letra+dígito + `_` )*

5. **Literal**
   - **Padrão de Formação:** (STRING + CHAR + INT + FLOAT)
   - **Subtipos:**
     - **STRING (String):** **Padrão de Formação:** `"(letra+dígito+simbolos)*"`
     - **CHAR (Caractere):** **Padrão de Formação:** `'(letra+dígito+simbolos)*'`
     - **INT (Inteiro):** **Padrão de Formação:** `dígito+`
     - **FLOAT (Flutuante):** **Padrão de Formação:** `dígito+.dígito+`

### Estruturas Sintáticas

As estruturas sintáticas da linguagem são formadas por combinações específicas de tokens. Aqui está a definição das estruturas aceitas:

1. **Expression (Expressão)**
   - **EXPRESSÃO:** Conjunto de tokens que formam uma expressão válida.
   - **Padrão de Formação:** `UNARY_OPERATOR*((IDENTIFIER + LITERAL) OPERATOR)* (IDENTIFIER + LITERAL)`

2. **Assignment (Atribuição)**
   - **Padrão de Formação:** `IDENTIFIER (OPERATOR[ASSIGNMENT] + OPERATOR[ARITHMETIC_ASSIGNMENT]) EXPRESSION';'`

3. **Declaration (Declaração)**
   - **Padrão de Formação:** `KEYWORD[DATA_TYPE] (OPERATOR '*')* IDENTIFIER ('=' (EXPRESSION + LITERAL + IDENTIFIER) ';' + ';')`

4. **if**
   - **Padrão de Formação:** `'if' '(' EXPRESSION ')' '{' STATEMENT* '}'`

5. **printf**
   - **Padrão de Formação:** `'printf' '(' LITERAL (',' (IDENTIFIER + LITERAL))* ')' ;`
   - **Descrição:** Função de escrita

6. **fgets**
   - **Padrão de Formação:** `'fgets' '(' IDENTIFIER ',' (IDENTIFIER + LITERAL) ',' IDENTIFIER ')' ;`
   - **Descrição:** Função de leitura

7. **return**
   - **Padrão de Formação:** `'return' (EXPRESSION + LITERAL + IDENTIFIER) ';'`
