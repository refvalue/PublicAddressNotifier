#pragma once

#include "service_misc.hpp"
#include "service_handle.hpp"
#include "service_account.hpp"

#include <vector>
#include <string>
#include <optional>
#include <string_view>

namespace zmystudio::service
{
	class service_installer
	{
	public:
		service_installer(const std::u8string_view& service_name);
		service_installer(const std::u8string_view& service_name, const std::u8string_view& display_name);
		virtual ~service_installer() = default;

		bool install();
		bool uninstall();

		uint32_t tag_id() const;
		service_type type() const;
		service_start_type start_type() const;
		service_error_control error_control() const;
		std::optional<service_account> account() const;
		std::optional<std::u8string> load_order_group() const;
		std::vector<std::u8string>& dependencies();

		void type(service_type value);
		void start_type(service_start_type value);
		void error_control(service_error_control value);
		void account(std::nullopt_t);
		void account(service_account&& value);
		void account(const service_account& value);
		void load_order_group(std::nullopt_t);
		void load_order_group(const std::u8string_view& value);
	private:
		std::u8string to_null_terminating_depenencies() const;
	private:
		uint32_t tag_id_;
		service_handle handle_;
		std::u8string service_name_;
		std::u8string display_name_;
		service_type type_;
		service_start_type start_type_;
		service_error_control error_control_;
		std::vector<std::u8string> dependencies_;
		std::optional<service_account> account_;
		std::optional<std::u8string> load_order_group_;
	};
}
