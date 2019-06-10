//
// Win32 utilities library
// Registry utilities.
//
// Jun-2019, Michael Lindner
// MIT license
//
#ifdef _WIN32
#include "registry.h"
#include <cassert>

using namespace std;


namespace
{
///////////////////

// Reads value of integer type from given registry entry.
template <typename Int>
std::optional<Int> readInt(HKEY key, const std::wstring& entryName)
{
   static_assert(is_integral_v<Int> && (sizeof(Int) == 4 || sizeof(Int) == 8));

   if (!key)
      return {};

   Int value = 0;
   DWORD numBytes = sizeof(value);
   const LSTATUS res = RegQueryValueExW(key, entryName.c_str(), nullptr, nullptr,
                                        reinterpret_cast<BYTE*>(&value), &numBytes);
   if (res != ERROR_SUCCESS)
      return {};

   assert(numBytes == sizeof(value));
   return value;
}


// Writes value of integer type to given registry entry.
template <typename Int> bool writeInt(HKEY key, const std::wstring& entryName, Int val)
{
   static_assert(is_integral_v<Int> && (sizeof(Int) == 4 || sizeof(Int) == 8));

   if (!key)
      return false;

   constexpr DWORD entryType = (sizeof(Int) == 4) ? REG_DWORD : REG_QWORD;
   const LSTATUS res = RegSetValueExW(key, entryName.c_str(), 0, entryType,
                                      reinterpret_cast<const BYTE*>(&val), sizeof(val));
   return (res == ERROR_SUCCESS);
}


// Reads value of string type from given registry entry.
template <typename Str>
std::optional<Str> readStr(HKEY key, const std::wstring& entryName)
{
   static_assert(is_same_v<Str, std::string> || is_same_v<Str, std::wstring>);
   using Char = typename Str::value_type;
   static_assert(is_same_v<Char, char> || is_same_v<Char, wchar_t>);

   if (!key)
      return {};

   DWORD numBytes = 0;
   LSTATUS res =
      RegQueryValueExW(key, entryName.c_str(), nullptr, nullptr, nullptr, &numBytes);
   if (res != ERROR_SUCCESS)
      return {};

   constexpr size_t charBytes = sizeof(Char);
   assert(numBytes % charBytes == 0);
   const size_t strLen = numBytes / charBytes;

   // '+ 1' for additional zero terminator to make sure the string is well formed.
   vector<Char> buffer(strLen + 1);
   DWORD numReadBytes = 0;
   res = RegQueryValueExW(key, entryName.c_str(), nullptr, nullptr,
                          reinterpret_cast<BYTE*>(buffer.data()), &numReadBytes);
   if (res != ERROR_SUCCESS)
      return {};

   assert(numReadBytes == numBytes);
   *buffer.rbegin() = 0;
   return buffer.data();
}


// Writes value of string type to given registry entry.
template <typename Str>
bool writeStr(HKEY key, const std::wstring& entryName, const Str& val)
{
   static_assert(is_same_v<Str, std::string> || is_same_v<Str, std::wstring>);
   using Char = typename Str::value_type;
   static_assert(is_same_v<Char, char> || is_same_v<Char, wchar_t>);

   if (!key)
      return {};

   const size_t numBytes = (val.size() + 1) * sizeof(Char);
   const LSTATUS res = RegSetValueExW(key, entryName.c_str(), 0, REG_SZ,
                                      reinterpret_cast<const BYTE*>(val.c_str()),
                                      static_cast<DWORD>(numBytes));
   return (res == ERROR_SUCCESS);
}

} // namespace


namespace win32
{
///////////////////

RegKey::RegKey(HKEY parent, const std::wstring& keyPath, REGSAM accessRights)
{
   create(parent, keyPath, accessRights);
}


RegKey::~RegKey()
{
   close();
}


RegKey::RegKey(RegKey&& other) noexcept
{
   swap(*this, other);
}


RegKey& RegKey::operator=(RegKey&& other) noexcept
{
   m_key = other.m_key;
   m_created = other.m_created;
   // Make sure dtor of moved-from object does nothing.
   other.m_key = NULL;
   other.m_created = false;
   return *this;
}


void swap(RegKey& a, RegKey& b) noexcept
{
   std::swap(a.m_key, b.m_key);
   std::swap(a.m_created, b.m_created);
}


bool RegKey::create(HKEY parent, const std::wstring& keyPath, REGSAM accessRights)
{
   close();

   DWORD disposition = 0;
   const LSTATUS res = RegCreateKeyExW(parent, keyPath.c_str(), 0, nullptr, 0,
                                       accessRights, nullptr, &m_key, &disposition);
   if (res != ERROR_SUCCESS)
   {
      m_key = NULL;
      return false;
   }

   m_created = (disposition == REG_CREATED_NEW_KEY);
   return true;
}


bool RegKey::open(HKEY parent, const std::wstring& keyPath, REGSAM accessRights)
{
   close();

   const LSTATUS res = RegOpenKeyExW(parent, keyPath.c_str(), 0, accessRights, &m_key);
   if (res != ERROR_SUCCESS)
   {
      m_key = NULL;
      return false;
   }
   return true;
}


void RegKey::close()
{
   if (m_key)
   {
      RegCloseKey(m_key);
      m_key = NULL;
   }
}


void RegKey::clear()
{
   m_key = NULL;
   m_created = false;
}


bool RegKey::keyExists(HKEY parent, const std::wstring& keyPath)
{
   RegKey key;
   return key.open(parent, keyPath, KEY_READ);
}


bool RegKey::removeKey(HKEY parent, const std::wstring& keyPath, bool wow64Bit)
{
   const LSTATUS res = RegDeleteKeyExW(parent, keyPath.c_str(),
                                       wow64Bit ? KEY_WOW64_64KEY : KEY_WOW64_32KEY, 0);
   return (res == ERROR_SUCCESS);
}


std::optional<int32_t> RegKey::readInt32(const std::wstring& entryName) const
{
   return readInt<int32_t>(m_key, entryName);
}


std::optional<int64_t> RegKey::readInt64(const std::wstring& entryName) const
{
   return readInt<int64_t>(m_key, entryName);
}


std::optional<std::string> RegKey::readString(const std::wstring& entryName) const
{
   return readStr<string>(m_key, entryName);
}

std::optional<std::wstring> RegKey::readWString(const std::wstring& entryName) const
{
   return readStr<wstring>(m_key, entryName);
}


std::size_t RegKey::readBinary(const std::wstring& entryName,
                               std::function<BYTE*(std::size_t)> getBuffer) const
{
   if (!m_key)
      return {};

   DWORD numBytes = 0;
   LSTATUS res =
      RegQueryValueExW(m_key, entryName.c_str(), nullptr, nullptr, nullptr, &numBytes);
   if (res != ERROR_SUCCESS)
      return {};

   DWORD numReadBytes = 0;
   res = RegQueryValueExW(m_key, entryName.c_str(), nullptr, nullptr, getBuffer(numBytes),
                          &numReadBytes);
   if (res != ERROR_SUCCESS)
      return {};

   return numReadBytes;
}


bool RegKey::writeInt32(const std::wstring& entryName, int32_t val) const
{
   return writeInt(m_key, entryName, val);
}


bool RegKey::writeInt64(const std::wstring& entryName, int64_t val) const
{
   return writeInt(m_key, entryName, val);
}


bool RegKey::writeString(const std::wstring& entryName, const std::string& val) const
{
   return writeStr(m_key, entryName, val);
}


bool RegKey::writeWString(const std::wstring& entryName, const std::wstring& val) const
{
   return writeStr(m_key, entryName, val);
}


bool RegKey::writeWBinary(const std::wstring& entryName, const BYTE* data,
                          std::size_t numBytes) const
{
   if (!m_key)
      return {};

   const LSTATUS res = RegSetValueExW(m_key, entryName.c_str(), 0, REG_BINARY, data,
                                      static_cast<DWORD>(numBytes));
   return (res == ERROR_SUCCESS);
}


bool RegKey::removeEntry(const std::wstring& entryName) const
{
   if (!m_key)
      return {};

   const LSTATUS res = RegDeleteValueW(m_key, entryName.c_str());
   return (res == ERROR_SUCCESS);
}


std::size_t RegKey::countSubkeys() const
{
   if (!m_key)
      return 0;

   DWORD numSubkeys = 0;
   const LSTATUS res =
      RegQueryInfoKeyW(m_key, nullptr, nullptr, nullptr, &numSubkeys, nullptr, nullptr,
                       nullptr, nullptr, nullptr, nullptr, nullptr);
   if (res != ERROR_SUCCESS)
      return 0;
   return numSubkeys;
}


std::vector<std::wstring> RegKey::subkeyNames() const
{
   if (!m_key)
      return {};

   DWORD maxSubkeyNameLen = 0;
   LSTATUS res =
      RegQueryInfoKeyW(m_key, nullptr, nullptr, nullptr, nullptr, &maxSubkeyNameLen,
                       nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
   if (res != ERROR_SUCCESS)
      return {};

   maxSubkeyNameLen += 1;
   vector<wchar_t> buffer(maxSubkeyNameLen, 0);

   vector<wstring> subkeys;
   DWORD idx = 0;
   DWORD nameLen = maxSubkeyNameLen;

   while (res == ERROR_SUCCESS)
   {
      nameLen = maxSubkeyNameLen;
      res = RegEnumKeyExW(m_key, idx++, buffer.data(), &nameLen, nullptr, nullptr,
                          nullptr, nullptr);
      if (res == ERROR_SUCCESS)
         subkeys.push_back(buffer.data());
   }

   if (res == ERROR_SUCCESS || res == ERROR_NO_MORE_ITEMS)
      subkeys;
   return {};
}


std::size_t RegKey::countEntries() const
{
   if (!m_key)
      return 0;

   DWORD numEntries = 0;
   const LSTATUS res =
      RegQueryInfoKeyW(m_key, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                       &numEntries, nullptr, nullptr, nullptr, nullptr);
   if (res != ERROR_SUCCESS)
      return 0;
   return numEntries;
}


std::vector<std::wstring> RegKey::entryNames() const
{
   if (!m_key)
      return {};

   DWORD maxEntryNameLen = 0;
   LSTATUS res =
      RegQueryInfoKeyW(m_key, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                       nullptr, &maxEntryNameLen, nullptr, nullptr, nullptr);
   if (res != ERROR_SUCCESS)
      return {};

   maxEntryNameLen += 1;
   vector<wchar_t> buffer(maxEntryNameLen, 0);

   vector<wstring> entries;
   DWORD idx = 0;
   DWORD nameLen = maxEntryNameLen;

   while (res == ERROR_SUCCESS)
   {
      nameLen = maxEntryNameLen;
      res = RegEnumValueW(m_key, idx++, buffer.data(), &nameLen, nullptr, nullptr,
                          nullptr, nullptr);
      if (res == ERROR_SUCCESS)
         entries.push_back(buffer.data());
   }

   if (res == ERROR_SUCCESS || res == ERROR_NO_MORE_ITEMS)
      entries;
   return {};
}

} // namespace win32

#endif //_WIN32
