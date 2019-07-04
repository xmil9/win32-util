//
// Win32 utilities library
// Tests for messaging utilities.
//
// Jun-2019, Michael Lindner
// MIT license
//
#include "message_util_tests.h"
#include "message_util.h"
#include "test_util.h"
#include "timer.h"
#include "window.h"
#include <array>
#include <functional>

using namespace win32;


namespace
{

///////////////////

constexpr UINT WM_MSG_1 = WM_USER + 1;
constexpr UINT WM_MSG_2 = WM_USER + 2;
constexpr UINT WM_MSG_3 = WM_USER + 3;


// Window that sends itself a sequence of messages and then closes itself.
class ModalTestWindow : public Window
{
 public:
   explicit ModalTestWindow(bool& closedFlag);

   void setVerificationCallback(std::function<void()> verifyFn);
   bool wasMessageReceived(UINT id) const;

 protected:
   LRESULT handleMessage(HWND hwnd, UINT msgId, WPARAM wParam, LPARAM lParam) override;

 private:
   std::size_t MessageIndex(UINT id) const;

   bool onClose() override;
   void onMsg(UINT msgId);

 private:
   bool& m_closedFlag;
   std::function<void()> m_verifyFn;
   std::array<bool, 3> m_msgReceived;
};


ModalTestWindow::ModalTestWindow(bool& closedFlag) : m_closedFlag{closedFlag}
{
   m_msgReceived.fill(false);
}


void ModalTestWindow::setVerificationCallback(std::function<void()> verifyFn)
{
   m_verifyFn = verifyFn;
}


bool ModalTestWindow::wasMessageReceived(UINT id) const
{
   return m_msgReceived[MessageIndex(id)];
}


LRESULT ModalTestWindow::handleMessage(HWND hwnd, UINT msgId, WPARAM wParam,
                                       LPARAM lParam)
{
   switch (msgId)
   {
   case WM_MSG_1:
   case WM_MSG_2:
   case WM_MSG_3:
      onMsg(msgId);
      return 0;
   }

   return win32::Window::handleMessage(hwnd, msgId, wParam, lParam);
}


std::size_t ModalTestWindow::MessageIndex(UINT id) const
{
   return id - WM_MSG_1;
}


bool ModalTestWindow::onClose()
{
   Window::onClose();
   m_closedFlag = true;
   return true;
}


void ModalTestWindow::onMsg(UINT msgId)
{
   m_msgReceived[MessageIndex(msgId)] = true;

   if (m_verifyFn)
      m_verifyFn();

   if (msgId == WM_MSG_3)
      close();
   else
      postMessage(msgId + 1);
}


///////////////////

void testMainMessageLoop(HWND testRunnerWnd)
{
   // Cannot test here but it is used as the main message loop of this
   // test app.
}


void testModalMessageLoop(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"modalMessageLoop message dispatching"};

      bool stopMsgLoopFlag = false;
      ModalTestWindow wnd{stopMsgLoopFlag};
      wnd.create(testRunnerWnd, {150, 150, 500, 250}, L"Modal dispatching test",
                 WS_OVERLAPPEDWINDOW);
      wnd.setVisible(true);
      wnd.postMessage(WM_MSG_1);

      modalMessageLoop(wnd, stopMsgLoopFlag);

      VERIFY(wnd.wasMessageReceived(WM_MSG_1), caseLabel);
      VERIFY(wnd.wasMessageReceived(WM_MSG_2), caseLabel);
      VERIFY(wnd.wasMessageReceived(WM_MSG_3), caseLabel);
   }
   {
      const std::string caseLabel{"modalMessageLoop disabling parent window during modal"
                                  " loop and reenabling it afterwards"};

      bool stopMsgLoopFlag = false;
      ModalTestWindow wnd{stopMsgLoopFlag};
      wnd.setVerificationCallback([testRunnerWnd, &caseLabel]() {
         VERIFY(!::IsWindowEnabled(testRunnerWnd), caseLabel);
      });
      wnd.create(testRunnerWnd, {150, 150, 500, 250}, L"Disabling modal parent test",
                 WS_OVERLAPPEDWINDOW);
      wnd.setVisible(true);
      wnd.postMessage(WM_MSG_1);

      modalMessageLoop(wnd, stopMsgLoopFlag, testRunnerWnd);

      VERIFY(::IsWindowEnabled(testRunnerWnd), caseLabel);
   }
   {
      const std::string caseLabel{"modalMessageLoop for null window"};

      bool stopMsgLoopFlag = false;
      std::size_t callCount = 0;
      TimedCallback timedCb{[&callCount, &stopMsgLoopFlag](DWORD sysTime) {
         if (++callCount == 10)
            stopMsgLoopFlag = true;
      }};
      timedCb.start(20);

      modalMessageLoop(NULL, stopMsgLoopFlag);

      VERIFY(callCount == 10, caseLabel);
   }
}

} // namespace


void testMessageUtil(HWND testRunnerWnd)
{
   testMainMessageLoop(testRunnerWnd);
   testModalMessageLoop(testRunnerWnd);
}
