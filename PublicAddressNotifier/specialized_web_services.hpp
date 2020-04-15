#pragma once

#include <memory>
#include <string>
#include <string_view>

#include <ppltasks.h>

namespace web::http::client
{
	class http_client;
}

namespace zmystudio::net
{
	class specialized_web_services
	{
	public:
		specialized_web_services();
		virtual ~specialized_web_services() = default;

		concurrency::task<std::u8string> get_public_address_async();
		concurrency::task<bool> push_new_public_address_async(const std::u8string_view& ip);
	private:
		std::u8string push_new_public_address_query_path_;
		std::shared_ptr<web::http::client::http_client> journal_client_;
		std::shared_ptr<web::http::client::http_client> public_address_client_;
	};
}
