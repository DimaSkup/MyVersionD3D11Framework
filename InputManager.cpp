#include "stdafx.h"

#include "InputManager.h"
#include "Log.h"

namespace D3D11Framework
{
//-------------------------------------------------------------------

	void InputManager::Init(void)
	{
		m_curx = m_cury = m_mouseWheel = 0;
		Log::Get()->Debug("InputManager::Init()");
	}

	void InputManager::Close(void)
	{
		if (!m_listener.empty())
			m_listener.clear();

		Log::Get()->Debug("InputManager::Close()");
	}

	void InputManager::AddInputListener(InputListener* listener)
	{
		m_listener.push_back(listener);
	}

	void InputManager::SetWindowZone(const RECT& rect)
	{
		m_winRect = rect;

		m_winRect.left = rect.left;
		m_winRect.top = rect.top;
		m_winRect.right = rect.right;
		m_winRect.bottom = rect.bottom;
	}

	void InputManager::RunEvent(const UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (m_listener.empty())
		{
			Log::Get()->Error("InputManager::RunEvent(): there is no listener yet");
			return;
		}

		eventCursor();

		eKeyCodes kCode;
		wchar_t buffer[1];
		BYTE lpKeyboardState[256];

		switch (message)
		{
			case WM_KEYDOWN:
				kCode = static_cast<eKeyCodes>(wParam);
				GetKeyboardState(lpKeyboardState);
				ToUnicode(wParam,
						  HIWORD(lParam & 0xFF),
						  lpKeyboardState,
						  buffer,
						  1,
						  0);
				eventKeyButton(buffer[0], kCode, true);
				break;
			case WM_KEYUP:
				kCode = static_cast<eKeyCodes>(wParam);
				GetKeyboardState(lpKeyboardState);
				ToUnicode(wParam,
						  HIWORD(lParam & 0xFF),
						  lpKeyboardState,
						  buffer,
						  1,
						  0);
				eventKeyButton(buffer[0], kCode, false);
				break;

			case WM_LBUTTONDOWN:
				eventMouseClick(MOUSE_LEFT, true);
				break;
			case WM_LBUTTONUP:
				eventMouseClick(MOUSE_LEFT, false);
				break;

			case WM_RBUTTONDOWN:
				eventMouseClick(MOUSE_RIGHT, true);
				break;
			case WM_RBUTTONUP:
				eventMouseClick(MOUSE_RIGHT, false);
				break;

			case WM_MBUTTONDOWN:
				eventMouseClick(MOUSE_MIDDLE, true);
				break;
			case WM_MBUTTONUP:
				eventMouseClick(MOUSE_MIDDLE, false);
				break;

			case WM_MOUSEWHEEL:
				eventMouseWheel(static_cast<short>(GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA));
				break;
			

		}  // switch
	} // RunEvent()

	void InputManager::eventCursor(void)
	{
		POINT pos;	// position of the cursor
		GetCursorPos(&pos);


		if ((m_curx == pos.x) && (m_cury == pos.y))
			return;

		m_curx = pos.x - m_winRect.left;
		m_cury = pos.y - m_winRect.top;

		for (auto it = m_listener.begin(); it != m_listener.end(); it++)
		{
			if (!(*it))
				continue;

			if ((*it)->MouseMove(MouseEvent(m_curx, m_cury)) == true)
				return;
		}
	}

	void InputManager::eventMouseClick(const eMouseKeyCodes code, bool press)
	{
		for (auto it = m_listener.begin(); it != m_listener.end(); it++)
		{
			if (!(*it))
				continue;

			// if some mouse button is pressed
			if (press)
			{
				if ((*it)->MousePressed(MouseClickEvent(code, m_curx, m_cury)) == true)
					return;
			}
			// some mouse button is released
			else
			{
				if ((*it)->MouseReleased(MouseClickEvent(code, m_curx, m_cury)) == true)
					return;
			}
		}
	}

	void InputManager::eventMouseWheel(short value)
	{
		if (m_mouseWheel == value)
			return;

		m_mouseWheel = value;

		for (auto it = m_listener.begin(); it != m_listener.end(); it++)
		{
			if (!(*it))
				continue;

			if ((*it)->MouseWheel(MouseWheelEvent(m_mouseWheel, m_curx, m_cury)) == true)
				return;
		}
	}

	void InputManager::eventKeyButton(const wchar_t wchar, const eKeyCodes code, bool press)
	{
		for (auto it = m_listener.begin(); it != m_listener.end(); it++)
		{
			if (!(*it))
				continue;

			// some keybutton is pressed
			if (press == true)
			{
				if ((*it)->KeyPressed(KeyEvent(wchar, code)) == true)
					return;
			}
			// some keybutton is released
			else
			{
				if ((*it)->KeyReleased(KeyEvent(wchar, code)) == true)
					return;
			}
		}
	}


//-------------------------------------------------------------------
}