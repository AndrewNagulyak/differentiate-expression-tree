

#include "pch.h"
#include <iostream>
using namespace std;
struct node {              
	char item;              
	node *next;
}*top = nullptr;
struct tree {             
	char item;
	tree *left;
	tree *right;
}*root;
struct tnode {            
	tree *item;           
	tnode *next;
}*mtop;
void push(char ch) {          
	node *p = new node;
	p->item = ch;
	p->next = top;
	top = p;
}
void mpush(tree *ptr) {        
	tnode *pp = new tnode;
	pp->item = ptr;
	pp->next = mtop;
	mtop = pp;
}
tree* mpop()
{
	if (mtop == NULL) {                      
		cout << "Stack is empty!\n";
		return NULL;
	}
	else {
		tnode *temp = mtop;
		mtop = mtop->next;
		tree *ptr = temp->item;
		delete temp;
		return ptr;
	}
}
char pop()
{
	if (top == NULL) {                      
		cout << "Stack is empty!\n";
		return NULL;
	}
	else
	{
		node *temp;
		temp = top;
		top = top->next;

		char ch = temp->item;
		delete temp;

		return ch;
	}
}
bool IsSymb(char c) {
	return(c == '+' || c == '-' || c == '*' || c == '/');
}
void CreateTree(char *p)
{
	tree* temp;
	for (int i = 0; p[i] != '\0'; i++) {
		if (!IsSymb(p[i])) {
			temp = new tree;
			temp->item = p[i];
			temp->left = temp->right = NULL;
			mpush(temp);
		}
		else {
			temp = new tree;
			temp->item = p[i];
			temp->right = mpop();
			temp->left = mpop();
			mpush(temp);
		}
	}
	root = mpop();
}
void inorder(tree *root) { 
	if (root != NULL) {
		if (root->right != NULL || root->left != NULL) cout << '(';
		inorder(root->left);
		cout << root->item;
		inorder(root->right);
		if (root->right != NULL || root->left != NULL) cout << ')';
	}
}
char *convert(char s[])
{
	int j = 0;
	char *postfix;
	postfix = new char[strlen(s)];
	for (int i = 0; s[i] != '\0'; i++)
	{
		if (s[i] == '/' || s[i] == '*' || s[i] == '(')
		{
			push(s[i]);
		}
		else if (s[i] == '+' || s[i] == '-')
		{
			if (top != NULL) {
				while (top->item == '*' || top->item == '/') {
					postfix[j++] = pop();
					if (top == NULL)
						break;
				}
				
					push(s[i]);
			}
		}
		else if (s[i] == ')')
		{
			while (top->item != '(') postfix[j++] = pop();
			pop();
		}
		else postfix[j++] = s[i];

	}
	while (top != NULL)postfix[j++] = pop();
	postfix[j] = '\0';
	return postfix;
}
tree *copy(tree *root) {      
	tree *new_root = nullptr;
	if (root != NULL) {
		new_root = new tree;
		new_root->item = root->item;
		new_root->left = copy(root->left);
		new_root->right = copy(root->right);
	}
	else if (root == NULL) return NULL;
	return new_root;
}
tree* diff(tree* root)
{
	if (root->item == '+' || root->item == '-') {
		root->left = diff(root->left);
		root->right = diff(root->right);
	}
	else if (root->item == 'x') root->item = '1';
	else if ((root->item) >= '0' && (root->item) <= '9') root->item = '0';
	else if (root->item == '*') {
		tree *help1 = new tree;
		tree *help2 = new tree;
		help1->left = root->left;
		help1->right = diff(copy(root->right));
		help2->left = diff(copy(root->left));
		help2->right = root->right;
		help1->item = '*';
		help2->item = '*';
		root->left = help1;
		root->right = help2;
		root->item = '+';
	}
	else if (root->item == '/') {
		tree *help1 = new tree;
		tree *help2 = new tree;
		tree *help3 = new tree;
		tree *help4 = new tree;
		help1->left = diff(copy(root->left));
		help1->right = root->right;
		help1->item = '*';
		help2->left = root->left;
		help2->right = diff(copy(root->right));
		help2->item = '*';
		help3->left = help1;
		help3->right = help2;
		help3->item = '-';
		help4->left = root->right;
		help4->right = root->right;
		help4->item = '*';
		root->left = help3;
		root->right = help4;
	}
	return root;
}
int main()
{
	tree* difroot;
	char *s = new char[10];
	cout << "Enter a polynomial containing the variable x\n";
	cin >> s;
	cout << "--------------------------------------------------------------------------\n";
	char *p = convert(s); //Before constructing the tree, convert the string into postorder
	cout << p;
	cout << endl;
	CreateTree(p);
	inorder(root);
	cout << "\n------------------------------------------------------------------------\n";
	difroot = diff(root);
	inorder(difroot);

}


