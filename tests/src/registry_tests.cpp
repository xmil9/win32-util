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

using namespace std;
using namespace win32;


namespace
{
///////////////////

const wstring TestsKeyPath = L"Software\\Projects\\win32_util\\tests";


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
   const LSTATUS res = RegDeleteKeyExW(parent, keyPath.c_str(), KEY_WOW64_64KEY, 0);
   return (res == ERROR_SUCCESS);
}


///////////////////

void testRegKeyDefaultCtor()
{
   {
      const string caseLabel{"RegKey default ctor"};
      RegKey rk;
      VERIFY(!rk, caseLabel);
      VERIFY(rk.hkey() == NULL, caseLabel);
   }
}


void testRegKeyCtorWithKeyInfo()
{
   {
      const string caseLabel{"RegKey ctor with key info for existing key"};

      const wstring keyPath = TestsKeyPath + L"\\RegKeyCtorWithKeyInfo";
      createKey(HKEY_CURRENT_USER, keyPath);

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      VERIFY(rk.hkey(), caseLabel);
      VERIFY(!rk.wasCreated(), caseLabel);
      rk.close();

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const string caseLabel{"RegKey ctor with key info for not existing key"};

      const wstring keyPath = TestsKeyPath + L"\\RegKeyCtorWithKeyInfo";
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
      const string caseLabel{"RegKey move ctor"};
      const wstring keyPath = TestsKeyPath + L"\\RegKeyMoveCtor";
      
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
      const string caseLabel{"RegKey move assignment"};
      const wstring keyPath = TestsKeyPath + L"\\RegKeyMoveAssignment";
      
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
      const string caseLabel{"RegKey operator bool for opened key"};
      const wstring keyPath = TestsKeyPath + L"\\RegKeyOperatorBool";

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      VERIFY(rk.operator bool(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const string caseLabel{"RegKey operator bool for empty key"};
      const wstring keyPath = TestsKeyPath + L"\\RegKeyOperatorBool";

      RegKey rk;
      VERIFY(!rk.operator bool(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}


void testRegKeyOperatorHKEY()
{
   {
      const string caseLabel{"RegKey operator HKEY for opened key"};
      const wstring keyPath = TestsKeyPath + L"\\RegKeyOperatorHKEY";

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      VERIFY(rk.operator HKEY() == rk.hkey(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const string caseLabel{"RegKey operator HKEY for empty key"};
      const wstring keyPath = TestsKeyPath + L"\\RegKeyOperatorHKEY";

      RegKey rk;
      VERIFY(rk.operator HKEY() == NULL, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}


void testRegKeyHkey()
{
   {
      const string caseLabel{"RegKey::hkey for opened key"};
      const wstring keyPath = TestsKeyPath + L"\\RegKeyHkey";

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      VERIFY(rk.hkey() != NULL, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const string caseLabel{"RegKey::hkey for empty key"};
      const wstring keyPath = TestsKeyPath + L"\\RegKeyHkey";

      RegKey rk;
      VERIFY(rk.hkey() == NULL, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}


void testRegKeySwap()
{
   {
      const string caseLabel{"RegKey swap"};
     
      const wstring keyPathA = TestsKeyPath + L"\\RegKeySwapA";
      RegKey a{HKEY_CURRENT_USER, keyPathA};
      HKEY hkeyA = a.hkey();

      const wstring keyPathB = TestsKeyPath + L"\\RegKeySwapB";
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
      const string caseLabel{"RegKey::create for not existing key"};
      const wstring keyPath = TestsKeyPath + L"\\RegKeyCreate";
      deleteKey(HKEY_CURRENT_USER, keyPath);
      
      RegKey rk;
      const bool res = rk.create(HKEY_CURRENT_USER, keyPath);
      VERIFY(res, caseLabel);
      VERIFY(rk.hkey() != NULL, caseLabel);
      VERIFY(rk.wasCreated(), caseLabel);
      
      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const string caseLabel{"RegKey::create for existing key"};
      const wstring keyPath = TestsKeyPath + L"\\RegKeyCreate";
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
      const string caseLabel{"RegKey::open for existing key"};
      const wstring keyPath = TestsKeyPath + L"\\RegKeyOpen";
      createKey(HKEY_CURRENT_USER, keyPath);
      
      RegKey rk;
      const bool res = rk.open(HKEY_CURRENT_USER, keyPath);
      VERIFY(res, caseLabel);
      VERIFY(rk.hkey() != NULL, caseLabel);
      VERIFY(rk.wasOpened(), caseLabel);
      
      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const string caseLabel{"RegKey::open for not existing key"};
      const wstring keyPath = TestsKeyPath + L"\\RegKeyOpen";
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
      const string caseLabel{"RegKey::close for open key"};
      const wstring keyPath = TestsKeyPath + L"\\RegKeyClose";
      
      RegKey rk{HKEY_CURRENT_USER, keyPath};
      rk.close();
      VERIFY(rk.hkey() == NULL, caseLabel);
      
      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const string caseLabel{"RegKey::close for empty key"};
      RegKey rk;
      rk.close();
      VERIFY(rk.hkey() == NULL, caseLabel);
   }
}


void testRegKeyClear()
{
   {
      const string caseLabel{"RegKey::clear for open key"};
      const wstring keyPath = TestsKeyPath + L"\\RegKeyClear";
      
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
      const string caseLabel{"RegKey::wasCreated for created key"};
      const wstring keyPath = TestsKeyPath + L"\\RegKeyWasCreated";
      
      RegKey rk{HKEY_CURRENT_USER, keyPath};
      VERIFY(rk.wasCreated(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const string caseLabel{"RegKey::wasCreated for opened key"};
      const wstring keyPath = TestsKeyPath + L"\\RegKeyWasCreated";
      createKey(HKEY_CURRENT_USER, keyPath);

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      VERIFY(!rk.wasCreated(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}

void testRegKeyWasOpened()
{
   {
      const string caseLabel{"RegKey::wasOpened for created key"};
      const wstring keyPath = TestsKeyPath + L"\\RegKeyWasOpened";
      
      RegKey rk{HKEY_CURRENT_USER, keyPath};
      VERIFY(!rk.wasOpened(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const string caseLabel{"RegKey::wasOpened for opened key"};
      const wstring keyPath = TestsKeyPath + L"\\RegKeyWasOpened";
      createKey(HKEY_CURRENT_USER, keyPath);

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      VERIFY(rk.wasOpened(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}


void testRegKeyKeyExists()
{
   {
      const string caseLabel{"RegKey::keyExists for existing key"};
      const wstring keyPath = TestsKeyPath + L"\\RegKeyKeyExists";
      createKey(HKEY_CURRENT_USER, keyPath);
      
      VERIFY(RegKey::keyExists(HKEY_CURRENT_USER, keyPath), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const string caseLabel{"RegKey::keyExists for not existing key"};
      const wstring keyPath = TestsKeyPath + L"\\RegKeyKeyExists";
      deleteKey(HKEY_CURRENT_USER, keyPath);
      
      VERIFY(!RegKey::keyExists(HKEY_CURRENT_USER, keyPath), caseLabel);
   }
}


void testRegKeyRemoveKey()
{
   {
      const string caseLabel{"RegKey::removeKey for existing key"};
      const wstring keyPath = TestsKeyPath + L"\\RegKeyRemoveKey";
      createKey(HKEY_CURRENT_USER, keyPath);
      
      const bool res = RegKey::removeKey(HKEY_CURRENT_USER, keyPath);
      VERIFY(res, caseLabel);
      VERIFY(!RegKey::keyExists(HKEY_CURRENT_USER, keyPath), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const string caseLabel{"RegKey::removeKey for not existing key"};
      const wstring keyPath = TestsKeyPath + L"\\RegKeyRemoveKey";
      deleteKey(HKEY_CURRENT_USER, keyPath);
      
      const bool res = RegKey::removeKey(HKEY_CURRENT_USER, keyPath);
      VERIFY(!res, caseLabel);
      VERIFY(!RegKey::keyExists(HKEY_CURRENT_USER, keyPath), caseLabel);
   }
}


void testRegKeyReadInt32()
{
   {
      const string caseLabel{"RegKey::readInt32 for existing value"};
      const wstring keyPath = TestsKeyPath + L"\\RegKeyReadInt32";
      createKey(HKEY_CURRENT_USER, keyPath);

      const wstring entryName = L"Int32";
      const int32_t val = 42;
      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         setup.writeInt32(entryName, val);
      }

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const optional<int32_t> res = rk.readInt32(entryName);
      VERIFY(res.has_value(), caseLabel);
      VERIFY(res.value() == val, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const string caseLabel{"RegKey::readInt32 for not existing value"};
      const wstring keyPath = TestsKeyPath + L"\\RegKeyReadInt32";
      createKey(HKEY_CURRENT_USER, keyPath);

      const wstring entryName = L"Int32";
      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const optional<int32_t> res = rk.readInt32(entryName);
      VERIFY(!res.has_value(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const string caseLabel{"RegKey::readInt32 for value of other type"};
      const wstring keyPath = TestsKeyPath + L"\\RegKeyReadInt32";
      createKey(HKEY_CURRENT_USER, keyPath);

      const wstring entryName = L"Int32";
      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         setup.writeString(entryName, "wrong type");
      }

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const optional<int32_t> res = rk.readInt32(entryName);
      VERIFY(!res.has_value(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}


void testRegKeyReadInt64()
{
   {
      const string caseLabel{"RegKey::readInt64 for existing value"};
      const wstring keyPath = TestsKeyPath + L"\\RegKeyReadInt64";
      createKey(HKEY_CURRENT_USER, keyPath);

      const wstring entryName = L"Int64";
      const int64_t val = 42;
      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         setup.writeInt64(entryName, val);
      }

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const optional<int64_t> res = rk.readInt64(entryName);
      VERIFY(res.has_value(), caseLabel);
      VERIFY(res.value() == val, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const string caseLabel{"RegKey::readInt64 for not existing value"};
      const wstring keyPath = TestsKeyPath + L"\\RegKeyReadInt64";
      createKey(HKEY_CURRENT_USER, keyPath);

      const wstring entryName = L"Int64";
      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const optional<int64_t> res = rk.readInt64(entryName);
      VERIFY(!res.has_value(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const string caseLabel{"RegKey::readInt64 for value of other type"};
      const wstring keyPath = TestsKeyPath + L"\\RegKeyReadInt64";
      createKey(HKEY_CURRENT_USER, keyPath);

      const wstring entryName = L"Int64";
      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         setup.writeString(entryName, "wrong type");
      }

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const optional<int64_t> res = rk.readInt64(entryName);
      VERIFY(!res.has_value(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
}


void testRegKeyReadString()
{
   {
      const string caseLabel{"RegKey::readString for existing value"};
      const wstring keyPath = TestsKeyPath + L"\\RegKeyReadString";
      createKey(HKEY_CURRENT_USER, keyPath);

      const wstring entryName = L"String";
      const string val = "test";
      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         setup.writeString(entryName, val);
      }

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const optional<string> res = rk.readString(entryName);
      VERIFY(res.has_value(), caseLabel);
      VERIFY(res.value() == val, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const string caseLabel{"RegKey::readString for not existing value"};
      const wstring keyPath = TestsKeyPath + L"\\RegKeyReadString";
      createKey(HKEY_CURRENT_USER, keyPath);

      const wstring entryName = L"String";
      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const optional<string> res = rk.readString(entryName);
      VERIFY(!res.has_value(), caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const string caseLabel{"RegKey::readString for empty string"};
      const wstring keyPath = TestsKeyPath + L"\\RegKeyReadString";
      createKey(HKEY_CURRENT_USER, keyPath);

      const wstring entryName = L"String";
      const string val = "";
      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         setup.writeString(entryName, val);
      }

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const optional<string> res = rk.readString(entryName);
      VERIFY(res.has_value(), caseLabel);
      VERIFY(res.value() == val, caseLabel);

      deleteKey(HKEY_CURRENT_USER, keyPath);
   }
   {
      const string caseLabel{"RegKey::readString for value of other type"};
      const wstring keyPath = TestsKeyPath + L"\\RegKeyReadString";
      createKey(HKEY_CURRENT_USER, keyPath);

      const wstring entryName = L"String";
      {
         RegKey setup{HKEY_CURRENT_USER, keyPath};
         setup.writeInt32(entryName, 1);
      }

      RegKey rk{HKEY_CURRENT_USER, keyPath};
      const optional<string> res = rk.readString(entryName);
      VERIFY(!res.has_value(), caseLabel);

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
}
