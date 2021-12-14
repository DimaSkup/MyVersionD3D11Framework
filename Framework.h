#pragma once

#include "Window.h"
#include "Render.h"
#include "InputManager.h"
#include "Log.h"


namespace D3D11Framework
{
// ------------------------------------------------------------------

	class Framework
	{
	public:
		Framework(void);
		~Framework(void);

		bool Init(void);
		void Run(void);
		void Close(void);

		void AddInputListener(InputListener* listener);
		void SetRender(Render* render);

	protected:
		bool m_frame(void);

		Window* m_wnd;
		InputManager* m_input;
		Render* m_render;
		Log m_log;
		bool m_init;
	};

// ------------------------------------------------------------------
}