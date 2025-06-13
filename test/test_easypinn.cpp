#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include "easypinn/public/MyClass.h"

TEST_CASE("MyClass::doSomething does not throw") {
    MyClass obj;
    CHECK_NOTHROW(obj.doSomething());
}
