#pragma once

#include <iostream>

/*
this file contains splay tree implementation.
I would not recommend to use it in your code, as it was not properly tested
also, it is not STL compatible at all. Probably, I would rewrite is some day
*/

template<typename T>
struct node
{
	T value;
	node<T> *left, *right, *parent;

	node(T value, node<T>* parent) : value(value), parent(parent)
	{
		left = right = nullptr;
	}
};

template<typename T>
class splayTree
{
	node<T>* root;

	bool isRoot(node<T>* p)
	{
		return p->parent == nullptr;
	}

	void clear(node<T>* p)
	{
		if (p == nullptr) return;
		clear(p->left);
		clear(p->right);
		delete p;
	}

	void setParent(node<T>* p, node<T>* parent)
	{
		if (p != nullptr) p->parent = parent;
	}

	void updateParent(node<T>* p)
	{
		setParent(p->left, p);
		setParent(p->right, p);
	}

	void rotate(node<T>* parent, node<T>* p)
	{
		node<T>* prep = parent->parent;
		if (prep != nullptr)
		{
			if (prep->left == parent) prep->left = p;
			else prep->right = p;
		}
		if (parent->left == p)
		{
			parent->left = p->right;
			p->right = parent;
		}
		else
		{
			parent->right = p->left;
			p->left = parent;
		}
		updateParent(p);
		updateParent(parent);
		p->parent = prep;
		if (isRoot(p)) root = p;
	}

	node<T>* splay(node<T>* p)
	{
		if (isRoot(p)) return p;
		node<T>* parent = p->parent;
		node<T>* prep = parent->parent;
		if (isRoot(parent))
		{
			rotate(parent, p);
			return p;
		}
		bool zigzig = (prep->left == parent) == (parent->left == p);
		if (zigzig)
		{
			rotate(prep, parent);
			rotate(parent, p);
		}
		else
		{
			rotate(parent, p);
			rotate(prep, p);
		}
		return splay(p);
	}

public:
	splayTree() : root(nullptr) {}

	T insert(T value)
	{
		std::pair<splayTree<T>, splayTree<T> > trees;
		split(trees, value);
		node<T>* p = new node<T>(value, nullptr);
		root = p;
		root->left = trees.first.root;
		root->right = trees.second.root;
		updateParent(root);
		trees.first.root = trees.second.root = nullptr;
		return value;
	}

	bool find(T value)
	{
		if (root == nullptr) return false;
		node<T>* p = root;
		while (true)
		{
			if (p->value == value)
			{
				splay(p);
				return true;
			}
			if (p->value > value)
			{
				if (p->left != nullptr) p = p->left;
				else
				{
					splay(p);
					return false;
				}
			}
			else
			{
				if (p->right != nullptr) p = p->right;
				else
				{
					splay(p);
					return false;
				}
			}
		}
	}

	std::pair<splayTree<T>, splayTree<T> >& split(std::pair<splayTree<T>, splayTree<T> >& out, T value)
	{
		find(value);
		if (root != nullptr)
		{
			if (root->value == value)
			{
				out.first.root = root->left;
				out.second.root = root->right;
				delete root;
			}
			else if (root->value > value)
			{
				out.second.root = root;
				out.first.root = root->left;
				root->left = nullptr;
			}
			else
			{
				out.second.root = root->right;
				out.first.root = root;
				root->right = nullptr;
			}
			setParent(out.first.root, nullptr);
			setParent(out.second.root, nullptr);
			root = nullptr;
		}
		return out;
	}

	void merge(std::pair<splayTree<T>, splayTree<T> >& in)
	{
		if (root != nullptr) return; // if tree is not empty do not merge
		if (in.first.root == nullptr)
		{
			root = in.second.root;
			in.second.root = nullptr;
			return;
		}
		if (in.second.root == nullptr)
		{
			root = in.first.root;
			in.first.root = nullptr;
			return;
		}
		in.second.find(in.first.root->value);
		root = in.second.root;
		root->left = in.first.root;
		in.first.root = nullptr;
		in.second.root = nullptr;
	}

	void erase(T value)
	{
		if (!find(value)) return;
		std::pair<splayTree<T>, splayTree<T> > trees;
		trees.first.root = root->left;
		trees.second.root = root->right;
		setParent(root->left, nullptr);
		setParent(root->right, nullptr);
		delete root;
		root = nullptr;
		merge(trees);
	}

	~splayTree()
	{
		clear(root);
	}

	template<typename T1>
	friend void display(splayTree<T1>&);

	template<typename T1>
	friend void printNode(node<T1>*);
};

template<typename T>
void printNode(node<T>* p)
{
	std::cout << p->value << " ";
	if (p->left != nullptr)
	{
		std::cout << "[L" << p->value << "]";
		printNode(p->left);
	}
	if (p->right != nullptr)
	{
		std::cout << "[R" << p->value << "]";
		printNode(p->right);
	}
}

template<typename T>
void display(splayTree<T>& tree)
{
	if (tree.root != nullptr) printNode(tree.root);
	std::cout << std::endl;
}
