#pragma once

#include "InputCodes.h"

namespace D3D11Framework
{
//---------------------------------------------------------------

	// a mouse moving event
	struct MouseEvent
	{
		MouseEvent(int nx, int ny) :
			x(nx), y(ny) {}


		int x;
		int y;
	};

	// a mouse click event
	struct MouseEventClick : public MouseEvent
	{
		MouseEventClick(eMouseKeyCodes kc, int nx, int ny) :
			MouseEvent(nx, ny), code(kc) {}

		const eMouseKeyCodes code;
	};

	// a mouse wheel event
	struct MouseEventWheel : public MouseEvent
	{
		MouseEventWheel(short wh, int nx, int ny) :
			MouseEvent(nx, ny), wheel(wh) {}

		int wheel;
	};

	// a keybutton event
	struct KeyEvent
	{
		KeyEvent(eKeyCodes c, wchar_t wch) :
			code(c), wchar(wch) {}

		const eKeyCodes code;
		const wchar_t wchar;
	};

	// an abstract InputListener class
	class InputListener 
	{
	public:
		virtual bool MousePressed(const MouseEventClick &arg) { return true; }
		virtual bool MouseReleased(const MouseEventClick &arg) { return true; }
		
		virtual bool KeyPressed(const KeyEvent &arg) { return true; }
		virtual bool KeyReleased(const KeyEvent &arg) { return true; }

		virtual bool MouseMove(const MouseEvent &arg) { return true; }
		virtual bool MouseWheel(const MouseEventWheel &arg) { return true; }
	};

//---------------------------------------------------------------
}
