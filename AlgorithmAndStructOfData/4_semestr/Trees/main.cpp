#include <iostream>
#include <vector>
#include <random>

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
	void print_vertical(ostream& out, Node* node, int space = 0, int spacing = 4) const {
		if (!node) return;

		// Сначала правое поддерево (чтобы было сверху)
		print_vertical(out, node->right, space + spacing, spacing);

		// Вывод текущего узла
		out << string(space, ' ');
		out << node->key << endl;

		// Затем левое поддерево (чтобы было снизу)
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

		else if (!node->left && node->right)//узел имеет одно поддерево
		{
			if (node == root)
				root = node->right;

			else if (node == prew->left)
				prew->left = node->right;

			else
				prew->right = node->right;

			delete node;
		}

		else if (node->left && !node->right)//узел имеет одно поддерево
		{
			if (node == root)
				root = node->left;

			else if (node == prew->left)
				prew->left = node->left;

			else
				prew->right = node->left;

			delete node;
		}

		else//узел имеет оба поддерева
		{
			Node* tmp = node->left;
			prew = node;

			while (tmp->left)
			{
				prew = tmp;
				tmp = tmp->left;
			}

			node->key = tmp->key;

			if (tmp == prew->left)
				prew->left = tmp->right;
			else
				prew->right = tmp->right;

			delete tmp;
		}
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
			mxnode = mxnode->left;

		return mxnode;
	}

	friend ostream& operator<<(ostream& out, const BinTree& tree) {
		if (!tree.root) {
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

		while (!level.empty())
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

			level = next_level;
		}

		return res;
	}

	void left_node_right()
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

	void node_left_right() 
	{
		if (!root) return;

		vector<Node*> st;
		Node* node = root;

		st.push_back(node);

		while (!st.empty()) 
		{
			node = st.back();
			st.pop_back();
			cout << node->key << " ";

			// Сначала кладем правое поддерево, чтобы левое обработалось первым
			if (node->right) st.push_back(node->right);
			if (node->left) st.push_back(node->left);
		}
		cout << endl;
	}

	void left_right_node() 
	{
		if (!root) return;

		vector<Node*> st;
		vector<Node*> output;
		Node* node = root;
		
		st.push_back(node);

		while (!st.empty()) 
		{
			node = st.back();
			st.pop_back();
			output.push_back(node);

			// Сначала левое, потом правое - в output пойдет в обратном порядке
			if (node->left) st.push_back(node->left);
			if (node->right) st.push_back(node->right);
		}

		// Выводим в обратном порядке
		while (!output.empty()) {
			cout << output.back()->key << " ";
			output.pop_back();
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
	tree.add(8);

	cout << tree << endl;

	Node* min = tree.min_node();
	cout << "Minimum: " << min->get_key() << endl;

	Node* max = tree.max_node();
	cout << "Maximum: " << max->get_key() << endl;
	cout << endl;

	cout << "node_left_right" << endl;
	tree.node_left_right();  // 5 3 2 4 7 6 8
	cout << endl;

	cout << "left_node_right" << endl;
	tree.left_node_right();  // 2 3 4 5 6 7 8
	cout << endl;

	cout << "left_right_node" << endl;
	tree.left_right_node();  // 2 4 3 6 8 7 5
	cout << endl;


	return 0;
}
