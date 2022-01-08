#include "stdafx.h"
#include "Framework.h"
#include "macros.h"

#include "Log.h"

namespace D3D11Framework
{
	// --------------------------------------------------------------

	Framework::Framework(void)
	{
		m_wnd = nullptr;
		m_input = nullptr;
		m_render = nullptr;
		m_init = false;
		Log::Get()->Debug("Framework::Framework()");	
	}

	Framework::~Framework(void)
	{
		Log::Get()->Debug("Framework::~Framework()");
	}

	void Framework::AddInputListener(InputListener* listener)
	{
		if (m_input)
			m_input->AddListener(listener);
	}

	bool Framework::Init(const FrameworkDesc &desc)
	{
		Log::Get()->Debug("Framework::Init()");

		m_render = desc.render;
		m_wnd = new (std::nothrow) Window();
		m_input = new (std::nothrow) InputManager();

		if (!m_wnd || !m_input)
		{
			Log::Get()->Error("Framework::Init(): can't allocate memory for "
				"the window or input manager");
			return false;
		}


		if (!m_wnd->Create(desc.wnd))
		{
			Log::Get()->Error("Framework::Init(): can't create the window");
			return false;
		}

		m_input->Init();
		m_wnd->SetInputManager(m_input);

		if (!m_render->CreateDevice(m_wnd->GetHWND()))
		{
			Log::Get()->Error("Framework::Init(): can't create the render");
			return false;
		}

		m_init = true;
		return true;
	}

	void Framework::Run(void)
	{
		if (m_init)
			while (m_frame());
	}

	void Framework::Close(void)
	{
		Log::Get()->Debug("Framework::Close()");

		m_render->Close();
		_DELETE(m_render);
		//_CLOSE(m_render);
		_CLOSE(m_input);
		_CLOSE(m_wnd);

		m_init = false;
	}

	bool Framework::m_frame(void)
	{
		// handle the window events
		m_wnd->RunEvent();

		// if the window isn't active, we stop frame process
		if (!m_wnd->IsActive())
			return true;

		// if there is an exit, we stop the framework work
		if (m_wnd->IsExit())
			return false;

		// if the window changed its size
		if (m_wnd->IsResize())
		{

		}

		m_render->BeginFrame();
		if (!m_render->Draw())
			return false;
		m_render->EndFrame();

		return true;
	}

	// --------------------------------------------------------------
}