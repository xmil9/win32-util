#ifdef _WIN32
#include "window.h"
#include <tchar.h>
#include <windowsx.h>
#include <array>
#include <cassert>
#include <cstring>
#include <utility>


namespace
{

using namespace win32;

///////////////////

const TCHAR* wndSelfProperty = _T("SelfProperty");


void setSelf(HWND hwnd, Window* self)
{
   ::SetProp(hwnd, wndSelfProperty, self);
}


Window* getSelf(HWND hwnd)
{
   return reinterpret_cast<Window*>(::GetProp(hwnd, wndSelfProperty));
}


void removeSelf(HWND hwnd)
{
   ::RemoveProp(hwnd, wndSelfProperty);
}


Window* initSelf(HWND hwnd, UINT msgId, LPARAM lParam)
{
   // Check for first message sent to window during creation and set up the self pointer.
   // Note: The very first message is actually a WM_GETMINMAXINFO message. Since for that
   //       message the Window object pointer is not available yet, this message cannot be
   //       handled easily with the Window class!
   if (msgId == WM_NCCREATE)
   {
      const CREATESTRUCT* createInfo = reinterpret_cast<CREATESTRUCT*>(lParam);
      Window* self = reinterpret_cast<Window*>(createInfo->lpCreateParams);
      setSelf(hwnd, self);
      return self;
   }
   // For any other message the self pointer should be set up already.
   return getSelf(hwnd);
}


///////////////////

BYTE decodeScanCode(LPARAM lParam)
{
   return static_cast<BYTE>(HIWORD(lParam));
}


bool decodeExtendedKeyFlag(LPARAM lParam)
{
   constexpr WORD isExtendedKeyMask = 0x0100;
   return HIWORD(lParam) & isExtendedKeyMask;
}


bool decodePreviousKeyState(LPARAM lParam)
{
   constexpr WORD wasPreviouslyDownMask = 0x4000;
   return HIWORD(lParam) & wasPreviouslyDownMask;
}


Point decodeMousePosition(LPARAM lParam)
{
   return {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
}

} // namespace


///////////////////

namespace win32
{

bool Window::create(HWND parent, const Rect& bounds, const String& title,
                    unsigned long style, unsigned long extStyle)
{
   registerWindowClass();

   const HWND hwnd = ::CreateWindowEx(extStyle, windowClassName(), title.c_str(), style,
                                      bounds.left, bounds.top, bounds.width(),
                                      bounds.height(), parent, NULL, appInstance(), this);
   assert(hwnd && hwnd == m_hwnd);

   setTitle(title);
   return (hwnd != NULL);
}


void Window::close()
{
   postMessage(WM_CLOSE);
}


void Window::closeImmediately()
{
   sendMessage(WM_CLOSE);
}


bool Window::exists() const
{
   if (hwnd())
      return ::IsWindow(hwnd());
   return false;
}


bool Window::isVisible() const
{
   if (hwnd())
      return ::IsWindowVisible(hwnd());
   return false;
}


void Window::setVisible(bool visible)
{
   if (hwnd())
      ::ShowWindow(hwnd(), visible ? SW_SHOW : SW_HIDE);
}


Rect Window::clientBounds() const
{
   if (!hwnd())
      return {};

   Rect bounds;
   ::GetClientRect(hwnd(), &bounds);
   return bounds;
}


Rect Window::windowBounds() const
{
   if (!hwnd())
      return {};

   Rect bounds;
   ::GetWindowRect(hwnd(), &bounds);
   return bounds;
}


SharedDC Window::clientDC() const
{
   if (hwnd())
      return SharedDC{::GetDC(hwnd()), hwnd()};
   return {};
}


SharedDC Window::windowDC() const
{
   if (hwnd())
      return SharedDC{::GetWindowDC(hwnd()), hwnd()};
   return {};
}


HWND Window::parent() const
{
   if (hwnd())
      return ::GetParent(hwnd());
   return NULL;
}


HWND Window::owner() const
{
   if (hwnd())
      return ::GetWindow(hwnd(), GW_OWNER);
   return NULL;
}


String Window::title() const
{
   if (!hwnd())
      return {};

   std::array<TCHAR, 512> buffer;
   ::GetWindowText(hwnd(), buffer.data(), static_cast<int>(buffer.size()));
   return buffer.data();
}


void Window::setTitle(const String& title)
{
   if (hwnd())
      ::SetWindowText(hwnd(), title.c_str());
}


bool Window::postMessage(UINT msgId, WPARAM wParam, LPARAM lParam) const
{
   if (hwnd())
      return ::PostMessage(hwnd(), msgId, wParam, lParam);
   return false;
}


LRESULT Window::sendMessage(UINT msgId, WPARAM wParam, LPARAM lParam) const
{
   if (hwnd())
      return ::SendMessage(hwnd(), msgId, wParam, lParam);
   return 0;
}


HINSTANCE Window::appInstance() const
{
   return ::GetModuleHandle(NULL);
}


const TCHAR* Window::windowClassName() const
{
   return _T("Win32WindowClass");
}


bool Window::isWindowClassRegistered() const
{
   WNDCLASSEX classInfo;
   return ::GetClassInfoEx(appInstance(), windowClassName(), &classInfo);
}


bool Window::registerWindowClass() const
{
   if (isWindowClassRegistered())
      return true;

   const HINSTANCE appInst = appInstance();

   WNDCLASSEX classInfo;
   std::memset(&classInfo, 0, sizeof(WNDCLASSEX));
   classInfo.cbSize = sizeof(WNDCLASSEX);
   classInfo.lpszClassName = windowClassName();
   classInfo.lpfnWndProc = windowProc;
   classInfo.hInstance = appInst;
   classInfo.style = CS_HREDRAW | CS_VREDRAW;
   classInfo.cbClsExtra = 0;
   classInfo.cbWndExtra = 0;
   classInfo.hIcon = NULL;
   classInfo.hIconSm = 0;
   classInfo.hCursor = ::LoadCursor(appInst, IDC_ARROW);
   // Either a HBRUSH for the background, a std system color value + 1, or
   // NULL.
   // The '+ 1' makes sure COLOR_SCROLLBAR and NULL can be distinguished!
   // The system deletes the background brush when the class is unregistered.
   classInfo.hbrBackground = ::CreateSolidBrush(RGB(255, 255, 255));
   classInfo.lpszMenuName = 0;

   const ATOM token = ::RegisterClassEx(&classInfo);
   assert(token != 0);
   return (token != 0);
}


LRESULT Window::windowProc(HWND hwnd, UINT msgId, WPARAM wParam, LPARAM lParam)
{
   Window* self = initSelf(hwnd, msgId, lParam);
   // The very first message is actually a WM_GETMINMAXINFO message for which the
   // self pointer is not available yet.
   assert(self || msgId == WM_GETMINMAXINFO);
   if (self)
      return self->handleMessage(hwnd, msgId, wParam, lParam);
   return ::DefWindowProc(hwnd, msgId, wParam, lParam);
}


LRESULT Window::handleMessage(HWND hwnd, UINT msgId, WPARAM wParam, LPARAM lParam)
{
   switch (msgId)
   {
   case WM_NCCREATE:
   {
      setupWindow(hwnd);
      const CreationResult res =
         onNcCreate(hwnd, reinterpret_cast<CREATESTRUCT*>(lParam));
      if (res == CreationResult::Handled)
         return TRUE;
      else if (res == CreationResult::Abort)
         return FALSE;
      // Call default proc below.
      break;
   }
   case WM_NCDESTROY:
   {
      const bool res = onNcDestroy();
      cleanupWindow();
      if (res)
         return 0;
      break;
   }
   case WM_CREATE:
   {
      const CreationResult res = onCreate(reinterpret_cast<CREATESTRUCT*>(lParam));
      if (res == CreationResult::Handled)
         return 0;
      else if (res == CreationResult::Abort)
         return -1;
      // Call default proc below.
      break;
   }
   case WM_DESTROY:
   {
      if (onDestroy())
         return 0;
      break;
   }
   case WM_CLOSE:
   {
      if (onClose())
         return 0;
      break;
   }
   case WM_NCPAINT:
   {
      if (onNcPaint(reinterpret_cast<HRGN>(wParam)))
         return 0;
      break;
   }
   case WM_PAINT:
   {
      if (onPaint())
         return 0;
      break;
   }
   case WM_KEYDOWN:
   {
      if (onKeyDown(static_cast<UINT>(wParam), LOWORD(lParam), decodeScanCode(lParam),
                    decodeExtendedKeyFlag(lParam), decodePreviousKeyState(lParam)))
         return 0;
      break;
   }
   case WM_HSCROLL:
   {
      if (onHScroll(LOWORD(wParam), HIWORD(wParam), reinterpret_cast<HWND>(lParam)))
         return 0;
      break;
   }
   case WM_VSCROLL:
   {
      if (onVScroll(LOWORD(wParam), HIWORD(wParam), reinterpret_cast<HWND>(lParam)))
         return 0;
      break;
   }
   case WM_MOUSEWHEEL:
   {
      if (onMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam), GET_KEYSTATE_WPARAM(wParam),
                       decodeMousePosition(lParam)))
         return 0;
      break;
   }
   case WM_MOUSEHWHEEL:
   {
      if (onMouseHorzWheel(GET_WHEEL_DELTA_WPARAM(wParam), GET_KEYSTATE_WPARAM(wParam),
                           decodeMousePosition(lParam)))
         return 0;
      break;
   }
   case WM_TIMER:
   {
      if (onTimer(wParam, reinterpret_cast<TIMERPROC>(lParam)))
         return 0;
      break;
   }
   default:
      break;
   }

   return ::DefWindowProc(hwnd, msgId, wParam, lParam);
}


Window::CreationResult Window::onNcCreate(HWND /*hwnd*/,
                                          const CREATESTRUCT* /*createInfo*/)
{
   return CreationResult::NotHandled;
}


Window::CreationResult Window::onCreate(const CREATESTRUCT* /*createInfo*/)
{
   return CreationResult::NotHandled;
}


bool Window::onClose()
{
   if (hwnd())
      ::DestroyWindow(hwnd());
   return true;
}


void Window::setupWindow(HWND hwnd)
{
   m_hwnd = hwnd;
}


void Window::cleanupWindow()
{
   removeSelf(m_hwnd);
   m_hwnd = NULL;
}

} // namespace win32

#endif //_WIN32
