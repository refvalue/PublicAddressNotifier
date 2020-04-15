#include "specialized_web_services.hpp"
#include "service_configration.hpp"
#include "utils.hpp"

#include <cuchar>

#include <pplawait.h>
#include <fmt/format.h>
#include <cpprest/http_client.h>

using web::uri;
using web::http::methods;
using web::http::http_exception;
using web::http::client::http_client;
using web::http::client::http_client_config;
using zmystudio::service::service_configuration;

namespace
{
	constexpr wchar_t public_address_uri[] = L"https://api.ipify.org/?format=text";

	const http_client_config default_config = std::invoke([]
	{
		http_client_config result;
		
		result.set_timeout(std::chrono::seconds{ 5 });

		return result;
	});
}

namespace zmystudio::net
{
	specialized_web_services::specialized_web_services()
	{
		public_address_client_ = std::make_shared<http_client>(service_configuration::current().public_address_uri(), default_config);
		journal_client_ = std::make_shared<http_client>(service_configuration::current().push_new_public_address_uri(), default_config);
	}

	concurrency::task<std::u8string> specialized_web_services::get_public_address_async()
	{
		try
		{
			auto response = co_await public_address_client_->request(web::http::methods::GET);

			if (response.status_code() == web::http::status_codes::OK)
			{
				co_return co_await response.extract_string();
			}

			co_return utils::empty_u8string;
		}
		catch (http_exception&)
		{
			co_return utils::empty_u8string;
		}
	}

	concurrency::task<bool> specialized_web_services::push_new_public_address_async(const std::u8string_view& ip)
	{
		try
		{
			auto response = co_await public_address_client_->request(methods::GET, fmt::format(push_new_public_address_query_path_, ip));

			co_return response.status_code() == web::http::status_codes::OK;
		}
		catch (http_exception&)
		{
			co_return false;
		}
	}
}
