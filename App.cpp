#include "App.h"
#include <queue>
#include <conio.h>
#include <random>

#define LEFT 0
#define RIGHT 1

Application::~Application() {
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

int Application::init() {

	m_window = SDL_CreateWindow("graphic", 0, 0, m_win_set.camera_width, m_win_set.camera_height, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
	if (m_window == nullptr) {
		std::cout << "Failed to create window : " << SDL_GetError();
		m_state = State::Closing;
		return -1;
	}

	if (m_ren.init(m_window) == -1)
	{
		m_state = State::Closing;
		return -1;
	}

	m_state = State::Running;

	return 0;
}

void Application::on_start() {

	int sz = /*rand() % 20 + */100;
	for (int i = 0; i < sz; i++) {
		m_tree.insert(rand() % 200);
	}

	m_ren.open_font(int(m_draw_set.circle_radius / 1.2));
	m_ren.open_ui_font(40);

	m_draw_set.node_distanse = int(m_draw_set.circle_radius * 1.5);

	system("cls");
	std::cout << "1)Append new node" << std::endl;
	std::cout << "2)Delete node" << std::endl;
	std::cout << "q)Exit" << std::endl;
	std::cout << std::endl;
	std::cout << "Enter command:" << std::endl;

	m_tree_changed = true;

	update_settings();

	m_win_set.x_offset = m_draw_set.x_root - m_draw_set.node_distanse - 10;
	m_win_set.y_offset = m_draw_set.y_root - m_draw_set.node_distanse - 10;

	m_ren.set_offsets(m_win_set.x_offset, m_win_set.y_offset);

	m_ren.clear();

	if (!m_tree.empty()) {
		draw_tree();
	}

	m_ren.update();

}

void Application::mouse_state_update() {
	m_mouse.update();

	int diffx = 0;
	int diffy = 0;

	if (m_mouse.button_pressed(SDL_BUTTON_LEFT)) {
		m_mouse.get_pos_diff(diffx, diffy);
		m_win_set.x_offset -= diffx;
		m_win_set.y_offset -= diffy;
	}
}

void Application::get_console_command() {
	int num = 0;
	std::string input;
	Node<int>* node;

	if (_kbhit()) {

		char key = _getch();

		/*if (key != '1' && key != '2' && key != 'q') {
			std::cout << "Unknown command!" << std::endl << std::endl;
		}*/

		while (true && (key == '1' || key == '2'))
		{
			std::cout << key << ")" << std::endl << "Enter node value: ";
			std::cin >> input;
			std::cout << std::endl;

			try
			{
				num = std::stoi(input);
				break;
			}
			catch (std::invalid_argument)
			{
				std::cout << "Only numbers are allowed!" << std::endl;
			}
			catch (std::out_of_range)
			{
				std::cout << "Number is out of range!" << std::endl;
			}
			catch (...)
			{
				std::cout << "Unknown error!" << std::endl;
			}
		}

		switch (key) {
		case '1':
			m_tree.insert(num);
			m_tree_changed = true;
			break;

		case '2':
			node = m_tree.find_node(num);
			m_tree.erase(Tree<int>::iterator(node, &m_tree));
			m_tree_changed = true;
			break;

		case 'q':
			SDL_Event e;
			e.type = SDL_QUIT;
			SDL_PushEvent(&e);
			break;

		case 'z':
			m_win_set.x_offset = m_draw_set.x_root - m_draw_set.node_distanse - 10;
			m_win_set.y_offset = m_draw_set.y_root - m_draw_set.node_distanse - 10;
			break;
		}

		system("cls");

		std::cout << "1)Append new node" << std::endl;
		std::cout << "2)Delete node" << std::endl;
		std::cout << "q)Exit" << std::endl;
		std::cout << std::endl;
		std::cout << "Enter command:" << std::endl;

	}
}

int get_tree_deep(const Node<int>* node) {

	if (node == nullptr) return -1;

	int deep = std::max(get_tree_deep(node->left), get_tree_deep(node->right));

	return ++deep;
}

int Application::calculate_tree_deep() {
	int deep = get_tree_deep(m_tree.get_root());
	return deep;
}

void Application::update_settings() {
	mouse_state_update();
	get_console_command();

	m_win_set.x_offset = std::clamp(m_win_set.x_offset, 0, m_win_set.max_width - m_win_set.camera_width);
	m_win_set.y_offset = std::clamp(m_win_set.y_offset, 0, m_win_set.max_height - m_win_set.camera_height);

	m_ren.set_offsets(m_win_set.x_offset, m_win_set.y_offset);

	m_draw_set.x_left_boundary = m_win_set.x_offset - m_draw_set.circle_radius;
	m_draw_set.y_left_boundary = m_win_set.y_offset - m_draw_set.circle_radius;

	m_draw_set.x_right_boundary = m_win_set.x_offset + m_win_set.camera_width + m_draw_set.circle_radius;
	m_draw_set.y_right_boundary = m_win_set.y_offset + m_win_set.camera_height + m_draw_set.circle_radius;

	if (m_tree_changed) {
		//m_win_set.min_height = calculate_tree_deep() * m_draw_set.node_distanse + m_draw_set.y_root + 100;
		m_draw_set.tree_deep = calculate_tree_deep();
		m_win_set.min_width = (m_draw_set.node_distanse) * pow(2, m_draw_set.tree_deep);
		m_draw_set.x_root = m_win_set.min_width > m_draw_set.x_root ? m_win_set.min_width : m_draw_set.x_root;
	}
	
}

void Application::event_handler() {

	while (SDL_PollEvent(&m_e)) {

		if (m_e.type == SDL_QUIT) {
			m_state = State::Closing;
		}

		m_mouse.event_handler(m_e);
	}
}

Node_graphic Application::make_node_graphic(const Node<int>* node, const Node_graphic& parent, bool side) {

	const char* color;
	color = node->color == Color::Black ? "BLACK" : "RED";

	Circle c = parent.circle;

	int child_x = 0;
	int child_y = 0;
	int offset;
	int y_offset;
	int node_deep = get_tree_deep(node);

	if (node_deep == 0)
	{
		offset = m_draw_set.node_distanse;
		y_offset = m_draw_set.node_distanse * 2;
	}
	else {
		offset = (m_draw_set.node_distanse) * pow(2, node_deep);
		y_offset = m_draw_set.node_distanse * node_deep * 2;
	}

	if (side) { //right child
		
		child_x = c.x + offset;
	}
	else {
		
		child_x = c.x - offset;
	}

	child_y = c.y + y_offset;


	return Node_graphic(parent.circle, node, child_x, child_y, c.radius, node->value, color, side);

}

void Application::draw_node(const Node_graphic& node) {

	if (node.circle.x > m_draw_set.x_left_boundary && node.circle.x < m_draw_set.x_right_boundary) {
		if (node.circle.y > m_draw_set.y_left_boundary && node.circle.y < m_draw_set.y_right_boundary) {
			m_ren.draw_node(node);
		}
	}

	if (node.parent.radius != 0) { // not root
		Node_graphic parent = node;
		parent.circle = node.parent;

		m_ren.connect_node_pair(node, parent);
	}
}

void Application::draw_tree(){

	if (m_tree_changed)
	{
		m_finished_tree.clear();
		std::queue<Node_graphic> q;

		Node<int>* r = m_tree.get_root();
		Node_graphic root = Node_graphic({ 0,0,0 }, r, m_draw_set.x_root, m_draw_set.y_root, m_draw_set.circle_radius, r->value, "BLACK", LEFT);

		q.push(root);
		m_finished_tree.push_back(root);

		Node_graphic node;
		Node_graphic new_node;

		while (!q.empty()) {
			node = q.front();

			q.pop();

			draw_node(node);

			if (node.ptr->left)
			{
				new_node = make_node_graphic(node.ptr->left, node, LEFT);
				q.push(new_node);
				m_finished_tree.push_back(new_node);
			}

			if (node.ptr->right)
			{
				new_node = make_node_graphic(node.ptr->right, node, RIGHT);
				q.push(new_node);
				m_finished_tree.push_back(new_node);
			}
		}

		m_tree_changed = false;
	}
	else {
		for (auto& node : m_finished_tree) {
			draw_node(node);
		}
	}
}

void Application::run() {

	on_start();

	while (true) {

		m_fps.start();

		if (m_state == State::Closing)
			return;

		update_settings();
		event_handler();
		m_ren.clear();

		m_ren.draw_grid(m_win_set.camera_width, m_win_set.camera_height, m_draw_set.grid_step);

		if (!m_tree.empty()) {
			draw_tree();
		}

		m_fps.stop();

		if (m_fps.time_is_up())
			m_fps.update_fps();

		m_ren.draw_fps(m_fps.get_fps(), m_win_set.camera_width - 40, 20);
		m_ren.update();
	}
}