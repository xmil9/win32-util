//
// essentutils
// String utilities.
//
// Jun-2019, Michael Lindner
// MIT license
//
#include "string_util.h"
#include <algorithm>
#include <clocale>
#include <cstddef>
#include <cstdlib>

using namespace std;


namespace
{
///////////////////

// RAII class for setting the global locale to UTF8 for the lifetime of
// an instance.
class Utf8Locale
{
 public:
   Utf8Locale();
   ~Utf8Locale();
   Utf8Locale(const Utf8Locale&) = delete;
   Utf8Locale(Utf8Locale&&) = default;
   Utf8Locale& operator=(const Utf8Locale&) = delete;
   Utf8Locale& operator=(Utf8Locale&&) = default;

 private:
   std::string m_prevLocale;
};


Utf8Locale::Utf8Locale()
{
   char* prevLocale = std::setlocale(LC_ALL, "en_US.utf8");
   if (prevLocale)
      m_prevLocale = prevLocale;
}

Utf8Locale::~Utf8Locale()
{
   if (!m_prevLocale.empty())
      std::setlocale(LC_ALL, m_prevLocale.c_str());
}

} // namespace


namespace sutil
{
///////////////////

std::string utf8(const std::string& s)
{
   // Nothing to do but it's convenient to have the overload available to
   // reduce checks in calling code.
   return s;
}


std::string utf8(const std::wstring& s)
{
   if (s.empty())
      return "";

   Utf8Locale utf8Locale;

   vector<char> buffer(s.size() * 4, 0);
   const size_t numBytesWritten = wcstombs(buffer.data(), s.c_str(), buffer.size());
   if (numBytesWritten == -1)
      return "";

   return buffer.data();
}


std::wstring utf16(const std::string& s)
{
   if (s.empty())
      return L"";

   Utf8Locale utf8Locale;

   vector<wchar_t> buffer(s.size() * 4, 0);
   const size_t numBytesWritten = mbstowcs(buffer.data(), s.c_str(), buffer.size());
   if (numBytesWritten == -1)
      return L"";

   return buffer.data();
}


std::wstring utf16(const std::wstring& s)
{
   // Nothing to do but it's convenient to have the overload available to
   // reduce checks in calling code.
   return s;
}

} // namespace util
