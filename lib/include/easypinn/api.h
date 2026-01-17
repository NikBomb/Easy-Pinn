#ifndef EASYPINN_API_H
#define EASYPINN_API_H

#if defined(_WIN32) || defined(_WIN64)
    #ifdef EASYPINN_EXPORTS
        #define EASYPINN_API __declspec(dllexport)
    #else
        #define EASYPINN_API __declspec(dllimport)
    #endif
#else
    #define EASYPINN_API __attribute__((visibility("default")))
#endif

#endif  // EASYPINN_API_H
