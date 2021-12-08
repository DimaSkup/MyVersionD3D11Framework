#include "stdafx.h"
#include "Window.h"
#include "InputManager.h"
#include "Log.h"

namespace D3D11Framework
{
	Window* m_wndthis = nullptr;

	Window::Window(void)
	{
		m_wndthis = this;
		m_hwnd = nullptr;
		m_inputManager = nullptr;
		m_isExit = false;
		m_isActive = true;
		m_isResize = false;
		m_minimized = false;
		m_maximized = true;
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

}