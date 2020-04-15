#pragma once

#include "any_castable_handle.hpp"

namespace zmystudio::service
{
	class service_handle : public any_castable_handle
	{
	public:
		using any_castable_handle::any_castable_handle;

		virtual bool valid() const override;
		service_handle& operator=(void* right);
	protected:
		virtual void close_handle(void* handle) const override;
	};
}
