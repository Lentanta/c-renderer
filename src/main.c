#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// constants
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

bool is_running = false;

// Declare a pointer to (the first element of) an array of uint32 elements
// Equal to unit32[]
uint32_t *color_buffer = NULL;

// SDL variables
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *color_buffer_texture = NULL;

bool initialize_window(void) {

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL\n");
    return false;
  }

  // NOTE: Create a SDL window
  window =
      SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_BORDERLESS);
  if (!window) {
    fprintf(stderr, "Error creating SDL window\n");
    return false;
  }

  // NOTE: Create a SDL renderer
  // -1 and 0 are for enable the default setting
  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    fprintf(stderr, "Error creating SDL renderer\n");
    return false;
  }

  return true;
}

void setup(void) {
  // Allocate the required memory to hold the color buffer
  int bytes = sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT;
  color_buffer = (uint32_t *)malloc(bytes);

  // Create color buffer texture
  color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           WINDOW_WIDTH, WINDOW_HEIGHT);
}

void process_input() {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
  case SDL_QUIT:
    is_running = false;
    break;
  case SDL_KEYDOWN:
    if (event.key.keysym.sym == SDLK_ESCAPE)
      is_running = false;
    break;
  }
}

void update() {}

void render_color_buffer(void) {
  // Number of bytes in row of pixel data
  int pitch = WINDOW_WIDTH * sizeof(uint32_t);

  // Copy pixel(colors) data from color_buffer to color_buffer_texture
  SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, pitch);
  SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void clear_color_buffer(uint32_t color) {
  // Convert from 2D array postions to 1D array postions
  for (int y = 0; y < WINDOW_HEIGHT; y++) {
    for (int x = 0; x < WINDOW_WIDTH; x++) {
      int position = (WINDOW_WIDTH * y) + x;
      color_buffer[position] = color;
    }
  }
}

void render() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  render_color_buffer();
  clear_color_buffer(0xFFFF00);

  SDL_RenderPresent(renderer);
}

void destroy_window(void) {
  free(color_buffer);
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
}

int main(void) {
  printf("Hello world!\n \a");
  is_running = initialize_window();

  setup();
  while (is_running) {
    process_input();
    update();
    render();
  };

  return 0;
}
