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

	void InputManager::SetInputListener(InputListener* listener)
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

	}

	void InputManager::eventMouseWheel(short value)
	{

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
				if ((*it)->KeyRelease(KeyEvent(wchar, code)) == true)
					return;
			}
		}
	}


//-------------------------------------------------------------------
}