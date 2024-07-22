#ifndef _APP_PLUGINS_H_
#define _APP_PLUGINS_H_ 

#include <entt/entt.hpp>
#include "App_Components.h"

#include <cmath>
// Function to calculate distance
inline double GetDist(double x1, double y1, double x2, double y2) {
    return std::sqrt(std::pow((x2 - x1), 2) + std::pow((y2 - y1), 2));
}


void         Set_SceneCtrl_Entity(entt::entity Entity); 
entt::entity Get_SceneCtrl_Entity();

void Plugin_Player_State(entt::registry& Reg,entt::entity ObjEntity,
                         double &dbActualFrameDiff_SEC,
                         entt::entity &ObjLifecycleEntity);


void Plugin_Enemy_Moving(entt::registry& Reg,entt::entity ObjEntity, 
                         double &dbActualFrameDiff_SEC,
                         entt::entity &ObjLifecycleEntity);
void Plugin_Enemy_Shooting(entt::registry& Reg,entt::entity ObjEntity,
                         double &dbActualFrameDiff_SEC,
                         entt::entity &ObjLifecycleEntity); 
void Plugin_Enemy_State(entt::registry& Reg,entt::entity ObjEntity,
                         double &dbActualFrameDiff_SEC,
                         entt::entity &ObjLifecycleEntity); 

void Plugin_Bullet_Moving(entt::registry& Reg,entt::entity ObjEntity,
                         double &dbActualFrameDiff_SEC,
                         entt::entity &ObjLifecycleEntity); 
void Plugin_Bullet_State(entt::registry& Reg,entt::entity ObjEntity,
                         double &dbActualFrameDiff_SEC,
                         entt::entity &ObjLifecycleEntity); 

void Plugin_SceneCtrl(entt::registry& Reg,entt::entity SceneEntity,
                         double &dbActualFrameDiff_SEC,
                         entt::entity &ObjLifecycleEntity);


void Spawn_Bullet( SDL_Texture * pTex,
                   const Pos_t& Pos,
                   State_t& State,
                   Obj_t& Obj
                   );
void Spawn_Burst( SDL_Texture * pTex,
                   const Pos_t& Pos,
                   const State_t& State,
                   Obj_t& Obj
                   );

#define COLLISION_RANGE (24) // Pixels
#define SEACHING_RANGE  (24) // Pixels

#define ENEMY_LIFE (1)
#define PLAYER_LIFE (1)
#define BULLET_LIFE (1)

#define BULLET_DAMAGE (1)

#define ENTER_KEY_INPUT_DELAY (0.3)
#endif /* ifndef _APP_PLUGINS_H_ */
