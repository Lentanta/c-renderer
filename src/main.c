#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

bool is_running = false;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

bool initialize_window(void) {

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL\n");
    return false;
  }

  // NOTE: Create a SDL window
  window =
      SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       800, 600, SDL_WINDOW_BORDERLESS);
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

void setup() {}

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

void render() {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  // TODO: code render

  SDL_RenderPresent(renderer);
}

int main(void) {
  printf("Hello world!\n");
  is_running = initialize_window();

  setup();
  while (is_running) {
    process_input();
    update();
    render();
  }

  return 0;
}
