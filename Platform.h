#pragma once
#ifndef PLATFORM_
#define PLATFORM_

#include "RaylibExtra.h"
#include <string>

class platform
{
public:
    float frame_width, frame_height;
    platform(const std::string& path_to_string_input, int frame_speed_in, int spd);
    ~platform();
    void set_rect_pos(const int& x, const int& y);
    void update_dest_rec(const float& x, const float& y, const float& width, const float& height);
    void draw() const;

    void update_resolution();

    Rectangle get_rect() const;

    void update();

    
private:
    std::string path_to_sprite_;
    Texture2D sprite_ = { 0,0,0,0,0 };
    Rectangle source_rec_ = { 0,0,0,0 }, dest_rec_ = { 0,0,0,0 };

    int speed_;
    float endX_, startX_;
    Vector2 velocity_ = { static_cast<float>(-speed_),0 };
    void apply_move();
};

#endif // !PLATFORM_