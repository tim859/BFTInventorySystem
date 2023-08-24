#include "Money.h"
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <iostream>

static const std::string invalidArgumentMessage = "Monetary quantity out of range.",
overflowMessage = "Monetary overflow.",
underflowMessage = "Monetary underflow.";

Money::Money()
{
	mills = 0;
}

Money::Money(long long int initMills, int dummyInt)
{
	mills = initMills;
}

Money::Money(long double pounds)
{
	// Convert pounds to mills
	long double totalPence = pounds * pencePerGBP;
	mills = static_cast<long long int>(totalPence * millsPerPenny);

	// Check for overflow/underflow
	if (mills > maxMoney)
		throw std::overflow_error(overflowMessage);
	if (mills < minMoney)
		throw std::underflow_error(underflowMessage);
}

Money::Money(long long int pounds, unsigned pence)
{
	// Convert pounds and pence to mills
	long long int totalPence = pounds * pencePerGBP + pence;
	mills = totalPence * millsPerPenny;

	// Check for overflow/underflow
	if (mills > maxMoney)
		throw std::overflow_error(overflowMessage);
	if (mills < minMoney)
		throw std::underflow_error(underflowMessage);
}

Money Money::operator+(const Money& other)
// Adds this and other, checking for overflow and underflow
{
	if (mills > 0 && maxMoney - mills < other.mills) {
		throw std::overflow_error(overflowMessage);
	}

	if (mills < 0 && minMoney - mills > other.mills) {
		throw std::underflow_error(underflowMessage);
	}

	return Money(mills + other.mills);
}

Money Money::operator-(const Money& other)
// Subtracts other from this, checking for overflow and underflow
{
	if (mills > 0 && maxMoney - mills < -other.mills) {
		throw std::overflow_error(overflowMessage);
	}

	if (mills < 0 && minMoney - mills > -other.mills) {
		throw std::underflow_error(underflowMessage);
	}

	return Money(mills - other.mills);
}

Money Money::operator<(const Money& other)
{
	return mills < other.mills;
}

std::string Money::ToString()
{
	std::stringstream toReturn;
	const auto pounds = mills / millsPerGBP;
	const auto pence = (mills >= 0) ? (mills % millsPerGBP) / millsPerPenny : -(mills % millsPerGBP) / millsPerPenny;

	toReturn << "\u00A3" << pounds << '.' << std::setw(2) << std::setfill('0') << std::right << pence;
	return toReturn.str();
}

double Money::ToDouble()
{
	int pounds = mills / millsPerGBP;
	int pence = (mills >= 0) ? (mills % millsPerGBP) / millsPerPenny : -(mills % millsPerGBP) / millsPerPenny;

	// Calculate the fractional value of pence
	double penceFraction = static_cast<double>(pence) / 100.0;

	// Combine pounds and penceFraction to form the total money value as a double
	double totalValue = static_cast<double>(pounds) + penceFraction;
	return totalValue;
}

long long int Money::GetMills() { 
	return mills; 
}

std::ostream& operator<< (std::ostream& os, Money x)
{
	return os << x.ToString();
}