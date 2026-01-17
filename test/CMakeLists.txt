add_executable(easypinn_tests test_easypinn.cpp)
target_link_libraries(easypinn_tests PRIVATE easypinn doctest::doctest)

add_test(NAME EasyPinnTests COMMAND easypinn_tests)
