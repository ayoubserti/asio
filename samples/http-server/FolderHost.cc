#include "FolderHost.h"
#include <algorithm>
#include <urlmon.h>
#include "Utils.h"

#pragma comment(lib, "urlmon.lib")

wpath towpath(const path& p)
{
	wpath wtmp = s2ws(p.string());
	return wtmp;
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

	return exists(tmp) && is_regular_file(tmp);
}

void FolderHost::get_file_content(const string& file_name, char buf[], std::size_t& len)
{
	path full_path = folder_path_;
	full_path /= file_name;
	FILE* file = fopen(full_path.string().c_str(), "r");
	if (file)
	{
		len = fread(buf, 1, len, file);
		fclose(file);
	}
	
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
	path full_path = folder_path_;
	full_path /= file_name;

	if (!exists(full_path) || !is_regular_file(full_path)) return result;
	
	if (full_path.extension() == ".js")
	{
		return "application/javascript";
	}
	else if (full_path.extension() == ".html" || full_path.extension() == ".htm")
	{
		return "text/html";
	}
	else if (full_path.extension() == ".css")
	{
		return "text/css"; 
	}

	char data[265];
	LPWSTR out;

	FILE* file = fopen(full_path.string().c_str(), "r");
	if (file)
	{
		fread(data, 1, 256, file);

		if (FindMimeFromData(NULL, NULL, data, 256, NULL, FMFD_DEFAULT, &out, 0) == 0)
		{
			wstring wres = out;
			result = ws2s(wres);
		}

		fclose(file);
	}

	return result;
}


//wide API

bool FolderHost::file_exist(const wstring& file_name)
{
	path tmp = folder_path_;

	wpath wtmp = towpath(tmp);

	wtmp /= file_name;

	return exists(wtmp) && is_regular_file(wtmp);
}

void FolderHost::get_file_content(const wstring& file_name, char buf[], std::size_t& len)
{
	path full_path = folder_path_;
	wpath wfull_path = towpath(full_path);
	wfull_path /= file_name;
	FILE* file = _wfopen(wfull_path.string().c_str(), L"r");
	if (file)
	{
		len = fread(buf, 1, len, file);
		fclose(file);
	}

}

std::size_t FolderHost::get_file_length(const wstring& file_name)
{
	path full_path = folder_path_;
	wpath wfull_path = towpath(full_path);
	wfull_path /= file_name;
	if (is_regular_file(wfull_path))
		return file_size(wfull_path);
	return 0;

}

std::string FolderHost::get_file_mime(const wstring& file_name)
{
	string result("");
	path full_path = folder_path_;
	wpath wfull_path = towpath(full_path);
	wfull_path /= file_name;

	if (!exists(wfull_path) || !is_regular_file(wfull_path)) return result;

	if (wfull_path.extension() == L".js")
	{
		return "application/javascript";
	}
	else if (wfull_path.extension() == L".html" || wfull_path.extension() == L".htm")
	{
		return "text/html";
	}
	else if (wfull_path.extension() == L".css")
	{
		return "text/css";
	}

	char data[265];
	LPWSTR out;

	FILE* file = _wfopen(wfull_path.string().c_str(), L"r");
	if (file)
	{
		fread(data, 1, 256, file);

		if (FindMimeFromData(NULL, NULL, data, 256, NULL, FMFD_DEFAULT, &out, 0) == 0)
		{
			wstring wres = out;
			result = ws2s(wres);
		}

		fclose(file);
	}

	return result;
}