#pragma once

#include "InputCodes.h"
#include "InputListener.h"

namespace D3D11Framework
{
//-------------------------------------------------------------------

	//class InputListener; // ????

	class InputManager
	{
	public:
		void Init(void);
		void Close(void);

		void AddInputListener(InputListener* listener);
		void SetWindowZone(const RECT& winRect);
		void RunEvent(const UINT message, WPARAM wParam, LPARAM lParam);

	private:
		void eventCursor(void);
		void eventMouseClick(const eMouseKeyCodes kcode, bool press);
		void eventMouseWheel(short value);
		void eventKeyButton(const wchar_t wc, const eKeyCodes kcode, bool press);

		std::list<InputListener*> m_listener;

		RECT m_winRect;
		int m_curx;
		int m_cury;
		int m_mouseWheel;
	};

//-------------------------------------------------------------------
}