#include "service_handle.hpp"

#include <Windows.h>

namespace zmystudio::service
{
	bool service_handle::valid() const
	{
		return *this != nullptr;
	}

	service_handle& service_handle::operator=(void* right)
	{
		assign(right);

		return *this;
	}

	void service_handle::close_handle(void* handle) const
	{
		CloseServiceHandle(static_cast<SC_HANDLE>(handle));
	}
}
