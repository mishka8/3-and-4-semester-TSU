#include <iostream>
#include <vector>
#include <random>
#include <time.h>

using namespace std;

class Node
{
    int key;
    Node* left;
    Node* right;

public:

    Node(int k = 0, Node* l = nullptr, Node* r = nullptr)
    {
        key = k;
        left = l;
        right = r;
    }

    ~Node() = default;

    int get_key()
    {
        return key;
    }

    friend class BinTree;
};

class BinTree
{
private:
    Node* root;

protected:
    //вспомогательные методы
    //тут написаны вспомогательные мтоды для каких-то операторов или методов
    //по сути выполняется принцип инкапсуляции
    //и так как они протектед если появится наследующий класс их можно будет переопределить

    //их можно сделать privite если мы не хотим ничего наследовать и методы friend не смогли бы их использовать и наследники не смогли бы их переопределить

    //или public но это нарушит принцип иникапсуляции
    void print_vertical(ostream& out, Node* node, int space = 0, int spacing = 4) const
    {
        if (!node) return;

        //сначала правое поддерево (чтобы было сверху)
        //рекурсивно обрабатываем правое поддерево
        //добавляем пробелы
        print_vertical(out, node->right, space + spacing, spacing);

        // Вывод текущего узла
        out << string(space, ' ');
        out << node->key << endl;

        // Затем левое поддерево (чтобы было снизу)
        //то же самое
        print_vertical(out, node->left, space + spacing, spacing);
    }

    Node* copy_tree(Node* tree)
    {
        if (!tree)
            return nullptr;

        Node* copy = new Node(tree->key);
        copy->left = copy_tree(tree->left);
        copy->right = copy_tree(tree->right);

        return copy;
    }

    void delete_tree(Node* tree)
    {
        if (tree == nullptr)
            return;

        delete_tree(tree->left);
        delete_tree(tree->right);

        delete tree;
    }

public:
    BinTree()
    {
        root = nullptr;
    }

    BinTree(int k)
    {
        root = new Node(k);
    }

    BinTree(const BinTree& tree)
    {
        if (tree.root == nullptr)
            root = nullptr;

        root = new Node(tree.root->key);


        root->left = copy_tree(tree.root->left);
        root->right = copy_tree(tree.root->right);
    }

    BinTree(Node* node)
    {
        root = node;
    }

    ~BinTree()
    {
        delete_tree(root);
    }

    BinTree& operator= (const BinTree& other)
    {
        if (this == &other)
            return *this;

        delete_tree(root);
        root = copy_tree(other.root);

        return *this;
    }

    Node* find_node(int val)
    {
        Node* node = root;

        while (node && node->key == val)
        {
            if (val < node->key)
                node = node->left;
            else
                node = node->right;
        }

        return node;
    }

    void add(int val)
    {
        Node* new_node = new Node(val);

        Node* prew = nullptr;
        Node* node = root;
        while (node)
        {
            prew = node;

            if (new_node->key < node->key)
                node = node->left;

            else
                node = node->right;
        }

        if (prew == nullptr)
            root = new_node;
        else if (val < prew->key)
            prew->left = new_node;
        else
            prew->right = new_node;

    }

    void del_node(int val)
    {
        Node* node = root;
        Node* prew = nullptr;

        while (node && node->key != val)
        {
            prew = node;

            if (val < node->key)
                node = node->left;
            else
                node = node->right;
        }

        if (!node)
        {
            cout << " value is not found" << endl;
            return;
        }

        if (!node->left && !node->right)//узел не имеет поддеревьев
        {
            if (node == root)
                root = nullptr;

            else if (node == prew->left)
                prew->left = nullptr;

            else
                prew->right = nullptr;

            delete node;
        }

        //узел имеет одно поддерево
        //зменяем удаляемый узел его единственным потомком
        //обновляем ссылку у родителя
        else if (!node->left && node->right)
        {
            if (node == root)
                root = node->right;

            else if (node == prew->left)
                prew->left = node->right;

            else
                prew->right = node->right;

            delete node;
        }

        //узел имеет одно поддерево
        //зменяем удаляемый узел его единственным потомком
        //обновляем ссылку у родителя
        else if (node->left && !node->right)
        {
            if (node == root)
                root = node->left;

            else if (node == prew->left)
                prew->left = node->left;

            else
                prew->right = node->left;

            delete node;
        }

        //минимальный узел в правом поддереве
        //максимальный в правом поддереве
        //копируем значение нужное значение в удаляемый узел
        //удаляем найденный узел-замену
        else
        {
            Node* minRight = node->right;//переходим к следующему
            Node* minRightParent = node;

            while (minRight->left)//ищем самый левый минимальный узел в правом поддереве
            {
                minRightParent = minRight;
                minRight = minRight->left;
            }

            // Копируем значение минимального узла в тот который удаляем
            node->key = minRight->key;

            //Удаляем оригинальный минимальный узел
            if (minRight == minRightParent->left)
                minRightParent->left = minRight->right;
            else
                minRightParent->right = minRight->right;

            delete minRight;
        }
        //на примере 5
        //node = 5
        //node->right = 7
        //и дальше ищем минимальный справа таким образом что переходим каждый раз влево
        //запоминаем минимальный справа дальше вставляем его на место удаляемого
        //и потом удаляем минимальный на том месте где мы его нашли
        // 
        //Исходное:       После замены:      После удаления:
        //     5               6                 6
        //    / \             / \               / \
        //   3    7          3   7             3   7
        //  / \  / \        / \ / \           / \   \
        // 2  4 6   8      2  4 6  8         2   4   8
    }

    Node* min_node()
    {
        Node* mnode = root;
        while (mnode && mnode->left)
            mnode = mnode->left;

        return mnode;
    }

    Node* max_node()
    {
        Node* mxnode = root;
        while (mxnode && mxnode->right)
            mxnode = mxnode->right;

        return mxnode;
    }

    friend ostream& operator<<(ostream& out, const BinTree& tree)
    {
        if (!tree.root)
        {
            out << "[Empty tree]";
            return out;
        }

        tree.print_vertical(out, tree.root);
        return out;
    }

    vector<Node*> way_in_width()
    {
        vector<Node*> res;

        if (!root)
            return res;

        vector<Node*> level;
        level.push_back(root);

        while (!level.empty())//посещаем все узлы текущего уровня
        {
            vector<Node*> next_level;

            for (auto node : level)
            {
                cout << node->key << " ";
                res.push_back(node);

                if (node->left)
                    next_level.push_back(node->left);

                if (node->right)
                    next_level.push_back(node->right);
            }

            level = next_level;//переход к следующему узлу
        }

        return res;
    }

    void left_node_right()//лево центр право
    {
        if (!root)
            return;

        vector<Node*> st;
        Node* node = root;

        while (node || !st.empty())
        {
            while (node)
            {
                st.push_back(node);
                node = node->left;
            }

            if (!st.empty())
            {
                node = st.back();
                st.pop_back();

                cout << node->key << " ";
                node = node->right;
            }
        }
        cout << endl;
    }
};

int main()
{
    srand(time(0));


    BinTree tree;
    tree.add(5);
    tree.add(3);
    tree.add(7);
    tree.add(2);
    tree.add(4);
    tree.add(6);
    tree.add(10);
    tree.add(8);
    tree.add(15);

    cout << tree << endl;

    tree.del_node(5);//переделать удаление

    cout << tree << endl;


    //Node* min = tree.min_node();
    //cout << "Minimum: " << min->get_key() << endl;

    //Node* max = tree.max_node();
    //cout << "Maximum: " << max->get_key() << endl;
    //cout << endl;


    //cout << "left_node_right" << endl;
    //tree.left_node_right();  // 2 3 4 5 6 7 8
    //cout << endl;

    //cout << "way_in_weight" << endl;
    //tree.way_in_width();
    //cout << endl;


    return 0;
}
