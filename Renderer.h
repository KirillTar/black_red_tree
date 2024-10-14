#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "Node.h"

struct Circle {

	Circle(int x, int y, int radius) {
		this->x = x;
		this->y = y;
		this->radius = radius;
	}

	int x, y, radius;
};

struct Node_graphic {

	Node_graphic() : parent(0,0,0), circle(0,0,0) {}

	Node_graphic(Circle parent, const Node<int>* ptr, int x, int y, int radius, int value, const char* color, bool side) :
		parent(parent),
		ptr(ptr),
		circle(x, y, radius), 
		value(value),         
		color(color),
		side(side)
	{}

	Circle parent;
	const Node<int>* ptr;
	Circle circle;
	int value;
	const char* color;
	bool side;
};

class Renderer {
public:
	Renderer();
	~Renderer();

	int init(SDL_Window*);
	void clear();
	void update();
	void open_font(int fontsize);
	void open_ui_font(int fontsize);
	void close_font();
	void close_ui_font();
	void set_offsets(int x, int y);
	void set_color(const char* color);
	void draw_grid(int camera_width, int camera_height, int grid_step);
	void draw_node(const Node_graphic&);
	void connect_node_pair(const Node_graphic&, const Node_graphic&);
	void draw_fps(int fps, int x, int y);

private:
	void draw_circle(const Circle&, const char* color);
	void draw_value(int value, int x, int y);
	std::pair<int, int> get_closest_point_from_circle(const Circle& circle, int pointx, int pointy);
	void connect_two_circles(const Circle&, const Circle&);

private:
	SDL_Renderer* m_ren;
	TTF_Font* m_font;
	TTF_Font* m_ui_font;

	int m_xoffset;
	int m_yoffset;
};
