#pragma once

namespace Utopia
{
	template <typename T>
	struct Shared
	{
		struct InternalShared
		{
			size_t refs = 1;
			T data;

			explicit InternalShared(T&& data)
				: data(std::move(data))
			{
			}
		};

		InternalShared* const data;

		explicit Shared(T&& data)
			: data(new Shared<T>::InternalShared(std::move(data)))
		{
		}
		
		explicit Shared(const Shared& b)
			: data(b.data)
		{
			data->refs++;
		}

		~Shared()
		{
			if (--data->refs == 0)
			{
				delete data;
			}
		}

		operator T () const
		{
			return data->data;
		}
	};
}
