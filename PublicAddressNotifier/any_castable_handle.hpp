#pragma once

#include "native_handle.hpp"

namespace zmystudio
{
	class any_castable_handle : public native_handle
	{
	public:
		using native_handle::native_handle;

		template<typename TAny>
		operator TAny* () const
		{
			void* ptr = *this;

			return static_cast<TAny*>(ptr);
		}

		any_castable_handle& operator=(void* right);
	};
}
