#include "SDL_ctx.h"
int Init_SDL_ctx(SDL_Window* &pWindow, SDL_Renderer* &pRenderer) {
  pWindow = NULL;

  //Initialize SDL
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
  {
    printf("\033[1;31m[%s][%d] :x: Could not Init SDL2 [%s] \033[m\n",
        __FUNCTION__,__LINE__, SDL_GetError());
    return -1;
  }

  pWindow = SDL_CreateWindow( "COSMO WAR", 
      SCREEN_POS_X, SCREEN_POS_Y, 
      SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
  if( pWindow == NULL )
  {
    printf("\033[1;31m[%s][%d] :x: error %s \033[m\n",
        __FUNCTION__,__LINE__,SDL_GetError());
    return -1;
  }

	pRenderer = SDL_CreateRenderer( pWindow, -1, 
      SDL_RENDERER_ACCELERATED  );

  if (!pRenderer) {
    printf("\033[1;31m[%s][%d] :x: error %s \033[m\n",
        __FUNCTION__,__LINE__,SDL_GetError());
    return -1;
  }


  return 0;
}

int DeInit_SDL_ctx(SDL_Window* &pWindow, SDL_Renderer* &pRenderer) {
  SDL_RenderClear(pRenderer);
  SDL_DestroyRenderer(pRenderer);
  //Destroy window
  SDL_DestroyWindow( pWindow );
  pWindow = NULL;
  //Quit SDL subsystems
  SDL_Quit();

  return 0;
}

int Init_Fonts(TTF_Font* &pFont) {
  if( TTF_Init() == -1 ) {
    printf("\033[1;33m[%s][%d] :x: SDL_ttf could not initialize!"
        " SDL_ttf Error: %s \033[m\n",__FUNCTION__,__LINE__,TTF_GetError());
    return -1;
  }
	pFont = TTF_OpenFont( "resource/fonts/NanumGothicCoding-Regular.ttf", 28 );
	if( pFont == NULL )
	{
    printf("\033[1;31m[%s][%d] :x: Failed to load font [%s] \033[m\n",
        __FUNCTION__,__LINE__,TTF_GetError());
    return -1;
	}
  else {
    
  }
  return 0;
}
int DeInit_Fonts(TTF_Font* &pFont) {
	TTF_CloseFont( pFont );
  return 0;
}

/**
 * @brief Create Texture of Text. warning! pTxtTexture should be destroy after
 *        using it.(ex :  SDL_DestroyTexture(pTxtTexture);) it causes memory leak
 *
 * @param pTxtTexture[OUT]
 * @param pFont[IN] font instance to use
 * @param strText[IN] text to draw
 * @param pRenderer[IN] renderer to use
 *
 * @return 
 */
int DrawText(SDL_Texture* &pTxtTexture,TTF_Font* &pFont, 
    std::string strText,SDL_Renderer* &pRenderer) 
{
  SDL_Color textColor = { 255, 255, 0 };
	SDL_Surface* pTxtSurface = TTF_RenderText_Solid( pFont, strText.c_str(), 
      textColor );
  if( pTxtSurface == NULL )
	{
    printf("\033[1;31m[%s][%d] :x: SDL_ttf Error: %s \033[m\n",
        __FUNCTION__,__LINE__, TTF_GetError());
    return -1;
	}
  pTxtTexture = SDL_CreateTextureFromSurface(pRenderer, pTxtSurface);
  SDL_FreeSurface(pTxtSurface);
	
  return 0;
}


/**
 * @brief Control frame rates
 *
 * @param dbFPS[IN]
 *
 * @return  Actual FPS
 */
double Frame_Rate_Control(const double dbFPS){
  static auto Frame_start = std::chrono::system_clock::now();
  static auto Frame_end   = std::chrono::system_clock::now();
  std::chrono::duration<double> Frame_diff_SEC;
  std::chrono::duration<double> Actual_Frame_diff_SEC;

  Frame_end = std::chrono::system_clock::now();
  Frame_diff_SEC = Frame_end - Frame_start;
  // :x:  Reference frame interval
  double dbRef_SEC= 1.0f/dbFPS;

  if (Frame_diff_SEC.count() < dbRef_SEC) {
    usleep((dbRef_SEC - Frame_diff_SEC.count())*1000000);
  }

  Frame_start = std::chrono::system_clock::now();
  Actual_Frame_diff_SEC = Frame_start - Frame_end;
  return (1.0f/Actual_Frame_diff_SEC.count());
}

