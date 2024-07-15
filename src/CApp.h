#ifndef _CAPP_H_
#define _CAPP_H_

#include "SDL_ctx.h"

#include <thread>
#include <unordered_map>
#include <entt/entt.hpp>
#include "App_Components.h"
#include "App_Systems.h"
class CApp {

public:
  CApp();
  ~CApp();
  void Create_ObjLifecycleEntity(
      const std::shared_ptr<entt::registry>& pECS,
      entt::entity& ObjLifeCycleControl,
      entt::entity& TextureMap
      );
  void Update_ObjLifecycle(const std::shared_ptr<entt::registry>& pECS,
      entt::entity& ObjLifeCycleControl);
  int Create_SceneCtrl (const std::shared_ptr<entt::registry>& pECS,
                        entt::entity& SceneCtrlEntity);

  void Create_Titles(const std::shared_ptr<entt::registry>& pECS,
      entt::entity& ObjLifeCycleControl);
  int Create_Fonts(std::unordered_map<std::string, TTF_Font*>& mapFonts);
  void Destroy_Fonts(std::unordered_map<std::string, TTF_Font*>& mapFonts);
  void Create_Textures(const std::shared_ptr<entt::registry>& pECS,
                  entt::entity& TextureMap);
  void Destroy_Textures(std::unordered_map<std::string,SDL_Texture *>& mapTextures);
  void Create_Entities(const std::shared_ptr<entt::registry>& pECS,entt::entity& ObjLifecycleEntity);
    // Entities 
    void Create_Enemies(const std::shared_ptr<entt::registry>& pECS,entt::entity& ObjLifeCycleControl);
    void Create_Player(const std::shared_ptr<entt::registry>& pECS,entt::entity& ObjLifeCycleControl);

  void Create_Inputs(const std::shared_ptr<entt::registry>& pECS);


  void Start();
  void Stop();

  // Loop Trigger
  bool m_bLoop = false;
  void Set_bLoop(const bool& Val){m_bLoop = Val;};
  bool& Get_bLoop(){return m_bLoop;};

  void MainLoop();
  std::shared_ptr<std::thread> m_pThrLoop = nullptr;

  // ECS instance
  std::shared_ptr<entt::registry> m_pECS_registry = nullptr;

  // ECS systems
  Sys_Sprite_t     m_Sys_Sprite;
  Sys_Input_t      m_Sys_Input;
  Sys_Logic_t      m_Sys_Logic;
  Sys_ObjPlugins_t m_Sys_ObjPlugins;
  Sys_SceneCtrl_t  m_Sys_SceneCtrl;

  // SDL context
  SDL_Window* m_pWindow     = nullptr; 
  SDL_Renderer* m_pRenderer = nullptr;
  std::unordered_map<std::string, TTF_Font*> m_mapFonts;

  std::unordered_map<std::string,SDL_Texture *> m_mapTextures;
  
  entt::entity m_ObjLifecycleEntity;
  entt::entity m_TextureMapEntity;
  entt::entity m_SceneCtrlEntity;
};

#endif /* ifndef _CAPP_H_ */
