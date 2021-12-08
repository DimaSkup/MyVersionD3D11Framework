#pragma once

#include "InputCodes.h"
#include "InputListener.h"

namespace D3D11Framework
{
//---------------------------------------------------------------

	class InputListener;

	class InputManager
	{
	public:
		void Init(void);	// initialization 
		void Close(void);

		// handle the event from the window
		void Run(const UINT &message, WPARAM wParam, LPARAM lParam);

		void SetWinRect(const RECT &rect);	// set the window zone
		void AddListener(InputListener* listener);		

	private:
		void m_eventMouse(void);											// a mouse move event
		void m_eventMouseClick(const eMouseKeyCodes code, bool press);		// a mouse button click event
		void m_eventMouseWheel(short value);								// a mouse wheel event
		void m_eventKey(const eKeyCodes c, const wchar_t wch, bool press);	// a keybutton event

		std::list<InputListener*> m_Listener;

		RECT m_wndrect;
		int m_curx;
		int m_cury;
		int m_mouseWheel;
	};

//---------------------------------------------------------------
}