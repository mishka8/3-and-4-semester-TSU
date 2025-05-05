#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <memory>
#include <algorithm>

using namespace std;
using uchar = unsigned char;

class Node {
public:

    using pointer = shared_ptr<Node>;

    pointer left{ nullptr };
    pointer right{ nullptr };
    pointer parent{ nullptr };

    Node() = default;
    Node(uchar uch, int f) : ch(uch), frequency(f) {}
    Node(const string& str, int f) : name(str), frequency(f) {}

    bool operator<(const Node& oth) const 
    {
        return frequency < oth.frequency;
    }

    friend ostream& operator<<(ostream& os, const Node& node);

    int get_frequency() const
    {
        return frequency;
    }

    void set_frequency(int f)
    {
        frequency = f;
    }

    char get_ch() const
    {
        return ch;
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
        if (ch == 0)
            return name;
        else
        {
            if (ch == 10)
                return "\\n";
            return string(1, static_cast<char>(ch));
        }
    }

    uchar get_byte() const
    {
        return ch;
    }


private:
    string name { "" };

    uchar ch { 0 };
    int frequency { 0 };
    string code_string { "" };
};

ostream& operator<<(ostream& os, const Node& node)
{
    return os << "[" << node.get_name() << "] = " << node.frequency;
}

class LowestPriority
{
public:
    bool operator()(const Node::pointer& left, const Node::pointer& right) const
    {
        if (left->get_frequency() == right->get_frequency())
        {
            return left->get_ch() > right->get_ch();
        }
        return left->get_frequency() > right->get_frequency();
    }
};

#endif //NODE_H