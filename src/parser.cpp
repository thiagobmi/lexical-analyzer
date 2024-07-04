#include "parser.hpp"

using namespace std;

bool afd_parser::match(token_type type, token_subtype subtype, const string &value)
{
    if (current >= tokens->size())
    {
        error_message = "Unexpected end of input.";
        return false;
    }

    token t = (*tokens)[current];

    if (t.type == type && (subtype == EMPTY || t.subtype == subtype) && (value.empty() || t.value == value))
    {
        // cout << "Matched token: " << t.value << endl;
        cout << t.value << " ";
        current++;
        return true;
    }

    error_message = "Expected token type " + lex.get_type_str(type);
    if (subtype != EMPTY)
    {
        error_message += " and subtype " + lex.get_subtype_str(subtype);
    }
    if (!value.empty())
    {
        error_message += " and value " + value;
    }
    error_message += " but got " + lex.get_type_str(type) + " with value " + t.value;
    return false;
}

bool afd_parser::return_statement()
{
    switch (return_state)
    {
    case R_START:
        if (match(KEYWORD, RETURN))
        {
            return_state = R_KEYWORD;
        }
        else
        {
            state = S_ERROR;
            error_message = "Expected 'return' keyword.";
            return false;
        }
        break;
    case R_KEYWORD:
        if (match(SEPARATOR, SEMICOLON))
        {
            return_state = R_DONE;
            state = S_DONE;
        }
        else if (match(LITERAL, STRING))
        {
            cout << "STRING LITERAL: " << (*tokens)[current - 1].value << endl;
            return_state = R_STRING;
        }
        else
        {
            state = S_EXPRESSION;
        }
        break;
    case R_STRING:
        if (match(SEPARATOR, SEMICOLON))
        {
            return_state = R_DONE;
            state = S_DONE;
        }
        else
        {
            state = S_ERROR;
            error_message = "Expected semicolon after string literal.";
            return false;
        }
        break;

    case R_DONE:
        return true;
    }
    return true;
}

bool afd_parser::declaration()
{
    static token_subtype data_type = EMPTY;
    static int count_pointer = 0;

    switch (decl_state)
    {
    case D_START:
        if (match(KEYWORD, DATA_TYPE) && lex.literal_map.count((*tokens)[current - 1].value))
        {
            data_type = lex.literal_map[(*tokens)[current - 1].value];
            decl_state = D_KEYWORD;
        }
        else
        {
            state = S_ERROR;
            error_message = "Invalid declaration start.";
            return false;
        }
        break;

    case D_KEYWORD:
        decl_state = D_POINTER;
        break;
    case D_POINTER:
        if (match(OPERATOR, EMPTY, "*"))
        {
            count_pointer++;
        }
        if (match(IDENTIFIER))
        {
            decl_state = D_IDENTIFIER;
        }
        else
        {
            state = S_ERROR;
            error_message = "Expected identifier after keyword.";
            return false;
        }
        break;
    case D_IDENTIFIER:
        if (match(SEPARATOR, SEMICOLON))
        {
            count_pointer = 0;
            data_type = EMPTY;
            decl_state = D_DONE;
            state = S_DONE;
        }
        else if (match(OPERATOR, ASSIGNMENT))
        {
            if (count_pointer == 1 && data_type == CHAR)
            {
                decl_state = D_ASSIGNMENT;
            }
            else if (count_pointer > 0)
            {
                state = S_ERROR;
                error_message = "Invalid assignment to pointer.";
                return false;
            }
            decl_state = D_ASSIGNMENT;
        }
        else
        {
            state = S_ERROR;
            error_message = "Expected semicolon or assignment after identifier.";
            return false;
        }
        break;
    case D_ASSIGNMENT:
        if (count_pointer == 1 && data_type == CHAR)
        {
            if (match(LITERAL, STRING))
            {
                decl_state = D_LITERAL_OR_IDENTIFIER;
            }
            else
            {
                state = S_ERROR;
                error_message = "Expected string literal after assignment.";
                return false;
            }
        }
        else
            state = S_EXPRESSION;
        break;
    case D_LITERAL_OR_IDENTIFIER:
        if (match(SEPARATOR, SEMICOLON))
        {
            decl_state = D_DONE;
            state = S_DONE;
            data_type = EMPTY;
            count_pointer = 0;
        }
        else
        {
            state = S_ERROR;
            error_message = "Expected semicolon after assignment.";
            return false;
        }
        break;
    case D_DONE:
        count_pointer = 0;
        data_type = EMPTY;
        return true;
    }
    return true;
}

bool afd_parser::assignment()
{
    switch (assign_state)
    {
    case A_START:
        if (match(IDENTIFIER))
        {
            assign_state = A_IDENTIFIER;
        }
        else
        {
            state = S_ERROR;
            error_message = "Expected identifier for assignment.";
            return false;
        }
        break;
    case A_IDENTIFIER:
        if (match(OPERATOR, ASSIGNMENT) || match(OPERATOR, ARITHMETIC_ASSIGNMENT))
        {
            assign_state = A_OPERATOR;
        }
        else
        {
            state = S_ERROR;
            error_message = "Expected assignment operator after identifier.";
            return false;
        }
        break;
    case A_OPERATOR:
        if (match(LITERAL, STRING))
        {
            assign_state = A_EXPRESSION;
        }
        else
            state = S_EXPRESSION;
        break;
    case A_EXPRESSION:
        if (match(SEPARATOR, SEMICOLON))
        {
            assign_state = A_DONE;
            state = S_DONE;
        }
        break;
    case A_DONE:
        return true;
    }
    return true;
}

bool afd_parser::if_condition()
{
    static int count_brackets = 0;
    switch (if_state)
    {
    case IF_KEYWORD:
        if (match(KEYWORD, CONTROL, "if"))
        {
            if_state = IF_START;
        }
        else
        {
            state = S_ERROR;
            error_message = "Expected 'if' keyword.";
            return false;
        }
        break;
    case IF_START:
        if (match(SEPARATOR, OPEN_PARENTHESIS))
        {
            if_state = IF_OPEN_PAREN;
        }
        else
        {
            state = S_ERROR;
            error_message = "Expected open parenthesis after 'if'.";
            return false;
        }
        break;
    case IF_OPEN_PAREN:
        state = S_CONDITION;
        break;
    case IF_CLOSE_PAREN:
        if (match(SEPARATOR, OPEN_BRACES))
        {
            if_state = IF_BODY;
        }
        else
        {
            state = S_ERROR;
            error_message = "Expected open braces after condition.";
            return false;
        }
        break;
    case IF_BODY:
        count_brackets++;
        count_print++;
        if_state = IF_DONE;
        break;
    case IF_CLOSE_BRACES:
        if (match(SEPARATOR, CLOSE_BRACES))
        {
            count_brackets--;
            count_print--;
            if (count_brackets >= 0)
            {
                if_state = IF_DONE;
                state = S_DONE;
            }
            else
            {
                current--;
                state = S_ERROR;
                error_message = "Unexpected closing brace.";
                return false;
            }
        }
        else
        {
            state = S_ERROR;
            error_message = "Expected closing brace.";
            return false;
        }
        break;
    case IF_DONE:
        state = S_DONE;
        return true;
    }
    return true;
}

bool afd_parser::expression(bool called_by_if)
{
    static int count_brackets = 0;
    switch (expr_state)
    {
    case E_START:
        if (match(IDENTIFIER) || match(LITERAL, INT) || match(LITERAL, FLOAT) || match(LITERAL, CHAR))
        {
            expr_state = E_LITERAL_OR_IDENTIFIER;
        }
        else if (match(SEPARATOR, OPEN_PARENTHESIS))
        {
            expr_state = E_PARENTHESIS;
        }
        else if (match(OPERATOR, UNARY) || match(OPERATOR, EMPTY, "&") || match(OPERATOR, EMPTY, "*"))
        {
            expr_state = E_START;
        }
        else
        {
            state = S_ERROR;
            error_message = "Invalid expression start.";
            return false;
        }
        break;
    case E_LITERAL_OR_IDENTIFIER:
        if (match(OPERATOR))
        {
            expr_state = E_OPERATOR;
        }
        else if (match(SEPARATOR, SEMICOLON))
        {
            expr_state = E_DONE;
            state = S_DONE;
        }
        else if (match(SEPARATOR, CLOSE_PARENTHESIS))
        {
            if (count_brackets == 0)
            {
                if (called_by_if)
                {
                    expr_state = E_DONE;
                    state = S_IF;
                    if_state = IF_CLOSE_PAREN;
                }
                else
                {
                    state = S_ERROR;
                    current--;
                    error_message = "Unexpected closing parenthesis.";
                    return false;
                }
            }
            else
            {
                count_brackets--;
                expr_state = E_LITERAL_OR_IDENTIFIER;
            }
        }
        else
        {
            state = S_ERROR;
            error_message = "Unexpected token in expression.";
            return false;
        }
        break;
    case E_OPERATOR:
        if (match(IDENTIFIER) || match(LITERAL, INT) || match(LITERAL, FLOAT) || match(LITERAL, CHAR))
        {
            expr_state = E_LITERAL_OR_IDENTIFIER;
        }
        else if (match(SEPARATOR, OPEN_PARENTHESIS))
        {
            expr_state = E_PARENTHESIS;
        }
        else
        {
            state = S_ERROR;
            error_message = "Expected literal or identifier after operator.";
            return false;
        }
        break;
    case E_PARENTHESIS:
        count_brackets++;
        expr_state = E_START;
        break;
    case E_DONE:
        return true;
    }
    return true;
}

bool afd_parser::input_statement()
{
    switch (input_state)
    {
    case I_START:
        if (match(IDENTIFIER, EMPTY, "fgets"))
        {
            input_state = I_KEYWORD;
        }
        break;
    case I_KEYWORD:
        if (match(SEPARATOR, OPEN_PARENTHESIS))
        {
            input_state = I_OPEN_PAREN;
        }
        else
        {
            state = S_ERROR;
            error_message = "Expected open parenthesis after 'fgets'.";
            return false;
        }
        break;
    case I_OPEN_PAREN:
        if (match(IDENTIFIER))
        {
            input_state = I_SIDENTIFIER;
        }
        else
        {
            state = S_ERROR;
            error_message = "Expected string, char, int, float or identifier after open parenthesis.";
            return false;
        }
        break;
    case I_SIDENTIFIER:
        if (match(SEPARATOR, COMMA, ","))
        {
            input_state = I_COMMA1;
        }
        else
        {
            state = S_ERROR;
            error_message = "Expected closing parenthesis after string or literal.";
            return false;
        }
        break;
    case I_COMMA1:
        if (match(LITERAL, INT) || match(IDENTIFIER))
        {
            input_state = I_INTEGER_OR_LITERAL;
        }
        else
        {
            state = S_ERROR;
            error_message = "Expected integer or literal after comma.";
            return false;
        }
        break;
    case I_INTEGER_OR_LITERAL:
        if (match(SEPARATOR, COMMA))
        {
            input_state = I_COMMA2;
        }
        else
        {
            state = S_ERROR;
            error_message = "Expected comma after integer or literal.";
            return false;
        }
        break;
    case I_COMMA2:
        if (match(IDENTIFIER))
        {
            input_state = I_IDENTIFIER;
        }
        else
        {
            state = S_ERROR;
            error_message = "Expected identifier after comma.";
            return false;
        }
        break;
    case I_IDENTIFIER:
        if (match(SEPARATOR, CLOSE_PARENTHESIS))
        {
            input_state = I_CLOSE_PAREN;
        }
        else
        {
            state = S_ERROR;
            error_message = "Expected closing parenthesis after identifier.";
            return false;
        }
        break;
    case I_CLOSE_PAREN:
        if (match(SEPARATOR, SEMICOLON))
        {
            input_state = I_DONE;
            state = S_DONE;
        }
        else
        {
            state = S_ERROR;
            error_message = "Expected semicolon after closing parenthesis.";
            return false;
        }
        break;

    case I_DONE:
        return true;
    }
    return true;
}

bool afd_parser::output_statement()
{

    switch (output_state)
    {
    case O_START:
        if (match(IDENTIFIER, EMPTY, "printf"))
        {
            output_state = O_KEYWORD;
        }
        break;
    case O_KEYWORD:
        if (match(SEPARATOR, OPEN_PARENTHESIS))
        {
            output_state = O_OPEN_PAREN;
        }
        else
        {
            state = S_ERROR;
            error_message = "Expected open parenthesis after 'printf'.";
            return false;
        }
        break;
    case O_OPEN_PAREN:
        if (match(LITERAL, STRING))
        {
            output_state = O_STRING;
        }
        else
        {
            state = S_ERROR;
            error_message = "Expected string after open parenthesis.";
            return false;
        }
        break;
    case O_STRING:
        if (match(SEPARATOR, COMMA))
        {
            output_state = O_COMMA1;
        }
        else if (match(SEPARATOR, CLOSE_PARENTHESIS))
        {
            output_state = O_CLOSE_PAREN;
        }
        else
        {
            state = S_ERROR;
            error_message = "Expected comma after string.";
            return false;
        }
        break;
    case O_COMMA1:
        if (match(IDENTIFIER) || match(LITERAL))
        {
            output_state = O_LITERAL_OR_IDENTIFIER;
        }
        else
        {
            state = S_ERROR;
            error_message = "Expected identifier or literal after comma.";
            return false;
        }
        break;
    case O_LITERAL_OR_IDENTIFIER:
        if (match(SEPARATOR, CLOSE_PARENTHESIS))
        {
            output_state = O_CLOSE_PAREN;
        }
        else if (match(SEPARATOR, COMMA))
        {
            output_state = O_COMMA2;
        }
        else
        {
            state = S_ERROR;
            error_message = "Expected closing parenthesis after identifier or literal.";
            return false;
        }
        break;
    case O_COMMA2:
        if (match(IDENTIFIER) || match(LITERAL))
        {
            output_state = O_LITERAL_OR_IDENTIFIER;
        }
        else
        {
            state = S_ERROR;
            error_message = "Expected identifier or literal after comma.";
            return false;
        }
        break;
    case O_CLOSE_PAREN:
        if (match(SEPARATOR, SEMICOLON))
        {
            output_state = O_DONE;
            state = S_DONE;
        }
        else
        {
            state = S_ERROR;
            error_message = "Expected semicolon after closing parenthesis.";
            return false;
        }
        break;

    case O_DONE:
        return true;
    }
    return true;
}

afd_parser::afd_parser(vector<token> *tokens) : tokens(tokens), current(0), state(S_INIT) {}

bool afd_parser::parse()
{
    bool done = false;
    count_print = 2;
    current = 0;
    decl_state = D_START;
    assign_state = A_START;
    expr_state = E_START;
    output_state = O_START;
    input_state = I_START;
    return_state = R_START;
    if_state = IF_START;

    while (current < tokens->size())
    {
        token t = le_token();
        if (state == S_ERROR)
        {
            cout << endl;
            cout << endl

                 << "Failed to parse at: " << t.value << endl;
            cout << "Error: " << error_message << endl;
            return false;
        }
    }

    return state == S_DONE;
}

token afd_parser::le_token()
{
    token t = (*tokens)[current];

    switch (state)
    {
    case S_INIT:
        assign_state = A_START;
        decl_state = D_START;
        expr_state = E_START;
        if_state = IF_KEYWORD;
        output_state = O_START;
        input_state = I_START;
        return_state = R_START;

        cout << endl;
        for(int i = 0; i < count_print; i++)
            cout << "   ";

        if (current < tokens->size())
        {
            if (t.type == KEYWORD && lex.literal_map.count(t.value))
            {
                state = S_DECLARATION;
                // cout << "DECLARATION" << endl;
            }
            else if (t.value == "return")
            {
                // cout << "RETURN_STATEMENT" << endl;
                state = S_RETURN;
            }
            else if (t.value == "fgets")
            {
                // cout << "INPUT_STATEMENT" << endl;
                state = S_INPUT;
            }
            else if (t.value == "printf")
            {
                // cout << "OUTPUT_STATEMENT" << endl;
                state = S_OUTPUT;
            }
            else if (t.type == KEYWORD && t.value == "if")
            {
                // cout << "IF_STATEMENT" << endl;
                state = S_IF;
            }
            else if (t.type == IDENTIFIER)
            {
                if (current + 1 < tokens->size() && (*tokens)[current + 1].type == OPERATOR &&
                    ((*tokens)[current + 1].subtype == ASSIGNMENT || (*tokens)[current + 1].subtype == ARITHMETIC_ASSIGNMENT))
                {
                    state = S_ASSIGNMENT;
                }
                else
                {
                    state = S_EXPRESSION;
                }
            }
            else if (t.type == LITERAL)
            {
                // cout << "EXPRESSION" << endl;
                state = S_EXPRESSION;
            }
            else if (t.value == "}")
            {
                state = S_IF;
                if_state = IF_CLOSE_BRACES;
            }
            else
            {
                error_message = "Unexpected token: " + t.value;
                state = S_ERROR;
                return t;
            }
        }
        break;

    case S_EXPRESSION:
        if (!expression())
        {
            state = S_ERROR;
            return t;
        }
        break;
    case S_CONDITION:
        if (!expression(true))
        {
            state = S_ERROR;
            return t;
        }
        break;
    case S_ASSIGNMENT:
        if (!assignment())
        {
            state = S_ERROR;
            return t;
        }
        break;
    case S_DECLARATION:
        if (!declaration())
        {
            state = S_ERROR;
            return t;
        }
        break;
    case S_IF:
        if (!if_condition())
        {
            state = S_ERROR;
            return t;
        }
        break;

    case S_INPUT:
        if (!input_statement())
        {
            state = S_ERROR;
            return t;
        }
        break;

    case S_OUTPUT:
        if (!output_statement())
        {
            state = S_ERROR;
            return t;
        }
        break;

    case S_RETURN:
        if (!return_statement())
        {
            state = S_ERROR;
            return t;
        }
        break;

    case S_DONE:
        state = S_INIT;
        return t;
    case S_ERROR:
        return t;
    }
    return t;
}