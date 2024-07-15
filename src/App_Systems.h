#ifndef _APP_SYSTEMS_H_
#define _APP_SYSTEMS_H_

#include <entt/entt.hpp>
#include <SDL2/SDL.h>
#include "App_Components.h"
#include <stdio.h>
#include "SDL_ctx.h"

#include <functional>
#include "App_Plugins.h"

#include "cxxabi.h"
typedef struct Sys_Sprite {
  void Update(entt::registry& Reg){
    static auto View = Reg.view<Sprite_t,Pos_t>();
    View.each([](auto &Sprite,auto &Pos) {
        Sprite.dst.x =  Pos.CurLocation.iX;
        Sprite.dst.y =  Pos.CurLocation.iY;
        Sprite.iAngle = Pos.CurLocation.iAngle;
        });
  }
  void Render( entt::registry& Reg,SDL_Renderer* pRenderer) {
    static auto View = Reg.view<Sprite_t>();
    View.each([pRenderer](auto &Sprite) {
        SDL_RenderCopyEx(pRenderer, Sprite.pTex, &Sprite.src, &Sprite.dst,
            (double)Sprite.iAngle, NULL, SDL_FLIP_NONE );
        });
  }

}Sys_Sprite_t;

typedef struct Sys_ObjPlugins {
  void Update(entt::registry& Reg, double &dbActualFrameDiff_SEC,
              entt::entity &ObjLifecycleEntity) {
   static auto View = Reg.view<Sprite_t,Pos_t,Attr_t,State_t>();
   for (auto Entity : View) {
     auto [Sprite,Pos,Attr,State] = View.get<Sprite_t,Pos_t,Attr_t,State_t>(Entity);
     for (auto Plugin : Attr.vecPlugins){
       Plugin(Reg,Entity,dbActualFrameDiff_SEC,ObjLifecycleEntity);
     }

   }

  }

}Sys_ObjPlugins_t;

typedef struct Sys_SceneCtrl {
  void Update(entt::registry& Reg, double &dbActualFrameDiff_SEC,
              entt::entity &SceneCtrlEntity,
              entt::entity &ObjLifecycleEntity) {
    auto &SceneCtrl = Reg.get<SceneCtrl_t>(SceneCtrlEntity);
    for (auto Plugin : SceneCtrl.vecPlugins) {
      Plugin(Reg,SceneCtrlEntity,dbActualFrameDiff_SEC,ObjLifecycleEntity);


    }
  }


} Sys_SceneCtrl_t;

typedef struct Sys_Logic {
  
  void Update(entt::registry& Reg,const double &dbActualFrameDiff_SEC,entt::entity &ObjLifecycleEntity){

    auto View = Reg.view<Attr_t,Pos_t,State_t>();
    auto &LifeCycle = Reg.get<ObjLifecyle_t>(ObjLifecycleEntity);

    entt::entity TextureMapEntity = LifeCycle.TextureMap;
    auto TextureMap = Reg.get<TextureMap_t>(TextureMapEntity);
    std::vector<Obj_t> Create_List ={};
    std::vector<entt::entity> Delete_List={};


    for (auto Item : Delete_List) {
      LifeCycle.Delete_List.insert(Item);
    }
    for (auto Item : Create_List) {
      LifeCycle.Create_List.push_back(Item);
    }

 } // :x: end void Update()

} Sys_Logic_t;



typedef struct Sys_Input {
  void Update(entt::registry& Reg,double &dbActual_Frame_diff_SEC,entt::entity &ObjLifecycleEntity) {

    // This function is working after SDL_PollEvent() calling
    const Uint8* Keys = SDL_GetKeyboardState(NULL);
    auto &LifeCycle = Reg.get<ObjLifecyle_t>(ObjLifecycleEntity);
    entt::entity TextureMapEntity = LifeCycle.TextureMap;
    auto TextureMap = Reg.get<TextureMap_t>(TextureMapEntity);
    SDL_Texture *pBulletTex = TextureMap.mapTextures["bullet00"];

    auto View = Reg.view<Attr_t,Pos_t,State_t>();
    std::vector<Obj_t> Create_List ={};
    View.each([pBulletTex,&Create_List,Keys,dbActual_Frame_diff_SEC] (auto &Attr,auto &Pos,auto &State){
          // Actions for Player1 -------------------------
          if (Attr.iFaction == PLAYER_1 ) {
            // Cool time operation
            if (State.bCoolTimeReady == false) {
              State.fCoolTimeLeft_Sec -= dbActual_Frame_diff_SEC;
            }
            if  ( State.fCoolTimeLeft_Sec < 0 )  {
              State.fCoolTimeLeft_Sec = Attr.fCoolTime_Sec;
              State.bCoolTimeReady = true;
            }

            // Moving Delta 
            int iPlayerDelta =Attr.iMovingSpeed_Pixel;

            if (Keys[SDL_SCANCODE_LEFT]) {
              if (Pos.CurLocation.iX-iPlayerDelta > 0) 
                Pos.CurLocation.iX-=iPlayerDelta;
            }

            if (Keys[SDL_SCANCODE_RIGHT]) {
              if (Pos.CurLocation.iX+iPlayerDelta < SCREEN_WIDTH-SPRITE_SIZE) 
                Pos.CurLocation.iX+=iPlayerDelta;
            }

            if (Keys[SDL_SCANCODE_SPACE]) {

              if (State.bCoolTimeReady == true) {
                State.bCoolTimeReady = false;
                // Spawn Bullet
                Pos_t BulletPos ;
                  // Spawn position : uppper side of the player
                BulletPos.CurLocation = { Pos.CurLocation.iX,
                                          Pos.CurLocation.iY-SPRITE_SIZE,
                                    SPRITE_SIZE,SPRITE_SIZE,0 };
                State_t BulletState;
                BulletState.Direction=DIRECTION_UP;
                BulletState.fCoolTimeLeft_Sec=0;
                BulletState.bCoolTimeReady=true;
                BulletState.TargetLocation={Pos.CurLocation.iX,0,SPRITE_SIZE,SPRITE_SIZE,0};
                Obj_t Obj;
                Spawn_Bullet(pBulletTex, BulletPos, BulletState,Obj);
                Create_List.push_back(Obj);

              }

            }
          } // end if (Attr.iFaction == PLAYER_1 ) 


        }
        );
    for (auto Item : Create_List) {
      LifeCycle.Create_List.push_back(Item);
    }

  }

}Sys_Input_t;

#endif /* ifndef _APP_SYSTEMS_H_ */