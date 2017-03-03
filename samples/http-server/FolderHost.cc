#include "FolderHost.h"
#include <algorithm>
#include <urlmon.h>
#include <codecvt>

#pragma comment(lib, "urlmon.lib")

//private function
string ws2s(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

FolderHost::FolderHost(const string& host_name)
{
	folder_path_ = sRootFolder;
	directory_entry dir(folder_path_);
	string tmp = host_name;
	auto it = std::remove(tmp.begin(), tmp.end(), ':');
	tmp.erase(it, tmp.end());
	folder_path_ /= tmp;
	folder_name_ = folder_path_.leaf();

}

path FolderHost::sRootFolder = current_path<path>();

bool FolderHost::file_exist(const string& file_name)
{
	path tmp = folder_path_;
	
	tmp /= file_name;
	return exists(tmp);
}

void FolderHost::get_file_content(const string& file_name, char buf[], std::size_t& len)
{
	path full_path = folder_path_;
	full_path /= file_name;
	FILE* file = fopen(full_path.string().c_str(), "r");
	len = fread(buf, 1, len, file);
	fclose(file);
}

std::size_t FolderHost::get_file_length(const string& file_name)
{
	path full_path = folder_path_;
	full_path /= file_name;
	if (is_regular_file(full_path))
		return file_size(full_path);
	return 0;
	
}

std::string FolderHost::get_file_mime(const string& file_name)
{
	string result("");
	char data[265];
	LPWSTR out;

	path full_path = folder_path_;
	full_path /= file_name;
	FILE* file = fopen(full_path.string().c_str(), "r");
	fread(data, 1, 256, file);
	
	if (FindMimeFromData(NULL, NULL, data, 256, NULL, FMFD_DEFAULT, &out, 0) == 0)
	{
		wstring wres = out;
		result = ws2s(wres);
	}


	fclose(file);

	return result;
}