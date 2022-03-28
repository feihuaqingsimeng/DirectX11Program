#pragma  once
#include <exception>
#include <string>
#include <windows.h>
#include <vector>

class Exception :public std::exception {
public:
	Exception(int line, const char* file);
	const char* what() const override;
	virtual const char* GetType() const;
	int GetLine() const;
	const std::string& GetFile() const;
	std::string GetOriginString() const;
private:
	int line;
	std::string file;
protected:
	mutable std::string whatBuffer;
};

class ResultException :public Exception {
public:
	ResultException(int line, const char* file, HRESULT hr);
	const char* what() const override;
	virtual const char* GetType() const;
	static std::string TranslateErrorCode(HRESULT hr);
	HRESULT GetErrorCode() const;
	std::string GetErrorString() const;
private:
	HRESULT hr;
	std::string info;
};

class InfoException :Exception {
public:
	InfoException(int line, const char* file,std::vector<std::string> infoMsgs);
	const char* what() const override;
	virtual const char* GetType() const;
	std::string GetErrorInfo() const;
private:
	std::string info;
};

#define  CHWND_EXCEPT(hr) ResultException(__LINE__,__FILE__,hr)
#define  CHWND_LAST_EXCEPT() ResultException(__LINE__,__FILE__,GetLastError());
#define THROW_FAILED(hrcall) if(FAILED(hr = (hrcall))) throw ResultException(__LINE__,__FILE__,hrcall)

#ifndef NDEBUG
#define  THROW_INFO_ONLY(call) infoManager.Set();(call);{auto v = infoManager.GetMessages();if(!v.empty()){ throw InfoException(__LINE__,__FILE__,v);}}
#endif
