#pragma once

enum class Color{Black, Red};

template<typename T>
class Node {

public:

	Node(T value, Node* const parent) {
		this->value = value;
		this->parent = parent;
	}

public:
	T value = 0;
	Color color = Color::Red;

	Node* left = nullptr;
	Node* right = nullptr;
	Node* parent = nullptr;
};
