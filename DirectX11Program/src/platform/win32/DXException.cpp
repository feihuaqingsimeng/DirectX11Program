#include "DXException.h"
#include <sstream>
DXException::DXException(int line, const char* file) 
	:line(line), file(file)
{

}

const char* DXException::what() const 
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* DXException::GetType() const 
{
	return "DXException";
}

int DXException::GetLine() const 
{
	return line;
}

const std::string& DXException::GetFile() const 
{
	return file;
}

std::string DXException::GetOriginString() const 
{
	std::ostringstream oss;
	oss << "[File]" << file << std::endl
		<< "[Line]" << line;
	return oss.str();
}

