#include "native_handle.hpp"

#include <Windows.h>

namespace zmystudio
{
	native_handle::native_handle() : handle_{}
	{
	}

	native_handle::native_handle(void* handle) : handle_{ handle }
	{
	}

	native_handle::~native_handle()
	{
		close_handle();
	}

	bool native_handle::valid() const
	{
		return handle_ != nullptr && handle_ != INVALID_HANDLE_VALUE;
	}

	native_handle::operator bool() const
	{
		return valid();
	}

	native_handle::operator void* () const
	{
		return handle_;
	}

	native_handle& native_handle::operator=(void* right)
	{
		assign(right);

		return *this;
	}

	void native_handle::close_handle()
	{
		if (valid())
		{
			close_handle(handle_);
			handle_ = nullptr;
		}
	}

	void native_handle::assign(void* handle)
	{
		close_handle();
		handle_ = handle;
	}

	void native_handle::close_handle(void* handle) const
	{
		CloseHandle(handle);
	}
}
