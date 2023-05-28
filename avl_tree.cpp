#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <unordered_set>
using namespace std;

struct node // структура для представления узлов дерева
{
	int key;
	unsigned char height;
	node* left;
	node* right;
	node(int k) { key = k; left = right = 0; height = 1; }
};

unsigned char height(node* p)
{
	return p?p->height:0;
}

int bfactor(node* p)
{
	return height(p->right)-height(p->left);
}

void fixheight(node* p)
{
	unsigned char hl = height(p->left);
	unsigned char hr = height(p->right);
	p->height = (hl>hr?hl:hr)+1;
}

node* rotateright(node* p) // правый поворот вокруг p
{
	node* q = p->left;
	p->left = q->right;
	q->right = p;
	fixheight(p);
	fixheight(q);
	return q;
}

node* rotateleft(node* q) // левый поворот вокруг q
{
	node* p = q->right;
	q->right = p->left;
	p->left = q;
	fixheight(q);
	fixheight(p);
	return p;
}

void generate_asymptote_code(const node* root);
node* balance(node* p, bool logs_on = false) // балансировка узла p
{
    generate_asymptote_code(p);
	fixheight(p);
	if( bfactor(p)==2 )
	{
        if (logs_on)
            cout << "\\textcolor{blue}{balancing node " << p->key << "...}" << endl << endl;
		if( bfactor(p->right) < 0 ) {
            if (logs_on)
                cout << "\\textcolor{blue}{rotate right:}" << endl << endl;
			p->right = rotateright(p->right);
            if (logs_on)
                generate_asymptote_code(p);
        }
        if (logs_on)
            cout << "\\textcolor{blue}{rotate left:}" << endl << endl;
        node *temp = rotateleft(p);
        if (logs_on)
            generate_asymptote_code(temp);
		return temp;
	}
	if( bfactor(p)==-2 )
	{
        if (logs_on)
            cout << "\\textcolor{blue}{balancing node " << p->key << "...}" << endl << endl;
		if( bfactor(p->left) > 0  ) {
            if (logs_on)
                cout << "\\textcolor{blue}{rotate left:}" << endl << endl;
			p->left = rotateleft(p->left);
            if (logs_on)
                generate_asymptote_code(p);
        }
        if (logs_on)
            cout << "\\textcolor{blue}{rotate right:}" << endl << endl;
        node *temp = rotateright(p);
        if (logs_on)
            generate_asymptote_code(p);
		return temp;
	}
	return p; // балансировка не нужна
}

node* insert(node* p, int k, bool logs_on = false) // вставка ключа k в дерево с корнем p
{
	if( !p ) return new node(k);
	if( k<p->key )
		p->left = insert(p->left,k, logs_on);
	else
		p->right = insert(p->right,k, logs_on);
	return balance(p, logs_on);
}

node* findmin(node* p) // поиск узла с минимальным ключом в дереве p 
{
	return p->left?findmin(p->left):p;
}

node* removemin(node* p) // удаление узла с минимальным ключом из дерева p
{
	if( p->left==0 )
		return p->right;
	p->left = removemin(p->left);
	return balance(p);
}

node* remove(node* p, int k) // удаление ключа k из дерева p
{
	if( !p ) return 0;
	if( k < p->key )
		p->left = remove(p->left,k);
	else if( k > p->key )
		p->right = remove(p->right,k);	
	else //  k == p->key 
	{
		node* q = p->left;
		node* r = p->right;
		delete p;
		if( !r ) return q;
		node* min = findmin(r);
		min->right = removemin(r);
		min->left = q;
		return balance(min);
	}
	return balance(p);
}

void left_root_right(const node *root, ostringstream &os) {
    if (root == nullptr) {
        return;
    }
    left_root_right(root->left, os);
    os << root->key << " ";
    left_root_right(root->right, os);
}

void root_left_right(const node *root, ostringstream &os) {
    if (root == nullptr) {
        return;
    }
    os << root->key << ", ";
    root_left_right(root->left, os);
    root_left_right(root->right, os);
}

void generate_asymptote_code(const node* root) {
    ostringstream os;
    os << "\\begin{center}\n";
    os << "\\begin{asy}\n";
    os << "import binarytree;\n";
    os << "picture pic;\n";
    os << "binarytree st = searchtree(";
    root_left_right(root, os);
    os.seekp(-2, os.cur);
    os << ");\n";
    os << "draw(pic, st, condensed=true);\n";
    os << "add(pic.fit(), (0, 0));\n";
    os << "\\end{asy}\n";
    os << "\\end{center}\n";
    cout << os.str() << endl;
}


int main() {
    srand(time(nullptr));
    unordered_set<int> elems;
    node *root = nullptr; //корень avl_tree
    while (elems.size() < 10)
    {
        int new_key = 1 + rand() / ((RAND_MAX + 1u) / 99); //random int in [1..99]
        if (elems.find(new_key) == elems.end()) {
            root = insert(root, new_key);
            elems.insert(new_key);
        }
    }
    cout << "left-root-right traversal: ";
    ostringstream os;
    left_root_right(root, os);
    cout << os.str() << endl << endl;
    cout << "height of the avl-tree: " << (int)height(root) << endl << endl;
    cout << "we start from this avl-tree:" << endl << endl;
    generate_asymptote_code(root);
    cout << "now we add 3 more nodes:" << endl << endl;
    int i = 0;
    while (elems.size() < 13) {
        int new_key = 1 + rand() / ((RAND_MAX + 1u) / 99); //random int in [1..99]
        if (elems.find(new_key) == elems.end()) {
             cout << ++i <<". adding node " << new_key << "..." << endl << endl;
            root = insert(root, new_key, true);
            elems.insert(new_key);
        }
        cout << "we get:" << endl << endl;
        generate_asymptote_code(root);
    }
}