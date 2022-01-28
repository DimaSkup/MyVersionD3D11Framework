// last revising at 28.01.22

#pragma once

namespace D3D11Framework
{
//-------------------------------------------------------------------

	class Render
	{
	public:
		Render(void);

		bool CreateDevice(HWND hWnd);
		void BeginFrame(void);
		void EndFrame(void);
		void Shutdown(void);

		virtual bool Init(HWND hWnd) = 0;
		virtual bool Draw(void) = 0;
		virtual void Close(void) = 0;

	protected:
		HRESULT m_compileShaderFromFile(WCHAR* filename, LPCSTR functionName, 
										LPCSTR shaderModel, ID3DBlob** ppShaderBlob);

		D3D_DRIVER_TYPE m_driverType;
		D3D_FEATURE_LEVEL m_featureLevel;

		ID3D11Device* m_pd3dDevice;
		ID3D11DeviceContext* m_pImmediateContext;
		IDXGISwapChain* m_pSwapChain;
		ID3D11RenderTargetView* m_pRenderTargetView;

		ID3D11Texture2D* m_pDepthStencil;
		ID3D11DepthStencilView* m_pDepthStencilView;
	};

//-------------------------------------------------------------------
}
