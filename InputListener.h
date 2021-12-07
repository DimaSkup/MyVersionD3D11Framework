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

	// a mouse wheel rotating event
	struct MouseEventWheel : public MouseEvent
	{
		MouseEventWheel(int wh, int nx, int ny) :
			MouseEvent(nx, ny), wheel(wh) {}

		int wheel;
	};

	// a keyboard event
	struct KeyEvent
	{
		KeyEvent(eKeyCodes kc, wchar_t w) :
			code(kc), wc(w) {}

		const eKeyCodes code;
		const wchar_t wc;
	};

	class InputListener
	{
	public:
		virtual bool KeyPressed(const KeyEvent &event) { return false; }
		virtual bool KeyReleased(const KeyEvent &event) { return false; }

		virtual bool MousePressed(const MouseEventClick &event) { return false; }
		virtual bool MouseReleased(const MouseEventClick &event) { return false; }

		virtual bool MouseMove(const MouseEvent &event) { return false; }
		virtual bool MouseWheel(const MouseEventWheel &event) { return false; }
	};


//---------------------------------------------------------------
}
