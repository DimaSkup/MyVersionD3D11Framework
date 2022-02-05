#pragma once

#include "InputManager.h"

namespace D3D11Framework
{
//-------------------------------------------------------------------
	class InputManager;

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
		void RunEvent(void);
		void SetInputManager(InputManager* inputManager);
		void Close(void);

		static Window* Get(void) { return m_instance; }
		const HWND& GetHWND(void) { return m_hwnd; }

		int GetWidth(void) { return m_desc.width; }
		int GetHeight(void) { return m_desc.height; }
		int GetLeft(void) { return m_desc.posx; }
		int GetTop(void) { return m_desc.posy; }

		const std::wstring& GetCaption(void) const { return m_desc.caption; }

		bool IsExit(void) const { return m_isExit; }
		bool IsActive(void) const { return m_isActive; }
		bool IsResizing(void) 
		{
			bool ret = m_isResizing;
			m_isResizing = false;
			return ret;
		}

		LRESULT CALLBACK WinProc(HWND hWnd, const UINT &message, WPARAM wParam, LPARAM lParam);

	private:
		void m_UpdateWindowState(void);

		static Window* m_instance;
		HWND m_hwnd;
		InputManager* m_inputManager;
		DescWindow m_desc;

		bool m_maximized;
		bool m_minimized;
		bool m_isActive;
		bool m_isExit;
		bool m_isResizing;
	};

	static LRESULT CALLBACK StaticWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//-------------------------------------------------------------------
};