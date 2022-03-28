#pragma once
#include <vector>
class DxgiInfoMgr {
public :
	DxgiInfoMgr();
	~DxgiInfoMgr();
	DxgiInfoMgr(const DxgiInfoMgr&) = delete;
	DxgiInfoMgr& operator=(const DxgiInfoMgr&) = delete;

	void Set();

	std::vector<std::string> GetMessages();
private:
	unsigned long long next = 0u;
	 struct IDXGIInfoQueue* pDxgiInfoQueue = nullptr;
};