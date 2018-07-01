#include <Windows.h> 
#include "Engine.h"
#include "EntityManager.h"

#include "Movement_Component.h"
#include "Transformation_Component.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	//Engine* engine = new Engine(); 
	//engine->BootUp();
	//engine->MainLoop();

	EntityManager em = EntityManager(100);


}