#include "Mouse_input.h"

void MouseInput::update() {
    m_prev_x = m_current_x;
    m_prev_y = m_current_y;

    m_current_mouse_state = SDL_GetMouseState(&m_current_x, &m_current_y);

    m_wheel_x = 0;
    m_wheel_y = 0;
}

void MouseInput::event_handler(const SDL_Event& event)
{
    switch (event.type)
    {
        case SDL_MOUSEWHEEL:
        {
            m_wheel_x = event.wheel.x;
            m_wheel_y = event.wheel.y;

            break;
        }
    }
}

void MouseInput::get_cursor_pos(int& x, int& y) const
{
    x = m_current_x;
    y = m_current_y;
}

void MouseInput::get_pos_diff(int& x_offset, int& y_offset) const
{
    x_offset = m_current_x - m_prev_x;
    y_offset = m_current_y - m_prev_y;
}

bool MouseInput::button_pressed(const Uint32 uButton) const
{
    return (SDL_BUTTON(uButton) & m_current_mouse_state) != 0;
}

int MouseInput::get_wheel_x(void) const
{
    return m_wheel_x;
}

int MouseInput::get_wheel_y(void) const
{
    return m_wheel_y;
}