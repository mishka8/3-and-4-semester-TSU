#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>

using namespace std;

struct Node//узел дерева
{
    char key;//символ
    unsigned long long period_key;//частота символа

    Node *left;//левый потомок
    Node *right;//правый потомок
};

Node* getNode(char key, unsigned long long period_key, Node *left, Node *right)//выделение нового узла дерева
{
    Node* node = new Node();

    node->key = key;
    node->period_key = period_key;
    node->left = left;
    node->right = right;

    return node;
}

struct compare//сравнение для создания приоритета
{
    bool operator()(const Node *L, const Node *R)
    {
        return L->period_key > R->period_key;
    }
};


//вспомогательная функция для проверки?????
bool lastNode(Node *main)
{
    return main->left == nullptr && main->right == nullptr;
}

void TableHaffmanCodes(Node *main, string str, unordered_map<char, string> &HaffmanCodes)
{
    if(main == nullptr)
        return;

    if(!lastNode(main))
    {
        HaffmanCodes[main->key] = str;
    }

    TableHaffmanCodes(main->left, str + "0", HaffmanCodes);
    TableHaffmanCodes(main->right, str + "1", HaffmanCodes);
}

int main()
{
    cout << "Hello World!" << endl;
    return 0;
}
