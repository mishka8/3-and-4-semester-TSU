#include "node.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <memory>
#include <algorithm>
using namespace std;

ifstream::pos_type get_size(const string& fn) 
{
    ifstream ifs(fn, ifstream::ate | ifstream::binary);
    return ifs.tellg();
}

void read_file(const string& filename, vector<int>& frequency)
{
    int filesize = get_size(filename);
    if (filesize < 0) 
    {
        cerr << "Error in [" << "]: ";
        return;
    }

    ifstream ifs(filename, ifstream::binary);
    if (!ifs) 
    {
        cerr << "Error in [" << "]: ";
        return;
    }

    while (true)
    {
        char ch;
        ifs.read(&ch, 1);

        if (ifs.eof())
        {
            break;
        }
        if (ch == 13)
            continue;
        frequency[static_cast<unsigned char>(ch)]++;
    }
    /*cout << endl;

    for (int i = 0; i < frequency.size(); ++i) 
    {
        if (frequency[i] != 0)
            cout << "[" << i << "] = " << frequency[i] << ", ";
    }
    cout << endl;*/
}

void make_code(Node::pointer& node, string str, vector<string>& codes)
{
    if (node->right != nullptr)
    {
        make_code(node->right, str + "1", codes);

    }
    if (node->left != nullptr)
    {
        make_code(node->left, str + "0", codes);

    }
    
    if (node->left == nullptr && node->right == nullptr)
    {
        node->code(str);
        codes[node->get_byte()] = str;
        cout << "leaf: " << *node << " code: " << node->code() << "\n";
    }

}

int main() 
{
    vector<int> frequency(0x100, 0);
    read_file("primer.txt", frequency);

    using que_type = priority_queue<Node::pointer, vector<Node::pointer>, LowestPriority>;
    que_type queue;
     
    //uchar byte = 0;
    /*for_each(frequency.begin(), frequency.end(), [&queue, &byte](const auto& value)
    {
        if (value != 0)
        {
            Node::pointer node = make_shared<Node>(byte, value);
            queue.push(node);
        }
        ++byte;
    });*/


    for (int i = 0; i < frequency.size(); ++i)//либо так либо for_each
    {
        if (frequency[i] != 0) {
            Node::pointer node = make_shared<Node>(i, frequency[i]);
            queue.push(node);
        }
    }
    
    while(queue.size() > 1)
    {
        Node::pointer x = queue.top();
        queue.pop();

        Node::pointer y = queue.top();
        queue.pop();

        string name = x->get_name() + y->get_name();
        Node::pointer z = make_shared<Node>(name, x->get_frequency() + y->get_frequency());

        z->left = x;
        z->right = y;
        
        x->parent = z;
        y->parent = z;

        queue.push(z);

    }
    
    vector<string> codes(0x100, "");
    Node::pointer root = queue.top();
    make_code(root, "", codes);


    /*while (!queue.empty())
    {
        cout << *(queue.top()) << " ";
        queue.pop();
    }
    cout << std::endl;*/


}