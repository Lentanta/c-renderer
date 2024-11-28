#include "display.h"
#include "vector.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// Declare an array of vectors/points
#define N_POINTS (9 * 9 * 9)
vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];

// constants
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

bool is_running = false;
float fov_factor = 600;
vec3_t camera_position = {0, 0, -5};

void setup(void) {
  // Allocate the required memory to hold the color buffer
  int bytes = sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT;
  color_buffer = (uint32_t *)malloc(bytes);

  // Create color buffer texture
  color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           window_width, window_height);

  // Create array of vectors
  // From -1 to 1 (in 9*9*9 cube)
  int points_count = 0;
  for (float x = -1; x <= 1; x += 0.25) {
    for (float y = -1; y <= 1; y += 0.25) {
      for (float z = -1; z <= 1; z += 0.25) {
        vec3_t point = {.x = x, .y = y, .z = z};
        cube_points[points_count] = point;
        points_count++;
      }
    }
  }
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
    if (event.key.keysym.sym == SDLK_RIGHT) {
      fov_factor = fov_factor + 1;
    }
    break;
  }
}

// ----- RENDER CODE ----- //

vec2_t project(vec3_t point) {
  vec2_t projected_point = {.x = (point.x * fov_factor) / point.z,
                            .y = (point.y * fov_factor) / point.z};
  return projected_point;
}

void update() {
  for (int i = 0; i < N_POINTS; i++) {
    vec3_t point = cube_points[i];

    // Move away from camera
    point.z -= camera_position.z;

    // 2D projected point
    vec2_t projected_point = project(point);

    // Save the projected 2D vector in the array of projected points
    projected_points[i] = projected_point;
  }
}

void render() {
  // -- No need to draw color and clear because we using color buffer
  // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  // SDL_RenderClear(renderer);

  for (int i = 0; i < N_POINTS; i++) {
    vec2_t projected_point = projected_points[i];
    draw_rect(projected_point.x + (window_width / 2),
              projected_point.y + (window_height / 2), 5, 5, 0x000000);
  }

  render_color_buffer();
  clear_color_buffer(0x478CBF);

  SDL_RenderPresent(renderer);
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
