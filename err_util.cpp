//
// Win32 utilities library
// Error utilities.
//
// Jun-2019, Michael Lindner
// MIT license
//
#ifdef _WIN32
#include "err_util.h"
#include <tchar.h>


namespace
{
///////////////////

// RAII class for freeing memory allocated by LocalAlloc.
// Implemented locally to keep the err_util component as free of dependencies as
// possible.
class LocalFreeGuard
{
 public:
   LocalFreeGuard() = delete;
   LocalFreeGuard(LPTSTR p) : m_ptr{p} {}
   ~LocalFreeGuard()
   {
      if (m_ptr)
         LocalFree(m_ptr);
   }
   LocalFreeGuard(const LocalFreeGuard&) = delete;
   LocalFreeGuard(LocalFreeGuard&&) = default;
   LocalFreeGuard& operator=(const LocalFreeGuard&) = delete;
   LocalFreeGuard& operator=(LocalFreeGuard&&) = default;

 private:
   LPTSTR m_ptr = nullptr;
};

} // namespace


namespace win32
{
namespace detail
{
///////////////////

// Returns error text for given Win32 error code.
String winErrorNativeText(DWORD errCode)
{
   // Pointer to a buffer that will be allocated by FormatMessage().
   TCHAR* buffer = nullptr;
   const DWORD charsWritten =
      ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                         FORMAT_MESSAGE_IGNORE_INSERTS,
                      nullptr, errCode, 0, (LPTSTR)&buffer,
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), nullptr);
   if (charsWritten == 0)
      return _T("<<Failed to get description of Windows error.>>");
   // FormatMessage documentation states to free returned memory with LocalFree.
   LocalFreeGuard memGuard(buffer);

   String errText = buffer;

   // Win32 API error messages sometimes contain newline character sequences at
   // the end. Strip them off.
   const String Newline = _T("\x0d\x0a");
   const String::size_type pos = errText.rfind(Newline);
   if (pos != String::npos)
      errText.replace(pos, Newline.size(), _T(""));

   return errText;
}

} // namespace detail
} // namespace win32

#endif //_WIN32
