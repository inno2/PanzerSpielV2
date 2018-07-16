#include <Windows.h> 
#include "Engine.h"
#include "AssetManager.h"
#include "Test.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	/*
	Engine* engine = new Engine(); 
	engine->BootUp();
	engine->MainLoop();
	

	AssetManager assets;
	assets.LoadObject("alfa147.obj");
	*/

	Test tester;
	Test::result res = tester.Test_EntityManager();
}