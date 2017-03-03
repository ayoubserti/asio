#pragma once
#include <string>
#include <filesystem>
#include <functional>


using namespace std;
using namespace std::tr2::sys;

class FolderHost
{
    string folder_name_;
	path   folder_path_;
	
	static path sRootFolder;
public:
	FolderHost(const string& host_name);

	bool file_exist(const string& file_name);

	void get_file_content(const string& file_name, std::function<void(const std::error_code& ec, char buf[], std::size_t len)> callback);

	void get_file_content(const string& file_name, char buf[], std::size_t& len);

	std::size_t get_file_length(const string& file_name);

	std::string get_file_mime(const string& file_name);
};