#include <iostream>
#include <vector>

using namespace std;

class Node
{
    int key;
    Node* left;
    Node* right;
    int hight;

public:

    Node(int k = 0, Node* l = nullptr, Node* r = nullptr, int h = 0)
    {
        key = k;
        left = l;
        right = r;
        hight = h;
    }

    ~Node() = default;

    int get_key()
    {
        return key;
    }

    int get_hight()
    {
        return hight;
    }

    void set_hight(int h)
    {
        hight = h;
    }

    friend class BinTree;
    friend class AvlTree;
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

    friend class AvlTree;
};

class AvlTree : public BinTree
{
private:
    void updateHeight(Node* node)//чтобы использвоал только этот класс
    {
        //вычисляем высоты поддеревьев
        int leftH = (node->left) ? node->left->get_hight() : 0;//проверяем наличие поддерева и берем его высоту если есть или -1 если его нет 
        int rightH = (node->right) ? node->right->get_hight() : 0;

        node->set_hight(max(leftH, rightH) + 1);//устанавливаем новую высоту и новый баланс
    }
    
    int getBalance(Node* node)
    {
        if (!node) 
            return 0;
        
        int leftH = (node->left) ? node->left->hight : 0;
        int rightH = (node->right) ? node->right->hight : 0;
        
        return rightH - leftH;
    }


public:
    //r: баланс -2; слева s: баланс -1 одинарный правый поворот 
    //r->left = s->right;
    //s->right = r;
    //return s;
    Node* RTurn(Node* r)//когда левое поддерево 
    {
        if (!r || !r->left)
            return r;

        Node* s = r->left;//запоминаем левого потомка

        r->left = s->right;//перемещаем правое в левое
        s->right = r;//делаем r правым потомком в s

        updateHeight(r);//балансим дополнительно
        updateHeight(s);

        return s;
    }

    //Однократный поворот налево L
    //r: баланс +2; справа s: баланс +1
    //r->right = s->left;
    //s->left = r;
    //return s;
    Node* LTurn(Node* r)
    {
        if (!r || !r->right)
            return r;
        
        Node* s = r->right;
        r->right = s->left;
        s->left = r;

        updateHeight(r);//балансим допольнительно
        updateHeight(s);

        return s;
    }

    //Двукратный поворот LR выполняется в случае
    //когда баланс в вершине
    //становится равным - 2, а баланс в потомке слева равен + 1
    //r: баланс -2; слева s: баланс +1.
    //s->right = p->left;
    //p->left = s;
    //r->left = p->right;
    //p->right = r;
    //return p;
    Node* LRTurn(Node* r)
    {
        if (!r || !r->right)
            return r;

        Node* s = r->left;
        if (!s || !s->right)
            return r;

        Node* p = s->right;

        s->right = p->left;
        p->left = s;

        r->left = p->right;
        p->right = r;

        updateHeight(s);//балансим 
        updateHeight(r);
        updateHeight(p);


        return p;
    }
    
    //Двукратный поворот RL(направо - налево) выполняется в случае
    //когда баланс в вершине
    //становится равным + 2, а баланс в потомке справа равен - 1
    //r: баланс + 2; справа s : баланс - 1.
    //s->left = p->right;
    //p->right = s;
    //r->right = p->left;
    //p->left = r;
    //return p;
    Node* RLTurn(Node* r) 
    { 
        if (!r || !r->right) 
            return r;
        
        Node* s = r->right;
        if (!s || !s->left) 
            return r;
        
        Node* p = s->left;

        s->left = p->right;
        p->right = s;

        r->right = p->left;
        p->left = r;

        updateHeight(s);//балансим
        updateHeight(r);
        updateHeight(p);

        return p;
    }
    


    Node* insertBalance(Node* node, int key)
    {
        if (node == nullptr)//дошли до места вставки
            return new Node(key, nullptr, nullptr, 0);

        //спускаемся по дереву смотрим куда идти влево или вправо
        if (key < node->key)
            node->left = insertBalance(node->left, key);
        else if (key > node->key)
            node->right = insertBalance(node->right, key);
        else
            return node;

        updateHeight(node);//обновляем высоту
        
        int balanse = getBalance(node);

        if (balanse == -2)//проверка баланса и подбираем по условиям нужный поворот
        {
            if (getBalance(node->left) == 1)
                return LRTurn(node);//двойной поворот когда разные знаки 
            else
                return RTurn(node);//елси одинаковые знаки то одинарный поворот
        }
        else if (balanse == 2)
        {
            if (getBalance(node->right) == -1)
                return RLTurn(node);
            else
                return LTurn(node);
        }

        return node;
    }
    
    
    void insert(int key)
    {
        root = insertBalance(root, key);
    }
};


int main()
{
    //srand(time(0));

    //BinTree tree;
    //tree.add(5);
    //tree.add(3);
    //tree.add(7);
    //tree.add(2);
    //tree.add(4);
    //tree.add(6);
    //tree.add(10);
    //tree.add(8);
    //tree.add(15);

    //cout << tree << endl;

    //tree.del_node(5);//переделать удаление

    //cout << tree << endl;

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

    AvlTree test;
    test.insert(10);
    test.insert(20);
    test.insert(30);
    test.insert(15);
    test.insert(5);
    test.insert(38);
    test.insert(23);
    test.insert(19);
    test.insert(3);
    test.insert(8);
    test.insert(16);
    

    cout << test << endl << endl << endl;

    AvlTree test2;
    test2.insert(10);
    test2.insert(20);
    test2.insert(30);
    test2.insert(15);
    test2.insert(5);
    test2.insert(38);
    test2.insert(23);
    test2.insert(19);
    test2.insert(3);
    test2.insert(8);
    test2.insert(16);
    test2.insert(18);
    test2.insert(17);
    test2.insert(15);
    test2.insert(14);
    test2.insert(2);
    
    cout << test2 << endl;
    
    test2.insert(45);
    //тут будет видно как меняется вершина по мере добавления новых ключей

    cout << test2 << endl;
    
    AvlTree test3;
    
    test3.insert(10);
    test3.insert(6);
    test3.insert(12);
    test3.insert(5);
    test3.insert(8);
    test3.insert(7);
    
    cout << test3 << endl;
    
    
    
    
}
