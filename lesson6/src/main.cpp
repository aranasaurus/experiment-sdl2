#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_TTF.h>

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

SDL_Texture* renderText( const string &message, TTF_Font *font, SDL_Color color, SDL_Renderer *renderer ) {
    SDL_Surface *surf = TTF_RenderText_Blended( font, message.c_str(), color);
    if ( surf == nullptr ) {
        TTF_CloseFont( font );
        logSDLError( cout, "TTF_RenderText_Blended" );
        return nullptr;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface( renderer, surf );
    if ( tex == nullptr ) {
        logSDLError( cout, "SDL_CreateTexture" );
    }

    SDL_FreeSurface( surf );
    return tex;
}

int main() {
    if ( SDL_Init(SDL_INIT_EVERYTHING ) != 0 ) {
        logSDLError( cout, "SDL_Init" );
        return 1;
    }

    if ( TTF_Init() != 0 ) {
        logSDLError( cout, "TTF_Init" );
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

    string filePath = getResourcePath( "lesson6") + "sample.ttf";
    TTF_Font *font = TTF_OpenFont( filePath.c_str(), 64 );
    if ( font == nullptr ) {
        logSDLError( cout, "TTF_OpenFont" );
        cleanup( win, ren );
        SDL_Quit();
        return 1;
    }

    SDL_Color color = { 255, 255, 255, 255 };
    SDL_Texture *image = renderText( "TTF fonts are cool!", font, color, ren );
    if ( image == nullptr ) {
        cleanup( ren, win );
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Rect dst;
    SDL_QueryTexture( image, NULL, NULL, &dst.w, &dst.h );
    dst.x = SCREEN_WIDTH/2 - dst.w/2;
    dst.y = SCREEN_HEIGHT/2 - dst.h/2;

    bool quit = false;
    SDL_Event e;
    while ( !quit ) {
        while ( SDL_PollEvent( &e ) ) {
            quit = ( e.type == SDL_QUIT || e.type == SDL_MOUSEBUTTONDOWN );

            if ( e.type == SDL_KEYDOWN ) {
                switch ( e.key.keysym.sym ) {
                    default:
                        quit = true;
                        break;
                }
            }
        }

        SDL_RenderClear( ren );

        SDL_RenderCopy( ren, image, NULL, &dst );

        SDL_RenderPresent( ren );
    }

    cleanup( ren, win );
    SDL_Quit();
    return 0;
}

