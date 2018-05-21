#pragma once
#include <vector>
#include <chrono>

using tick = std::chrono::duration<unsigned long, std::ratio<1, 60>>;

// forward declarations
class System;
class GameObjectFactory;

class Engine
{
public:
	Engine();
	~Engine();

	bool BootUp();	
	void MainLoop();
	void Stop();

private:
	tick m_current_tick;
	bool run_main_loop;
	
	GameObjectFactory* m_GameObjectFactory;
	std::vector<System*> m_systems;
};