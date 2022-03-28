#pragma once

#include "d3d11.h"
#include "DxgiInfoMgr.h"
#include <wrl.h>
using namespace Microsoft::WRL;

class Graphics {

public:
	Graphics(HWND hwnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;
	void EndFrame();
	void ClearBuffer(float red, float green, float blue);
	void DrawTestTriangle();
private:
#ifndef NDEBUG
	DxgiInfoMgr infoManager;
#endif
	ComPtr <ID3D11Device> pDevice = nullptr;
	ComPtr < IDXGISwapChain> pSwap = nullptr;
	ComPtr < ID3D11DeviceContext> pContext = nullptr;
	ComPtr < ID3D11RenderTargetView> pTarget = nullptr;
};