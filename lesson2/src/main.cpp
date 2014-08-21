#include <iostream>
#include <string>

#include <SDL2/SDL.h>

#include "res_path.h"
#include "cleanup.h"

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;

using namespace std;

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
SDL_Texture *loadTexture( const string &file, SDL_Renderer *ren ) {
    // Initialize to nullptr to avoid dangling pointer issues
    SDL_Texture *texture = nullptr;

    // Load the image
    SDL_Surface *loadedImage = SDL_LoadBMP( file.c_str() );

    if ( loadedImage == nullptr ) {
        logSDLError( cout, "SDL_LoadBMP" );
        return texture;
    }

    texture = SDL_CreateTextureFromSurface( ren, loadedImage );
    SDL_FreeSurface( loadedImage );

    if ( texture == nullptr ) {
        logSDLError( cout, "SDL_CreateTextureFromSurface" );
        return texture;
    }

    return texture;
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
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;

    SDL_QueryTexture( tex, NULL, NULL, &dst.w, &dst.h );
    SDL_RenderCopy( ren, tex, NULL, &dst );
}

int main() {
    if ( SDL_Init(SDL_INIT_EVERYTHING ) != 0 ) {
        logSDLError( cout, "SDL_Init" );
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow( "Hello World!", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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

    const string resPath = getResourcePath( "lesson2" );
    SDL_Texture *background = loadTexture( resPath + "background.bmp", ren );
    SDL_Texture *image = loadTexture( resPath + "image.bmp", ren );
    if ( background == nullptr || image == nullptr ) {
        cleanup( background, image, ren, win );
        SDL_Quit();
        return 1;
    }

    SDL_RenderClear( ren );
    int bW, bH;
    SDL_QueryTexture( background, NULL, NULL, &bW, &bH );
    for ( int y = 0; y < SCREEN_HEIGHT / bH; y++ ) {
        for ( int x = 0; x < SCREEN_WIDTH; x++ ) {
            renderTexture( background, ren, bW * x, bH * y );
        }
    }

    int iW, iH;
    SDL_QueryTexture( image, NULL, NULL, &iW, &iH );
    int x = SCREEN_WIDTH / 2 - iW / 2;
    int y = SCREEN_HEIGHT / 2 - iH / 2;
    renderTexture( image, ren, x, y );

    SDL_RenderPresent( ren );
    SDL_Delay( 2000 );

    cleanup( background, image, ren, win );
    SDL_Quit();
    return 0;
}

