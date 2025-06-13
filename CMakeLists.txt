cmake_minimum_required(VERSION 3.15)
project(easyPinn VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/CompilerWarnings.cmake)

add_subdirectory(lib)
add_subdirectory(app)
enable_testing()
add_subdirectory(test)
include(CMakePackageConfigHelpers)


configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/EasyPinnConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/EasyPinnConfig.cmake
    INSTALL_DESTINATION lib/cmake/EasyPinn
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EasyPinnConfig.cmake
        DESTINATION lib/cmake/EasyPinn
	COMPONENT library)

