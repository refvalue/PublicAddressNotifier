#include "service_installer.hpp"
#include "utils.hpp"

#include <sstream>

#include <Windows.h>

namespace zmystudio::service
{
	service_installer::service_installer(const std::u8string_view& service_name) : service_installer{ service_name, service_name }
	{
	}

	service_installer::service_installer(const std::u8string_view& service_name, const std::u8string_view& display_name) : tag_id_{}, service_name_ { service_name }, display_name_{ display_name }, type_{ service_type::win32_own_process }, start_type_{ service_start_type::auto_start }, error_control_{ service_error_control::normal }
	{
		handle_ = OpenSCManager(nullptr, SERVICES_ACTIVE_DATABASE, SC_MANAGER_ALL_ACCESS);
	}

	bool service_installer::install()
	{
		if (!handle_)
		{
			return false;
		}

		auto path = utils::get_executable_path();
		auto dependencies = to_null_terminating_depenencies();
		
		service_handle handle = CreateService(
			handle_,
			utils::widen(service_name_).c_str(),
			utils::widen(display_name_).c_str(),
			SERVICE_ALL_ACCESS,
			static_cast<uint32_t>(type_),
			static_cast<uint32_t>(start_type_),
			static_cast<uint32_t>(error_control_),
			utils::widen(path).c_str(),
			load_order_group_ ? utils::widen(*load_order_group_).c_str() : nullptr,
			load_order_group_ ? reinterpret_cast<DWORD*>(&tag_id_) : nullptr,
			dependencies.empty() ? nullptr : utils::widen(dependencies).c_str(),
			account_ ? utils::widen(account_->name).c_str() : nullptr,
			account_ && account_->password ? utils::widen(*account_->password).c_str() : nullptr
		);

		return handle;
	}

	bool service_installer::uninstall()
	{
		if (!handle_)
		{
			return false;
		}

		service_handle handle = OpenService(handle_, utils::widen(service_name_).c_str(), SERVICE_ALL_ACCESS);

		if (!handle)
		{
			return false;
		}

		SERVICE_STATUS status;

		if (!ControlService(handle, SERVICE_CONTROL_STOP, &status) && GetLastError() != ERROR_SERVICE_NOT_ACTIVE)
		{
			return false;
		}
		
		return DeleteService(handle);
	}

	uint32_t service_installer::tag_id() const
	{
		return tag_id_;
	}

	service_type service_installer::type() const
	{
		return type_;
	}

	service_start_type service_installer::start_type() const
	{
		return start_type_;
	}

	service_error_control service_installer::error_control() const
	{
		return error_control_;
	}

	std::optional<service_account> service_installer::account() const
	{
		return account_;
	}

	std::optional<std::u8string> service_installer::load_order_group() const
	{
		return load_order_group_;
	}

	std::vector<std::u8string>& service_installer::dependencies()
	{
		return dependencies_;
	}

	void service_installer::type(service_type value)
	{
		type_ = value;
	}

	void service_installer::start_type(service_start_type value)
	{
		start_type_ = value;
	}

	void service_installer::error_control(service_error_control value)
	{
		error_control_ = value;
	}

	void service_installer::account(std::nullopt_t)
	{
		account_.reset();
	}

	void service_installer::account(service_account&& value)
	{
		account_ = std::move(value);
	}

	void service_installer::account(const service_account& value)
	{
		account_ = value;
	}

	void service_installer::load_order_group(std::nullopt_t)
	{
		load_order_group_.reset();
	}

	void service_installer::load_order_group(const std::u8string_view& value)
	{
		load_order_group_ = value;
	}

	std::u8string service_installer::to_null_terminating_depenencies() const
	{
		std::basic_stringstream<char8_t> result;

		std::for_each(std::begin(dependencies_), std::end(dependencies_), [&](const std::u8string& item) { result << item << u8'\0'; });

		return result.str();
	}
}
