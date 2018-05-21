#include "Engine.h"
#include "System.h"
#include <chrono>
#include <algorithm>
#include <windows.h>

// Systems
#include "InputSystem.h"

using namespace std::chrono;

Engine::Engine()
{
	m_current_tick.zero();
}

bool Engine::BootUp()
{
	InputSystem* inputsystem = new InputSystem();
	
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
		for	each (System* system in m_systems)
		{
			if (!system->Update(delta_time))
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

void Engine::Stop()
{
	run_main_loop = false;
}
