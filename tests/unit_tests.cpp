#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>

#include "../include/decimal.h"
#include <iostream>
#include <sstream>

TEST_CASE("libdecimal") {

    SECTION("CONSTRUCTION") {
        SECTION("default") {
            Decimal decimal{};

            SECTION("initializes integerPart to \"0\"")
                REQUIRE(decimal.getIntegerPart() == "0");

            SECTION("initializes fractionalPart to \"\"")
                REQUIRE(decimal.getFractionalPart() == "");

            SECTION("initializes fractionalPart to 0")
                REQUIRE(decimal.getPrecision() == 0);

            SECTION("initializes isNegative to false")
                REQUIRE(decimal.getIsNegative() == false);

        }

        SECTION("string") {
            SECTION("number") {
                SECTION("is positive") {
                    Decimal decimal{"132.4444449"};

                    SECTION("initializes integerPart to \"132\"")
                        REQUIRE(decimal.getIntegerPart() == "132");

                    SECTION("initializes fractionalPart to \"4444449\"")
                        REQUIRE(decimal.getFractionalPart() == "4444449");

                    SECTION("initializes precision to 7")
                        REQUIRE(decimal.getPrecision() == 7);

                    SECTION("initializes isNegative to true")
                        REQUIRE(decimal.getIsNegative() == false);
                }

                SECTION("is negative") {
                    Decimal decimal{"-132.4444449"};

                    SECTION("initializes integerPart to \"132\"")
                        REQUIRE(decimal.getIntegerPart() == "132");

                    SECTION("initializes fractionalPart to \"4444449\"")
                        REQUIRE(decimal.getFractionalPart() == "4444449");

                    SECTION("initializes precision to 7")
                        REQUIRE(decimal.getPrecision() == 7);

                    SECTION("initializes isNegative to true")
                        REQUIRE(decimal.getIsNegative() == true);
                }
            }
/*
            SECTION("expression") {
                Decimal decimal{"130 + 29 / 4 - 1 * 2"};

                SECTION("initializes integerPart to \"135\"")
                    REQUIRE(decimal.getIntegerPart() == "135");

                SECTION("initializes fractionalPart to \"25\"")
                    REQUIRE(decimal.getFractionalPart() == "25");

                SECTION("initializes precision to 2")
                    REQUIRE(decimal.getPrecision() == 2);

                SECTION("initializes isNegative to false")
                    REQUIRE(decimal.getIsNegative() == false);
            }
*/
            SECTION("invalid") {
                Decimal decimal{"-adasdas131231"};

                SECTION("initializes integerPart to \"0\"")
                    REQUIRE(decimal.getIntegerPart() == "0");

                SECTION("initializes fractionalPart to \"\"")
                    REQUIRE(decimal.getFractionalPart() == "");

                SECTION("initializes precision to 0")
                    REQUIRE(decimal.getPrecision() == 0);

                SECTION("initializes isNegative to false")
                    REQUIRE(decimal.getIsNegative() == false);
            }

            SECTION("empty") {
                Decimal decimal{""};

                SECTION("initializes integerPart to \"0\"")
                    REQUIRE(decimal.getIntegerPart() == "0");

                SECTION("initializes fractionalPart to \"\"")
                    REQUIRE(decimal.getFractionalPart() == "");

                SECTION("initializes precision to 0")
                    REQUIRE(decimal.getPrecision() == 0);

                SECTION("initializes isNegative to false")
                    REQUIRE(decimal.getIsNegative() == false);
            }
        }

        SECTION("copy") {
            Decimal decimal{"-131.013"};
            Decimal copy_decimal{decimal};

            SECTION("initializes copy_decimal.integerPart to decimal.integerPart")
                REQUIRE(decimal.getIntegerPart() == copy_decimal.getIntegerPart());

            SECTION("initializes copy_decimal.fractionalPart to decimal.freactionalPart")
                REQUIRE(decimal.getFractionalPart() == copy_decimal.getFractionalPart());

            SECTION("initializes copy_decimal.precision to decimal.precision")
                REQUIRE(decimal.getPrecision() == copy_decimal.getPrecision());

            SECTION("initializes copy_decimal.isNegative to decimal.isNegative")
                REQUIRE(decimal.getIsNegative() == copy_decimal.getIsNegative());
        }

        SECTION("move") {
            Decimal decimal{"-131.013"};
            std::string originalInteger = decimal.getIntegerPart();
            std::string originalFractional = decimal.getFractionalPart();
            int originalPrecision = decimal.getPrecision();
            bool originalIsNegative = decimal.getIsNegative();

            Decimal moved_decimal{std::move(decimal)};

            SECTION("moved_decimal contains original values") {
                REQUIRE(originalInteger == moved_decimal.getIntegerPart());
                REQUIRE(originalFractional == moved_decimal.getFractionalPart());
                REQUIRE(originalPrecision == moved_decimal.getPrecision());
                REQUIRE(originalIsNegative == moved_decimal.getIsNegative());
            }

            SECTION("the moved object is in a valid state") {
                REQUIRE(decimal.getIntegerPart().empty());
                REQUIRE(decimal.getFractionalPart().empty());
                REQUIRE(decimal.getPrecision() == 0);
                REQUIRE_FALSE(decimal.getIsNegative());
            }
        }
    }

    SECTION("Operators") {
        SECTION("copy") {
            Decimal decimal{"-131.013"};
            Decimal copy_decimal = decimal;

            SECTION("initializes copy_decimal.integerPart to decimal.integerPart")
                REQUIRE(decimal.getIntegerPart() == copy_decimal.getIntegerPart());

            SECTION("initializes copy_decimal.fractionalPart to decimal.freactionalPart")
                REQUIRE(decimal.getFractionalPart() == copy_decimal.getFractionalPart());

            SECTION("initializes copy_decimal.precision to decimal.precision")
                REQUIRE(decimal.getPrecision() == copy_decimal.getPrecision());

            SECTION("initializes copy_decimal.isNegative to decimal.isNegative")
                REQUIRE(decimal.getIsNegative() == copy_decimal.getIsNegative());
        }

        SECTION("move") {
            Decimal decimal{"-131.013"};
            std::string originalInteger = decimal.getIntegerPart();
            std::string originalFractional = decimal.getFractionalPart();
            int originalPrecision = decimal.getPrecision();
            bool originalIsNegative = decimal.getIsNegative();

            Decimal moved_decimal = std::move(decimal);

            SECTION("moved_decimal contains original values") {
                REQUIRE(originalInteger == moved_decimal.getIntegerPart());
                REQUIRE(originalFractional == moved_decimal.getFractionalPart());
                REQUIRE(originalPrecision == moved_decimal.getPrecision());
                REQUIRE(originalIsNegative == moved_decimal.getIsNegative());
            }

            SECTION("the moved object is in a valid state") {
                REQUIRE(decimal.getIntegerPart().empty());
                REQUIRE(decimal.getFractionalPart().empty());
                REQUIRE(decimal.getPrecision() == 0);
                REQUIRE_FALSE(decimal.getIsNegative());
            }
        }

        SECTION("Equality") {
            SECTION("true") {
                Decimal d1("123.456");
                Decimal d2("123.456");
                REQUIRE(d1 == d2);
            }

            SECTION("false") {
                Decimal d1("123.456");
                Decimal d2("123.457");
                REQUIRE_FALSE(d1 == d2);
            }

        }

        SECTION("Inequality") {
            SECTION("true") {
                Decimal d1("123.456");
                Decimal d2("123.457");
                REQUIRE(d1 != d2);
            }

            SECTION("false") {
                Decimal d1("123.456");
                Decimal d2("123.456");
                REQUIRE_FALSE(d1 != d2);
            }
        }

        SECTION("Less Than") {
            SECTION("true") {
                Decimal d1("123.456");
                Decimal d2("123.457");
                REQUIRE(d1 < d2);
            }

            SECTION("false") {
                Decimal d1("123.456");
                Decimal d2("123.455");
                REQUIRE_FALSE(d1 < d2);
            }

            SECTION("Less Than with negative numbers") {
                Decimal d1("-123.456");
                Decimal d2("-123.455");
                REQUIRE(d1 < d2);
            }
        }

        SECTION("Less Than or Equal") {
            SECTION("true") {
                Decimal d1("123.456");
                Decimal d2("123.456");
                REQUIRE(d1 <= d2);
            }

            SECTION("false") {
                Decimal d1("123.456");
                Decimal d2("123.455");
                REQUIRE_FALSE(d1 <= d2);
            }
        }

        SECTION("Greater Than") {
            SECTION("true") {
                Decimal d1("123.457");
                Decimal d2("123.456");
                REQUIRE(d1 > d2);
            }

            SECTION("false") {
                Decimal d1("123.455");
                Decimal d2("123.456");
                REQUIRE_FALSE(d1 > d2);
            }

            SECTION("Greater Than with negative numbers") {
                Decimal d1("-123.455");
                Decimal d2("-123.456");
                REQUIRE(d1 > d2);
            }
        }

       SECTION("Greater Than or Equal") {
            SECTION("true") {
                Decimal d1("123.456");
                Decimal d2("123.456");
                REQUIRE(d1 >= d2);
            }

            SECTION("false") {
                Decimal d1("123.455");
                Decimal d2("123.456");
                REQUIRE_FALSE(d1 >= d2);
            }
        }

        SECTION("Addition") {
            SECTION("all positive") {
                Decimal d1("123.456");
                Decimal d2("789.123");
                Decimal result = d1 + d2;
                REQUIRE(result.toString() == "912.579");
            }

            SECTION("all negative") {
                Decimal d5("-123.456");
                Decimal d6("-789.123");
                REQUIRE((d5 + d6).toString() == "-912.579");
            }

            SECTION("one negative") {
                Decimal d3("-123.456");
                Decimal d4("789.123");
                REQUIRE((d3 + d4).toString() == "665.667");
            }

            SECTION("Addition with zero") {
                Decimal d1("123.456");
                Decimal d2("0");
                REQUIRE((d1 + d2).toString() == "123.456");
            }
        }

        SECTION("Subtraction") {
            SECTION("all positive") {
                Decimal d1("789.123");
                Decimal d2("123.456");
                Decimal result = d1 - d2;
                REQUIRE(result.toString() == "665.667");
            }

            SECTION("all negative") {
                Decimal d5("-789.123");
                Decimal d6("-123.456");
                REQUIRE((d5 - d6).toString() == "-665.667");
            }

            SECTION("one negative") {
                Decimal d3("789.123");
                Decimal d4("-123.456");
                REQUIRE((d3 - d4).toString() == "912.579");
            }

            SECTION("one negative") {
                Decimal d3("-789.123");
                Decimal d4("123.456");
                REQUIRE((d3 - d4).toString() == "-912.579");
            }

            SECTION("Subtraction with zero") {
                Decimal d1("123.456");
                Decimal d2("0");
                REQUIRE((d1 - d2).toString() == "123.456");
            }
        }
/*
        SECTION("Multiplication") {
            SECTION("all positive") {
                Decimal d1("123.456");
                Decimal d2("2");
                Decimal result = d1 * d2;
                REQUIRE(result.toString() == "246.912");
            }

            SECTION("all negative") {
                Decimal d5("-123.456");
                Decimal d6("-2");
                REQUIRE((d5 * d6).toString() == "246.912");
            }

            SECTION("one negative") {
                Decimal d3("123.456");
                Decimal d4("-2");
                REQUIRE((d3 * d4).toString() == "-246.912");
            }

            SECTION("to zero") {
                Decimal d7("123.456");
                Decimal d8("0");
                REQUIRE((d7 * d8).toString() == "0");
            }

            SECTION("Multiplication by one") {
                Decimal d1("123.456");
                Decimal d2("1");
                REQUIRE((d1 * d2).toString() == "123.456");
            }
        }
        */

 /*       SECTION("Division") {
            SECTION("all positive") {
                Decimal d1("123.456");
                Decimal d2("2");
                Decimal result = d1 / d2;
                REQUIRE(result.toString() == "61.728");
            }

            SECTION("all negative") {
                Decimal d5("-123.456");
                Decimal d6("-2");
                REQUIRE((d5 / d6).toString() == "61.728");
            }

            SECTION("one negative") {
                Decimal d3("123.456");
                Decimal d4("-2");
                REQUIRE((d3 / d4).toString() == "-61.728");
            }

            SECTION("to zero") {
                Decimal d7("123.456");
                Decimal d8("0");
                REQUIRE_THROWS_AS(d7 / d8, std::invalid_argument);
            }

            SECTION("Division by one") {
                Decimal d1("123.456");
                Decimal d2("1");
                REQUIRE((d1 / d2).toString() == "123.456");
            }
        }*/
    }

    SECTION("Decimal Utility Functions") {
        SECTION("Round") {
            Decimal d("999.999455");
            d.round(3);
            REQUIRE(d.toString() == "1000.000");
        }

        SECTION("Round to zero precision") {
            Decimal d("123.456");
            d.round(0);
            REQUIRE(d.toString() == "123");
        }

        SECTION("Set zero precision") {
            Decimal d("123.456");
            d.setPrecision(0);
            REQUIRE(d.toString() == "123");
        }

        SECTION("Set Precision >") {
            Decimal d("123.456");
            d.setPrecision(5);
            REQUIRE(d.toString() == "123.45600");
        }

        SECTION("Set Precision ==") {
            Decimal d("123.456");
            d.setPrecision(3);
            REQUIRE(d.toString() == "123.456");
        }

        SECTION("Set Precision <") {
            Decimal d("123.456339955");
            d.setPrecision(7);
            REQUIRE(d.toString() == "123.4563400");
        }

        SECTION("To Int") {
            Decimal d("123.456");
            REQUIRE(d.toInt() == 123);
        }

        SECTION("To Int with negative number") {
            Decimal d("-123.456");
            REQUIRE(d.toInt() == -123);
        }

        SECTION("To Double") {
            Decimal d("123.456");
            REQUIRE(d.toDouble() == Catch::Approx(123.456));
        }

        SECTION("To Double with negative number") {
            Decimal d("-123.456");
            REQUIRE(d.toDouble() == Catch::Approx(-123.456));
        }
    }

    SECTION("Decimal Input/Output") {
        SECTION("Output Stream") {
            Decimal d("123.456");
            std::ostringstream oss;
            oss << d;
            REQUIRE(oss.str() == "123.456");
        }

        SECTION("Input Stream") {
            Decimal d;
            std::istringstream iss("123.456");
            iss >> d;
            REQUIRE(d.toString() == "123.456");
        }

        SECTION("Invalid input") {
            Decimal d;
            std::istringstream iss("abc");
            iss >> d;
            REQUIRE(d.toString() == "0");
        }
    }

    SECTION("Geters") {
        SECTION ("Getters for double number") {
            Decimal decimal{"-132.4444449"};

            SECTION("getIntegerPart")
                REQUIRE(decimal.getIntegerPart() == "132");

            SECTION("getFractionalPart")
                REQUIRE(decimal.getFractionalPart() == "4444449");

            SECTION("getPrecision")
                REQUIRE(decimal.getPrecision() == 7);

            SECTION("getIsNegative")
                REQUIRE(decimal.getIsNegative() == true);
        }

        SECTION("Getters for integer number") {
            Decimal d("123");

            SECTION("getIntegerPart")
                REQUIRE(d.getIntegerPart() == "123");

            SECTION("getFractionalPart")
                REQUIRE(d.getFractionalPart() == "");

            SECTION("getPrecision")
                REQUIRE(d.getPrecision() == 0);

            SECTION("getIsNegative")
                REQUIRE(d.getIsNegative() == false);
        }
    }
}

int main(int argc, char* argv[]) {
    return Catch::Session().run(argc, argv);
}