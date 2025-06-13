#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #ifdef EASYPINN_EXPORTS
        #define EASYPINN_API __declspec(dllexport)
    #else
        #define EASYPINN_API __declspec(dllimport)
    #endif
#else
    #define EASYPINN_API __attribute__((visibility("default")))
#endif
