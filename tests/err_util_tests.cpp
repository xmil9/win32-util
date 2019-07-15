//
// Win32 utilities library
// Tests for error utilities.
//
// Jun-2019, Michael Lindner
// MIT license
//
#include "err_util_tests.h"
#include "err_util.h"
#include "test_util.h"

using namespace win32;


namespace
{
///////////////////

void testComErrorText()
{
   {
      const std::string caseLabel{"comErrorText for existing error"};
      VERIFY(comErrorText<std::string>(OLE_E_INVALIDHWND) == "Invalid window handle",
             caseLabel);
      VERIFY(comErrorText<std::string>(RPC_E_TIMEOUT) ==
                "This operation returned because the timeout period expired.",
             caseLabel);
   }
   {
      const std::string caseLabel{"comErrorText for not existing error"};
      VERIFY(comErrorText<std::string>(100000000) == "Unknown error 0x5F5E100",
             caseLabel);
   }
   {
      const std::string caseLabel{"comErrorText with std::wstring"};
      VERIFY(comErrorText<std::wstring>(OLE_E_INVALIDHWND) == L"Invalid window handle",
             caseLabel);
   }
}


void testWinErrorText()
{
   {
      const std::string caseLabel{"winErrorText for existing error"};
      VERIFY(winErrorText<std::string>(ERROR_FILE_NOT_FOUND) ==
                "The system cannot find the file specified.",
             caseLabel);
      VERIFY(winErrorText<std::string>(ERROR_LOCKED) ==
                "The segment is locked and cannot be reallocated.",
             caseLabel);
   }
   {
      const std::string caseLabel{"winErrorText for not existing error"};
      VERIFY(winErrorText<std::string>(100000000) ==
                "<<Failed to get description of Windows error.>>",
             caseLabel);
   }
   {
      const std::string caseLabel{"winErrorText with std::wstring"};
      VERIFY(winErrorText<std::wstring>(ERROR_FILE_NOT_FOUND) ==
                L"The system cannot find the file specified.",
             caseLabel);
   }
}


void testLastWinErrorText()
{
   {
      const std::string caseLabel{"lastWinErrorText for existing error"};
      SetLastError(ERROR_ACCESS_DENIED);
      VERIFY(lastWinErrorText<std::string>() == "Access is denied.", caseLabel);
      SetLastError(ERROR_SUCCESS);
   }
   {
      const std::string caseLabel{"lastWinErrorText for not existing error"};
      SetLastError(ERROR_SUCCESS);
      VERIFY(lastWinErrorText<std::string>("default text") == "default text", caseLabel);
   }
   {
      const std::string caseLabel{
         "lastWinErrorText for not existing error without default text"};
      SetLastError(ERROR_SUCCESS);
      VERIFY(lastWinErrorText<std::string>() == "", caseLabel);
   }
   {
      const std::string caseLabel{"lastWinErrorText with std::wstring"};
      SetLastError(ERROR_ACCESS_DENIED);
      VERIFY(lastWinErrorText<std::wstring>() == L"Access is denied.", caseLabel);
      SetLastError(ERROR_SUCCESS);
   }
}

} // namespace


void testErrUtil()
{
   testComErrorText();
   testWinErrorText();
   testLastWinErrorText();
}
