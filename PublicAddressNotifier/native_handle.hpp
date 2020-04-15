#pragma once

namespace zmystudio
{
	class native_handle
	{
	public:
		native_handle();
		native_handle(void* handle);
		virtual ~native_handle();

		virtual bool valid() const;
		operator bool() const;
		operator void*() const;
		native_handle& operator=(void* right);
	protected:
		void close_handle();
		void assign(void* handle);
		virtual void close_handle(void* handle) const;
	private:
		void* handle_;
	};
}
