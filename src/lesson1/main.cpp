#include <iostream>
#include <string>

#include <SDL2/SDL.h>

#include "res_path.h"
#include "cleanup.h"

int main( int argc, char **argv ) {
    if ( SDL_Init(SDL_INIT_EVERYTHING ) != 0 ) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    /*
    SDL_Window *win = SDL_CreateWindow( "Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN );
    if ( win == nullptr ) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    std::cout << "SDL Window created." << std::endl;

    SDL_Renderer *ren = SDL_CreateRenderer( win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
    if ( ren == nullptr ) {
        cleanup( win );
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    std::cout << "SDL Renderer created." << std::endl;

    std::cout << "Loading bitmap..." << std::endl;
    std::string imagePath = getResourcePath( "Lesson1" ) + "hello.bmp";
    SDL_Surface *bmp = SDL_LoadBMP( imagePath.c_str() );
    if ( bmp == nullptr ) {
        cleanup( win, ren );
        std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface( ren, bmp );
    cleanup( bmp );
    if ( tex == nullptr ) {
        cleanup( win, ren );
        std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_RenderClear( ren );
    SDL_RenderCopy( ren, tex, NULL, NULL );
    SDL_RenderPresent( ren );

    SDL_Delay(2000);

    cleanup( tex, ren, win );
    */

    std::cout << "Resource path is: " << getResourcePath() << std::endl;

    SDL_Quit();
    return 0;
}
