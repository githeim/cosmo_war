#include "SDL_ctx.h"
#include "App_Plugins.h"
#include <stdlib.h>



static entt::entity g_SceneCtrl_Entity;
void Set_SceneCtrl_Entity(entt::entity Entity) {
  g_SceneCtrl_Entity = Entity;
}
entt::entity Get_SceneCtrl_Entity() {
  return g_SceneCtrl_Entity;
}




void Create_Player(entt::registry& Reg,
                         entt::entity& ObjLifecycleEntity) {
  auto &LifeCycle = Reg.get<ObjLifecyle_t>(ObjLifecycleEntity);
  // Get TextureMap
  entt::entity TextureMapEntity = LifeCycle.TextureMap;
  static auto &TextureMap = Reg.get<TextureMap_t>(TextureMapEntity);

  // Base Position
  Pos_t Pos; 
  Pos.CurLocation = { SCREEN_WIDTH/2,SCREEN_HEIGHT*4/5,SPRITE_SIZE,SPRITE_SIZE,0 };
  // Base Attribute
  Attr_t Attr;
  Attr.iFaction = PLAYER_1;
  Attr.iLife = PLAYER_LIFE;
  Attr.fCoolTime_Sec = .1f;  // Firing cool time
  Attr.iMovingSpeed_Pixel = 3;  // Moving Speed
  Attr.vecPlugins.push_back(Plugin_Player_State);
  // Base Sprite
  Sprite_t Sprite;
  Sprite.pTex = TextureMap.mapTextures["spaceship"];
  Sprite.src ={0,0,SPRITE_SIZE,SPRITE_SIZE};
  Sprite.dst = {0,0,SPRITE_SIZE,SPRITE_SIZE}; 

  // Base State
  State_t State;
  State.fCoolTimeLeft_Sec=Attr.fCoolTime_Sec;
  State.bCoolTimeReady=true;
  State.TargetLocation ={};
  State.Direction ={};
  State.iLife = Attr.iLife;

  // Create obj to insert create list
  Obj_t Obj = {Sprite,Pos,Attr,State};

  LifeCycle.Create_List.push_back(Obj);
}
void Create_Enemies(entt::registry& Reg,
                         entt::entity& ObjLifecycleEntity) {
  auto &LifeCycle = Reg.get<ObjLifecyle_t>(ObjLifecycleEntity);
  
  // Get TextureMap
  entt::entity TextureMapEntity = LifeCycle.TextureMap;
  static auto &TextureMap = Reg.get<TextureMap_t>(TextureMapEntity);

  // Base Position
  Pos_t Pos ;
  Pos.CurLocation = { 30,60,SPRITE_SIZE,SPRITE_SIZE,0 };
  // Base Attribute
  Attr_t Attr;
  Attr.iFaction = ENEMY_1;
  Attr.iLife = ENEMY_LIFE;
  Attr.fCoolTime_Sec = .1f;  // Firing cool time
  Attr.iMovingSpeed_Pixel = 2;  // Moving Speed

  // Moving processing plugin
  Attr.vecPlugins.push_back(Plugin_Enemy_Moving);
  // Shooting processing plugin
  Attr.vecPlugins.push_back(Plugin_Enemy_Shooting);
  Attr.vecPlugins.push_back(Plugin_Enemy_State);


  int iIntervalX = SPRITE_SIZE*2;


  // Base Sprite
  Sprite_t Sprite;
  Sprite.pTex = TextureMap.mapTextures["enemy00"];
  Sprite.src ={0,0,SPRITE_SIZE,SPRITE_SIZE};
  Sprite.dst = {0,0,SPRITE_SIZE,SPRITE_SIZE}; 

  // Base State
  State_t State;
  State.fCoolTimeLeft_Sec=Attr.fCoolTime_Sec;
  State.bCoolTimeReady=true;
  State.TargetLocation ={0,0,SPRITE_SIZE,SPRITE_SIZE,0};// TargetLocation initial value;
  State.Direction = DIRECTION_RIGHT;
  State.iLife = Attr.iLife;



  // Create 6 enemies
  for (int i =0 ; i < 6 ; i++) {
    Pos.CurLocation.iX = 30 + (iIntervalX*i);
    State.Direction =DIRECTION_RIGHT;
    State.TargetLocation.iX=Pos.CurLocation.iX + ENEMY_MOVEMENT_WIDTH;
    Obj_t Obj = {Sprite,Pos,Attr,State};
    LifeCycle.Create_List.push_back(Obj);
  }
}


void Plugin_Player_State(entt::registry& Reg,entt::entity ObjEntity,
                         double &dbActualFrameDiff_SEC,
                         entt::entity &ObjLifecycleEntity) 
{
  std::vector<Obj_t> Create_List ={};
  std::vector<entt::entity> Delete_List={};
  auto &LifeCycle = Reg.get<ObjLifecyle_t>(ObjLifecycleEntity);
  auto &PlayerState  = Reg.get<State_t>(ObjEntity);

  if (PlayerState.iLife <= 0 ) {
    Delete_List.push_back(ObjEntity);
  }

  for (auto Item : Create_List) {
    LifeCycle.Create_List.push_back(Item);
  }
  for (auto Item : Delete_List) {
    LifeCycle.Delete_List.insert(Item);
  }

}

/**
 * @brief Create Text Object
 *
 * @param TextObj[OUT]
 * @param pTex[IN]
 * @param Pos[IN]
 */
void Create_TextObj(Obj_t& TextObj,SDL_Texture* pTex,Pos_t& Pos) {
  SDL_Rect Src={};
  SDL_Rect Dst={};
  SDL_QueryTexture(pTex, NULL, NULL, &Src.w, &Src.h);
  if (Pos.CurLocation.iW == 0 && Pos.CurLocation.iH == 0) {
    Pos.CurLocation.iW = Src.w; 
    Pos.CurLocation.iH = Src.h; 
  }

  Dst.x = Pos.CurLocation.iX;
  Dst.y = Pos.CurLocation.iY;
  Dst.w = Pos.CurLocation.iW;
  Dst.h = Pos.CurLocation.iH;

  TextObj = {
    {pTex,Src,Dst,0}, // Sprite_t
    Pos,                       // Pos_t
  };
}

void Plugin_SceneCtrl(entt::registry& Reg,entt::entity SceneEntity,
                         double &dbActualFrameDiff_SEC,
                         entt::entity &ObjLifecycleEntity) 
{
  auto &LifeCycle = Reg.get<ObjLifecyle_t>(ObjLifecycleEntity);
  auto &SceneCtrl = Reg.get<SceneCtrl_t>(SceneEntity);

  std::vector<Obj_t> Create_List ={};
  std::vector<entt::entity> Delete_List={};
  Scene_idx_t RequestedScene = SCENE_NONE;

  // Get TextureMap
  entt::entity TextureMapEntity = LifeCycle.TextureMap;
  static auto &TextureMap = Reg.get<TextureMap_t>(TextureMapEntity);

  // Get Requested scene from the vector
  if (SceneCtrl.vecScene.size() != 0) {
    RequestedScene = *SceneCtrl.vecScene.begin();
    SceneCtrl.vecScene.erase(SceneCtrl.vecScene.begin());
  }

  if (RequestedScene == SCENE_TITLE) {
    //Delete Objs
    auto ViewDelete = Reg.view<Sprite_t,Pos_t,Attr_t,State_t>();
    for (auto Entity : ViewDelete) {
      LifeCycle.Delete_List.insert(Entity);
    }

    // Create Main Title
    auto pTitleTextTex = TextureMap.mapTextures["TitleText"];
    Pos_t Pos;
    Obj_t Title;
    memset(&Pos,0x00,sizeof(Pos_t));
    Pos.CurLocation.iX = 100;
    Pos.CurLocation.iY = 100;
    Create_TextObj(Title,pTitleTextTex,  Pos);
    Create_List.push_back(Title);
    
    // Create "Press Enter Key" Title
    Obj_t Press_Enter_Key;
    auto pPressEnterKeyTex = TextureMap.mapTextures["Press_Enter_Key"];
    memset(&Pos,0x00,sizeof(Pos_t));
    Pos.CurLocation.iX = 130;
    Pos.CurLocation.iY = 350;
    Create_TextObj(Press_Enter_Key,pPressEnterKeyTex,  Pos);
    Create_List.push_back(Press_Enter_Key);

    // Create Credit Title
    Obj_t Credit;
    auto pCreditTex = TextureMap.mapTextures["credit"];
    memset(&Pos,0x00,sizeof(Pos_t));
    Pos.CurLocation.iX = 50;
    Pos.CurLocation.iY = 450;
    Create_TextObj(Credit,pCreditTex,  Pos);
    Create_List.push_back(Credit);

    // Change Current Scene
    SceneCtrl.CurrentScene = SCENE_TITLE;
  }
  else if (RequestedScene == SCENE_PLAY) {
    // Delete titles, Objects
    auto ViewDelete = Reg.view<Sprite_t,Pos_t,Attr_t,State_t>();
    for (auto Entity : ViewDelete) {
      LifeCycle.Delete_List.insert(Entity);
    }
    // Create Objects
    Create_Player(Reg,ObjLifecycleEntity);
    Create_Enemies(Reg,ObjLifecycleEntity);
    // Change Current Scene
    SceneCtrl.CurrentScene = SCENE_PLAY;
  }
  else if (RequestedScene == SCENE_WIN_ENDING) {
    printf("\033[1;33m[%s][%d] :x: SCENE_WIN_ENDING \033[m\n",__FUNCTION__,__LINE__);
    // Delete titles, Objects
    auto ViewDelete = Reg.view<Sprite_t,Pos_t,Attr_t,State_t>();
    for (auto Entity : ViewDelete) {
      LifeCycle.Delete_List.insert(Entity);
    }
    // Create Win Title
    auto pWinTex = TextureMap.mapTextures["You_Win"];
    Pos_t Pos;
    Obj_t Title;
    memset(&Pos,0x00,sizeof(Pos_t));
    Pos.CurLocation.iX = 320-(336/2);
    Pos.CurLocation.iY = 240;
    Create_TextObj(Title,pWinTex,  Pos);
    Create_List.push_back(Title);
    SceneCtrl.CurrentScene = SCENE_WIN_ENDING;
  }
  else if (RequestedScene == SCENE_DEFEATED_ENDING) {
    printf("\033[1;33m[%s][%d] :x: SCENE_DEFEATED_ENDING \033[m\n",__FUNCTION__,__LINE__);
    // Delete titles, Objects
    auto ViewDelete = Reg.view<Sprite_t,Pos_t,Attr_t,State_t>();
    for (auto Entity : ViewDelete) {
      LifeCycle.Delete_List.insert(Entity);
    }
    // Create Defeated Title
    auto pDefeatedTex = TextureMap.mapTextures["Defeated"];
    Pos_t Pos;
    Obj_t Title;
    memset(&Pos,0x00,sizeof(Pos_t));
    Pos.CurLocation.iX = 320-(384/2);
    Pos.CurLocation.iY = 240;
    Create_TextObj(Title,pDefeatedTex,  Pos);

    Create_List.push_back(Title);
    SceneCtrl.CurrentScene = SCENE_DEFEATED_ENDING;
  }


  for (auto Item : Create_List) {
    LifeCycle.Create_List.push_back(Item);
  }
  for (auto Item : Delete_List) {
    LifeCycle.Delete_List.insert(Item);
  }
}
void Plugin_Enemy_State(entt::registry& Reg,entt::entity ObjEntity,
                         double &dbActualFrameDiff_SEC,
                         entt::entity &ObjLifecycleEntity) 
{
  std::vector<Obj_t> Create_List ={};
  std::vector<entt::entity> Delete_List={};
  auto &LifeCycle = Reg.get<ObjLifecyle_t>(ObjLifecycleEntity);
  auto &EnemyState  = Reg.get<State_t>(ObjEntity);

  if (EnemyState.iLife <= 0 ) {
    Delete_List.push_back(ObjEntity);
  }

  for (auto Item : Create_List) {
    LifeCycle.Create_List.push_back(Item);
  }
  for (auto Item : Delete_List) {
    LifeCycle.Delete_List.insert(Item);
  }

}

void Plugin_Enemy_Moving(entt::registry& Reg,entt::entity ObjEntity,
                         double &dbActualFrameDiff_SEC,
                         entt::entity &ObjLifecycleEntity) {
  auto [Sprite,Pos,Attr,State] = 
                              Reg.get<Sprite_t,Pos_t,Attr_t,State_t>(ObjEntity);

  // Moving Delta 
  int iEnemyDelta = Attr.iMovingSpeed_Pixel;
  // Repeat left and right movement
  if (Pos.CurLocation.iX == State.TargetLocation.iX 
     ){
    if ( State.Direction == DIRECTION_RIGHT) {
      State.TargetLocation.iX = State.TargetLocation.iX-ENEMY_MOVEMENT_WIDTH;
      State.Direction = DIRECTION_LEFT;
    } else if ( State.Direction == DIRECTION_LEFT){
      State.TargetLocation.iX = State.TargetLocation.iX+ENEMY_MOVEMENT_WIDTH;
      State.Direction = DIRECTION_RIGHT;
    }
  }
  else if (Pos.CurLocation.iX < State.TargetLocation.iX) {
    Pos.CurLocation.iX+=iEnemyDelta;
  }
  else if (Pos.CurLocation.iX > State.TargetLocation.iX) {
    Pos.CurLocation.iX-=iEnemyDelta;
  }

  return;
}

void Plugin_Enemy_Shooting(entt::registry& Reg,entt::entity ObjEntity,
                         double &dbActualFrameDiff_SEC,
                         entt::entity &ObjLifecycleEntity) {
  // For Spawning bullets
  std::vector<Obj_t> Create_List ={};
  // Get TextureMap
  auto &LifeCycle = Reg.get<ObjLifecyle_t>(ObjLifecycleEntity);
  entt::entity TextureMapEntity = LifeCycle.TextureMap;
  static auto &TextureMap = Reg.get<TextureMap_t>(TextureMapEntity);
  auto pBulletTex = TextureMap.mapTextures["bullet00"];

  // if the player is in the x range,the enemy fires the bullet
  auto [EnemyPos,EnemyAttr,EnemyState] = Reg.get<Pos_t,Attr_t,State_t>(ObjEntity);
  static auto ViewObjs = Reg.view<Sprite_t,Pos_t,Attr_t,State_t>();

  if (EnemyState.bCoolTimeReady == false) {
    EnemyState.fCoolTimeLeft_Sec -= dbActualFrameDiff_SEC;
  }
  if  ( EnemyState.fCoolTimeLeft_Sec < 0 )  {
    EnemyState.fCoolTimeLeft_Sec = EnemyAttr.fCoolTime_Sec;
    EnemyState.bCoolTimeReady = true;
  }

  if (EnemyState.bCoolTimeReady == false) {
    return;
  }

  // Seaching range 
  int iRange = SEACHING_RANGE; // pixel
  // Find target(player faction)
  static const int iFactionFilter = PLAYER_1 | PLAYER_2;
  for (auto Entity : ViewObjs) {
     auto [TargetPos,TargetAttr] = ViewObjs.get<Pos_t,Attr_t>(Entity);
     
     if ( EnemyState.bCoolTimeReady &&  (TargetAttr.iFaction & iFactionFilter)) 
     {
       int iDist = abs(EnemyPos.CurLocation.iX -TargetPos.CurLocation.iX);
       if(iDist < iRange ) {
         // Update cooltime state
         EnemyState.bCoolTimeReady = false;
         // Spawn Bullet
         // Spawn Bullet
         Pos_t BulletPos ;
           // Spawn position : down side of the enemy 
         BulletPos.CurLocation = { EnemyPos.CurLocation.iX,
                                   EnemyPos.CurLocation.iY+SPRITE_SIZE,
                                   SPRITE_SIZE,SPRITE_SIZE,
                                   180  }; // down side - 180
         State_t BulletState;
         BulletState.Direction=DIRECTION_DOWN;
         BulletState.fCoolTimeLeft_Sec=0;
         BulletState.bCoolTimeReady=true;
         BulletState.TargetLocation=
                           {EnemyPos.CurLocation.iX,0,SPRITE_SIZE,SPRITE_SIZE,0};
         Obj_t Obj;
         Spawn_Bullet(pBulletTex, BulletPos, BulletState,Obj);
         Create_List.push_back(Obj);
       }
     }
  } // end for (auto Entity : ViewObjs)
    
  for (auto Item : Create_List) {
    LifeCycle.Create_List.push_back(Item);
  }

} // end void Plugin_Enemy_Shooting()




void Plugin_Bullet_State(entt::registry& Reg,entt::entity ObjEntity,
    double &dbActualFrameDiff_SEC,
    entt::entity &ObjLifecycleEntity) {
  std::vector<Obj_t> Create_List ={};
  std::vector<entt::entity> Delete_List={};
  auto &LifeCycle = Reg.get<ObjLifecyle_t>(ObjLifecycleEntity);

  auto &BulletState  = Reg.get<State_t>(ObjEntity);

  if (BulletState.iLife <= 0 ) {
    Delete_List.push_back(ObjEntity);
  }

  for (auto Item : Create_List) {
    LifeCycle.Create_List.push_back(Item);
  }
  for (auto Item : Delete_List) {
    LifeCycle.Delete_List.insert(Item);
  }


}
void Plugin_Bullet_Moving(entt::registry& Reg,entt::entity ObjEntity,
                         double &dbActualFrameDiff_SEC,
                         entt::entity &ObjLifecycleEntity) {
  std::vector<Obj_t> Create_List ={};
  std::vector<entt::entity> Delete_List={};
  auto &LifeCycle = Reg.get<ObjLifecyle_t>(ObjLifecycleEntity);

  static auto ViewObjs = Reg.view<Sprite_t,Pos_t,Attr_t,State_t>();

  // Get TextureMap of burst
  entt::entity TextureMapEntity = LifeCycle.TextureMap;
  static auto &TextureMap = Reg.get<TextureMap_t>(TextureMapEntity);
  static auto pBurstTex = TextureMap.mapTextures["burst00"];



  auto [BulletPos,BulletAttr,BulletState] = Reg.get<Pos_t,Attr_t,State_t>(ObjEntity);
  // Moving Delta 
  int iBulletDelta = BulletAttr.iMovingSpeed_Pixel;
  if (BulletState.Direction == DIRECTION_UP) {
    BulletPos.CurLocation.iY-=iBulletDelta;
  }
  else if (BulletState.Direction == DIRECTION_DOWN) {
    BulletPos.CurLocation.iY+=iBulletDelta;
  }
  // Delete bullets at the boundaries and spawn burst
  if (BulletPos.CurLocation.iY-(SPRITE_SIZE/2) <=0 || 
      BulletPos.CurLocation.iY+(SPRITE_SIZE)>=SCREEN_HEIGHT) {
    // Spawn Burst
    State_t BurstState;
    BurstState.fCoolTimeLeft_Sec = .3f; 
    Pos_t   BurstPos;
    Obj_t   BurstObj;
    BurstPos.CurLocation = {BulletPos.CurLocation.iX,
                            BulletPos.CurLocation.iY,
                            SPRITE_SIZE,SPRITE_SIZE,0
                           };

    Spawn_Burst(pBurstTex, BurstPos, BurstState, BurstObj);

    Create_List.push_back(BurstObj);

    Delete_List.push_back(ObjEntity);
  }

  // collision target
  static const int iFactionFilter = PLAYER_1 | PLAYER_2 |ENEMY_1| 
                                    ENEMY_2 | BULLET_1 ;


  int iCollisionRange = COLLISION_RANGE; // pixel
  // Collision Check
  for (auto Entity : ViewObjs) {
    if (Entity == ObjEntity ) // skip the self object 
      continue;
    auto [TargetPos,TargetAttr] = ViewObjs.get<Pos_t,Attr_t>(Entity);
    if (TargetAttr.iFaction & iFactionFilter)
    {
      double dbDist = GetDist(TargetPos.CurLocation.iX,TargetPos.CurLocation.iY,
          BulletPos.CurLocation.iX,BulletPos.CurLocation.iY);

      if (dbDist < iCollisionRange ) {
        // Spawn Burst
        State_t BurstState;
        BurstState.fCoolTimeLeft_Sec = .3f; 
        Pos_t   BurstPos;
        Obj_t   BurstObj;
        BurstPos.CurLocation = {BulletPos.CurLocation.iX,
          BulletPos.CurLocation.iY,
          SPRITE_SIZE,SPRITE_SIZE,0
        };

        // reduce the life gauge of the target
        auto &TargetState = ViewObjs.get<State_t>(Entity);
        TargetState.iLife -= BULLET_DAMAGE;

        Spawn_Burst(pBurstTex, BurstPos, BurstState, BurstObj);

        Create_List.push_back(BurstObj);

        Delete_List.push_back(ObjEntity);
      }

    }
  }
 

  for (auto Item : Create_List) {
    LifeCycle.Create_List.push_back(Item);
  }
  for (auto Item : Delete_List) {
    LifeCycle.Delete_List.insert(Item);
  }

} // end void Plugin_Bullet_Moving()
void Plugin_Burst(entt::registry& Reg,entt::entity ObjEntity,
                       double &dbActualFrameDiff_SEC,
                       entt::entity &ObjLifecycleEntity) {
  auto &LifeCycle = Reg.get<ObjLifecyle_t>(ObjLifecycleEntity);
  auto &BurstState = Reg.get<State_t>(ObjEntity);
  std::vector<entt::entity> Delete_List={};

  if (BurstState.fCoolTimeLeft_Sec >= 0 ) {
    BurstState.fCoolTimeLeft_Sec-=dbActualFrameDiff_SEC;
  }

  if (BurstState.fCoolTimeLeft_Sec < 0 ) {
    // destroy by itself
    Delete_List.push_back(ObjEntity);
  }

  for (auto Item : Delete_List) {
    LifeCycle.Delete_List.insert(Item);
  }

}

void Spawn_Bullet(
                   SDL_Texture * pTex,
                   const Pos_t& Pos,
                   State_t& State,
                   Obj_t& Obj
                   ) {
  Sprite_t Sprite;
  Sprite.pTex = pTex;
  Sprite.src ={0,0,SPRITE_SIZE,SPRITE_SIZE};
  Sprite.dst = {0,0,SPRITE_SIZE,SPRITE_SIZE}; 

  Attr_t Attr;
  Attr.iFaction = BULLET_1;
  Attr.iLife = BULLET_LIFE; // life gauge
  Attr.fCoolTime_Sec = 1.f;  // Firing cool time
  Attr.iMovingSpeed_Pixel = 5;  // Moving Speed
  Attr.vecPlugins.push_back(Plugin_Bullet_Moving);
  Attr.vecPlugins.push_back(Plugin_Bullet_State);

  State.iLife = Attr.iLife;
  Obj = {Sprite,Pos,Attr,State};
}

void Spawn_Burst(
                   SDL_Texture * pTex,
                   const Pos_t& Pos,
                   const State_t& State,
                   Obj_t& Obj
                   ) {
  Sprite_t Sprite;
  Sprite.pTex = pTex;
  Sprite.src ={0,0,SPRITE_SIZE,SPRITE_SIZE};
  Sprite.dst = {0,0,SPRITE_SIZE,SPRITE_SIZE}; 

  Attr_t Attr;
  Attr.iFaction = BURST_1;
  Attr.iLife = 10;
  Attr.fCoolTime_Sec = .0f;  
  Attr.iMovingSpeed_Pixel = 0; 
  Attr.vecPlugins.push_back(Plugin_Burst);

  Obj = {Sprite,Pos,Attr,State};
}
