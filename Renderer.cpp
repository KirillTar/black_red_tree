#include "Renderer.h"
#include <iostream>

Renderer::Renderer() {
	m_ren = nullptr;
	m_font = nullptr;
	m_ui_font = nullptr;
	m_xoffset = 0;
	m_yoffset = 0;
}

Renderer::~Renderer() {
	if (m_font)
		this->close_font();
	if (m_ui_font)
		this->close_ui_font();
	SDL_DestroyRenderer(m_ren);
}

int Renderer::init(SDL_Window* window) {

	m_ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (m_ren == nullptr) {
		std::cout << "Failed to create renderer : " << SDL_GetError();
		return -1;
	}

	if (TTF_Init() == -1) {
		std::cout << "Failed to init TTF : " << SDL_GetError();
		return -1;

	}

	SDL_SetRenderDrawBlendMode(m_ren, SDL_BLENDMODE_BLEND);
	return 0;
}

void Renderer::clear() {
	set_color("WHITE");
	SDL_RenderClear(m_ren);
}

void Renderer::update() {
	SDL_RenderPresent(m_ren);
}

void Renderer::set_offsets(int x, int y) {
	m_xoffset = x;
	m_yoffset = y;
}

void Renderer::set_color(const char* color) {
	if (color == "BLACK") {
		SDL_SetRenderDrawColor(m_ren, 0, 0, 0, 255);
		return;
	}
	else if (color == "WHITE") {
		SDL_SetRenderDrawColor(m_ren, 255, 255, 255, 255);
		return;
	}
	else if (color == "RED") {
		SDL_SetRenderDrawColor(m_ren, 255, 0, 0, 255);
		return;
	}
	else if (color == "GRID") {
		SDL_SetRenderDrawColor(m_ren, 112, 128, 144, 150);
		return;
	}
	else {
		std::cout << "Unknow color! Returning to default color (white)" << std::endl;
		SDL_SetRenderDrawColor(m_ren, 255, 255, 255, 255);
		return;
	}
}

void Renderer::open_font(int fontsize) {
	m_font = TTF_OpenFont("sample.ttf", fontsize);
}

void Renderer::open_ui_font(int fontsize) {
	m_ui_font = TTF_OpenFont("sample.ttf", fontsize);
}

void Renderer::close_font() {
	TTF_CloseFont(m_font);
}

void Renderer::close_ui_font() {
	TTF_CloseFont(m_ui_font);
}

void Renderer::draw_grid(int camera_width, int camera_height, int grid_step) {
	set_color("GRID");

	int start = m_xoffset > 0 ? 0 : m_xoffset;

	for (int i = start; i <= m_xoffset + camera_width; i += grid_step) {
		if (i >= m_xoffset) {
			SDL_RenderDrawLine(m_ren, i - m_xoffset, 0, i - m_xoffset, camera_height);
		}
	}

	for (int i = start; i <= m_yoffset + camera_height; i += grid_step) {
		if (i >= m_yoffset) {
			SDL_RenderDrawLine(m_ren, 0, i - m_yoffset, camera_width, i - m_yoffset);
		}
	}
}

void Renderer::draw_node(const Node_graphic& node) {
	Circle c = node.circle;
	auto color = node.color;
	c.x -= m_xoffset;
	c.y -= m_yoffset;

	draw_circle(c, node.color);
	draw_value(node.value, c.x, c.y);
}

void Renderer::connect_node_pair(const Node_graphic& first, const Node_graphic& second) {
	connect_two_circles(first.circle, second.circle);
}

void Renderer::draw_circle(const Circle& circle, const char* color) {

	int x = 0;
	int y = circle.radius;
	int gap = 0;
	int delta = (2 - 2 * y);

	set_color(color);

	while (y >= 0)
	{
		SDL_RenderDrawPoint(m_ren, circle.x + x, circle.y + y);
		SDL_RenderDrawPoint(m_ren, circle.x - x, circle.y + y);
		SDL_RenderDrawPoint(m_ren, circle.x + x, circle.y - y);
		SDL_RenderDrawPoint(m_ren, circle.x - x, circle.y - y);

		SDL_RenderDrawLine(m_ren, circle.x + x, circle.y + y, circle.x + x, circle.y - y); // filling circle
		SDL_RenderDrawLine(m_ren, circle.x - x, circle.y + y, circle.x - x, circle.y - y);

		gap = 2 * (delta + y) - 1;
		if (delta < 0 && gap <= 0)
		{
			x++;
			delta += 2 * x + 1;
			continue;
		}
		if (delta > 0 && gap > 0)
		{
			y--;
			delta -= 2 * y + 1;
			continue;
		}
		x++;
		delta += 2 * (x - y);
		y--;
	}
}

void Renderer::draw_value(int value, int x, int y) {
	
	SDL_Surface* textSurface = NULL;
	SDL_Texture* texture = NULL;
	SDL_Rect textLocation = { 0, 0, 0, 0 };
	SDL_Color clr = { 255, 255, 255, 255 };

	textSurface = TTF_RenderText_Blended(m_font, std::to_string(value).c_str(), clr);
	texture = SDL_CreateTextureFromSurface(m_ren, textSurface);
	textLocation = { int(x - textSurface->w / 2), int(y - textSurface->h / 2), textSurface->w, textSurface->h };
	SDL_RenderCopy(m_ren, texture, NULL, &textLocation);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(textSurface);
}

std::pair<int, int> Renderer::get_closest_point_from_circle(const Circle& circle, int pointx, int pointy)
{
	int deltax = pointx - circle.x;
	int deltay = pointy - circle.y;
	double ratio = circle.radius / (sqrt(deltax*deltax + deltay*deltay));
	int x = int(circle.x + deltax * ratio);
	int y = int(circle.y + deltay * ratio);
	return { x, y };
}

void Renderer::connect_two_circles(const Circle& first, const Circle& second) {

	const auto [x1, y1] = get_closest_point_from_circle(first, second.x, second.y);
	const auto [x2, y2] = get_closest_point_from_circle(second, first.x, first.y);
	set_color("BLACK");
	SDL_RenderDrawLine(m_ren, x1 - m_xoffset, y1 - m_yoffset, x2 - m_xoffset, y2 - m_yoffset);
}

void Renderer::draw_fps(int value, int x, int y) {
	SDL_Surface* textSurface = NULL;
	SDL_Texture* texture = NULL;
	SDL_Rect textLocation = { 0, 0, 0, 0 };
	SDL_Color clr = { 0, 0, 0, 255 };

	textSurface = TTF_RenderText_Blended(m_ui_font, std::to_string(value).c_str(), clr);
	texture = SDL_CreateTextureFromSurface(m_ren, textSurface);
	textLocation = { int(x - textSurface->w / 2), int(y - textSurface->h / 2), textSurface->w, textSurface->h };
	SDL_RenderCopy(m_ren, texture, NULL, &textLocation);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(textSurface);
}