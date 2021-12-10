#pragma once

#include "Window.h"
#include "InputManager.h"
#include "Render.h"

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
		bool m_init;
	};

// ------------------------------------------------------------------
}