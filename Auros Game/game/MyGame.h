#pragma once
#include "Game.h"


class CMyGame : public CGame
{
public:
	CMyGame();
	~CMyGame();

    // ----  Declare your game variables and objects here -------------

	// Variables
	int score;
	int level;
	float tilt;
	float rotation;
	float zoom;


    // Declare Models
    CModel player;   // animated model
    CModel asteroid;
    CModel boss;
    CModel enemy;
	CModel enemyBlue;
	CModel enemyStar;
	CModel planet;

	// Declare in-game sounds
	CSoundPlayer worldMusic;
	CSoundPlayer shotSFX;
	CSoundPlayer explosionSound;
	CSoundPlayer impactSound;
	CSoundPlayer engine1;
	CSoundPlayer engine2;
	CSoundPlayer success;

	// Explosion spritesheets
	CSprite explosion;
	CSpriteList explosions;

	// Health bar
	CHealthBar hbar;
    
    // the skydome is a CModel too
    CModel skydome;
	
	// Declare Model Lists
	CModelList enemies;
	CModelList enemiesBlue;
	CModelList enemiesStar;
	CModelList asteroids;
	CModelList shots;
		
	// game world floor
	CFloor floor;
	
	// Font
	CFont font;
	
	CSprite startScreen;


	// -----   Add you member functions here ------
   
   void PlayerControl();
   void CameraControl(CGraphics* g);
   void EnemyControl();
   void ShotsControl();
   void BossControl();

	
   // ---------------------Event Handling --------------------------

	// Game Loop Funtions
	virtual void OnUpdate();
	virtual void OnDraw(CGraphics* g);
	virtual void OnRender3D(CGraphics* g);
	

	// Game Life Cycle
	virtual void OnInitialize();
	virtual void OnDisplayMenu();
	virtual void OnStartLevel(int level);
	virtual void OnStartGame();
	void SetupLevel1();
	void SetupLevel2();
	void SetupLevel3();
	virtual void OnGameOver();
	virtual void OnTerminate();

	// Keyboard Event Handlers
	virtual void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	virtual void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

	// Mouse Events Handlers
	virtual void OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle);
	virtual void OnLButtonDown(Uint16 x,Uint16 y);
	virtual void OnLButtonUp(Uint16 x,Uint16 y);
	virtual void OnRButtonDown(Uint16 x,Uint16 y);
	virtual void OnRButtonUp(Uint16 x,Uint16 y);
	virtual void OnMButtonDown(Uint16 x,Uint16 y);
	virtual void OnMButtonUp(Uint16 x,Uint16 y);
};
