#pragma once
#ifndef RAYLIB_WINDOW_CPP
#define RAYLIB_WINDOW_CPP

#include <stdexcept>
#include <string>
#include <raylib.h>

class window
{
public:
    window(int width, int height, const std::string& title, bool late_init);

    static bool init(int width, int height, const std::string& title);
    static bool should_close();
    static bool is_cursor_on_screen();
    static bool is_ready();
    static bool is_fullscreen();
    static bool is_hidden();
    static bool is_minimized();
    static bool is_maximized();
    static bool is_focused();
    static bool is_resized();
    static bool is_state(unsigned int flag);

    window& set_state(unsigned int flag);
    window& clear_state(unsigned int flag);
    window& clear_background(const Color& color = BLACK);
    window& toggle_fullscreen();
    window& set_fullscreen(bool fullscreen);
    window& maximize();
    window& minimize();
    window& restore();
    window& set_icon(const Image& image);
    window& set_title(const std::string& title);
    window& set_position(int x, int y);
    window& set_position(const Vector2& position);
    window& set_monitor(int monitor);
    window& set_min_size(int width, int height);
    window& set_min_size(const Vector2& size);
    window& set_size(int width, int height);
    window& set_size(const Vector2& size);
    static Vector2 get_size();
    static void* get_handle();
    window& begin_drawing();
    window& end_drawing();
    static int get_width();
    static int get_height();
    static Vector2 get_position();
    static Vector2 get_scale_dpi();
    window& set_target_fps(int fps);
    static int get_fps();
    window& draw_fps(int pos_x = 10, int pos_y = 10);
    static float get_frame_time();
    static double get_time();

    ~window();
private:
};

inline window::window(const int width, const int height, const std::string& title = "raylib", const bool late_init = false)
{
    if (!late_init)
    {
        if (!init(width, height, title))
        {
            throw std::runtime_error("Failed to create Window");
        }
    }
}

inline bool window::init(const int width = 800, const int height = 450, const std::string& title = "raylib")
{
    InitWindow(width, height, title.c_str());
    return IsWindowReady();
}

inline bool window::should_close()
{
    return WindowShouldClose();
}

inline bool window::is_cursor_on_screen()
{
    return IsCursorOnScreen();
}

inline bool window::is_ready()
{
    return IsWindowReady();
}

inline bool window::is_fullscreen()
{
    return IsWindowFullscreen();
}

inline bool window::is_hidden()
{
    return IsWindowHidden();
}

inline bool window::is_minimized()
{
    return IsWindowMinimized();
}

inline bool window::is_maximized()
{
    return IsWindowMaximized();
}

inline bool window::is_focused()
{
    return IsWindowFocused();
}

inline bool window::is_resized()
{
    return IsWindowResized();
}

inline bool window::is_state(const unsigned int flag)
{
    return IsWindowState(flag);
}

inline window& window::set_state(const unsigned int flag)
{
    // TODO: insert return statement here
    SetWindowState(flag);
    return *this;
}

inline window& window::clear_state(const unsigned int flag)
{
    // TODO: insert return statement here
    ClearWindowState(flag);
    return *this;
}

inline window& window::clear_background(const Color& color)
{
    // TODO: insert return statement here
    ::ClearBackground(color);
    return *this;
}

inline window& window::toggle_fullscreen()
{
    // TODO: insert return statement here
    ::ToggleFullscreen();
    return *this;
}

inline window& window::set_fullscreen(const bool fullscreen)
{
    // TODO: insert return statement here
    if (fullscreen)
    {
        if (!is_fullscreen())
        {
            toggle_fullscreen();
        }
    }
    else
    {
        if (is_fullscreen())
        {
            toggle_fullscreen();
        }
    }

    return *this;
}

inline window& window::maximize()
{
    // TODO: insert return statement here
    MaximizeWindow();
    return *this;
}

inline window& window::minimize()
{
    // TODO: insert return statement here
    MinimizeWindow();
    return *this;
}

inline window& window::restore()
{
    // TODO: insert return statement here
    RestoreWindow();
    return *this;
}

inline window& window::set_icon(const Image& image)
{
    // TODO: insert return statement here
    SetWindowIcon(image);
    return *this;
}

inline window& window::set_title(const std::string& title)
{
    // TODO: insert return statement here
    SetWindowTitle(title.c_str());
    return *this;
}

inline window& window::set_position(int x, int y)
{
    // TODO: insert return statement here
    SetWindowPosition(x, y);
    return *this;
}

inline window& window::set_position(const Vector2& position)
{
    SetWindowPosition(static_cast<int>(position.x), static_cast<int>(position.y));
    return *this;
}

inline window& window::set_monitor(int monitor)
{
    SetWindowMonitor(monitor);
    return *this;
}

inline window& window::set_min_size(int width, int height)
{
    // TODO: insert return statement here
    SetWindowMinSize(width, height);
    return *this;
}

inline window& window::set_min_size(const Vector2& size)
{
    SetWindowMinSize(static_cast<int>(size.x), static_cast<int>(size.y));
    return *this;
}

inline window& window::set_size(int width, int height)
{
    // TODO: insert return statement here
    SetWindowSize(width, height);
    return *this;
}

inline window& window::set_size(const Vector2& size)
{
    // TODO: insert return statement here
    return set_size(static_cast<int>(size.x), static_cast<int>(size.y));
}

inline Vector2 window::get_size()
{
    return {static_cast<float>(get_width()), static_cast<float>(get_height())};
}

inline void* window::get_handle()
{
    return GetWindowHandle();
}

inline window& window::begin_drawing()
{
    // TODO: insert return statement here
    ::BeginDrawing();
    return *this;
}

inline window& window::end_drawing()
{
    // TODO: insert return statement here
    ::EndDrawing();
    return *this;
}

inline int window::get_width()
{
    return GetScreenWidth();
}

inline int window::get_height()
{
    return GetScreenHeight();
}

inline Vector2 window::get_position()
{
    return GetWindowPosition();
}

inline Vector2 window::get_scale_dpi()
{
    return GetWindowScaleDPI();
}

inline window& window::set_target_fps(int fps)
{
    ::SetTargetFPS(fps);
    return *this;
    // TODO: insert return statement here
}

inline int window::get_fps()
{
    return ::GetFPS();
}

inline window& window::draw_fps(int pos_x, int pos_y)
{
    // TODO: insert return statement here
    ::DrawFPS(pos_x, pos_y);
    return *this;
}

inline float window::get_frame_time()
{
    return ::GetFrameTime();
}

inline double window::get_time()
{
    return ::GetTime();
}

inline window::~window()
{
    CloseWindow();
}


#endif // !RAYLIB_WINDOW_CPP
