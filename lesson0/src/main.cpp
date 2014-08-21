#include <iostream>
#include <string>

#include <SDL2/SDL.h>

#include "res_path.h"

int main() {
  if ( SDL_Init(SDL_INIT_EVERYTHING ) != 0 ) {
    std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return 1;
  }

  std::cout << "Resource path is: " << getResourcePath() << std::endl;

  SDL_Quit();
  return 0;
}

