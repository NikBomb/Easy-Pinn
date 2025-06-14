add_library(easypinn SHARED
    src/finiteDifferences.cpp
    src/PINN.cpp
)

target_include_directories(easypinn
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_definitions(easypinn
    PUBLIC EASYPINN_EXPORTS
)

target_compile_options(easypinn PRIVATE ${EASYPINN_WARNINGS})

install(TARGETS easypinn
        EXPORT EasyPinnTargets
        LIBRARY DESTINATION lib COMPONENT library
        ARCHIVE DESTINATION lib COMPONENT library
        RUNTIME DESTINATION bin COMPONENT library
        INCLUDES DESTINATION include)

install(DIRECTORY include/easypinn/public
        DESTINATION include/easypinn
        COMPONENT library)

install(EXPORT EasyPinnTargets
        FILE EasyPinnTargets.cmake
        NAMESPACE EasyPinn::
        DESTINATION lib/cmake/EasyPinn
        COMPONENT library)

