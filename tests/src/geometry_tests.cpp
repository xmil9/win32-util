#include "geometry_tests.h"
#include "geometry.h"
#include "test_util.h"

using namespace win32;


namespace
{

///////////////////

void testPointDefaultCtor()
{
   {
      const std::string caseLabel{"Point default ctor"};
      Point pt;
      VERIFY(pt.x == 0, caseLabel);
      VERIFY(pt.y == 0, caseLabel);
   }
}


void testPointCtorForCoordinates()
{
   {
      const std::string caseLabel{"Point coordinate ctor"};
      Point pt{1, 2};
      VERIFY(pt.x == 1, caseLabel);
      VERIFY(pt.y == 2, caseLabel);
   }
   {
      const std::string caseLabel{"Point coordinate ctor for negative coordinates"};
      Point pt{-1, -2};
      VERIFY(pt.x == -1, caseLabel);
      VERIFY(pt.y == -2, caseLabel);
   }
}


void testPointCtorForPOINT()
{
   {
      const std::string caseLabel{"Point POINT ctor"};
      POINT src{4, 10};
      Point pt{src};
      VERIFY(pt.x == src.x, caseLabel);
      VERIFY(pt.y == src.y, caseLabel);
   }
   {
      const std::string caseLabel{"Point POINT ctor for negative coordinates"};
      POINT src{-4, -10};
      Point pt{src};
      VERIFY(pt.x == src.x, caseLabel);
      VERIFY(pt.y == src.y, caseLabel);
   }
}


void testPointCopyCtor()
{
   {
      const std::string caseLabel{"Point copy ctor"};
      Point src{4, 10};
      Point copy{src};
      VERIFY(copy.x == src.x, caseLabel);
      VERIFY(copy.y == src.y, caseLabel);
   }
}


void testPointMoveCtor()
{
   {
      const std::string caseLabel{"Point move ctor"};
      Point src{4, -10};
      Point movedTo{std::move(src)};
      VERIFY(movedTo.x == 4, caseLabel);
      VERIFY(movedTo.y == -10, caseLabel);
   }
}


void testPointCopyAssignment()
{
   {
      const std::string caseLabel{"Point copy assignment"};
      Point src{4, 10};
      Point copy;
      copy = src;
      VERIFY(copy.x == src.x, caseLabel);
      VERIFY(copy.y == src.y, caseLabel);
   }
}


void testPointMoveAssignment()
{
   {
      const std::string caseLabel{"Point move assignment"};
      Point src{-4, 10};
      Point copy;
      copy = std::move(src);
      VERIFY(copy.x == -4, caseLabel);
      VERIFY(copy.y == 10, caseLabel);
   }
}


void testPointSwap()
{
   {
      const std::string caseLabel{"Point swap"};
      Point a{1, 2};
      Point b{3, 4};
      swap(a, b);
      VERIFY(a.x == 3, caseLabel);
      VERIFY(a.y == 4, caseLabel);
      VERIFY(b.x == 1, caseLabel);
      VERIFY(b.y == 2, caseLabel);
   }
}


void testPointEquality()
{
   {
      const std::string caseLabel{"Point equality for equal objects"};
      Point a{1, 2};
      Point b{1, 2};
      VERIFY(a == b, caseLabel);
   }
   {
      const std::string caseLabel{"Point equality for unequal x coordinate"};
      Point a{1, 2};
      Point b{2, 2};
      VERIFY(!(a == b), caseLabel);
   }
   {
      const std::string caseLabel{"Point equality for unequal y coordinate"};
      Point a{1, 2};
      Point b{1, -3};
      VERIFY(!(a == b), caseLabel);
   }
}


void testPointInequality()
{
   {
      const std::string caseLabel{"Point inequality for equal objects"};
      Point a{1, 2};
      Point b{1, 2};
      VERIFY(!(a != b), caseLabel);
   }
   {
      const std::string caseLabel{"Point inequality for unequal x coordinate"};
      Point a{1, 2};
      Point b{2, 2};
      VERIFY(a != b, caseLabel);
   }
   {
      const std::string caseLabel{"Point inequality for unequal y coordinate"};
      Point a{1, 2};
      Point b{1, -3};
      VERIFY(a != b, caseLabel);
   }
}


///////////////////

void testRectDefaultCtor()
{
   {
      const std::string caseLabel{"Rect default ctor"};
      Rect r;
      VERIFY(r.left == 0, caseLabel);
      VERIFY(r.top == 0, caseLabel);
      VERIFY(r.right == 0, caseLabel);
      VERIFY(r.bottom == 0, caseLabel);
   }
}


void testRectCtorForCoordinates()
{
   {
      const std::string caseLabel{"Rect coordinate ctor"};
      Rect r{1, 2, 3, 4};
      VERIFY(r.left == 1, caseLabel);
      VERIFY(r.top == 2, caseLabel);
      VERIFY(r.right == 3, caseLabel);
      VERIFY(r.bottom == 4, caseLabel);
   }
   {
      const std::string caseLabel{"Rect coordinate ctor for negative coordinates"};
      Rect r{-10, -20, -5, -15};
      VERIFY(r.left == -10, caseLabel);
      VERIFY(r.top == -20, caseLabel);
      VERIFY(r.right == -5, caseLabel);
      VERIFY(r.bottom == -15, caseLabel);
   }
}


void testRectCtorForRECT()
{
   {
      const std::string caseLabel{"Rect ctor for RECT"};
      RECT src{1, 2, 3, 4};
      Rect r{src};
      VERIFY(r.left == src.left, caseLabel);
      VERIFY(r.top == src.top, caseLabel);
      VERIFY(r.right == src.right, caseLabel);
      VERIFY(r.bottom == src.bottom, caseLabel);
   }
   {
      const std::string caseLabel{"Rect ctor for RECT with negative coordinates"};
      RECT src{-10, -20, -3, -4};
      Rect r{src};
      VERIFY(r.left == src.left, caseLabel);
      VERIFY(r.top == src.top, caseLabel);
      VERIFY(r.right == src.right, caseLabel);
      VERIFY(r.bottom == src.bottom, caseLabel);
   }
}


void testRectCopyCtor()
{
   {
      const std::string caseLabel{"Rect copy ctor"};
      Rect src{1, 2, 3, 4};
      Rect copy{src};
      VERIFY(copy.left == src.left, caseLabel);
      VERIFY(copy.top == src.top, caseLabel);
      VERIFY(copy.right == src.right, caseLabel);
      VERIFY(copy.bottom == src.bottom, caseLabel);
   }
}


void testRectMoveCtor()
{
   {
      const std::string caseLabel{"Rect move ctor"};
      Rect src{1, 2, 3, 4};
      Rect movedTo{std::move(src)};
      VERIFY(movedTo.left == 1, caseLabel);
      VERIFY(movedTo.top == 2, caseLabel);
      VERIFY(movedTo.right == 3, caseLabel);
      VERIFY(movedTo.bottom == 4, caseLabel);
   }
}


void testRectCopyAssignment()
{
   {
      const std::string caseLabel{"Rect copy assignment"};
      Rect src{1, 2, 3, 4};
      Rect copy;
      copy = src;
      VERIFY(copy.left == src.left, caseLabel);
      VERIFY(copy.top == src.top, caseLabel);
      VERIFY(copy.right == src.right, caseLabel);
      VERIFY(copy.bottom == src.bottom, caseLabel);
   }
}


void testRectMoveAssignment()
{
   {
      const std::string caseLabel{"Rect move assignment"};
      Rect src{1, 2, 3, 4};
      Rect movedTo;
      movedTo = std::move(src);
      VERIFY(movedTo.left == src.left, caseLabel);
      VERIFY(movedTo.top == src.top, caseLabel);
      VERIFY(movedTo.right == src.right, caseLabel);
      VERIFY(movedTo.bottom == src.bottom, caseLabel);
   }
}


void testRectSwap()
{
   {
      const std::string caseLabel{"Rect swap"};
      Rect a{1, 2, 3, 4};
      Rect b{6, 7, 8, 9};
      swap(a, b);
      VERIFY(a.left == 6, caseLabel);
      VERIFY(a.top == 7, caseLabel);
      VERIFY(a.right == 8, caseLabel);
      VERIFY(a.bottom == 9, caseLabel);
      VERIFY(b.left == 1, caseLabel);
      VERIFY(b.top == 2, caseLabel);
      VERIFY(b.right == 3, caseLabel);
      VERIFY(b.bottom == 4, caseLabel);
   }
}


void testRectWidth()
{
   {
      const std::string caseLabel{"Rect::width"};
      Rect r{1, 2, 10, 4};
      VERIFY(r.width() == 9, caseLabel);
   }
   {
      const std::string caseLabel{"Rect::width for negative coordinates"};
      Rect r{-10, -20, -5, -4};
      VERIFY(r.width() == 5, caseLabel);
   }
   {
      const std::string caseLabel{"Rect::width for mixed coordinates"};
      Rect r{-10, -20, 5, 4};
      VERIFY(r.width() == 15, caseLabel);
   }
   {
      const std::string caseLabel{"Rect::width for non-normalized rect"};
      Rect r{20, 2, 10, 4};
      VERIFY(r.width() == 10, caseLabel);
   }
   {
      const std::string caseLabel{"Rect::width for width-less rect"};
      Rect r{20, 2, 20, 4};
      VERIFY(r.width() == 0, caseLabel);
   }
}


void testRectHeight()
{
   {
      const std::string caseLabel{"Rect::height"};
      Rect r{1, 2, 10, 4};
      VERIFY(r.height() == 2, caseLabel);
   }
   {
      const std::string caseLabel{"Rect::height for negative coordinates"};
      Rect r{-10, -20, -5, -4};
      VERIFY(r.height() == 16, caseLabel);
   }
   {
      const std::string caseLabel{"Rect::height for mixed coordinates"};
      Rect r{-10, -20, 5, 4};
      VERIFY(r.height() == 24, caseLabel);
   }
   {
      const std::string caseLabel{"Rect::height for non-normalized rect"};
      Rect r{5, 20, 10, 4};
      VERIFY(r.height() == 16, caseLabel);
   }
   {
      const std::string caseLabel{"Rect::height for height-less rect"};
      Rect r{10, 4, 20, 4};
      VERIFY(r.height() == 0, caseLabel);
   }
}


void testRectOffset()
{
   {
      const std::string caseLabel{"Rect::offset"};
      Rect r{1, 2, 30, 40};
      r.offset(2, 3);
      VERIFY(r.left == 3, caseLabel);
      VERIFY(r.top == 5, caseLabel);
      VERIFY(r.right == 32, caseLabel);
      VERIFY(r.bottom == 43, caseLabel);
   }
   {
      const std::string caseLabel{"Rect::offset for no offset"};
      Rect r{1, 2, 30, 40};
      r.offset(0, 0);
      VERIFY(r.left == 1, caseLabel);
      VERIFY(r.top == 2, caseLabel);
      VERIFY(r.right == 30, caseLabel);
      VERIFY(r.bottom == 40, caseLabel);
   }
   {
      const std::string caseLabel{"Rect::offset for negative offset"};
      Rect r{10, 20, 30, 40};
      r.offset(-5, -1);
      VERIFY(r.left == 5, caseLabel);
      VERIFY(r.top == 19, caseLabel);
      VERIFY(r.right == 25, caseLabel);
      VERIFY(r.bottom == 39, caseLabel);
   }
   {
      const std::string caseLabel{
         "Rect::offset for negative coordinates and positive offset"};
      Rect r{-10, -20, -5, -4};
      r.offset(6, 8);
      VERIFY(r.left == -4, caseLabel);
      VERIFY(r.top == -12, caseLabel);
      VERIFY(r.right == 1, caseLabel);
      VERIFY(r.bottom == 4, caseLabel);
   }
   {
      const std::string caseLabel{
         "Rect::offset for negative coordinates and negative offset"};
      Rect r{-10, -20, -5, -4};
      r.offset(-6, -8);
      VERIFY(r.left == -16, caseLabel);
      VERIFY(r.top == -28, caseLabel);
      VERIFY(r.right == -11, caseLabel);
      VERIFY(r.bottom == -12, caseLabel);
   }
}


void testRectEquality()
{
   {
      const std::string caseLabel{"Rect equality for equal objects"};
      Rect a{1, 2, 3, 4};
      Rect b{1, 2, 3, 4};
      VERIFY(a == b, caseLabel);
   }
   {
      const std::string caseLabel{"Rect equality for unequal left coordinate"};
      Rect a{1, 2, 3, 4};
      Rect b{2, 2, 3, 4};
      VERIFY(!(a == b), caseLabel);
   }
   {
      const std::string caseLabel{"Rect equality for unequal top coordinate"};
      Rect a{1, 2, 3, 4};
      Rect b{1, 20, 3, 4};
      VERIFY(!(a == b), caseLabel);
   }
   {
      const std::string caseLabel{"Rect equality for unequal right coordinate"};
      Rect a{1, 2, 3, 4};
      Rect b{1, 2, -3, 4};
      VERIFY(!(a == b), caseLabel);
   }
   {
      const std::string caseLabel{"Rect equality for unequal bottom coordinate"};
      Rect a{1, 2, 3, 4};
      Rect b{1, 2, 3, 0};
      VERIFY(!(a == b), caseLabel);
   }
}


void testRectInequality()
{
   {
      const std::string caseLabel{"Rect inequality for equal objects"};
      Rect a{1, 2, 3, 4};
      Rect b{1, 2, 3, 4};
      VERIFY(!(a != b), caseLabel);
   }
   {
      const std::string caseLabel{"Rect inequality for unequal left coordinate"};
      Rect a{1, 2, 3, 4};
      Rect b{2, 2, 3, 4};
      VERIFY(a != b, caseLabel);
   }
   {
      const std::string caseLabel{"Rect inequality for unequal top coordinate"};
      Rect a{1, 2, 3, 4};
      Rect b{1, 20, 3, 4};
      VERIFY(a != b, caseLabel);
   }
   {
      const std::string caseLabel{"Rect inequality for unequal right coordinate"};
      Rect a{1, 2, 3, 4};
      Rect b{1, 2, -3, 4};
      VERIFY(a != b, caseLabel);
   }
   {
      const std::string caseLabel{"Rect inequality for unequal bottom coordinate"};
      Rect a{1, 2, 3, 4};
      Rect b{1, 2, 3, 0};
      VERIFY(a != b, caseLabel);
   }
}


void testRectIntersect()
{
   {
      const std::string caseLabel{
         "Intersect rects intersecting at vertical sides with positive coordinates."};
      Rect a{10, 20, 30, 40};
      Rect b{5, 25, 20, 35};

      auto intersection = intersect(a, b);

      VERIFY(intersection.first, caseLabel);
      VERIFY(intersection.second == Rect(10, 25, 20, 35), caseLabel);
   }
   {
      const std::string caseLabel{
         "Intersect rects intersecting at vertical sides with negative coordinates."};
      Rect a{-30, 20, -10, 40};
      Rect b{-20, 25, -5, 35};

      auto intersection = intersect(a, b);

      VERIFY(intersection.first, caseLabel);
      VERIFY(intersection.second == Rect(-20, 25, -10, 35), caseLabel);
   }
   {
      const std::string caseLabel{
         "Intersect rects intersecting at horizontal sides with positive coordinates."};
      Rect a{10, 20, 30, 40};
      Rect b{15, 10, 25, 30};

      auto intersection = intersect(a, b);

      VERIFY(intersection.first, caseLabel);
      VERIFY(intersection.second == Rect(15, 20, 25, 30), caseLabel);
   }
   {
      const std::string caseLabel{
         "Intersect rects intersecting at horizontal sides with negative coordinates."};
      Rect a{10, -40, 30, -20};
      Rect b{15, -30, 25, -10};

      auto intersection = intersect(a, b);

      VERIFY(intersection.first, caseLabel);
      VERIFY(intersection.second == Rect(15, -30, 25, -20), caseLabel);
   }
   {
      const std::string caseLabel{"Intersect rects where one is fully inside the other."};
      Rect a{10, 5, 30, 40};
      Rect b{15, 10, 25, 30};

      auto intersection = intersect(a, b);

      VERIFY(intersection.first, caseLabel);
      VERIFY(intersection.second == b, caseLabel);
   }
   {
      const std::string caseLabel{"Intersect rects that have no vertical intersection."};
      Rect a{10, 5, 30, 40};
      Rect b{15, 50, 25, 60};

      auto intersection = intersect(a, b);

      VERIFY(!intersection.first, caseLabel);
   }
   {
      const std::string caseLabel{
         "Intersect rects that have no horizontal intersection."};
      Rect a{10, 5, 30, 40};
      Rect b{-10, 10, 0, 30};

      auto intersection = intersect(a, b);

      VERIFY(!intersection.first, caseLabel);
   }
   {
      const std::string caseLabel{"Intersect rects that don't overlap at all."};
      Rect a{10, 5, 30, 40};
      Rect b{-10, 50, 0, 60};

      auto intersection = intersect(a, b);

      VERIFY(!intersection.first, caseLabel);
   }
}


void testRectUnite()
{
   {
      const std::string caseLabel{
         "Unite rects that overlap at vertical sides with positive coordinates."};
      Rect a{10, 20, 30, 40};
      Rect b{5, 25, 20, 35};

      Rect united = unite(a, b);

      VERIFY(united == Rect(5, 20, 30, 40), caseLabel);
   }
   {
      const std::string caseLabel{
         "Unite rects that overlap at vertical sides with negative coordinates."};
      Rect a{-30, 20, -10, 40};
      Rect b{-20, 25, -5, 35};

      Rect united = unite(a, b);

      VERIFY(united == Rect(-30, 20, -5, 40), caseLabel);
   }
   {
      const std::string caseLabel{
         "Unite rects that overlap at horizontal sides with positive coordinates."};
      Rect a{10, 20, 30, 40};
      Rect b{15, 10, 25, 30};

      Rect united = unite(a, b);

      VERIFY(united == Rect(10, 10, 30, 40), caseLabel);
   }
   {
      const std::string caseLabel{
         "Unite rects that overlap at horizontal sides with negative coordinates."};
      Rect a{10, -40, 30, -20};
      Rect b{15, -30, 25, -10};

      Rect united = unite(a, b);

      VERIFY(united == Rect(10, -40, 30, -10), caseLabel);
   }
   {
      const std::string caseLabel{"Unite rects that don't intersect."};
      Rect a{10, 20, 30, 40};
      Rect b{-10, -5, -20, -35};

      Rect united = unite(a, b);

      VERIFY(united == Rect(-10, -5, 30, 40), caseLabel);
   }
   {
      const std::string caseLabel{"Unite rects that overlap completely."};
      Rect a{10, 20, 30, 40};
      Rect b{12, 22, 28, 38};

      Rect united = unite(a, b);

      VERIFY(united == a, caseLabel);
   }
}

} // namespace


void testGeometry(HWND /*testRunnerWnd*/)
{
   testPointDefaultCtor();
   testPointCtorForCoordinates();
   testPointCtorForPOINT();
   testPointCopyCtor();
   testPointMoveCtor();
   testPointCopyAssignment();
   testPointMoveAssignment();
   testPointSwap();
   testPointEquality();
   testPointInequality();

   testRectDefaultCtor();
   testRectCtorForCoordinates();
   testRectCtorForRECT();
   testRectCopyCtor();
   testRectMoveCtor();
   testRectCopyAssignment();
   testRectMoveAssignment();
   testRectSwap();
   testRectWidth();
   testRectHeight();
   testRectOffset();
   testRectEquality();
   testRectInequality();
   testRectIntersect();
   testRectUnite();
}
