#include "utils.hpp"

#include <cuchar>
#include <optional>

#include <Windows.h>

namespace
{
	constexpr size_t max_possible_path_size = 32768;
}

namespace zmystudio::utils
{
	std::wstring widen(const std::u8string_view& str)
	{
		std::wstring result;
		int size = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(str.data()), static_cast<int>(str.size()), nullptr, 0);

		if (size <= 0)
		{
			return result;
		}

		result.resize(size);
		MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(str.data()), static_cast<int>(str.size()), result.data(), static_cast<int>(result.size()));

		return result;
	}

	std::u8string narrow(const std::wstring_view& str)
	{
		std::u8string result;
		int size = WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), nullptr, 0, nullptr, nullptr);

		if (size <= 0)
		{
			return result;
		}

		result.resize(size);
		WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), reinterpret_cast<char*>(result.data()), static_cast<int>(result.size()), nullptr, nullptr);

		return result;
	}

	std::u8string get_executable_path()
	{
		thread_local std::optional<std::u8string> path;

		if (!path)
		{
			wchar_t path_wide[max_possible_path_size + 1];
			uint32_t real_size = GetModuleFileName(nullptr, path_wide, sizeof(path_wide) / sizeof(wchar_t));

			// If fails, the function returns zero, so that it makes the string empty.
			path.emplace(narrow(path_wide));
		}

		return *path;
	}

	std::u8string get_executable_directory()
	{
		thread_local std::optional<std::u8string> path;

		if (!path)
		{
			path = std::filesystem::path{ get_executable_path() }.parent_path().u8string();
		}

		return *path;
	}

	std::filesystem::path get_executable_path_file_system()
	{
		return get_executable_path();
	}

	std::filesystem::path get_executable_directory_file_system()
	{
		return get_executable_directory();
	}
}
