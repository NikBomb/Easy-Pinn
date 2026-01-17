add_executable(my_tool my_tool/main.cpp)
target_link_libraries(my_tool PRIVATE easypinn)

add_executable(problem0_app problem0/problem0_main.cpp)
target_include_directories(problem0_app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/problem0)
target_link_libraries(problem0_app PRIVATE easypinn)

add_executable(problem0_tests problem0/problem0_tests.cpp)
target_include_directories(problem0_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/problem0)
target_link_libraries(problem0_tests PRIVATE easypinn doctest::doctest)
add_test(NAME Problem0Tests COMMAND problem0_tests)
