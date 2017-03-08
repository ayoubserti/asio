#include "Utils.h"
std::string ws2s(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

std::wstring s2ws(const std::string& str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	return  converter.from_bytes(str);
}

inline unsigned char from_hex(
	unsigned char ch
	)
{
	if (ch <= '9' && ch >= '0')
		ch -= '0';
	else if (ch <= 'f' && ch >= 'a')
		ch -= 'a' - 10;
	else if (ch <= 'F' && ch >= 'A')
		ch -= 'A' - 10;
	else
		ch = 0;
	return ch;
}

std::string decodeUrl(const std::string& url)
{
	using namespace std;
	string result;
	string::size_type i;

	 
	for (i = 0; i < url.size(); ++i)
	{
		if (url[i] == '+')
		{
			result += ' ';
		}
		else if (url[i] == '%' && url.size() > i + 2)
		{
			const unsigned char ch1 = from_hex(url[i + 1]);
			const unsigned char ch2 = from_hex(url[i + 2]);
			const unsigned char ch = (ch1 << 4) | ch2;
			result += ch;
			i += 2;
		}
		else
		{
			result += url[i];
		}
	}
	return ws2s(s2ws(result));
}