#include "window_tests.h"
#include "message_util.h"
#include "window.h"
#include "test_util.h"
#include <tchar.h>
#include <functional>

using namespace win32;


namespace
{

///////////////////

// Window that calls a given function when receiving a WM_TIMER event.
class DelayedTestWindow : public Window
{
 public:
   using Callback_t = std::function<void(UINT_PTR timerId, Window& wnd)>;
   explicit DelayedTestWindow(bool& closeFlag, Callback_t callback);

 private:
   bool onClose() override;
   bool onTimer(UINT_PTR timerId, TIMERPROC callback) override;

 private:
   bool& m_closeFlag;
   Callback_t m_callback;
};


DelayedTestWindow::DelayedTestWindow(bool& closeFlag, Callback_t callback)
: m_closeFlag{closeFlag}, m_callback{callback}
{
}


bool DelayedTestWindow::onClose()
{
   Window::onClose();
   m_closeFlag = true;
   return true;
}


bool DelayedTestWindow::onTimer(UINT_PTR timerId, TIMERPROC /*callback*/)
{
   m_callback(timerId, *this);
   return true;
}


///////////////////

constexpr UINT WM_POSTED_MSG = WM_USER + 1;
constexpr UINT WM_SENT_MSG = WM_USER + 2;

// Window that checks reception of test messages.
class MessageTestWindow : public Window
{
 public:
   explicit MessageTestWindow(bool& closeFlag);

   bool wasPostedMessageReceived() const { return m_postedMsgReceived; }
   bool wasSentMessageReceived() const { return m_sentMsgReceived; }

 private:
   LRESULT handleMessage(HWND hwnd, UINT msgId, WPARAM wParam, LPARAM lParam) override;
   bool onClose() override;

 private:
   bool& m_closeFlag;
   bool m_postedMsgReceived = false;
   bool m_sentMsgReceived = false;
};


MessageTestWindow::MessageTestWindow(bool& closeFlag) : m_closeFlag{closeFlag}
{
}


LRESULT MessageTestWindow::handleMessage(HWND hwnd, UINT msgId, WPARAM wParam,
                                         LPARAM lParam)
{
   switch (msgId)
   {
   case WM_POSTED_MSG:
      m_postedMsgReceived = true;
      return 0;
   case WM_SENT_MSG:
      m_sentMsgReceived = true;
      return 0;
   }

   return Window::handleMessage(hwnd, msgId, wParam, lParam);
}


bool MessageTestWindow::onClose()
{
   Window::onClose();
   m_closeFlag = true;
   return true;
}


///////////////////

void testWindowDefaultCtor(HWND /*testRunnerWnd*/)
{
   {
      const std::string caseLabel{"Window default ctor"};
      Window wnd;
      VERIFY(wnd.hwnd() == NULL, caseLabel);
   }
}


void testWindowCtorForHwnd(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Window ctor for HWND"};
      Window wnd{testRunnerWnd};
      VERIFY(wnd.hwnd() == testRunnerWnd, caseLabel);
   }
}


void testWindowDtor(HWND /*testRunnerWnd*/)
{
   // Dtor does nothing.
}


void testWindowMoveCtor(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Window move ctor"};
      Window a{testRunnerWnd};
      Window b{std::move(a)};
      VERIFY(b.hwnd() == testRunnerWnd, caseLabel);
      VERIFY(a.hwnd() == testRunnerWnd, caseLabel);
   }
}


void testWindowMoveAssignment(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Window move assignment"};
      Window a{testRunnerWnd};
      Window b;
      b = std::move(a);
      VERIFY(b.hwnd() == testRunnerWnd, caseLabel);
      VERIFY(a.hwnd() == testRunnerWnd, caseLabel);
   }
}


void testWindowConversionToBool(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Window conversion to bool for populated instance"};
      Window wnd{testRunnerWnd};
      VERIFY(wnd.operator bool(), caseLabel);
   }
   {
      const std::string caseLabel{"Window conversion to bool for empty instance"};
      Window wnd;
      VERIFY(!wnd.operator bool(), caseLabel);
   }
}


void testWindowConversionToHwnd(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Window conversion to HWND"};
      Window wnd{testRunnerWnd};
      HWND converted = wnd;
      VERIFY(converted == wnd.hwnd(), caseLabel);
   }
}


void testWindowSwap(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Window swap"};
      Window a{testRunnerWnd};
      Window b;
      swap(a, b);
      VERIFY(a.hwnd() == NULL, caseLabel);
      VERIFY(b.hwnd() == testRunnerWnd, caseLabel);
   }
}


void testWindowHwnd(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Window::hwnd"};
      Window wnd{testRunnerWnd};
      VERIFY(wnd.hwnd() == testRunnerWnd, caseLabel);
   }
}


void testWindowCreate(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Window::create creates a window"};
      Window wnd;
      wnd.create(testRunnerWnd, {100, 100, 400, 300}, _T("Test window"),
                 WS_OVERLAPPEDWINDOW, 0);
      VERIFY(wnd.hwnd() != NULL, caseLabel);
      wnd.closeImmediately();
   }
   {
      const std::string caseLabel{"Window::create sets parent"};
      Window wnd;
      wnd.create(testRunnerWnd, {100, 100, 400, 300}, _T("Test window"),
                 WS_OVERLAPPEDWINDOW, 0);
      VERIFY(wnd.parent() != testRunnerWnd, caseLabel);
      wnd.closeImmediately();
   }
   {
      const std::string caseLabel{"Window::create sets dimensions"};
      const Rect bounds{100, 100, 400, 300};
      Window wnd;
      wnd.create(testRunnerWnd, bounds, _T("Test window"), WS_OVERLAPPEDWINDOW, 0);
      VERIFY(wnd.windowBounds() == bounds, caseLabel);
      wnd.closeImmediately();
   }
   {
      const std::string caseLabel{"Window::create sets title"};
      const Rect bounds{100, 100, 400, 300};
      Window wnd;
      wnd.create(testRunnerWnd, bounds, _T("Test window"), WS_OVERLAPPEDWINDOW, 0);
      VERIFY(wnd.title() == _T("Test window"), caseLabel);
      wnd.closeImmediately();
   }
}


void testWindowClose(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Window::close"};

      bool stopMsgLoopFlag = false;
      constexpr UINT DelayMs = 100;
      constexpr UINT_PTR TimerId = 1;
      auto closeCallback = [TimerId](UINT_PTR timerId, Window& wnd) {
         if (timerId == TimerId)
         {
            wnd.close();
            ::KillTimer(wnd, timerId);
         }
      };

      DelayedTestWindow wnd{stopMsgLoopFlag, closeCallback};
      wnd.create(testRunnerWnd, {10, 10, 200, 200}, L"Delayed test", WS_OVERLAPPEDWINDOW);
      const HWND rememberedHwnd = wnd;
      wnd.setVisible(true);

      ::SetTimer(wnd, TimerId, DelayMs, NULL);
      modalMessageLoop(wnd, stopMsgLoopFlag);

      VERIFY(wnd.hwnd() == NULL, caseLabel);
      VERIFY(!::IsWindow(rememberedHwnd), caseLabel);
   }
}


void testWindowCloseImmediately(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Window::closeImmediately"};
      Window wnd;
      wnd.create(testRunnerWnd, {100, 100, 400, 300}, _T("Test window"),
                 WS_OVERLAPPEDWINDOW, 0);
      const HWND rememberedHwnd = wnd;

      wnd.closeImmediately();

      VERIFY(wnd.hwnd() == NULL, caseLabel);
      VERIFY(!::IsWindow(rememberedHwnd), caseLabel);
   }
}


void testWindowExists(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Window::exists for existing window"};
      Window wnd;
      wnd.create(testRunnerWnd, {100, 100, 400, 300}, _T("Test window"),
                 WS_OVERLAPPEDWINDOW, 0);
      VERIFY(wnd.exists(), caseLabel);
      wnd.closeImmediately();
   }
   {
      const std::string caseLabel{"Window::exists for closed window"};
      Window wnd;
      wnd.create(testRunnerWnd, {100, 100, 400, 300}, _T("Test window"),
                 WS_OVERLAPPEDWINDOW, 0);
      wnd.closeImmediately();

      VERIFY(!wnd.exists(), caseLabel);
   }
   {
      const std::string caseLabel{"Window::exists for window that hasn't been created"};
      Window wnd;
      VERIFY(!wnd.exists(), caseLabel);
   }
}


void testWindowIsVisible(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Window::isVisible for visible window"};
      Window wnd;
      wnd.create(testRunnerWnd, {100, 100, 400, 300}, _T("Test window"),
                 WS_OVERLAPPEDWINDOW, 0);
      ::ShowWindow(wnd, SW_SHOW);
      VERIFY(wnd.isVisible(), caseLabel);
      wnd.closeImmediately();
   }
   {
      const std::string caseLabel{"Window::isVisible for hidden window"};
      Window wnd;
      wnd.create(testRunnerWnd, {100, 100, 400, 300}, _T("Test window"),
                 WS_OVERLAPPEDWINDOW, 0);
      ::ShowWindow(wnd, SW_HIDE);
      VERIFY(!wnd.isVisible(), caseLabel);
      wnd.closeImmediately();
   }
}


void testWindowSetVisible(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Window::setVisible"};
      Window wnd;
      wnd.create(testRunnerWnd, {100, 100, 400, 300}, _T("Test window"),
                 WS_OVERLAPPEDWINDOW, 0);
      wnd.setVisible(true);
      VERIFY(wnd.isVisible(), caseLabel);
      wnd.setVisible(false);
      VERIFY(!wnd.isVisible(), caseLabel);
      wnd.closeImmediately();
   }
}


void testWindowClientBounds(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Window::clientBounds"};
      Window wnd;
      wnd.create(testRunnerWnd, {100, 100, 400, 300}, _T("Test window"),
                 WS_OVERLAPPEDWINDOW, 0);
      RECT expectedBounds;
      ::GetClientRect(wnd, &expectedBounds);
      VERIFY(wnd.clientBounds() == expectedBounds, caseLabel);
      wnd.closeImmediately();
   }
}


void testWindowWindowBounds(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Window::windowBounds"};
      Window wnd;
      wnd.create(testRunnerWnd, {100, 100, 400, 300}, _T("Test window"),
                 WS_OVERLAPPEDWINDOW, 0);
      RECT expectedBounds;
      ::GetWindowRect(wnd, &expectedBounds);
      VERIFY(wnd.windowBounds() == expectedBounds, caseLabel);
      wnd.closeImmediately();
   }
}


void testWindowClientDC(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Window::clientDC"};
      Window wnd;
      wnd.create(testRunnerWnd, {100, 100, 400, 300}, _T("Test window"),
                 WS_OVERLAPPEDWINDOW, 0);
      SharedDC dc = wnd.clientDC();
      VERIFY(dc.hdc() != NULL, caseLabel);
      wnd.closeImmediately();
   }
}


void testWindowWindowDC(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Window::windowDC"};
      Window wnd;
      wnd.create(testRunnerWnd, {100, 100, 400, 300}, _T("Test window"),
                 WS_OVERLAPPEDWINDOW, 0);
      SharedDC dc = wnd.windowDC();
      VERIFY(dc.hdc() != NULL, caseLabel);
      wnd.closeImmediately();
   }
}


void testWindowParent(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Window::parent for overlapped window"};
      Window wnd;
      wnd.create(testRunnerWnd, {100, 100, 400, 300}, _T("Test window"),
                 WS_OVERLAPPEDWINDOW, 0);
      // Will be null regardless of passed parent.
      VERIFY(wnd.parent() == NULL, caseLabel);
      wnd.closeImmediately();
   }
   {
      const std::string caseLabel{"Window::parent for child window"};
      Window wnd;
      wnd.create(testRunnerWnd, {100, 100, 400, 300}, _T("Test window"), WS_CHILD, 0);
      VERIFY(wnd.parent() == testRunnerWnd, caseLabel);
      wnd.closeImmediately();
   }
   {
      const std::string caseLabel{"Window::parent for popup window"};
      Window wnd;
      wnd.create(testRunnerWnd, {100, 100, 400, 300}, _T("Test window"), WS_POPUPWINDOW,
                 0);
      VERIFY(wnd.parent() == testRunnerWnd, caseLabel);
      wnd.closeImmediately();
   }
}


void testWindowOwner(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Window::owner for overlapped window"};
      Window wnd;
      wnd.create(testRunnerWnd, {100, 100, 400, 300}, _T("Test window"),
                 WS_OVERLAPPEDWINDOW, 0);
      VERIFY(wnd.owner() == testRunnerWnd, caseLabel);
      wnd.closeImmediately();
   }
   {
      const std::string caseLabel{"Window::owner for child window"};
      Window wnd;
      wnd.create(testRunnerWnd, {100, 100, 400, 300}, _T("Test window"), WS_CHILD, 0);
      // Child windows don't have owners.
      VERIFY(wnd.owner() == NULL, caseLabel);
      wnd.closeImmediately();
   }
   {
      const std::string caseLabel{"Window::owner for popup window"};
      Window wnd;
      wnd.create(testRunnerWnd, {100, 100, 400, 300}, _T("Test window"), WS_POPUPWINDOW,
                 0);
      VERIFY(wnd.owner() == testRunnerWnd, caseLabel);
      wnd.closeImmediately();
   }
}


void testWindowTitle(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Window::title for overlapped window"};
      const TString title = _T("Test window");
      Window wnd;
      wnd.create(testRunnerWnd, {100, 100, 400, 300}, title, WS_OVERLAPPEDWINDOW, 0);
      VERIFY(wnd.title() == title, caseLabel);
      wnd.closeImmediately();
   }
}


void testWindowSetTitle(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Window::setTitle for overlapped window"};
      Window wnd;
      wnd.create(testRunnerWnd, {100, 100, 400, 300}, _T("Original title"),
                 WS_OVERLAPPEDWINDOW, 0);
      const TString title = _T("New title");
      wnd.setTitle(title);
      VERIFY(wnd.title() == title, caseLabel);
      wnd.closeImmediately();
   }
}


void testWindowPostMessage(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Window::postMessage"};

      bool stopMsgLoop = false;
      MessageTestWindow wnd{stopMsgLoop};
      wnd.create(testRunnerWnd, {100, 100, 400, 300}, _T("Test window"),
                 WS_OVERLAPPEDWINDOW, 0);
      wnd.postMessage(WM_POSTED_MSG);
      wnd.close();
      modalMessageLoop(wnd, stopMsgLoop, testRunnerWnd);
      VERIFY(wnd.wasPostedMessageReceived(), caseLabel);
   }
}


void testWindowSendMessage(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Window::sendMessage"};

      bool stopMsgLoop = false;
      MessageTestWindow wnd{stopMsgLoop};
      wnd.create(testRunnerWnd, {100, 100, 400, 300}, _T("Test window"),
                 WS_OVERLAPPEDWINDOW, 0);
      wnd.sendMessage(WM_SENT_MSG);
      VERIFY(wnd.wasSentMessageReceived(), caseLabel);
      wnd.closeImmediately();
   }
}

} // namespace


void testWindow(HWND testRunnerWnd)
{
   testWindowDefaultCtor(testRunnerWnd);
   testWindowCtorForHwnd(testRunnerWnd);
   testWindowDtor(testRunnerWnd);
   testWindowMoveCtor(testRunnerWnd);
   testWindowMoveAssignment(testRunnerWnd);
   testWindowConversionToBool(testRunnerWnd);
   testWindowConversionToHwnd(testRunnerWnd);
   testWindowSwap(testRunnerWnd);
   testWindowHwnd(testRunnerWnd);
   testWindowCreate(testRunnerWnd);
   testWindowClose(testRunnerWnd);
   testWindowCloseImmediately(testRunnerWnd);
   testWindowExists(testRunnerWnd);
   testWindowIsVisible(testRunnerWnd);
   testWindowSetVisible(testRunnerWnd);
   testWindowClientBounds(testRunnerWnd);
   testWindowWindowBounds(testRunnerWnd);
   testWindowClientDC(testRunnerWnd);
   testWindowWindowDC(testRunnerWnd);
   testWindowParent(testRunnerWnd);
   testWindowOwner(testRunnerWnd);
   testWindowTitle(testRunnerWnd);
   testWindowSetTitle(testRunnerWnd);
   testWindowPostMessage(testRunnerWnd);
   testWindowSendMessage(testRunnerWnd);
}
