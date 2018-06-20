#include "InputSystem.h"
#include "GameObjectFactory.h"                   
#include "InputComponent.h"
#include "Engine.h"

using namespace std::chrono;

Key_State InputSystem::m_keys[];

LRESULT InputSystem::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static InputSystem* inputsys = reinterpret_cast<InputSystem*>(Engine::GetSystemPtr(InputSystem::GetName()));

	switch (msg)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_PAINT:
		// this is necessary even if we dont use it
		PAINTSTRUCT ps;
		BeginPaint(hwnd, &ps);
		// nothing here 
		EndPaint(hwnd, &ps);
		break;
	case WM_CREATE:
		break;
	case WM_ERASEBKGND:
		return TRUE;
	case WM_SIZE:
		break;
	case WM_MOUSEMOVE:
		//inputsys->CaptureMouseMovement(lParam);
		break;
		// Inputs
	case WM_LBUTTONDOWN:
	case WM_KEYDOWN:
		inputsys->SetKeystate(wParam, Key_State::down);	
		break;
	case WM_LBUTTONUP: // for some reason wParam isnt the vk code in this case
		inputsys->SetKeystate(VK_LBUTTON, Key_State::clicked);
		break;
	case WM_KEYUP:
		inputsys->SetKeystate(wParam, Key_State::up);	
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}



bool InputSystem::Update(microseconds deltatime)
{
	// Dispatch all messages in Queue
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return false;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}	
	}
}

void InputSystem::SetKeystate(uint32_t vkey, Key_State state)
{
	if (vkey > 0 && vkey < 255)
	{
		m_keys[vkey] = state;
	}
}

bool InputSystem::WasKeyClicked(uint32_t vkey)
{
	if (m_keys[vkey] == Key_State::clicked)
	{
		m_keys[vkey] = Key_State::up;
		return true;
	}
	return false;
}

bool InputSystem::IsKeyPressed(uint32_t vkey)
{
	return m_keys[vkey] == Key_State::down;
}

void InputSystem::Shutdown()
{
}

bool InputSystem::Init()
{
	return true;
}

std::string InputSystem::GetName()
{
	return "InputSystem";
}
