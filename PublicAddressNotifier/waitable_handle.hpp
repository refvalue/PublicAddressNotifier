#pragma once

#include "native_handle.hpp"

#include <cstdint>

namespace zmystudio
{
	class waitable_handle : public native_handle
	{
	public:
		using native_handle::native_handle;
		
		virtual ~waitable_handle() = default;
		bool wait();
		bool wait_for(uint32_t milliseconds);
	};
}
