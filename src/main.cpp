#include <stdio.h>
#include "libmodule.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string>

#include <chrono>

#include <unistd.h>
#include <memory>

#include "CApp.h"


int main(int argc, char *argv[]) {

  auto pApp = std::make_shared<CApp>();
  
  pApp->Start();
  while(pApp->Get_bLoop() == true) 
  {
    usleep(100000);
  }
  pApp->Stop();




//  SDL_Window* pWindow = NULL;
//  SDL_Renderer* pRenderer = NULL;
//
//  TTF_Font *pFont = NULL;
//  SDL_Texture* pImgTexture;
//  SDL_Texture* pTxtTexture;
//
//
//  if ( Init_SDL_ctx(pWindow,pRenderer) )
//    return -1;
//
//  if ( Init_Fonts(pFont) ) {
//    return -1;
//  }
//
//  //pImgTexture = IMG_LoadTexture(pRenderer, "resource/pics/gui_common_ex00.png");
//  pImgTexture = IMG_LoadTexture(pRenderer, "resource/pics/spaceship_00.png");
//  //pImgTexture = IMG_LoadTexture(pRenderer, "resource/pics/enemy_00.png");
//  if (!pImgTexture)
//    printf("\033[1;33m[%s][%d] :x: error %s \033[m\n",__FUNCTION__,__LINE__,SDL_GetError());
//
//  SDL_Rect posTexture00; 
//  SDL_Rect posTexture01; 
//  int iWidth,iHeight;
//  SDL_QueryTexture(pImgTexture, NULL, NULL, &iWidth, &iHeight);
//  // Keep Position Center
//  posTexture00.x = SCREEN_WIDTH/2  - (iWidth/2); 
//  posTexture00.y = SCREEN_HEIGHT/2 - (iHeight/2); 
//  posTexture00.w = iWidth; 
//  posTexture00.h = iHeight;
//
//  posTexture01.x = 30; 
//  posTexture01.y = 30; 
//  posTexture01.w = 160; 
//  posTexture01.h = 160; 
//
//  SDL_Rect Txt_Pos={10,10,0,0}; 
//
//
//
//  // Draw Text
//  if (
//      DrawText(pTxtTexture,pFont,
//        std::string("Q : exit, a/s : rotate the picture"),pRenderer)
//     ) {
//    printf("\033[1;31m[%s][%d] :x: Draw Text error \033[m\n",
//        __FUNCTION__,__LINE__);
//    return -1;
//  }
//  SDL_QueryTexture(pTxtTexture, NULL, NULL, &Txt_Pos.w, &Txt_Pos.h);
//  SDL_DestroyTexture(pTxtTexture);
//
//  SDL_Event Evt;
//  bool bQuit = false;
//  double dbAngle =0.0f;
//  double dbActualFPS=0.0f;
//
//  while (!bQuit) {
//
//    if (bQuit ==true) 
//      break;
//    // clear screen
//    SDL_SetRenderDrawColor(pRenderer, 55, 55, 55, SDL_ALPHA_OPAQUE);
//    SDL_RenderClear(pRenderer);
//    // draw texture
//    SDL_RenderCopyEx(pRenderer, pImgTexture, NULL, &posTexture00,dbAngle, NULL, SDL_FLIP_NONE );
//    SDL_RenderCopyEx(pRenderer, pImgTexture, NULL, &posTexture01,dbAngle, NULL, SDL_FLIP_NONE );
//
//    // draw lines (draw triangle in the center)
//    static SDL_Point points[POINTS_COUNT] = {
//      {320, 200},
//      {300, 240},
//      {340, 240},
//      {320, 200}
//    };
//    SDL_SetRenderDrawColor(pRenderer, 255, 55, 55, SDL_ALPHA_OPAQUE);
//    SDL_RenderDrawLines(pRenderer, points, POINTS_COUNT);
//    SDL_SetRenderDrawColor(pRenderer, 55, 55, 55, SDL_ALPHA_OPAQUE);
//    // draw text
//    DrawText(pTxtTexture,pFont,
//        std::string("q : exit, a/s : rotate the picture , FPS =") +std::to_string((int)(dbActualFPS)),pRenderer);
//    SDL_RenderCopy(pRenderer, pTxtTexture, NULL, &Txt_Pos );
//    SDL_DestroyTexture(pTxtTexture);
//    // render
//    SDL_RenderPresent(pRenderer);
//
//    // evt handling
//    while( SDL_PollEvent( &Evt ) != 0 )
//    {
//      //User requests quit
//      if( Evt.type == SDL_QUIT )
//      {
//        bQuit = true;
//      }
//      else if( Evt.type == SDL_KEYDOWN )
//      {
//        switch( Evt.key.keysym.sym )
//        {
//          case SDLK_q:
//            bQuit = true;
//            break;
//
//          case SDLK_a:
//            dbAngle-=1;
//            break;
//          case SDLK_d:
//            dbAngle+=1;
//            break;
//          case SDLK_w:
//            break;
//          case SDLK_s:
//            break;
//        }
//      }
//
//    }
//    // FPS control
//    dbActualFPS = Frame_Rate_Control(SCREEN_FPS);
//  }
//
//  SDL_Delay( 100 );
//
//  SDL_DestroyTexture(pTxtTexture);
//  SDL_DestroyTexture(pImgTexture);
//  DeInit_Fonts(pFont);
//  DeInit_SDL_ctx(pWindow, pRenderer);

  return 0;
}
