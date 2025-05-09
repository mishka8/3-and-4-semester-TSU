#include "node.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <memory>
#include <algorithm>
#include <bitset>

using namespace std;
using que_type = priority_queue<Node::pointer, vector<Node::pointer>, LowestPriority>;

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
        cerr << "Error with size ";
        return;
    }

    ifstream ifs(filename, ifstream::binary);
    if (!ifs) 
    {
        cerr << "Error file is not open ";
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
}

void make_code(Node::pointer& node, string str, vector<string>& codes)
{
    if (node->left != nullptr)
    {
        make_code(node->left, str + "0", codes);

    }

    if (node->right != nullptr)
    {
        make_code(node->right, str + "1", codes);

    }

    if (node->left == nullptr && node->right == nullptr)
    {
        node->code(str);
        codes[node->get_byte()] = str;
    }
}

void filling_queue(vector<int> frequency, que_type& queue)
{
    for (int i = 0; i < frequency.size(); ++i)//либо так либо for_each
    {
        if (frequency[i] != 0) {
            Node::pointer node = make_shared<Node>(i, frequency[i]);
            queue.push(node);
        }
    }
}

void build_tree(que_type& queue)
{
    while (queue.size() > 1)
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
}

string FileToString(const string& filename,const vector<string>& codes)
{
    string text{ "" };
    ifstream ifs(filename, ifstream::binary);
    if (!ifs)
    {
        cerr << "Error";
        return text;
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
        text += codes[static_cast<uchar>(ch)];
    }
    
    return text;
}

void write_file(const string& filename, vector<int> freq, const que_type& queue, const string text)
{
    ofstream out("encode_" + filename, ios::binary);
    if (!out)
    {
        cerr << "File is not open";
        return;
    }

    int count = count_if(freq.begin(), freq.end(), [](int x) { return x > 0; });
    out.write(reinterpret_cast<const char*>(&count), sizeof(int));

    for (int i = 0; i < freq.size(); ++i)
    {
        if (freq[i] > 0)
        {
            uchar ch = static_cast<uchar>(i);
            out.write(reinterpret_cast<const char*>(&ch), sizeof(uchar));
            out.write(reinterpret_cast<const char*>(&freq[i]), sizeof(int));
        }
    }

    int bit_count = text.size();
    out.write(reinterpret_cast<const char*>(&bit_count), sizeof(int));

    string buffer;
    for (char bit : text)
    {
        buffer += bit;
        if (buffer.size() == 8)
        {
            bitset<8> bits(buffer);
            char byte = static_cast<char>(bits.to_ulong());
            out.write(&byte, 1);
            buffer.clear();
        }
    }

    if (!buffer.empty())
    {
        while (buffer.size() < 8)
        {
            buffer += '0';
        }
        bitset<8> bits(buffer);
        char byte = static_cast<char>(bits.to_ulong());
        out.write(&byte, 1);
    }


    cout << "Encode file " << "encode_" << filename  << endl;
    out.close();
}

Node::pointer read_compressed(const string& filename, vector<int>& freq, string& text_encode)
{
    ifstream in(filename, ios::binary);
    if (!in)
    {
        cerr << "File is not open" << endl;
        return nullptr;
    }

    int unique_count;
    in.read(reinterpret_cast<char*>(&unique_count), sizeof(int));


    freq.assign(256, 0);
    for (int i = 0; i < unique_count; ++i)
    {
        uchar ch;
        int frequency;
        in.read(reinterpret_cast<char*>(&ch), sizeof(uchar));
        in.read(reinterpret_cast<char*>(&frequency), sizeof(int));
        freq[ch] = frequency;
    }

    int bit_count;
    in.read(reinterpret_cast<char*>(&bit_count), sizeof(int));

    text_encode.clear();
    char byte;
    while (in.read(&byte, 1))
    {
        bitset<8> bits(static_cast<uchar>(byte));
        text_encode += bits.to_string();
    }

    if (text_encode.size() > bit_count)
    {
        text_encode.resize(bit_count);
    }

    que_type queue;
    filling_queue(freq, queue);
    build_tree(queue);

    return queue.top();
}

void write_decode_file(const string& filename, const string& encode_text, Node::pointer root)
{
    ofstream out("decode_" + filename, ios::binary);
    if (!out)
    {
        cerr << "file is not open" << endl;
        return;
    }

    Node::pointer current = root;
    for (char bit : encode_text)
    {
        if (bit == '0')
        {
            current = current->left;
        }
        else if (bit == '1')
        {
            current = current->right;
        }
        
        if (current->chek())
        {
            char ch = static_cast<char>(current->get_byte());
            out.write(&ch, 1);

            current = root;

        }
    }

    out.close();
    cout << "Decoded file written as "<< "decode_" << filename << endl;
}


void Haffman_encode_decode(const string& filename)
{
    vector<int> frequency(256, 0);
    read_file(filename, frequency);

    que_type queue;
    filling_queue(frequency, queue);


    build_tree(queue);
    vector<string> codes(0x100, "");


    Node::pointer root = queue.top();
    make_code(root, "", codes);


    string text = FileToString(filename, codes);

    write_file(filename, frequency, queue, text);


    vector<int> freq_encode(256, 0);
    string read_encode = "";
    Node::pointer root_encode = read_compressed(filename + ".ehff", freq_encode, read_encode);

    
    if (!root_encode)
    {
        cerr << "ERROR" << endl;
        return;
    }
    write_decode_file(filename, read_encode, root_encode);
}






int main() 
{
    //string filename = "primer.txt";
    //string filename = "primer2.txt";
    string filename = "primer3.txt";
    
    Haffman_encode_decode(filename);
}