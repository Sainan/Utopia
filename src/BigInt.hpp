#pragma once

#include <string>
#include <vector>

namespace Utopia
{
	struct BigInt
	{
		std::vector<unsigned long long> segments;
		bool negative;

		BigInt();
		BigInt(unsigned long long value, bool negative);
		BigInt(long long value);
		BigInt(const BigInt& b);
		BigInt(BigInt&& b);

		BigInt& operator=(long long value);
		BigInt& operator=(const BigInt& b);
		BigInt& operator=(BigInt&& b);
		BigInt& reset();

		[[nodiscard]] bool operator ==(const BigInt& b) const;
		[[nodiscard]] bool operator !=(const BigInt& b) const;
		[[nodiscard]] bool isZero() const;

		[[nodiscard]] bool isGreaterThan(const BigInt& b, bool ret_if_eq) const;
		[[nodiscard]] bool operator >(const BigInt& b) const;
		[[nodiscard]] bool operator <(const BigInt& b) const;
		[[nodiscard]] bool operator >=(const BigInt& b) const;
		[[nodiscard]] bool operator <=(const BigInt& b) const;

		[[nodiscard]] static constexpr unsigned long long getBytesPerSegment();
		[[nodiscard]] static constexpr unsigned long long getBitsPerSegment();
		[[nodiscard]] unsigned long long getHighestAllocatedBitIndex() const;
		[[nodiscard]] unsigned long long getHighestUsedBitIndex() const;

		[[nodiscard]] uint8_t getBit(unsigned long long i) const;
		[[nodiscard]] uint8_t operator [](unsigned long long i) const;
		void setBit(unsigned long long i, bool on);

		BigInt& operator >>=(unsigned long long rep);
		[[nodiscard]] BigInt operator >>(unsigned long long rep) const;
		BigInt& operator <<=(unsigned long long rep);
		[[nodiscard]] BigInt operator <<(unsigned long long rep) const;

		BigInt& operator +=(const BigInt& b);
		[[nodiscard]] BigInt operator +(const BigInt& b) const;
		BigInt& operator ++();
		BigInt operator ++(int);

		BigInt& operator -=(const BigInt& subtrahend);
		[[nodiscard]] BigInt operator -(const BigInt& subtrahend) const;
		BigInt& operator --();
		BigInt operator --(int);

		BigInt& operator *=(const BigInt& b);
		[[nodiscard]] BigInt operator *(const BigInt& b) const;

		[[nodiscard]] std::pair<BigInt, BigInt> divide(const BigInt& divisor) const; // (Quotient, Remainder)
		[[nodiscard]] std::pair<BigInt, BigInt> unsignedDivide(const BigInt& divisor) const; // (Quotient, Remainder)
		[[nodiscard]] BigInt operator /(const BigInt& divisor) const;
		[[nodiscard]] BigInt operator %(const BigInt& divisor) const;

		[[nodiscard]] long long toLongLong() const;
		[[nodiscard]] unsigned long long toUnsignedLongLong() const;
		[[nodiscard]] std::string toString(int base = 10, const char* charmap = "0123456789ABCDEF") const; // if base > 16, provide an ample charmap or get sigsev'ed
	};
}
