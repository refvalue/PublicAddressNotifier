#pragma once

#include <mutex>
#include <optional>
#include <type_traits>

namespace zmystudio
{
	template<typename T>
	struct singleton
	{
		static T& current()
		{
			static std::mutex lock;
			static std::optional<T> instance;
			{
				std::lock_guard guard{ lock };

				if (!instance)
				{
					instance = T{};
				}
			}

			return *instance;
		}

		virtual ~singleton() = default;
	};
}
