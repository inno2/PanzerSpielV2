#include "InputSystem.h"
#include "GameObjectFactory.h"                   
#include "InputComponent.h"

using namespace std::chrono;

LRESULT InputSystem::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
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
		//inputsystem->CaptureMouseMovement(lParam);
		break;
		// Inputs
	case WM_LBUTTONDOWN:
	case WM_KEYDOWN:
		if (wParam > 0 && wParam < 255)
		{
			InputSystem::m_keys[wParam] = Key_State::down;
		}
		break;
	case WM_LBUTTONUP: // for some reason wParam isnt the vk code in this case
		//inputsystem->SetKeystate(VK_LBUTTON, keystate::clicked);		
		InputSystem::m_keys[VK_LBUTTON] = Key_State::clicked;		
		break;
	case WM_KEYUP:
		if (wParam > 0 && wParam < 255)
		{
			InputSystem::m_keys[wParam] = Key_State::up;
		}
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}



bool InputSystem::InitWindow(RECT client_size, DWORD window_style)
{
	AdjustWindowRect(&client_size, window_style, false);

	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.cbSize = sizeof(wndClass);
	wndClass.hInstance = GetModuleHandle(NULL);
	wndClass.lpfnWndProc = WndProc;
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
	m_windowHandle = CreateWindowEx(WS_EX_APPWINDOW, wndClass.lpszClassName, wndClass.lpszClassName, window_style, pos_x, pos_y, client_size.right - client_size.left, client_size.bottom - client_size.top, NULL, NULL, wndClass.hInstance, NULL);

	if (!m_windowHandle)
	{
		MessageBoxW(NULL, L"CreateWindowEx Failed!", L"Fatal Error", MB_OK);
		UnregisterClass(wndClass.lpszClassName, wndClass.hInstance);
		return false;
	}

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_windowHandle, SW_SHOW);
	SetForegroundWindow(m_windowHandle);
	SetFocus(m_windowHandle);

	return true;
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

	for each (Input_Component inputcomp in GameObjectFactory::m_InputCompManager.GetAllComponents())
	{
		inputcomp.status = m_keys[inputcomp.key];
	}
}
