#pragma once 
#include <string>
#include <codecvt>

std::string ws2s(const std::wstring& wstr);

std::string decodeUrl(const std::string& url);

std::wstring s2ws(const std::string& str);