#include "../include/decimal.h"
#include <limits>
#include <regex>

Decimal::Decimal() {
    integerPart = "0";
    fractionalPart = "";
    precision = 0;
    isNegative = false;
}

Decimal::Decimal(const Decimal& other) {
    integerPart = other.integerPart;
    fractionalPart = other.fractionalPart;
    precision = other.precision;
    isNegative = other.isNegative;
}

Decimal::Decimal(Decimal&& other) noexcept : integerPart(other.integerPart), fractionalPart(other.fractionalPart),
                                             precision(other.precision), isNegative(other.isNegative){
    other.integerPart.clear();
    other.fractionalPart.clear();
    other.precision = 0;
    other.isNegative = false;
}

Decimal& Decimal::operator=(const Decimal& other){
    if (this != &other) {
        integerPart = other.integerPart;
        fractionalPart = other.fractionalPart;
        precision = other.precision;
        isNegative = other.isNegative;
    }
    return *this;
}

Decimal& Decimal::operator=(Decimal&& other) noexcept {
    if (this != &other) {
        integerPart = other.integerPart;
        fractionalPart = other.fractionalPart;
        precision = other.precision;
        isNegative = other.isNegative;

        other.integerPart.clear();
        other.fractionalPart.clear();
        other.precision = 0;
        other.isNegative = false;
    }
    return *this;
}

Decimal Decimal::operator+(const Decimal& other) const {

}

Decimal Decimal::operator-(const Decimal& other) const{

}

Decimal Decimal::operator*(const Decimal& other) const{

}

Decimal Decimal::operator/(const Decimal& other) const{

}

bool Decimal::operator==(const Decimal& other) const{
    if (other.integerPart == integerPart &&
            other.fractionalPart == fractionalPart &&
                other.isNegative == isNegative)
        return true;
    return false;
}

bool Decimal::operator!=(const Decimal& other) const{
    if (other.integerPart != integerPart ||
            other.fractionalPart != fractionalPart ||
                other.isNegative != isNegative)
        return true;
    return false;
}

bool Decimal::operator<(const Decimal& other) const{
    if ((isNegative && !other.isNegative) ||
        (other.isNegative == isNegative &&
               (other.integerPart > integerPart ||
               (other.integerPart == integerPart && other.fractionalPart > fractionalPart))))
        return true;
    return false;
}

bool Decimal::operator<=(const Decimal& other) const{
    if ((isNegative && !other.isNegative) ||
        (other.isNegative == isNegative &&
               (other.integerPart >= integerPart && other.fractionalPart >= fractionalPart)))
        return true;
    return false;
}

bool Decimal::operator>(const Decimal& other) const{
    if ((!isNegative && other.isNegative) ||
        (other.isNegative == isNegative &&
               (other.integerPart < integerPart ||
               (other.integerPart == integerPart && other.fractionalPart < fractionalPart))))
        return true;
    return false;
}

bool Decimal::operator>=(const Decimal& other) const {
    if ((!isNegative && other.isNegative) ||
        (other.isNegative == isNegative &&
               (other.integerPart <= integerPart && other.fractionalPart <= fractionalPart)))
        return true;
    return false;
}

void Decimal::round(int in_precision) {
    if (in_precision > precision) {
        addZeroes(in_precision - precision);
        precision = in_precision;
        return;
    }

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
    double fractionalValue = 0.0;
    double divisor = 1.0;
    double fractionalResult = 0;
    int sign = isNegative ? -1 : 1;

    for (char c : integerPart) {
        if (result > (std::numeric_limits<double>::max() - (c - '0')) / 10) {
            throw std::out_of_range("Integer part is too large for double");
        }
        result = result * 10 + (c - '0');
    }

    for (char c : fractionalPart) {
        if (fractionalValue > (std::numeric_limits<double>::max() - (c - '0')) / 10) {
            throw std::out_of_range("Fractional part is too large for double");
        }
        fractionalValue = fractionalValue * 10 + (c - '0');
        divisor *= 10;
    }

    fractionalResult = fractionalValue / divisor;
    result += fractionalResult;

    return result * sign;
}

void Decimal::setPrecision(int in_precision) {
    if (in_precision == precision)
        return;
    else round(in_precision);
}

int Decimal::getPrecision() {
    return precision;
}

void Decimal::addZeroes(int count){
    for (auto i = 0; i < count; i++) {
        fractionalPart += '0';
    }
}

std::ostream& operator<<(std::ostream& os, const Decimal& d) {
    std::string out = d.isNegative ? "-" : "";
    out += d.integerPart + '.' + d.fractionalPart;
    os << out;
    return os;
}

void Decimal::parse(const std::string& value) {
    if (isValid(value)) {
        size_t dotPos = value.find('.');
        if (dotPos == std::string::npos) {
            integerPart = value;
            fractionalPart = "";
        } else {
            integerPart = value.substr(0, dotPos);
            fractionalPart = value.substr(dotPos + 1);
        }
        precision = fractionalPart.size();

        if (!integerPart.empty() && integerPart[0] == '-') {
            isNegative = true;
            integerPart.erase(0, 1);
        }
    }
    else {
        integerPart = "0";
        fractionalPart = "";
        precision = 0;
        isNegative = false;
    }
}

std::istream& operator>>(std::istream& is, Decimal& d) {
    std::string inp;
    is >> inp;
    d.parse(inp);
    return is;
}

bool Decimal::isValid(const std::string& str) {
    static const std::regex pattern(R"(^[+-]?(0|[1-9]\d*)(\.\d+)?$)");
    return std::regex_match(str, pattern);
}