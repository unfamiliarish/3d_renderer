#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "display.h"
#include "vector.h"

bool is_running = false;

////////////////////////////////////////////////////////////////////////////////
// Declare an array of vectors/points
////////////////////////////////////////////////////////////////////////////////
#define N_POINTS (9 * 9 * 9)
vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];

vec3_t camera_position = {.x = 0, .y = 0, .z = -5};

float fov_factor = 640;

void setup(void)
{
    // Allocate the required memory in bytes to hold the color buffer
    color_buffer = (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);

    // Creating an SDL texture that is used to display the color buffer
    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        window_width,
        window_height);

    int point_count = 0;

    // Start loading my array of vectors
    // From -1 to 1 (in this 9x9x9 cube)
    for (float x = -1; x <= 1; x += .25)
    {
        for (float y = -1; y <= 1; y += .25)
        {
            for (float z = -1; z <= 1; z += .25)
            {
                vec3_t new_point = {.x = x, .y = y, .z = z};
                cube_points[point_count++] = new_point;
            }
        }
    }
}

void process_input(void)
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        is_running = false;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
            is_running = false;
        break;
    }
}
////////////////////////////////////////////////////////////////////////////////
// Function that received a 3d vector and returns a 2d point
////////////////////////////////////////////////////////////////////////////////
vec2_t project_ortholinear(vec3_t point)
{
    vec2_t projected_point = {
        .x = fov_factor * point.x,
        .y = fov_factor * point.y};
    return projected_point;
}

vec2_t project_perspective(vec3_t point)
{
    vec2_t projected_point = {
        .x = (fov_factor * point.x) / point.z,
        .y = (fov_factor * point.y) / point.z};
    return projected_point;
}

void update(void)
{
    for (int i = 0; i < N_POINTS; i++)
    {
        vec3_t point = cube_points[i];

        // move the points away from the camera
        point.z -= camera_position.z;

        // project the current point
        vec2_t projected_point = project_perspective(point);

        // save the projected 2d vector in the array of projected points
        projected_points[i] = projected_point;
    }
}

void render(void)
{
    draw_grid(0xFF333333);

    // Loop all projected points and render them
    for (int i = 0; i < N_POINTS; i++)
    {
        vec2_t projected_point = projected_points[i];
        draw_rect(
            projected_point.x + window_width / 2,
            projected_point.y + window_height / 2,
            4,
            4,
            0xFFFFFF00);
    }

    render_color_buffer();
    clear_color_buffer(0xFF000000);

    SDL_RenderPresent(renderer);
}

int main(void)
{
    is_running = initialize_window();

    setup();

    while (is_running)
    {
        process_input();
        update();
        render();
    }

    destroy_window();

    return 0;
}
