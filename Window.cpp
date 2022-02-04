#include "stdafx.h"

#include "InputManager.h"
#include "Window.h"
#include "Log.h"

namespace D3D11Framework
{
//-------------------------------------------------------------------

	Window* Window::m_instance = nullptr;

	Window::Window(void)
	{
		if (!m_instance)
		{
			m_instance = this;
			m_inputManager = nullptr;
			m_hwnd = NULL;


			m_minimized = false;
			m_maximized = false;
			m_isExit = false;
			m_isResizing = false;
		}
		else
		{
			Log::Get()->Error("Window::Window(): the window has already created");
		}

		Log::Get()->Debug("Window::Window()");
	}

	bool Window::Create(const DescWindow& desc)
	{
		Log::Get()->Debug("Window::Create(): the beginning");

		m_desc = desc;

		// definition of the window class description
		WNDCLASSEX wcd;
		ZeroMemory(&wcd, sizeof(WNDCLASSEX));

		wcd.cbSize = sizeof(WNDCLASSEX);
		
			wcd.style = CS_VREDRAW | CS_HREDRAW;// | CS_DBLCLKS | CS_OWNDC;
		wcd.lpfnWndProc = StaticWinProc;
		wcd.hInstance = NULL;
		wcd.cbWndExtra = NULL;
		wcd.cbClsExtra = NULL;
		wcd.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcd.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wcd.hIconSm = wcd.hIcon;
		wcd.hbrBackground = (HBRUSH)(GetStockObject(COLOR_WINDOW));
		wcd.lpszMenuName = nullptr;
		wcd.lpszClassName = L"D3D11F";
		
		if (!RegisterClassEx(&wcd))
		{
			Log::Get()->Error("Window::Create(): can't register a class for the window");
			return false;
		}

		RECT winRect{ 0, 0, m_desc.width, m_desc.height };
		AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW | WS_VISIBLE, NULL);
	
		m_hwnd = CreateWindowEx(NULL,
								L"D3D11F",
								L"My_Window",
								WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			20,20,
								//m_desc.posx,
								//m_desc.posy,
								640, 480,//winRect.right - winRect.left,
								//winRect.bottom - winRect.top,
								NULL,
								NULL,
								NULL,
								NULL);

		if (!m_hwnd)
		{
			Log::Get()->Error("Window::Create(): can't create the window");
			return false;
		}

		ShowWindow(m_hwnd, SW_SHOW);
		UpdateWindow(m_hwnd);

		Log::Get()->Debug("Window::Window(): the window is created successfully");
		return true;
	}

	void Window::RunEvent()
	{
		MSG msg;

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void Window::Close(void)
	{
		if (m_hwnd)
			DestroyWindow(m_hwnd);
		m_hwnd = NULL;

		Log::Get()->Debug("Window::Close()");
	}

	void Window::SetInputManager(InputManager* inputManager)
	{
		if (inputManager)
			m_inputManager = inputManager;
		Log::Get()->Debug("Window::SetInputManager(): the input manager is set");
	}

	

	LRESULT CALLBACK Window::WinProc(HWND hWnd, const UINT &message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_CREATE:
			return 0;

		case WM_CLOSE:
			m_isExit = true;
			return 0;

		case WM_ACTIVATE:
			if (LOWORD(wParam) == WA_INACTIVE)
				return 0;

			m_desc.posx = LOWORD(lParam);
			m_desc.posy = HIWORD(lParam);
			Log::Get()->Debug("WINDOW POS: %d : %d", m_desc.posx, m_desc.posy);
			m_isActive = true;

			UpdateWindowState();
			return 0;

		case WM_SIZE:
			if (!m_isResizing)
				return 0;

			m_desc.width = LOWORD(lParam);
			m_desc.height = HIWORD(lParam);
			Log::Get()->Debug("WINDOW SIZE: %d : %d", m_desc.width, m_desc.height);
			m_isResizing = true;

			if (wParam == SIZE_MINIMIZED)
			{
				m_minimized = true;
				m_maximized = false;
				m_isActive = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				m_minimized = false;
				m_maximized = true;
				m_isActive = true;
			}
			else if (wParam == SIZE_RESTORED)
			{
				if (m_minimized)
				{
					m_minimized = false;
					m_isActive = true;
				}
				else if (m_maximized)
				{
					m_maximized = false;
					m_isActive = true;
				}
			}

			UpdateWindowState();
			return 0;

		case WM_KEYDOWN: case WM_KEYUP:
		case WM_RBUTTONDOWN: case WM_RBUTTONUP:
		case WM_LBUTTONDOWN: case WM_LBUTTONUP:
		case WM_MBUTTONDOWN: case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
			if (m_inputManager)
				m_inputManager->RunEvent(message, wParam, lParam);
			return 0;
		}

		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	void Window::UpdateWindowState(void)
	{
		RECT clientRect = { m_desc.posx, m_desc.posy, m_desc.width, m_desc.height };
		if (m_inputManager)
			m_inputManager->SetWindowZone(clientRect);
	}
	


//-------------------------------------------------------------------
}