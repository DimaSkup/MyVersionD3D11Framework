#include "stdafx.h"

#include "macros.h"
#include "Log.h"
#include "Framework.h"
#include "InputListener.h"
#include "InputManager.h"
#include "Window.h"

namespace D3D11Framework
{
	//-------------------------------------------------------------------

	Framework::Framework(void)
	{
		m_wnd = nullptr;
		m_input = nullptr;

		m_init = false;

		Log::Get()->Debug("Framework::Framework()");
	}

	bool Framework::Init(void)
	{
		Log::Get()->Debug("Framework::Init()");

		m_wnd = new (std::nothrow) Window();
		m_input = new (std::nothrow) InputManager();

		if (!m_wnd || !m_input)
		{
			Log::Get()->Error("Framework::Init(): can't allocate the memory");
			return false;
		}

		DescWindow desc;

		if (!m_wnd->Create(desc))
		{
			Log::Get()->Error("Framework::Init(): can't create the window");
			return false;
		}

		m_input->Init();
		m_wnd->SetInputManager(m_input);



		/*
		
		if (!m_render->CreateDevice(m_wnd->GetHWND()))
		{
			Log::Get()->Error("Framework::Init(): can't create the render system");
			return false;
		}
		*/
		
		m_init = true;
		return true;
	}

	void Framework::Run(void)
	{
		if (m_init)
			while(true) m_draw();
	}

	void Framework::Close(void)
	{
		_CLOSE(m_wnd);
		_CLOSE(m_input);

		m_init = false;
	}

	void Framework::AddListener(InputListener* listener)
	{
		if (m_input)
			m_input->AddInputListener(listener);
	}

	bool Framework::m_draw(void)
	{
		m_wnd->RunEvent();

		// if the window is inactive we stop the frame rendering
		if (!m_wnd->isActive())
			return false;

		// if there is an exit we stop the engine process
		if (m_wnd->isExit())
			return false;

		/*
		m_render->BeginFrame();
		if (m_render->Draw())
		{
			return true;
		}
		m_render->EndFrame();
		*/

		return false;
	}

	//-------------------------------------------------------------------
}