#pragma once

#include "native_event.hpp"
#include "legacy_callback_wrapper.hpp"

#include <atomic>
#include <memory>
#include <thread>
#include <functional>

#include <ppltasks.h>

namespace zmystudio::utils
{
    class waitable_timer
    {
    public:
        waitable_timer();
        virtual ~waitable_timer();
        void set(int64_t due_time, int period, const std::function<void()>& callback);
        void kill();
    private:
        void worker_routine();
    private:
        void* timer_handle_;
        std::atomic_bool is_cancelled_;
        native_event initialization_event_;
        std::shared_ptr<std::thread> worker_thread_;
        std::shared_ptr<legacy_callback_wrapper<void(uint64_t, uint64_t)>> callback_wrapper_;
    };
}
