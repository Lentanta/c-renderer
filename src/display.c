#include "display.h"
#include <stdint.h>

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

// Declare a pointer to (the first element of) an array of uint32 elements
// Equal to unit32[]
uint32_t *color_buffer = NULL;
SDL_Texture *color_buffer_texture;

int window_width;
int window_height;

bool initialize_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL\n");
    return false;
  }

  // Use SDL to query what is the fullscreen max
  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode);

  window_width = 800;  // display_mode.w;
  window_height = 600; // display_mode.h;

  // Create SDL window
  window =
      SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       window_width, window_height, SDL_WINDOW_BORDERLESS);
  if (!window) {
    fprintf(stderr, "Error creating SDL window\n");
    return false;
  }

  // Create a SDL renderer
  // -1 and 0 are for enable the default setting
  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    fprintf(stderr, "Error creating SDL renderer\n");
    return false;
  }

  // Enable this for display fullscreen
  // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
  return true;
}

void render_color_buffer(void) {
  // Number of bytes in row of pixel data
  int pitch = window_width * sizeof(uint32_t);

  // Copy pixel(colors) data from color_buffer to color_buffer_texture
  SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, pitch);
  SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void clear_color_buffer(uint32_t color) {
  // Convert from 2D array postions to 1D array postions
  for (int y = 0; y < window_height; y++) {
    for (int x = 0; x < window_width; x++) {
      int position = (window_width * y) + x;
      color_buffer[position] = color;
    }
  }
}

void destroy_window(void) {
  free(color_buffer);
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
}

// ---------------------- //
// |   Draw functions   | //
// ---------------------- //

void draw_pixel(int x, int y, uint32_t color) {
  if (x < 0 || x > window_width || y < 0 || y > window_height) {
    return;
  }

  int position = (window_width * y) + x;
  color_buffer[position] = color;
}

void draw_grid(int number) {
  for (int y = 0; y < window_height; y++) {
    for (int x = 0; x < window_width; x++) {
      int position = (window_width * y) + x;
      if (y % number == 0 || x % number == 0) {
        color_buffer[position] = 0x000000;
      }
    }
  }
}

void draw_rect(int x, int y, int width, int height, uint32_t color) {
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      int current_x = x + i;
      int current_y = y + j;
      draw_pixel(current_x, current_y, color);
    }
  }
}
