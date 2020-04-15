#pragma once

#include <cstdint>
#include <functional>

namespace zmystudio::utils
{
    template <typename TFunctor>
    class legacy_callback_wrapper;

    /// <summary>
    /// A wrapper for a legacy C-style callback.
    /// </summary>
    template<typename TResult, typename... TArgs>
    class legacy_callback_wrapper<TResult(TArgs...)>
    {
    public:
        using function_type = std::function<TResult(TArgs...)>;
    public:
        legacy_callback_wrapper() = default;

        legacy_callback_wrapper(const function_type& callback) : callback_{ callback }
        {
        }

        legacy_callback_wrapper(function_type&& callback) : callback_{ std::move(callback) }
        {
        }

        virtual ~legacy_callback_wrapper() = default;

        void set(const function_type& callback)
        {
            callback_ = callback;
        }

        void set(function_type&& callback)
        {
            callback_ = std::move(callback);
        }

        template<typename TFunctionPointer>
        auto as() const
        {
            return reinterpret_cast<TFunctionPointer>(&legacy_routine);
        }

        auto void_ptr_adapter() const
        {
            return &legacy_routine;
        }

        auto intptr_t_adapter() const
        {
            return reinterpret_cast<TResult(__stdcall*)(intptr_t, TArgs...)>(&legacy_routine);
        }

        inline auto uintptr_t_adapter() const
        {
            return reinterpret_cast<TResult(__stdcall*)(uintptr_t, TArgs...)>(&legacy_routine);
        }
    private:
        static TResult __stdcall legacy_routine(void* param, TArgs... args)
        {
            auto this_ptr = static_cast<legacy_callback_wrapper*>(param);

            if (this_ptr->callback_)
            {
                return this_ptr->callback_(args...);
            }

            if constexpr (!std::is_void_v<TResult>)
            {
                TResult result{};

                return result;
            }
        }
    private:
        function_type callback_;
    };
}
