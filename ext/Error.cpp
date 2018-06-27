#include "nbind/api.h"

#include <windows.h>
#include <strsafe.h>
#include <tchar.h>

std::string WError(LPTSTR lpszFunction)
{
  // Retrieve the system error message for the last-error code

  LPVOID lpMsgBuf;
  LPVOID lpDisplayBuf = NULL;
  DWORD dw = GetLastError();

  FormatMessage(
      FORMAT_MESSAGE_ALLOCATE_BUFFER |
          FORMAT_MESSAGE_FROM_SYSTEM |
          FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL,
      dw,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      (LPTSTR)&lpMsgBuf,
      0, NULL);

  // Display the error message and exit the process

  lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
                                    (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
  StringCchPrintf((LPTSTR)lpDisplayBuf,
                  LocalSize(lpDisplayBuf) / sizeof(TCHAR),
                  TEXT("[%lx] %s%s"),
                  dw, lpMsgBuf, lpszFunction);
  std::string result((const char *)lpDisplayBuf);

  LocalFree(lpMsgBuf);
  LocalFree(lpDisplayBuf);

  return result;
}

void JSError(LPTSTR lpszFunction)
{
  NBIND_ERR(WError(lpszFunction).c_str());
}
