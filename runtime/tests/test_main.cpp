//
// Created by meian on 2026/3/22.
//
#include  "gtest/gtest.h"
#include "../object/fraction.hpp"
#include "../object/string.hpp"
#include "../object/value.hpp"

TEST(TestMain, Test) {
    const lmx::runtime::Fraction fraction("100.02");
    ASSERT_EQ(fraction.to_string(), "5001/50");

    const lmx::runtime::Fraction fraction3(2, 3);
    ASSERT_EQ(fraction3.to_string(), "2/3");

    auto tmp = lmx::runtime::Fraction(15103,150);
    ASSERT_TRUE((fraction + fraction3).equals(&tmp));

    tmp = lmx::runtime::Fraction(14903,150);
    ASSERT_TRUE((fraction - fraction3).equals(&tmp));

    tmp = lmx::runtime::Fraction(10002,150);
    ASSERT_TRUE((fraction * fraction3).equals(&tmp));

    tmp = lmx::runtime::Fraction(15003,100);
    ASSERT_TRUE((fraction / fraction3).equals(&tmp));

    auto ptr = &lmx::runtime::Fraction::operator!=;
    (fraction.*ptr)(*(lmx::runtime::Fraction{4,7}.clone()));

    lmx::runtime::Value v;
}