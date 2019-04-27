#pragma once
#ifdef _WIN32
#include "device_context.h"
#include "geometry.h"
#include "string_util.h"
#include "win32_util_api.h"
#include <windows.h>
#include <string>


namespace win32
{

///////////////////

// Wrapper for Win32 HWND handles.
// Provides API to interact with windows. No ownership semantics.
// Destroying the window clears the internal hwnd handle.
class WIN32UTIL_API Window
{
 public:
   Window() = default;
   explicit Window(HWND hwnd);
   virtual ~Window() = default;
   Window(const Window&) = delete;
   Window(Window&& src) = default;
   Window& operator=(const Window&) = delete;
   Window& operator=(Window&& src) = default;

   explicit operator bool() const;
   operator HWND() const;
   friend void swap(Window& a, Window& b) noexcept;

   HWND hwnd() const;
   bool create(HWND parent, const Rect& bounds, const String& title,
               unsigned long style, unsigned long extStyle = 0);
   // Posts WM_CLOSE message.
   void close();
   // Sends WM_CLOSE message.
   void closeImmediately();
   // Checks if the window exists. Calls IsWindow() Win32 function.
   bool exists() const;
   bool isVisible() const;
   void setVisible(bool visible);
   Rect clientBounds() const;
   Rect windowBounds() const;
   SharedDC clientDC() const;
   SharedDC windowDC() const;
   HWND parent() const;
   HWND owner() const;
   String title() const;
   void setTitle(const String& title);
   bool postMessage(UINT msgId, WPARAM wParam = 0, LPARAM lParam = 0) const;
   LRESULT sendMessage(UINT msgId, WPARAM wParam = 0, LPARAM lParam = 0) const;

 protected:
   enum class CreationResult
   {
      Handled,
      NotHandled,
      Abort
   };

   virtual HINSTANCE appInstance() const;
   virtual const TCHAR* windowClassName() const;
   bool isWindowClassRegistered() const;
   virtual bool registerWindowClass() const;

   static LRESULT windowProc(HWND hwnd, UINT msgId, WPARAM wParam, LPARAM lParam);
   virtual LRESULT handleMessage(HWND hwnd, UINT msgId, WPARAM wParam, LPARAM lParam);
   virtual CreationResult onNcCreate(HWND hwnd, const CREATESTRUCT* createInfo);
   virtual bool onNcDestroy() { return false; }
   virtual CreationResult onCreate(const CREATESTRUCT* createInfo);
   virtual bool onDestroy() { return false; }
   virtual bool onClose();
   virtual bool onNcPaint(HRGN /*updateRegion*/) { return false; }
   virtual bool onPaint() { return false; }
   virtual bool onTimer(UINT_PTR /*timerId*/, TIMERPROC /*callback*/) { return false; }

 private:
   void setupWindow(HWND hwnd);
   void cleanupWindow();

 private:
   HWND m_hwnd = NULL;
};



inline Window::Window(HWND hwnd)
   : m_hwnd{hwnd}
{
}

inline Window::operator bool() const
{
   return hwnd() != NULL;
}

inline Window::operator HWND() const
{
   return hwnd();
}

inline void swap(Window& a, Window& b) noexcept
{
   std::swap(a.m_hwnd, b.m_hwnd);
}

inline HWND Window::hwnd() const
{
   return m_hwnd;
}

} // namespace win32

#endif //_WIN32
