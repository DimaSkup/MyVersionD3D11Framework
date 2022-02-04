#pragma once

#include "InputListener.h"
#include "InputManager.h"
#include "Window.h"

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

		//void SetRender(Render* render);
		void AddListener(InputListener* listener);
		
	private:
		bool m_draw(void);
		Window* m_wnd;
		InputManager* m_input;
		Log log;

		bool m_init;
	};


	//-------------------------------------------------------------------
}