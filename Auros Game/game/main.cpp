#include "Sprite.h"

#include "GameApp.h"
#include "MyGame.h"


int main(int argc, char* argv[])
{
	CGameApp app;
	CMyGame game;
	
	//app.OpenWindow(1890, 990, "AORUS - created by Michael Slape");
    app.OpenFullScreen();

	app.Run(&game);
	return(0);
}
