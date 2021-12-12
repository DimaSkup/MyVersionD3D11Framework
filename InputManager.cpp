#include "stdafx.h"

#include "InputManager.h"
#include "InputCodes.h"
#include "InputListener.h"
#include "Log.h"

namespace D3D11Framework
{
//---------------------------------------------------------------

	void InputManager::Init(void)
	{
		m_curx = m_cury = m_mouseWheel = 0;
		Log::Get()->Debug("InputManager::Init()");
	}

	void InputManager::Close(void)
	{
		if (!m_Listener.empty())
			m_Listener.clear();

		Log::Get()->Debug("InputManager::Close()");
	}

	void InputManager::SetWinRect(const RECT &rect)
	{
		m_wndrect.left = rect.left;
		m_wndrect.top = rect.top;
		m_wndrect.right = rect.right;
		m_wndrect.bottom = rect.bottom;
	}

	void InputManager::AddListener(InputListener* listener)
	{
		m_Listener.push_back(listener);
	}

	void InputManager::Run(const UINT &message, WPARAM wParam, LPARAM lParam)
	{
		if (m_Listener.empty())
		{
			Log::Get()->Error("InputManager::Run(): there is no listener yet!");
			return;
		}

		eKeyCodes keyCode;
		wchar_t symbol[1];
		BYTE lpKeyState[256];

		m_eventMouse();		// handle the mouse moving

		switch (message)
		{
			// keybutton events handling
			case WM_KEYDOWN:
				keyCode = static_cast<eKeyCodes>(wParam);
				GetKeyboardState(lpKeyState);
				ToUnicode(wParam,
							HIWORD(lParam) & 0xFF,
							lpKeyState,
							symbol,
							1,
							0);
				m_eventKey(keyCode, symbol[0], true);
				break;
			case WM_KEYUP:
				keyCode = static_cast<eKeyCodes>(wParam);
				GetKeyboardState(lpKeyState);
				ToUnicode(wParam,
							HIWORD(lParam) & 0xFF,
							lpKeyState,
							symbol,
							1,
							0);
				m_eventKey(keyCode, symbol[0], false);
				break;

			// mouse buttons events handling
			case WM_LBUTTONDOWN:
				m_eventMouseClick(MOUSE_LEFT, true);
				break;
			case WM_LBUTTONUP:
				m_eventMouseClick(MOUSE_LEFT, false);
				break;

			case WM_RBUTTONDOWN:
				m_eventMouseClick(MOUSE_RIGHT, true);
				break;
			case WM_RBUTTONUP:
				m_eventMouseClick(MOUSE_RIGHT, false);
				break;

			case WM_MBUTTONDOWN:
				m_eventMouseClick(MOUSE_MIDDLE, true);
				break;
			case WM_MBUTTONUP:
				m_eventMouseClick(MOUSE_MIDDLE, false);
				break;

			// mouse wheel events handling
			case WM_MOUSEWHEEL:
				m_eventMouseWheel(static_cast<short>(GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA));
				break;
		}
	}

	// mouse moving handling
	void InputManager::m_eventMouse(void)
	{
		POINT pos;
		GetCursorPos(&pos);

		pos.x -= m_wndrect.left;
		pos.y -= m_wndrect.top;

		if (m_curx == pos.x && m_cury == pos.y)
			return;

		m_curx = pos.x;
		m_cury = pos.y;

		for (auto it = m_Listener.begin(); it != m_Listener.end(); ++it)
		{
			if (!(*it))
				continue;
			else if ((*it)->MouseMove(MouseEvent(m_curx, m_cury)) == true)
				return;
		}
	}

	// mouse click events handling
	void InputManager::m_eventMouseClick(const eMouseKeyCodes code, bool press)
	{
		for (auto it = m_Listener.begin(); it != m_Listener.end(); ++it)
		{
			if (!(*it))
				continue;
			
			// a mouse button is pressed
			if (press == true)
			{
				if ((*it)->MousePressed(MouseEventClick(code, m_curx, m_cury)) == true)
					return;
			}
			// a mouse button is released
			else 
			{
				if ((*it)->MouseReleased(MouseEventClick(code, m_curx, m_cury)) == true)
					return;
			}
		}
	}


	// mouse wheel events handling
	void InputManager::m_eventMouseWheel(short value)
	{
		// if we rotate the wheel in the same direction as before
		if (value == m_mouseWheel)		
			return;

		m_mouseWheel = value;

		for (auto it = m_Listener.begin(); it != m_Listener.end(); ++it)
		{
			if (!(*it))
				continue;
			else if ((*it)->MouseWheel(MouseEventWheel(value, m_curx, m_cury)) == true)
				return;
		}
	}

	// keybutton events handling
	void InputManager::m_eventKey(const eKeyCodes code, wchar_t wchar, bool press)
	{
		for (auto it = m_Listener.begin(); it != m_Listener.end(); ++it)
		{
			if (!(*it))
				continue;

			// if a keybutton is pressed
			if (press == true)
			{
				if ((*it)->KeyPressed(KeyEvent(code, wchar)) == true)
					return;
			}
			// a keybutton is released
			else
			{
				if ((*it)->KeyReleased(KeyEvent(code, wchar)) == true)
					return;
			}

		}
	}

//---------------------------------------------------------------
}