#pragma once

#include "InputListener.h"
#include "InputManager.h"
#include "Window.h"
#include "Render.h"

namespace D3D11Framework
{
	//-------------------------------------------------------------------

	class Framework
	{
	public:
		Framework(void);

		bool Init(void);
		void Run(void);
		void Close();

		void SetRender(Render* render) { m_render = render; };
		void AddInputListener(InputListener* listener);
		
	protected:
		bool m_frame(void);
		Window* m_wnd;
		InputManager* m_input;
		Render* m_render;
		Log log;

		bool m_init;
	};


	//-------------------------------------------------------------------
}