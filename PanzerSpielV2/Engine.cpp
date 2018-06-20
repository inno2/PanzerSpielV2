#include "Engine.h"
#include "System.h"
#include <chrono>
#include <algorithm>
#include <windows.h>

// Systems
#include "InputSystem.h"

using namespace std::chrono;

Engine* Engine::m_Instance = nullptr;

Engine::Engine()
{
	m_Instance = this;
	m_current_tick.zero();
}

Engine::~Engine()
{
	ShutdownSystems();
	m_Instance = nullptr;
}

bool Engine::BootUp()
{
	RECT window_size = RECT{ 0, 0, 800, 600 };
	DWORD window_style = WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_DLGFRAME;;

	if (!InitWindow(window_size, window_style))
		return false;

	SystemKVP inputsystem;
	inputsystem.name = InputSystem::GetName();
	inputsystem.pointer = new InputSystem();
	
	m_systems.push_back(inputsystem);


	return true;
}

void Engine::MainLoop()
{		
	run_main_loop = true;
	microseconds current_time = duration_cast<microseconds>(steady_clock::now().time_since_epoch());
	
	tick t(1);

	while (run_main_loop)
	{
		microseconds start_time = duration_cast<microseconds>(steady_clock::now().time_since_epoch());
		microseconds delta_time = start_time - current_time;
		current_time = start_time;
		
		microseconds sleepamount = duration_cast<microseconds>(t) /*length of one tick in microseconds*/ - delta_time;
				
		// Update all Systems
		for	each (SystemKVP system in m_systems)
		{
			if (!system.pointer->Update(delta_time))
			{
				// if one of the systems returns false -> stop
				this->Stop();
			}
		}
		
		// Sleep to cap tickrate
		if (sleepamount.count() > 0) 
		{			
			Sleep(duration_cast<milliseconds>(sleepamount).count());
		}		

		m_current_tick++;
	}
}

bool Engine::InitWindow(const RECT& window_size, const DWORD window_style)
{
	RECT client_size = window_size;
	AdjustWindowRect(&client_size, window_style, false);

	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.cbSize = sizeof(wndClass);
	wndClass.hInstance = GetModuleHandle(NULL);
	wndClass.lpfnWndProc = InputSystem::WndProc;
	wndClass.lpszClassName = L"PanzerWindowClass";
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hIconSm = wndClass.hIcon;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);


	if (!RegisterClassEx(&wndClass))
	{
		MessageBoxW(NULL, L"RegisterClassEx Failed!", L"Fatal Error", MB_OK);
		return false;
	}

	// Open the Window in the middle of the screen
	int pos_x = (GetSystemMetrics(SM_CXSCREEN) - (client_size.right - client_size.left)) / 2;
	int pos_y = (GetSystemMetrics(SM_CYSCREEN) - (client_size.bottom - client_size.top)) / 2;

	//hWindow = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT, Class.lpszClassName, Class.lpszClassName, WS_OVERLAPPED |WS_CAPTION | WS_SYSMENU, x, y, width, height, NULL, NULL, Class.hInstance, NULL);
	m_window = CreateWindowEx(WS_EX_APPWINDOW, wndClass.lpszClassName, wndClass.lpszClassName, window_style, pos_x, pos_y, client_size.right - client_size.left, client_size.bottom - client_size.top, NULL, NULL, wndClass.hInstance, NULL);

	if (!m_window)
	{
		MessageBoxW(NULL, L"CreateWindowEx Failed!", L"Fatal Error", MB_OK);
		UnregisterClass(wndClass.lpszClassName, wndClass.hInstance);
		return false;
	}

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_window, SW_SHOW);
	SetForegroundWindow(m_window);
	SetFocus(m_window);

	return true;
}

void Engine::ShutdownSystems()
{
	for	each (SystemKVP system in m_systems)
	{
		system.pointer->Shutdown();
	}
}

void Engine::Stop()
{
	run_main_loop = false;
}

Engine * Engine::GetInstance()
{
	return m_Instance;
}

System * Engine::GetSystemPtr(std::string system_name)
{
	for each (SystemKVP system in m_Instance->m_systems)
	{
		if (system.name == system_name)
		{
			return system.pointer;
		}
	}

	return nullptr;
}
