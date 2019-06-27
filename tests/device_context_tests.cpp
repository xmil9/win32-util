//
// Win32 utilities library
// Tests for device context utilities.
//
// Jun-2019, Michael Lindner
// MIT license
//
#include "device_context_tests.h"
#include "device_context.h"
#include "message_util.h"
#include "test_util.h"
#include "window.h"
#include <cassert>
#include <functional>

using namespace win32;


namespace
{

///////////////////

void testDCDefaultCtor(HWND /*testRunnerWnd*/)
{
   {
      const std::string caseLabel{"DC default ctor"};
      DC dc;
      VERIFY(dc.hdc() == NULL, caseLabel);
   }
}


void testDCCtorForHDC(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"DC ctor for HDC"};
      HDC hdc = ::GetDC(testRunnerWnd);
      DC dc{hdc};
      VERIFY(dc.hdc() == hdc, caseLabel);
      ::ReleaseDC(testRunnerWnd, hdc);
   }
}


void testDCDtor(HWND /*testRunnerWnd*/)
{
   // Dtor does nothing.
}


void testDCCopyCtor(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"DC copy ctor"};
      HDC hdc = ::GetDC(testRunnerWnd);
      DC a{hdc};
      DC b{a};
      VERIFY(b.hdc() == hdc, caseLabel);
      VERIFY(a.hdc() == hdc, caseLabel);
      ::ReleaseDC(testRunnerWnd, hdc);
   }
}


void testDCMoveCtor(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"DC move ctor"};
      HDC hdc = ::GetDC(testRunnerWnd);
      DC a{hdc};
      DC b{std::move(a)};
      VERIFY(b.hdc() == hdc, caseLabel);
      VERIFY(a.hdc() == NULL, caseLabel);
      ::ReleaseDC(testRunnerWnd, hdc);
   }
}


void testDCCopyAssignment(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"DC copy assignment"};
      HDC hdc = ::GetDC(testRunnerWnd);
      DC a{hdc};
      DC b;
      b = a;
      VERIFY(b.hdc() == hdc, caseLabel);
      VERIFY(a.hdc() == hdc, caseLabel);
      ::ReleaseDC(testRunnerWnd, hdc);
   }
}


void testDCMoveAssignment(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"DC move assignment"};
      HDC hdc = ::GetDC(testRunnerWnd);
      DC a{hdc};
      DC b;
      b = std::move(a);
      VERIFY(b.hdc() == hdc, caseLabel);
      VERIFY(a.hdc() == NULL, caseLabel);
      ::ReleaseDC(testRunnerWnd, hdc);
   }
}


void testDCHdc(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"DC::hdc"};
      HDC hdc = ::GetDC(testRunnerWnd);
      DC dc{hdc};
      VERIFY(dc.hdc() == hdc, caseLabel);
      ::ReleaseDC(testRunnerWnd, hdc);
   }
}


void testDCConversionToBool(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"DC conversion to bool for populated DC"};
      HDC hdc = ::GetDC(testRunnerWnd);
      DC dc{hdc};
      VERIFY(dc.operator bool(), caseLabel);
      ::ReleaseDC(testRunnerWnd, hdc);
   }
   {
      const std::string caseLabel{"DC conversion to bool for empty DC"};
      DC dc;
      VERIFY(!dc.operator bool(), caseLabel);
   }
}


void testDCConversionToHDC(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"DC conversion to HDC"};
      HDC hdc = ::GetDC(testRunnerWnd);
      DC dc{hdc};
      VERIFY(dc.operator HDC() == hdc, caseLabel);
      ::ReleaseDC(testRunnerWnd, hdc);
   }
}


void testDCSwap(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"DC swap"};
      HDC hdc = ::GetDC(testRunnerWnd);
      DC a{hdc};
      DC b;
      swap(a, b);
      VERIFY(a.hdc() == NULL, caseLabel);
      VERIFY(b.hdc() == hdc, caseLabel);
      ::ReleaseDC(testRunnerWnd, hdc);
   }
}


void testDCClear(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"DC::clear"};
      HDC hdc = ::GetDC(testRunnerWnd);
      DC dc{hdc};
      dc.clear();
      VERIFY(dc.hdc() == NULL, caseLabel);
      ::ReleaseDC(testRunnerWnd, hdc);
   }
}


void testDCSelectedObject(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"DC::selectedObject for pen"};
      HDC hdc = ::GetDC(testRunnerWnd);
      HPEN pen = ::CreatePen(PS_SOLID, 1, RGB(200, 100, 0));
      HPEN prevPen = reinterpret_cast<HPEN>(::SelectObject(hdc, pen));
      DC dc{hdc};
      VERIFY(dc.selectedObject<HPEN>(GdiObjectType::Pen) == pen, caseLabel);
      ::SelectObject(hdc, prevPen);
      ::DeleteObject(pen);
      ::ReleaseDC(testRunnerWnd, hdc);
   }
   {
      const std::string caseLabel{"DC::selectedObject for brush"};
      HDC hdc = ::GetDC(testRunnerWnd);
      HBRUSH brush = ::CreateSolidBrush(RGB(200, 100, 0));
      HBRUSH prevBrush = reinterpret_cast<HBRUSH>(::SelectObject(hdc, brush));
      DC dc{hdc};
      VERIFY(dc.selectedObject<HBRUSH>(GdiObjectType::Brush) == brush, caseLabel);
      ::SelectObject(hdc, prevBrush);
      ::DeleteObject(brush);
      ::ReleaseDC(testRunnerWnd, hdc);
   }
}


void testDCSelectObject(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"DC::selectObject for pen"};
      HDC hdc = ::GetDC(testRunnerWnd);
      DC dc{hdc};
      HPEN prevPen = dc.selectedObject<HPEN>(GdiObjectType::Pen);
      HPEN pen = ::CreatePen(PS_SOLID, 1, RGB(200, 100, 0));
      {
         DeselectedObj<HPEN> deselectedPen = dc.selectObject(pen);
         VERIFY(dc.selectedObject<HPEN>(GdiObjectType::Pen) == pen, caseLabel);
         VERIFY(deselectedPen.handle() == prevPen, caseLabel);
      }
      ::DeleteObject(pen);
      ::ReleaseDC(testRunnerWnd, hdc);
   }
   {
      const std::string caseLabel{"DC::selectObject for brush"};
      HDC hdc = ::GetDC(testRunnerWnd);
      DC dc{hdc};
      HBRUSH prevBrush = dc.selectedObject<HBRUSH>(GdiObjectType::Brush);
      HBRUSH brush = ::CreateSolidBrush(RGB(200, 100, 0));
      {
         DeselectedObj<HBRUSH> deselectedBrush = dc.selectObject(brush);
         VERIFY(dc.selectedObject<HBRUSH>(GdiObjectType::Brush) == brush, caseLabel);
         VERIFY(deselectedBrush.handle() == prevBrush, caseLabel);
      }
      ::DeleteObject(brush);
      ::ReleaseDC(testRunnerWnd, hdc);
   }
}


///////////////////

void testSharedDCDefaultCtor(HWND /*testRunnerWnd*/)
{
   {
      const std::string caseLabel{"SharedDC default ctor"};
      SharedDC dc;
      VERIFY(dc.hdc() == NULL, caseLabel);
      VERIFY(dc.hwnd() == NULL, caseLabel);
   }
}


void testSharedDCCtorForHDC(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"SharedDC ctor for DC and window"};
      HDC hdc = ::GetDC(testRunnerWnd);
      SharedDC dc{hdc, testRunnerWnd};
      VERIFY(dc.hdc() == hdc, caseLabel);
      VERIFY(dc.hwnd() == testRunnerWnd, caseLabel);
   }
}


void testSharedDCDtor(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"SharedDC dtor"};
      HDC hdc = ::GetDC(testRunnerWnd);
      {
         SharedDC dc{hdc, testRunnerWnd};
      }
      // Cannot verify.
   }
}


void testSharedDCMoveCtor(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"SharedDC move ctor"};
      HDC hdc = ::GetDC(testRunnerWnd);
      SharedDC a{hdc, testRunnerWnd};
      SharedDC b{std::move(a)};
      VERIFY(b.hdc() == hdc, caseLabel);
      VERIFY(b.hwnd() == testRunnerWnd, caseLabel);
      VERIFY(a.hdc() == NULL, caseLabel);
      VERIFY(a.hwnd() == NULL, caseLabel);
   }
}


void testSharedDCMoveAssignment(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"SharedDC move assignment"};
      HDC hdc = ::GetDC(testRunnerWnd);
      SharedDC a{hdc, testRunnerWnd};
      SharedDC b;
      b = std::move(a);
      VERIFY(b.hdc() == hdc, caseLabel);
      VERIFY(b.hwnd() == testRunnerWnd, caseLabel);
      VERIFY(a.hdc() == NULL, caseLabel);
      VERIFY(a.hwnd() == NULL, caseLabel);
   }
}


void testSharedDCHwnd(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"SharedDC::sourceWindow"};
      HDC hdc = ::GetDC(testRunnerWnd);
      SharedDC dc{hdc, testRunnerWnd};
      VERIFY(dc.hwnd() == testRunnerWnd, caseLabel);
   }
}


void testSharedDCSwap(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"SharedDC swap"};
      HDC hdc = ::GetDC(testRunnerWnd);
      SharedDC a{hdc, testRunnerWnd};
      HDC screenHdc = ::GetDC(NULL);
      SharedDC b{screenHdc, NULL};
      swap(a, b);
      VERIFY(a.hdc() == screenHdc, caseLabel);
      VERIFY(a.hwnd() == NULL, caseLabel);
      VERIFY(b.hdc() == hdc, caseLabel);
      VERIFY(b.hwnd() == testRunnerWnd, caseLabel);
   }
}


void testSharedDCClear(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"SharedDC::clear"};
      HDC hdc = ::GetDC(testRunnerWnd);
      SharedDC dc{hdc, testRunnerWnd};
      dc.clear();
      VERIFY(dc.hdc() == NULL, caseLabel);
      VERIFY(dc.hwnd() == NULL, caseLabel);
   }
}


void testSharedDCRelease(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"SharedDC::release"};
      HDC hdc = ::GetDC(testRunnerWnd);
      SharedDC dc{hdc, testRunnerWnd};
      dc.release();
      VERIFY(dc.hdc() == NULL, caseLabel);
      VERIFY(dc.hwnd() == NULL, caseLabel);
   }
}


///////////////////

void testOwnedDCDefaultCtor(HWND /*testRunnerWnd*/)
{
   {
      const std::string caseLabel{"OwnedDC default ctor"};
      OwnedDC dc;
      VERIFY(dc.hdc() == NULL, caseLabel);
   }
}


void testOwnedDCCtorForHDC(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"OwnedDC default ctor"};
      HDC hdc = ::GetDC(testRunnerWnd);
      HDC createdDc = ::CreateCompatibleDC(hdc);
      OwnedDC dc{createdDc};
      VERIFY(dc.hdc() == createdDc, caseLabel);
      ::ReleaseDC(testRunnerWnd, hdc);
   }
}


void testOwnedDCDtor(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"OwnedDC ctor for HDC"};
      HDC hdc = ::GetDC(testRunnerWnd);
      HDC createdDc = ::CreateCompatibleDC(hdc);
      {
         OwnedDC dc{createdDc};
      }
      // Cannot verify.
      ::ReleaseDC(testRunnerWnd, hdc);
   }
}


void testOwnedDCMoveCtor(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"OwnedDC move ctor"};
      HDC hdc = ::GetDC(testRunnerWnd);
      HDC createdDc = ::CreateCompatibleDC(hdc);
      OwnedDC a{createdDc};
      OwnedDC b{std::move(a)};
      VERIFY(b.hdc() == createdDc, caseLabel);
      VERIFY(a.hdc() == NULL, caseLabel);
      ::ReleaseDC(testRunnerWnd, hdc);
   }
}


void testOwnedDCMoveAssignment(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"OwnedDC move assignment"};
      HDC hdc = ::GetDC(testRunnerWnd);
      HDC createdDc = ::CreateCompatibleDC(hdc);
      OwnedDC a{createdDc};
      OwnedDC b;
      b = std::move(a);
      VERIFY(b.hdc() == createdDc, caseLabel);
      VERIFY(a.hdc() == NULL, caseLabel);
      ::ReleaseDC(testRunnerWnd, hdc);
   }
}


void testOwnedDCSwap(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"OwnedDC swap"};
      HDC hdc = ::GetDC(testRunnerWnd);
      HDC hdcA = ::CreateCompatibleDC(hdc);
      OwnedDC a{hdcA};
      HDC hdcB = ::CreateCompatibleDC(hdc);
      OwnedDC b{hdcB};
      swap(a, b);
      VERIFY(a.hdc() == hdcB, caseLabel);
      VERIFY(b.hdc() == hdcA, caseLabel);
   }
}


void testOwnedDCRelease(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"OwnedDC::release"};
      HDC hdc = ::GetDC(testRunnerWnd);
      HDC createdDc = ::CreateCompatibleDC(hdc);
      OwnedDC dc{createdDc};
      dc.release();
      VERIFY(dc.hdc() == NULL, caseLabel);
   }
}


///////////////////

// Window that can hosts test cases in its WM_PAINT message handling.
class PaintDCTestWindow : public Window
{
 public:
   using PaintCallback_t = std::function<void(HWND)>;
   explicit PaintDCTestWindow(PaintCallback_t testCallback, bool& closeFlag);

 private:
   bool onClose() override;
   bool onPaint() override;

 private:
   PaintCallback_t m_testCallback;
   bool& m_closeFlag;
};


PaintDCTestWindow::PaintDCTestWindow(PaintCallback_t testCallback, bool& closeFlag)
   : m_testCallback{testCallback}, m_closeFlag{closeFlag}
{
   assert(m_testCallback);
}

bool PaintDCTestWindow::onClose()
{
   Window::onClose();
   m_closeFlag = true;
   return true;
}

bool PaintDCTestWindow::onPaint()
{
   m_testCallback(hwnd());
   // Close the window. This will also stop the modal message loop.
   close();
   return true;
}


// Shows window that hosts a given PaintDC test case.
void runPaintDCTest(HWND testRunnerWnd, PaintDCTestWindow::PaintCallback_t testCallback)
{
   bool stopMsgLoopFlag = false;
   PaintDCTestWindow wnd{testCallback, stopMsgLoopFlag};
   wnd.create(testRunnerWnd, {10, 10, 200, 200}, L"PaintDC test", WS_OVERLAPPEDWINDOW);
   wnd.setVisible(true);
   ::InvalidateRect(wnd.hwnd(), NULL, true);
   // Start a modal message loop because the main message loop does not get a chance
   // to execute inside the test code.
   modalMessageLoop(wnd, stopMsgLoopFlag);
}


///////////////////

void testPaintDCDefaultCtor(HWND /*testRunnerWnd*/)
{
   {
      const std::string caseLabel{"PaintDC default ctor"};
      PaintDC dc;
      VERIFY(dc.hdc() == NULL, caseLabel);
      VERIFY(dc.hwnd() == NULL, caseLabel);
      VERIFY(dc.paintInfo().hdc == NULL, caseLabel);
   }
}


void testPaintDCCtorForHwnd(HWND testRunnerWnd)
{
   auto testCase = [](HWND testWnd) {
      const std::string caseLabel{"PaintDC ctor for hwnd"};
      PaintDC dc{testWnd};
      VERIFY(dc.hdc() != NULL, caseLabel);
      VERIFY(dc.hwnd() == testWnd, caseLabel);
      VERIFY(dc.paintInfo().hdc == dc.hdc(), caseLabel);
   };
   runPaintDCTest(testRunnerWnd, testCase);
}


void testPaintDCDtor(HWND testRunnerWnd)
{
   auto testCase = [](HWND testWnd) {
      const std::string caseLabel{"PaintDC dtor"};
      {
         PaintDC dc{testWnd};
      }
      // Cannot verify.
   };
   runPaintDCTest(testRunnerWnd, testCase);
}


void testPaintDCMoveCtor(HWND testRunnerWnd)
{
   auto testCase = [](HWND testWnd) {
      const std::string caseLabel{"PaintDC move ctor"};
      PaintDC a{testWnd};
      PaintDC b{std::move(a)};
      VERIFY(b.hdc() != NULL, caseLabel);
      VERIFY(b.hwnd() == testWnd, caseLabel);
      VERIFY(b.paintInfo().hdc == b.hdc(), caseLabel);
      VERIFY(a.hdc() == NULL, caseLabel);
   };
   runPaintDCTest(testRunnerWnd, testCase);
}


void testPaintDCMoveAssignment(HWND testRunnerWnd)
{
   auto testCase = [](HWND testWnd) {
      const std::string caseLabel{"PaintDC move assignment"};
      PaintDC a{testWnd};
      PaintDC b;
      
      b = std::move(a);
      
      VERIFY(b.hdc() != NULL, caseLabel);
      VERIFY(b.hwnd() == testWnd, caseLabel);
      VERIFY(b.paintInfo().hdc == b.hdc(), caseLabel);
      VERIFY(a.hdc() == NULL, caseLabel);
   };
   runPaintDCTest(testRunnerWnd, testCase);
}


void testPaintDCHwnd(HWND testRunnerWnd)
{
   auto testCase = [](HWND testWnd) {
      const std::string caseLabel{"PaintDC::hwnd"};
      PaintDC dc{testWnd};
      VERIFY(dc.hwnd() == testWnd, caseLabel);
   };
   runPaintDCTest(testRunnerWnd, testCase);
}


void testPaintDCPaintInfo(HWND testRunnerWnd)
{
   auto testCase = [](HWND testWnd) {
      const std::string caseLabel{"PaintDC::paintInfo"};
      PaintDC dc{testWnd};
      VERIFY(dc.paintInfo().hdc == dc.hdc(), caseLabel);
   };
   runPaintDCTest(testRunnerWnd, testCase);
}


void testPaintDCSwap(HWND testRunnerWnd)
{
   auto testCase = [](HWND testWnd) {
      const std::string caseLabel{"PaintDC swap"};
      PaintDC a{testWnd};
      PaintDC b;
      
      swap(a, b);
      
      VERIFY(b.hdc() != NULL, caseLabel);
      VERIFY(b.hwnd() == testWnd, caseLabel);
      VERIFY(b.paintInfo().hdc == b.hdc(), caseLabel);
      VERIFY(a.hdc() == NULL, caseLabel);
      VERIFY(a.hwnd() == NULL, caseLabel);
      VERIFY(a.paintInfo().hdc == NULL, caseLabel);
   };
   runPaintDCTest(testRunnerWnd, testCase);
}


void testPaintDCClear(HWND testRunnerWnd)
{
   auto testCase = [](HWND testWnd) {
      const std::string caseLabel{"PaintDC::clear"};
      PaintDC dc{testWnd};
      // Keep a copy of the paint info for ending the paint operation manually.
      PAINTSTRUCT paintInfoCopy = dc.paintInfo();
      
      dc.clear();
      
      VERIFY(dc.hdc() == NULL, caseLabel);
      VERIFY(dc.hwnd() == NULL, caseLabel);
      VERIFY(dc.paintInfo().hdc == NULL, caseLabel);
      
      ::EndPaint(testWnd, &paintInfoCopy);
   };
   runPaintDCTest(testRunnerWnd, testCase);
}


void testPaintDCRelease(HWND testRunnerWnd)
{
   auto testCase = [](HWND testWnd) {
      const std::string caseLabel{"PaintDC::release"};
      PaintDC dc{testWnd};
      
      dc.release();
      
      VERIFY(dc.hdc() == NULL, caseLabel);
      VERIFY(dc.hwnd() == NULL, caseLabel);
      VERIFY(dc.paintInfo().hdc == NULL, caseLabel);
   };
   runPaintDCTest(testRunnerWnd, testCase);
}

} // namespace


void testDeviceContext(HWND testRunnerWnd)
{
   testDCDefaultCtor(testRunnerWnd);
   testDCCtorForHDC(testRunnerWnd);
   testDCDtor(testRunnerWnd);
   testDCCopyCtor(testRunnerWnd);
   testDCMoveCtor(testRunnerWnd);
   testDCCopyAssignment(testRunnerWnd);
   testDCMoveAssignment(testRunnerWnd);

   testDCHdc(testRunnerWnd);
   testDCConversionToBool(testRunnerWnd);
   testDCConversionToHDC(testRunnerWnd);
   testDCSwap(testRunnerWnd);
   testDCClear(testRunnerWnd);
   testDCSelectedObject(testRunnerWnd);
   testDCSelectObject(testRunnerWnd);

   testSharedDCDefaultCtor(testRunnerWnd);
   testSharedDCCtorForHDC(testRunnerWnd);
   testSharedDCDtor(testRunnerWnd);
   testSharedDCMoveCtor(testRunnerWnd);
   testSharedDCMoveAssignment(testRunnerWnd);
   testSharedDCHwnd(testRunnerWnd);
   testSharedDCSwap(testRunnerWnd);
   testSharedDCClear(testRunnerWnd);
   testSharedDCRelease(testRunnerWnd);

   testOwnedDCDefaultCtor(testRunnerWnd);
   testOwnedDCCtorForHDC(testRunnerWnd);
   testOwnedDCDtor(testRunnerWnd);
   testOwnedDCMoveCtor(testRunnerWnd);
   testOwnedDCMoveAssignment(testRunnerWnd);
   testOwnedDCSwap(testRunnerWnd);
   testOwnedDCRelease(testRunnerWnd);

   testPaintDCDefaultCtor(testRunnerWnd);
   testPaintDCCtorForHwnd(testRunnerWnd);
   testPaintDCDtor(testRunnerWnd);
   testPaintDCMoveCtor(testRunnerWnd);
   testPaintDCMoveAssignment(testRunnerWnd);
   testPaintDCHwnd(testRunnerWnd);
   testPaintDCPaintInfo(testRunnerWnd);
   testPaintDCSwap(testRunnerWnd);
   testPaintDCClear(testRunnerWnd);
   testPaintDCRelease(testRunnerWnd);
}
