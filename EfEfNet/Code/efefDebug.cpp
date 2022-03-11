#include "efefDebug.h"

#include "efefPrivateGlobals.h"

#ifdef EFEF_DEBUG
    #include <iostream>
#endif

int efef::DebugError(str str)
{
#ifdef EFEF_DEBUG
    int error = WSAGetLastError();
    std::cout << str << " -> " << error << '\n';
#endif
    return EFEF_ERROR;
}
