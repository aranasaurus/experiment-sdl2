#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_Image.h>

#include "res_path.h"
#include "cleanup.h"

using namespace std;

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;
const int SPRITE_W = 100;
const int SPRITE_H = 100;
const int SPRITE_ROWS = 2;
const int SPRITE_COLS = 2;

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
* Draw an SDL_Texture to an SDL_Renderer at some destination rect
* taking a clip of the texture if desired
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param dst The destination rectangle to render the texture to
* @param clip The sub-section of the texture to draw (clipping rect)
*		default of nullptr draws the entire texture
*/
void renderTexture( SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip = nullptr ) {
    SDL_RenderCopy( ren, tex, clip, &dst );
}

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height and taking a clip of the texture if desired
* If a clip is passed, the clip's width and height will be used instead of
*	the texture's
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
* @param clip The sub-section of the texture to draw (clipping rect)
*		default of nullptr draws the entire texture
*/
void renderTexture( SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip = nullptr ) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    if ( clip != nullptr ) {
        dst.w = clip->w;
        dst.h = clip->h;
    } else {
        SDL_QueryTexture( tex, NULL, NULL, &dst.w, &dst.h );
    }
    renderTexture( tex, ren, dst, clip );
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

    string filePath = getResourcePath( "lesson5/image.png" );
    SDL_Texture *image = loadTexture( filePath, ren );
    if ( image == nullptr ) {
        cleanup( win, ren );
        logSDLError( cout, "loadTexture" );
        SDL_Quit();
        return 1;
    }

    int v = 10;
    SDL_Rect dest;
    SDL_QueryTexture( image, NULL, NULL, &dest.w, &dest.h );
    dest.x = SCREEN_WIDTH/2 - dest.w/2;
    dest.y = SCREEN_HEIGHT/2 - dest.h/2;

    // Set up clipping
    SDL_Rect clips[SPRITE_COLS * SPRITE_ROWS];
    for ( int i=0; i < SPRITE_COLS * SPRITE_ROWS; i++ ) {
        clips[i].x = i / SPRITE_ROWS * SPRITE_W;
        clips[i].y = i % SPRITE_COLS * SPRITE_H;
        clips[i].w = SPRITE_W;
        clips[i].h = SPRITE_H;
    }

    int clipIndex = 0;

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
                        dest.y -= v;
                        break;
                    case SDLK_DOWN:
                    case SDLK_d:
                    case SDLK_j:
                        dest.y += v;
                        break;
                    case SDLK_LEFT:
                    case SDLK_s:
                    case SDLK_h:
                        dest.x -= v;
                        break;
                    case SDLK_RIGHT:
                    case SDLK_f:
                    case SDLK_l:
                        dest.x += v;
                        break;
                    case SDLK_1:
                        clipIndex = 0;
                        break;
                    case SDLK_2:
                        clipIndex = 1;
                        break;
                    case SDLK_3:
                        clipIndex = 2;
                        break;
                    case SDLK_4:
                        clipIndex = 3;
                        break;
                    default:
                        quit = true;
                        break;
                }
            }
        }

        SDL_RenderClear( ren );
        renderTexture( image, ren, dest, &clips[clipIndex] );
        SDL_RenderPresent( ren );
    }

    cleanup( image, ren, win );
    SDL_Quit();
    return 0;
}

