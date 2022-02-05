#include "stdafx.h"

#include "macros.h"
#include "Render.h"
#include "Log.h"

namespace D3D11Framework
{
//-------------------------------------------------------------------

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

		Log::Get()->Debug("Render::Render");
	}

	bool Render::CreateDevice(HWND hWnd)
	{
		HRESULT hr = S_OK;

		// get size of the window
		RECT clientRect;
		BOOL isClientRect = GetClientRect(hWnd, &clientRect);
		if (!isClientRect)
		{
			Log::Get()->Error("Render::CreateDevice(): can't get the client rect");
			return false;
		}

		UINT width = clientRect.right - clientRect.left;
		UINT height = clientRect.bottom - clientRect.top;


		// definition of the create device flag
		UINT createDeviceFlag = 0;
#ifdef _DEBUG
		createDeviceFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		// definition of driver types and feature levels
		D3D_DRIVER_TYPE driverTypes[] = {
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_SOFTWARE,
		};

		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		D3D_FEATURE_LEVEL featureLevels[] = {
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
		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;
		scd.OutputWindow = hWnd;
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		scd.Windowed = TRUE;

		
		// creation of the device, device context and swap chain
		// using the swap chain description
		for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
		{
			m_driverType = driverTypes[driverTypeIndex];

			hr = D3D11CreateDeviceAndSwapChain(nullptr,
												m_driverType,
												NULL,
												createDeviceFlag,
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


		// creation of the render target view
		ID3D11Texture2D* pBackBuffer = nullptr;
		hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		if (FAILED(hr))
		{
			_RELEASE(pBackBuffer);
			Log::Get()->Error("Render::CreateDevice(): can't get the swap chain buffer");
			return false;
		}

		hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
		_RELEASE(pBackBuffer);
		if (FAILED(hr))
		{
			Log::Get()->Error("Render::CreateDevice(): can't create the render target view");
			return false;
		}


		// definition of the depth stencil and depth stencil view
		D3D11_TEXTURE2D_DESC depthDesc;
		ZeroMemory(&depthDesc, sizeof(D3D11_TEXTURE2D_DESC));

		depthDesc.Width = width;
		depthDesc.Height = height;
		depthDesc.MipLevels = 1;
		depthDesc.ArraySize = 1;
		depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthDesc.Usage = D3D11_USAGE_DEFAULT;
		depthDesc.SampleDesc.Count = 1;
		depthDesc.SampleDesc.Quality = 0;
		depthDesc.MiscFlags = 0;
		depthDesc.CPUAccessFlags = 0;

		hr = m_pd3dDevice->CreateTexture2D(&depthDesc, nullptr, &m_pDepthStencil);
		if (FAILED(hr))
		{
			Log::Get()->Error("Render::CreateDevice(): can't create the depth stencil 2D texture");
			return false;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

		descDSV.Format = depthDesc.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;

		hr = m_pd3dDevice->CreateDepthStencilView(m_pDepthStencil, nullptr, &m_pDepthStencilView);
		if (FAILED(hr))
		{
			Log::Get()->Error("Render::CreateDevice(): can't create the depth stencil view");
			return false;
		}


		m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);


		// definition of the viewport
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
		float clearColor[] = { 0.2f, 0.4f, 0.6f, 1.0f };
		m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);
		m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void Render::EndFrame(void)
	{
		m_pSwapChain->Present(0, 0);
	}

	void Render::Shutdown(void)
	{
		Close();

		if (m_pImmediateContext)
			m_pImmediateContext->ClearState();

		_RELEASE(m_pDepthStencilView);
		_RELEASE(m_pDepthStencil);

		_RELEASE(m_pRenderTargetView);
		_RELEASE(m_pSwapChain);
		_RELEASE(m_pImmediateContext);
		_RELEASE(m_pd3dDevice);

		Log::Get()->Debug("Render::Shutdown()");
	}


	// *************************************
	//    PROTECTED METHODS DEFINITION
	// *************************************
	HRESULT Render::m_compileShaderFromFile(WCHAR* filename, LPCSTR functionName,
											LPCSTR shaderModel, ID3DBlob** ppShaderBlob)
	{
		HRESULT hr = S_OK;
		ID3DBlob* pErrorMsgs = nullptr;

		UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
		compileFlags |= D3DCOMPILE_DEBUG;
#endif

		hr = D3DX11CompileFromFile(filename, nullptr, NULL,
									functionName, shaderModel,
									compileFlags, NULL, 
									nullptr, ppShaderBlob,
									&pErrorMsgs, nullptr);
		if (FAILED(hr) && (pErrorMsgs != nullptr))
		{
			Log::Get()->Error("Render::m_compileShaderFromFile(): shader \"%S\"; function \"%s\"", filename, functionName);
			Log::Get()->Error("%s", (char*)pErrorMsgs->GetBufferPointer());
		}

		_RELEASE(pErrorMsgs);
		return hr;
	}

//-------------------------------------------------------------------
}