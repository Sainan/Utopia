#pragma once

#include <cstdint>

namespace Utopia 
{
	template <typename T>
	struct Shared
	{
		struct Data
		{
			T data;
			size_t refs = 0;

			explicit Data(T&& data)
				: data(std::move(data))
			{
			}
		};

		Data* const data;

		explicit Shared(T&& data)
			: data(new Data(std::move(data)))
		{
		}

		explicit Shared(const Shared& b)
			: data(b.data)
		{
			data->refs++;
		}

		~Shared()
		{
			if (data->refs == 0)
			{
				delete data;
			}
			else
			{
				data->refs--;
			}
		}

		operator T& () const
		{
			return data->data;
		}
	};
}
