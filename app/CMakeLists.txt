add_executable(my_tool my_tool/main.cpp)
target_link_libraries(my_tool PRIVATE easypinn)

add_executable(problem0_app problem0_main.cpp)
target_link_libraries(problem0_app PRIVATE easypinn)
