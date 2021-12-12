#pragma once

namespace D3D11Framework
{
// ------------------------------------------------------------------

	class InputManager;

	struct DescWindow
	{
		DescWindow() :
			caption(L""),
			width(640),
			height(480),
			posx(200),
			posy(20),
			resizing(true)
		{}


		int posx;
		int posy;
		int width;
		int height;
		std::wstring caption;
		bool resizing;
	};

	class Window
	{
	public:
		Window(void);
		~Window(void);

		static Window* Get() { return m_wndthis; }

		bool Create(const DescWindow &desc);
		void RunEvent(void);
		void Close(void);

		void SetInputManager(InputManager* inputManager);

		HWND GetHWND(void) const { return m_hwnd; }
		int Window::GetWidth(void) const { return m_desc.width; }
		int Window::GetHeight(void) const { return m_desc.height; }
		int Window::GetLeft(void) const { return m_desc.posx; }
		int Window::GetTop(void) const { return m_desc.posy; }

		const std::wstring& GetCaption(void) const { return m_desc.caption; }

		// if there is an exit, notifies the listeners about it
		bool IsExit(void) { return m_isExit; }
		// if the window changed activation status, notifies the listeners about it
		bool IsActive(void) { return m_isActive; }
		// if the window changed its size, notifies the listener about it
		bool IsResize(void)
		{
			bool ret = m_isResize;
			m_isResize = false;
			return ret;
		}

		LRESULT WinProc(HWND hWnd, const UINT &message, WPARAM wParam, LPARAM lParam);

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

// ------------------------------------------------------------------
}