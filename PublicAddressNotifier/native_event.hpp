#pragma once

#include "waitable_handle.hpp"

#include <string_view>

namespace zmystudio
{
	class native_event : public waitable_handle
	{
	public:
		native_event();
		native_event(bool manual_reset);
		native_event(bool manual_reset, bool initial_state);
		native_event(const std::u8string_view& name);
		native_event(const std::u8string_view& name, bool manual_reset);
		native_event(const std::u8string_view& name, bool manual_reset, bool initial_state);
		virtual ~native_event() = default;

		void set();
		void reset();
	};
}
