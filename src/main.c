#include "darray.h"
#include "display.h"
#include "mesh.h"
#include "triangle.h"
#include "vector.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_timer.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// Array of triangles that should be rendered
triangle_t *triangles_to_render = NULL;

// constants
int BLACK_COLOR = 0x000000;
int previous_frame_time = 0;

bool is_running = false;
float fov_factor = 600;

vec3_t camera_position = {0, 0, -5};

void setup(void) {
  // Allocate the required memory to hold the color buffer
  int bytes = sizeof(uint32_t) * window_width * window_height;
  color_buffer = (uint32_t *)malloc(bytes);

  // Create color buffer texture
  color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           window_width, window_height);
  // load resources
  // load_cube_mesh();
  load_obj_file("assets/cube.obj");
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

  // initialize the array of triangles to render
  triangles_to_render = NULL;

  mesh.rotation.x += 0.05;
  mesh.rotation.y += 0.00;
  mesh.rotation.z += 0.00;

  // Loop all triangle faces of mesh
  int number_of_faces = darray_size(mesh.faces);
  for (int i = 0; i < number_of_faces; i++) {
    face_t mesh_face = mesh.faces[i];

    int triangle_points = 3;
    vec3_t face_vertices[triangle_points];

    // Minus 1 because index start at 0
    face_vertices[0] = mesh.vertices[mesh_face.a - 1];
    face_vertices[1] = mesh.vertices[mesh_face.b - 1];
    face_vertices[2] = mesh.vertices[mesh_face.c - 1];

    // Loop all three vertices of this current face and apply transformations
    triangle_t projected_triangle;
    for (int j = 0; j < triangle_points; j++) {
      vec3_t transformed_vertex = face_vertices[j];
      transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
      transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
      transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

      // Translate camera position
      transformed_vertex.z += camera_position.z;

      // Project the current vertex (turn 3D into 2D for render)
      vec2_t projected_point = project(transformed_vertex);

      // Scale and translate the projected points
      projected_point.x += (float)window_width / 2;
      projected_point.y += (float)window_height / 2;
      projected_triangle.points[j] = projected_point;
    };

    // Save the projected triangle to render
    darray_push(triangles_to_render, projected_triangle);
  };
}

void render() {
  int number_of_triangle = darray_size(triangles_to_render);
  for (int i = 0; i < number_of_triangle; i++) {
    triangle_t projected_point = triangles_to_render[i];

    // Draw vertices points
    // draw_rect(projected_point.points[0].x, projected_point.points[0].y, 5, 5,
    //           0x000000);
    // draw_rect(projected_point.points[1].x, projected_point.points[1].y, 5, 5,
    //           0x000000);
    // draw_rect(projected_point.points[2].x, projected_point.points[2].y, 5, 5,
    //           0x000000);

    // Draw triangle line
    draw_triangle_w_Triagnle(projected_point, BLACK_COLOR);
  };

  // Clear the array of triangles to render every frame
  darray_free(triangles_to_render);

  render_color_buffer();
  clear_color_buffer(0x478CBF);

  SDL_RenderPresent(renderer);
}

void free_resources(void) {
  free(color_buffer);
  darray_free(mesh.vertices);
  darray_free(mesh.faces);
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

  free_resources();
  destroy_window();

  return 0;
}
