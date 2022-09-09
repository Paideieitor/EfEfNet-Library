#include "efefDebug.h"

#include "efefPrivateGlobals.h"

int efef::DebugError(wstr str)
{
#ifdef EFEF_DEBUG
    int error = WSAGetLastError();
    //render->Log(str);
    wchar_t s[16];
    _itow_s(error, s, 16, 10);
    //render->Log(s);
#endif
    return EFEF_ERROR;
}
