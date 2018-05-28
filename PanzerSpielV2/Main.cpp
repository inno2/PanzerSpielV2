#include <Windows.h> 
#include "Engine.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	Engine* engine = new Engine(); 
	engine->BootUp();
	engine->MainLoop();
}