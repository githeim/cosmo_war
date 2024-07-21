#ifndef _APP_COMPONENTS_
#define _APP_COMPONENTS_ 

#include <vector>
#include <SDL2/SDL.h>
#include <entt/entt.hpp>

#define SPRITE_SIZE (48)

// Enemy Movement Width (pixel)
#define ENEMY_MOVEMENT_WIDTH (60)


typedef enum {
  SCENE_NONE           = 0x0,
  SCENE_TITLE          = 0x1,
  SCENE_PLAY           = 0x2,
  SCENE_WIN_ENDING     = 0x4,
  SCENE_DEFEAT_ENDING  = 0x8
} Scene_idx_t;

typedef struct SceneCtrl {
  // Requested Scene
  std::vector<Scene_idx_t> vecScene;
  Scene_idx_t CurrentScene;

  std::vector<std::function<void(entt::registry& Reg, entt::entity ObjEntity,
                            double &dbActualFrameDiff_SEC,
                            entt::entity &ObjLifecycleEntity)>>  vecPlugins;
} SceneCtrl_t;


typedef struct Location {
  int iX;
  int iY;
  int iW;
  int iH;
  int iAngle;
}Location_t;
typedef struct Pos {
  Location_t CurLocation;
} Pos_t;
typedef struct Sprite {
  SDL_Texture * pTex;
  SDL_Rect src;
  SDL_Rect dst;
  int iAngle;
}Sprite_t;

typedef enum {
  PLAYER_1 =    0x1,
  PLAYER_2 =    0x2,
  ENEMY_1  =    0x4,
  ENEMY_2  =    0x8, // if the different type of enemy is existed
  BULLET_1 =   0x10,
  BURST_1  =   0x20
}Faction_t;


typedef enum Direction{
  DIRECTION_LEFT,
  DIRECTION_RIGHT,
  DIRECTION_UP,
  DIRECTION_DOWN
} Direction_t;

// Structure for fixed attributes
typedef struct Attr {
  Faction_t iFaction;
  int iLife; // Life gauge 
  float fCoolTime_Sec;
  int iMovingSpeed_Pixel; // Moving Speed of Obj
  std::vector<std::function<void(entt::registry& Reg, entt::entity ObjEntity,
                            double &dbActualFrameDiff_SEC,
                            entt::entity &ObjLifecycleEntity)>>  vecPlugins;
}Attr_t;

// Structure for mutable State
typedef struct State {
  float fCoolTimeLeft_Sec;
  bool bCoolTimeReady;
  int iLife; // Current Life gauge 
  Location_t TargetLocation;
  Direction_t Direction; // Moving Direction
}State_t;

// Game objects like enemy/player/bullets ...
typedef struct Obj {
  Sprite_t Sprite;
  Pos_t Pos;
  Attr_t Attr;
  State_t State;
}Obj_t;

// Object update component , it controls entities lifecycle
typedef struct ObjLifecyle {
  std::vector<Obj_t> Create_List;
  std::set<entt::entity> Delete_List;
  entt::entity TextureMap;
} ObjLifecyle_t;

typedef struct TextureMap {
  std::unordered_map<std::string,SDL_Texture *> mapTextures;
} TextureMap_t;



#endif /* ifndef _APP_COMPONENTS_ */
