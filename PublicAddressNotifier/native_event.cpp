#include "native_event.hpp"
#include "utils.hpp"

#include <Windows.h>

namespace zmystudio
{
	native_event::native_event() : native_event{ false, false }
	{
	}
	native_event::native_event(bool manual_reset) : native_event{ manual_reset, false }
	{
	}

	native_event::native_event(bool manual_reset, bool initial_state) : waitable_handle{ CreateEvent(nullptr, manual_reset, initial_state, nullptr) }
	{
	}

	native_event::native_event(const std::u8string_view& name) : native_event{ name, false, false }
	{
	}

	native_event::native_event(const std::u8string_view& name, bool manual_reset) : native_event{ name, manual_reset, false }
	{
	}

	native_event::native_event(const std::u8string_view& name, bool manual_reset, bool initial_state) : waitable_handle{ CreateEvent(nullptr, manual_reset, initial_state, utils::widen(name).c_str()) }
	{
	}
	
	void native_event::set()
	{
		if (valid())
		{
			SetEvent(*this);
		}
	}

	void native_event::reset()
	{
		if (valid())
		{
			ResetEvent(*this);
		}
	}
}
