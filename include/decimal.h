#ifndef LIBDECIMAL_DECIMAL_H
#define LIBDECIMAL_DECIMAL_H

#include <iostream>

class Decimal {
    std::string integerPart;
    std::string fractionalPart;
    int precision;
    bool isNegative;

    static bool isValidNum(const std::string& num);
    static bool isValidExp(const std::string& exp);
    static std::string calculateExp(const std::string& exp);
    static int compareStrings(const std::string& str1, const std::string& str2);
    static int precedence(char op);
    static std::string subtractStrings(std::string a, std::string b);
    static Decimal applyOp(Decimal num1, Decimal num2, char op);
    void addZeros(int count);
    void parse(const std::string& value);

public:
    Decimal() noexcept;
    Decimal(const std::string& value);
    Decimal(const Decimal& other);
    Decimal(Decimal&& other) noexcept;

    ~Decimal() = default;

    Decimal& operator=(const Decimal& other);
    Decimal& operator=(Decimal&& other) noexcept;

    Decimal operator+(const Decimal& other) const;
    Decimal operator-(const Decimal& other) const;
    Decimal operator*(const Decimal& other) const;
    Decimal operator/(const Decimal& other) const;

    bool operator==(const Decimal& other) const;
    bool operator!=(const Decimal& other) const;
    bool operator<(const Decimal& other) const;
    bool operator<=(const Decimal& other) const;
    bool operator>(const Decimal& other) const;
    bool operator>=(const Decimal& other) const;

    void round(int in_precision);

    std::string toString() const;
    int toInt() const;
    double toDouble() const;

    void setPrecision(int in_precision);
    int getPrecision() const noexcept;

    std::string getIntegerPart() const noexcept;
    std::string getFractionalPart() const noexcept;
    bool getIsNegative() const noexcept;

    friend std::ostream& operator<<(std::ostream& os, const Decimal& d);
    friend std::istream& operator>>(std::istream& is, Decimal& d);
};

#endif
