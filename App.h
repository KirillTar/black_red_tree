#pragma once
#include "Mouse_input.h"
#include "Renderer.h"
#include "Tree.h"
#include "FPS.h"
#include <vector>

class Application {

public:
	Application() = default;
	~Application();

	int init();
	void run();

	Application& operator=(const Application&) = delete;
	Application& operator=(const Application&&) = delete;
	Application(const Application&) = delete;
	Application(const Application&&) = delete;

private:
	void on_start();
	void update_settings();
	void event_handler();
	void mouse_state_update();
	void get_console_command();
	void draw_node(const Node_graphic&);
	void draw_tree();
	int calculate_tree_deep();

	Node_graphic make_node_graphic(const Node<int>* node, const Node_graphic& parent, bool side) /*side: false - left, true - right*/;

private:

	struct Window_settings {
		int max_width = 1'000'000;
		int max_height = 1'000'000;

		int camera_width = 1500;
		int camera_height = 800;

		int min_width = camera_width;
		int min_height = camera_height;

		int x_offset = 0;
		int y_offset = 0;
	};

	struct Draw_settings {
		int x_left_boundary = 0;
		int y_left_boundary = 0;

		int x_right_boundary = 0;
		int y_right_boundary = 0;

		int x_root = 1000;
		int y_root = 500;

		int circle_radius = 10;
		int node_distanse;
		int grid_step = 50;

		int tree_deep;
	};

	enum class State { Default, Running, Closing };

	SDL_Window* m_window = nullptr;
	SDL_Event m_e;

	State m_state = State::Default;

	Renderer m_ren;
	MouseInput m_mouse;

	Window_settings m_win_set;
	Draw_settings m_draw_set;

	FPS m_fps;

	std::vector<Node_graphic> m_finished_tree;
	Tree<int> m_tree;

	bool m_tree_changed;
};
