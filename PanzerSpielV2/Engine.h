#pragma once
#include <vector>
#include <chrono>
#include <Windows.h>
#include <string>
#include <utility>
	

using tick = std::chrono::duration<unsigned long, std::ratio<1, 60>>;

// forward declarations
class System;
class GameObjectFactory;

struct SystemKVP
{
	std::string name;
	System* pointer;
};

class Engine
{
public:
	Engine();	
	~Engine();

	bool BootUp();	
	void MainLoop();	
	void Stop();

	static Engine* GetInstance();
	static System* GetSystemPtr(std::string system_name);

private:
	bool InitWindow(const RECT& window_size, const DWORD window_style);
	void ShutdownSystems();

private:
	static Engine* m_Instance;
	HWND m_window;
	
	tick m_current_tick;
	bool run_main_loop;	

	GameObjectFactory* m_GameObjectFactory;
	std::vector<SystemKVP> m_systems;
};

