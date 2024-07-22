#ifndef _SDL_CTX_H_
#define _SDL_CTX_H_ 
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <string>

#include <chrono>

#include <unistd.h>

#define SCREEN_POS_X (0)
#define SCREEN_POS_Y (0)

#define SCREEN_WIDTH  (640)
#define SCREEN_HEIGHT (480)

#define SCREEN_FPS (50)
#define POINTS_COUNT (4)

int Init_SDL_ctx(SDL_Window* &pWindow, SDL_Renderer* &pRenderer);
int DeInit_SDL_ctx(SDL_Window* &pWindow, SDL_Renderer* &pRenderer); 

int Init_Fonts(TTF_Font* &pFont) ;
int DeInit_Fonts(TTF_Font* &pFont) ;

int DrawText(SDL_Texture* &pTxtTexture,TTF_Font* &pFont,
    std::string strText,SDL_Renderer* &pRenderer) ;

double Frame_Rate_Control(double dbFPS);

#endif /* ifndef _SDL_CTX_H_ */
