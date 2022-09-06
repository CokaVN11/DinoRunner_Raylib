#pragma once
#ifndef PLAYER_
#define PLAYER_

#include "RaylibExtra.h"
#include <string>
#include <map>

#include "Platform.h"

class player
{
public:
    float frame_width = 0;
    float frame_height = 0;

    player(std::string path_to_sprite_input,
        int num_of_frame_input,
        int frame_speed_in,
        int fps_input,
        int offset = 0,
        float jump_height = 0,
        float gravity = 0.7f);
    void update_dest_rec(float x, float y, float width, float height);
    void set_rect_pos(float x, float y);
    void change_rotation(float rotation_input);
    void draw_player(float rotation_input);
    void init_animation(const std::string& name, int frame_start, int frame_end);
    void reset_animation();
    void set_current_animation(const std::string& name);
    void play_animation();

    void update_resolution();

    Rectangle get_collider() const;

    void set_collider(float rel_x, float rel_y, float width, float height);
    void set_ground_rect(const platform& ground);
    void set_ground_height(const float& ground_height);
    void keep_on_ground(const platform& ground);
    void draw_on_ground(const platform& ground, float rotation_input);
    std::string get_animation_state();

    void update();

    ~player() noexcept;

    // Debug
    void draw_collider();
private:
    std::string path_to_sprite_;
    Texture2D sprite_ = { 0, 0, 0, 0, 0 };

    int num_of_frame_ = 0;
    float rotation_ = 0;
    int frame_spd_ = 0;
    int fps_ = 0;
    int frame_counter_ = 0;
    int frame_current_ = 0;
    int offset_ = 0;

    Vector2 origin_ = { 0, 0 };
    Rectangle source_rec_ = { 0, 0, 0, 0 };
    Rectangle dest_rec_ = { 0, 0, 0, 0 };

    Vector2 relative_pos_collider_ = { 0,0 };
    Rectangle collider_rec_ = { 0,0,0,0 };
    std::map<std::string, std::pair<int, int>> animation_; //name, begin frame, end frame
    std::pair<int, int> ani_state_current_;

    Vector2 top_ = { 0, 0 }, bottom_ = { 0, 0 }, right_ = { 0, 0 }, left_ = { 0, 0 };
    Vector2 topleft_ = { 0, 0 }, topright_ = { 0, 0 };
    Vector2 bottomleft_ = { 0, 0 }, bottomright_ = { 0, 0 };
    Vector2 midtop_ = { 0, 0 }, midbottom_ = { 0, 0 }, midright_ = { 0, 0 }, midleft_ = { 0, 0 };
    Vector2 center_ = { 0, 0 };

    Rectangle ground_rect_ = { 0,0,0,0 };
    bool can_sneak_ = true;
    bool is_grounded_ = false;
    bool set_sneak_pos_ = false;

    float jump_height_ = 0;
    float gravity_ = 1.0f;
    float ground_y_ = 0.0f;
    float ground_height_ = 0.0f;
    float sneak_height = 0.0f;
    float normal_height = 0.0f;

    Vector2 normal_pos = { 0,0 };
    Vector2 sneak_pos = { 0,0 };
    Vector2 velocity_ = { 0,0 };

    void check_input();
    void check_is_grounded();
    void jump();
    void sneak();
    void release_sneak();
    void apply_move();
};


#endif // !PLAYER_