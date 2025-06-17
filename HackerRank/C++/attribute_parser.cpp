#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <stack>

using namespace std;

bool is_closing_tag(string tag)
{
    if (tag[1] == '/')
        return true;
    else
        return false;
}

vector<string> tokenizer(string str, bool is_query = false)
{
    vector<string> tokens;
    string token;
    char c, s = ' ';

    if (is_query)
    {
        s = '.';
    }

    for (int i = 0; i <= str.length(); i++)
    {
        if ((i == str.length()) && (token != ""))
        {
            tokens.push_back(token);
        }
        else
        {
            c = str[i];
            if ((c == s) || (c == '=') || (c == '"') || (c == '~'))
            {
                if (token != "")
                {
                    tokens.push_back(token);
                }
                token = "";
            }
            else
            {
                token.push_back(c);
            }
        }
    }
    return tokens;
}

struct node_t
{
    string tag_name;
    unordered_map<string, string> attributes;
    unordered_map<string, node_t *> children;
};

int main()
{
    stack<node_t *> parents;

    int N, Q, token_start_index;

    vector<string> tokens;

    cin >> N >> Q;
    cin.ignore();

    string line, tag_name;
    unordered_map<string, node_t *> roots;
    node_t *node_ptr;

    bool tag_existed;

    string queries_results[Q];

    /* Take HRML lines from the user and process it*/
    for (int i = 0; i < N; i++)
    {
        getline(cin, line);

        if (is_closing_tag(line))
        {
            parents.pop();
        }
        else
        {
            /* Get the important values from the line */
            line = line.substr(1, line.length() - 2);
            tokens = tokenizer(line);

            /* Create the node */
            node_ptr = new node_t();
            node_ptr->tag_name = tokens.at(0);
            for (int i = 1; i < tokens.size() - 1; i++)
            {
                node_ptr->attributes.insert({tokens.at(i), tokens.at(i + 1)});
            }

            /* Assign the node to its specified parent */
            if (parents.empty())
            {
                roots.insert({node_ptr->tag_name, node_ptr});
            }
            else
            {
                parents.top()->children.insert({node_ptr->tag_name, node_ptr});
            }
            parents.push(node_ptr);
        }
    }

    /* Take queries from the user */
    for (int i = 0; i < Q; i++)
    {
        getline(cin, line);
        tokens = tokenizer(line, true);

        tag_existed = true;

        if (roots.find(tokens[0]) != roots.end())
        {
            node_ptr = roots[tokens[0]];
            for (int j = 1; j < tokens.size() - 1; j++)
            {
                if (node_ptr->children.find(tokens[j]) != node_ptr->children.end())
                {
                    node_ptr = node_ptr->children[tokens[j]];
                }
                else
                {
                    tag_existed = false;
                    break;
                }
            }

            if (tag_existed)
            {
                auto it = node_ptr->attributes.find(tokens[tokens.size() - 1]);
                if (it != node_ptr->attributes.end())
                {
                    queries_results[i] = it->second;
                }
                else
                {
                    queries_results[i] = "Not Found!";
                }
            }
            else
            {
                queries_results[i] = "Not Found!";
            }
        }
        else
        {
            queries_results[i] = "Not Found!";
        }
    }

    for (auto result : queries_results)
    {
        cout << result << endl;
    }

    return 0;
}
