#include "RaylibExtra.h"

#include "Window.h"
#include "Platform.h"
#include "Player.h"
#include "obstacle.h"
#include <iostream>
using namespace std;

int main()
{
    window window(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib Learning");
    SetTargetFPS(FPS);

    const string dino_src = "resources/Graphics/dino.png";
    const string ground_src = "resources/Graphics/ground.png";
    const string sky_src = "resources/Graphics/Sky.png";
    const string ground_obstacles = "resources/Graphics/obstacle.png";
    const string fly_obstacles = "resources/Graphics/fly.png";

    Texture2D dinoTex = LoadTexture(dino_src.c_str());

    player dino(dino_src, 24, 12, FPS, 3, 10, 0.5f);
    // dino.update_dest_rec(80, 300 - dino.frame_height * 3 + 3*3, dino.frame_width * 3, dino.frame_height * 3);
    dino.update_dest_rec(80, 0, dino.frame_width * 3, dino.frame_height * 3);
    dino.set_collider(7 * 3, 4 * 3, 9 * 3, 17 * 3);
    dino.update_resolution();
    dino.init_animation("idle", 0, 3);
    dino.init_animation("move", 4, 10);
    dino.init_animation("kick", 11, 13);
    dino.init_animation("hurt", 14, 16);
    dino.init_animation("sneak", 17, 23);
    dino.reset_animation();

    platform sky1(sky_src, FPS, 4);
    platform sky2(sky_src, FPS, 4);
    sky1.update_dest_rec(0, 0, sky1.frame_width, sky1.frame_height);
    sky2.update_dest_rec(sky1.frame_width, 0, sky2.frame_width, sky2.frame_height);
    sky1.update_resolution();
    sky2.update_resolution();

    platform ground1(ground_src, FPS, 4);
    platform ground2(ground_src, FPS, 4);
    ground1.update_dest_rec(0, sky1.frame_height, ground1.frame_width, ground1.frame_height);
    ground2.update_dest_rec(ground1.frame_width, sky1.frame_height, ground2.frame_width, ground2.frame_height);
    ground1.update_resolution();
    ground2.update_resolution();

    dino.set_current_animation("move");
    dino.set_ground_height(ground1.get_rect().y);

    spawning spawnner(5, 0.5f, 3, ground1.get_rect().y);

    while (!WindowShouldClose())
    {

        string animation_name = "Animation: ";
        string str2 = "PRESS RIGHT/LEFT KEYS to CHANGE ANIMATION!";
        dino.play_animation();
        animation_name += dino.get_animation_state();

        const int offset = MeasureText(animation_name.c_str(), 20);
        const float offset2 = MeasureText(str2.c_str(), 10);

        window.begin_drawing();
        window.clear_background(RAYWHITE);
        /*obj.set_player(dino.get_collider());
        fly.set_player(dino.get_collider());*/
        // Draw background
        // Draw sky
        sky1.update();
        sky1.draw();

        sky2.update();
        sky2.draw();
        // Draw ground
        ground1.update();
        ground1.draw();

        ground2.update();
        ground2.draw();

        DrawText(animation_name.c_str(), SCREEN_WIDTH / 2 - offset / static_cast<float>(2), 110, 20, DARKGRAY);
        DrawText(str2.c_str(), (SCREEN_WIDTH - offset2) / 2.0f, 150, 10, DARKGRAY);

        // DrawPixel(80,300,BLACK);
        dino.set_ground_rect(ground1);
        dino.set_ground_rect(ground2);
        dino.update();
        dino.draw_player(0);
        // Debug
        // dino.draw_collider();

        spawnner.update(dino.get_collider(), dino.frame_height * 3);

        DrawText(to_string(score).c_str(), 10, 10, 20 * (SCREEN_HEIGHT / ROOT_HEIGHT), DARKGRAY);

        window.end_drawing();
    }
    return 0;
}
