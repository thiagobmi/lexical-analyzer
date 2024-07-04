#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "tokenizer.hpp"

class afd_parser
{
    tokenizer lex;
    std::vector<token> *tokens;
    std::string error_message;
    int current;

    enum state_type
    {
        S_INIT,
        S_ASSIGNMENT,
        S_DECLARATION,
        S_EXPRESSION,
        S_CONDITION,
        S_IF,
        S_CLOSE_BRACES,
        S_INPUT,
        S_RETURN,
        S_OUTPUT,
        S_DONE,
        S_ERROR
    };
    state_type state;

    enum assignment_state
    {
        A_START,
        A_IDENTIFIER,
        A_OPERATOR,
        A_EXPRESSION,
        A_DONE
    };
    assignment_state assign_state;

    enum declaration_state
    {
        D_START,
        D_KEYWORD,
        D_POINTER,
        D_IDENTIFIER,
        D_SEMICOLON,
        D_ASSIGNMENT,
        D_LITERAL_OR_IDENTIFIER,
        D_DONE
    };
    declaration_state decl_state;

    enum expression_state
    {
        E_START,
        E_LITERAL_OR_IDENTIFIER,
        E_OPERATOR,
        E_PARENTHESIS,
        E_PARENTHESIS_DONE,
        E_DONE
    };
    expression_state expr_state;

    enum IfState
    {
        IF_KEYWORD,
        IF_START,
        IF_OPEN_PAREN,
        IF_CONDITION,
        IF_CLOSE_PAREN,
        IF_BODY,
        IF_CLOSE_BRACES,
        IF_DONE
    };
    IfState if_state;

    enum InputState
    {
        I_START,
        I_KEYWORD,
        I_OPEN_PAREN,
        I_SIDENTIFIER,
        I_COMMA1,
        I_INTEGER_OR_LITERAL,
        I_COMMA2,
        I_IDENTIFIER,
        I_CLOSE_PAREN,
        I_DONE
    };
    InputState input_state;

    enum OutputState
    {
        O_START,
        O_KEYWORD,
        O_OPEN_PAREN,
        O_STRING,
        O_COMMA1,
        O_LITERAL_OR_IDENTIFIER,
        O_COMMA2,
        O_CLOSE_PAREN,
        O_DONE
    };
    OutputState output_state;

    enum ReturnState
    {
        R_START,
        R_KEYWORD,
        R_STRING,
        R_DONE
    };
    ReturnState return_state;

    int count_print;

    bool match(token_type type, token_subtype subtype = EMPTY, const string &value = "");
    bool return_statement();
    bool declaration();
    bool assignment();
    bool if_condition();
    bool expression(bool called_by_if = false);
    bool input_statement();
    bool output_statement();

public:
    afd_parser(std::vector<token> *tokens);
    bool parse();
    token le_token();
};

#endif // PARSER_HPP
