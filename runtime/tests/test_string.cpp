//
// Created by meian on 2026/3/28.
//

#include <gtest/gtest.h>
#include "../object/fraction.hpp"
#include "../object/string.hpp"
#include "../object/value.hpp"

TEST(TestString, Test) {
    using namespace lmx::runtime;

    String s("Hello");
    s += " World";

    String s3;
    auto s2(s);
    std::cout << s << std::endl << s2 << std::endl;
    std::cout << s3 << std::endl;
    ASSERT_TRUE(s == String("Hello World"));
}