#include "apc.hpp"

#include <Windows.h>

namespace zmystudio::utils
{
	void apc::queue(std::thread& thread, const std::function<void()>& callback)
	{
		if (thread.joinable())
		{
			auto wrapper = std::make_shared<legacy_callback_wrapper<void()>>();

			wrapper->set([wrapper, callback = callback]() mutable
			{
				wrapper.reset();

				if (callback)
				{
					callback();
				}

			});

			QueueUserAPC(wrapper->uintptr_t_adapter(), thread.native_handle(), reinterpret_cast<uintptr_t>(wrapper.get()));
		}
	}

	void apc::queue_nop(std::thread& thread)
	{
		if (thread.joinable())
		{
			QueueUserAPC([](uintptr_t) {}, thread.native_handle(), 0);
		}
	}
}
