#include "mesh.h"
#include "darray.h"
#include "triangle.h"
#include "vector.h"
#include <stdio.h>
#include <string.h>

mesh_t mesh = {
    .vertices = NULL,
    .faces = NULL,
    .rotation = {0, 0, 0},
};

vec3_t cube_vertices[N_CUBE_VERTICES] = {
    {.x = -1, .y = -1, .z = -1}, // 1
    {.x = -1, .y = 1, .z = -1},  // 2
    {.x = 1, .y = 1, .z = -1},   // 3
    {.x = 1, .y = -1, .z = -1},  // 4
    {.x = 1, .y = 1, .z = 1},    // 5
    {.x = 1, .y = -1, .z = 1},   // 6
    {.x = -1, .y = 1, .z = 1},   // 7
    {.x = -1, .y = -1, .z = 1}   // 8
};

face_t cube_faces[N_CUBE_FACES] = {
    // front
    {.a = 1, .b = 2, .c = 3},
    {.a = 1, .b = 3, .c = 4},
    // right
    {.a = 4, .b = 3, .c = 5},
    {.a = 4, .b = 5, .c = 6},
    // back
    {.a = 6, .b = 5, .c = 7},
    {.a = 6, .b = 7, .c = 8},
    // left
    {.a = 8, .b = 7, .c = 2},
    {.a = 8, .b = 2, .c = 1},
    // top
    {.a = 2, .b = 7, .c = 5},
    {.a = 2, .b = 5, .c = 3},
    // bottom
    {.a = 6, .b = 8, .c = 1},
    {.a = 6, .b = 1, .c = 4} //
};

void load_cube_mesh(void) {
  for (int i = 0; i < N_CUBE_VERTICES; i++) {
    vec3_t cube_vertex = cube_vertices[i];
    darray_push(mesh.vertices, cube_vertex);
  }
  for (int i = 0; i < N_CUBE_FACES; i++) {
    face_t cube_face = cube_faces[i];
    darray_push(mesh.faces, cube_face);
  }
}

void load_obj_file(char *file_name) {
  FILE *file_ptr;
  int string_length = 512;

  file_ptr = fopen(file_name, "r");
  char line[string_length];

  // read every line in .obj file
  while (fgets(line, string_length, file_ptr) != NULL) {

    // strncmp is compare string at position (n=2 is 2 first characters)
    if (strncmp(line, "v ", 2) == 0) {
      // Vertex information
      vec3_t vertex;
      sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
      darray_push(mesh.vertices, vertex);
    };

    // Getting faces information
    if (strncmp(line, "f ", 2) == 0) {
      int vertex_indeces[3];
      long texture_indeces[3];
      int normal_indeces[3];

      // Format of the file vertex/texture/normal index
      // clang-format off
      sscanf(line, "f %d/%ld/%d %d/%ld/%d %d/%ld/%d", 
             &vertex_indeces[0], &texture_indeces[0], &normal_indeces[0], 
             &vertex_indeces[1], &texture_indeces[1], &normal_indeces[1], 
             &vertex_indeces[2], &texture_indeces[2], &normal_indeces[2]
      );
      //clang-format on
      face_t face = {
        .a = vertex_indeces[0],
        .b = vertex_indeces[1],
        .c = vertex_indeces[2],
      };

      darray_push(mesh.faces, face);
    };
  };

  fclose(file_ptr);
  return;
}
