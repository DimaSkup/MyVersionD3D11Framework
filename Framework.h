#pragma once

#include "Window.h"
#include "Render.h"
#include "InputManager.h"
#include "Log.h"


namespace D3D11Framework
{
// ------------------------------------------------------------------

	struct FrameworkDesc
	{
		DescWindow wnd;
		Render* render;
	};

	class Framework
	{
	public:
		Framework(void);
		~Framework(void);

		bool Init(const FrameworkDesc &desc);
		void Run(void);
		void Close(void);

		void AddInputListener(InputListener* listener);

	protected:
		bool m_frame(void);

		FrameworkDesc m_desc;
		Window* m_wnd;
		InputManager* m_input;
		Render* m_render;
		Log m_log;
		bool m_init;
	};

// ------------------------------------------------------------------
}