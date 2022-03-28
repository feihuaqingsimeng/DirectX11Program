#include "DxgiInfoMgr.h"
#include <windows.h>
#include "Graphics.h"
#include "DXException.h"
#include <dxgidebug.h>

#pragma comment(lib,"dxguid.lib")

#define GFX_THROW_NOINFO(hrcall) if(FAILED(hr = (hrcall))) throw ResultException(__LINE__,__FILE__,hr)

DxgiInfoMgr::DxgiInfoMgr()
{
	typedef HRESULT  (WINAPI* DXGIGetDebugInterface)(REFIID, void **);
	const auto hModDxgiDebug = LoadLibraryEx(L"dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (hModDxgiDebug == nullptr) {
		throw CHWND_LAST_EXCEPT();
	}
	const auto dxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
		reinterpret_cast<void*>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface")));
	if (dxgiGetDebugInterface == nullptr)
		throw CHWND_LAST_EXCEPT();
	HRESULT hr;
	GFX_THROW_NOINFO(dxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), reinterpret_cast<void**>(&pDxgiInfoQueue)));
}
DxgiInfoMgr::~DxgiInfoMgr()
{
	if (pDxgiInfoQueue != nullptr) {
		pDxgiInfoQueue->Release();
	}
}

void DxgiInfoMgr::Set()
{
	next = pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> DxgiInfoMgr::GetMessages()
{
	std::vector<std::string> messages;
	const auto end = pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	for (auto i = next; i < end; i++) {
		HRESULT hr;
		SIZE_T messageLength;
		GFX_THROW_NOINFO(pDxgiInfoQueue->GetMessageW(DXGI_DEBUG_ALL, i, nullptr, &messageLength));
		auto bytes = std::make_unique<byte[]>(messageLength);
		auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
		GFX_THROW_NOINFO(pDxgiInfoQueue->GetMessageW(DXGI_DEBUG_ALL, i, pMessage, &messageLength));
		messages.emplace_back(pMessage->pDescription);
	}
	return messages;
}


