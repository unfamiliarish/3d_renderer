#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

SDL_Window *window;
SDL_Renderer *renderer;

uint32_t *color_buffer;
SDL_Texture *color_buffer_texture;

int window_width;
int window_height;

bool initialize_window(void);
void destroy_window(void);

void render_color_buffer(void);
void clear_color_buffer(uint32_t color);

void draw_grid(uint32_t color);
void draw_rect(int x, int y, int width, int height, uint32_t color);

#endif