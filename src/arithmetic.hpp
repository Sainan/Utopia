#pragma once

namespace Utopia
{
	using arithmetic_func_t = long long(*)(const long long, const long long);

	long long arithmetic_plus(const long long left, const long long right);
	long long arithmetic_minus(const long long left, const long long right);
	long long arithmetic_multiply(const long long left, const long long right);
	long long arithmetic_divide(const long long left, long long right);
}
