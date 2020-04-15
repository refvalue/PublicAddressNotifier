#include "utils.hpp"
#include "application.hpp"
#include "waitable_timer.hpp"
#include "service_installer.hpp"
#include "service_configration.hpp"
#include "specialized_web_services.hpp"

#include <iostream>

#include <pplawait.h>
#include <fmt/format.h>
#include <gflags/gflags.h>

using zmystudio::service::application;
using zmystudio::utils::waitable_timer;
using zmystudio::service::service_installer;
using zmystudio::net::specialized_web_services;
using zmystudio::service::service_configuration;

namespace utils = zmystudio::utils;

namespace
{
	constexpr int business_period = 1000 * 60 * 10;
	constexpr std::u8string_view service_name{ u8"PublicAddressNotifier" };
	constexpr std::u8string_view service_installation_tip_pattern{ u8"Service installation {}." };
	constexpr std::u8string_view service_uninstallation_tip_pattern{ u8"Service uninstallation {}." };
	constexpr std::u8string_view service_tip_succeeded{ u8"succeeded" };
	constexpr std::u8string_view service_tip_failed{ u8"failed" };
}

namespace
{
	DEFINE_bool(install, false, "Install the service and start it.");
	DEFINE_bool(uninstall, false, "Stop the service and uninstall it.");
}

namespace
{
	concurrency::task<void> on_timer_async()
	{
		thread_local std::u8string previous_address;
		thread_local specialized_web_services services;

		auto current_address = co_await services.get_public_address_async();

		if (!current_address.empty() && current_address != previous_address)
		{
			co_await services.push_new_public_address_async(current_address);
			previous_address = std::move(current_address);
		}
	}
}

int main(int argc, char** argv)
{
	gflags::ParseCommandLineFlags(&argc, &argv, false);

	if (FLAGS_install || FLAGS_uninstall)
	{
		service_installer installer{ service_name };
		auto pattern = FLAGS_install ? service_installation_tip_pattern : service_uninstallation_tip_pattern;
		auto handler = [&] { return FLAGS_install ? installer.install() : installer.uninstall(); };
	
		std::wcout << utils::widen( fmt::format(pattern, handler() ? service_tip_succeeded : service_tip_failed)) << std::endl;
	}
	else
	{
		waitable_timer timer;

		application::on_start([&](const std::vector<std::u8string_view>& arguments)
		{
			//timer.set(0, business_period, &on_timer_async);
		});

		application::on_stop([&]
		{
			//timer.kill();
		});

		application::run(service_name);
	}

	gflags::ShutDownCommandLineFlags();
}
