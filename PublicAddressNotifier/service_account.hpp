#pragma once

#include <string>
#include <optional>

namespace zmystudio::service
{
	struct service_account
	{
		std::u8string name;
		std::optional<std::u8string> password;
	};

	struct service_accounts
	{
		// These built-in accounts must contain empty passwords.
		inline static const service_account local_service{ .name = u8R"(NT AUTHORITY\LocalService)", .password = std::make_optional<std::u8string>() };
		inline static const service_account network_service{ .name = u8R"(NT AUTHORITY\NetworkService)", .password = std::make_optional<std::u8string>() };
	};
}
