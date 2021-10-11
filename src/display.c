#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <unistd.h> // notice this! you need it!

#include "display.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

uint32_t *color_buffer = NULL;
SDL_Texture *color_buffer_texture = NULL;

int window_width = 800;
int window_height = 600;

bool initialize_window(void)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }

    // Use SDL to query fullscreen monitor max width and height
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);

    window_width = display_mode.w;
    window_height = display_mode.h;

    // Create SDL window
    window = SDL_CreateWindow(
        "hello world",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width,
        window_height,
        SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        fprintf(stderr, "Error creating SDL window.\n");
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
    {
        fprintf(stderr, "Error creating SDL renderer.\n");
        return false;
    }
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    return true;
}

void draw_grid(uint32_t color)
{
    for (int row = 0; row < window_height; row += 10)
    {
        for (int col = 0; col < window_width; col += 10)
        {
            color_buffer[row * window_width + col] = color;
        }
    }
}

void draw_pixel(int x, int y, uint32_t color)
{
    if (x < window_width && y < window_height)
    {
        color_buffer[y * window_width + x] = color;
    }
}

void draw_rect(int x, int y, int width, int height, uint32_t color)
{
    for (int w = 0; w < width; w++)
    {
        for (int h = 0; h < height; h++)
        {
            color_buffer[(y + h) * window_width + (x + w)] = color;
        }
    }
}

void render_color_buffer(void)
{
    SDL_UpdateTexture(
        color_buffer_texture,
        NULL,
        color_buffer,
        (int)(window_width * sizeof(uint32_t)));

    SDL_RenderCopy(
        renderer,
        color_buffer_texture,
        NULL,
        NULL);
}

void clear_color_buffer(uint32_t color)
{
    for (int row = 0; row < window_height; row++)
    {
        for (int col = 0; col < window_width; col++)
        {
            color_buffer[row * window_width + col] = color;
        }
    }
}

void destroy_window(void)
{
    free(color_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
