#include "service_configration.hpp"
#include "utils.hpp"

#include <string>
#include <fstream>
#include <string_view>

using web::json::value;
using web::json::object;

namespace
{
	constexpr std::string_view config_file_name{ "service_config.json" };
	const std::filesystem::path config_file_path = zmystudio::utils::get_executable_directory_file_system() / config_file_name;
}

namespace zmystudio::service
{
	service_configuration::~service_configuration()
	{
		flush();
	}

	void service_configuration::flush()
	{
		std::ofstream stream{ config_file_path, std::ios::binary | std::ios::trunc };

		root_.serialize(stream);
	}

	std::u8string service_configuration::public_address_uri()
	{
		return root_[u8"publicAddress"][u8"uri"].as_string();
	}

	std::u8string service_configuration::push_new_public_address_uri()
	{
		return root_[u8"pushNewPublicAddress"][u8"uri"].as_string();
	}

	std::u8string service_configuration::push_new_public_address_uri_query_path()
	{
		return root_[u8"pushNewPublicAddress"][u8"queryPath"].as_string();
	}

	service_configuration::service_configuration()
	{
		std::error_code code;
		std::ifstream stream{ config_file_path, std::ios::binary };
		
		// If failed, the value is null.
		root_ = value::parse(stream, code);
	}
}
