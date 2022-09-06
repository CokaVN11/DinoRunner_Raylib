#include "obstacle.h"
#include <assert.h>
#include <iostream>

obstacles::obstacles(std::string path_to_sprite_input, int num_of_frame_input, int frame_speed_in, float speed_in, int fps_input, ObstacleType type)
	:path_to_sprite_(std::move(path_to_sprite_input)), num_of_frame_(num_of_frame_input), 
	frame_spd_(frame_speed_in), spd_(speed_in), fps_(fps_input), type_(type)
{
	assert(!path_to_sprite_.empty());
	sprite_ = LoadTexture(path_to_sprite_.c_str());
	frame_width = static_cast<float>(sprite_.width) / static_cast<float>(num_of_frame_);
	frame_height = static_cast<float>(sprite_.height);
	source_rec_.width = frame_width;
	source_rec_.height = frame_height;
	source_rec_.x = offset_width_ + 1;
	source_rec_.y = offset_height_ + 1;
}

obstacles::~obstacles()
{
	assert(!path_to_sprite_.empty());
	UnloadTexture(sprite_);
}

void obstacles::update_dest_rec(float x, float y, float width, float height)
{
	dest_rec_.x = x;
	dest_rec_.y = y;
	dest_rec_.width = width;
	dest_rec_.height = height;
	startX_ = dest_rec_.x + SCREEN_WIDTH;
	endX_ = -dest_rec_.width - 50;
	dest_rec_.x = startX_;
}

void obstacles::update_resolution()
{
	float ration = static_cast<float> (SCREEN_WIDTH) / static_cast<float>(ROOT_WIDTH);

	dest_rec_.width *= ration;
	dest_rec_.height *= ration;
	if (startX_ < SCREEN_WIDTH)
		startX_ *= ration;
	endX_ *= ration;
	
	dest_rec_.x = startX_;
	dest_rec_.y *= ration;

	collider_rec_.x *= ration;
	collider_rec_.y *= ration;
	collider_rec_.width *= ration;
	collider_rec_.height *= ration;
	relative_pos_collider_.x *= ration;
	relative_pos_collider_.y *= ration;
}

void obstacles::init_animation(const std::string& name, int frame_start, int frame_end)
{
	animation_[name] = { frame_start, frame_end };
}

void obstacles::reset_animation()
{
	frame_counter_ = 0;
	frame_current_ = 0;
}

void obstacles::set_current_animation(const std::string& name)
{
	assert(animation_.find(name) != animation_.end());
	ani_state_current_ = animation_[name];
	frame_current_ = ani_state_current_.first;
}

void obstacles::play_animation()
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

void obstacles::set_ground_height(const float& ground_height)
{
	ground_y_ = ground_height;
	ground_height_ = ground_y_ - collider_rec_.height - relative_pos_collider_.y;
}

void obstacles::set_player(const Rectangle& player_rect)
{
	player_rect_ = player_rect;
}

void obstacles::set_collider(float rel_x, float rel_y, float width, float height)
{
	collider_rec_ = { dest_rec_.x + rel_x, dest_rec_.y + rel_y, width, height };

	relative_pos_collider_ = { rel_x, rel_y };
}

Rectangle obstacles::get_collider()
{
	return collider_rec_;
}

void obstacles::update()
{
	check_is_grounded();
	check_is_collided();
	apply_move();
}

void obstacles::draw()
{
	DrawTexturePro(sprite_, source_rec_, dest_rec_, origin_, 0, WHITE);
}

bool obstacles::is_available()
{
	return (!is_collided_ && dest_rec_.x >= endX_);
}

bool obstacles::can_score()
{
	return dest_rec_.x < endX_;
}

void obstacles::draw_collider()
{
	DrawRectangleLines(collider_rec_.x, collider_rec_.y, collider_rec_.width, collider_rec_.height, BLACK);
	DrawRectangleLines(dest_rec_.x, dest_rec_.y, dest_rec_.width, dest_rec_.height, BLACK);
}

void obstacles::check_is_grounded()
{
	is_grounded_ = dest_rec_.y >= ground_y_ - dest_rec_.height;
}

void obstacles::check_is_collided()
{
	is_collided_ = CheckCollisionRecs(collider_rec_, player_rect_);
	if (is_collided_) std::cout << collider_rec_.x << " " << player_rect_.x + player_rect_.width << "\n";
 }

void obstacles::apply_move() {
	assert(endX_ != startX_);
	if (!is_grounded_) {
		velocity_.y += gravity_;
	}
	dest_rec_.x += velocity_.x;
	dest_rec_.y += velocity_.y;
	if (is_grounded_ && velocity_.y > 0) {
		velocity_.y = 0;
		dest_rec_.y = ground_y_ - dest_rec_.height;
	}
	/*if (dest_rec_.x <= endX_) {
		dest_rec_.x = startX_;
		score++;
		std::cout << "Obstacle: " << score << "\n";
	}

	if (is_collided_) dest_rec_.x = startX_;*/

	collider_rec_.x = dest_rec_.x + relative_pos_collider_.x;
	collider_rec_.y = dest_rec_.y + relative_pos_collider_.y;
	//std::cout << ((type_ == ObstacleType::ground) ? "Ground X: " : "Fly X: ") << startX_ << "\n";
	//std::cout << ((type_ == ObstacleType::ground) ? "Ground: " : "Fly: ") << is_collided_ << "\n";

}

spawning::spawning(double startTime, double decreaseTime, double minTime, float ground_height)
	: startTime_(startTime), decreaseTime_(decreaseTime), minTime_(minTime), ground_height_(ground_height)
{
	srand(time(0));
}


void spawning::update(const Rectangle& player_rect, const float& player_height)
{
	if (timeBtwSpawn_ <= 0) {
		int rand_type = (rand() % 2) + 1;
		std::cout << rand_type << "\n";
		switch (rand_type)
		{
		case ObstacleType::ground:
		{
			std::cout << "Ground\n";
			obstacles* ground = new obstacles(ground_obstacles, 3, 10, 4, FPS, ObstacleType::ground);
			ground->update_dest_rec(0, 237, ground->frame_width * 1.25f, ground->frame_height * 1.25f);
			ground->set_collider(4 * 1.25f, 6 * 1.25f, 15 * 1.25f, 18 * 1.25f);
			ground->update_resolution();
			ground->init_animation("idle", 0, 2);
			ground->reset_animation();
			ground->set_current_animation("idle");
			ground->set_ground_height(ground_height_ + (SCREEN_HEIGHT / ROOT_HEIGHT));
			obj_.push_back(ground);
			break;
		}
		case ObstacleType::fly:
		{
			obstacles* fly = new obstacles(fly_obstacles, 3, 12, 4, FPS, ObstacleType::fly);
			fly->update_dest_rec(0, 237 - fly->frame_height * 1.25f, fly->frame_width * 1.25f, fly->frame_height * 1.25f);
			fly->set_collider(0 * 1.25f, 4 * 1.25f, 24 * 1.25f, 17 * 1.25f);
			fly->update_resolution();
			fly->init_animation("idle", 0, 2);
			fly->reset_animation();
			fly->set_current_animation("idle");
			fly->set_ground_height(ground_height_ - (player_height - fly->frame_height * 1.25f - 5) * (SCREEN_HEIGHT / ROOT_HEIGHT));
			obj_.push_back(fly);
			break;
		}
		default:
			break;
		}
		timeBtwSpawn_ = startTime_;
		if (startTime_ > minTime_) startTime_ -= decreaseTime_;
		else if (startTime_ < minTime_) startTime_ = minTime_;
	}
	else timeBtwSpawn_ -= GetFrameTime();

	for (auto i = 0; i < obj_.size();i++) {
		/*if (x->is_available()) x->update();
		if (x->can_score()) score++;
		if (!x->is_available()) {
			auto del = x;
			obj_.erase()
		}*/
		obj_[i]->set_player(player_rect);
		if (obj_[i]->is_available()) {
			obj_[i]->play_animation();
			obj_[i]->update();
			obj_[i]->draw();
		}
		if (obj_[i]->can_score()) score++;
		if (!obj_[i]->is_available()) {
			auto del = obj_[i];
			obj_.erase(obj_.begin() + i);
			delete del;
		}
	}
}

spawning::~spawning()
{
	if (obj_.size() <= 0) return;
	for (auto x : obj_) {
		delete x;
	}
}
