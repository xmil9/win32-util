#include "timer_tests.h"
#include "message_util.h"
#include "timer.h"
#include "test_util.h"
#include "window.h"
#include <tchar.h>
#include <functional>

using namespace win32;


namespace
{

///////////////////

class TimerWindow : public Window
{
 public:
   explicit TimerWindow(bool& closeFlag, std::function<void(Window&)> onExpired);

 private:
   bool onClose() override;
   bool onTimer(UINT_PTR timerId, TIMERPROC callback) override;

 private:
   bool& m_closeFlag;
   std::function<void(Window&)> m_onExpired;
};


TimerWindow::TimerWindow(bool& closeFlag, std::function<void(Window&)> onExpired)
: m_closeFlag{closeFlag}, m_onExpired{onExpired}
{
}


bool TimerWindow::onTimer(UINT_PTR /*timerId*/, TIMERPROC /*callback*/)
{
   if (m_onExpired)
      m_onExpired(*this);
   return true;
}


bool TimerWindow::onClose()
{
   Window::onClose();
   m_closeFlag = true;
   return true;
}


///////////////////

void testTimerDefaultCtor(HWND /*testRunnerWnd*/)
{
   {
      const std::string caseLabel{"Timer default ctor"};
      Timer timer;
      VERIFY(timer.hwnd() == NULL, caseLabel);
      VERIFY(timer.id() == 0, caseLabel);
   }
}


void testTimerCtorForHwndAndId(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Timer ctor for hwnd and timer id"};
      constexpr UINT_PTR id = 1;
      Timer timer{testRunnerWnd, id};
      VERIFY(timer.hwnd() == testRunnerWnd, caseLabel);
      VERIFY(timer.id() == id, caseLabel);
   }
}


void testTimerDtor(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Timer dtor"};

      bool stopMsgLoop = false;
      TimerWindow wnd{stopMsgLoop, {}};
      wnd.create(testRunnerWnd, {100, 100, 400, 300}, _T("Test window"),
                 WS_OVERLAPPEDWINDOW);

      constexpr UINT_PTR id = 1;
      {
         Timer timer{wnd, id};
         timer.start(100);
      }

      // Kill timer again. Should fail.
      const bool res = ::KillTimer(wnd, id);
      VERIFY(!res, caseLabel);
      wnd.closeImmediately();
   }
}


void testTimerMoveCtor(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Timer move ctor"};

      constexpr UINT_PTR id = 1;
      Timer a{testRunnerWnd, id};
      Timer b{std::move(a)};
   
      VERIFY(b.hwnd() == testRunnerWnd, caseLabel);
      VERIFY(b.id() == id, caseLabel);
      VERIFY(a.hwnd() == NULL, caseLabel);
      VERIFY(a.id() == 0, caseLabel);
   }
}


void testTimerMoveAssignment(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Timer move assignment"};

      constexpr UINT_PTR id = 1;
      Timer a{testRunnerWnd, id};
      Timer b;
      b = std::move(a);
   
      VERIFY(b.hwnd() == testRunnerWnd, caseLabel);
      VERIFY(b.id() == id, caseLabel);
      VERIFY(a.hwnd() == NULL, caseLabel);
      VERIFY(a.id() == 0, caseLabel);
   }
}


void testTimerHwnd(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Timer::hwnd"};
      constexpr UINT_PTR id = 1;
      Timer timer{testRunnerWnd, id};
      VERIFY(timer.hwnd() == testRunnerWnd, caseLabel);
   }
}


void testTimerId(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Timer::id"};
      constexpr UINT_PTR id = 1;
      Timer timer{testRunnerWnd, id};
      VERIFY(timer.id() == id, caseLabel);
   }
}


void testTimerConversionToBool(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Timer conversion to bool for populated timer"};
      constexpr UINT_PTR id = 1;
      Timer timer{testRunnerWnd, id};
      VERIFY(timer.operator bool(), caseLabel);
   }
   {
      const std::string caseLabel{"Timer conversion to bool for default timer"};
      Timer timer;
      VERIFY(!timer.operator bool(), caseLabel);
   }
}


void testTimerSwap(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Timer swap"};

      constexpr UINT_PTR id = 1;
      Timer a{testRunnerWnd, id};
      Timer b;
      swap(a, b);
   
      VERIFY(b.hwnd() == testRunnerWnd, caseLabel);
      VERIFY(b.id() == id, caseLabel);
      VERIFY(a.hwnd() == NULL, caseLabel);
      VERIFY(a.id() == 0, caseLabel);
   }
}


void testTimerStart(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Timer::start for populated timer"};

      bool stopMsgLoop = false;
      bool callbackReceived = false;
      Timer timer;
      auto onExpired = [&timer, &callbackReceived](Window& wnd) {
         callbackReceived = true;
         timer.stop();
         wnd.close();
      };
      TimerWindow wnd{stopMsgLoop, onExpired};
      wnd.create(testRunnerWnd, {100, 100, 400, 300}, _T("Test window"),
                 WS_OVERLAPPEDWINDOW);

      constexpr UINT_PTR id = 1;
      timer = std::move(Timer{wnd, id});
      const bool startResult = timer.start(100);
      modalMessageLoop(wnd, stopMsgLoop, testRunnerWnd);

      VERIFY(startResult, caseLabel);
      VERIFY(callbackReceived, caseLabel);
   }
   {
      const std::string caseLabel{"Timer::start for default timer"};
      Timer timer;
      const bool startResult = timer.start(100);
      VERIFY(!startResult, caseLabel);
   }
}


void testTimerStop(HWND testRunnerWnd)
{
   {
      const std::string caseLabel{"Timer::stop for started timer"};

      bool stopMsgLoop = false;
      bool timerStopResult = false;
      Timer timer;
      auto onExpired = [&timer, &timerStopResult](Window& wnd) {
         timerStopResult = timer.stop();
         wnd.close();
      };
      TimerWindow wnd{stopMsgLoop, onExpired};
      wnd.create(testRunnerWnd, {100, 100, 400, 300}, _T("Test window"),
                 WS_OVERLAPPEDWINDOW);

      constexpr UINT_PTR id = 1;
      timer = std::move(Timer{wnd, id});
      timer.start(100);
      modalMessageLoop(wnd, stopMsgLoop, testRunnerWnd);

      VERIFY(timerStopResult, caseLabel);
   }
   {
      const std::string caseLabel{"Timer::stop for not started timer"};

      bool stopMsgLoop = false;
      TimerWindow wnd{stopMsgLoop, {}};
      wnd.create(testRunnerWnd, {100, 100, 400, 300}, _T("Test window"),
                 WS_OVERLAPPEDWINDOW);

      constexpr UINT_PTR id = 1;
      Timer timer{wnd, id};
      const bool res = timer.stop();

      VERIFY(!res, caseLabel);
      wnd.closeImmediately();
   }
}

} // namespace


void testTimer(HWND testRunnerWnd)
{
   testTimerDefaultCtor(testRunnerWnd);
   testTimerCtorForHwndAndId(testRunnerWnd);
   testTimerDtor(testRunnerWnd);
   testTimerMoveCtor(testRunnerWnd);
   testTimerMoveAssignment(testRunnerWnd);
   testTimerHwnd(testRunnerWnd);
   testTimerId(testRunnerWnd);
   testTimerConversionToBool(testRunnerWnd);
   testTimerSwap(testRunnerWnd);
   testTimerStart(testRunnerWnd);
   testTimerStop(testRunnerWnd);
}
