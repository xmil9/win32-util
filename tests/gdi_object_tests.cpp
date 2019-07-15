//
// Win32 utilities library
// Tests for GDI object utilities.
//
// Jun-2019, Michael Lindner
// MIT license
//
#include "gdi_object_tests.h"
#include "gdi_object.h"
#include "test_util.h"
#include <cassert>

using namespace win32;


namespace
{
///////////////////

void testGdiObjValidTypes()
{
   GdiObj<HPEN> pen;
   GdiObj<HBRUSH> brush;
   GdiObj<HFONT> font;
   GdiObj<HBITMAP> bm;
   GdiObj<HRGN> region;
   GdiObj<HPALETTE> palette;
}


void testGdiObjDefaultCtor()
{
   {
      const std::string caseLabel{"GdiObj default ctor"};
      GdiObj<HBRUSH> obj;
      VERIFY(obj.handle() == NULL, caseLabel);
   }
}


void testGdiObjHandleCtor()
{
   {
      const std::string caseLabel{"GdiObj handle ctor"};
      HBRUSH h = ::CreateSolidBrush(RGB(100, 100, 100));
      GdiObj<HBRUSH> obj{h};
      VERIFY(obj.handle() == h, caseLabel);
   }
   {
      const std::string caseLabel{"GdiObj handle ctor for NULL handle"};
      GdiObj<HBRUSH> obj{NULL};
      VERIFY(obj.handle() == NULL, caseLabel);
   }
}


void testGdiObjDtor()
{
   {
      const std::string caseLabel{"GdiObj dtor"};
      HBRUSH h = ::CreateSolidBrush(RGB(100, 100, 100));
      {
         GdiObj<HBRUSH> obj{h};
      }
      VERIFY(!isValidBrush(h), caseLabel);
   }
}


void testGdiObjMoveCtor()
{
   {
      const std::string caseLabel{"GdiObj move ctor"};
      HBRUSH h = ::CreateSolidBrush(RGB(100, 100, 100));
      GdiObj<HBRUSH> a{h};
      GdiObj<HBRUSH> b{std::move(a)};
      VERIFY(b.handle() == h, caseLabel);
      VERIFY(a.handle() == NULL, caseLabel);
   }
}


void testGdiObjMoveAssignment()
{
   {
      const std::string caseLabel{"GdiObj move assignment"};
      HBRUSH h = ::CreateSolidBrush(RGB(100, 100, 100));
      GdiObj<HBRUSH> a{h};
      GdiObj<HBRUSH> b;
      b = std::move(a);
      VERIFY(b.handle() == h, caseLabel);
      VERIFY(a.handle() == NULL, caseLabel);
   }
}


void testGdiObjConversionToBool()
{
   {
      const std::string caseLabel{"GdiObj conversion to bool for non-null handle"};
      HBRUSH h = ::CreateSolidBrush(RGB(100, 100, 100));
      GdiObj<HBRUSH> obj{h};
      VERIFY(static_cast<bool>(obj), caseLabel);
   }
   {
      const std::string caseLabel{"GdiObj conversion to bool for null handle"};
      GdiObj<HBRUSH> obj;
      VERIFY(!static_cast<bool>(obj), caseLabel);
   }
}


void testGdiObjConversionToHandle()
{
   {
      const std::string caseLabel{"GdiObj conversion to handle for non-null handle"};
      HBRUSH h = ::CreateSolidBrush(RGB(100, 100, 100));
      GdiObj<HBRUSH> obj{h};
      VERIFY(static_cast<HBRUSH>(obj) == h, caseLabel);
   }
   {
      const std::string caseLabel{"GdiObj conversion to handle for null handle"};
      GdiObj<HBRUSH> obj;
      VERIFY(static_cast<HBRUSH>(obj) == NULL, caseLabel);
   }
}


void testGdiObjHandle()
{
   {
      const std::string caseLabel{"GdiObj::handle"};
      HBRUSH h = ::CreateSolidBrush(RGB(100, 100, 100));
      GdiObj<HBRUSH> obj{h};
      VERIFY(obj.handle() == h, caseLabel);
   }
   {
      const std::string caseLabel{"GdiObj::handle for null handle"};
      GdiObj<HBRUSH> obj;
      VERIFY(obj.handle() == NULL, caseLabel);
   }
}


void testGdiObjSwap()
{
   {
      const std::string caseLabel{"GdiObj swap"};
      HBRUSH ha = ::CreateSolidBrush(RGB(100, 100, 100));
      GdiObj<HBRUSH> a{ha};
      HBRUSH hb = ::CreateSolidBrush(RGB(200, 200, 200));
      GdiObj<HBRUSH> b{hb};
      swap(a, b);
      VERIFY(a.handle() == hb, caseLabel);
      VERIFY(b.handle() == ha, caseLabel);
   }
}


void testGdiObjClear()
{
   {
      const std::string caseLabel{"GdiObj::clear"};
      HBRUSH h = ::CreateSolidBrush(RGB(100, 100, 100));
      GdiObj<HBRUSH> obj{h};
      obj.clear();
      VERIFY(obj.handle() == NULL, caseLabel);
      ::DeleteObject(h);
   }
}


void testGdiObjRelease()
{
   {
      const std::string caseLabel{"GdiObj::release"};
      HBRUSH h = ::CreateSolidBrush(RGB(100, 100, 100));
      GdiObj<HBRUSH> obj{h};
      obj.release();
      VERIFY(!isValidBrush(h), caseLabel);
      VERIFY(obj.handle() == NULL, caseLabel);
   }
}


///////////////////

void testDeselectedObjectValidTypes()
{
   GdiObj<HPEN> pen;
   GdiObj<HBRUSH> brush;
   GdiObj<HFONT> font;
   GdiObj<HBITMAP> bm;
   GdiObj<HRGN> region;
}


void testDeselectedObjectDefaultCtor()
{
   {
      const std::string caseLabel{"DeselectedObj default ctor"};
      DeselectedObj<HBRUSH> deselected;
      VERIFY(deselected.hdc() == NULL, caseLabel);
      VERIFY(deselected.handle() == NULL, caseLabel);
   }
}


void testDeselectedObjectCtorForDcAndHandle(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"DeselectedObj ctor for DC and handle"};
      HDC hdc = ::GetDC(testRunnerWnd);
      HPEN newPen = ::CreatePen(PS_SOLID, 2, RGB(200, 200, 200));
      HPEN prevPen = reinterpret_cast<HPEN>(::SelectObject(hdc, newPen));
      {
         DeselectedObj<HPEN> deselected{hdc, prevPen};
         VERIFY(deselected.hdc() == hdc, caseLabel);
         VERIFY(deselected.handle() == prevPen, caseLabel);
      }
      ::DeleteObject(newPen);
      ::ReleaseDC(testRunnerWnd, hdc);
   }
}


void testDeselectedObjectDtor(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"DeselectedObj dtor"};
      HDC hdc = ::GetDC(testRunnerWnd);
      HPEN newPen = ::CreatePen(PS_SOLID, 2, RGB(200, 200, 200));
      HPEN prevPen = reinterpret_cast<HPEN>(::SelectObject(hdc, newPen));
      assert(reinterpret_cast<HPEN>(::GetCurrentObject(hdc, OBJ_PEN)) == newPen);
      {
         DeselectedObj<HPEN> deselected{hdc, prevPen};
      }
      VERIFY(reinterpret_cast<HPEN>(::GetCurrentObject(hdc, OBJ_PEN)) == prevPen, caseLabel);
      ::DeleteObject(newPen);
      ::ReleaseDC(testRunnerWnd, hdc);
   }
}


void testDeselectedObjectMoveCtor(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"DeselectedObj move ctor"};
      HDC hdc = ::GetDC(testRunnerWnd);
      HPEN newPen = ::CreatePen(PS_SOLID, 2, RGB(200, 200, 200));
      HPEN prevPen = reinterpret_cast<HPEN>(::SelectObject(hdc, newPen));
      {
         DeselectedObj<HPEN> a{hdc, prevPen};
         DeselectedObj<HPEN> b{std::move(a)};
         VERIFY(a.hdc() == NULL, caseLabel);
         VERIFY(a.handle() == NULL, caseLabel);
         VERIFY(b.hdc() == hdc, caseLabel);
         VERIFY(b.handle() == prevPen, caseLabel);
      }
      ::DeleteObject(newPen);
      ::ReleaseDC(testRunnerWnd, hdc);
   }
}


void testDeselectedObjectMoveAssignment(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"DeselectedObj move assignment"};
      HDC hdc = ::GetDC(testRunnerWnd);
      HPEN newPen = ::CreatePen(PS_SOLID, 2, RGB(200, 200, 200));
      HPEN prevPen = reinterpret_cast<HPEN>(::SelectObject(hdc, newPen));
      {
         DeselectedObj<HPEN> a{hdc, prevPen};
         DeselectedObj<HPEN> b;
         b = std::move(a);
         VERIFY(a.hdc() == NULL, caseLabel);
         VERIFY(a.handle() == NULL, caseLabel);
         VERIFY(b.hdc() == hdc, caseLabel);
         VERIFY(b.handle() == prevPen, caseLabel);
      }
      ::DeleteObject(newPen);
      ::ReleaseDC(testRunnerWnd, hdc);
   }
}


void testDeselectedObjectHdc(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"DeselectedObj::hdc"};
      HDC hdc = ::GetDC(testRunnerWnd);
      HPEN newPen = ::CreatePen(PS_SOLID, 2, RGB(200, 200, 200));
      HPEN prevPen = reinterpret_cast<HPEN>(::SelectObject(hdc, newPen));
      {
         DeselectedObj<HPEN> deselected{hdc, prevPen};
         VERIFY(deselected.hdc() == hdc, caseLabel);
      }
      ::DeleteObject(newPen);
      ::ReleaseDC(testRunnerWnd, hdc);
   }
   {
      const std::string caseLabel{"DeselectedObj::hdc for default constructed object"};
      DeselectedObj<HPEN> deselected;
      VERIFY(deselected.hdc() == NULL, caseLabel);
   }
}


void testDeselectedObjectHandle(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"DeselectedObj::handle"};
      HDC hdc = ::GetDC(testRunnerWnd);
      HPEN newPen = ::CreatePen(PS_SOLID, 2, RGB(200, 200, 200));
      HPEN prevPen = reinterpret_cast<HPEN>(::SelectObject(hdc, newPen));
      {
         DeselectedObj<HPEN> deselected{hdc, prevPen};
         VERIFY(deselected.handle() == prevPen, caseLabel);
      }
      ::DeleteObject(newPen);
      ::ReleaseDC(testRunnerWnd, hdc);
   }
   {
      const std::string caseLabel{"DeselectedObj::handle for default constructed object"};
      DeselectedObj<HPEN> deselected;
      VERIFY(deselected.handle() == NULL, caseLabel);
   }
}


void testDeselectedObjectConversionToBool(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"DeselectedObj conversion to bool for populated object"};
      HDC hdc = ::GetDC(testRunnerWnd);
      HPEN newPen = ::CreatePen(PS_SOLID, 2, RGB(200, 200, 200));
      HPEN prevPen = reinterpret_cast<HPEN>(::SelectObject(hdc, newPen));
      {
         DeselectedObj<HPEN> deselected{hdc, prevPen};
         VERIFY(deselected.operator bool(), caseLabel);
      }
      ::DeleteObject(newPen);
      ::ReleaseDC(testRunnerWnd, hdc);
   }
   {
      const std::string caseLabel{"DeselectedObj conversion to bool for default constructed object"};
      DeselectedObj<HPEN> deselected;
      VERIFY(!deselected.operator bool(), caseLabel);
   }
}


void testDeselectedObjectSwap(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"DeselectedObj swap"};
      HDC hdc = ::GetDC(testRunnerWnd);
      HBRUSH ha = ::CreateSolidBrush(RGB(100, 100, 100));
      HBRUSH hb = ::CreateSolidBrush(RGB(200, 200, 200));
      {
         HBRUSH prevA = reinterpret_cast<HBRUSH>(::SelectObject(hdc, ha));
         DeselectedObj<HBRUSH> deselectedA{hdc, prevA};
         HBRUSH prevB = reinterpret_cast<HBRUSH>(::SelectObject(hdc, hb));
         DeselectedObj<HBRUSH> deselectedB{hdc, prevB};
         swap(deselectedA, deselectedB);
         VERIFY(deselectedA.hdc() == hdc, caseLabel);
         VERIFY(deselectedA.handle() == prevB, caseLabel);
         VERIFY(deselectedB.hdc() == hdc, caseLabel);
         VERIFY(deselectedB.handle() == prevA, caseLabel);

      }
      ::DeleteObject(ha);
      ::DeleteObject(hb);
      ::ReleaseDC(testRunnerWnd, hdc);
   }
}


void testDeselectedObjectClear(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"DeselectedObj::clear"};
      HDC hdc = ::GetDC(testRunnerWnd);
      HPEN newPen = ::CreatePen(PS_SOLID, 2, RGB(200, 200, 200));
      HPEN prevPen = reinterpret_cast<HPEN>(::SelectObject(hdc, newPen));
      {
         DeselectedObj<HPEN> deselected{hdc, prevPen};
         deselected.clear();
         VERIFY(deselected.hdc() == NULL, caseLabel);
         VERIFY(deselected.handle() == NULL, caseLabel);
         ::SelectObject(hdc, prevPen);
      }
      ::DeleteObject(newPen);
      ::ReleaseDC(testRunnerWnd, hdc);
   }
}


void testDeselectedObjectRestore(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"DeselectedObj::restore"};
      HDC hdc = ::GetDC(testRunnerWnd);
      HPEN newPen = ::CreatePen(PS_SOLID, 2, RGB(200, 200, 200));
      HPEN prevPen = reinterpret_cast<HPEN>(::SelectObject(hdc, newPen));
      {
         DeselectedObj<HPEN> deselected{hdc, prevPen};
         deselected.restore();
         VERIFY(reinterpret_cast<HPEN>(::GetCurrentObject(hdc, OBJ_PEN)) == prevPen, caseLabel);
         VERIFY(deselected.hdc() == NULL, caseLabel);
         VERIFY(deselected.handle() == NULL, caseLabel);
      }
      ::DeleteObject(newPen);
      ::ReleaseDC(testRunnerWnd, hdc);
   }
}


///////////////////

void testSelectObject(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"selectObject for pen"};
      HDC hdc = ::GetDC(testRunnerWnd);
      HPEN pen = ::CreatePen(PS_SOLID, 2, RGB(200, 200, 200));
      HPEN prevPen = reinterpret_cast<HPEN>(::GetCurrentObject(hdc, OBJ_PEN));
      {
         DeselectedObj<HPEN> deselectedPen = selectObject(hdc, pen);
         VERIFY(reinterpret_cast<HPEN>(::GetCurrentObject(hdc, OBJ_PEN)) == pen, caseLabel);
         VERIFY(deselectedPen.handle() == prevPen, caseLabel);
      }
      ::DeleteObject(pen);
      ::ReleaseDC(testRunnerWnd, hdc);
   }
   {
      const std::string caseLabel{"selectObject for brush"};
      HDC hdc = ::GetDC(testRunnerWnd);
      HBRUSH brush = ::CreateSolidBrush(RGB(100, 100, 100));
      HBRUSH prevBrush = reinterpret_cast<HBRUSH>(::GetCurrentObject(hdc, OBJ_BRUSH));
      {
         DeselectedObj<HBRUSH> deselectedBrush = selectObject(hdc, brush);
         VERIFY(reinterpret_cast<HBRUSH>(::GetCurrentObject(hdc, OBJ_BRUSH)) == brush, caseLabel);
         VERIFY(deselectedBrush.handle() == prevBrush, caseLabel);
      }
      ::DeleteObject(brush);
      ::ReleaseDC(testRunnerWnd, hdc);
   }
}


void testIsValidBrush()
{
   {
      const std::string caseLabel{"isValidBrush for valid brush"};
      HBRUSH h = ::CreateSolidBrush(RGB(100, 100, 100));
      VERIFY(isValidBrush(h), caseLabel);
      ::DeleteObject(h);
   }
   {
      const std::string caseLabel{"isValidBrush for null brush"};
      VERIFY(!isValidBrush(NULL), caseLabel);
   }
   {
      const std::string caseLabel{"isValidBrush for handle that is not a brush"};
      HPEN h = ::CreatePen(PS_SOLID, 2, RGB(100, 100, 100));
      VERIFY(!isValidBrush(reinterpret_cast<HBRUSH>(h)), caseLabel);
      ::DeleteObject(h);
   }
   {
      const std::string caseLabel{"isValidBrush for brush that has been deleted"};
      HBRUSH h = ::CreateSolidBrush(RGB(100, 100, 100));
      ::DeleteObject(h);
      VERIFY(!isValidBrush(h), caseLabel);
   }
}


void testIsValidPen()
{
   {
      const std::string caseLabel{"isValidPen for valid pen"};
      HPEN h = ::CreatePen(PS_SOLID, 2, RGB(100, 100, 100));
      VERIFY(isValidPen(h), caseLabel);
      ::DeleteObject(h);
   }
   {
      const std::string caseLabel{"isValidPen for null pen"};
      VERIFY(!isValidPen(NULL), caseLabel);
   }
   {
      const std::string caseLabel{"isValidPen for handle that is not a pen"};
      HBRUSH h = ::CreateSolidBrush(RGB(100, 100, 100));
      VERIFY(!isValidPen(reinterpret_cast<HPEN>(h)), caseLabel);
      ::DeleteObject(h);
   }
   {
      const std::string caseLabel{"isValidPen for pen that has been deleted"};
      HPEN h = ::CreatePen(PS_SOLID, 2, RGB(100, 100, 100));
      ::DeleteObject(h);
      VERIFY(!isValidPen(h), caseLabel);
   }
}

} // namespace


void testGdiObject(HWND testRunnerWnd)
{
   testGdiObjValidTypes();
   testGdiObjDefaultCtor();
   testGdiObjHandleCtor();
   testGdiObjDtor();
   testGdiObjMoveCtor();
   testGdiObjMoveAssignment();
   testGdiObjConversionToBool();
   testGdiObjConversionToHandle();
   testGdiObjHandle();
   testGdiObjSwap();
   testGdiObjClear();
   testGdiObjRelease();

   testDeselectedObjectValidTypes();
   testDeselectedObjectDefaultCtor();
   testDeselectedObjectCtorForDcAndHandle(testRunnerWnd);
   testDeselectedObjectDtor(testRunnerWnd);
   testDeselectedObjectMoveCtor(testRunnerWnd);
   testDeselectedObjectMoveAssignment(testRunnerWnd);
   testDeselectedObjectHdc(testRunnerWnd);
   testDeselectedObjectHandle(testRunnerWnd);
   testDeselectedObjectConversionToBool(testRunnerWnd);
   testDeselectedObjectSwap(testRunnerWnd);
   testDeselectedObjectClear(testRunnerWnd);
   testDeselectedObjectRestore(testRunnerWnd);

   testSelectObject(testRunnerWnd);
   testIsValidBrush();
   testIsValidPen();
}
