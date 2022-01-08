#include "stdafx.h"
#include "macros.h"
#include "Render.h"
#include "Log.h"

#include <d3dcompiler.h>


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
		m_pDepthStencil = nullptr;
		m_pDepthStencilView = nullptr;


		Log::Get()->Debug("Render::Render()");
	}

	Render::~Render(void)
	{

	}

	HRESULT Render::m_compileShaderFromFile(WCHAR* Filename, LPCSTR FunctionName,
											LPCSTR ShaderModel, ID3DBlob** ppShaderBlob)
	{
		HRESULT hr = S_OK;

		UINT compileFlags = D3DCOMPILE_WARNINGS_ARE_ERRORS | D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(_DEBUG) || defined(DEBUG)
		compileFlags |= D3DCOMPILE_DEBUG;
#endif

		ID3DBlob* pErrorMsgs = nullptr;

		hr = D3DX11CompileFromFile(L"shader.fx", nullptr, NULL,
									FunctionName, ShaderModel,
									compileFlags, NULL, nullptr,
									ppShaderBlob, &pErrorMsgs, nullptr);
		if (FAILED(hr) && (pErrorMsgs != nullptr))
			OutputDebugStringA((char*)pErrorMsgs->GetBufferPointer());

		_RELEASE(pErrorMsgs);
		return hr;
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

		// Initialization of the depth stencil and depth stencil view
		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory(&descDepth, sizeof(D3D11_TEXTURE2D_DESC));

		descDepth.Width = width;
		descDepth.Height = height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;

		hr = m_pd3dDevice->CreateTexture2D(&descDepth, nullptr, &m_pDepthStencil);
		if (FAILED(hr))
		{
			Log::Get()->Error("Render::CreateDevice(): can't create the depth stencil");
			return false;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;

		hr = m_pd3dDevice->CreateDepthStencilView(m_pDepthStencil, &descDSV, &m_pDepthStencilView);
		if (FAILED(hr))
		{
			Log::Get()->Error("Render::CreateDevice(): can't create the depth stencil view");
			return false;
		}

		m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);



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
		m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void Render::EndFrame(void)
	{
		m_pSwapChain->Present(0, 0);
	}

	void Render::Shutdown()
	{
		Close();

		if (m_pImmediateContext)
			m_pImmediateContext->ClearState();

		_RELEASE(m_pDepthStencil);
		_RELEASE(m_pDepthStencilView);
		_RELEASE(m_pRenderTargetView);
		_RELEASE(m_pSwapChain);
		_RELEASE(m_pImmediateContext);
		_RELEASE(m_pd3dDevice);
	}

// ------------------------------------------------------------------
}


