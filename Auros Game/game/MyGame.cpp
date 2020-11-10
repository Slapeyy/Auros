#include "MyGame.h"
#include "Sprite.h"

using namespace std;

CMyGame::CMyGame(void) {}

CMyGame::~CMyGame(void) {}



// --------  game initialisation --------
void CMyGame::OnInitialize()
{
	// Loading graphics and sound assets
	cout << "Loading assets" << endl;
	
	CTexture::smooth=true;
	
	font.LoadDefault(); 
	
    // Player model
	player.LoadModel("Player.obj","texture2.bmp");  
	player.SetScale(100.0f);

    // enemy model
    enemy.LoadModel("ArwingBlack.obj","OrangeBlack.bmp");
    enemy.SetScale(100.0f);    

	// blue enemy model
	enemyBlue.LoadModel("ArwingBlue.obj", "texture3.bmp");
	enemyBlue.SetScale(100.0f);

	// star enemy model
	enemyStar.LoadModel("Arwing_Star.obj", "texture4.bmp");
	enemyStar.SetScale(100.0f);
    
    // boss model
    boss.LoadModel("moon.obj","moon.bmp");
    boss.SetScale(0.5f);

	// asteroid model
	asteroid.LoadModel("Asteroid.obj", "asteroidTexture.bmp");
	asteroid.SetScale(10.0f);

	// planet model
	planet.LoadModel("Planet1.obj", "Planet_Diffuse.bmp");
	planet.SetScale(70.0f);

    // Load skydome geometry and texture
    skydome.LoadModel("Skydome2.obj","Milky Way.bmp");
    skydome.SetScale(1000);  // scale to desired size
    skydome.SetY(-200.0f); // move down to avoid seeing the base
    
	// load explosion animation
	explosion.LoadImage("explosionLine.bmp", CColor::White(), 16);

    startScreen.LoadImage("startScreen.bmp");
    startScreen.SetPosition(Width/2.0f,Height/2.0f);
	
	//Setup Health
	hbar.SetSize(30, 5);
	hbar.SetHealth(100);
	Light.Enable();  
}


void CMyGame::OnStartLevel(int level)
{  
	  shots.clear();
	  enemies.clear();
	  enemiesBlue.clear();
	  if (level == 1) SetupLevel1();
	  if (level == 2) SetupLevel2();
	  if (level == 3) SetupLevel3();

   //Setup music
  worldMusic.Play("Corneria.wav", -1);

  score = 0; tilt = 30; rotation = 45; zoom = 2.0f;
}

void CMyGame::SetupLevel1()
{
	planet.SetPosition(-90, -18500, 18000);
	planet.SetOmega(0, 3, 0);
	player.SetPosition(-10000, -10000, 10000);

	// adding 200 asteroid objects to the game world at random positions
	// random angular direction and distance from the centre
	// always clone existing model
	asteroids.clear();
	for (int n = 1; n <= 400; n++)
	{
		CModel* pAsteroid = asteroid.Clone();
		pAsteroid->SetDirection(rand() % 360);
		pAsteroid->SetPosition(-20000 + rand() % 40000, -20000 + rand() % 40000, -20000 + rand() % 40000); // moved to desired random position
		pAsteroid->SetOmega(-10 + rand() % 20, -10 + rand() % 20, -10 + rand() % 20); // random rotations
		asteroids.push_back(pAsteroid); // add asteroid to list
	}	for (int e = 1; e <= 3; e++)
	{
		CModel* pEnemy = enemy.Clone();
		pEnemy->SetDirection(-10 + rand() % 20, -10 + rand() % 20);
		pEnemy->SetRotationV(pEnemy->GetDirectionV());
		pEnemy->SetPosition(-5000 + rand() % 10000, -5000 + rand() % 10000, -5000 + rand() % 10000); // place at random position
		enemies.push_back(pEnemy); // add enemies to list	
	}
}

void CMyGame::SetupLevel2()
{
	for (int b = 1; b <= 7; b++)
	{
		CModel* pEnemyBlue = enemyBlue.Clone();
		pEnemyBlue->SetDirection(-10 + rand() % 20, -10 + rand() % 20);
		pEnemyBlue->SetRotationV(pEnemyBlue->GetDirectionV());
		pEnemyBlue->SetPosition(-5000 + rand() % 10000, -5000 + rand() % 10000, -5000 + rand() % 10000); // place at random position
		enemiesBlue.push_back(pEnemyBlue); // add enemies to list	
	}
}

void CMyGame::SetupLevel3()
{
	for (int s = 1; s <= 15; s++)
	{
		CModel* pEnemyStar = enemyStar.Clone();
		pEnemyStar->SetDirection(planet.GetX(), planet.GetY(), planet.GetZ());
		pEnemyStar->SetRotationV(pEnemyStar->GetDirectionV());
		pEnemyStar->SetPosition((boss.GetX() + 100) + rand() % -5000, (boss.GetY() + 100) + rand() % -5000, (boss.GetZ() + 100) + rand() % -5000); // place at random position
		enemiesStar.push_back(pEnemyStar); // add enemies to list	
	}
	BossControl();
}

// Game Logic in the OnUpdate function called every frame

void CMyGame::OnUpdate() 
{
	if (IsMenuMode() || IsGameOver()) return;
	
	long t =  GetTime();

	if (enemies.size() == 0 && level == 1)
	{
		level = 2;
		SetupLevel2();
	}

	if (enemiesBlue.size() == 0 && level == 2)
	{
		level = 3;
		SetupLevel3();
	}
	
	// --- updating models ----
	player.Update( t);
	boss.Update( t);
	planet.Update(t);

	for (CModel* pAsteroid : asteroids) pAsteroid->Update(t);
	for (CModel* pEnemy : enemies) pEnemy->Update(t);
	for (CModel* pEnemyBlue : enemiesBlue) pEnemyBlue->Update(t);
	for (CModel* pEnemyStar : enemiesStar) pEnemyStar->Update(t);
	for (CModel* pShot : shots) pShot->Update(t);
	for (CSprite* pExplosion : explosions) pExplosion->Update(t);

	PlayerControl();
	ShotsControl();
	EnemyControl();

	if (player.GetHealth() <= 0)
	{
		GameOver();
	}

	if (IsGameOver() && player.GetHealth() > 0)
	{
		worldMusic.Stop();
		success.Play("Success.wav");
		success.SetVolume(200);
	}
}

void CMyGame::PlayerControl()
{
	// Test if player hits max distance and wrap around
	if (player.GetX() > 20000)  player.SetX(-20000);
	if (player.GetY() > 20000)  player.SetY(-20000);
	if (player.GetZ() > 20000)  player.SetZ(-20000);

	if (player.GetX() < -20000)  player.SetX(20000);
	if (player.GetY() < -20000)  player.SetY(20000);
	if (player.GetZ() < -20000)  player.SetZ(20000);


	// Player speed and drag conditions
	if (IsKeyDown(SDLK_SPACE)) 
	{ 	
		player.SetSpeed(player.GetSpeed() + 50);
		if (player.GetSpeed() > 2500) player.SetSpeed(2500);
		//engine2.Play("YWing-Fly2.wav");
	}
	else
	{
		player.SetSpeed(player.GetSpeed() - 50);
		if (player.GetSpeed() < 500) player.SetSpeed(500);
		//engine1.Play("YWing-Fly1.wav");
	}
	
	for (CModel* pAsteroid : asteroids)
	{
		if (player.HitTest(pAsteroid))
		{
			pAsteroid->Delete();
			player.SetHealth(player.GetHealth() - 20);

			impactSound.Play("ShipImpact.wav");
		}		
	}
	asteroids.remove_if(deleted);

	for (CModel* pEnemy : enemies)
	{
		if (player.HitTest(pEnemy))
		{
			pEnemy->Delete();
			player.SetHealth(player.GetHealth() - 50);
			explosionSound.Play("explosion.wav");
			score++;
		}
	}

	for (CModel* pEnemyBlue : enemiesBlue)
	{
		if (player.HitTest(pEnemyBlue))
		{
			pEnemyBlue->Delete();
			player.SetHealth(player.GetHealth() - 50);
			explosionSound.Play("explosion.wav");
			score++;
		}
	}

	for (CModel* pEnemyStar : enemiesStar)
	{
		if (player.HitTest(pEnemyStar))
		{
			pEnemyStar->Delete();
			player.SetHealth(player.GetHealth() - 50);
			explosionSound.Play("explosion.wav");
			score++;
		}
	}
	enemies.remove_if(deleted);
	enemiesBlue.remove_if(deleted);
	enemiesStar.remove_if(deleted);

	if (player.HitTest(&boss) && level == 3)
	{
		GameOver();
	}
}

void CMyGame::EnemyControl()
{
	// distance between enemy and player less than 6000?
	for (CModel* pEnemy : enemies)
	{
		if (pEnemy->HitTest(player.GetPositionV(), 6000))
		{
			// enemy detects player and flees
			pEnemy->SetDirectionV(player.GetDirectionV());
			pEnemy->SetRotationV(pEnemy->GetDirectionV());
			pEnemy->SetSpeed(1700);
		}
		else
		{
			pEnemy->SetSpeed(700);
		}
	}

	// Test if enemies have hit max distance and wrap around
	for (CModel* pEnemy : enemies)
	{
		if (pEnemy->GetX() > 20000)  pEnemy->SetX(-20000);
		if (pEnemy->GetY() > 20000)  pEnemy->SetY(-20000);
		if (pEnemy->GetZ() > 20000)  pEnemy->SetZ(-20000);

		if (pEnemy->GetX() < -20000)  pEnemy->SetX(20000);
		if (pEnemy->GetY() < -20000)  pEnemy->SetY(20000);
		if (pEnemy->GetZ() < -20000)  pEnemy->SetZ(20000);

	}

	// distance between blue enemies and player less than 3000?
	for (CModel* pEnemyBlue : enemiesBlue)
	{
		if (pEnemyBlue->HitTest(player.GetPositionV(), 3000))
		{
			// enemy detects player and flees
			pEnemyBlue->SetDirectionV(player.GetDirectionV());
			pEnemyBlue->SetRotationV(pEnemyBlue->GetDirectionV());
			pEnemyBlue->SetSpeed(3000);
		}
		else
		{
			pEnemyBlue->SetSpeed(1000);
		}
	}

	// Test if enemies have hit max distance and wrap around
	for (CModel* pEnemyBlue : enemiesBlue)
	{
		if (pEnemyBlue->GetX() > 20000)  pEnemyBlue->SetX(-20000);
		if (pEnemyBlue->GetY() > 20000)  pEnemyBlue->SetY(-20000);
		if (pEnemyBlue->GetZ() > 20000)  pEnemyBlue->SetZ(-20000);

		if (pEnemyBlue->GetX() < -20000)  pEnemyBlue->SetX(20000);
		if (pEnemyBlue->GetY() < -20000)  pEnemyBlue->SetY(20000);
		if (pEnemyBlue->GetZ() < -20000)  pEnemyBlue->SetZ(20000);

	}

	// distance between starry enemies and player less than 8000?
	for (CModel* pEnemyStar : enemiesStar)
	{
		if (pEnemyStar->HitTest(player.GetPositionV(), 8000))
		{
			// enemy detects player and hunts
			pEnemyStar->SetDirectionAndRotationToPoint(player.GetY(), player.GetZ());
			pEnemyStar->SetSpeed(2300);
		}
		else
		{
			pEnemyStar->SetSpeed(1500);
		}
	}

	// Test if enemies have hit max distance and wrap around
	for (CModel* pEnemyStar : enemiesStar)
	{
		if (pEnemyStar->GetX() > 20000)  pEnemyStar->SetX(-20000);
		if (pEnemyStar->GetY() > 20000)  pEnemyStar->SetY(-20000);
		if (pEnemyStar->GetZ() > 20000)  pEnemyStar->SetZ(-20000);

		if (pEnemyStar->GetX() < -20000)  pEnemyStar->SetX(20000);
		if (pEnemyStar->GetY() < -20000)  pEnemyStar->SetY(20000);
		if (pEnemyStar->GetZ() < -20000)  pEnemyStar->SetZ(20000);

	}

}

void CMyGame::BossControl()
{
// Boss controls
	boss.SetHealth(1000);
	boss.SetPosition(-3000, 20, -900);
	boss.SetDirectionV(planet.GetPositionV());
	boss.SetRotationV(boss.GetDirectionV());
	boss.SetSpeed(100);
	   
	if (boss.HitTest(&planet))
	{
		GameOver();
	}

	for (CModel* pAsteroid : asteroids)
	{
		if (boss.HitTest(pAsteroid))
		{
			pAsteroid->Delete();
			explosionSound.Play("explosion.wav");
		}
	}
	asteroids.remove_if(deleted);
}

void CMyGame::ShotsControl()
{
	// collision detection between shots and enemies
	for (CModel* pEnemy : enemies)
	{
		for (CModel* pShot : shots)
		{
			if (pEnemy->HitTest(pShot->GetPositionV()))
			{
				pEnemy->SetHealth(pEnemy->GetHealth() - 20);
				pShot->Delete();
				if (pEnemy->GetHealth() <= 0)
				{
					// Create explosion when shots hit enemy
					CVector pos = WorldToScreenCoordinate(pEnemy->GetPositionV());
					CSprite* pExplosion = new CSprite(pos.x, pos.y, explosion, GetTime());
					// play animation from frame 1 to 16
					pExplosion->PlayAnimation(1, 16, 10, false);
					explosion.Die(350);
					explosions.push_back(pExplosion);

					pEnemy->Delete();
					pShot->Delete();
					score++;

					explosionSound.Play("explosion.wav");
				}
			}
		}
	}

	for (CModel* pEnemyBlue : enemiesBlue)
	{
		for (CModel* pShot : shots)
		{
			if (pEnemyBlue->HitTest(pShot->GetPositionV()))
			{
				pEnemyBlue->SetHealth(pEnemyBlue->GetHealth() - 50);
				pShot->Delete();
				if (pEnemyBlue->GetHealth() <= 0)
				{
					// Create explosion when shots hit enemy
					CVector pos = WorldToScreenCoordinate(pEnemyBlue->GetPositionV());
					CSprite* pExplosion = new CSprite(pos.x, pos.y, explosion, GetTime());
					// play animation from frame 1 to 16
					pExplosion->PlayAnimation(1, 16, 10, false);
					explosion.Die(350);
					explosions.push_back(pExplosion);

					pEnemyBlue->Delete();
					pShot->Delete();
					score++;

					explosionSound.Play("explosion.wav");
				}
			}
		}
	}

	for (CModel* pEnemyStar : enemiesStar)
	{
		for (CModel* pShot : shots)
		{
			if (pEnemyStar->HitTest(pShot->GetPositionV()))
			{
				pEnemyStar->SetHealth(pEnemyStar->GetHealth() - 50);
				pShot->Delete();
				if (pEnemyStar->GetHealth() <= 0)
				{
					// Create explosion when shots hit enemy
					CVector pos = WorldToScreenCoordinate(pEnemyStar->GetPositionV());
					CSprite* pExplosion = new CSprite(pos.x, pos.y, explosion, GetTime());
					// play animation from frame 1 to 16
					pExplosion->PlayAnimation(1, 16, 10, false);
					explosion.Die(350);
					explosions.push_back(pExplosion);

					pEnemyStar->Delete();
					pShot->Delete();
					score++;

					explosionSound.Play("explosion.wav");
				}
			}
		}
	}


	// collision between shots and asteroids
	for (CModel* pAsteroid : asteroids)
	{
		for (CModel* pShot : shots)
		{
			// remove shots when hitting asteroids   
			if (pAsteroid->HitTest(pShot->GetPositionV()))
			{
				CVector pos = WorldToScreenCoordinate(pAsteroid->GetPositionV());
				CSprite* pExplosion = new CSprite(pos.x, pos.y, explosion, GetTime());
				pExplosion->PlayAnimation(1, 16, 10, false);
				explosion.Die(350);
				explosions.push_back(pExplosion);
							   
				pAsteroid->Delete();
				pShot->Delete();
				explosionSound.Play("explosion.wav");
			}
		}
	}

	if (level == 3)
	{
		// Test if shots have hit the boss
		for (CModel* pShot : shots)
		{
			// remove shots when hitting boss   
			if (boss.HitTest(pShot->GetPositionV()))
			{
				boss.SetHealth(boss.GetHealth() - 5);
				pShot->Delete();
				if (boss.GetHealth() <= 0)
				{
					CVector pos = WorldToScreenCoordinate(boss.GetPositionV());
					CSprite* pExplosion = new CSprite(pos.x, pos.y, explosion, GetTime());
					pExplosion->PlayAnimation(1, 16, 10, false);
					explosion.Die(350);
					explosions.push_back(pExplosion);

					boss.Delete();
					pShot->Delete();
					explosionSound.Play("explosion.wav");
					GameOver();
				}
			}
		}
		shots.remove_if(deleted);
	}
	// Test if shots have hit max distance and wrap around
	for (CModel* pShot : shots)
	{
		if (pShot->GetX() > 20000)  pShot->SetX(-20000);
		if (pShot->GetY() > 20000)  pShot->SetY(-20000);
		if (pShot->GetZ() > 20000)  pShot->SetZ(-20000);
	}

	asteroids.remove_if(deleted);	
	enemies.remove_if(deleted);
	enemiesBlue.remove_if(deleted);
	enemiesStar.remove_if(deleted);
	shots.remove_if(deleted);
	explosions.remove_if(deleted);
}

void CMyGame::OnDraw(CGraphics* g)
{
    if (IsMenuMode()) 
    { 
		startScreen.Draw(g);
		return;
	}

	// draw GTEC 3D text
	//font.SetColor( CColor::Blue()); font.SetSize( 40); font.DrawText( Width-200,Height-50, "GTEC 3D" );
	// draw score
	font.SetColor( CColor::White()); font.DrawNumber(10,Height-50, score);
	
	// obtaining the screen coordinates of the player model
	CVector pos = WorldToScreenCoordinate(player.GetPositionV());
	hbar.SetPosition(pos.x, pos.y + 100); // position the health bar 100 pixels above the player in 2D
	hbar.SetHealth(player.GetHealth());
	hbar.Draw(g);

	for (CModel* pEnemy : enemies)
	{
		pos = WorldToScreenCoordinate(pEnemy->GetPositionV());	// set up health bar for AI
		hbar.SetPosition(pos.x, pos.y + 50);
		hbar.SetHealth(pEnemy->GetHealth());

		CVector v1 = pEnemy->GetPositionV() - player.GetPositionV();

		if( v1.Dot(player.GetDirectionV()) > 0) hbar.Draw(g);
	}

	for (CModel* pEnemyBlue : enemiesBlue)
	{
		pos = WorldToScreenCoordinate(pEnemyBlue->GetPositionV());	// set up health bar for AI
		hbar.SetPosition(pos.x, pos.y + 50);
		hbar.SetHealth(pEnemyBlue->GetHealth());

		CVector v2 = pEnemyBlue->GetPositionV() - player.GetPositionV();

		if (v2.Dot(player.GetDirectionV()) > 0) hbar.Draw(g);
	}

	for (CModel* pEnemyStar : enemiesStar)
	{
		pos = WorldToScreenCoordinate(pEnemyStar->GetPositionV());	// set up health bar for AI
		hbar.SetPosition(pos.x, pos.y + 50);
		hbar.SetHealth(pEnemyStar->GetHealth());

		CVector v3 = pEnemyStar->GetPositionV() - player.GetPositionV();

		if (v3.Dot(player.GetDirectionV()) > 0) hbar.Draw(g);
	}

	if (level == 3)
	{
		pos = WorldToScreenCoordinate(boss.GetPositionV());	// set up health bar for AI
		hbar.SetPosition(pos.x, pos.y + 200);
		hbar.SetHealth(boss.GetHealth());

		CVector v3 = boss.GetPositionV() - player.GetPositionV();

		if (v3.Dot(player.GetDirectionV()) > 0) hbar.Draw(g);
	}

	for (CSprite* pExplosion : explosions) pExplosion->Draw(g);

	// draw GAME OVER if game over
   	if (IsGameOver() && player.GetHealth() <= 0)
   	{
		player.Delete();

		font.SetSize(100); font.SetColor( CColor::Red()); font.DrawText(960, 560, "GAME OVER");
	}
	// draw SUCCESS if game won
	else if (IsGameOver() && player.GetHealth() > 0)
	{
		font.SetSize(100); font.SetColor(CColor::Blue()); font.DrawText(960, 560, "YOU WON!");
	}
}


void CMyGame::CameraControl(CGraphics* g)
{
	/*if (IsKeyDown(SDLK_UP)) tilt-=1.0f;
	if (IsKeyDown(SDLK_DOWN)) tilt+=1.0f;*/
	
	// ------ Global Transformation Functions -----
	glTranslatef(0,-300,1000);
	//glRotatef(tilt,1,0,0);			// tilt game world around x axis
    glScalef(zoom,zoom,zoom);		// scale the game world
  
	
    // rotate game world with player and face forward
    glRotatef(-player.GetRotationA().z,1,0,0);	 
    glRotatef(-player.GetRotationA().y + 90.00f,0,1,0);	 
    
    // comment out to avoid y-movement of skybox when player rises
   // glTranslatef( 0, -player.GetY(), 0); 
    
    // draw the skydome before game world is translated
    // it makes the skydome stationary
    skydome.Draw(g); 
    
     // position game world at the player position (excluding skydome)
	glTranslatef(-player.GetX(), -player.GetY(), -player.GetZ()); 
	
	UpdateView();
	Light.Apply();
}

void CMyGame::OnRender3D(CGraphics* g)
{
    CameraControl(g);
    
	// ------- Draw your 3D Models here ----------
	if (level == 3)
	{
	boss.Draw(g);

	}
	player.Draw(g);
	planet.Draw(g);

	for (CModel* pAsteroid : asteroids) pAsteroid->Draw(g);
	for (CModel* pEnemy : enemies) pEnemy->Draw(g);
	for (CModel* pEnemyBlue : enemiesBlue) pEnemyBlue->Draw(g);
	for (CModel* pEnemyStar : enemiesStar) pEnemyStar->Draw(g);
	for (CModel* pShot : shots) pShot->Draw(g);

	
	//ShowBoundingBoxes();
	//ShowCoordinateSystem();
	
}


// called at the start of a new game - display menu here
void CMyGame::OnDisplayMenu()
{
	ShowMouse();
	SDL_WM_GrabInput(SDL_GRAB_OFF);
}

// called when Game Mode entered
void CMyGame::OnStartGame()
{
	level = 1;
	HideMouse();
	SDL_WM_GrabInput(SDL_GRAB_ON);
	OnStartLevel(1);
}

// called when Game is Over
void CMyGame::OnGameOver()
{
	ShowMouse();
	SDL_WM_GrabInput(SDL_GRAB_OFF);
}


// one time termination code
void CMyGame::OnTerminate()
{
	
}

// -------    Keyboard Event Handling ------------

void CMyGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	if (sym == SDLK_F4 && (mod & (KMOD_LALT | KMOD_RALT)))
		StopGame();
	
	if (sym == SDLK_ESCAPE && !IsPaused())
	{
		PauseGame();
		worldMusic.SetVolume(50);
		ShowMouse();
		SDL_WM_GrabInput(SDL_GRAB_OFF);
	}
	else if (IsPaused())
	{
		if (sym == SDLK_ESCAPE)
		{
			ResumeGame();
			worldMusic.SetVolume(100);
			HideMouse();
			SDL_WM_GrabInput(SDL_GRAB_ON);
		}
	}

	if (sym == SDLK_F2)
	{
		NewGame();
		worldMusic.Stop();
		OnDisplayMenu();
	}
	
	if (sym==SDLK_z)
	{
		zoom+=0.1f;
		
	} 
	if (sym==SDLK_x)
	{
		zoom-=0.1f;
		
	} 
}

void CMyGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	
}

// -----  Mouse Events Handlers -------------

void CMyGame::OnMouseMove(Uint16 x, Uint16 y, Sint16 relx, Sint16 rely, bool bLeft, bool bRight, bool bMiddle)
{

	if (!IsMenuMode() && !IsPaused())
	{
		player.Rotate(0, -0.5*relx, -0.5*rely);

		player.SetDirectionV(player.GetRotationV());
	}

	//cout << player.GetDirectionV().x << ' ';         // Testing coords
	//cout << player.GetDirectionV().y << ' ';
	//cout << player.GetDirectionV().z << ' ';
	//cout << endl;
}

void CMyGame::OnLButtonDown(Uint16 x,Uint16 y)
{    
	// start game
	if (IsMenuMode()) StartGame();
	else if(!IsPaused())
	{
		// create a new shot as a short line
		CModel* pShot = new CLine(player.GetPositionV(), 100);
		// rotation and direction same as the player
		pShot->SetDirectionV(player.GetDirectionV());
		pShot->SetRotationV(player.GetRotationV());
		pShot->SetSpeed(5000);
		pShot->Die(2500);
		shotSFX.Play("shot.wav");
		shots.push_back(pShot);
	}
		
}

void CMyGame::OnLButtonUp(Uint16 x,Uint16 y)
{
}

void CMyGame::OnRButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnRButtonUp(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonDown(Uint16 x,Uint16 y)
{

}

void CMyGame::OnMButtonUp(Uint16 x,Uint16 y)
{
}
