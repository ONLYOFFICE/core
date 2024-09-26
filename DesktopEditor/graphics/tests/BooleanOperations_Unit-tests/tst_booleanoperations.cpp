
//#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include "../../GraphicsPath.h"

using namespace testing;

TEST(BooleanOperations, NoIntersOutside)
{
	Aggplus::CGraphicsPath path1, path2, resultIntersect, resultUnite, resultSubtract;

	path1.StartFigure();
	path1.MoveTo(100.0, 100.0);
	path1.LineTo(100.0, 200.0);
	path1.LineTo(200.0, 200.0);
	path1.LineTo(200.0, 100.0);
	path1.LineTo(100.0, 100.0);
	path1.CloseFigure();

	path2.StartFigure();
	path2.MoveTo(300.0, 300.0);
	path2.LineTo(300.0, 400.0);
	path2.LineTo(400.0, 400.0);
	path2.LineTo(400.0, 300.0);
	path2.LineTo(300.0, 300.0);
	path2.CloseFigure();

	resultIntersect.StartFigure();
	resultIntersect.CloseFigure();

	resultUnite.StartFigure();
	resultUnite.MoveTo(100.0, 100.0);
	resultUnite.LineTo(100.0, 200.0);
	resultUnite.LineTo(200.0, 200.0);
	resultUnite.LineTo(200.0, 100.0);
	resultUnite.LineTo(100.0, 100.0);
	resultUnite.MoveTo(300.0, 300.0);
	resultUnite.LineTo(300.0, 400.0);
	resultUnite.LineTo(400.0, 400.0);
	resultUnite.LineTo(400.0, 300.0);
	resultUnite.LineTo(300.0, 300.0);
	resultUnite.CloseFigure();

	resultSubtract.StartFigure();
	resultSubtract.MoveTo(100.0, 100.0);
	resultSubtract.LineTo(100.0, 200.0);
	resultSubtract.LineTo(200.0, 200.0);
	resultSubtract.LineTo(200.0, 100.0);
	resultSubtract.LineTo(100.0, 100.0);
	resultSubtract.CloseFigure();


	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Intersection) == resultIntersect);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Union) == resultUnite);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Subtraction) == resultSubtract);
}

TEST(BooleanOperations, NoIntersInside)
{
	Aggplus::CGraphicsPath path1, path2, resultIntersect, resultUnite, resultSubtract;

	path1.StartFigure();
	path1.MoveTo(100.0, 100.0);
	path1.LineTo(100.0, 400.0);
	path1.LineTo(400.0, 400.0);
	path1.LineTo(400.0, 100.0);
	path1.LineTo(100.0, 100.0);
	path1.CloseFigure();

	path2.StartFigure();
	path2.MoveTo(300.0, 300.0);
	path2.LineTo(300.0, 200.0);
	path2.LineTo(200.0, 200.0);
	path2.LineTo(200.0, 300.0);
	path2.LineTo(300.0, 300.0);
	path2.CloseFigure();

	resultIntersect.StartFigure();
	resultIntersect.MoveTo(300.0, 300.0);
	resultIntersect.LineTo(300.0, 200.0);
	resultIntersect.LineTo(200.0, 200.0);
	resultIntersect.LineTo(200.0, 300.0);
	resultIntersect.LineTo(300.0, 300.0);
	resultIntersect.CloseFigure();

	resultUnite.StartFigure();
	resultUnite.MoveTo(100.0, 100.0);
	resultUnite.LineTo(100.0, 400.0);
	resultUnite.LineTo(400.0, 400.0);
	resultUnite.LineTo(400.0, 100.0);
	resultUnite.LineTo(100.0, 100.0);
	resultUnite.CloseFigure();

	resultSubtract.StartFigure();
	resultSubtract.MoveTo(100.0, 100.0);
	resultSubtract.LineTo(100.0, 400.0);
	resultSubtract.LineTo(400.0, 400.0);
	resultSubtract.LineTo(400.0, 100.0);
	resultSubtract.LineTo(100.0, 100.0);
	resultSubtract.MoveTo(300.0, 300.0);
	resultSubtract.LineTo(200.0, 300.0);
	resultSubtract.LineTo(200.0, 200.0);
	resultSubtract.LineTo(300.0, 200.0);
	resultSubtract.LineTo(300.0, 300.0);
	resultSubtract.CloseFigure();


	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Intersection) == resultIntersect);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Union) == resultUnite);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Subtraction) == resultSubtract);
}

TEST(BooleanOperations, OneIntersOutside)
{
	Aggplus::CGraphicsPath path1, path2, resultIntersect, resultUnite, resultSubtract;

	path1.StartFigure();
	path1.MoveTo(100.0, 100.0);
	path1.LineTo(100.0, 200.0);
	path1.LineTo(200.0, 200.0);
	path1.LineTo(200.0, 100.0);
	path1.LineTo(100.0, 100.0);
	path1.CloseFigure();

	path2.StartFigure();
	path2.MoveTo(200.0, 150.0);
	path2.LineTo(300.0, 150.0);
	path2.LineTo(250.0, 200.0);
	path2.LineTo(200.0, 150.0);
	path2.CloseFigure();

	resultIntersect.StartFigure();
	resultIntersect.MoveTo(200.0, 150.0);
	resultIntersect.CloseFigure();

	resultUnite.StartFigure();
	resultUnite.MoveTo(100.0, 100.0);
	resultUnite.LineTo(100.0, 200.0);
	resultUnite.LineTo(200.0, 200.0);
	resultUnite.LineTo(200.0, 150.0);
	resultUnite.LineTo(250.0, 200.0);
	resultUnite.LineTo(300.0, 150.0);
	resultUnite.LineTo(200.0, 150.0);
	resultUnite.LineTo(200.0, 100.0);
	resultUnite.LineTo(100.0, 100.0);
	resultUnite.CloseFigure();

	resultSubtract.StartFigure();
	resultSubtract.MoveTo(100.0, 100.0);
	resultSubtract.LineTo(100.0, 200.0);
	resultSubtract.LineTo(200.0, 200.0);
	resultSubtract.LineTo(200.0, 100.0);
	resultSubtract.LineTo(100.0, 100.0);
	resultSubtract.CloseFigure();


	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Intersection) == resultIntersect);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Union) == resultUnite);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Subtraction) == resultSubtract);
}

TEST(BooleanOperations, OneIntersInside)
{
	Aggplus::CGraphicsPath path1, path2, resultIntersect, resultUnite, resultSubtract;

	path1.StartFigure();
	path1.MoveTo(100.0, 100.0);
	path1.LineTo(100.0, 200.0);
	path1.LineTo(200.0, 200.0);
	path1.LineTo(200.0, 100.0);
	path1.LineTo(100.0, 100.0);
	path1.CloseFigure();

	path2.StartFigure();
	path2.MoveTo(200.0, 150.0);
	path2.LineTo(150.0, 150.0);
	path2.LineTo(175.0, 175.0);
	path2.LineTo(200.0, 150.0);
	path2.CloseFigure();

	resultIntersect.StartFigure();
	resultIntersect.MoveTo(200.0, 150.0);
	resultIntersect.LineTo(150.0, 150.0);
	resultIntersect.LineTo(175.0, 175.0);
	resultIntersect.LineTo(200.0, 150.0);
	resultIntersect.CloseFigure();

	resultUnite.StartFigure();
	resultUnite.MoveTo(100.0, 100.0);
	resultUnite.LineTo(100.0, 200.0);
	resultUnite.LineTo(200.0, 200.0);
	resultUnite.LineTo(200.0, 100.0);
	resultUnite.LineTo(100.0, 100.0);
	resultUnite.CloseFigure();

	resultSubtract.StartFigure();
	resultSubtract.MoveTo(100.0, 100.0);
	resultSubtract.LineTo(100.0, 200.0);
	resultSubtract.LineTo(200.0, 200.0);
	resultSubtract.LineTo(200.0, 150.0);
	resultSubtract.LineTo(175.0, 175.0);
	resultSubtract.LineTo(150.0, 150.0);
	resultSubtract.LineTo(200.0, 150.0);
	resultSubtract.LineTo(200.0, 100.0);
	resultSubtract.LineTo(100.0, 100.0);
	resultSubtract.CloseFigure();


	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Intersection) == resultIntersect);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Union) == resultUnite);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Subtraction) == resultSubtract);
}

TEST(BooleanOperations, OverlapOutside)
{
	Aggplus::CGraphicsPath path1, path2, resultIntersect, resultUnite, resultSubtract;

	path1.StartFigure();
	path1.MoveTo(100.0, 100.0);
	path1.LineTo(100.0, 300.0);
	path1.LineTo(300.0, 300.0);
	path1.LineTo(300.0, 100.0);
	path1.LineTo(100.0, 100.0);
	path1.CloseFigure();

	path2.StartFigure();
	path2.MoveTo(300.0, 200.0);
	path2.LineTo(300.0, 400.0);
	path2.LineTo(400.0, 400.0);
	path2.LineTo(400.0, 200.0);
	path2.LineTo(300.0, 200.0);
	path2.CloseFigure();

	resultIntersect.StartFigure();
	resultIntersect.MoveTo(300.0, 300.0);
	resultIntersect.LineTo(300.0, 200.0);
	resultIntersect.LineTo(300.0, 300.0);
	resultIntersect.CloseFigure();

	resultUnite.StartFigure();
	resultUnite.MoveTo(100.0, 100.0);
	resultUnite.LineTo(100.0, 300.0);
	resultUnite.LineTo(300.0, 300.0);
	resultUnite.LineTo(300.0, 400.0);
	resultUnite.LineTo(400.0, 400.0);
	resultUnite.LineTo(400.0, 200.0);
	resultUnite.LineTo(300.0, 200.0);
	resultUnite.LineTo(300.0, 100.0);
	resultUnite.LineTo(100.0, 100.0);
	resultUnite.CloseFigure();

	resultSubtract.StartFigure();
	resultSubtract.MoveTo(100.0, 100.0);
	resultSubtract.LineTo(100.0, 300.0);
	resultSubtract.LineTo(300.0, 300.0);
	resultSubtract.LineTo(300.0, 100.0);
	resultSubtract.LineTo(100.0, 100.0);
	resultSubtract.CloseFigure();


	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Intersection) == resultIntersect);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Union) == resultUnite);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Subtraction) == resultSubtract);
}

TEST(BooleanOperations, OverlapInside)
{
	Aggplus::CGraphicsPath path1, path2, resultIntersect, resultUnite, resultSubtract;

	path1.StartFigure();
	path1.MoveTo(100.0, 100.0);
	path1.LineTo(100.0, 400.0);
	path1.LineTo(400.0, 400.0);
	path1.LineTo(400.0, 100.0);
	path1.LineTo(100.0, 100.0);
	path1.CloseFigure();

	path2.StartFigure();
	path2.MoveTo(200.0, 200.0);
	path2.LineTo(400.0, 200.0);
	path2.LineTo(400.0, 300.0);
	path2.LineTo(200.0, 300.0);
	path2.LineTo(200.0, 200.0);
	path2.CloseFigure();

	resultIntersect.StartFigure();
	resultIntersect.MoveTo(400.0, 300.0);
	resultIntersect.LineTo(400.0, 200.0);
	resultIntersect.LineTo(200.0, 200.0);
	resultIntersect.LineTo(200.0, 300.0);
	resultIntersect.LineTo(400.0, 300.0);
	resultIntersect.CloseFigure();

	resultUnite.StartFigure();
	resultUnite.MoveTo(100.0, 100.0);
	resultUnite.LineTo(100.0, 400.0);
	resultUnite.LineTo(400.0, 400.0);
	resultUnite.LineTo(400.0, 100.0);
	resultUnite.LineTo(100.0, 100.0);
	resultUnite.CloseFigure();

	resultSubtract.StartFigure();
	resultSubtract.MoveTo(100.0, 100.0);
	resultSubtract.LineTo(100.0, 400.0);
	resultSubtract.LineTo(400.0, 400.0);
	resultSubtract.LineTo(400.0, 300.0);
	resultSubtract.LineTo(200.0, 300.0);
	resultSubtract.LineTo(200.0, 200.0);
	resultSubtract.LineTo(400.0, 200.0);
	resultSubtract.LineTo(400.0, 100.0);
	resultSubtract.LineTo(100.0, 100.0);
	resultSubtract.CloseFigure();


	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Intersection) == resultIntersect);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Union) == resultUnite);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Subtraction) == resultSubtract);
}

TEST(BooleanOperations, LineIntersLine)
{
	Aggplus::CGraphicsPath path1, path2, resultIntersect, resultUnite, resultSubtract;

	path1.StartFigure();
	path1.MoveTo(100.0, 100.0);
	path1.LineTo(100.0, 300.0);
	path1.LineTo(300.0, 300.0);
	path1.LineTo(300.0, 100.0);
	path1.LineTo(100.0, 100.0);
	path1.CloseFigure();

	path2.StartFigure();
	path2.MoveTo(200.0, 200.0);
	path2.LineTo(400.0, 200.0);
	path2.LineTo(400.0, 400.0);
	path2.LineTo(200.0, 400.0);
	path2.LineTo(200.0, 200.0);
	path2.CloseFigure();

	resultIntersect.StartFigure();
	resultIntersect.MoveTo(200.0, 300.0);
	resultIntersect.LineTo(300.0, 300.0);
	resultIntersect.LineTo(300.0, 200.0);
	resultIntersect.LineTo(200.0, 200.0);
	resultIntersect.LineTo(200.0, 300.0);
	resultIntersect.CloseFigure();

	resultUnite.StartFigure();
	resultUnite.MoveTo(100.0, 100.0);
	resultUnite.LineTo(100.0, 300.0);
	resultUnite.LineTo(200.0, 300.0);
	resultUnite.LineTo(200.0, 400.0);
	resultUnite.LineTo(400.0, 400.0);
	resultUnite.LineTo(400.0, 200.0);
	resultUnite.LineTo(300.0, 200.0);
	resultUnite.LineTo(300.0, 100.0);
	resultUnite.LineTo(100.0, 100.0);
	resultUnite.CloseFigure();

	resultSubtract.StartFigure();
	resultSubtract.MoveTo(100.0, 100.0);
	resultSubtract.LineTo(100.0, 300.0);
	resultSubtract.LineTo(200.0, 300.0);
	resultSubtract.LineTo(200.0, 200.0);
	resultSubtract.LineTo(300.0, 200.0);
	resultSubtract.LineTo(300.0, 100.0);
	resultSubtract.LineTo(100.0, 100.0);
	resultSubtract.CloseFigure();


	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Intersection) == resultIntersect);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Union) == resultUnite);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Subtraction) == resultSubtract);
}

TEST(BooleanOperations, CurveIntersLine)
{
	Aggplus::CGraphicsPath path1, path2, resultIntersect, resultUnite, resultSubtract;

	path1.AddEllipse(-300.0, -300.0, 200.0, 200.0);

	path2.StartFigure();
	path2.MoveTo(-200.0, -200.0);
	path2.LineTo(0.0, -200.0);
	path2.LineTo(0.0, 0.0);
	path2.LineTo(-200.0, 0.0);
	path2.LineTo(-200.0, -200.0);
	path2.CloseFigure();

	resultIntersect.StartFigure();
	resultIntersect.MoveTo(-100.0, -200.0);
	resultIntersect.CurveTo(-100.0, -144.772, -144.772, -100.0, -200.0, -100.0);
	resultIntersect.LineTo(-200.0, -200.0);
	resultIntersect.LineTo(-100.0, -200.0);
	resultIntersect.CloseFigure();

	resultUnite.StartFigure();
	resultUnite.MoveTo(-100.0, -200.0);
	resultUnite.LineTo(0.0, -200.0);
	resultUnite.LineTo(0.0, 0.0);
	resultUnite.LineTo(-200.0, 0.0);
	resultUnite.LineTo(-200.0, -100.0);
	resultUnite.CurveTo(-255.228, -100.0, -300.0, -144.772, -300.0, -200.0);
	resultUnite.CurveTo(-300.0, -255.228, -255.228, -300.0, -200.0, -300.0);
	resultUnite.CurveTo(-144.772, -300.0, -100.0, -255.228, -100.0, -200.0);
	resultUnite.CloseFigure();

	resultSubtract.StartFigure();
	resultSubtract.MoveTo(-100.0, -200.0);
	resultSubtract.LineTo(-200.0, -200.0);
	resultSubtract.LineTo(-200.0, -100.0);
	resultSubtract.CurveTo(-255.228, -100.0, -300.0, -144.772, -300.0, -200.0);
	resultSubtract.CurveTo(-300.0, -255.228, -255.228, -300.0, -200.0, -300.0);
	resultSubtract.CurveTo(-144.772, -300.0, -100.0, -255.228, -100.0, -200.0);
	resultSubtract.CloseFigure();


	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Intersection) == resultIntersect);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Union) == resultUnite);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Subtraction) == resultSubtract);
}

TEST(BooleanOperations, CurveIntersCurve)
{
	Aggplus::CGraphicsPath path1, path2, resultIntersect, resultUnite, resultSubtract;

	path1.AddEllipse(-300.0, -300.0, 200.0, 200.0);

	path2.AddEllipse(-200.0, -200.0, 200.0, 200.0);

	resultIntersect.StartFigure();
	resultIntersect.MoveTo(-100.0, -200.0);
	resultIntersect.CurveTo(-100.0, -144.772, -144.772, -100.0, -200.0, -100.0);
	resultIntersect.CurveTo(-200.0, -155.228, -155.228, -200.0, -100.0, -200.0);
	resultIntersect.CloseFigure();

	resultUnite.StartFigure();
	resultUnite.MoveTo(-100.0, -200.0);
	resultUnite.CurveTo(-44.772, -200.0, 0.0, -155.228, 0.0, -100.0);
	resultUnite.CurveTo(0.0, -44.772, -44.772, 0.0, -100.0, 0.0);
	resultUnite.CurveTo(-155.228, 0.0, -200.0, -44.772, -200.0, -100.0);
	resultUnite.CurveTo(-255.228, -100.0, -300.0, -144.772, -300.0, -200.0);
	resultUnite.CurveTo(-300.0, -255.228, -255.228, -300.0, -200.0, -300.0);
	resultUnite.CurveTo(-144.772, -300.0, -100.0, -255.228, -100.0, -200.0);
	resultUnite.CloseFigure();

	resultSubtract.StartFigure();
	resultSubtract.MoveTo(-100.0, -200.0);
	resultSubtract.CurveTo(-155.228, -200.0, -200.0, -155.228, -200.0, -100.0);
	resultSubtract.CurveTo(-255.228, -100.0, -300.0, -144.772, -300.0, -200.0);
	resultSubtract.CurveTo(-300.0, -255.228, -255.228, -300.0, -200.0, -300.0);
	resultSubtract.CurveTo(-144.772, -300.0, -100.0, -255.228, -100.0, -200.0);
	resultSubtract.CloseFigure();


	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Intersection) == resultIntersect);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Union) == resultUnite);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Subtraction) == resultSubtract);
}
