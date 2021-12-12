#include "stdafx.h"
#include "Window.h"
#include "InputManager.h"
#include "Log.h"

namespace D3D11Framework
{
// ------------------------------------------------------------------

	Window* Window::m_wndthis = nullptr;

	Window::Window(void)
	{
		m_hwnd = nullptr;
		m_inputManager = nullptr;
		m_isExit = false;
		m_isActive = true;
		m_isResize = false;
		m_minimized = false;
		m_maximized = false;
		if (!m_wndthis)
			m_wndthis = this;
		else
			Log::Get()->Error("Window::Window(): has already been created");

		Log::Get()->Debug("Window::Window(): constructor");
	}


	Window::~Window(void)
	{
		Log::Get()->Debug("Window::~Window(): destructor");
	}


	// creation of the window
	bool Window::Create(const DescWindow &desc)
	{
		Log::Get()->Debug("Window::Create(): beginning");

		m_desc = desc;

		WNDCLASSEXW wc;
		ZeroMemory(&wc, sizeof(WNDCLASSEXW));

		wc.cbSize = sizeof(WNDCLASSEXW);
		wc.cbClsExtra = NULL;
		wc.cbWndExtra = NULL;
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = StaticWinProc;
		wc.hInstance = NULL;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wc.hIconSm = wc.hIcon;
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = L"D3D11F";

		if (!RegisterClassExW(&wc))
		{
			Log::Get()->Error("Window::Create(): can't register the window class");
			return false;
		}

		RECT winRect = { 0, 0, m_desc.width, m_desc.height };
		AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW | WS_VISIBLE, FALSE);

		m_hwnd = CreateWindowExW(NULL,
								wc.lpszClassName,
								m_desc.caption.c_str(),
								WS_OVERLAPPEDWINDOW | WS_VISIBLE,
								m_desc.posx,
								m_desc.posy,
								winRect.right - winRect.left,
								winRect.bottom - winRect.top,
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

		return true;
	}

	void Window::RunEvent(void)
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
		m_hwnd = nullptr;

		Log::Get()->Debug("Window::Close()");
	}

	LRESULT Window::WinProc(HWND hWnd, const UINT &message, WPARAM wParam, LPARAM lParam)
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
					m_isActive = false;
				else
					m_isActive = true;
				return 0;
			case WM_MOVE:
				m_desc.posx = LOWORD(lParam);
				m_desc.posy = HIWORD(lParam);

				m_updateWindowState();
				return 0;

			case WM_SIZE:
				if (!m_desc.resizing)
					return 0;

				m_desc.width = LOWORD(lParam);
				m_desc.height = HIWORD(lParam);
				m_isResize = true;

				if (wParam == SIZE_MAXIMIZED)
				{
					m_isActive = true;
					m_minimized = false;
					m_maximized = true;
				}
				else if (wParam == SIZE_MINIMIZED)
				{
					m_isActive = false;
					m_minimized = true;
					m_maximized = false;
				}
				else if (wParam == SIZE_RESTORED)
				{
					if (m_maximized)
					{
						m_isActive = true;
						m_maximized = false;
					}
					else if (m_minimized)
					{
						m_isActive = true;
						m_minimized = false;
					}
				}

				m_updateWindowState();
				return 0;

			case WM_MOUSEMOVE:
			case WM_KEYDOWN: case WM_KEYUP:
			case WM_LBUTTONDOWN: case WM_LBUTTONUP:
			case WM_RBUTTONDOWN: case WM_RBUTTONUP:
			case WM_MBUTTONDOWN: case WM_MBUTTONUP:
			case WM_MOUSEWHEEL:
				if (m_inputManager)
					m_inputManager->Run(message, wParam, lParam);
				return 0;
		}

		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	void Window::SetInputManager(InputManager* inputManager)
	{
		m_inputManager = inputManager;
		m_updateWindowState();
	}

	void Window::m_updateWindowState(void)
	{

		//RECT clientRect;
		//clientRect = { m_desc.posx, m_desc.posy, m_desc.width, m_desc.height };

		RECT clientRect;
		clientRect.left = m_desc.posx;
		clientRect.top = m_desc.posy;
		clientRect.right = m_desc.width;
		clientRect.bottom = m_desc.height;

		if (m_inputManager)
		{
			m_inputManager->SetWinRect(clientRect);
		}
	}

	LRESULT CALLBACK StaticWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return Window::Get()->WinProc(hWnd, message, wParam, lParam);
	}

// ------------------------------------------------------------------
}