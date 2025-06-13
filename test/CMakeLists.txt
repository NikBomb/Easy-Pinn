include(FetchContent)

FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG v2.4.11
)

FetchContent_MakeAvailable(doctest)

add_executable(easypinn_tests test_easypinn.cpp)
target_link_libraries(easypinn_tests PRIVATE easypinn doctest::doctest)

add_test(NAME EasyPinnTests COMMAND easypinn_tests)

