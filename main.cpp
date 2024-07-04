#include "./src/parser.hpp"
#include "./src/tokenizer.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
int main(int argc, char **argv)
{

    if (argc > 1)
    {
        string filename = argv[1];
        ifstream file(filename);

        if (!file.is_open())
        {
            cout << "Could not open file " << filename << endl;
            return 1;
        }

        string line;
        string file_contents;
        while (getline(file, line))
        {
            file_contents += line + "\n";
        }

        tokenizer t;
        vector<token> *tokens = t.tokenize(file_contents);
        afd_parser p(tokens);

        cout << endl
             << "------------ Parsing " << filename << " -----------" << endl;
        if (p.parse())
        {
            cout << endl
                 << endl
                 << "Parsing " << filename << " SUCCESSFUL" << endl;
        }
        else
        {
            cout << endl
                 << "Parsing " << filename << " FAILED" << endl;
        }
    }
    else
    {

        string filename_1 = "./exemplos/exemplo_1.c";
        string filename_2 = "./exemplos/exemplo_2.c";

        vector<string> filenames = {filename_1, filename_2};

        for (const string &filename : filenames)
        {
            ifstream file(filename);

            if (!file.is_open())
            {
                cout << "Could not open file " << filename << endl;
                return 1;
            }

            string line;
            string file_contents;
            while (getline(file, line))
            {
                file_contents += line + "\n";
            }

            tokenizer t;
            vector<token> *tokens = t.tokenize(file_contents);
            afd_parser p(tokens);

            cout << endl
                 << "----------- Parsing " << filename << " -----------" << endl;
            if (p.parse())
            {
                cout << endl
                     << endl
                     << "[Parsing " << filename << " SUCCESSFUL]" << endl;
            }
            else
            {
                cout << endl
                     << "[Parsing " << filename << " FAILED]" << endl;
            }
        }
    }
    return 0;
}