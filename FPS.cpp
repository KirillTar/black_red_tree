#include "FPS.h"

using namespace std::chrono;

void FPS::start() {
	m_start_point = high_resolution_clock::now();
}

void FPS::stop() {
	m_end_point = high_resolution_clock::now();

	auto elapced_start = time_point_cast<microseconds>(m_start_point).time_since_epoch().count();
	auto elapced_end = time_point_cast<microseconds>(m_end_point).time_since_epoch().count();

	auto duration = elapced_end - elapced_start;
	m_duration_reduction += duration;

	m_current_fps = 1'000'000 / duration;
}

void FPS::update_fps() {
	m_fps = m_current_fps;
	m_duration_reduction = 0;
}

int FPS::get_fps() {
	return m_fps;
}

bool FPS::time_is_up() {
	return (m_duration_reduction > 250'000);
}