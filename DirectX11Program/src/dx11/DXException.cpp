#include "DXException.h"
#include <sstream>
Exception::Exception(int line, const char* file)
	:line(line), file(file)
{

}

const char* Exception::what() const 
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Exception::GetType() const 
{
	return "DXException";
}

int Exception::GetLine() const 
{
	return line;
}

const std::string& Exception::GetFile() const 
{
	return file;
}

std::string Exception::GetOriginString() const 
{
	std::ostringstream oss;
	oss << "[File]" << file << std::endl
		<< "[Line]" << line;
	return oss.str();
}

ResultException::ResultException(int line, const char* file, HRESULT hr)
	:Exception(line, file), hr(hr)
{

}

const char* ResultException::what() const
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code]" << GetErrorCode() << std::endl
		<< "[Description]" << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* ResultException::GetType() const
{
	return "Exception";
}

std::string ResultException::TranslateErrorCode(HRESULT hr)
{
	char* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&pMsgBuf),
		0, nullptr);
	if (nMsgLen == 0)
		return "Unidentified error code";
	std::string errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;
}

HRESULT ResultException::GetErrorCode() const
{
	return hr;
}

std::string ResultException::GetErrorString() const
{
	return TranslateErrorCode(hr);
}
InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs)
	:Exception(line,file)
{
	for (const auto& m : infoMsgs) {
		info += m;
		info.push_back('\n');
	}
	if (!info.empty()) {
		info.pop_back();
	}
}

const char * InfoException::what() const
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char * InfoException::GetType() const
{
	return "Info Exception";
}

std::string InfoException::GetErrorInfo() const
{
	return info;
}