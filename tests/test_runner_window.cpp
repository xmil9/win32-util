//
// Win32 utilities library
// Window that executes all test cases.
//
// Jun-2019, Michael Lindner
// MIT license
//
#include "test_runner_window.h"
#include "device_context_tests.h"
#include "err_util_tests.h"
#include "gdi_object_tests.h"
#include "geometry_tests.h"
#include "mem_util_tests.h"
#include "message_util_tests.h"
#include "registry_tests.h"
#include "timer_tests.h"
#include "tstring_tests.h"
#include "window_tests.h"



LRESULT TestRunnerWindow::handleMessage(HWND hwnd, UINT msgId, WPARAM wParam, LPARAM lParam)
{
   switch (msgId)
   {
   case WM_RUN_TESTS:
      onRunTests();
      return 0;
   }

   return win32::Window::handleMessage(hwnd, msgId, wParam, lParam);
}


void TestRunnerWindow::onRunTests()
{
   HWND runnerWnd = hwnd();
   testDeviceContext(runnerWnd);
   testErrUtil();
   testGdiObject(runnerWnd);
   testGeometry(runnerWnd);
   testMemUtil();
   testMessageUtil(runnerWnd);
   testRegistry();
   testTString(runnerWnd);
   testTimer(runnerWnd);
   testWindow(runnerWnd);

   PostQuitMessage(EXIT_SUCCESS);
}
