#include "test_util.h"
#include "win32_windows.h"
#include <string>


static std::string composeErrorMessage(const std::string& label,
                                       const std::string& condStr,
                                       const std::string& fileName, int lineNum)
{
   std::string msg;
   msg += "Failed - ";
   msg += label;
   msg += ": '";
   msg += condStr;
   msg += "'.";
   msg += " (file: ";
   msg += fileName;
   msg += ", line: ";
   msg += std::to_string(lineNum);
   msg += ").";
   return msg;
}


bool verify(bool cond, const std::string& label, const std::string& condStr,
            const std::string& fileName, int lineNum)
{
   if (!cond)
      MessageBoxA(NULL, composeErrorMessage(label, condStr, fileName, lineNum).c_str(),
                  "Test failure", MB_OK);
   return cond;
}
