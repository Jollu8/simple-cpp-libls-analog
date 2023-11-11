/// Tests

#include "variant.h"


#include <sstream>
#include <iostream>
#include <functional>

 struct Foo {
    int field;
};


void Check(bool v, int line, const char *condition) {
    if (!v) {
        std::stringstream ss;
        ss << "Error at ine " << line << ": " << condition;
        throw std::runtime_error(ss.str());
    }
}

#define CHECK(cond) Check(cond, __LINE__, #cond)

void Exception_expected(std::function<void()> func) {
    bool hasException = false;
    try {
        func();
    } catch (const std::exception &) {
        hasException = true;
    }
    CHECK(hasException);
}

int main() {
    try {
        {
            Variant a = Variant(10);
            CHECK(a.value<int>() == 10);
        }

        {
            Variant a;
            CHECK(a.isType<int>() == false);
        }
        {
            Variant a;
            Exception_expected([&]() { ;
                CHECK(a.valueRef<int>() == 0);
            });
        }

        {
            Variant a;
            CHECK(a.value<int>() == 0);
        }

        {
            Variant a;
            CHECK(a.isEmpty());
            a = 10;
            CHECK(!a.isEmpty());
            a.clear();
            CHECK(a.isEmpty());
        }

        {
            auto a = Variant::fromValue(10);
            CHECK(a.value<int>() == 10);
        }

        {
            Variant a = Variant( 10);
            const Variant& b(a);
            CHECK(b.value<int>() == 10);
        }


    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception " << std::endl;
        return 2;
    }
    std::cout << "All test completed successfully " << std::endl;

}
