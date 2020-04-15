#pragma once

#include <cstdint>

namespace zmystudio::service
{
	enum class service_type : uint32_t
	{
		win32_own_process = 0x10,
		win32_share_process = 0x20,
		user_own_process = 0x50,
		user_share_process = 0x60,
		interactive_process = 0x100
	};

	enum class service_start_type : uint32_t
	{
		auto_start = 0x2,
		demand_start = 0x3,
		disabled = 0x4
	};

	enum class service_error_control : uint32_t
	{
		critical = 0x3,
		ignore = 0x0,
		normal = 0x1,
		severe = 0x2
	};
}
