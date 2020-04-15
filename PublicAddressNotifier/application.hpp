#pragma once

#include "service_misc.hpp"

#include <vector>
#include <string>
#include <functional>
#include <string_view>

namespace zmystudio::service
{
	class application
	{
	public:
		using stop_handler_type = std::function<void()>;
		using start_handler_type = std::function<void(const std::vector<std::u8string_view>&)>;
	public:
		static void run(const std::u8string_view& service_name);
		static void run(service_type type, const std::u8string_view& service_name);
		static void on_start(const start_handler_type& handler);
		static void on_stop(const stop_handler_type& handler);
	private:
		static void __stdcall control_handler(uint32_t control);
		static void __stdcall service_main(uint32_t argc, wchar_t** argv);
		static void report_status(uint32_t current_state, uint32_t exit_code, uint32_t wait_hint);
	private:
		inline static stop_handler_type stop_handler_;
		inline static start_handler_type start_handler_;
	};
}
