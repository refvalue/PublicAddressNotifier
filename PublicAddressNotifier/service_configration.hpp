#pragma once

#include "singleton.hpp"

#include <string>

#include <cpprest/json.h>

namespace zmystudio::service
{
	class service_configuration : public singleton<service_configuration>
	{
		friend struct singleton;
	public:
		virtual ~service_configuration();

		void flush();
		std::u8string public_address_uri();
		std::u8string push_new_public_address_uri();
		std::u8string push_new_public_address_uri_query_path();
	private:
		service_configuration();
	private:
		web::json::value root_;
	};
}
