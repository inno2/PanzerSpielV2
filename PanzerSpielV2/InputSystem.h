#pragma once
#include "System.h"
#include <Windows.h> 
#include <chrono>
#include "InputKeyState.h"
#include <string>

class InputSystem : public System
{
public:
	// needs to be static
	static LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void CaptureMouseMovement(LPARAM mouse);

	// Geerbt über System
	virtual bool Init() override;
	static std::string GetName();

private:
	WNDCLASSEX wndClass;
	static Key_State m_keys[255];	

	// Geerbt über System
	virtual bool Update(std::chrono::microseconds delta_time) override;

	void SetKeystate(uint32_t vkey, Key_State state);

	bool WasKeyClicked(uint32_t vkey);

	bool IsKeyPressed(uint32_t vkey);

	// Geerbt über System
	virtual void Shutdown() override;
};