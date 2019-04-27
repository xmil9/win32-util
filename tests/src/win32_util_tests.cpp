#include "message_util.h"
#include "resources/resource.h"
#include "test_runner_window.h"
#include <windows.h>

using namespace win32;


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE /*hPrevInstance*/,
                      _In_ LPWSTR /*lpCmdLine*/, _In_ int /*nCmdShow*/)
{
   TestRunnerWindow runnerWnd;
   runnerWnd.create(NULL, {100, 100, 500, 300}, L"Test runner", WS_OVERLAPPEDWINDOW);
   runnerWnd.setVisible(true);
   runnerWnd.postMessage(WM_RUN_TESTS);

   HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32UTILTESTS));
   const MsgLoopResult res = mainMessageLoop(hAccelTable);
   return res.exitCode;
}
