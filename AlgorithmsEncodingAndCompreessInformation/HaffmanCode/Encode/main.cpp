//#include <node.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <memory>
#include <algorithm>
using namespace std;

void read_file(const string& filename, vector<int>& frequency)//функция чтения файла
{
    ifstream ifs(filename, ifstream::binary);
    if (!ifs)
    {
        cerr << "ERROR File is not open" << endl;
        return;
    }

    char ch;
    while(ifs.get(ch))
    {
        frequency[static_cast<unsigned char>(ch)]++;        
    }
}

//файл node.hpp
//#indef NODE_H
//#define NODE_H
using uchar = unsigned char;

class Node
{
public:
    using pointer = shared_ptr<Node>;

    pointer left{ nullptr };
    pointer right{ nullptr };
    pointer parent{ nullptr };


    Node() = default;
    Node(uchar uch, int f) : ch(uch), freq(f) {}
    Node(const string& str, int f) : name(str), freq(f) {}

    
    int get_freq() const
    {
        return freq;
    }

    void set_freq(int f)
    {
        freq = f;
    }


    string code() const
    {
        return code_string;
    }

    void code(const string& c)
    {
        code_string = c;
    }

    string get_name() const
    {
        if (ch == 0)//значит узел объединенный
            return name;
        else
        {
            if (ch == 10)
            {
                return "\\n";
            }
            if (ch == 32)
                return " ";
            return string(1, static_cast<char>(ch));
        }
    }
    
    
    bool operator<(const Node& other) const
    {
        if (freq < other.freq)
            return true;
            
        return false;
    }

    friend ostream& operator<<(ostream& os, const Node& node)
    {
        return os << "[" << node.get_name() << "] = " << node.freq;
    }

private:

    uchar ch{ 0 };
    int freq{ 0 };
    string name{ "" };
    string code_string{ "" };
};

class LowPriority
{
public:
    bool operator()(Node::pointer& left, const Node::pointer& right) const
    {
        return left->get_freq() > right->get_freq();
    }

};





//#endif //NODE_H
//конец файла

int main() 
{
    vector<int> freq_m(256, 0);
    read_file("primer.txt", freq_m);

    using que_type = priority_queue<Node::pointer, vector<Node::pointer>, LowPriority>;

    que_type queue;

    uchar byte = 0;
    //for_each удобнее
    //и ещё используем функцию лямбда
    /*for_each(freq_m.begin(), freq_m.end(), [&queue, &byte](const auto& value)
    {
        if (value != 0)
        {
            Node::pointer node = make_shared<Node>(byte, value);
            queue.push(node);
        }
        ++byte;
    });*/
    for (int i = 0; i < 256; ++i)
    {
        if (freq_m[i] != 0)
        {
            Node::pointer node = make_shared<Node>(static_cast<unsigned char>(i), freq_m[i]);
            queue.push(node);
        }
    }
    //неправильно работает чтение файла не хватает пару элементов 
    //и из-за этого неправильно строиться дерево
    //осталось заполнить дерево 0 и 1 
    //закодировать записать в файл
    //и декодировать

    while(queue.size() > 1)
    {
        Node::pointer x = queue.top();
        queue.pop();

        Node::pointer y = queue.top();
        queue.pop();


        string name = x->get_name() + y->get_name();
        Node::pointer z = make_shared<Node>(0, x->get_freq() + y->get_freq());

        z->left = x;
        z->right = y;

        x->parent = z;
        y->parent = z;

        queue.push(z);
    }

    while (!queue.empty())
    {
        cout << *(queue.top()) << " ";
        queue.pop();

    }

    return 0;
}
