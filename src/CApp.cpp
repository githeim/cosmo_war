#include "entt/entt.hpp"
#include "CApp.h"
#include "SDL_ctx.h"

void test() {
  return;
}


CApp::CApp() {
  m_pECS_registry = std::make_shared<entt::registry>();
}
CApp::~CApp() {

}

/**
 * @brief Create map of textures
 *
 * @param pECS[OUT]       ECS registry
 * @param TextureMap[OUT] texture map entity
 */
void CApp::Create_Textures(const std::shared_ptr<entt::registry>& pECS,
    entt::entity& TextureMap) {

  TextureMap =  pECS->create();
  TextureMap_t Map;
  Map.mapTextures["spaceship"] = 
    IMG_LoadTexture(m_pRenderer, "resource/pics/spaceship_00.png");
  m_mapTextures["spaceship"] =Map.mapTextures["spaceship"];
  Map.mapTextures["enemy00"] = 
    IMG_LoadTexture(m_pRenderer, "resource/pics/enemy_00.png");
  m_mapTextures["enemy00"] =Map.mapTextures["enemy00"];
  Map.mapTextures["bullet00"] = 
    IMG_LoadTexture(m_pRenderer, "resource/pics/bullet_00.png");
  m_mapTextures["bullet00"] =Map.mapTextures["bullet00"];
  Map.mapTextures["burst00"] = 
    IMG_LoadTexture(m_pRenderer, "resource/pics/burst_00.png");
  m_mapTextures["burst00"] =Map.mapTextures["burst00"];

  SDL_Texture* pTxtTexture;

  DrawText(pTxtTexture,m_mapFonts["Text_LARGE"],"COSMO WAR",m_pRenderer);
  Map.mapTextures["TitleText"] = pTxtTexture;
  m_mapTextures["TitleText"] = pTxtTexture;

  DrawText(pTxtTexture,m_mapFonts["Text_MIDDLE"],"Press Enter Key",m_pRenderer);
  Map.mapTextures["Press_Enter_Key"] = pTxtTexture;
  m_mapTextures["Press_Enter_Key"] = pTxtTexture;

  DrawText(pTxtTexture,m_mapFonts["Text_SMALL"],"windheim 2024.07.02",m_pRenderer);
  Map.mapTextures["credit"] = pTxtTexture;
  m_mapTextures["credit"] = pTxtTexture;

  DrawText(pTxtTexture,m_mapFonts["Text_LARGE"],"You Win",m_pRenderer);
  Map.mapTextures["You_Win"] = pTxtTexture;
  m_mapTextures  ["You_Win"] = pTxtTexture;

  DrawText(pTxtTexture,m_mapFonts["Text_LARGE"],"Defeated",m_pRenderer);
  Map.mapTextures["Defeated"] = pTxtTexture;
  m_mapTextures  ["Defeated"] = pTxtTexture;


  pECS->emplace<TextureMap_t>(TextureMap, Map);

}
void CApp::Destroy_Textures(std::unordered_map<std::string,SDL_Texture *>& mapTextures) {
  for (const auto &pTexture : mapTextures) {
    SDL_DestroyTexture(pTexture.second);
  }
}

/**
 * @brief create object lifecycle controller
 *        ObjLifecyle controller contains the obj entities to create & delete
 * @param pECS[OUT]
 * @param ObjLifeCycleControl[OUT] entity of the lifecycle controller
 * @param TextureMap[IN] texture map
 */
void CApp::Create_ObjLifecycleEntity(
             const std::shared_ptr<entt::registry>& pECS,
             entt::entity& ObjLifeCycleControl,entt::entity& TextureMap
    ) {
  ObjLifeCycleControl =  pECS->create();
  ObjLifecyle_t ObjLifecyle ={};
  ObjLifecyle.TextureMap = TextureMap;
  pECS->emplace<ObjLifecyle_t>(ObjLifeCycleControl, ObjLifecyle);
}

void CApp::Update_ObjLifecycle(const std::shared_ptr<entt::registry>& pECS,
      entt::entity& ObjLifeCycleControl) {
  auto &LifeCycle = pECS->get<ObjLifecyle_t>(ObjLifeCycleControl);
  // Create Entities
  for (auto Item  : LifeCycle.Create_List) {
    auto Obj = pECS->create();
    pECS->emplace<Sprite_t>(Obj,Item.Sprite);
    pECS->emplace<Pos_t>(Obj,Item.Pos);
    pECS->emplace<Attr_t>(Obj,Item.Attr);
    pECS->emplace<State_t>(Obj,Item.State);
  }

  LifeCycle.Create_List.clear();
  // Delete Entities
  for (auto Item : LifeCycle.Delete_List) {
    pECS->destroy(Item);
  }
  LifeCycle.Delete_List.clear();

}

void CApp::Start() {
  Set_bLoop(true);
  m_pThrLoop = std::make_shared<std::thread>(&CApp::MainLoop,this);
}

void CApp::Stop() {
  Set_bLoop(false);
  if (m_pThrLoop != nullptr)
  {
    m_pThrLoop->join();
    m_pThrLoop = nullptr;
    DeInit_SDL_ctx(m_pWindow, m_pRenderer);
  }
}

int CApp::Create_SceneCtrl (const std::shared_ptr<entt::registry>& pECS,
                            entt::entity& SceneCtrlEntity)
{
  // Create Scene Controller
  SceneCtrl_t SceneCtrl;
  SceneCtrl.CurrentScene = SCENE_NONE;
  SceneCtrl.vecScene.push_back(SCENE_TITLE);
  SceneCtrl.vecPlugins.push_back( Plugin_SceneCtrl);
  SceneCtrlEntity = pECS->create();
  pECS->emplace<SceneCtrl_t>(SceneCtrlEntity, SceneCtrl);
  return 0;
}

int CApp::Create_Fonts (std::unordered_map<std::string, TTF_Font*>& mapFonts){
  TTF_Font* pFont;
  if( TTF_Init() == -1 ) {
    printf("\033[1;33m[%s][%d] :x: SDL_ttf could not initialize!"
        " SDL_ttf Error: %s \033[m\n",__FUNCTION__,__LINE__,TTF_GetError());
    return -1;
  }
	pFont = TTF_OpenFont( "resource/fonts/NanumGothicCoding-Regular.ttf", 96 );
	if( pFont == NULL )
	{
    printf("\033[1;31m[%s][%d] :x: Failed to load font [%s] \033[m\n",
        __FUNCTION__,__LINE__,TTF_GetError());
    return -1;
	}

  mapFonts["Text_LARGE"]  = pFont;

	pFont = TTF_OpenFont( "resource/fonts/NanumGothicCoding-Regular.ttf", 48 );
	if( pFont == NULL )
	{
    printf("\033[1;31m[%s][%d] :x: Failed to load font [%s] \033[m\n",
        __FUNCTION__,__LINE__,TTF_GetError());
    return -1;
	}

  m_mapFonts["Text_MIDDLE"] = pFont;
	pFont = TTF_OpenFont( "resource/fonts/NanumGothicCoding-Regular.ttf", 24 );
	if( pFont == NULL )
	{
    printf("\033[1;31m[%s][%d] :x: Failed to load font [%s] \033[m\n",
        __FUNCTION__,__LINE__,TTF_GetError());
    return -1;
	}
  m_mapFonts["Text_SMALL"]  = pFont;
  return 0;

}
void CApp::Destroy_Fonts(std::unordered_map<std::string, TTF_Font*>& mapFonts){
  for (const auto &Font : mapFonts) {
	  TTF_CloseFont(Font.second);
  }
}


void CApp::MainLoop() {
  Init_SDL_ctx(m_pWindow, m_pRenderer);


  if (m_pECS_registry == nullptr) 
    return;
  Create_Fonts(m_mapFonts);
  Create_Textures(m_pECS_registry,m_TextureMapEntity);
  Create_SceneCtrl(m_pECS_registry,m_SceneCtrlEntity);
  Set_SceneCtrl_Entity(m_SceneCtrlEntity);

  Create_ObjLifecycleEntity(m_pECS_registry,m_ObjLifecycleEntity,m_TextureMapEntity);

  SDL_SetRenderDrawColor(m_pRenderer, 55, 55, 55, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(m_pRenderer);

  SDL_RenderPresent(m_pRenderer);
  double dbActualFPS=0.f;
  double dbActual_Frame_diff_SEC =1.f /SCREEN_FPS;
  (void)dbActualFPS;

  while (Get_bLoop()) {
    SDL_RenderClear(m_pRenderer);
    // basic key evt handling
    SDL_Event Evt;
    while( SDL_PollEvent( &Evt ) != 0 ){ 
      //User requests quit               
      if( Evt.type == SDL_QUIT )
      {
        Set_bLoop(false);
      }
      else if( Evt.type == SDL_KEYDOWN )
      {
        switch( Evt.key.keysym.sym )
        {
          case SDLK_q:
            Set_bLoop(false);
            break;
        }
      }
    }

    // Update Systems
    // Obj LifeCycle
    Update_ObjLifecycle(m_pECS_registry,m_ObjLifecycleEntity);
    // Sprite
    m_Sys_Sprite.Update(*m_pECS_registry);
    //SDL_RenderClear(m_pRenderer);
    m_Sys_Sprite.Render(*m_pECS_registry, m_pRenderer);
    // Input
    m_Sys_Input.Update(*m_pECS_registry,dbActual_Frame_diff_SEC,m_ObjLifecycleEntity);
    // Scene Controller
    m_Sys_SceneCtrl.Update(*m_pECS_registry,dbActual_Frame_diff_SEC,
                           m_SceneCtrlEntity,m_ObjLifecycleEntity);

    // Obj plugins
    m_Sys_ObjPlugins.Update(*m_pECS_registry,dbActual_Frame_diff_SEC, 
                             m_ObjLifecycleEntity);

    // Logic
    m_Sys_Logic.Update(*m_pECS_registry,dbActual_Frame_diff_SEC,
                        m_ObjLifecycleEntity);


    SDL_RenderPresent(m_pRenderer);

    // Frame rate control
    dbActualFPS = Frame_Rate_Control(SCREEN_FPS);
    dbActual_Frame_diff_SEC = 1/ dbActualFPS;
  }
  Destroy_Fonts(m_mapFonts);
  Destroy_Textures(m_mapTextures);

}
