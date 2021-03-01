#include "BigInt.hpp"

#include <stdexcept>

namespace Utopia
{
	BigInt::BigInt()
		: segments({}), negative(false)
	{
	}

	BigInt::BigInt(unsigned long long value, bool negative)
		: segments({}), negative(negative)
	{
		if (value != 0)
		{
			segments.emplace_back(value);
		}
	}
	
	BigInt::BigInt(long long value)
		: BigInt(unsigned long long(abs(value)), value < 0)
	{
	}

	BigInt::BigInt(const BigInt& b)
		: segments(b.segments), negative(b.negative)
	{
	}

	BigInt::BigInt(BigInt&& b)
		: segments(std::move(b.segments)), negative(b.negative)
	{
	}

	BigInt& BigInt::operator =(long long value)
	{
		segments = { unsigned long long(abs(value)) };
		negative = value < 0;
		return *this;
	}

	BigInt& BigInt::operator =(const BigInt& b)
	{
		segments = b.segments;
		negative = b.negative;
		return *this;
	}

	BigInt& BigInt::operator =(BigInt&& b)
	{
		segments = std::move(b.segments);
		negative = b.negative;
		return *this;
	}

	BigInt& BigInt::reset()
	{
		segments.clear();
		negative = false;
		return *this;
	}

	bool BigInt::operator ==(const BigInt& b) const
	{
		return negative == b.negative && segments == b.segments;
	}

	bool BigInt::operator !=(const BigInt& b) const
	{
		return !operator ==(b);
	}

	bool BigInt::isZero() const
	{
		return segments.empty();
	}

	template <typename T>
	static T max(T a, T b)
	{
		return a > b ? a : b;
	}

	static unsigned long long bitlimit(const BigInt& a, const BigInt& b)
	{
		return max(a.segments.size(), b.segments.size()) * sizeof(long long) * 8;
	}

	bool BigInt::isGreaterThan(const BigInt& b, bool ret_if_eq) const
	{
		if (negative != b.negative)
		{
			return b.negative;
		}
		unsigned long long i = bitlimit(*this, b);
		do
		{
			if (b[i])
			{
				if (!getBit(i))
				{
					return false;
				}
			}
			else
			{
				if (getBit(i))
				{
					return true;
				}
			}
		} while (i--);
		return ret_if_eq;
	}

	bool BigInt::operator >(const BigInt& b) const
	{
		return isGreaterThan(b, false);
	}

	bool BigInt::operator <(const BigInt& b) const
	{
		return b > *this;
	}

	bool BigInt::operator >=(const BigInt& b) const
	{
		return isGreaterThan(b, true);
	}

	bool BigInt::operator <=(const BigInt& b) const
	{
		return b >= *this;
	}

	constexpr unsigned long long BigInt::getBytesPerSegment()
	{
		return sizeof(long long);
	}

	constexpr unsigned long long BigInt::getBitsPerSegment()
	{
		return getBytesPerSegment() * 8;
	}

	unsigned long long BigInt::getHighestAllocatedBitIndex() const
	{
		return segments.size() * getBitsPerSegment();
	}

	unsigned long long BigInt::getHighestUsedBitIndex() const
	{
		auto i = getHighestAllocatedBitIndex();
		while (i != 0)
		{
			--i;
			if (getBit(i))
			{
				break;
			}
		}
		return i;
	}

	uint8_t BigInt::getBit(unsigned long long i) const
	{
		constexpr auto bits_per_segment = getBitsPerSegment();
		auto segment = i / bits_per_segment;
		if (segment >= segments.size())
		{
			return 0;
		}
		return ((segments.at(segment) >> (i - (segment * bits_per_segment))) & 0b1);
	}

	uint8_t BigInt::operator [](unsigned long long i) const
	{
		return getBit(i);
	}

	void BigInt::setBit(unsigned long long i, bool on)
	{
		constexpr auto bits_per_segment = getBitsPerSegment();
		auto segment = i / bits_per_segment;
		if (on)
		{
			while (segment >= segments.size())
			{
				segments.emplace_back(unsigned long long(0));
			}
			segments.at(segment) |= (unsigned long long(1) << (i - (segment * bits_per_segment)));
		}
		else if (segment < segments.size())
		{
			segments.at(segment) &= ~(unsigned long long(1) << (i - (segment * bits_per_segment)));
			if (segment == segments.size() - 1)
			{
				auto i = segments.begin() + segment;
				while (*i == 0)
				{
					i = segments.erase(i);
					if (i == segments.begin())
					{
						if (segments.empty())
						{
							negative = false;
						}
						break;
					}
					--i;
				}
			}
		}
	}

	BigInt& BigInt::operator >>=(unsigned long long rep)
	{
		const auto bits = getHighestAllocatedBitIndex();
		for (auto i = 0; i < bits; i++)
		{
			setBit(i, getBit(i + rep));
		}
		return *this;
	}

	BigInt BigInt::operator >>(unsigned long long rep) const
	{
		auto res = BigInt(*this);
		res >>= rep;
		return res;
	}

	BigInt& BigInt::operator <<=(unsigned long long rep)
	{
		auto i = getHighestAllocatedBitIndex();
		while (i != 0)
		{
			--i;
			setBit(i + rep, getBit(i));
		}
		while (rep != 0)
		{
			--rep;
			setBit(rep, false);
		}
		return *this;
	}

	BigInt BigInt::operator <<(unsigned long long rep) const
	{
		auto res = BigInt(*this);
		res <<= rep;
		return res;
	}

	BigInt& BigInt::operator +=(const BigInt& b)
	{
		unsigned long long limit = bitlimit(*this, b);
		uint8_t carry = 0;
		unsigned long long i = 0;
		do
		{
			carry += getBit(i) + b[i];
			setBit(i, carry & 0b1);
			carry >>= 1;
		} while (++i < limit);
		setBit(i, carry & 0b1);
		return *this;
	}

	BigInt BigInt::operator +(const BigInt& b) const
	{
		unsigned long long limit = bitlimit(*this, b);
		auto res = BigInt();
		uint8_t carry = 0;
		unsigned long long i = 0;
		do
		{
			carry += getBit(i) + b[i];
			res.setBit(i, carry & 0b1);
			carry >>= 1;
		} while (++i < limit);
		res.setBit(i, carry & 0b1);
		return res;
	}

	BigInt& BigInt::operator ++()
	{
		return operator +=(1);
	}

	BigInt BigInt::operator ++(int)
	{
		BigInt og(*this);
		operator ++();
		return og;
	}

	BigInt& BigInt::operator -=(const BigInt& subtrahend)
	{
		if (*this < subtrahend)
		{
			segments = (subtrahend - *this).segments;
			negative = !isZero();
			return *this;
		}
		BigInt minuend(*this);
		const auto bits = getHighestAllocatedBitIndex();
		for (unsigned long long i = 0; i < bits; i++)
		{
			if (minuend[i])
			{
				setBit(i, !subtrahend[i]);
			}
			else if(!subtrahend[i])
			{
				setBit(i, false);
			}
			else
			{
				setBit(i, true);
				auto j = i;
				while (!minuend.getBit(++j))
				{
					minuend.setBit(j, true);
				}
				minuend.setBit(j, false);
			}
		}
		return *this;
	}

	BigInt BigInt::operator -(const BigInt& subtrahend) const
	{
		auto res = BigInt(*this);
		res -= subtrahend;
		return res;
	}

	BigInt& BigInt::operator --()
	{
		return operator -=(1);
	}

	BigInt BigInt::operator --(int)
	{
		BigInt og(*this);
		operator --();
		return og;
	}

	BigInt& BigInt::operator *=(const BigInt& b)
	{
		if (b.isZero())
		{
			reset();
		}
		else
		{
			BigInt rep(b);
			BigInt add(*this);
			while (!(--rep).isZero())
			{
				operator +=(add);
			}
		}
		return *this;
	}

	BigInt BigInt::operator *(const BigInt& b) const
	{
		auto res = BigInt(*this);
		res *= b;
		return res;
	}

	std::pair<BigInt, BigInt> BigInt::divide(const BigInt& divisor) const
	{
		if (divisor.negative)
		{
			BigInt divisor(divisor);
			divisor.negative = false;
			auto res = divide(divisor);
			res.first.negative ^= 1;
			return res;
		}
		if (negative)
		{
			BigInt dividend(*this);
			dividend.negative = false;
			auto res = dividend.divide(divisor);
			res.first.negative ^= 1;
			if (!res.second.isZero())
			{
				res.first -= 1;
				res.second = divisor - res.second;
			}
			return res;
		}
		return unsignedDivide(divisor);
	}

	std::pair<BigInt, BigInt> BigInt::unsignedDivide(const BigInt& divisor) const
	{
		if (divisor.isZero())
		{
			throw std::logic_error("Division by zero");
		}
		/*std::pair<BigInt, BigInt> res{};
		auto i = getHighestUsedBitIndex();
		while (i != 0)
		{
			res.second <<= 1;
			res.second.setBit(0, getBit(i));
			if (res.second >= divisor)
			{
				std::cout << res.second.toLongLong() << " - " << divisor.toLongLong() << " = ";
				res.second -= divisor;
				std::cout << res.second.toLongLong() << std::endl;
				res.first.setBit(i, 1);
			}
			--i;
		}*/
		std::pair<BigInt, BigInt> res{0, *this};
		while (res.second >= divisor)
		{
			++res.first;
			res.second -= divisor;
		}
		return res;
	}

	BigInt BigInt::operator /(const BigInt& divisor) const
	{
		return divide(divisor).first;
	}

	BigInt BigInt::operator %(const BigInt& divisor) const
	{
		return divide(divisor).second;
	}

	long long BigInt::toLongLong() const
	{
		switch (segments.size())
		{
		case 0:
			return 0;
			
		case 1:
			auto val = long long(segments.at(0));
			if (val >= 0)
			{
				if (negative)
				{
					val *= -1;
				}
				return val;
			}
		}
		throw std::out_of_range("This BigInt can't be represented as long long");
	}

	unsigned long long BigInt::toUnsignedLongLong() const
	{
		if (!negative)
		{
			switch (segments.size())
			{
			case 0:
				return 0;

			case 1:
				return segments.at(0);
			}
		}
		throw std::out_of_range("This BigInt can't be represented as unsigned long long");
	}

	std::string BigInt::toString(int base, const char* charmap) const
	{
		std::string str{};
		BigInt quotient(*this);
		quotient.negative = false;
		if (quotient.isZero())
		{
			str.append(1, charmap[0]);
		}
		else do
		{
			auto res = quotient.divide(base);
			str.insert(0, 1, charmap[res.second.toUnsignedLongLong()]);
			quotient = std::move(res.first);
		} while (!quotient.isZero());
		if (negative)
		{
			str.insert(0, 1, '-');
		}
		return str;
	}
}
