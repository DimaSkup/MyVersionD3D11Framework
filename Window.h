#pragma once

#include "InputManager.h"

namespace D3D11Framework
{
//-------------------------------------------------------------------

	struct DescWindow
	{
		DescWindow() 
		{
			caption = L"";
			width = 640;
			height = 480;
			posx = 20;
			posy = 20;
			resize = false;
		}

		std::wstring caption;
		int width;
		int height;
		int posx;
		int posy;
		bool resize;
	};

	class Window
	{
	public:
		Window(void);

		bool Create(const DescWindow& desc);
		void SetInputManager(InputManager* inputManager);
		void Close(void);

		const HWND& GetHWND(void) { return m_hwnd; }

		int GetWidth(void) { return m_desc.width; }
		int GetHeight(void) { return m_desc.height; }
		int GetLeft(void) { return m_desc.posx; }
		int GetTop(void) { return m_desc.posy; }

		const std::wstring& GetCaption(void) { return m_desc.caption; }

		bool isExit(void) { return m_isExit; }
		bool isResizing(void) 
		{
			bool ret = m_isResizing;
			m_isResizing = false;
			return ret;
		}

	private:
		void UpdateWindowState(void);

		HWND m_hwnd;
		InputManager* m_inputManager;
		DescWindow m_desc;

		bool m_maximized;
		bool m_minimized;
		bool m_isExit;
		bool m_isResizing;
	};


//-------------------------------------------------------------------
};