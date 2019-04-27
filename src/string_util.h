#pragma once
#ifdef _WIN32
#include <windows.h>
#include <string>

namespace win32
{

///////////////////

// Std string type based on TCHAR type.
using String = std::basic_string<TCHAR>;

// String traits for wchar_t.
template <typename Char> struct StringTraits
{
   static std::wstring to_string(int val) { return std::to_wstring(val); }
   static std::wstring to_string(unsigned int val) { return std::to_wstring(val); }
   static std::wstring to_string(long val) { return std::to_wstring(val); }
   static std::wstring to_string(unsigned long val) { return std::to_wstring(val); }
   static std::wstring to_string(long long val) { return std::to_wstring(val); }
   static std::wstring to_string(unsigned long long val) { return std::to_wstring(val); }
   static std::wstring to_string(float val) { return std::to_wstring(val); }
   static std::wstring to_string(double val) { return std::to_wstring(val); }
   static std::wstring to_string(long double val) { return std::to_wstring(val); }
};

// String traits for char.
template <> struct StringTraits<char>
{
   static std::string to_string(int val) { return std::to_string(val); }
   static std::string to_string(unsigned int val) { return std::to_string(val); }
   static std::string to_string(long val) { return std::to_string(val); }
   static std::string to_string(unsigned long val) { return std::to_string(val); }
   static std::string to_string(long long val) { return std::to_string(val); }
   static std::string to_string(unsigned long long val) { return std::to_string(val); }
   static std::string to_string(float val) { return std::to_string(val); }
   static std::string to_string(double val) { return std::to_string(val); }
   static std::string to_string(long double val) { return std::to_string(val); }
};

// Selects between std::to_string() and std::to_wstring() calls based on the TCHAR type.
template <typename Val> String to_string(Val val)
{
   return StringTraits<TCHAR>::to_string(val);
}

} // namespace win32

#endif //_WIN32
