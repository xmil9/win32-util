//
// Win32 utilities library
// Tests for registry utilities.
//
// Jun-2019, Michael Lindner
// MIT license
//
#include "registry_tests.h"
#include "registry.h"
#include "test_util.h"

using namespace win32;


namespace
{
///////////////////

const std::wstring TestsKeyPath = L"Software\\Projects\\win32_util\\tests";


bool createKey(HKEY parent, const std::wstring& keyPath)
{
   HKEY hkey = NULL;
   const LSTATUS res = RegCreateKeyExW(parent, keyPath.c_str(), 0, nullptr, 0,
                                       KEY_ALL_ACCESS, nullptr, &hkey, nullptr);
   if (res == ERROR_SUCCESS)
      RegCloseKey(hkey);
   return (res == ERROR_SUCCESS);
}


bool deleteKey(HKEY parent, const std::wstring& keyPath)
{
   const LSTATUS res = RegDeleteTreeW(parent, keyPath.c_str());
   return (res == ERROR_SUCCESS);
}


///////////////////

void testRegKeyDefaultCtor()
{
   {
      const std::string caseLabel{"RegKey default ctor"};
      RegKey rk;
      VERIFY(!rk, caseLabel);
      VERIFY(rk.hkey() == NULL, caseLabel);
   }
}


void testRegKeyCtorWithKeyInfo()
{
   {
      const std::string caseLabel{"RegKey ctor with key info for existing key"};

      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyCtorWithKeyInfo";
      createKey(HKEY_CURRENT_USER, keyPath);

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      VERIFY(rk.hkey(), caseLabel);
      VERIFY(!rk.wasCreated(), caseLabel);
      rk.close();

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey ctor with key info for not existing key"};

      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyCtorWithKeyInfo";
      // Make sure key does not exists.
      deleteKey(HKEY_CURRENT_USER, keyPath);

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      VERIFY(rk.hkey(), caseLabel);
      VERIFY(rk.wasCreated(), caseLabel);
      rk.close();

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}


void testRegKeyDtor()
{
   // todo - how to check that HKEY has been closed?
}


void testRegKeyMoveCtor()
{
   {
      const std::string caseLabel{"RegKey move ctor"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyMoveCtor";

      RegKey a{HKEY_CURRENT_USER, keyPath};
      HKEY hkey = a.hkey();
      RegKey b{std::move(a)};
      VERIFY(b.hkey() == hkey, caseLabel);
      VERIFY(a.hkey() == NULL, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}


void testRegKeyMoveAssignment()
{
   {
      const std::string caseLabel{"RegKey move assignment"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyMoveAssignment";

      RegKey a{HKEY_CURRENT_USER, keyPath};
      HKEY hkey = a.hkey();
      RegKey b;
      b = std::move(a);
      VERIFY(b.hkey() == hkey, caseLabel);
      VERIFY(a.hkey() == NULL, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}


void testRegKeyOperatorBool()
{
   {
      const std::string caseLabel{"RegKey operator bool for opened key"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyOperatorBool";

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      VERIFY(rk.operator bool(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey operator bool for empty key"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyOperatorBool";

      RegKey rk;
      VERIFY(!rk.operator bool(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}


void testRegKeyOperatorHKEY()
{
   {
      const std::string caseLabel{"RegKey operator HKEY for opened key"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyOperatorHKEY";

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      VERIFY(rk.operator HKEY() == rk.hkey(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey operator HKEY for empty key"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyOperatorHKEY";

      RegKey rk;
      VERIFY(rk.operator HKEY() == NULL, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}


void testRegKeyHkey()
{
   {
      const std::string caseLabel{"RegKey::hkey for opened key"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyHkey";

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      VERIFY(rk.hkey() != NULL, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::hkey for empty key"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyHkey";

      RegKey rk;
      VERIFY(rk.hkey() == NULL, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}


void testRegKeySwap()
{
   {
      const std::string caseLabel{"RegKey swap"};

      const std::wstring keyPathA = TestsKeyPath + L"\\RegKeySwapA";
      RegKey a{HKEY_CURRENT_USER, keyPathA};
      HKEY hkeyA = a.hkey();

      const std::wstring keyPathB = TestsKeyPath + L"\\RegKeySwapB";
      RegKey b{HKEY_CURRENT_USER, keyPathB};
      HKEY hkeyB = b.hkey();

      swap(a, b);
      VERIFY(a.hkey() == hkeyB, caseLabel);
      VERIFY(b.hkey() == hkeyA, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPathA);
      deleteKey(HKEY_CURRENT_USER, keyPathB);
   }
}


void testRegKeyCreate()
{
   {
      const std::string caseLabel{"RegKey::create for not existing key"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyCreate";
      deleteKey(HKEY_CURRENT_USER, keyPath);

      RegKey rk;
      const bool res = rk.create(HKEY_CURRENT_USER, keyPath);
      VERIFY(res, caseLabel);
      VERIFY(rk.hkey() != NULL, caseLabel);
      VERIFY(rk.wasCreated(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::create for existing key"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyCreate";
      createKey(HKEY_CURRENT_USER, keyPath);

      RegKey rk;
      const bool res = rk.create(HKEY_CURRENT_USER, keyPath);
      VERIFY(res, caseLabel);
      VERIFY(rk.hkey() != NULL, caseLabel);
      VERIFY(!rk.wasCreated(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}


void testRegKeyOpen()
{
   {
      const std::string caseLabel{"RegKey::open for existing key"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyOpen";
      createKey(HKEY_CURRENT_USER, keyPath);

      RegKey rk;
      const bool res = rk.open(HKEY_CURRENT_USER, keyPath);
      VERIFY(res, caseLabel);
      VERIFY(rk.hkey() != NULL, caseLabel);
      VERIFY(rk.wasOpened(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::open for not existing key"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyOpen";
      deleteKey(HKEY_CURRENT_USER, keyPath);

      RegKey rk;
      const bool res = rk.open(HKEY_CURRENT_USER, keyPath);
      VERIFY(!res, caseLabel);
      VERIFY(rk.hkey() == NULL, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}


void testRegKeyClose()
{
   {
      const std::string caseLabel{"RegKey::close for open key"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyClose";

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      rk.close();
      VERIFY(rk.hkey() == NULL, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::close for empty key"};
      RegKey rk;
      rk.close();
      VERIFY(rk.hkey() == NULL, caseLabel);
   }
}


void testRegKeyClear()
{
   {
      const std::string caseLabel{"RegKey::clear for open key"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyClear";

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      HKEY hkey = rk.hkey();
      rk.clear();
      VERIFY(rk.hkey() == NULL, caseLabel);

      if (hkey)
         RegCloseKey(hkey);
      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}

void testRegKeyWasCreated()
{
   {
      const std::string caseLabel{"RegKey::wasCreated for created key"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyWasCreated";

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      VERIFY(rk.wasCreated(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::wasCreated for opened key"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyWasCreated";
      createKey(HKEY_CURRENT_USER, keyPath);

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      VERIFY(!rk.wasCreated(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}

void testRegKeyWasOpened()
{
   {
      const std::string caseLabel{"RegKey::wasOpened for created key"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyWasOpened";

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      VERIFY(!rk.wasOpened(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::wasOpened for opened key"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyWasOpened";
      createKey(HKEY_CURRENT_USER, keyPath);

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      VERIFY(rk.wasOpened(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}


void testRegKeyKeyExists()
{
   {
      const std::string caseLabel{"RegKey::keyExists for existing key"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyKeyExists";
      createKey(HKEY_CURRENT_USER, keyPath);

      VERIFY(RegKey::keyExists(HKEY_CURRENT_USER, keyPath), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::keyExists for not existing key"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyKeyExists";
      deleteKey(HKEY_CURRENT_USER, keyPath);

      VERIFY(!RegKey::keyExists(HKEY_CURRENT_USER, keyPath), caseLabel);
   }
}


void testRegKeyRemoveKey()
{
   {
      const std::string caseLabel{"RegKey::removeKey for existing key"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyRemoveKey";
      createKey(HKEY_CURRENT_USER, keyPath);

      const bool res = RegKey::removeKey(HKEY_CURRENT_USER, keyPath);
      VERIFY(res, caseLabel);
      VERIFY(!RegKey::keyExists(HKEY_CURRENT_USER, keyPath), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::removeKey for not existing key"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyRemoveKey";
      deleteKey(HKEY_CURRENT_USER, keyPath);

      const bool res = RegKey::removeKey(HKEY_CURRENT_USER, keyPath);
      VERIFY(!res, caseLabel);
      VERIFY(!RegKey::keyExists(HKEY_CURRENT_USER, keyPath), caseLabel);
   }
   {
      const std::string caseLabel{"RegKey::removeKey for key with subkeys"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyRemoveKey";

      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         const std::vector<std::wstring> subkeys{L"sub1", L"sub2", L"sub3"};
         for (const std::wstring& keyName : subkeys)
         {
            RegKey subkey;
            subkey.create(setup, keyName);
         }
      }

      const bool res = RegKey::removeKey(HKEY_CURRENT_USER, keyPath);
      VERIFY(res, caseLabel);
      VERIFY(!RegKey::keyExists(HKEY_CURRENT_USER, keyPath), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}


void testRegKeyReadInt32()
{
   {
      const std::string caseLabel{"RegKey::readInt32 for existing value"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyReadInt32";
      createKey(HKEY_CURRENT_USER, keyPath);

      const std::wstring entryName = L"Int32";
      const int32_t val = 42;
      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         setup.writeInt32(entryName, val);
      }

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const std::optional<int32_t> res = rk.readInt32(entryName);
      VERIFY(res.has_value(), caseLabel);
      VERIFY(res.value() == val, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::readInt32 for not existing value"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyReadInt32";
      createKey(HKEY_CURRENT_USER, keyPath);

      const std::wstring entryName = L"Int32";
      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const std::optional<int32_t> res = rk.readInt32(entryName);
      VERIFY(!res.has_value(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::readInt32 for value of other type"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyReadInt32";
      createKey(HKEY_CURRENT_USER, keyPath);

      const std::wstring entryName = L"Int32";
      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         setup.writeString(entryName, "wrong type");
      }

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const std::optional<int32_t> res = rk.readInt32(entryName);
      VERIFY(!res.has_value(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::readInt32 for value of other integer type"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyReadInt32";
      createKey(HKEY_CURRENT_USER, keyPath);

      const std::wstring entryName = L"Int32";
      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         setup.writeInt64(entryName, 1);
      }

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const std::optional<int32_t> res = rk.readInt32(entryName);
      VERIFY(!res.has_value(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}


void testRegKeyReadInt64()
{
   {
      const std::string caseLabel{"RegKey::readInt64 for existing value"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyReadInt64";
      createKey(HKEY_CURRENT_USER, keyPath);

      const std::wstring entryName = L"Int64";
      const int64_t val = 42;
      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         setup.writeInt64(entryName, val);
      }

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const std::optional<int64_t> res = rk.readInt64(entryName);
      VERIFY(res.has_value(), caseLabel);
      VERIFY(res.value() == val, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::readInt64 for not existing value"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyReadInt64";
      createKey(HKEY_CURRENT_USER, keyPath);

      const std::wstring entryName = L"Int64";
      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const std::optional<int64_t> res = rk.readInt64(entryName);
      VERIFY(!res.has_value(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::readInt64 for value of other type"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyReadInt64";
      createKey(HKEY_CURRENT_USER, keyPath);

      const std::wstring entryName = L"Int64";
      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         setup.writeString(entryName, "wrong type");
      }

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const std::optional<int64_t> res = rk.readInt64(entryName);
      VERIFY(!res.has_value(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::readInt64 for value of other integer type"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyReadInt64";
      createKey(HKEY_CURRENT_USER, keyPath);

      const std::wstring entryName = L"Int64";
      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         setup.writeInt32(entryName, 1);
      }

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const std::optional<int64_t> res = rk.readInt64(entryName);
      VERIFY(!res.has_value(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}


void testRegKeyReadString()
{
   {
      const std::string caseLabel{"RegKey::readString for existing value"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyReadString";
      createKey(HKEY_CURRENT_USER, keyPath);

      const std::wstring entryName = L"String";
      const std::string val = "test";
      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         setup.writeString(entryName, val);
      }

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const std::optional<std::string> res = rk.readString(entryName);
      VERIFY(res.has_value(), caseLabel);
      VERIFY(res.value() == val, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::readString for not existing value"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyReadString";
      createKey(HKEY_CURRENT_USER, keyPath);

      const std::wstring entryName = L"String";
      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const std::optional<std::string> res = rk.readString(entryName);
      VERIFY(!res.has_value(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::readString for empty string"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyReadString";
      createKey(HKEY_CURRENT_USER, keyPath);

      const std::wstring entryName = L"String";
      const std::string val = "";
      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         setup.writeString(entryName, val);
      }

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const std::optional<std::string> res = rk.readString(entryName);
      VERIFY(res.has_value(), caseLabel);
      VERIFY(res.value() == val, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::readString for value of other type"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyReadString";
      createKey(HKEY_CURRENT_USER, keyPath);

      const std::wstring entryName = L"String";
      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         setup.writeInt32(entryName, 1);
      }

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const std::optional<std::string> res = rk.readString(entryName);
      VERIFY(!res.has_value(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}


void testRegKeyReadWString()
{
   {
      const std::string caseLabel{"RegKey::readWString for existing value"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyReadWString";
      createKey(HKEY_CURRENT_USER, keyPath);

      const std::wstring entryName = L"String";
      const std::wstring val = L"test";
      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         setup.writeWString(entryName, val);
      }

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const std::optional<std::wstring> res = rk.readWString(entryName);
      VERIFY(res.has_value(), caseLabel);
      VERIFY(res.value() == val, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::readWString for not existing value"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyReadWString";
      createKey(HKEY_CURRENT_USER, keyPath);

      const std::wstring entryName = L"String";
      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const std::optional<std::wstring> res = rk.readWString(entryName);
      VERIFY(!res.has_value(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::readWString for empty string"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyReadWString";
      createKey(HKEY_CURRENT_USER, keyPath);

      const std::wstring entryName = L"String";
      const std::wstring val = L"";
      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         setup.writeWString(entryName, val);
      }

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const std::optional<std::wstring> res = rk.readWString(entryName);
      VERIFY(res.has_value(), caseLabel);
      VERIFY(res.value() == val, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::readWString for value of other type"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyReadWString";
      createKey(HKEY_CURRENT_USER, keyPath);

      const std::wstring entryName = L"String";
      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         setup.writeInt32(entryName, 1);
      }

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const std::optional<std::wstring> res = rk.readWString(entryName);
      VERIFY(!res.has_value(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}


void testRegKeyReadBinary()
{
   {
      const std::string caseLabel{"RegKey::readBinary for existing value"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyReadBinary";
      createKey(HKEY_CURRENT_USER, keyPath);

      const std::wstring entryName = L"Bin";
      const std::vector<BYTE> val{1, 2, 3, 4, 5, 6};
      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         setup.writeBinary(entryName, val.data(), val.size());
      }

      std::vector<BYTE> outBuffer;
      outBuffer.resize(val.size());
      auto getOutBuffer = [&outBuffer](size_t numBytes) -> BYTE* {
         if (numBytes <= outBuffer.size())
            return outBuffer.data();
         return nullptr;
      };

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const size_t bytesRead = rk.readBinary(entryName, getOutBuffer);
      VERIFY(bytesRead == val.size(), caseLabel);
      VERIFY(outBuffer == val, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::readBinary for not existing value"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyReadBinary";
      createKey(HKEY_CURRENT_USER, keyPath);

      std::vector<BYTE> outBuffer;
      auto getOutBuffer = [&outBuffer](size_t numBytes) -> BYTE* {
         if (numBytes <= outBuffer.size())
            return outBuffer.data();
         return nullptr;
      };

      const std::wstring entryName = L"String";
      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const size_t bytesRead = rk.readBinary(entryName, getOutBuffer);
      VERIFY(bytesRead == 0, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::readBinary for failing to provide buffer"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyReadBinary";
      createKey(HKEY_CURRENT_USER, keyPath);

      const std::wstring entryName = L"Bin";
      const std::vector<BYTE> val{1, 2, 3, 4, 5, 6};
      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         setup.writeBinary(entryName, val.data(), val.size());
      }

      std::vector<BYTE> outBuffer;
      auto getOutBuffer = [&outBuffer](size_t numBytes) -> BYTE* { return nullptr; };

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const size_t bytesRead = rk.readBinary(entryName, getOutBuffer);
      VERIFY(bytesRead == 0, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::readBinary for empty buffer"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyReadBinary";
      createKey(HKEY_CURRENT_USER, keyPath);

      const std::wstring entryName = L"Bin";
      const std::vector<BYTE> val;
      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         setup.writeBinary(entryName, val.data(), val.size());
      }

      std::vector<BYTE> outBuffer{0, 0, 0};
      auto getOutBuffer = [&outBuffer](size_t numBytes) -> BYTE* {
         if (numBytes <= outBuffer.size())
            return outBuffer.data();
         return nullptr;
      };

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const size_t bytesRead = rk.readBinary(entryName, getOutBuffer);
      VERIFY(bytesRead == val.size(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::readBinary for value of other type"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyReadBinary";
      createKey(HKEY_CURRENT_USER, keyPath);

      const std::wstring entryName = L"Bin";
      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         setup.writeInt32(entryName, 1);
      }

      std::vector<BYTE> outBuffer;
      auto getOutBuffer = [&outBuffer](size_t numBytes) -> BYTE* {
         if (numBytes <= outBuffer.size())
            return outBuffer.data();
         return nullptr;
      };

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const size_t bytesRead = rk.readBinary(entryName, getOutBuffer);
      VERIFY(bytesRead == 0, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}


void testRegKeyWriteInt32()
{
   {
      const std::string caseLabel{"RegKey::writeInt32"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyWriteInt32";
      createKey(HKEY_CURRENT_USER, keyPath);

      const std::wstring entryName = L"Int32";
      const int32_t val = 42;

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      rk.writeInt32(entryName, val);

      const std::optional<int32_t> res = rk.readInt32(entryName);
      VERIFY(res.has_value(), caseLabel);
      VERIFY(res.value() == val, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}


void testRegKeyWriteInt64()
{
   {
      const std::string caseLabel{"RegKey::writeInt64"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyWriteInt64";
      createKey(HKEY_CURRENT_USER, keyPath);

      const std::wstring entryName = L"Int64";
      const int64_t val = 42;

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      rk.writeInt64(entryName, val);

      const std::optional<int64_t> res = rk.readInt64(entryName);
      VERIFY(res.has_value(), caseLabel);
      VERIFY(res.value() == val, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}


void testRegKeyWriteString()
{
   {
      const std::string caseLabel{"RegKey::writeString"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyWriteString";
      createKey(HKEY_CURRENT_USER, keyPath);

      const std::wstring entryName = L"String";
      const std::string val = "test";

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      rk.writeString(entryName, val);

      const std::optional<std::string> res = rk.readString(entryName);
      VERIFY(res.has_value(), caseLabel);
      VERIFY(res.value() == val, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}


void testRegKeyWriteWString()
{
   {
      const std::string caseLabel{"RegKey::writeWString"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyWriteWString";
      createKey(HKEY_CURRENT_USER, keyPath);

      const std::wstring entryName = L"WString";
      const std::wstring val = L"test";

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      rk.writeWString(entryName, val);

      const std::optional<std::wstring> res = rk.readWString(entryName);
      VERIFY(res.has_value(), caseLabel);
      VERIFY(res.value() == val, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}


void testRegKeyWriteBinary()
{
   {
      const std::string caseLabel{"RegKey::writeBinary"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyWriteBinary";
      createKey(HKEY_CURRENT_USER, keyPath);

      const std::wstring entryName = L"Bin";
      const std::vector<BYTE> val{1, 2, 3, 4};

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      rk.writeBinary(entryName, val.data(), val.size());

      std::vector<BYTE> outBuffer;
      outBuffer.resize(val.size());
      auto getOutBuffer = [&outBuffer](size_t numBytes) -> BYTE* {
         if (numBytes <= outBuffer.size())
            return outBuffer.data();
         return nullptr;
      };
      const size_t numBytes = rk.readBinary(entryName, getOutBuffer);

      VERIFY(numBytes == val.size(), caseLabel);
      VERIFY(outBuffer == val, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}


void testRegKeyRemoveEntry()
{
   {
      const std::string caseLabel{"RegKey::removeEntry for exiting entry"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyRemoveEntry";
      createKey(HKEY_CURRENT_USER, keyPath);

      const std::wstring entryName = L"Test";
      const int32_t val = 42;
      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         setup.writeInt32(entryName, val);
      }

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const bool isRemoved = rk.removeEntry(entryName);
      VERIFY(isRemoved, caseLabel);

      const std::optional<int32_t> res = rk.readInt32(entryName);
      VERIFY(!res.has_value(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::removeEntry for not exiting entry"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyRemoveEntry";
      createKey(HKEY_CURRENT_USER, keyPath);

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const bool isRemoved = rk.removeEntry(L"NotExisting");
      VERIFY(!isRemoved, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}


void testRegKeyCountSubkeys()
{
   {
      const std::string caseLabel{"RegKey::countSubkeys for multiple subkeys"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyCountSubkeys";
      createKey(HKEY_CURRENT_USER, keyPath);

      const std::vector<std::wstring> subkeys{L"sub1", L"sub2", L"sub3"};
      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         for (const std::wstring& keyName : subkeys)
         {
            RegKey subkey;
            subkey.create(setup, keyName);
         }
      }

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const size_t numSubkeys = rk.countSubkeys();
      VERIFY(numSubkeys == subkeys.size(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::countSubkeys for single subkey"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyCountSubkeys";
      createKey(HKEY_CURRENT_USER, keyPath);

      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         RegKey subkey{setup, L"sub1"};
      }

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const size_t numSubkeys = rk.countSubkeys();
      VERIFY(numSubkeys == 1, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::countSubkeys for no subkeys"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyCountSubkeys";
      createKey(HKEY_CURRENT_USER, keyPath);

      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
      }

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const size_t numSubkeys = rk.countSubkeys();
      VERIFY(numSubkeys == 0, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}


void testRegKeySubkeyNames()
{
   {
      const std::string caseLabel{"RegKey::subkeyNames for multiple subkeys"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeySubkeyNames";
      createKey(HKEY_CURRENT_USER, keyPath);

      const std::vector<std::wstring> subkeys{L"sub1", L"sub2", L"sub3"};
      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         for (const std::wstring& keyName : subkeys)
         {
            RegKey subkey;
            subkey.create(setup, keyName);
         }
      }

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const std::vector<std::wstring> res = rk.subkeyNames();
      VERIFY(res == subkeys, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::subkeyNames for single subkey"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeySubkeyNames";
      createKey(HKEY_CURRENT_USER, keyPath);

      const std::vector<std::wstring> subkeys{L"sub1"};
      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         for (const std::wstring& keyName : subkeys)
         {
            RegKey subkey;
            subkey.create(setup, keyName);
         }
      }

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const std::vector<std::wstring> res = rk.subkeyNames();
      VERIFY(res == subkeys, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::subkeyNames for no subkeys"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeySubkeyNames";
      createKey(HKEY_CURRENT_USER, keyPath);

      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
      }

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const std::vector<std::wstring> res = rk.subkeyNames();
      VERIFY(res.empty(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}


void testRegKeyCountEntries()
{
   {
      const std::string caseLabel{"RegKey::countEntries for multiple entries"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyCountEntries";
      createKey(HKEY_CURRENT_USER, keyPath);

      const std::vector<std::wstring> entries{L"entry1", L"entry2", L"entry3"};
      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         for (const std::wstring& entryName : entries)
            setup.writeInt32(entryName, 1);
      }

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const size_t numEntries = rk.countEntries();
      VERIFY(numEntries == entries.size(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::countEntries for single entry"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyCountEntries";
      createKey(HKEY_CURRENT_USER, keyPath);

      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         setup.writeInt32(L"entry", 1);
      }

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const size_t numEntries = rk.countEntries();
      VERIFY(numEntries == 1, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::countEntries for no entries"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyCountEntries";
      createKey(HKEY_CURRENT_USER, keyPath);

      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
      }

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const size_t numEntries = rk.countEntries();
      VERIFY(numEntries == 0, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}


void testRegKeyEntryNames()
{
   {
      const std::string caseLabel{"RegKey::entryNames for multiple entries"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyEntryNames";
      createKey(HKEY_CURRENT_USER, keyPath);

      const std::vector<std::wstring> entries{L"entry1", L"entry2", L"entry3"};
      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         for (const std::wstring& entryName : entries)
            setup.writeInt32(entryName, 1);
      }

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const std::vector<std::wstring> res = rk.entryNames();
      VERIFY(res == entries, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::entryNames for single entry"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyEntryNames";
      createKey(HKEY_CURRENT_USER, keyPath);

      const std::vector<std::wstring> entries{L"entry1"};
      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         for (const std::wstring& entryName : entries)
            setup.writeInt32(entryName, 1);
      }

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const std::vector<std::wstring> res = rk.entryNames();
      VERIFY(res == entries, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const std::string caseLabel{"RegKey::entryNames for no entries"};
      const std::wstring keyPath = TestsKeyPath + L"\\RegKeyEntryNames";
      createKey(HKEY_CURRENT_USER, keyPath);

      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
      }

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const std::vector<std::wstring> res = rk.entryNames();
      VERIFY(res.empty(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}

} // namespace


void testRegistry()
{
   testRegKeyDefaultCtor();
   testRegKeyCtorWithKeyInfo();
   testRegKeyDtor();
   testRegKeyMoveCtor();
   testRegKeyMoveAssignment();
   testRegKeyOperatorBool();
   testRegKeyOperatorHKEY();
   testRegKeyHkey();
   testRegKeySwap();
   testRegKeyCreate();
   testRegKeyOpen();
   testRegKeyClose();
   testRegKeyClear();
   testRegKeyWasCreated();
   testRegKeyWasOpened();
   testRegKeyKeyExists();
   testRegKeyRemoveKey();
   testRegKeyReadInt32();
   testRegKeyReadInt64();
   testRegKeyReadString();
   testRegKeyReadWString();
   testRegKeyReadBinary();
   testRegKeyWriteInt32();
   testRegKeyWriteInt64();
   testRegKeyWriteString();
   testRegKeyWriteWString();
   testRegKeyWriteBinary();
   testRegKeyRemoveEntry();
   testRegKeyCountSubkeys();
   testRegKeySubkeyNames();
   testRegKeyCountEntries();
   testRegKeyEntryNames();
}
