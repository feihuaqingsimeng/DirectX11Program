#include "Graphics.h"
#include "framework.h"
#include <d3dcompiler.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")





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
		struct
		{
			float x;
			float y;
		} pos;
		struct
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		} color;
		
	};
	 Vertex vertices[] = {
		{0.0f,0.5f,255,0.0f,0.0f},
		{0.5f,-0.5f,0.0f,255,0.0f},
		{-0.5f,-0.5f,0.0f,0.0f,255},
		{-0.3f,0.3f,0,255,0,0},
		{0.3f,0.3f,0,0,255,0},
		{0.0f,-0.8f,255,0,0,0},
	};
	 const unsigned short indices[] = {
		 0,1,2,
		 0,2,3,
		 0,4,1,
		 2,1,5,
	 };
	//vertex buffer
	ComPtr<ID3D11Buffer> pVertextBuffer;
	D3D11_BUFFER_DESC vbd = {};
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.CPUAccessFlags = 0u;
	vbd.MiscFlags = 0u;
	vbd.ByteWidth = sizeof(vertices);
	vbd.StructureByteStride = sizeof(Vertex);
	
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;

	THROW_FAILED(pDevice->CreateBuffer(&vbd, &sd, &pVertextBuffer));
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u, pVertextBuffer.GetAddressOf(), &stride, &offset);

	//index buffer
	ComPtr<ID3D11Buffer> pIndexBuffer;
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.ByteWidth = sizeof(indices);
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.StructureByteStride = sizeof(unsigned short);
	ibd.Usage = D3D11_USAGE_DEFAULT;
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;
	THROW_FAILED( pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer));
	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	// create pixel shader
	ComPtr<ID3D11PixelShader> pPixelShader;
	ComPtr<ID3DBlob> blob;
	THROW_INFO_ONLY(D3DReadFileToBlob(L"PixelShader.cso", &blob));
	THROW_INFO_ONLY(pDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pPixelShader));
	//bind pixel shader
	THROW_INFO_ONLY(pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u));

	//create vertex shader
	ComPtr<ID3D11VertexShader> pVertextShader;
	THROW_INFO_ONLY(D3DReadFileToBlob(L"vertexShader.cso",&blob));
	THROW_INFO_ONLY(pDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pVertextShader));
	//bind vertex shader
	pContext->VSSetShader(pVertextShader.Get(), nullptr, 0u);

	//input (vertex) layout
	ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Position",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"Color",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,8u,D3D11_INPUT_PER_VERTEX_DATA,0},
	};
	THROW_FAILED(pDevice->CreateInputLayout(ied, std::size(ied), blob->GetBufferPointer(),
		blob->GetBufferSize(), &pInputLayout));

	pContext->IASetInputLayout(pInputLayout.Get());
	//bind render target
	THROW_INFO_ONLY(pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr));

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//viewport
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 100;
	vp.TopLeftY = 100;
	vp.Width = 400;
	vp.Height = 300;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;

	pContext->RSSetViewports(1u, &vp);

	THROW_INFO_ONLY(pContext->DrawIndexed(std::size(indices), 0u, 0u));
}

