#include "../include/decimal.h"
#include <limits>
#include <regex>
#include <vector>

Decimal::Decimal() {
    integerPart = "0";
    fractionalPart = "";
    precision = 0;
    isNegative = false;
}

Decimal::Decimal(std::string const std::string& value) {
    parse(value);
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
    if (isNegative == other.isNegative) {

    }
    return *this - other;
}

Decimal Decimal::operator-(const Decimal& other) const{
    if (isNegative != other.isNegative) {

    }
    return *this + other;
}

Decimal Decimal::operator*(const Decimal& other) const{
    std::string num1 = integerPart + fractionalPart;
    std::string num2 = other.integerPart + other.fractionalPart;
    std::string resultString = (isNegative == other.isNegative) ? "" : "-";
    std::string integerPartString = "";
    std::string fractionalPartString = "";
    const int len1 = num1.size();
    const int len2 = num2.size();
    const int newPrecision = precision + other.precision;
    const int indexFractionalPart = len1 + len2 - newPrecision;
    int skipZeros = 0;
    std::vector<char> resultVector(len1 + len2, '0');

    for (auto i = len1 - 1; i >= 0; --i) {
        for (auto j = len2 - 1; j >= 0; ++j) {
            auto multi = (num1[i] - '0') * (num2[j] - '0');
            auto sum = multi + resultVector[i + j + 1];
            resultVector[i + j + 1] = sum % 10;
            resultVector[i + j] = resultVector[i + j] + sum / 10;
        }
    }

    fractionalPartString = std::string(resultVector.begin() + indexFractionalPart, resultVector.end());

    while (skipZeros < indexFractionalPart && resultVector[skipZeros] == '0')
        ++skipZeros;

    if (indexFractionalPart == skipZeros)
        integerPartString = "0";
    else
        integerPartString = std::string(resultVector.begin() + skipZeros, resultVector.begin() + indexFractionalPart);

    if (newPrecision == 0)
        resultString = integerPartString == "0" ? "0" : (resultString + integerPartString);
    else
        resultString += integerPartString + '.' + fractionalPartString;

    return Decimal{resultString};
}

Decimal Decimal::operator/(const Decimal& other) const{
    std::string resultString = (isNegative == other.isNegative) ? "" : "-";

    return Decimal{resultString};
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
        addZeros(in_precision - precision);
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
    else
        round(in_precision);
}

int Decimal::getPrecision() {
    return precision;
}

void Decimal::addZeros(int count){
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

static bool isValidExp(const std::string& str) {

    return true;
}

static std::string calculateExp(const std::string& str) {
    std::string result = "";

    return result;
}

void Decimal::parse(const std::string& value) {
    if (isValidStr(value)) {
        isNegative = (value[0] == '-');
        std::string absValue = (isNegative) ? value.substr(1) : value;
        size_t dotPos = absValue.find('.');

        if (dotPos != std::string::npos) {
            integerPart = absValue.substr(0, dotPos);
            fractionalPart = absValue.substr(dotPos + 1);
        } else {
            integerPart = absValue;
            fractionalPart = "";
        }

        precision = fractionalPart.size();
    }
    else if (isValidExp(value)) {
        calculateExp(value);
    }
    else
        *this = Decimal();
}

std::istream& operator>>(std::istream& is, Decimal& d) {
    std::string inp;
    is >> inp;
    d.parse(inp);
    return is;
}

bool Decimal::isValidStr(const std::string& str) {
    static const std::regex pattern(R"(^[-]?(0|[1-9]\d*)(\.\d+)?$)");
    return std::regex_match(str, pattern);
}