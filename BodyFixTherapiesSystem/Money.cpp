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

Money::Money(long long int newMills, std::string anything) {
	mills = newMills;
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
{
    long long int totalMills = mills + other.mills;

    if (totalMills > maxMoney) {
        throw std::overflow_error(overflowMessage);
    }

    if (totalMills < minMoney) {
        throw std::underflow_error(underflowMessage);
    }

    return Money(totalMills, "anything"); // Using the constructor that directly takes mills
}

// adds an int (in mills) directly to the mills value. Do NOT have the last number as anything other than 0 or it will permanently mess the accuracy of the money object up.
Money Money::operator+(const long long int other)
{
	long long int totalMills = mills + other;

	if (totalMills > maxMoney) {
		throw std::overflow_error(overflowMessage);
	}

	if (totalMills < minMoney) {
		throw std::underflow_error(underflowMessage);
	}

	return Money(totalMills, "anything"); // Using the constructor that directly takes mills
}

Money& Money::operator+=(const Money& other)
{
	if (mills > 0 && maxMoney - mills < other.mills) {
		throw std::overflow_error(overflowMessage);
	}

	if (mills < 0 && minMoney - mills > other.mills) {
		throw std::underflow_error(underflowMessage);
	}

	mills += other.mills;
	return *this; // Return a reference to the modified object
}

// adds an int (in mills) directly to the mills value. Do NOT have the last number as anything other than 0 or it will permanently mess the accuracy of the money object up.
Money& Money::operator+=(const long long int other)
{
	if (mills > 0 && maxMoney - mills < other) {
		throw std::overflow_error(overflowMessage);
	}

	if (mills < 0 && minMoney - mills > other) {
		throw std::underflow_error(underflowMessage);
	}

	mills += other;
	return *this; // Return a reference to the modified object
}

Money Money::operator-(const Money& other)
// Subtracts other from this, checking for overflow and underflow
{
	long long int totalMills = mills - other.mills;

	if (totalMills > maxMoney) {
		throw std::overflow_error(overflowMessage);
	}

	if (totalMills < minMoney) {
		throw std::underflow_error(underflowMessage);
	}

	return Money(totalMills, "anything");
}

Money Money::operator*(const int scalar)
{
	long long int totalMills = mills * scalar;

	if (totalMills > maxMoney) {
		throw std::overflow_error(overflowMessage);
	}

	if (totalMills < minMoney) {
		throw std::underflow_error(underflowMessage);
	}

	return Money(totalMills, "anything");
}

Money Money::operator*(const double scalar)
{
	// Calculate the total amount in mills as a floating-point number
	double totalMills = static_cast<double>(mills) * scalar;

	// Convert the result back to long long int with rounding
	long long int roundedTotalMills = static_cast<long long int>(std::round(totalMills));

	// Check for overflow
	if (roundedTotalMills > maxMoney) {
		throw std::overflow_error(overflowMessage);
	}

	// Check for underflow
	if (roundedTotalMills < minMoney) {
		throw std::underflow_error(underflowMessage);
	}

	// Return the new Money object
	return Money(roundedTotalMills, "anything");
}



Money Money::operator*(const Money& other)
{
	long long int resultMills = (mills * other.mills) / millsPerPenny;

	if (resultMills > maxMoney) {
		throw std::overflow_error(overflowMessage);
	}

	if (resultMills < minMoney) {
		throw std::underflow_error(underflowMessage);
	}

	return Money(resultMills, "anything");
}

Money Money::operator/(const Money& other)
{
	if (other.mills == 0) {
		throw std::runtime_error("Division by zero");
	}

	long long int resultMills = (mills * millsPerPenny) / other.mills;

	if (resultMills > maxMoney) {
		throw std::overflow_error(overflowMessage);
	}

	if (resultMills < minMoney) {
		throw std::underflow_error(underflowMessage);
	}

	return Money(resultMills, "anything");
}



bool Money::operator<(const Money& other)
{
	return mills < other.mills;
}

bool Money::operator==(const Money other)
{
	return mills == other.mills;
}

bool Money::operator!=(const Money other)
{
	return mills != other.mills;
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

const long long int Money::GetMills() { 
	return mills; 
}

std::ostream& operator<< (std::ostream& os, Money x)
{
	return os << x.ToString();
}