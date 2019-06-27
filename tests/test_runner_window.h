//
// Win32 utilities library
// Window that executes all test cases.
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#include "window.h"


class TestRunnerWindow : public win32::Window
{
 protected:
   LRESULT handleMessage(HWND hwnd, UINT msgId, WPARAM wParam, LPARAM lParam) override;
   void onRunTests();
};


// Triggers test runner window to execute the tests.
constexpr UINT WM_RUN_TESTS = WM_USER + 1;
