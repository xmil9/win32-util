#include "tstring_tests.h"
#include "tstring.h"
#include "test_util.h"
#include <tchar.h>

using namespace win32;


namespace
{

///////////////////

void testStringTraitsForWchar_t()
{
   {
      const std::string caseLabel{"StringTraits<wchar_t>::to_string for int"};
      VERIFY(StringTraits<wchar_t>::to_string(static_cast<int>(5)) == L"5", caseLabel);
   }
   {
      const std::string caseLabel{"StringTraits<wchar_t>::to_string for unsigned int"};
      VERIFY(StringTraits<wchar_t>::to_string(static_cast<unsigned int>(5)) == L"5",
             caseLabel);
   }
   {
      const std::string caseLabel{"StringTraits<wchar_t>::to_string for long"};
      VERIFY(StringTraits<wchar_t>::to_string(static_cast<long>(5)) == L"5", caseLabel);
   }
   {
      const std::string caseLabel{"StringTraits<wchar_t>::to_string for unsigned long"};
      VERIFY(StringTraits<wchar_t>::to_string(static_cast<unsigned long>(5)) == L"5",
             caseLabel);
   }
   {
      const std::string caseLabel{"StringTraits<wchar_t>::to_string for long long"};
      VERIFY(StringTraits<wchar_t>::to_string(static_cast<long long>(5)) == L"5",
             caseLabel);
   }
   {
      const std::string caseLabel{
         "StringTraits<wchar_t>::to_string for unsigned long long"};
      VERIFY(StringTraits<wchar_t>::to_string(static_cast<unsigned long long>(5)) == L"5",
             caseLabel);
   }
   {
      const std::string caseLabel{"StringTraits<wchar_t>::to_string for float"};
      VERIFY(StringTraits<wchar_t>::to_string(static_cast<float>(5.1)) == L"5.100000",
             caseLabel);
   }
   {
      const std::string caseLabel{"StringTraits<wchar_t>::to_string for double"};
      VERIFY(StringTraits<wchar_t>::to_string(static_cast<double>(5.1)) == L"5.100000",
             caseLabel);
   }
   {
      const std::string caseLabel{"StringTraits<wchar_t>::to_string for long double"};
      VERIFY(StringTraits<wchar_t>::to_string(static_cast<long double>(5.1)) ==
                L"5.100000",
             caseLabel);
   }
}


void testStringTraitsForChar()
{
   {
      const std::string caseLabel{"StringTraits<char>::to_string for int"};
      VERIFY(StringTraits<char>::to_string(static_cast<int>(5)) == "5", caseLabel);
   }
   {
      const std::string caseLabel{"StringTraits<char>::to_string for unsigned int"};
      VERIFY(StringTraits<char>::to_string(static_cast<unsigned int>(5)) == "5",
             caseLabel);
   }
   {
      const std::string caseLabel{"StringTraits<char>::to_string for long"};
      VERIFY(StringTraits<char>::to_string(static_cast<long>(5)) == "5", caseLabel);
   }
   {
      const std::string caseLabel{"StringTraits<char>::to_string for unsigned long"};
      VERIFY(StringTraits<char>::to_string(static_cast<unsigned long>(5)) == "5",
             caseLabel);
   }
   {
      const std::string caseLabel{"StringTraits<char>::to_string for long long"};
      VERIFY(StringTraits<char>::to_string(static_cast<long long>(5)) == "5", caseLabel);
   }
   {
      const std::string caseLabel{"StringTraits<char>::to_string for unsigned long long"};
      VERIFY(StringTraits<char>::to_string(static_cast<unsigned long long>(5)) == "5",
             caseLabel);
   }
   {
      const std::string caseLabel{"StringTraits<char>::to_string for float"};
      VERIFY(StringTraits<char>::to_string(static_cast<float>(5.1)) == "5.100000",
             caseLabel);
   }
   {
      const std::string caseLabel{"StringTraits<char>::to_string for double"};
      VERIFY(StringTraits<char>::to_string(static_cast<double>(5.1)) == "5.100000",
             caseLabel);
   }
   {
      const std::string caseLabel{"StringTraits<char>::to_string for long double"};
      VERIFY(StringTraits<char>::to_string(static_cast<long double>(5.1)) == "5.100000",
             caseLabel);
   }
}


void testToString()
{
   {
      const std::string caseLabel{"to_string for int"};
      VERIFY(to_string(static_cast<int>(5)) == _T("5"), caseLabel);
   }
   {
      const std::string caseLabel{"to_string for unsigned int"};
      VERIFY(to_string(static_cast<unsigned int>(5)) == _T("5"), caseLabel);
   }
   {
      const std::string caseLabel{"to_string for long"};
      VERIFY(to_string(static_cast<long>(5)) == _T("5"), caseLabel);
   }
   {
      const std::string caseLabel{"to_string for unsigned long"};
      VERIFY(to_string(static_cast<unsigned long>(5)) == _T("5"), caseLabel);
   }
   {
      const std::string caseLabel{"to_string for long long"};
      VERIFY(to_string(static_cast<long long>(5)) == _T("5"), caseLabel);
   }
   {
      const std::string caseLabel{"to_string for unsigned long long"};
      VERIFY(to_string(static_cast<unsigned long long>(5)) == _T("5"), caseLabel);
   }
   {
      const std::string caseLabel{"to_string for float"};
      VERIFY(to_string(static_cast<float>(5.1)) == _T("5.100000"), caseLabel);
   }
   {
      const std::string caseLabel{"to_string for double"};
      VERIFY(to_string(static_cast<double>(5.1)) == _T("5.100000"), caseLabel);
   }
   {
      const std::string caseLabel{"to_string for long double"};
      VERIFY(to_string(static_cast<long double>(5.1)) == _T("5.100000"), caseLabel);
   }
}

} // namespace


void testTString(HWND /*testRunnerWnd*/)
{
   testStringTraitsForWchar_t();
   testStringTraitsForChar();
   testToString();
}
