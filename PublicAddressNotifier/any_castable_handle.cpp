#include "any_castable_handle.hpp"

namespace zmystudio
{
	any_castable_handle& any_castable_handle::operator=(void* right)
	{
		assign(right);

		return *this;
	}
}
