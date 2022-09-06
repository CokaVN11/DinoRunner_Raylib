#include "Player.h"

#include <iostream>
#include <utility>
#include <assert.h>

player::player(std::string path_to_sprite_input, const int num_of_frame_input, const int frame_speed_in,
               const int fps_input, const int offset, const float jump_height, const float gravity)
    : path_to_sprite_(std::move(path_to_sprite_input)), num_of_frame_(num_of_frame_input), frame_spd_(frame_speed_in),
      fps_(fps_input), offset_(offset), jump_height_(jump_height), gravity_(gravity)
{
    if (!path_to_sprite_.empty())
    {
        sprite_ = LoadTexture(path_to_sprite_.c_str());
        frame_width = static_cast<float>(sprite_.width) / static_cast<float>(num_of_frame_);
        frame_height = static_cast<float>(sprite_.height);
        source_rec_.width = frame_width;
        source_rec_.height = frame_height;
    }
}

void player::update_dest_rec(const float x, const float y, const float width, const float height)
{
    set_rect_pos(x, y);
    dest_rec_.x = bottomleft_.x;
    dest_rec_.y = bottomleft_.y;
    dest_rec_.width = width;
    dest_rec_.height = height;

    

    normal_pos = { dest_rec_.x, dest_rec_.y };
    sneak_pos = { dest_rec_.x, dest_rec_.y + 4 * 3 };
}

void player::set_rect_pos(const float x, const float y)
{
    topleft_ = {x, y};
    midtop_ = {x - dest_rec_.width / 2, y};
    topright_ = {x - dest_rec_.width, y};

    center_ = {x - dest_rec_.width / 2, y - dest_rec_.height / 2};

    bottomright_ = {x - dest_rec_.width, y - dest_rec_.height};
    midbottom_ = {x - dest_rec_.width / 2, y - dest_rec_.height};
    bottomleft_ = {x, y - dest_rec_.height};

    midleft_ = {x, y - dest_rec_.height / 2};
    midright_ = {x - dest_rec_.width, y - dest_rec_.height / 2};

    left_ = midleft_;
    right_ = midright_;
    bottom_ = midbottom_;
    top_ = midtop_;
}

void player::change_rotation(const float rotation_input)
{
    rotation_ = rotation_input;
}

void player::draw_player(const float rotation_input)
{
    rotation_ = rotation_input;
    DrawTexturePro(sprite_, source_rec_, dest_rec_, origin_, rotation_input, WHITE);
}

void player::init_animation(const std::string& name, int frame_start, int frame_end)
{
    animation_[name] = {frame_start, frame_end};
}

void player::reset_animation()
{
    frame_counter_ = 0;
    frame_current_ = 0;
}

void player::set_current_animation(const std::string& name)
{
    assert(animation_.find(name) != animation_.end());
    ani_state_current_ = animation_[name];
    frame_current_ = ani_state_current_.first;
}


void player::play_animation()
{
    frame_counter_++;
    if (frame_counter_ >= (fps_ / frame_spd_))
    {
        frame_counter_ = 0;
        frame_current_++;

        if (frame_current_ >= ani_state_current_.second) frame_current_ = ani_state_current_.first;
        source_rec_.x = static_cast<float>(frame_current_) * frame_width;
    }
}

void player::update_resolution()
{
    float ration = static_cast<float> (SCREEN_WIDTH) / static_cast<float>(ROOT_WIDTH);

    dest_rec_.width *= ration;
    dest_rec_.height *= ration;
    normal_height *= ration;
    sneak_height *= ration;
    jump_height_ *= ration;
    gravity_ *= ration;

    collider_rec_.x *= ration;
    collider_rec_.y *= ration;
    collider_rec_.width *= ration;
    collider_rec_.height *= ration;
    relative_pos_collider_.x *= ration;
    relative_pos_collider_.y *= ration;
}

Rectangle player::get_collider() const
{
    return collider_rec_;
}

void player::set_collider(float rel_x, float rel_y, float width, float height)
{
    collider_rec_.x = dest_rec_.x + rel_x;
    collider_rec_.y = dest_rec_.y + rel_y;
    collider_rec_.width = width;
    collider_rec_.height = height;

    normal_height = collider_rec_.height;
    sneak_height = normal_height - 4*3;

    relative_pos_collider_ = { rel_x, rel_y };
}

void player::set_ground_rect(const platform& ground)
{
    ground_rect_ = ground.get_rect();
}

void player::set_ground_height(const float& ground_height)
{
    ground_y_ = ground_height;
    ground_height_ = ground_y_ - collider_rec_.height - relative_pos_collider_.y;
}

void player::keep_on_ground(const platform& ground)
{
    while (!CheckCollisionRecs(dest_rec_, ground.get_rect()))
    {
        velocity_.y += gravity_;
    }
}

void player::draw_on_ground(const platform& ground, const float rotation_input)
{
    keep_on_ground(ground);
    draw_player(rotation_input);
}

std::string player::get_animation_state()
{
    for (const auto& [key, value] : animation_) {
        if (value.first == ani_state_current_.first)
            return key;
    }
}

void player::update()
{
    check_input();    
    check_is_grounded();
    apply_move();
}

player::~player() noexcept
{
    if (!path_to_sprite_.empty())
        UnloadTexture(sprite_);
}

void player::draw_collider()
{
    //DrawPixel(dest_rec_.x, dest_rec_.y, DARKGRAY);
    DrawRectangleLines(collider_rec_.x, collider_rec_.y, collider_rec_.width, collider_rec_.height, BLACK);
    DrawRectangleLines(dest_rec_.x, dest_rec_.y, dest_rec_.width, dest_rec_.height, BLACK);

    //std::cout << collider_rec_.height << "\n";
}

void player::check_input()
{
    if (IsKeyPressed(KEY_SPACE))
    {
        //std::cout << "JUMP\n";
        jump();
        set_current_animation("idle");
    }
    else if (IsKeyDown(KEY_DOWN)) {
        //std::cout << "SNEAK\n";
        sneak();
    }
    if (IsKeyUp(KEY_DOWN)) release_sneak();
}

void player::check_is_grounded()
{
    /*is_grounded_ = CheckCollisionRecs(dest_rec_, ground_rect_);*/
    is_grounded_ = collider_rec_.y >= ground_y_ - collider_rec_.height;
}

void player::jump()
{
    if (!is_grounded_) return;
    velocity_.y = -jump_height_;
    
}

void player::sneak()
{
    if (can_sneak_ && is_grounded_) {
        can_sneak_ = false;
        collider_rec_.height = sneak_height;
        set_current_animation("sneak");

        //std::cout <<"R u see me: " << sneak_pos.y << "\n";
        //dest_rec_.y = sneak_pos.y;
    }
}

void player::release_sneak()
{
    if (!can_sneak_) {
        can_sneak_ = true;
        collider_rec_.height = normal_height;
        set_current_animation("move");

        //dest_rec_.y = normal_pos.y;
    }
}

void player::apply_move()
{
    if (!is_grounded_)
    {
        velocity_.y += gravity_;
    }
    if (is_grounded_ && velocity_.y > 0) {
        velocity_.y = 0;
        if (can_sneak_) set_current_animation("move");
    }
    dest_rec_.x += velocity_.x;
    dest_rec_.y += velocity_.y;

    //float ground_height = ground_y_ - collider_rec_.height - relative_pos_collider_.y;
    //std::cout << "Ground height: " << ground_height_ << "\n";
    
    if (dest_rec_.y > ground_height_)
        dest_rec_.y = ground_height_;

    
    collider_rec_.x = dest_rec_.x + relative_pos_collider_.x;
    if (can_sneak_)
        collider_rec_.y = dest_rec_.y + relative_pos_collider_.y;
    else collider_rec_.y = ground_y_ -  sneak_height;

    //std::cout << ((is_grounded_) ? "On ground\n" : "In sky\n");
}

