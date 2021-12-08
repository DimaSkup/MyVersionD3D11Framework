#pragma once

namespace D3D11Framework
{
	class InputManager;

	struct DescWindow
	{
		int posx;
		int posy;
		int width;
		int height;
		std::wstring caption;
	};

	class Window
	{
	public:
		Window(void);
		~Window(void);

		static Window* Get() { return m_wndthis; }

		bool Create(const DescWindow &desc);
		void Run(void);
		void Close(void);

		void SetInputManager(InputManager* inputManager);

		HWND GetHWND(void) const { return m_hwnd; }
		int GetWidth(void) const { return m_desc.width; }
		int GetHeight(void) const { return m_desc.height; }
		int GetLeft(void) const { return m_desc.posx; }
		int GetTop(void) const { return m_desc.posy; }

		const std::wstring& GetCaption(void) const { return m_desc.caption; }

		// if there is an exit, notifies the listeners about it
		bool IsExit(void) { return m_isExit; }
		// if the window changed activation status, notifies the listeners about it
		bool IsActive(void) { return m_isActive; }
		// if the window changed its size, notifies the listener about it
		bool IsResize(void)
		{
			bool temp = m_isResize;
			m_isResize = false;
			return temp;
		}

		LRESULT WinProc(const UINT &message, WPARAM wParam, LPARAM lParam);

	private:
		void m_updateWindowState(void);

		static Window* m_wndthis;

		InputManager* m_inputManager;
		HWND m_hwnd;
		DescWindow m_desc;

		bool m_isExit;
		bool m_isActive;
		bool m_isResize;
		bool m_maximized;
		bool m_minimized;
	};

	static LRESULT CALLBACK StaticWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
}