#pragma once
#include <iostream>
#include "Node.h"
#include "Iterators.h"
#include <iterator>

template<typename T>
class Tree {

public:
	friend class Tree_Iterator<T>;
	friend class Tree_Const_Iterator<T>;

	using value_type = T;
	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;
	using size_type = std::size_t;

	using iterator = Tree_Iterator<T>;
	using const_iterator = Tree_Const_Iterator<T>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
	Tree();
	Tree(const Tree&) noexcept;
	Tree(Tree&&) noexcept;
	Tree(std::initializer_list<T>) noexcept;

	Tree& operator= (Tree<T>) noexcept;
	Tree& operator= (Tree<T>&&) noexcept;
	Tree& operator= (std::initializer_list<T>) noexcept;

	~Tree();

	friend void swap(Tree<T>& first, Tree<T>& second)
	{
		using std::swap;
		swap(first.m_tree_size, second.m_tree_size);
		swap(first.m_root, second.m_root);
		swap(first.m_first, second.m_first);
		swap(first.m_last, second.m_last);
	}

	void insert(T);
	inline void clear();
	inline size_t size() const;
	inline bool empty() const;

	iterator erase(iterator);

	iterator begin();
	iterator end();

	const_iterator begin() const;
	const_iterator end() const;

	const_iterator cbegin() const;
	const_iterator cend() const;

	reverse_iterator rbegin();
	reverse_iterator rend();

	const_reverse_iterator rbegin() const;
	const_reverse_iterator rend() const;

	const_reverse_iterator crbegin() const;
	const_reverse_iterator crend() const;

	Node<T>* find_node(T) const noexcept;
	Node<T>* get_root(); /*exists only for graphics, not in real container*/

private:

	void balance_tree_case_1(Node<T>*);
	void balance_tree_case_2(Node<T>*);
	void balance_tree_case_3(Node<T>*);
	void balance_tree_case_4(Node<T>*);
	void balance_tree_case_5(Node<T>*);

	void delete_case_1(Node<T>*);
	void delete_case_2(Node<T>*);
	void delete_case_3(Node<T>*);
	void delete_case_4(Node<T>*);
	void delete_case_5(Node<T>*);
	void delete_case_6(Node<T>*);

	void rotate_right(Node<T>*);
	void rotate_left(Node<T>*);

	void delete_all_nodes(Node<T>*);

	Node<T>* grandparent(Node<T>* const);
	Node<T>* uncle(Node<T>* const);
	Node<T>* sibling(Node<T>* const);

	Node<T>* next(Node<T>*) const;
	Node<T>* prev(Node<T>*) const;

private:
	Node<T>* m_root = nullptr;
	size_t   m_tree_size = 0;
	Node<T>* m_first = nullptr;
	Node<T>* m_last = nullptr;
};

template<typename T>
Node<T>* Tree<T>::get_root() /*exists only for graphics, not in real container*/ {
	return m_root;
}

template<typename T>
Tree<T>::Tree() {
	m_last = new Node<T>(0, nullptr);
	m_first = new Node<T>(0, m_last);
}

template<typename T>
Tree<T>::~Tree() {
	clear();
	delete(m_root);
	delete(m_first);
	delete(m_last);
}

template<typename T>
Tree<T>::Tree(std::initializer_list<T> list) noexcept : Tree() {
	for (auto it = list.begin(); it != list.end(); ++it) {
		insert(*it); 
	}
}

template<typename T>
Tree<T>::Tree(const Tree& other_tree) noexcept : Tree() {
	if (!other_tree.empty()) {
		for (Tree<int>::const_iterator iter = other_tree.begin(); iter != other_tree.end(); ++iter) {
			insert(*iter);
		}
	}
}

template<typename T>
Tree<T>::Tree(Tree&& other_tree) noexcept : Tree() {
	swap(*this, other_tree);
}

template<typename T>
Tree<T>& Tree<T>::operator=(Tree<T> other_tree) noexcept {
	swap(*this, other_tree);
	return *this;
}

template<typename T>
Tree<T>& Tree<T>::operator=(Tree<T>&& other_tree) noexcept {
	swap(*this, other_tree);
	return *this;
}

template<typename T>
Tree<T>& Tree<T>::operator=(std::initializer_list<T> list) noexcept {
	clear();
	for (auto it = list.begin(); it != list.end(); ++it) {
		insert(*it);
	}
	return *this;
}

template<typename T>
Node<T>* Tree<T>::find_node(T value) const noexcept {
	Node<T>* current_node = m_root;

	while (current_node && current_node->value != value) {

		if (value < current_node->value)
			current_node = current_node->left;
		else
			current_node = current_node->right;
	}

	if (current_node == m_root && current_node->value != value)
		return nullptr;

	return current_node;
}

template<typename T>
Node<T>* Tree<T>::grandparent(Node<T>* const node) {
	Node<T>* parent = node->parent;
	if (parent && parent->parent) {
		return parent->parent;
	}
	else {
		return nullptr;
	}
}

template<typename T>
Node<T>* Tree<T>::uncle(Node<T>* const node) {
	Node<T>* grandparent = Tree::grandparent(node);
	if (grandparent) {
		if (grandparent->left == node->parent) {
			return grandparent->right;
		}
		else {
			return grandparent->left;
		}
	}
	else
		return nullptr;
}

template<typename T>
Node<T>* Tree<T>::sibling(Node<T>* const node) {

	if (!node->parent) {
		return nullptr;
	}

	Node<T>* parent = node->parent;

	if (node == parent->left) {
		return parent->right;
	}
	else {
		return parent->left;
	}
}

template<typename T>
void Tree<T>::rotate_right(Node<T>* node) {

	Node<T>* left = node->left;
	left->parent = node->parent;

	if (node->parent) {
		if (node->parent->left == node)
			node->parent->left = left;
		else
			node->parent->right = left;
	}
	else {
		m_root = left;
	}

	node->left = left->right;
	if (left->right)
		left->right->parent = node;

	node->parent = left;
	left->right = node;
}

template<typename T>
void Tree<T>::rotate_left(Node<T>* node) {

	Node<T>* right = node->right;
	right->parent = node->parent;

	if (node->parent) {
		if (node->parent->left == node)
			node->parent->left = right;
		else
			node->parent->right = right;
	}
	else {
		m_root = right;
	}

	node->right = right->left;
	if (right->left)
		right->left->parent = node;

	node->parent = right;
	right->left = node;
}

template<typename T>
void Tree<T>::balance_tree_case_1(Node<T>* node) {

	if (!node->parent) {
		node->color = Color::Black;
	}
	else {
		balance_tree_case_2(node);
	}

}

template<typename T>
void Tree<T>::balance_tree_case_2(Node<T>* node) {
	if (node->parent->color == Color::Black) {
		return;
	}
	else {
		balance_tree_case_3(node);
	}
}

template<typename T>
void Tree<T>::balance_tree_case_3(Node<T>* node) {

	Node<T>* uncle = Tree::uncle(node);

	if (uncle && uncle->color == Color::Red) {
		node->parent->color = Color::Black;
		uncle->color = Color::Black;
		Node<T>* grandparent = Tree::grandparent(node);
		grandparent->color = Color::Red;
		balance_tree_case_1(grandparent);
	}
	else {
		balance_tree_case_4(node);
	}
}

template<typename T>
void Tree<T>::balance_tree_case_4(Node<T>* node) {

	Node<T>* grandparent = Tree::grandparent(node);
	Node<T>* tmp = node;
	Node<T>* parent = node->parent;

	if (node == parent->right && parent == grandparent->left) {
		rotate_left(parent);
		tmp = node->left;
	}
	else if (node == parent->left && parent == grandparent->right) {
		rotate_right(parent);
		tmp = node->right;
	}

	balance_tree_case_5(tmp);
}

template<typename T>
void Tree<T>::balance_tree_case_5(Node<T>* node) {

	Node<T>* parent = node->parent;
	parent->color = Color::Black;

	Node<T>* grandparent = Tree::grandparent(node);
	grandparent->color = Color::Red;

	if (node == parent->left) {
		rotate_right(grandparent);
	}
	else {
		rotate_left(grandparent);
	}
}

template<typename T>
void Tree<T>::insert(T value) {

	if (m_root == nullptr)
	{
		m_root = new Node<T>(value, nullptr);
		m_root->color = Color::Black;
		m_first->parent = m_root;
		m_last->parent = m_root;
		m_tree_size++;
		return;
	}

	Node<T>* current_node = m_root;
	Node<T>* parent = m_root;

	bool mooving_only_left = true;
	bool mooving_only_right = true;

	while (current_node) {
		parent = current_node;

		if (value < current_node->value)
		{
			current_node = current_node->left;
			mooving_only_right = false;
		}
		else {
			current_node = current_node->right;
			mooving_only_left = false;
		}
	}

	Node<T>* new_node = new Node<T>(value, parent);

	if (mooving_only_left)
	{
		m_first->parent = new_node;
	}

	if (mooving_only_right)
	{
		m_last->parent = new_node;
	}

	if (!parent) {
		m_root = new_node;
	}

	if (value < parent->value)
	{
		parent->left = new_node;
	}
	else
	{
		parent->right = new_node;
	}

	balance_tree_case_1(new_node);
	m_tree_size++;
}

template<typename T>
void Tree<T>::delete_case_1(Node<T>* node) {
	if (node->parent) {
		delete_case_2(node);
	}
}

template<typename T>
void Tree<T>::delete_case_2(Node<T>* node) {

	Node<T>* sibling = Tree::sibling(node);
	Node<T>* parent = node->parent;

	Color sibling_color = Color::Black;

	if (sibling) {
		sibling_color = sibling->color;
	}

	if (sibling_color != Color::Black) {
		parent->color = Color::Red;

		if (sibling)
			sibling->color = Color::Black;

		if (node == parent->left) {
			rotate_left(parent);
		}
		else {
			rotate_right(parent);
		}
	}

	delete_case_3(node);
}

template<typename T>
void Tree<T>::delete_case_3(Node<T>* node) {

	Node<T>* sibling = Tree::sibling(node);
	Node<T>* parent = node->parent;

	Color sibling_color = Color::Black;
	if (sibling) {
		sibling_color = sibling->color;
	}

	Color sibling_lc_color = Color::Black;
	if (sibling && sibling->left)
		sibling_lc_color = sibling->left->color;

	Color sibling_rc_color = Color::Black;
	if (sibling && sibling->right)
		sibling_rc_color = sibling->right->color;

	if (parent->color == Color::Black && sibling_color == Color::Black && sibling_lc_color == Color::Black && sibling_rc_color == Color::Black) {
		if (sibling) {
			sibling->color = Color::Red;
		}
		delete_case_1(parent);
	}
	else {
		delete_case_4(node);
	}
}

template<typename T>
void Tree<T>::delete_case_4(Node<T>* node) {
	Node<T>* sibling = Tree::sibling(node);
	Node<T>* parent = node->parent;

	Color sibling_color = Color::Black;
	if (sibling) {
		sibling_color = sibling->color;
	}

	Color sibling_lc_color = Color::Black;
	if (sibling && sibling->left)
		sibling_lc_color = sibling->left->color;

	Color sibling_rc_color = Color::Black;
	if (sibling && sibling->right)
		sibling_rc_color = sibling->right->color;

	if (parent->color == Color::Red && sibling_color == Color::Black && sibling_lc_color == Color::Black && sibling_rc_color == Color::Black) {
		if (sibling) {
			sibling->color = Color::Red;
		}
		parent->color = Color::Black;
	}
	else {
		delete_case_5(node);
	}
}

template<typename T>
void Tree<T>::delete_case_5(Node<T>* node) {
	Node<T>* sibling = Tree::sibling(node);
	Node<T>* parent = node->parent;

	Color sibling_lc_color = Color::Black;
	if (sibling->left)
		sibling_lc_color = sibling->left->color;

	Color sibling_rc_color = Color::Black;
	if (sibling->right)
		sibling_rc_color = sibling->right->color;

	if (sibling->color == Color::Black) {
		if (node == parent->left && sibling_rc_color == Color::Black && sibling_lc_color == Color::Red) {
			sibling->color = Color::Red;
			sibling->left->color = Color::Black;
			rotate_right(sibling);
		}
		else if (node == parent->right && sibling_lc_color == Color::Black && sibling_rc_color == Color::Red) {
			sibling->color = Color::Red;
			sibling->right->color = Color::Black;
			rotate_left(sibling);
		}
	}

	delete_case_6(node);
}

template<typename T>
void Tree<T>::delete_case_6(Node<T>* node) {
	Node<T>* sibling = Tree::sibling(node);
	Node<T>* parent = node->parent;

	sibling->color = parent->color;
	parent->color = Color::Black;

	if (node == parent->left) {
		if (sibling->right) {
			sibling->right->color = Color::Black;
		}
		rotate_left(parent);
	}
	else {
		if (sibling->left) {
			sibling->left->color = Color::Black;
		}
		rotate_right(parent);
	}
}

template<typename T>
typename Tree<T>::iterator Tree<T>::erase(iterator iter) {

	Node<T>* tmp = iter._get_node_ptr();

	if (tmp == m_first) {
		return --begin();
	}

	if (tmp == m_last) {
		return end();
	}

	iterator next = iter;
	++next;

	// only root exists in tree
	if (!tmp->parent && !tmp->left && !tmp->right) {
		delete(m_root);
		m_root = nullptr;
		m_first->parent = m_last;
		m_last->parent = nullptr;
		m_tree_size = 0;
		return begin();
	}

	// node has valid childs
	if (tmp->left && tmp->right) {

		// looking for rightmost child in left subtree
		Node<T>* removed = tmp->left;
		while (removed->right) {
			removed = removed->right;
		}
		tmp->value = removed->value;
		tmp = removed;
	}

	Node<T>* child = tmp->left ? tmp->left : tmp->right;

	if (!child) {
		if (tmp->color == Color::Black) {
			delete_case_1(tmp);
		}

		Node<T>* parent = tmp->parent;

		if (parent->left == tmp) {
			if (tmp == m_first->parent)
				m_first->parent = parent;
			delete(parent->left);
			parent->left = nullptr;
		}
		else {
			if (tmp == m_last->parent)
				m_last->parent = parent;
			delete(parent->right);
			parent->right = nullptr;
		}
	}
	else {
		Node<T>* parent = tmp->parent;
		child->parent = parent;

		if (child == tmp->left) {
			tmp->left = nullptr;
		}
		else {
			tmp->right = nullptr;
		}

		if (parent) {
			if (tmp == parent->left) {
				parent->left = child;
			}
			else {
				parent->right = child;
			}
		}
		else {
			m_root = child;
			//std::cout << "new root (child became): " << m_root->value << std::endl;
			m_first->parent = child;
			m_last->parent = child;
		}

		if (tmp->color == Color::Black) {
			if (child->color == Color::Red)
				child->color = Color::Black;
			else
				delete_case_1(child);
		}

		if (tmp == m_first->parent)
			m_first->parent = parent;

		if (tmp == m_last->parent)
			m_last->parent = parent;

		delete(tmp);
	}
	m_tree_size--;
	return next;
}

template <typename T>
void Tree<T>::delete_all_nodes(Node<T>* node) {
	if (node == nullptr)
		return;

	delete_all_nodes(node->left);
	delete_all_nodes(node->right);
	delete(node);
}

template<typename T>
inline void Tree<T>::clear() {
	delete_all_nodes(m_root);
	m_root = nullptr;
	m_first->parent = m_last;
	m_last->parent = nullptr;
	m_tree_size = 0;
}

template<typename T>
inline size_t Tree<T>::size() const {
	return this->m_tree_size;
}

template<typename T>
inline bool Tree<T>::empty() const {
	return m_tree_size == 0;
}

template<typename T>
Node<T>* Tree<T>::next(Node<T>* node) const{

	if (node == m_last->parent)
		return m_last;

	if (node == m_first)
		return m_first->parent;

	Node<T>* tmp = node->right; 

	if (tmp) {
		while (tmp->left) tmp = tmp->left;
		return tmp;
	}

	tmp = node; 
	while (tmp->parent && tmp == tmp->parent->right)
		tmp = tmp->parent;
	
	return tmp->parent;
}

template<typename T>
Node<T>* Tree<T>::prev(Node<T>* node) const{

	if (node == m_last)
		return m_last->parent;

	if (node == m_first->parent)
		return m_first;

	Node<T>* tmp = node->left;

	if (tmp) {
		while (tmp->right) tmp = tmp->right;
		return tmp;
	}

	tmp = node;
	while (tmp->parent && tmp == tmp->parent->left)
		tmp = tmp->parent;

	return tmp->parent;
}

template<typename T>
typename Tree<T>::iterator Tree<T>::begin(){
	const Tree<T>* ptr = this;
	return iterator(next(m_first), ptr);
}

template<typename T>
typename Tree<T>::iterator Tree<T>::end(){
	const Tree<T>* ptr = this;
	return iterator(m_last, ptr);
}

template<typename T>
typename Tree<T>::const_iterator Tree<T>::begin() const{
	const Tree<T>* ptr = this;
	return const_iterator(next(m_first), ptr);
}

template<typename T>
typename Tree<T>::const_iterator Tree<T>::end() const{
	const Tree<T>* ptr = this;
	return const_iterator(m_last, ptr);
}

template<typename T>
typename Tree<T>::const_iterator Tree<T>::cbegin() const {
	const Tree<T>* ptr = this;
	return const_iterator(next(m_first), ptr);
}

template<typename T>
typename Tree<T>::const_iterator Tree<T>::cend() const{
	const Tree<T>* ptr = this;
	return const_iterator(m_last, ptr);
}

template<typename T>
typename Tree<T>::reverse_iterator Tree<T>::rbegin() {
	return reverse_iterator(this->end());
}

template<typename T>
typename Tree<T>::reverse_iterator Tree<T>::rend() {
	return reverse_iterator(this->begin());
}

template<typename T>
typename Tree<T>::const_reverse_iterator Tree<T>::rbegin() const{
	return const_reverse_iterator(this->cend());
}

template<typename T>
typename Tree<T>::const_reverse_iterator Tree<T>::rend() const {
	return const_reverse_iterator(this->cbegin());
}

template<typename T>
typename Tree<T>::const_reverse_iterator Tree<T>::crbegin() const {
	return const_reverse_iterator(this->cend());
}

template<typename T>
typename Tree<T>::const_reverse_iterator Tree<T>::crend() const {
	return const_reverse_iterator(this->cbegin());
}