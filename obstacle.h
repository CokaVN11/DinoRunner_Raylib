#pragma once
#ifndef OBSTACLES_
#define OBSTACLES_

#include "RaylibExtra.h"
#include <string>
#include <map>
#include <vector>

class obstacles
{
public:
	float frame_width, frame_height;

	obstacles(std::string path_to_sprite_input,
			  int num_of_frame_input,
			  int frame_speed_in,
			  float speed_in,
			  int fps_input,
			  ObstacleType type = ground);
	~obstacles();
	void update_dest_rec(float x, float y, float width, float height);
	void update_resolution();
	void init_animation(const std::string &name, int frame_start, int frame_end);
	void reset_animation();
	void set_current_animation(const std::string &name);
	void play_animation();

	void set_ground_height(const float &ground_height);
	void set_player(const Rectangle &player_rect);

	void set_collider(float rel_x, float rel_y, float width, float height);
	Rectangle get_collider();

	void update();
	void draw();

	bool is_available();
	bool can_score();

	// debug
	void draw_collider();

private:
	std::string path_to_sprite_;
	Texture2D sprite_ = {0, 0, 0, 0, 0};

	int num_of_frame_ = 0;
	int frame_spd_ = 0;
	int fps_ = 0;
	int frame_counter_ = 0, frame_current_ = 0;
	int offset_width_ = 0;
	int offset_height_ = 0;
	int gravity_ = 10.0f;
	float spd_ = 0.0f;

	float startX_ = 0.0f, endX_ = 0.0f;

	ObstacleType type_ = none;
	Vector2 origin_ = {0, 0};
	Rectangle source_rec_ = {0, 0, 0, 0};
	Rectangle dest_rec_ = {0, 0, 0, 0};

	Vector2 relative_pos_collider_ = {0, 0};
	Rectangle collider_rec_ = {0, 0, 0, 0};

	std::map<std::string, std::pair<int, int>> animation_; // name, begin frame, end frame
	std::pair<int, int> ani_state_current_;

	Rectangle player_rect_ = {0, 0, 0, 0};

	Vector2 velocity_ = {-spd_, 0.0f};

	bool is_grounded_ = false;
	bool is_collided_ = false;

	float ground_y_ = 0.0f;
	float ground_height_ = 0.0f;

	void check_is_grounded();
	void check_is_collided();
	void apply_move();
};

class spawning
{
public:
	spawning(double startTime, double decreaseTime, double minTime, float ground_height);
	void update(const Rectangle &player_rect, const float &player_height);
	~spawning();

private:
	std::vector<obstacles *> obj_;
	double timeBtwSpawn_ = 0.0f;
	double decreaseTime_ = 0.0f;
	double minTime_ = 0.0f;
	double startTime_ = 0.0f;

	float ground_height_ = 0.0f;

	const std::string ground_obstacles = "resources/Graphics/obstacle.png";
	const std::string fly_obstacles = "resources/Graphics/fly.png";
};
#endif // !OBSTACLES_
