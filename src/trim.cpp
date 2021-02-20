#include "trim.hpp"

#include "is_whitespace.hpp"

namespace Utopia
{
	void trim(std::string& str)
	{
		ltrim(str);
		rtrim(str);
	}

	void ltrim(std::string& str)
	{
		while (!str.empty())
		{
			auto i = str.cbegin();
			const char c = *i;
			if (!is_whitespace(c))
			{
				return;
			}
			str.erase(i);
		}
	}

	void rtrim(std::string& str)
	{
		while (!str.empty())
		{
			auto i = (str.cend() - 1);
			const char  c = *i;
			if (!is_whitespace(c))
			{
				return;
			}
			str.erase(i);
		}
	}
}
