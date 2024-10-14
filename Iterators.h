#pragma once
#include "Node.h"

template<typename T>
class Tree;

template<typename T>
class Iterator_base {

public:

	using difference_type = std::ptrdiff_t;
	using value_type = std::remove_cv_t<T>;
	using pointer = T*;
	using reference = T&;
	using iterator_category = std::bidirectional_iterator_tag;

	Iterator_base();
	Iterator_base(Node<T>*, const Tree<T>*);
	Iterator_base(const Iterator_base&);

	virtual ~Iterator_base() = default;

	bool operator==(const Iterator_base&) const;
	bool operator!=(const Iterator_base&) const;

	virtual Iterator_base& operator++() = 0;
	virtual Iterator_base& operator++(int) = 0;

	virtual Iterator_base& operator--() = 0;
	virtual Iterator_base& operator--(int) = 0;

	Node<T>* _get_node_ptr();

protected:
	Node<T>* current_node = nullptr;
	const Tree<T>* owner = nullptr;
};

template<typename T>
Iterator_base<T>::Iterator_base() : current_node(), owner() {

}

template<typename T>
Iterator_base<T>::Iterator_base(Node<T>* node, const Tree<T>* tree) : current_node(node), owner(tree) {

}

template<typename T>
Iterator_base<T>::Iterator_base(const Iterator_base& other) : current_node(other.current_node), owner(other.owner) {

}

template<typename T>
bool Iterator_base<T>::operator==(const Iterator_base& other) const {
	return (this->current_node == other.current_node);
}

template<typename T>
bool Iterator_base<T>::operator!=(const Iterator_base& other) const {
	return !(*this == other);
}


template<typename T>
Node<T>* Iterator_base<T>::_get_node_ptr() {
	return current_node;
}

template<typename T>
class Tree_Const_Iterator : public Iterator_base<T> {

public:

	using difference_type = std::ptrdiff_t;
	using value_type = std::remove_cv_t<T>;
	using pointer = const T*;
	using reference = const T&;
	using iterator_category = std::bidirectional_iterator_tag;

	using Iterator_base<T>::Iterator_base;

	Tree_Const_Iterator& operator++() override;
	Tree_Const_Iterator& operator++(int) override;

	Tree_Const_Iterator& operator--() override;
	Tree_Const_Iterator& operator--(int) override;

	const T& operator*() const;
};

template<typename T>
Tree_Const_Iterator<T>& Tree_Const_Iterator<T>::operator++() {
	this->current_node = this->owner->next(this->current_node);
	return *this;
}

template<typename T>
Tree_Const_Iterator<T>& Tree_Const_Iterator<T>::operator--() {
	this->current_node = this->owner->prev(this->current_node);
	return *this;
}

template<typename T>
Tree_Const_Iterator<T>& Tree_Const_Iterator<T>::operator++(int) {
	Tree_Const_Iterator<T> tmp = *this;
	++*this;
	return tmp;
}

template<typename T>
Tree_Const_Iterator<T>& Tree_Const_Iterator<T>::operator--(int) {
	Tree_Const_Iterator<T> tmp = *this;
	--*this;
	return tmp;
}

template<typename T>
const T& Tree_Const_Iterator<T>::operator*()const {
	return this->current_node->value;
}


template<typename T>
class Tree_Iterator : public Tree_Const_Iterator<T> {

public:

	using difference_type = std::ptrdiff_t;
	using value_type = std::remove_cv_t<T>;
	using pointer = T*;
	using reference = T&;
	using iterator_category = std::bidirectional_iterator_tag;

	using Tree_Const_Iterator<T>::Tree_Const_Iterator;

	Tree_Iterator& operator++() override;
	Tree_Iterator& operator++(int) override;

	Tree_Iterator& operator--() override;
	Tree_Iterator& operator--(int) override;

	T& operator*() const;
};

template<typename T>
Tree_Iterator<T>& Tree_Iterator<T>::operator++() {
	this->current_node = this->owner->next(this->current_node);
	return *this;
}

template<typename T>
Tree_Iterator<T>& Tree_Iterator<T>::operator--() {
	this->current_node = this->owner->prev(this->current_node);
	return *this;
}

template<typename T>
Tree_Iterator<T>& Tree_Iterator<T>::operator++(int) {
	Tree_Iterator<T> tmp = *this;
	++*this;
	return tmp;
}

template<typename T>
Tree_Iterator<T>& Tree_Iterator<T>::operator--(int) {
	Tree_Iterator<T> tmp = *this;
	--*this;
	return tmp;
}

template<typename T>
T& Tree_Iterator<T>::operator*() const {
	return const_cast<T&>(Tree_Const_Iterator<T>::operator*());
}


