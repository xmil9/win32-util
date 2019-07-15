//
// Win32 utilities library
// Tests for registry utilities.
//
// Jun-2019, Michael Lindner
// MIT license
//
#include "mem_util_tests.h"
#include "mem_util.h"
#include "test_util.h"

using namespace win32;


namespace
{
///////////////////

void testCoTaskMemDefaultCtor()
{
   {
      const std::string caseLabel{"CoTaskMem default ctor"};
      CoTaskMem<int> mem;
      VERIFY(mem.ptr() == nullptr, caseLabel);
   }
}


void testCoTaskMemCtorWithPointer()
{
   {
      const std::string caseLabel{"CoTaskMem pointer ctor"};
      int* p = static_cast<int*>(CoTaskMemAlloc(5 * sizeof(int)));
      CoTaskMem<int> mem(p);
      VERIFY(mem.ptr() == p, caseLabel);
   }
}


void testCoTaskMemDtor()
{
   // Todo - how to check that mem was freed?
}


void testCoTaskMemMoveCtor()
{
   {
      const std::string caseLabel{"CoTaskMem move ctor"};
      int* p = static_cast<int*>(CoTaskMemAlloc(5 * sizeof(int)));
      CoTaskMem<int> memA{p};
      CoTaskMem<int> memB{std::move(memA)};
      VERIFY(memA.ptr() == nullptr, caseLabel);
      VERIFY(memB.ptr() == p, caseLabel);
   }
}


void testCoTaskMemMoveAssignment()
{
   {
      const std::string caseLabel{"CoTaskMem move assignment"};
      int* p = static_cast<int*>(CoTaskMemAlloc(5 * sizeof(int)));
      CoTaskMem<int> memA{p};
      CoTaskMem<int> memB;
      memB = std::move(memA);
      VERIFY(memA.ptr() == nullptr, caseLabel);
      VERIFY(memB.ptr() == p, caseLabel);
   }
}


void testCoTaskMemOperatorBool()
{
   {
      const std::string caseLabel{"CoTaskMem operator bool for allocated memory"};
      int* p = static_cast<int*>(CoTaskMemAlloc(5 * sizeof(int)));
      CoTaskMem<int> mem{p};
      VERIFY(mem.operator bool(), caseLabel);
   }
   {
      const std::string caseLabel{"CoTaskMem operator bool for empty object"};
      CoTaskMem<int> mem;
      VERIFY(!mem.operator bool(), caseLabel);
   }
}


void testCoTaskMemSwap()
{
   {
      const std::string caseLabel{"CoTaskMem swap"};
      int* pA = static_cast<int*>(CoTaskMemAlloc(5 * sizeof(int)));
      CoTaskMem<int> memA{pA};
      int* pB = static_cast<int*>(CoTaskMemAlloc(2 * sizeof(int)));
      CoTaskMem<int> memB{pB};
      swap(memA, memB);
      VERIFY(memA.ptr() == pB, caseLabel);
      VERIFY(memB.ptr() == pA, caseLabel);
   }
}


void testCoTaskMemPtr()
{
   {
      const std::string caseLabel{"CoTaskMem::ptr"};
      int* p = static_cast<int*>(CoTaskMemAlloc(5 * sizeof(int)));
      CoTaskMem<int> mem{p};
      VERIFY(mem.ptr() == p, caseLabel);
   }
}


void testCoTaskMemConstPtr()
{
   {
      const std::string caseLabel{"CoTaskMem::ptr const"};
      int* p = static_cast<int*>(CoTaskMemAlloc(5 * sizeof(int)));
      const CoTaskMem<int> mem{p};
      VERIFY(mem.ptr() == p, caseLabel);
   }
}


void testCoTaskMemAddr()
{
   {
      const std::string caseLabel{"CoTaskMem::addr for allocated memory"};
      int* p = static_cast<int*>(CoTaskMemAlloc(sizeof(int)));
      *p = 42;
      CoTaskMem<int> mem{p};
      int** address = mem.addr();
      VERIFY(**address == *p, caseLabel);
   }
   {
      const std::string caseLabel{"CoTaskMem::addr for empty object"};
      CoTaskMem<int> mem;
      int** address = mem.addr();
      VERIFY(*address == nullptr, caseLabel);
   }
}


void testCoTaskMemClear()
{
   {
      const std::string caseLabel{"CoTaskMem::clear"};
      int* p = static_cast<int*>(CoTaskMemAlloc(sizeof(int)));
      CoTaskMem<int> mem{p};
      mem.clear();
      VERIFY(mem.ptr() == nullptr, caseLabel);
   }
}


void testCoTaskMemFree()
{
   {
      const std::string caseLabel{"CoTaskMem::free"};
      int* p = static_cast<int*>(CoTaskMemAlloc(sizeof(int)));
      CoTaskMem<int> mem{p};
      mem.free();
      VERIFY(mem.ptr() == nullptr, caseLabel);
   }
}


void testGlobalMemDefaultCtor()
{
   {
      const std::string caseLabel{"GlobalMem default ctor"};
      GlobalMem<float> mem;
      VERIFY(mem.ptr() == nullptr, caseLabel);
   }
}


void testGlobalMemCtorWithPointer()
{
   {
      const std::string caseLabel{"GlobalMem pointer ctor"};
      float* p = static_cast<float*>(GlobalAlloc(0, 5 * sizeof(float)));
      GlobalMem<float> mem(p);
      VERIFY(mem.ptr() == p, caseLabel);
   }
}


void testGlobalMemDtor()
{
   // Todo - how to check that mem was freed?
}


void testGlobalMemMoveCtor()
{
   {
      const std::string caseLabel{"GlobalMem move ctor"};
      float* p = static_cast<float*>(GlobalAlloc(0, 5 * sizeof(float)));
      GlobalMem<float> memA{p};
      GlobalMem<float> memB{std::move(memA)};
      VERIFY(memA.ptr() == nullptr, caseLabel);
      VERIFY(memB.ptr() == p, caseLabel);
   }
}


void testGlobalMemMoveAssignment()
{
   {
      const std::string caseLabel{"GlobalMem move assignment"};
      float* p = static_cast<float*>(GlobalAlloc(0, 5 * sizeof(float)));
      GlobalMem<float> memA{p};
      GlobalMem<float> memB;
      memB = std::move(memA);
      VERIFY(memA.ptr() == nullptr, caseLabel);
      VERIFY(memB.ptr() == p, caseLabel);
   }
}


void testGlobalMemOperatorBool()
{
   {
      const std::string caseLabel{"GlobalMem operator bool for allocated memory"};
      float* p = static_cast<float*>(GlobalAlloc(0, 5 * sizeof(float)));
      GlobalMem<float> mem{p};
      VERIFY(mem.operator bool(), caseLabel);
   }
   {
      const std::string caseLabel{"GlobalMem operator bool for empty object"};
      GlobalMem<float> mem;
      VERIFY(!mem.operator bool(), caseLabel);
   }
}


void testGlobalMemSwap()
{
   {
      const std::string caseLabel{"GlobalMem swap"};
      float* pA = static_cast<float*>(GlobalAlloc(0, 5 * sizeof(float)));
      GlobalMem<float> memA{pA};
      float* pB = static_cast<float*>(GlobalAlloc(0, 2 * sizeof(float)));
      GlobalMem<float> memB{pB};
      swap(memA, memB);
      VERIFY(memA.ptr() == pB, caseLabel);
      VERIFY(memB.ptr() == pA, caseLabel);
   }
}


void testGlobalMemPtr()
{
   {
      const std::string caseLabel{"GlobalMem::ptr"};
      float* p = static_cast<float*>(GlobalAlloc(0, 5 * sizeof(float)));
      GlobalMem<float> mem{p};
      VERIFY(mem.ptr() == p, caseLabel);
   }
}


void testGlobalMemConstPtr()
{
   {
      const std::string caseLabel{"GlobalMem::ptr const"};
      float* p = static_cast<float*>(GlobalAlloc(0, 5 * sizeof(float)));
      const GlobalMem<float> mem{p};
      VERIFY(mem.ptr() == p, caseLabel);
   }
}


void testGlobalMemAddr()
{
   {
      const std::string caseLabel{"GlobalMem::addr for allocated memory"};
      float* p = static_cast<float*>(GlobalAlloc(0, sizeof(float)));
      *p = 42.0f;
      GlobalMem<float> mem{p};
      float** address = mem.addr();
      VERIFY(**address == *p, caseLabel);
   }
   {
      const std::string caseLabel{"GlobalMem::addr for empty object"};
      GlobalMem<float> mem;
      float** address = mem.addr();
      VERIFY(*address == nullptr, caseLabel);
   }
}


void testGlobalMemClear()
{
   {
      const std::string caseLabel{"GlobalMem::clear"};
      float* p = static_cast<float*>(GlobalAlloc(0, sizeof(float)));
      GlobalMem<float> mem{p};
      mem.clear();
      VERIFY(mem.ptr() == nullptr, caseLabel);
   }
}


void testGlobalMemFree()
{
   {
      const std::string caseLabel{"GlobalMem::free"};
      float* p = static_cast<float*>(GlobalAlloc(0, sizeof(float)));
      GlobalMem<float> mem{p};
      mem.free();
      VERIFY(mem.ptr() == nullptr, caseLabel);
   }
}


void testLocalMemDefaultCtor()
{
   {
      const std::string caseLabel{"LocalMem default ctor"};
      LocalMem<char> mem;
      VERIFY(mem.ptr() == nullptr, caseLabel);
   }
}


void testLocalMemCtorWithPointer()
{
   {
      const std::string caseLabel{"LocalMem pointer ctor"};
      char* p = static_cast<char*>(LocalAlloc(0, 5 * sizeof(char)));
      LocalMem<char> mem(p);
      VERIFY(mem.ptr() == p, caseLabel);
   }
}


void testLocalMemDtor()
{
   // Todo - how to check that mem was freed?
}


void testLocalMemMoveCtor()
{
   {
      const std::string caseLabel{"LocalMem move ctor"};
      char* p = static_cast<char*>(LocalAlloc(0, 5 * sizeof(char)));
      LocalMem<char> memA{p};
      LocalMem<char> memB{std::move(memA)};
      VERIFY(memA.ptr() == nullptr, caseLabel);
      VERIFY(memB.ptr() == p, caseLabel);
   }
}


void testLocalMemMoveAssignment()
{
   {
      const std::string caseLabel{"LocalMem move assignment"};
      char* p = static_cast<char*>(LocalAlloc(0, 5 * sizeof(char)));
      LocalMem<char> memA{p};
      LocalMem<char> memB;
      memB = std::move(memA);
      VERIFY(memA.ptr() == nullptr, caseLabel);
      VERIFY(memB.ptr() == p, caseLabel);
   }
}


void testLocalMemOperatorBool()
{
   {
      const std::string caseLabel{"LocalMem operator bool for allocated memory"};
      char* p = static_cast<char*>(LocalAlloc(0, 5 * sizeof(char)));
      LocalMem<char> mem{p};
      VERIFY(mem.operator bool(), caseLabel);
   }
   {
      const std::string caseLabel{"LocalMem operator bool for empty object"};
      LocalMem<char> mem;
      VERIFY(!mem.operator bool(), caseLabel);
   }
}


void testLocalMemSwap()
{
   {
      const std::string caseLabel{"LocalMem swap"};
      char* pA = static_cast<char*>(LocalAlloc(0, 5 * sizeof(char)));
      LocalMem<char> memA{pA};
      char* pB = static_cast<char*>(LocalAlloc(0, 2 * sizeof(char)));
      LocalMem<char> memB{pB};
      swap(memA, memB);
      VERIFY(memA.ptr() == pB, caseLabel);
      VERIFY(memB.ptr() == pA, caseLabel);
   }
}


void testLocalMemPtr()
{
   {
      const std::string caseLabel{"LocalMem::ptr"};
      char* p = static_cast<char*>(LocalAlloc(0, 5 * sizeof(char)));
      LocalMem<char> mem{p};
      VERIFY(mem.ptr() == p, caseLabel);
   }
}


void testLocalMemConstPtr()
{
   {
      const std::string caseLabel{"LocalMem::ptr const"};
      char* p = static_cast<char*>(LocalAlloc(0, 5 * sizeof(char)));
      const LocalMem<char> mem{p};
      VERIFY(mem.ptr() == p, caseLabel);
   }
}


void testLocalMemAddr()
{
   {
      const std::string caseLabel{"LocalMem::addr for allocated memory"};
      char* p = static_cast<char*>(LocalAlloc(0, sizeof(char)));
      *p = 'a';
      LocalMem<char> mem{p};
      char** address = mem.addr();
      VERIFY(**address == *p, caseLabel);
   }
   {
      const std::string caseLabel{"LocalMem::addr for empty object"};
      LocalMem<char> mem;
      char** address = mem.addr();
      VERIFY(*address == nullptr, caseLabel);
   }
}


void testLocalMemClear()
{
   {
      const std::string caseLabel{"LocalMem::clear"};
      char* p = static_cast<char*>(LocalAlloc(0, sizeof(char)));
      LocalMem<char> mem{p};
      mem.clear();
      VERIFY(mem.ptr() == nullptr, caseLabel);
   }
}


void testLocalMemFree()
{
   {
      const std::string caseLabel{"LocalMem::free"};
      char* p = static_cast<char*>(LocalAlloc(0, sizeof(char)));
      LocalMem<char> mem{p};
      mem.free();
      VERIFY(mem.ptr() == nullptr, caseLabel);
   }
}


void testHeapMemDefaultCtor()
{
   {
      const std::string caseLabel{"HeapMem default ctor"};
      HeapMem<wchar_t> mem;
      VERIFY(mem.ptr() == nullptr, caseLabel);
   }
}


void testHeapMemCtorWithPointer()
{
   {
      const std::string caseLabel{"HeapMem pointer ctor"};
      wchar_t* p =
         static_cast<wchar_t*>(HeapAlloc(GetProcessHeap(), 0, 5 * sizeof(wchar_t)));
      HeapMem<wchar_t> mem(p);
      VERIFY(mem.ptr() == p, caseLabel);
   }
}


void testHeapMemCtorWithAll()
{
   {
      const std::string caseLabel{"HeapMem ctor with all parameters"};
      wchar_t* p =
         static_cast<wchar_t*>(HeapAlloc(GetProcessHeap(), 0, 5 * sizeof(wchar_t)));
      HeapMem<wchar_t> mem(GetProcessHeap(), 0, p);
      VERIFY(mem.ptr() == p, caseLabel);
   }
}


void testHeapMemDtor()
{
   // Todo - how to check that mem was freed?
}


void testHeapMemMoveCtor()
{
   {
      const std::string caseLabel{"HeapMem move ctor"};
      wchar_t* p =
         static_cast<wchar_t*>(HeapAlloc(GetProcessHeap(), 0, 5 * sizeof(wchar_t)));
      HeapMem<wchar_t> memA{p};
      HeapMem<wchar_t> memB{std::move(memA)};
      VERIFY(memA.ptr() == nullptr, caseLabel);
      VERIFY(memB.ptr() == p, caseLabel);
   }
}


void testHeapMemMoveAssignment()
{
   {
      const std::string caseLabel{"HeapMem move assignment"};
      wchar_t* p =
         static_cast<wchar_t*>(HeapAlloc(GetProcessHeap(), 0, 5 * sizeof(wchar_t)));
      HeapMem<wchar_t> memA{p};
      HeapMem<wchar_t> memB;
      memB = std::move(memA);
      VERIFY(memA.ptr() == nullptr, caseLabel);
      VERIFY(memB.ptr() == p, caseLabel);
   }
}


void testHeapMemOperatorBool()
{
   {
      const std::string caseLabel{"HeapMem operator bool for allocated memory"};
      wchar_t* p =
         static_cast<wchar_t*>(HeapAlloc(GetProcessHeap(), 0, 5 * sizeof(wchar_t)));
      HeapMem<wchar_t> mem{p};
      VERIFY(mem.operator bool(), caseLabel);
   }
   {
      const std::string caseLabel{"HeapMem operator bool for empty object"};
      HeapMem<wchar_t> mem;
      VERIFY(!mem.operator bool(), caseLabel);
   }
}


void testHeapMemSwap()
{
   {
      const std::string caseLabel{"HeapMem swap"};
      wchar_t* pA =
         static_cast<wchar_t*>(HeapAlloc(GetProcessHeap(), 0, 5 * sizeof(wchar_t)));
      HeapMem<wchar_t> memA{pA};
      wchar_t* pB =
         static_cast<wchar_t*>(HeapAlloc(GetProcessHeap(), 0, 2 * sizeof(wchar_t)));
      HeapMem<wchar_t> memB{pB};
      swap(memA, memB);
      VERIFY(memA.ptr() == pB, caseLabel);
      VERIFY(memB.ptr() == pA, caseLabel);
   }
}


void testHeapMemPtr()
{
   {
      const std::string caseLabel{"HeapMem::ptr"};
      wchar_t* p =
         static_cast<wchar_t*>(HeapAlloc(GetProcessHeap(), 0, 5 * sizeof(wchar_t)));
      HeapMem<wchar_t> mem{p};
      VERIFY(mem.ptr() == p, caseLabel);
   }
}


void testHeapMemConstPtr()
{
   {
      const std::string caseLabel{"HeapMem::ptr const"};
      wchar_t* p =
         static_cast<wchar_t*>(HeapAlloc(GetProcessHeap(), 0, 5 * sizeof(wchar_t)));
      const HeapMem<wchar_t> mem{p};
      VERIFY(mem.ptr() == p, caseLabel);
   }
}


void testHeapMemClear()
{
   {
      const std::string caseLabel{"HeapMem::clear"};
      wchar_t* p = static_cast<wchar_t*>(HeapAlloc(GetProcessHeap(), 0, sizeof(wchar_t)));
      HeapMem<wchar_t> mem{p};
      mem.clear();
      VERIFY(mem.ptr() == nullptr, caseLabel);
   }
}


void testHeapMemFree()
{
   {
      const std::string caseLabel{"HeapMem::free"};
      wchar_t* p = static_cast<wchar_t*>(HeapAlloc(GetProcessHeap(), 0, sizeof(wchar_t)));
      HeapMem<wchar_t> mem{p};
      mem.free();
      VERIFY(mem.ptr() == nullptr, caseLabel);
   }
}

} // namespace


void testMemUtil()
{
   testCoTaskMemDefaultCtor();
   testCoTaskMemCtorWithPointer();
   testCoTaskMemDtor();
   testCoTaskMemMoveCtor();
   testCoTaskMemMoveAssignment();
   testCoTaskMemOperatorBool();
   testCoTaskMemSwap();
   testCoTaskMemPtr();
   testCoTaskMemConstPtr();
   testCoTaskMemAddr();
   testCoTaskMemClear();
   testCoTaskMemFree();

   testGlobalMemDefaultCtor();
   testGlobalMemCtorWithPointer();
   testGlobalMemDtor();
   testGlobalMemMoveCtor();
   testGlobalMemMoveAssignment();
   testGlobalMemOperatorBool();
   testGlobalMemSwap();
   testGlobalMemPtr();
   testGlobalMemConstPtr();
   testGlobalMemAddr();
   testGlobalMemClear();
   testGlobalMemFree();

   testLocalMemDefaultCtor();
   testLocalMemCtorWithPointer();
   testLocalMemDtor();
   testLocalMemMoveCtor();
   testLocalMemMoveAssignment();
   testLocalMemOperatorBool();
   testLocalMemSwap();
   testLocalMemPtr();
   testLocalMemConstPtr();
   testLocalMemAddr();
   testLocalMemClear();
   testLocalMemFree();

   testHeapMemDefaultCtor();
   testHeapMemCtorWithPointer();
   testHeapMemCtorWithAll();
   testHeapMemDtor();
   testHeapMemMoveCtor();
   testHeapMemMoveAssignment();
   testHeapMemOperatorBool();
   testHeapMemSwap();
   testHeapMemPtr();
   testHeapMemConstPtr();
   testHeapMemClear();
   testHeapMemFree();
}
