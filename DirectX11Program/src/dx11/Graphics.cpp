#include "Graphics.h"
#include "framework.h"
#pragma comment(lib,"d3d11.lib")






Graphics::Graphics(HWND hwnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hwnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;
	HRESULT hr;
	THROW_FAILED( D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext));
	ComPtr< ID3D11Resource> pBackBuffer = nullptr;
	THROW_FAILED(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
	THROW_FAILED(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget));
}

void Graphics::EndFrame()
{
	HRESULT hr;
	if (FAILED(hr = pSwap->Present(1u, 0u))) {
		if (hr == DXGI_ERROR_DEVICE_REMOVED) {

		}
		else {
			THROW_FAILED(hr);
		}
	}
}

void Graphics::ClearBuffer(float red, float green, float blue)
{
	const float color[]{ red,green,blue,1.0f };
	pContext.Get()->ClearRenderTargetView(pTarget.Get(), color);
}

void Graphics::DrawTestTriangle()
{
	HRESULT hr;
	struct Vertex
	{
		float x;
		float y;
	};
	const Vertex vertices[] = {
		{0.0f,0.5f},
		{0.5f,-0.5f},
		{-0.5f,-0.5f}
	};
	ComPtr<ID3D11Buffer> pVertextBuffer;
	D3D11_BUFFER_DESC db = {};
	db.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	db.Usage = D3D11_USAGE_DEFAULT;
	db.CPUAccessFlags = 0u;
	db.MiscFlags = 0u;
	db.ByteWidth = sizeof(vertices);
	db.StructureByteStride = sizeof(Vertex);
	
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;

	THROW_FAILED(pDevice->CreateBuffer(&db, &sd, &pVertextBuffer));
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u, &pVertextBuffer, &stride, &offset);

	THROW_INFO_ONLY( pContext->Draw(3u, 0u));
}

