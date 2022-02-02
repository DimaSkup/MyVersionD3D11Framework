#include "stdafx.h"

#include "InputManager.h"
#include "Window.h"
#include "Log.h"

namespace D3D11Framework
{
//-------------------------------------------------------------------

	Window::Window(void)
	{
		m_inputManager = nullptr;
		m_hwnd = NULL;

		m_minimized = false;
		m_maximized = false;
		m_isExit = false;
		m_isResizing = false;

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
		wcd.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_OWNDC;
	}


//-------------------------------------------------------------------
}