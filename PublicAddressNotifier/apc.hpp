#pragma once

#include "legacy_callback_wrapper.hpp"

#include <thread>
#include <functional>

namespace zmystudio::utils
{
    class apc final
    {
    public:
        static void queue(std::thread& thread, const std::function<void()>& callback);
        static void queue_nop(std::thread& thread);
    };
}
