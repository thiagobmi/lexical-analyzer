#include "tokenizer.hpp"

tokenizer::tokenizer()
{
    types.push_back({KEYWORD, {"int", "float", "char", "double", "void", "if", "else", "for", "while", "do", "return"}});
    types.push_back({SEPARATOR, {"(", ")", "{", "}", "[", "]", ";", ","}});
    types.push_back({OPERATOR, {"+", "-", "*", "/", "%", "++", "--", "==", "!=", ">", "<", ">=", "<=", "&&", "||", "!", "=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", ">>=", "~", "&", "|", "^", "<<", ">>"}});

    subtypes[KEYWORD] = {
        {DATA_TYPE, {"int", "float", "char", "double", "void"}},
        {CONTROL, {"if", "else", "for", "while", "do"}},
        {RETURN, {"return"}}};

    subtypes[OPERATOR] = {
        {ARITHMETIC, {"+", "-", "*", "/", "%"}},
        {RELATIONAL, {"==", "!=", ">", "<", ">=", "<="}},
        {LOGICAL, {"&&", "||", "&", "|", "^", "<<", ">>"}},
        {ASSIGNMENT, {"="}},
        {ARITHMETIC_ASSIGNMENT, {"+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", ">>="}},
        {ARITHMETIC_UNARY, {"++", "--"}},
        {UNARY, {"~", "!", "&", "*"}}};

    subtypes[LITERAL] = {
        {STRING, {}},
        {CHAR, {}},
        {INT, {}},
        {FLOAT, {}}};

    subtypes[SEPARATOR] = {
        {OPEN_PARENTHESIS, {"("}},
        {CLOSE_PARENTHESIS, {")"}},
        {OPEN_BRACES, {"{"}},
        {CLOSE_BRACES, {"}"}},
        {OPEN_BRACKETS, {"["}},
        {CLOSE_BRACKETS, {"]"}},
        {SEMICOLON, {";"}},
        {COMMA, {","}}};

    literal_map["char"] = CHAR;
    literal_map["int"] = INT;
    literal_map["float"] = FLOAT;
    literal_map["double"] = FLOAT;

    type_to_str[KEYWORD] = "KEYWORD";
    type_to_str[SEPARATOR] = "SEPARATOR";
    type_to_str[OPERATOR] = "OPERATOR";
    type_to_str[IDENTIFIER] = "IDENTIFIER";
    type_to_str[LITERAL] = "LITERAL";

    subtype_to_str[ARITHMETIC] = "ARITHMETIC";
    subtype_to_str[RELATIONAL] = "RELATIONAL";
    subtype_to_str[LOGICAL] = "LOGICAL";
    subtype_to_str[ASSIGNMENT] = "ASSIGNMENT";
    subtype_to_str[ARITHMETIC_ASSIGNMENT] = "ARITHMETIC_ASSIGNMENT";
    subtype_to_str[ARITHMETIC_UNARY] = "ARITHMETIC_UNARY";
    subtype_to_str[UNARY] = "UNARY";
    subtype_to_str[DATA_TYPE] = "DATA_TYPE";
    subtype_to_str[CONTROL] = "CONTROL";
    subtype_to_str[RETURN] = "RETURN";
    subtype_to_str[STRING] = "STRING";
    subtype_to_str[CHAR] = "CHAR";
    subtype_to_str[INT] = "INT";
    subtype_to_str[FLOAT] = "FLOAT";
    subtype_to_str[OPEN_PARENTHESIS] = "OPEN_PARENTHESIS";
    subtype_to_str[CLOSE_PARENTHESIS] = "CLOSE_PARENTHESIS";
    subtype_to_str[OPEN_BRACES] = "OPEN_BRACES";
    subtype_to_str[CLOSE_BRACES] = "CLOSE_BRACES";
    subtype_to_str[OPEN_BRACKETS] = "OPEN_BRACKETS";
    subtype_to_str[CLOSE_BRACKETS] = "CLOSE_BRACKETS";
    subtype_to_str[SEMICOLON] = "SEMICOLON";
    subtype_to_str[COMMA] = "COMMA";
}

string tokenizer::get_subtype_str(token_subtype t)
{
    return subtype_to_str[t];
}

string tokenizer::get_type_str(token_type t)
{
    return type_to_str[t];
}

vector<token> *tokenizer::get_tokens() { return tokens; }

bool tokenizer::is_keyword(const string &s)
{
    return find(types[KEYWORD].second.begin(), types[KEYWORD].second.end(), s) != types[KEYWORD].second.end();
}

bool tokenizer::is_separator(const string &s)
{
    return find(types[SEPARATOR].second.begin(), types[SEPARATOR].second.end(), s) != types[SEPARATOR].second.end();
}

bool tokenizer::is_operator(const string &s)
{
    return find(types[OPERATOR].second.begin(), types[OPERATOR].second.end(), s) != types[OPERATOR].second.end();
}

bool tokenizer::is_digit(char c) // dígito: Padrão de formação: 0-9
{
    return c >= '0' && c <= '9';
}

bool tokenizer::is_integer(const string &s) // int: Padrão de formação: dígito+
{
    for (char c : s)
    {
        if (!is_digit(c))
            return false;
    }
    return true;
}

//
bool tokenizer::is_literal(const string &s)
{
    if ((s[0] == '"' && s[s.length() - 1] == '"') || // string: Padrão de formação: "(letra+dígito+simbolos)*"
        (s[0] == '\'' && s[s.length() - 1] == '\'')) // char: Padrão de formação: '(letra+dígito+simbolos)*' (C aceita declarações de char com mais de um caractere)
        return true;

    if (s.find('.') != string::npos) // float: Padrão de formação: dígito+.dígito+
        return is_integer(s.substr(0, s.find('.')) + s.substr(s.find('.') + 1));

    if (is_integer(s)) // int: Padrão de formação: dígito+
        return true;

    return false;
}

// IDENTIFIER: ; Padrão de formação: letra(letra+dígito)*
bool tokenizer::is_identifier(const string &s)
{
    if (isdigit(s[0])) // Identificadores não podem começar com dígitos
    {
        return false;
    }

    for (char c : s) // Identificadores podem conter letras, dígitos e o caractere '_'
    {
        if (!isdigit(c) && !isalpha(c) && c != '_')
            return false;
    }
    return !is_keyword(s) && !is_literal(s) && !is_operator(s) && !is_separator(s) && !is_digit(s[0]);
}

token_subtype tokenizer::get_literal_subtype(const string &s)
{
    if (s[0] == '"' && s[s.length() - 1] == '"')
        return STRING;
    else if (s[0] == '\'' && s[s.length() - 1] == '\'')
        return CHAR;
    else if (s.find('.') != string::npos)
        return FLOAT;
    else if (is_integer(s))
        return INT;

    return EMPTY;
}

token_subtype tokenizer::get_operator_subtype(const string &s)
{
    for (const auto &a : subtypes[OPERATOR])
    {
        if (find(a.second.begin(), a.second.end(), s) != a.second.end())
        {
            return a.first;
        }
    }
    return EMPTY;
}

token_subtype tokenizer::get_keyword_subtype(const string &s)
{
    for (const auto &a : subtypes[KEYWORD])
    {
        if (find(a.second.begin(), a.second.end(), s) != a.second.end())
        {
            return a.first;
        }
    }
    return EMPTY;
}

token_subtype tokenizer::get_separator_subtype(const string &s)
{
    for (const auto &a : subtypes[SEPARATOR])
    {
        if (find(a.second.begin(), a.second.end(), s) != a.second.end())
        {
            return a.first;
        }
    }
    return EMPTY;
}

vector<token> *tokenizer::tokenize(const string &s)
{
    string temp = "";
    vector<token> *toks = new vector<token>;
    bool possibly_string = false;

    for (size_t i = 0; i < s.size() + 1; ++i)
    {
        char a = s[i];
        if (a == '"')
            possibly_string = !possibly_string;

        // Ignorar comentários de uma linha
        if (a == '/' && i + 1 < s.size() && s[i + 1] == '/')
        {
            while (i < s.size() && s[i] != '\n')
                ++i;
            continue;
        }

        if ((a == ' ' || a == '\n' || a == '\t' || is_operator(string(1, a)) || is_separator(string(1, a)) || i == s.size()) && !possibly_string)
        {
            if (!temp.empty())
            {
                if (is_keyword(temp))
                {
                    token t = {KEYWORD};
                    t.subtype = get_keyword_subtype(temp);
                    t.value = temp;
                    toks->push_back(t);
                }
                else if (is_literal(temp))
                {
                    toks->push_back({LITERAL, get_literal_subtype(temp), temp});
                }
                else if (is_identifier(temp))
                {
                    toks->push_back({IDENTIFIER, IDENTIFIER_SUBTYPE, temp});
                }
                else
                {
                    cout << "ERROR: INVALID TOKEN: " << temp << endl;
                    return nullptr;
                }
                temp = "";
            }

            if (is_operator(string(1, a)))
            {
                if (i + 1 < s.size() && is_operator(string(1, a) + string(1, s[i + 1])))
                {
                    token t = {OPERATOR};
                    t.subtype = get_operator_subtype(string(1, a) + string(1, s[i + 1]));
                    t.value = string(1, a) + string(1, s[i + 1]);
                    toks->push_back(t);
                    i++;
                }
                else
                {
                    token t = {OPERATOR};
                    t.subtype = get_operator_subtype(string(1, a));
                    t.value = string(1, a);
                    toks->push_back(t);
                }
            }

            if (is_separator(string(1, a)))
            {
                token t = {SEPARATOR};
                t.subtype = get_separator_subtype(string(1, a));
                t.value = string(1, a);
                toks->push_back(t);
            }
        }
        else
        {
            temp += a;
        }
    }
    tokens = toks;
    return tokens;
}

void tokenizer::print_tokens(vector<token> *ts)
{
    if (ts == nullptr)
        ts = tokens;

    for (const auto &t : *ts)
    {
        cout << "Token: " << t.value << " " << type_to_str[t.type];
        if (t.subtype != -1)
        {
            cout << " " << subtype_to_str[t.subtype] << endl;
        }
    }
}