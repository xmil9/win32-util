#ifdef _WIN32
#include "timer.h"
#include "string_util.h"
#include <tchar.h>
#include <atomic>
#include <mutex>
#include <unordered_map>


namespace
{

using namespace win32;

///////////////////

uint64_t makeUniqueId()
{
   static std::atomic<uint64_t> counter = 0;
   return ++counter;
}


String makeTimerPropertyName(uint64_t id)
{
   static String timerPropertyPrefix = _T("TimerProperty_");
   return String(timerPropertyPrefix) + win32::to_string(id);
}


void attachTimer(HWND hwnd, TimerOld* timer)
{
   const String propName = makeTimerPropertyName(timer->id());
   SetProp(hwnd, propName.c_str(), timer);
}


void detachTimer(HWND hwnd, const TimerOld* timer)
{
   const String propName = makeTimerPropertyName(timer->id());
   RemoveProp(hwnd, propName.c_str());
}


TimerOld* getAttachedTimer(HWND hwnd, UINT_PTR id)
{
   const String propName = makeTimerPropertyName(id);
   return reinterpret_cast<TimerOld*>(GetProp(hwnd, propName.c_str()));
}

} // namespace


namespace win32
{

///////////////////

TimerOld::TimerOld(HWND hwnd, Callback_t callback)
: m_hwnd{hwnd}, m_callback{callback}, m_id{makeUniqueId()}
{
   assert(m_hwnd && "TimerOld needs a window to attached data to.");
   setup();
}


TimerOld::~TimerOld()
{
   stop();
   cleanup();
}


TimerOld::TimerOld(TimerOld&& other) noexcept
{
   swap(*this, other);
}


TimerOld& TimerOld::operator=(TimerOld&& other) noexcept
{
   m_hwnd = other.m_hwnd;
   m_callback = other.m_callback;
   m_id = other.m_id;
   m_timeOutMs = other.m_timeOutMs;
   m_isContinuous = other.m_isContinuous;
   // Make sure dtor of moved-from timer does nothing.
   other.m_hwnd = NULL;
   other.m_id = 0;
   return *this;
}


void TimerOld::start(unsigned int timeOutMs, bool continuous)
{
   if (m_hwnd)
   {
      m_timeOutMs = timeOutMs;
      m_isContinuous = continuous;
      m_id = ::SetTimer(m_hwnd, m_id, timeOutMs, reinterpret_cast<TIMERPROC>(timerProc));
   }
}


void TimerOld::stop()
{
   if (isRunning())
   {
      ::KillTimer(m_hwnd, m_id);
      m_id = 0;
   }
}


void TimerOld::timerProc(HWND hwnd, UINT msgId, UINT timerId, DWORD systemTime)
{
   assert(msgId == WM_TIMER);
   if (msgId != WM_TIMER)
      return;

   if (hwnd)
   {
      TimerOld* timer = getAttachedTimer(hwnd, timerId);
      if (timer)
         timer->onTimerElapsed(systemTime);
   }
}


void TimerOld::setup()
{
   if (m_hwnd)
      attachTimer(m_hwnd, this);
}


void TimerOld::cleanup()
{
   if (m_hwnd)
      detachTimer(m_hwnd, this);
}


void TimerOld::onTimerElapsed(DWORD systemTime)
{
   m_callback(systemTime);
   if (m_isContinuous)
      start(m_timeOutMs, m_isContinuous);
}


///////////////////

Timer& Timer::operator=(Timer&& other) noexcept
{
   m_hwnd = other.m_hwnd;
   m_id = other.m_id;
   // Make sure dtor of moved-from object does nothing.
   other.m_hwnd = NULL;
   other.m_id = 0;
   return *this;
}


bool Timer::start(unsigned int timeOutMs)
{
   if (isValid())
      return (::SetTimer(m_hwnd, m_id, timeOutMs, NULL) != 0);
   return false;
}


bool Timer::stop()
{
   if (isValid())
      return ::KillTimer(m_hwnd, m_id);
   return false;
}


///////////////////

// Keeps track of all TimedCallback objects.
// Thread-safe.
class TimedCallbackRegistry
{
public:
   static void registerTimer(UINT_PTR id, TimedCallback* timer);
   static void unregisterTimer(UINT_PTR id);
   static TimedCallback* getTimer(UINT_PTR id);

private:
   static std::unordered_map<UINT_PTR, TimedCallback*> m_timers;
   static std::mutex m_guard;
};


std::unordered_map<UINT_PTR, TimedCallback*> TimedCallbackRegistry::m_timers;
std::mutex TimedCallbackRegistry::m_guard;


void TimedCallbackRegistry::registerTimer(UINT_PTR id, TimedCallback* timer)
{
   std::lock_guard<std::mutex> lock(m_guard);
   m_timers[id] = timer;
}

void TimedCallbackRegistry::unregisterTimer(UINT_PTR id)
{
   std::lock_guard<std::mutex> lock(m_guard);
   m_timers.erase(id);
}

TimedCallback* TimedCallbackRegistry::getTimer(UINT_PTR id)
{
   std::lock_guard<std::mutex> lock(m_guard);
   auto pos = m_timers.find(id);
   if (pos != m_timers.end())
      return pos->second;
   return nullptr;
}


///////////////////

TimedCallback::TimedCallback(Callback_t callback)
: m_callback{callback}, m_id{makeUniqueId()}
{
   setup();
}


TimedCallback::~TimedCallback()
{
   stop();
   cleanup();
}


TimedCallback::TimedCallback(TimedCallback&& other) noexcept
{
   swap(*this, other);
}


TimedCallback& TimedCallback::operator=(TimedCallback&& other) noexcept
{
   m_callback = other.m_callback;
   m_id = other.m_id;
   m_timeOutMs = other.m_timeOutMs;
   m_isContinuous = other.m_isContinuous;
   // Make sure dtor of moved-from timer does nothing.
   other.m_id = 0;
   return *this;
}


void TimedCallback::start(unsigned int timeOutMs, bool continuous)
{
   m_timeOutMs = timeOutMs;
   m_isContinuous = continuous;
   m_id = ::SetTimer(NULL, m_id, timeOutMs, reinterpret_cast<TIMERPROC>(timerProc));
}


void TimedCallback::stop()
{
   if (isRunning())
   {
      ::KillTimer(NULL, m_id);
      m_id = 0;
   }
}

void swap(TimedCallback& a, TimedCallback& b) noexcept
{
   using std::swap;
   swap(a.m_callback, b.m_callback);
   swap(a.m_id, b.m_id);
   swap(a.m_timeOutMs, b.m_timeOutMs);
   swap(a.m_isContinuous, b.m_isContinuous);
}


void TimedCallback::timerProc(HWND /*hwnd*/, UINT msgId, UINT timerId, DWORD systemTime)
{
   assert(msgId == WM_TIMER);
   if (msgId != WM_TIMER)
      return;

   TimedCallback* timer = TimedCallbackRegistry::getTimer(timerId);
   if (timer)
      timer->onTimerElapsed(systemTime);
}


void TimedCallback::setup()
{
   TimedCallbackRegistry::registerTimer(m_id, this);
}


void TimedCallback::cleanup()
{
   TimedCallbackRegistry::unregisterTimer(m_id);
}


void TimedCallback::onTimerElapsed(DWORD systemTime)
{
   m_callback(systemTime);
   if (m_isContinuous)
      start(m_timeOutMs, m_isContinuous);
}

} // namespace win32

#endif //_WIN32
