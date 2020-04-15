#include "application.hpp"
#include "native_event.hpp"
#include "utils.hpp"

#include <array>

#include <Windows.h>

// Internal data and constants.
namespace
{
	constexpr uint32_t default_pending_timeout = 3000;

	zmystudio::native_event stop_event;
	SERVICE_STATUS_HANDLE status_handle;
	SERVICE_STATUS current_status = { .dwServiceSpecificExitCode = 0 };
}

namespace zmystudio::service
{
	void application::run(const std::u8string_view& service_name)
	{
		run(service_type::win32_own_process, service_name);
	}

	void application::run(service_type type, const std::u8string_view& service_name)
	{
		auto service_name_wide = utils::widen(service_name);
		current_status.dwServiceType = static_cast<uint32_t>(type);

		std::array entries =
		{
			SERVICE_TABLE_ENTRY{.lpServiceName = service_name_wide.data(), .lpServiceProc = reinterpret_cast<LPSERVICE_MAIN_FUNCTION>(&application::service_main) },
			SERVICE_TABLE_ENTRY{.lpServiceName = nullptr, .lpServiceProc = nullptr }
		};

		StartServiceCtrlDispatcher(entries.data());
	}

	void application::on_start(const start_handler_type& handler)
	{
		start_handler_ = handler;
	}

	void application::on_stop(const stop_handler_type& handler)
	{
		stop_handler_ = handler;
	}

	void __stdcall application::control_handler(uint32_t control)
	{
		switch (control)
		{
		case SERVICE_CONTROL_STOP:
		{
			report_status(SERVICE_STOP_PENDING, NO_ERROR, 0);

			// Signals the service to stop.
			stop_event.set();
			report_status(current_status.dwCurrentState, NO_ERROR, 0);

			break;
		}
		case SERVICE_CONTROL_INTERROGATE:
			break;
		default:
			break;
		}
	}

	void __stdcall application::service_main(uint32_t argc, wchar_t** argv)
	{
		status_handle = RegisterServiceCtrlHandler(argv[0], reinterpret_cast<LPHANDLER_FUNCTION>(&application::control_handler));

		if (status_handle == nullptr)
		{
			return;
		}

		std::vector<std::u8string_view> arguments;

		for (size_t i = 0; i < argc; i++)
		{
			arguments.emplace_back(utils::narrow(argv[i]));
		}

		// Reports pending.
		report_status(SERVICE_START_PENDING, NO_ERROR, default_pending_timeout);

		// Reports running.
		report_status(SERVICE_RUNNING, NO_ERROR, 0);

		if (start_handler_)
		{
			start_handler_(arguments);
		}

		// Waiting for the stopping signal.
		stop_event.wait();

		if (stop_handler_)
		{
			stop_handler_();
		}

		report_status(SERVICE_STOPPED, NO_ERROR, 0);
	}

	void application::report_status(uint32_t current_state, uint32_t exit_code, uint32_t wait_hint)
	{
		static uint32_t check_point = 1;

		current_status.dwCurrentState = current_state;
		current_status.dwWin32ExitCode = exit_code;
		current_status.dwWaitHint = wait_hint;
		current_status.dwControlsAccepted = current_state == SERVICE_START_PENDING ? 0 : SERVICE_ACCEPT_STOP;
		current_status.dwCheckPoint = (current_state == SERVICE_RUNNING || current_state == SERVICE_STOPPED) ? 0 : check_point++;

		// Reports the status of the service to the SCM.
		SetServiceStatus(status_handle, &current_status);
	}
}
