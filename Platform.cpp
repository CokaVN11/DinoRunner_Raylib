#include "Platform.h"
#include <iostream>
#include <raymath.h>

platform::platform(const std::string& path_to_string_input, int frame_speed_in, int spd)
{
    if (!path_to_string_input.empty())
    {
        path_to_sprite_ = path_to_string_input;
        sprite_ = LoadTexture(path_to_string_input.c_str());
        frame_width = static_cast<float>(sprite_.width);
        frame_height = static_cast<float>(sprite_.height);
        source_rec_.width = frame_width;
        source_rec_.height = frame_height;
        speed_ = spd;
    }
}

platform::~platform()
{
    if (!path_to_sprite_.empty()) UnloadTexture(sprite_);
}

void platform::set_rect_pos(const int& x, const int& y)
{
    source_rec_.x= static_cast<float>(x);
    source_rec_.y= static_cast<float>(y);
}

void platform::update_dest_rec(const float& x, const float& y, const float& width, const float& height)
{
    dest_rec_ = { x,y,width,height };
    startX_ = dest_rec_.x;
    endX_ = startX_ - dest_rec_.width;
}

void platform::draw() const
{
    DrawTexturePro(sprite_, source_rec_, dest_rec_, {0,0}, 0,WHITE);
}

void platform::update_resolution()
{
    float ration_x = static_cast<float> (SCREEN_WIDTH) / static_cast<float>(ROOT_WIDTH);
    float ration_y = static_cast<float> (SCREEN_HEIGHT) / static_cast<float>(ROOT_HEIGHT);

    dest_rec_.width *= ration_x;
    dest_rec_.height *= ration_y;
    dest_rec_.x *= ration_x;
    dest_rec_.y *= ration_y;
    startX_ *= ration_x;
    endX_ *= ration_x;
}

Rectangle platform::get_rect() const
{
    return dest_rec_;
}

void platform::update() {
    //std::cout << GetTime() << "\n";
    apply_move();
    //std::cout << "Ground X = " << dest_rec_.x << "\n";
}

void platform::apply_move()
{
    if (endX_ == startX_) return;

    dest_rec_.x -= speed_;

    if (dest_rec_.x <= endX_) dest_rec_.x = startX_;
}
