#pragma once
#include <chrono>

using namespace std::chrono;

class FPS {

public:

	FPS() = default;
	void start();
	void stop();
	void update_fps();
	int get_fps();
	bool time_is_up();

private:

	time_point<high_resolution_clock> m_start_point;
	time_point<high_resolution_clock> m_end_point;
	int m_current_fps = 0;
	int m_fps = 0;
	long long m_duration_reduction = 250'000;
};