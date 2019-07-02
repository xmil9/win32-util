//
// Win32 utilities library
//
// Jul-2019, Michael Lindner
// MIT license
//
#include "screen_tests.h"
#include "screen.h"
#include "test_util.h"

using namespace win32;


namespace
{
///////////////////

void testVertScreenDpi()
{
   {
      const std::string caseLabel{"vertScreenDpi"};
      VERIFY(vertScreenDpi() > 0, caseLabel);
   }
}


void testHorzScreenDpi()
{
   {
      const std::string caseLabel{"horzScreenDpi"};
      VERIFY(horzScreenDpi() > 0, caseLabel);
   }
}

} // namespace


void testScreen()
{
   testVertScreenDpi();
   testHorzScreenDpi();
}
