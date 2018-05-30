#pragma once
#include "System.h"
#include <Windows.h> 
#include <chrono>
#include "InputKeyState.h"


class InputSystem : public System
{
public:
	static LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); // needs to be static
	bool InitWindow(RECT client_size, DWORD window_style);

private:
	WNDCLASSEX wndClass;
	HWND m_windowHandle;
	static Key_State m_keys[255];

	// Geerbt �ber System
	virtual bool Update(std::chrono::microseconds delta_time) override;
};