#include "display.h"
#include "vector.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_timer.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// constants
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;
int previous_frame_time = 0;

bool is_running = false;
float fov_factor = 600;
vec3_t camera_position = {0, 0, -5};
vec3_t cube_rotation = {0, 0, 0};

void setup(void) {
  // Allocate the required memory to hold the color buffer
  int bytes = sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT;
  color_buffer = (uint32_t *)malloc(bytes);

  // Create color buffer texture
  color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           window_width, window_height);
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
    // if (event.key.keysym.sym == SDLK_RIGHT) {
    //   camera_position.x += 0.1;
    // }
    // if (event.key.keysym.sym == SDLK_LEFT) {
    //   camera_position.x -= 0.1;
    // }
    // if (event.key.keysym.sym == SDLK_UP) {
    //   camera_position.z += 0.1;
    // }
    // if (event.key.keysym.sym == SDLK_DOWN) {
    //   camera_position.z -= 0.1;
    // }
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
  // --- Lock begin: until the time passed (Locked fixed time step) --- //
  int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
  if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
    SDL_Delay(time_to_wait);
  };
  previous_frame_time = SDL_GetTicks();
  // --- Lock end ---//

  cube_rotation.x += 0.01;
  cube_rotation.y += 0.01;
  cube_rotation.z += 0.01;

  // for (int i = 0; i < N_POINTS; i++) {
  //   vec3_t point = cube_points[i];
  //
  //   // Rotate the cube by x,y and z
  //   vec3_t transformed_point = vec3_rotate_y(point, cube_rotation.y);
  //   transformed_point = vec3_rotate_x(transformed_point, cube_rotation.x);
  //   transformed_point = vec3_rotate_z(transformed_point, cube_rotation.z);
  //
  //   // Translate the point away from camera
  //   transformed_point.z += camera_position.z;
  //   transformed_point.x += camera_position.x;
  //   transformed_point.y += camera_position.y;
  //
  //   // 2D projected point
  //   vec2_t projected_point = project(transformed_point);
  //
  //   // Save the projected 2D vector in the array of projected points
  //   projected_points[i] = projected_point;
  // }
}

void render() {
  // -- No need to draw color and clear because we using color buffer
  // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  // SDL_RenderClear(renderer);

  // for (int i = 0; i < N_POINTS; i++) {
  //   vec2_t projected_point = projected_points[i];
  //   draw_rect(projected_point.x + (window_width / 2),
  //             projected_point.y + (window_height / 2), 5, 5, 0x000000);
  // }

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
