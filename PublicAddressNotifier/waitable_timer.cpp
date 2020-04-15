#include "waitable_timer.hpp"
#include "apc.hpp"

#include <Windows.h>

namespace zmystudio::utils
{
    waitable_timer::waitable_timer() : is_cancelled_{ false }, timer_handle_{}, initialization_event_{ true }, worker_thread_{ std::make_shared<std::thread>(std::bind(&waitable_timer::worker_routine, this)) }
    {
        initialization_event_.wait();
    }

    waitable_timer::~waitable_timer()
    {
        is_cancelled_ = true;

        if (worker_thread_->joinable())
        {
            worker_thread_->detach();
        }
    }

    void waitable_timer::set(int64_t due_time, int period, const std::function<void()>& callback)
    {
        is_cancelled_ = false;

        // Inserts an APC call to start the timer.
        apc::queue(*worker_thread_, [this, due_time, period, callback = callback]
        {
            LARGE_INTEGER due{ .QuadPart = due_time };

            callback_wrapper_.reset(new legacy_callback_wrapper<void(uint64_t, uint64_t)>{ [=](auto, auto) { callback(); } });
            SetWaitableTimer(timer_handle_, &due, period, callback_wrapper_->as<PTIMERAPCROUTINE>(), callback_wrapper_.get(), FALSE);
        });
    }

    void waitable_timer::kill()
    {
        apc::queue(*worker_thread_, [=]
        {
            if (timer_handle_ != nullptr)
            {
                CancelWaitableTimer(timer_handle_);
            }
        });
    }

    void waitable_timer::worker_routine()
    {
        timer_handle_ = CreateWaitableTimer(nullptr, TRUE, nullptr);
        initialization_event_.set();

        // Waits infinitely to wait for an alerting signal.
        while (!is_cancelled_)
        {
            SleepEx(INFINITE, TRUE);
        }

        if (timer_handle_ != nullptr)
        {
            CloseHandle(timer_handle_);
            timer_handle_ = nullptr;
        }
    }
}
