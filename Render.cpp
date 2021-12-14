#include "stdafx.h"
#include "macros.h"
#include "Render.h"
#include "Log.h"


namespace D3D11Framework
{
// ------------------------------------------------------------------

	Render::Render(void)
	{
		m_driverType = D3D_DRIVER_TYPE_NULL;
		m_featureLevel = D3D_FEATURE_LEVEL_11_0;
		m_pd3dDevice = nullptr;
		m_pImmediateContext = nullptr;
		m_pSwapChain = nullptr;
		m_pRenderTargetView = nullptr;
		Log::Get()->Debug("Render::Render()");
	}

	Render::~Render(void)
	{

	}

	bool Render::CreateDevice(HWND hWnd)
	{
		Log::Get()->Debug("Render::CreateDevice()");
		HRESULT hr = S_OK;

		RECT windowRect;
		BOOL isClientRect = GetClientRect(hWnd, &windowRect);

		if (isClientRect == FALSE)
		{
			Log::Get()->Error("Render::CreateDevice(): can't get client rect data");
			return false;
		}

		UINT width = windowRect.right - windowRect.left;
		UINT height = windowRect.bottom - windowRect.top;


		UINT createDeviceFlags = 0;
#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_SOFTWARE,
		};

		UINT numDriverTypes = ARRAYSIZE(driverTypes);


		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};

		UINT numFeatureLevels = ARRAYSIZE(featureLevels);


		// definition of the swap chain description
		DXGI_SWAP_CHAIN_DESC scd;
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

		scd.BufferCount = 1;
		scd.BufferDesc.Width = width;
		scd.BufferDesc.Height = height;
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.OutputWindow = hWnd;
		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;
		//scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		scd.Windowed = TRUE;

		// creation of the device, device context and swap chain 
		// using the swap chain description
		for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
		{
			m_driverType = driverTypes[driverTypeIndex];

			hr = D3D11CreateDeviceAndSwapChain(NULL, 
												m_driverType,
												NULL,
												createDeviceFlags,
												featureLevels,
												numFeatureLevels,
												D3D11_SDK_VERSION,
												&scd,
												&m_pSwapChain,
												&m_pd3dDevice,
												&m_featureLevel,
												&m_pImmediateContext);

			if (SUCCEEDED(hr))
				break;
		}

		if (FAILED(hr))
		{
			Log::Get()->Error("Render::CreateDevice(): can't create the swap chain");
			return false;
		}


		// Initialization of the render target view
		ID3D11Texture2D* pBackBuffer = nullptr;
		hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		if (FAILED(hr))
		{
			_RELEASE(pBackBuffer);
			Log::Get()->Error("Render::CreateDevice(): can't get buffer from the swap chain");
			return false;
		}

		hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
		_RELEASE(pBackBuffer);
		if (FAILED(hr))
		{
			Log::Get()->Error("Render::CreateDevice(): can't create the render target view");
			return false;
		}

		m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);



		// Set up the viewport
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.Width = width;
		viewport.Height = height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;

		m_pImmediateContext->RSSetViewports(1, &viewport);
		

		Log::Get()->Debug("Render::CreateDevice(): the device is created successfully");
		return Init(hWnd);
	}

	void Render::BeginFrame(void)
	{
		FLOAT clearColor[] = { 0.2f, 0.4f, 0.6f, 1.0f };
		m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);
	}

	void Render::EndFrame(void)
	{
		m_pSwapChain->Present(0, 0);
	}

// ------------------------------------------------------------------
}