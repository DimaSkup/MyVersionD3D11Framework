#pragma once

#include "InputCodes.h"

namespace D3D11Framework
{
//-------------------------------------------------------------------

	struct MouseEvent
	{
		MouseEvent(int nx, int ny) :
			x(nx), y(ny) {};

		int x;
		int y;
	};

	struct MouseClickEvent : public MouseEvent
	{
		MouseClickEvent(const eMouseKeyCodes nbtn, int nx, int ny) :
			button(nbtn), MouseEvent(nx, ny) {}

		const eMouseKeyCodes button;
	};

	struct MouseWheelEvent : public MouseEvent
	{
		MouseWheelEvent(short wh, int nx, int ny) :
			wheel(wh), MouseEvent(nx, ny) {}

		short wheel;
	};

	struct KeyEvent
	{
		KeyEvent(const wchar_t wc, const eKeyCodes c) :
			wch(wc), code(c) {}

		const wchar_t wch;
		const eKeyCodes code;
	};

	class InputListener
	{
	public:
		virtual bool MousePressed(const MouseClickEvent& arg) { return false; }
		virtual bool MouseReleased(const MouseClickEvent& arg) { return false; }

		virtual bool KeyPressed(const KeyEvent& arg) { return false; }
		virtual bool KeyReleased(const KeyEvent& arg) { return false; }

		virtual bool MouseMove(const MouseEvent& arg) { return false; }
		virtual bool MouseWheel(const MouseWheelEvent& arg) { return false; }
	};

//-------------------------------------------------------------------
}