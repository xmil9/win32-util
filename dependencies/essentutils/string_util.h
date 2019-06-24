//
// essentutils
// String utilities.
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#include "generic_string_util.h"
#include "sutils_api.h"
#include <algorithm>
#include <iterator>
#include <locale>
#include <optional>
#include <string>
#include <vector>


namespace sutil
{
///////////////////

// Interface

bool startsWith(const std::string& s, const std::string& start);
bool startsWith(const std::wstring& s, const std::wstring& start);
bool endsWith(const std::string& s, const std::string& tail);
bool endsWith(const std::wstring& s, const std::wstring& tail);
std::string lowercase(const std::string& s);
std::wstring lowercase(const std::wstring& s);
std::string uppercase(const std::string& s);
std::wstring uppercase(const std::wstring& s);
std::string trim(const std::string& s, char ch);
std::wstring trim(const std::wstring& s, wchar_t ch);
std::string trimLeft(const std::string& s, char ch);
std::wstring trimLeft(const std::wstring& s, wchar_t ch);
std::string trimRight(const std::string& s, char ch);
std::wstring trimRight(const std::wstring& s, wchar_t ch);
std::vector<std::string> split(const std::string& s, const std::string& separator);
std::vector<std::wstring> split(const std::wstring& s, const std::wstring& separator);
template <typename Iter>
std::string join(Iter it, Iter end, const std::string& separator = "");
template <typename Iter>
std::wstring join(Iter it, Iter end, const std::wstring& separator = "");

template <typename Int> Int intFromStrThrow(const std::string& s);
template <typename Int> Int intFromStrThrow(const std::wstring& s);
template <typename Int> Int intFromStr(const std::string& s, Int defaultValue) noexcept;
template <typename Int> Int intFromStr(const std::wstring& s, Int defaultValue) noexcept;
template <typename Int> std::optional<Int> intFromStr(const std::string& s) noexcept;
template <typename Int> std::optional<Int> intFromStr(const std::wstring& s) noexcept;
template <typename FP> FP fpFromStrThrow(const std::string& s);
template <typename FP> FP fpFromStrThrow(const std::wstring& s);
template <typename FP> FP fpFromStr(const std::string& s, FP defaultValue) noexcept;
template <typename FP> FP fpFromStr(const std::wstring& s, FP defaultValue) noexcept;
template <typename FP> std::optional<FP> fpFromStr(const std::string& s) noexcept;
template <typename FP> std::optional<FP> fpFromStr(const std::wstring& s) noexcept;

SUTILS_API std::string utf8(const std::string& s);
SUTILS_API std::string utf8(const std::wstring& s);
SUTILS_API std::wstring utf16(const std::string& s);
SUTILS_API std::wstring utf16(const std::wstring& s);


///////////////////

// Implementation

inline bool startsWith(const std::string& s, const std::string& start)
{
   return genstr::startsWith(s, start);
}


inline bool startsWith(const std::wstring& s, const std::wstring& start)
{
   return genstr::startsWith(s, start);
}


inline bool endsWith(const std::string& s, const std::string& tail)
{
   return genstr::endsWith(s, tail);
}


inline bool endsWith(const std::wstring& s, const std::wstring& tail)
{
   return genstr::endsWith(s, tail);
}


inline std::string lowercase(const std::string& s)
{
   std::locale defaultLocale;
   std::string lower;
   std::transform(s.begin(), s.end(), back_inserter(lower),
                  [&defaultLocale](char ch) { return std::tolower(ch, defaultLocale); });
   return lower;
}


inline std::wstring lowercase(const std::wstring& s)
{
   std::locale utf8Locale{"en_US.utf8"};
   std::wstring lower;
   std::transform(s.begin(), s.end(), back_inserter(lower),
                  [&utf8Locale](wchar_t ch) { return std::tolower(ch, utf8Locale); });
   return lower;
}


inline std::string uppercase(const std::string& s)
{
   std::locale defaultLocale;
   std::string upper;
   std::transform(s.begin(), s.end(), back_inserter(upper),
                  [&defaultLocale](char ch) { return std::toupper(ch, defaultLocale); });
   return upper;
}


inline std::wstring uppercase(const std::wstring& s)
{
   std::locale utf8Locale{"en_US.utf8"};
   std::wstring upper;
   std::transform(s.begin(), s.end(), back_inserter(upper),
                  [&utf8Locale](wchar_t ch) { return std::toupper(ch, utf8Locale); });
   return upper;
}


inline std::string trim(const std::string& s, char ch)
{
   return genstr::trim(s, ch);
}


inline std::wstring trim(const std::wstring& s, wchar_t ch)
{
   return genstr::trim(s, ch);
}


inline std::string trimLeft(const std::string& s, char ch)
{
   return genstr::trimLeft(s, ch);
}


inline std::wstring trimLeft(const std::wstring& s, wchar_t ch)
{
   return genstr::trimLeft(s, ch);
}


inline std::string trimRight(const std::string& s, char ch)
{
   return genstr::trimRight(s, ch);
}


inline std::wstring trimRight(const std::wstring& s, wchar_t ch)
{
   return genstr::trimRight(s, ch);
}


inline std::vector<std::string> split(const std::string& s, const std::string& separator)
{
   return genstr::split(s, separator);
}


inline std::vector<std::wstring> split(const std::wstring& s,
                                       const std::wstring& separator)
{
   return genstr::split(s, separator);
}


template <typename Iter>
std::string join(Iter it, Iter end, const std::string& separator)
{
   return genstr::join(it, end, separator);
}


template <typename Iter>
std::wstring join(Iter it, Iter end, const std::wstring& separator)
{
   return genstr::join(it, end, separator);
}


template <typename Int> Int intFromStrThrow(const std::string& s)
{
   return genstr::intFromStrThrow<Int, std::string>(s);
}


template <typename Int> Int intFromStrThrow(const std::wstring& s)
{
   return genstr::intFromStrThrow<Int, std::wstring>(s);
}


template <typename Int> Int intFromStr(const std::string& s, Int defaultValue) noexcept
{
   return genstr::intFromStr(s, defaultValue);
}


template <typename Int> Int intFromStr(const std::wstring& s, Int defaultValue) noexcept
{
   return genstr::intFromStr(s, defaultValue);
}


template <typename Int> std::optional<Int> intFromStr(const std::string& s) noexcept
{
   return genstr::intFromStr<Int, std::string>(s);
}


template <typename Int> std::optional<Int> intFromStr(const std::wstring& s) noexcept
{
   return genstr::intFromStr<Int, std::wstring>(s);
}


template <typename FP> FP fpFromStrThrow(const std::string& s)
{
   return genstr::fpFromStrThrow<FP, std::string>(s);
}


template <typename FP> FP fpFromStrThrow(const std::wstring& s)
{
   return genstr::fpFromStrThrow<FP, std::wstring>(s);
}


template <typename FP> FP fpFromStr(const std::string& s, FP defaultValue) noexcept
{
   return genstr::fpFromStr(s, defaultValue);
}


template <typename FP> FP fpFromStr(const std::wstring& s, FP defaultValue) noexcept
{
   return genstr::fpFromStr(s, defaultValue);
}


template <typename FP> std::optional<FP> fpFromStr(const std::string& s) noexcept
{
   return genstr::fpFromStr<FP, std::string>(s);
}


template <typename FP> std::optional<FP> fpFromStr(const std::wstring& s) noexcept
{
   return genstr::fpFromStr<FP, std::wstring>(s);
}

} // namespace sutil
