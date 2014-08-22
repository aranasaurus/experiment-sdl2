#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_Image.h>

#include "res_path.h"
#include "cleanup.h"

using namespace std;

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;

/**
 * Log an SDL error with some error message to the output stream of our choice
 * @param os The output stream to write the message to
 * @param msg The error message to write, format will be msg error: SDL_GetError()
 */
void logSDLError( ostream &os, const string &msg ) {
    os << msg << " error: " << SDL_GetError() << endl;
}

/**
* Loads a BMP image into a texture on the rendering device
* @param file The BMP image file to load
* @param ren The renderer to load the texture onto
* @return the loaded texture, or nullptr if something went wrong.
*/
SDL_Texture* loadTexture( const string &file, SDL_Renderer *ren ) {
    // Initialize to nullptr to avoid dangling pointer issues
    SDL_Texture *texture = IMG_LoadTexture( ren, file.c_str() );

    if ( texture == nullptr ) {
        logSDLError( cout, "IMG_LoadTexture" );
    }

    return texture;
}

/**
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, with some desired
 * width and height
 * @param tex The source texture we want to draw
 * @param ren The renderer we want to draw to
 * @param x The x coordinate to draw to
 * @param y The y coordinate to draw to
 * @param w The width of the texture to draw
 * @param h The height of the texture to draw
 */
void renderTexture( SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h ) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;

    SDL_RenderCopy( ren, tex, NULL, &dst );
}

/**
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
 * the texture's width and height.
 * @param tex The source texture we want to draw
 * @param ren The renderer we want to draw to
 * @param x The x coordinate to draw to
 * @param y The y coordinate to draw to
 */
void renderTexture( SDL_Texture *tex, SDL_Renderer *ren, int x, int y ) {
    int w, h;
    SDL_QueryTexture( tex, NULL, NULL, &w, &h );
    renderTexture( tex, ren, x, y, w, h );
}

int main() {
    if ( SDL_Init(SDL_INIT_EVERYTHING ) != 0 ) {
        logSDLError( cout, "SDL_Init" );
        return 1;
    }

    if ( ( IMG_Init( IMG_INIT_PNG ) & IMG_INIT_PNG ) != IMG_INIT_PNG ) {
        logSDLError( cout, "IMG_Init" );
        SDL_Quit();
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow( "Lesson 3", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if ( win == nullptr ) {
        logSDLError( cout, "SDL_CreateWindow" );
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *ren = SDL_CreateRenderer( win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
    if ( ren == nullptr ) {
        cleanup( win );
        logSDLError( cout, "SDL_CreateRenderer" );
        SDL_Quit();
        return 1;
    }

    string filePath = getResourcePath( "lesson4/image.png" );
    SDL_Texture *image = loadTexture( filePath, ren );
    if ( image == nullptr ) {
        cleanup( win, ren );
        logSDLError( cout, "loadTexture" );
        SDL_Quit();
        return 1;
    }

    int x, y, w, h, v = 2;
    SDL_QueryTexture( image, NULL, NULL, &w, &h );
    x = SCREEN_WIDTH/2 - w/2;
    y = SCREEN_HEIGHT/2 - h/2;

    bool quit = false;
    SDL_Event e;
    while ( !quit ) {
        while ( SDL_PollEvent( &e ) ) {
            quit = ( e.type == SDL_QUIT || e.type == SDL_MOUSEBUTTONDOWN );

            if ( e.type == SDL_KEYDOWN ) {
                switch ( e.key.keysym.sym ) {
                    case SDLK_UP:
                    case SDLK_e:
                    case SDLK_k:
                        y -= v;
                        break;
                    case SDLK_DOWN:
                    case SDLK_d:
                    case SDLK_j:
                        y += v;
                        break;
                    case SDLK_LEFT:
                    case SDLK_s:
                    case SDLK_h:
                        x -= v;
                        break;
                    case SDLK_RIGHT:
                    case SDLK_f:
                    case SDLK_l:
                        x += v;
                        break;
                    default:
                        quit = true;
                        break;
                }
            }
        }

        SDL_RenderClear( ren );
        renderTexture( image, ren, x, y );
        SDL_RenderPresent( ren );
    }

    cleanup( image, ren, win );
    SDL_Quit();
    return 0;
}

