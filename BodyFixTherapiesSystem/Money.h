#pragma once

#include <string>

class Money {

private:
	/* “The mill is a unit of currency, used in several countries as one-
 * thousandth of the main unit.” (In this case, the GBP.)  This is
 * at least 64 bits wide, to be able to represent amounts larger than
 * £2,147,483.64.  It has one extra digit of precision, to ensure
 * proper rounding.
 *
 * If a long double has fewer than 63 mantissa bits, this implementation
 * might incorrectly round off extremely large credits or debits.  You
 * might want to flag this and throw a std::range_error exception.
 */
	long long int mills;

	// copied, work out how it works later
	static constexpr auto maxMoney = std::numeric_limits<decltype(mills)>::max();
	static constexpr auto minMoney = std::numeric_limits<decltype(mills)>::min();
	int millsPerGBP = 1000;
	int millsPerPenny = 10;
	int pencePerGBP = 100;

public:
	Money();
	// Constructor takes long long int initMills that will be used to represent mills internally and a dummyInt that is only used to differentiate between overloaded constructors
	Money(long long int initMills, int dummyInt);
	// Constructor converts currency in decimal to currency in mills for internal representation and calculation
	Money(long double pounds);
	// Constructor converts currency in two seperate ints to currency in mills for internal representation and calculation
	Money(long long int pounds, unsigned pence);

	Money operator+(const Money&);
	Money operator-(const Money&);
	Money operator<(const Money&);

	// Returns GBP in format £XX.xx
	std::string ToString();
	// Returns internal value of mills
	long long int GetMills();
};

std::ostream& operator<< (std::ostream&, Money);