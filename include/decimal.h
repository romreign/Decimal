#ifndef LIBDECIMAL_DECIMAL_H
#define LIBDECIMAL_DECIMAL_H

#include <iostream>

class Decimal {
    std::string integerPart;
    std::string fractionalPart;
    int precision;
    bool isNegative;

    bool isValid(const std::string& str);
    void removeZeroes();

public:
    Decimal();
    Decimal(const std::string& value, int in_precision);
    Decimal(const Decimal& other);
    Decimal(Decimal&& other) noexcept;

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
    int getPrecision();

    friend std::ostream& operator<<(std::ostream& os, const Decimal& d);
};


#endif
