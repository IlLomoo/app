#include "framework.h"


DWORD
WINAPI
FormatMessage(
              DWORD dwFlags,
              LPCVOID lpSource,
              DWORD dwMessageId,
              DWORD dwLanguageId,
              LPSTR lpBuffer,
              DWORD nSize,
              va_list *Arguments
              )
{


    return 0;


}


VOID
WINAPI
OutputDebugStringA(
                   LPCSTR lpOutputString
                   )
{


    fprintf(stdout, "%s", lpOutputString);


}




VOID
WINAPI
OutputDebugStringW(
                   LPCWSTR lpOutputString
                   )
{


    OutputDebugStringA(vsstring(lpOutputString));


}



