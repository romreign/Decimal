#include "../include/decimal.h"
#include <limits>
#include <regex>
#include <vector>

Decimal::Decimal() noexcept{
    integerPart = "0";
    fractionalPart = "";
    precision = 0;
    isNegative = false;
}

Decimal::Decimal(const std::string& value) {
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

    if (other.integerPart == "0" && other.fractionalPart.empty()) {
        return *this;
    }
    if (integerPart == "0" && fractionalPart.empty()) {
        return other;
    }

    Decimal result;
    std::string num1 = integerPart + fractionalPart;
    std::string num2 = other.integerPart + other.fractionalPart;

    int maxFractionalLength = std::max(precision, other.precision);
    num1 += std::string(maxFractionalLength - precision, '0');
    num2 += std::string(maxFractionalLength - other.precision, '0');

    if (isNegative != other.isNegative) {
        if (isNegative) {
            Decimal temp = *this;
            temp.isNegative = false;
            return other - temp;
        } else {
            Decimal temp = other;
            temp.isNegative = false;
            return *this - temp;
        }
    }

    int carry = 0;
    std::string sum;
    for (int i = num1.size() - 1; i >= 0; --i) {
        int digit1 = num1[i] - '0';
        int digit2 = num2[i] - '0';
        int total = digit1 + digit2 + carry;
        carry = total / 10;
        sum.push_back((total % 10) + '0');
    }

    if (carry)
        sum.push_back(carry + '0');


    std::reverse(sum.begin(), sum.end());

    size_t dotPos = sum.size() - maxFractionalLength;
    result.integerPart = sum.substr(0, dotPos);
    result.fractionalPart = sum.substr(dotPos);
    result.precision = maxFractionalLength;
    result.isNegative = isNegative;

    return result;
}

Decimal Decimal::operator-(const Decimal& other) const{
    if (other.integerPart == "0" && other.fractionalPart.empty())
        return *this;

    if (*this == other)
        return Decimal{0};

    Decimal result;
    if (integerPart == "0" && fractionalPart.empty()) {
        result = other;
        result.isNegative = !other.isNegative;
        return result;
    }

    std::string num1 = integerPart + fractionalPart;
    std::string num2 = other.integerPart + other.fractionalPart;

    int maxFractionalLength = std::max(precision, other.precision);
    num1 += std::string(maxFractionalLength - precision, '0');
    num2 += std::string(maxFractionalLength - other.precision, '0');

    if (isNegative != other.isNegative) {
        Decimal temp = other;
        temp.isNegative = isNegative;
        return *this + temp;
    }

    bool isFirstLarger = true;
    if (num1.size() < num2.size() || (num1.size() == num2.size() && num1 < num2))
        isFirstLarger = false;


    std::string diff;
    int borrow = 0;
    for (int i = num1.size() - 1; i >= 0; --i) {
        int digit1 = num1[i] - '0' - borrow;
        int digit2 = num2[i] - '0';
        if (digit1 < digit2) {
            digit1 += 10;
            borrow = 1;
        } else
            borrow = 0;

        diff.push_back((digit1 - digit2) + '0');
    }

    std::reverse(diff.begin(), diff.end());

    size_t dotPos = diff.size() - maxFractionalLength;
    result.integerPart = diff.substr(0, dotPos);
    result.fractionalPart = diff.substr(dotPos);
    result.precision = maxFractionalLength;
    result.isNegative = (isFirstLarger ? isNegative : !isNegative);

    return result;
}

Decimal Decimal::operator*(const Decimal& other) const{
    std::string num1 = integerPart + fractionalPart;
    std::string num2 = other.integerPart + other.fractionalPart;

    bool isResultNegative = (isNegative != other.isNegative);

    int maxFractionalLength = std::max(precision, other.precision);
    num1 += std::string(maxFractionalLength - precision, '0');
    num2 += std::string(maxFractionalLength - other.precision, '0');

    const int len1 = num1.size();
    const int len2 = num2.size();
    const int resultSize = len1 + len2;
    std::vector<int> resultVector(resultSize, 0);

    for (int i = len1 - 1; i >= 0; --i) {
        for (int j = len2 - 1; j >= 0; --j) {
            int digit1 = num1[i] - '0';
            int digit2 = num2[j] - '0';
            int product = digit1 * digit2;
            int pos = i + j + 1;
            resultVector[pos] += product;

            if (resultVector[pos] > 9) {
                resultVector[pos - 1] += resultVector[pos] / 10;
                resultVector[pos] %= 10;
            }
        }
    }

    int skipZeros = 0;
    while (skipZeros < resultVector.size() && resultVector[skipZeros] == 0)
        ++skipZeros;

    std::string integerPartString;
    for (int i = skipZeros; i < resultVector.size() - maxFractionalLength; ++i)
        integerPartString += ('0' + resultVector[i]);

    if (integerPartString.empty())
        integerPartString = "0";

    std::string fractionalPartString;
    for (int i = resultVector.size() - maxFractionalLength; i < resultVector.size(); ++i)
        fractionalPartString += ('0' + resultVector[i]);

    fractionalPartString.erase(fractionalPartString.find_last_not_of('0') + 1);

    std::string resultString;
    if (fractionalPartString.empty())
        resultString = integerPartString;
    else
        resultString = integerPartString + '.' + fractionalPartString;

    if (isResultNegative && resultString != "0")
        resultString = '-' + resultString;

    return Decimal{resultString};
}

Decimal Decimal::operator/(const Decimal& other) const{
    if (other.integerPart == "0" && other.fractionalPart == "")
        throw std::invalid_argument("Division by zero");

    std::string resultString = (isNegative == other.isNegative) ? "" : "-";

    std::string dividend = integerPart + fractionalPart;
    std::string divisor = other.integerPart + other.fractionalPart;

    int maxFractionalLength = std::max(precision, other.precision);
    dividend += std::string(maxFractionalLength - precision, '0');
    divisor += std::string(maxFractionalLength - other.precision, '0');

    divisor.erase(0, divisor.find_first_not_of('0'));
    if (divisor.empty()) {
        divisor = "0";
    }

    std::string quotient;
    std::string remainder = "0";

    for (size_t i = 0; i < dividend.size(); ++i) {
        remainder += dividend[i];

        remainder.erase(0, remainder.find_first_not_of('0'));
        if (remainder.empty()) {
            remainder = "0";
        }

        int count = 0;
        while (compareStrings(remainder, divisor) >= 0) {
            remainder = subtractStrings(remainder, divisor);
            count++;
        }

        quotient += ('0' + count);
    }

    size_t dotPos = quotient.size() - maxFractionalLength;
    std::string integerPartString = quotient.substr(0, dotPos);
    std::string fractionalPartString = quotient.substr(dotPos);

    fractionalPartString.erase(fractionalPartString.find_last_not_of('0') + 1);

    if (fractionalPartString.empty())
        resultString += integerPartString;
    else
        resultString += integerPartString + '.' + fractionalPartString;


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
    if (isNegative && !other.isNegative)
        return true;

    if (isNegative && isNegative == other.isNegative) {
        if (integerPart > other.integerPart)
            return true;
        if ((integerPart == other.integerPart) && (fractionalPart > other.fractionalPart))
            return true;
    }

    if (!isNegative && isNegative == other.isNegative) {
        if (integerPart < other.integerPart)
            return true;
        if ((integerPart == other.integerPart) && (fractionalPart < other.fractionalPart))
            return true;
    }

    return false;
}

bool Decimal::operator<=(const Decimal& other) const{
    if (isNegative && !other.isNegative)
        return true;

    if (isNegative && isNegative == other.isNegative) {
        if (integerPart > other.integerPart)
            return true;
        if ((integerPart == other.integerPart) && (fractionalPart >= other.fractionalPart))
            return true;
    }

    if (!isNegative && isNegative == other.isNegative) {
        if (integerPart < other.integerPart)
            return true;
        if ((integerPart == other.integerPart) && (fractionalPart <= other.fractionalPart))
            return true;
    }

    return false;
}

bool Decimal::operator>(const Decimal& other) const{
    if (!isNegative && other.isNegative)
        return true;

    if (isNegative && isNegative == other.isNegative) {
        if (integerPart < other.integerPart)
            return true;
        if ((integerPart == other.integerPart) && (fractionalPart < other.fractionalPart))
            return true;
    }

    if (!isNegative && isNegative == other.isNegative) {
        if (integerPart > other.integerPart)
            return true;
        if ((integerPart == other.integerPart) && (fractionalPart > other.fractionalPart))
            return true;
    }

    return false;
}

bool Decimal::operator>=(const Decimal& other) const {
    if (!isNegative && other.isNegative)
        return true;

    if (isNegative && isNegative == other.isNegative) {
        if (integerPart < other.integerPart)
            return true;
        if ((integerPart == other.integerPart) && (fractionalPart <= other.fractionalPart))
            return true;
    }

    if (!isNegative && isNegative == other.isNegative) {
        if (integerPart > other.integerPart)
            return true;
        if ((integerPart == other.integerPart) && (fractionalPart >= other.fractionalPart))
            return true;
    }

    return false;
}

void Decimal::round(int in_precision) {
    if (in_precision < 0)
        throw std::invalid_argument("Precision cannot be negative");

    if (in_precision > precision) {
        addZeros(in_precision - precision);
        precision = in_precision;
        return;
    }

    int index = precision - 1;
    bool isRound = false;

    while (index >= 0 && (index >= in_precision || isRound)) {
        if (index > 0 && (fractionalPart[index] - '0' >= 5 || isRound)) {
            fractionalPart[index - 1] = fractionalPart[index - 1] == '9' ? '0' : (((fractionalPart[index - 1] - '0') + 1) + '0');
            isRound = fractionalPart[index - 1] == '0' ? true : false;
        } else if (index == 0)
            isRound = fractionalPart[index] == '0' ? true : false;
        --index;
    }

    fractionalPart = fractionalPart.substr(0, in_precision);


    if (isRound) {
        index = integerPart.size() - 1;
        while (index >= 0 && isRound) {
            if (index > 0) {
                integerPart[index] = integerPart[index] == '9' ? '0' : (((integerPart[index] - '0') + 1) + '0');
                isRound = integerPart[index] == '0' ? true : false;
            } else if (index == 0) {
                integerPart[index] = integerPart[index] == '9' ? '0' : (((integerPart[index] - '0') + 1) + '0');
                integerPart = (integerPart[index] == '0')? "1" + integerPart : integerPart;
                break;
            }
            --index;
        }
    }

    precision = fractionalPart.size();
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
    if (in_precision < 0)
        throw std::invalid_argument("Precision cannot be negative");

    if (in_precision == precision)
        return;
    else
        round(in_precision);
}

int Decimal::getPrecision() const noexcept{
    return precision;
}

void Decimal::addZeros(int count){
    if (count < 0)
        throw std::invalid_argument("Count cannot be negative");

    for (auto i = 0; i < count; i++)
        fractionalPart += '0';
}

std::ostream& operator<<(std::ostream& os, const Decimal& d) {
    std::string out = d.isNegative ? "-" : "";

    out += d.integerPart;
    if (d.precision != 0)
        out += '.' + d.fractionalPart;

    os << out;
    return os;
}

bool Decimal::isValidExp(const std::string& exp) {
    std::regex pattern(R"(^([+-]?(\d+(\.\d+)?|\d+\.\d*)(\s*[-+*/]\s*[+-]?(\d+(\.\d+)?|\d+\.\d*))*?)?$)");
    return std::regex_match(exp, pattern);
}

std::string Decimal::calculateExp(const std::string& exp) {
    if (exp.empty())
        return "0";

    std::stack<Decimal> values;
    std::stack<char> ops;

    for (size_t i = 0; i < exp.size(); ++i) {
        if (exp[i] == ' ') {
            continue;
        } else if (isdigit(exp[i]) || exp[i] == '.') {
            std::string numStr;
            while (i < exp.size() && (isdigit(exp[i]) || exp[i] == '.')) {
                numStr += exp[i++];
            }
            values.push(Decimal(numStr));
            --i;
        } else if (exp[i] == '(') {
            ops.push(exp[i]);
        } else if (exp[i] == ')') {
            while (ops.top() != '(') {
                Decimal val2 = values.top(); values.pop();
                Decimal val1 = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOp(val1, val2, op));
            }
            ops.pop();
        } else if (exp[i] == '+' || exp[i] == '-' || exp[i] == '*' || exp[i] == '/') {
            while (!ops.empty() && precedence(ops.top()) >= precedence(exp[i])) {
                Decimal val2 = values.top(); values.pop();
                Decimal val1 = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOp(val1, val2, op));
            }
            ops.push(exp[i]);
        }
    }

    while (!ops.empty()) {
        Decimal val2 = values.top(); values.pop();
        Decimal val1 = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        values.push(applyOp(val1, val2, op));
    }

    return values.top().toString();
}

void Decimal::parse(const std::string& value) {
    if (isValidNum(value)) {
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
        *this = Decimal(calculateExp(value));
    }
    else
        *this = Decimal();
}

std::istream& operator>>(std::istream& is, Decimal& d) {
    std::string inp;
    is >> inp;
    if (inp.empty()) {
        d = Decimal();
        return is;
    }
    d.parse(inp);
    return is;
}

bool Decimal::isValidNum(const std::string& num) {
    static const std::regex pattern(R"(^[-]?(0|[1-9]\d*)(\.\d+)?$)");
    return std::regex_match(num, pattern);
}

int Decimal::compareStrings(const std::string& str1, const std::string& str2) {
    if (str1.size() < str2.size()) return -1;
    if (str1.size() > str2.size()) return 1;
    return str1.compare(str2);
}

std::string Decimal::subtractStrings(std::string a, std::string b) {
    if (a.size() < b.size())
        a.insert(0, b.size() - a.size(), '0');
    else if (b.size() < a.size())
        b.insert(0, a.size() - b.size(), '0');

    std::string result;
    int borrow = 0;

    for (int i = a.size() - 1; i >= 0; --i) {
        int digitA = a[i] - '0' - borrow;
        int digitB = b[i] - '0';

        if (digitA < digitB) {
            digitA += 10;
            borrow = 1;
        } else
            borrow = 0;


        result += ('0' + (digitA - digitB));
    }

    result.erase(result.find_last_not_of('0') + 1);
    if (result.empty())
        result = "0";
    std::reverse(result.begin(), result.end());

    return result.empty() ? "0" : result;
}

Decimal Decimal::applyOp(Decimal num1, Decimal num2, char op) {
    switch (op) {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        case '/': return num1 / num2;
        default: throw std::invalid_argument("Invalid operator");
    }
}

int Decimal::precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

std::string Decimal::getIntegerPart() const noexcept{
    return integerPart;
}

std::string Decimal::getFractionalPart() const noexcept{
    return fractionalPart;
}

bool Decimal::getIsNegative() const noexcept{
    return isNegative;
}