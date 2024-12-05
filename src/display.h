#ifndef DISPLAY_H
#define DISPLAY_H

#include "triangle.h"
#include "vector.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

#define FPS (30)
#define FRAME_TARGET_TIME (1000 / FPS)

// SDL window and renderer
extern SDL_Window *window;
extern SDL_Renderer *renderer;

// Color buffer and texture buffer
extern uint32_t *color_buffer;
extern SDL_Texture *color_buffer_texture;

// Window width and height
extern int window_width;
extern int window_height;

bool initialize_window(void);
void render_color_buffer(void);
void clear_color_buffer(uint32_t color);
void destroy_window(void);

void draw_pixel(int x, int y, uint32_t color);
void draw_grid(int grid_size);
void draw_rect(int x, int y, int width, int height, uint32_t color);
void draw_line(int x0, int y0, int x1, int y1, uint32_t color);

void draw_line_w_vec2D(vec2_t pointA, vec2_t pointB, uint32_t color);
void draw_triangle_w_Triagnle(triangle_t t, uint32_t color);

#endif
