#include "../include/decimal.h"
#include <limits>
#include <cmath>

bool Decimal::operator==(const Decimal& other) const{

}

bool Decimal::operator!=(const Decimal& other) const{

}

bool Decimal::operator<(const Decimal& other) const{

}

bool Decimal::operator<=(const Decimal& other) const{

}

bool Decimal::operator>(const Decimal& other) const{

}

bool Decimal::operator>=(const Decimal& other) const {

}

std::string Decimal::toString() const{
    std::string result = "";

    if (isNegative)
        result += '-';

    result += integerPart;

    if (precision > 0)
        result += '.' + fractionalPart;

    return result;
}

int Decimal::toInt() const {
    long long int result = 0;
    int sign = isNegative ? -1 : 1;

    for (auto i = 0; i < integerPart.size(); i++) {
        result = result * 10 + (integerPart[i] - '0');
        if (result > std::numeric_limits<int>::max() || -result < std::numeric_limits<int>::min()) {
            throw std::out_of_range("out of range");
        }
    }

    return static_cast<int>(result * sign);
}


double Decimal::toDouble() const {
    double result = 0.0;
    int sign = isNegative ? -1 : 1;

    for (char ch : integerPart) {
        result = result * 10 + (ch - '0');
        if (std::isinf(result)) {
            throw std::out_of_range("Integer part is too large for double");
        }
    }

    double fractionalValue = 0.0;
    double divisor = 1.0;
    for (char ch : fractionalPart) {
        divisor *= 10;
        fractionalValue = fractionalValue * 10 + (ch - '0');
        if (std::isinf(fractionalValue) || std::isinf(divisor)) {
            throw std::out_of_range("Fractional part is too large for double");
        }
    }

    double fractionalResult = fractionalValue / divisor;
    if (std::isinf(fractionalResult)) {
        throw std::out_of_range("Fractional part is too small or too large for double");
    }

    result += fractionalResult;
    return result * sign;
}


void Decimal::setPrecision(int in_precision) {
    if (in_precision > precision) {
        for (auto i = precision; i < in_precision; i++)
            fractionalPart += '0';
        precision = in_precision;
    }
    else if (in_precision < precision) {
        round(in_precision);
    }
}

int Decimal::getPrecision() {
    return precision;
}