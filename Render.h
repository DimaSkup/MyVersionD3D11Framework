#pragma once

namespace D3D11Framework
{
	class Render
	{
	public:
		Render();
		virtual ~Render();

		bool CreateDevice(HWND hWnd);
		void BeginFrame();
		void EndFrame();

		virtual bool Init(HWND hWnd) = 0;
		virtual bool Draw(void) = 0;
		virtual void Close(void) = 0;

	protected:
		D3D_DRIVER_TYPE m_driverType;
		D3D_FEATURE_LEVEL m_featureLevel;
		ID3D11Device* m_pd3dDevice;
		ID3D11DeviceContext* m_pImmediateContext;
		IDXGISwapChain* m_pSwapChain;
		ID3D11RenderTargetView* m_pRenderTargetView;
	};
}