#include "waitable_handle.hpp"

#include <Windows.h>

namespace zmystudio
{
	bool waitable_handle::wait()
	{
		return wait_for(INFINITE);
	}

	bool waitable_handle::wait_for(uint32_t milliseconds)
	{
		return valid() ? WaitForSingleObject(*this, milliseconds) : false;
	}
}
