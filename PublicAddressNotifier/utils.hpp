#pragma once

#include <string>
#include <filesystem>
#include <string_view>
#include <type_traits>

#include <ppltasks.h>

namespace zmystudio::utils
{
	inline const std::string empty_string;
	inline const std::wstring empty_wstring;
	inline const std::u8string empty_u8string;

	std::wstring widen(const std::u8string_view& str);
	std::u8string narrow(const std::wstring_view& str);
	std::u8string get_executable_path();
	std::u8string get_executable_directory();
	std::filesystem::path get_executable_path_file_system();
	std::filesystem::path get_executable_directory_file_system();
}
