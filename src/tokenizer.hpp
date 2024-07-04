#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

enum token_type
{
    KEYWORD,
    SEPARATOR,
    OPERATOR,
    LITERAL,
    IDENTIFIER
};

enum token_subtype
{
    DATA_TYPE,
    CONTROL,
    RETURN,
    ARITHMETIC,
    RELATIONAL,
    LOGICAL,
    ASSIGNMENT,
    ARITHMETIC_ASSIGNMENT,
    ARITHMETIC_UNARY,
    UNARY,
    STRING,
    CHAR,
    INT,
    FLOAT,
    OPEN_PARENTHESIS,
    CLOSE_PARENTHESIS,
    OPEN_BRACES,
    CLOSE_BRACES,
    OPEN_BRACKETS,
    CLOSE_BRACKETS,
    SEMICOLON,
    COMMA,
    IDENTIFIER_SUBTYPE,
    EMPTY
};

struct token
{
    token_type type;
    token_subtype subtype;
    string value;
};


class tokenizer
{
    vector<token> *tokens;

public:
    vector<pair<token_type, vector<string>>> types;
    unordered_map<token_type, vector<pair<token_subtype, vector<string>>>> subtypes;
    unordered_map<string, token_subtype> literal_map;
    string subtype_to_str[EMPTY + 1];
    string type_to_str[IDENTIFIER + 1];

    tokenizer();

    vector<token> *get_tokens();

    bool is_keyword(const string &s);

    bool is_separator(const string &s);

    bool is_operator(const string &s);

    bool is_digit(char c);

    bool is_integer(const string &s);

    bool is_literal(const string &s);

    bool is_identifier(const string &s);

    token_subtype get_literal_subtype(const string &s);

    token_subtype get_operator_subtype(const string &s);

    token_subtype get_keyword_subtype(const string &s);

    token_subtype get_separator_subtype(const string &s);

    vector<token> *tokenize(const string &s);

    string get_subtype_str(token_subtype t);

    string get_type_str(token_type t);

    void print_tokens(vector<token> *ts = nullptr);
};

#endif // TOKENIZER_HPP
