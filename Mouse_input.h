#pragma once
#include <SDL.h>

class MouseInput {
public:
    MouseInput() = default;

    void update();

    void event_handler(const SDL_Event&);

    void get_cursor_pos(int&, int&) const;
    void get_pos_diff(int&, int&) const;

    bool button_pressed(const Uint32) const;

    int get_wheel_x() const;
    int get_wheel_y() const;

private:
    int m_current_x = 0;
    int m_current_y = 0;

    int m_prev_x = 0;
    int m_prev_y = 0;

    Sint32 m_wheel_x = 0;
    Sint32 m_wheel_y = 0;

    Uint32 m_current_mouse_state = 0;
};