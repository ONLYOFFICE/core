
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

TEST(BooleanOperations, RectIntersRect)
{
	Aggplus::CGraphicsPath path1, path2, resultIntersect, resultUnite, resultSubtract;

	path1.StartFigure();
	path1.MoveTo(55.0, 25.0);
	path1.LineTo(255.0, 25.0);
	path1.LineTo(255.0, 225.0);
	path1.LineTo(55.0, 225.0);
	path1.LineTo(55.0, 25.0);
	path1.CloseFigure();

	path2.StartFigure();
	path2.MoveTo(152.0, 28.0);
	path2.LineTo(352.0, 28.0);
	path2.LineTo(352.0, 228.0);
	path2.LineTo(152.0, 228.0);
	path2.LineTo(152.0, 28.0);
	path2.CloseFigure();

	resultIntersect.StartFigure();
	resultIntersect.MoveTo(255.0, 28.0);
	resultIntersect.LineTo(255.0, 225.0);
	resultIntersect.LineTo(152.0, 225.0);
	resultIntersect.LineTo(152.0, 28.0);
	resultIntersect.LineTo(255.0, 28.0);
	resultIntersect.CloseFigure();

	resultUnite.StartFigure();
	resultUnite.MoveTo(55.0, 25.0);
	resultUnite.LineTo(255.0, 25.0);
	resultUnite.LineTo(255.0, 28.0);
	resultUnite.LineTo(352.0, 28.0);
	resultUnite.LineTo(352.0, 228.0);
	resultUnite.LineTo(152.0, 228.0);
	resultUnite.LineTo(152.0, 225.0);
	resultUnite.LineTo(55.0, 225.0);
	resultUnite.LineTo(55.0, 25.0);
	resultUnite.CloseFigure();

	resultSubtract.StartFigure();
	resultSubtract.MoveTo(55.0, 25.0);
	resultSubtract.LineTo(255.0, 25.0);
	resultSubtract.LineTo(255.0, 28.0);
	resultSubtract.LineTo(152.0, 28.0);
	resultSubtract.LineTo(152.0, 225.0);
	resultSubtract.LineTo(55.0, 225.0);
	resultSubtract.LineTo(55.0, 25.0);
	resultSubtract.CloseFigure();

	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Intersection) == resultIntersect);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Union) == resultUnite);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Subtraction) == resultSubtract);
}

TEST(BooleanOperations, EllipseIntersEllipse)
{
	Aggplus::CGraphicsPath path1, path2, resultIntersect, resultUnite, resultSubtract;

	path1.AddEllipse(148.0, 82.0, 200.0, 200.0);
	path2.AddEllipse(39.0, 130.0, 200.0, 200.0);

	resultIntersect.StartFigure();
	resultIntersect.MoveTo(225.883, 279.546);
	resultIntersect.CurveTo(181.297, 269.48, 148.0, 229.628, 148.0, 182.0);
	resultIntersect.CurveTo(148.0, 163.973, 152.77, 147.06, 161.117, 132.454);
	resultIntersect.CurveTo(205.703, 142.52, 239.0, 182.372, 239.0, 230.0);
	resultIntersect.CurveTo(239.0, 248.027, 234.23, 264.94, 225.883, 279.546);
	resultIntersect.CloseFigure();

	resultUnite.StartFigure();
	resultUnite.MoveTo(348.0, 182.0);
	resultUnite.CurveTo(348.0, 237.228, 303.228, 282.0, 248.0, 282.0);
	resultUnite.CurveTo(240.4, 282.0, 232.998, 281.152, 225.883, 279.546);
	resultUnite.CurveTo(208.658, 309.686, 176.201, 330.0, 139.0, 330.0);
	resultUnite.CurveTo(83.7715, 330.0, 39.0, 285.228, 39.0, 230.0);
	resultUnite.CurveTo(39.0, 174.772, 83.7715, 130.0, 139.0, 130.0);
	resultUnite.CurveTo(146.6, 130.0, 154.002, 130.848, 161.117, 132.454);
	resultUnite.CurveTo(178.342, 102.314, 210.799, 82.0, 248.0, 82.0);
	resultUnite.CurveTo(303.228, 82.0, 348.0, 126.772, 348.0, 182.0);
	resultUnite.CloseFigure();

	resultSubtract.StartFigure();
	resultSubtract.MoveTo(348.0, 182.0);
	resultSubtract.CurveTo(348.0, 237.228, 303.228, 282.0, 248.0, 282.0);
	resultSubtract.CurveTo(240.4, 282.0, 232.998, 281.152, 225.883, 279.546);
	resultSubtract.CurveTo(234.23, 264.94, 239.0, 248.027, 239.0, 230.0);
	resultSubtract.CurveTo(239.0, 182.372, 205.703, 142.52, 161.117, 132.454);
	resultSubtract.CurveTo(178.342, 102.314, 210.799, 82.0, 248.0, 82.0);
	resultSubtract.CurveTo(303.228, 82.0, 348.0, 126.772, 348.0, 182.0);
	resultSubtract.CloseFigure();

	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Intersection) == resultIntersect);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Union) == resultUnite);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Subtraction) == resultSubtract);
}

TEST(BooleanOperations, EllipseIntersCross)
{
	Aggplus::CGraphicsPath path1, path2, resultIntersect, resultUnite, resultSubtract;

	path1.AddEllipse(33.0, 60.0, 200.0, 200.0);

	path2.StartFigure();
	path2.MoveTo(147.0, 235.0);
	path2.LineTo(167.5, 235.0);
	path2.LineTo(167.5, 214.5);
	path2.LineTo(188.0, 214.5);
	path2.LineTo(188.0, 235.0);
	path2.LineTo(208.5, 235.0);
	path2.LineTo(208.5, 255.5);
	path2.LineTo(188.0, 255.5);
	path2.LineTo(188.0, 276.0);
	path2.LineTo(167.5, 276.0);
	path2.LineTo(167.5, 255.5);
	path2.LineTo(147.0, 255.5);
	path2.LineTo(147.0, 235.0);
	path2.CloseFigure();

	resultIntersect.StartFigure();
	resultIntersect.MoveTo(199.146, 235);
	resultIntersect.CurveTo(188.719, 244.203, 176.358, 251.266, 162.75, 255.5);
	resultIntersect.LineTo(147, 255.5);
	resultIntersect.LineTo(147, 235);
	resultIntersect.LineTo(167.5, 235);
	resultIntersect.LineTo(167.5, 214.5);
	resultIntersect.LineTo(188, 214.5);
	resultIntersect.LineTo(188, 235);
	resultIntersect.LineTo(199.146, 235);
	resultIntersect.CloseFigure();

	resultUnite.StartFigure();
	resultUnite.MoveTo(233, 160);
	resultUnite.CurveTo(233, 189.867, 219.906, 216.676, 199.146, 235);
	resultUnite.LineTo(208.5, 235);
	resultUnite.LineTo(208.5, 255.5);
	resultUnite.LineTo(188, 255.5);
	resultUnite.LineTo(188, 276);
	resultUnite.LineTo(167.5, 276);
	resultUnite.LineTo(167.5, 255.5);
	resultUnite.LineTo(162.75, 255.5);
	resultUnite.CurveTo(153.353, 258.424, 143.36, 260, 133, 260);
	resultUnite.CurveTo(77.7715, 260, 33, 215.228, 33, 160);
	resultUnite.CurveTo(33, 104.772, 77.7715, 60, 133, 60);
	resultUnite.CurveTo(188.228, 60, 233, 104.772, 233, 160);
	resultUnite.CloseFigure();

	resultSubtract.StartFigure();
	resultSubtract.MoveTo(233, 160);
	resultSubtract.CurveTo(233, 189.867, 219.906, 216.676, 199.146, 235);
	resultSubtract.LineTo(188, 235);
	resultSubtract.LineTo(188, 214.5);
	resultSubtract.LineTo(167.5, 214.5);
	resultSubtract.LineTo(167.5, 235);
	resultSubtract.LineTo(147, 235);
	resultSubtract.LineTo(147, 255.5);
	resultSubtract.LineTo(162.75, 255.5);
	resultSubtract.CurveTo(153.353, 258.424, 143.36, 260, 133, 260);
	resultSubtract.CurveTo(77.7715, 260, 33, 215.228, 33, 160);
	resultSubtract.CurveTo(33, 104.772, 77.7715, 60, 133, 60);
	resultSubtract.CurveTo(188.228, 60, 233, 104.772, 233, 160);
	resultSubtract.CloseFigure();

	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Intersection) == resultIntersect);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Union) == resultUnite);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Subtraction) == resultSubtract);
}

TEST(BooleanOperations, TriangleIntersEllipse)
{
	Aggplus::CGraphicsPath path1, path2, resultIntersect, resultUnite, resultSubtract;

	path1.StartFigure();
	path1.MoveTo(227, 157);
	path1.LineTo(327, 357);
	path1.LineTo(127, 257);
	path1.LineTo(227, 157);
	path1.CloseFigure();

	path2.AddEllipse(64, 119, 200, 200);

	resultIntersect.StartFigure();
	resultIntersect.MoveTo(227, 157);
	resultIntersect.LineTo(263.419, 229.839);
	resultIntersect.CurveTo(260.127, 260.386, 243.078, 286.812, 218.593, 302.797);
	resultIntersect.LineTo(127, 257);
	resultIntersect.LineTo(227, 157);
	resultIntersect.CloseFigure();

	resultUnite.StartFigure();
	resultUnite.MoveTo(263.419, 229.839);
	resultUnite.LineTo(327, 357);
	resultUnite.LineTo(218.593, 302.797);
	resultUnite.CurveTo(202.897, 313.044, 184.144, 319, 164, 319);
	resultUnite.CurveTo(108.772, 319, 64, 274.228, 64, 219);
	resultUnite.CurveTo(64, 163.772, 108.772, 119, 164, 119);
	resultUnite.CurveTo(219.228, 119, 264, 163.772, 264, 219);
	resultUnite.CurveTo(264, 222.662, 263.803, 226.279, 263.419, 229.839);
	resultUnite.CloseFigure();

	resultSubtract.StartFigure();
	resultSubtract.MoveTo(263.419, 229.839);
	resultSubtract.LineTo(327, 357);
	resultSubtract.LineTo(218.593, 302.797);
	resultSubtract.CurveTo(243.078, 286.812, 260.127, 260.386, 263.419, 229.839);
	resultSubtract.CloseFigure();

	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Intersection) == resultIntersect);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Union) == resultUnite);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Subtraction) == resultSubtract);
}

TEST(BooleanOperations, TwoVerticesInters)
{
	Aggplus::CGraphicsPath path1, path2, resultIntersect, resultUnite, resultSubtract;

	path1.StartFigure();
	path1.MoveTo(-300, -300);
	path1.LineTo(-100, -300);
	path1.LineTo(-100, -100);
	path1.LineTo(-300, -100);
	path1.LineTo(-300, -300);
	path1.CloseFigure();

	path2.StartFigure();
	path2.MoveTo(-100, -200);
	path2.LineTo(0, 0);
	path2.LineTo(-200, -100);
	path2.LineTo(-100, -200);
	path2.CloseFigure();

	resultIntersect.StartFigure();
	resultIntersect.MoveTo(-100, -200);
	resultIntersect.LineTo(-100, -100);
	resultIntersect.LineTo(-200, -100);
	resultIntersect.LineTo(-100, -200);
	resultIntersect.CloseFigure();

	resultUnite.StartFigure();
	resultUnite.MoveTo(-300, -300);
	resultUnite.LineTo(-100, -300);
	resultUnite.LineTo(-100, -200);
	resultUnite.LineTo(0, 0);
	resultUnite.LineTo(-200, -100);
	resultUnite.LineTo(-300, -100);
	resultUnite.LineTo(-300, -300);
	resultUnite.CloseFigure();

	resultSubtract.StartFigure();
	resultSubtract.MoveTo(-300, -300);
	resultSubtract.LineTo(-100, -300);
	resultSubtract.LineTo(-100, -200);
	resultSubtract.LineTo(-200, -100);
	resultSubtract.LineTo(-300, -100);
	resultSubtract.LineTo(-300, -300);
	resultSubtract.CloseFigure();

	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Intersection) == resultIntersect);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Union) == resultUnite);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Subtraction) == resultSubtract);
}

TEST(BooleanOperations, RectIntersEllipse)
{
	Aggplus::CGraphicsPath path1, path2, resultIntersect, resultUnite, resultSubtract;

	path1.StartFigure();
	path1.MoveTo(60, 45);
	path1.LineTo(260, 45);
	path1.LineTo(260, 245);
	path1.LineTo(60, 245);
	path1.LineTo(60, 45);
	path1.CloseFigure();

	path2.AddEllipse(138, 147, 128, 128);

	resultIntersect.StartFigure();
	resultIntersect.MoveTo(260, 183.908);
	resultIntersect.LineTo(260, 238.092);
	resultIntersect.CurveTo(258.883, 240.478, 257.623, 242.785, 256.232, 245);
	resultIntersect.LineTo(147.768, 245);
	resultIntersect.CurveTo(141.579, 235.149, 138, 223.493, 138, 211);
	resultIntersect.CurveTo(138, 175.654, 166.654, 147, 202, 147);
	resultIntersect.CurveTo(227.663, 147, 249.798, 162.105, 260, 183.908);
	resultIntersect.CloseFigure();

	resultUnite.StartFigure();
	resultUnite.MoveTo(60, 45);
	resultUnite.LineTo(260, 45);
	resultUnite.LineTo(260, 183.908);
	resultUnite.CurveTo(263.849, 192.136, 266, 201.317, 266, 211);
	resultUnite.CurveTo(266, 220.683, 263.849, 229.864, 260, 238.092);
	resultUnite.LineTo(260, 245);
	resultUnite.LineTo(256.232, 245);
	resultUnite.CurveTo(244.91, 263.021, 224.854, 275, 202, 275);
	resultUnite.CurveTo(179.146, 275, 159.09, 263.021, 147.768, 245);
	resultUnite.LineTo(60, 245);
	resultUnite.LineTo(60, 45);
	resultUnite.CloseFigure();

	resultSubtract.StartFigure();
	resultSubtract.MoveTo(60, 45);
	resultSubtract.LineTo(260, 45);
	resultSubtract.LineTo(260, 183.908);
	resultSubtract.CurveTo(249.798, 162.105, 227.663, 147, 202, 147);
	resultSubtract.CurveTo(166.654, 147, 138, 175.654, 138, 211);
	resultSubtract.CurveTo(138, 223.493, 141.579, 235.149, 147.768, 245);
	resultSubtract.LineTo(60, 245);
	resultSubtract.LineTo(60, 45);
	resultSubtract.MoveTo(260, 238.092);
	resultSubtract.LineTo(260, 245);
	resultSubtract.LineTo(256.232, 245);
	resultSubtract.CurveTo(257.623, 242.785, 258.883, 240.478, 260, 238.092);
	resultSubtract.CloseFigure();

	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Intersection) == resultIntersect);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Union) == resultUnite);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Subtraction) == resultSubtract);
}

TEST(BooleanOperations, RectIntersCross)
{
	Aggplus::CGraphicsPath path1, path2, resultIntersect, resultUnite, resultSubtract;

	path1.StartFigure();
	path1.MoveTo(-147, -176);
	path1.LineTo(53, -176);
	path1.LineTo(53, 24);
	path1.LineTo(-147, 24);
	path1.LineTo(-147, -176);
	path1.CloseFigure();

	path2.StartFigure();
	path2.MoveTo(-171, -111);
	path2.LineTo(-89.5, -111);
	path2.LineTo(-89.5, -192.5);
	path2.LineTo(-8, -192.5);
	path2.LineTo(-8, -111);
	path2.LineTo(73.5, -111);
	path2.LineTo(73.5, -29.5);
	path2.LineTo(-8, -29.5);
	path2.LineTo(-8, 52);
	path2.LineTo(-89.5, 52);
	path2.LineTo(-89.5, -29.5);
	path2.LineTo(-171, -29.5);
	path2.LineTo(-171, -111);
	path2.CloseFigure();

	resultIntersect.StartFigure();
	resultIntersect.MoveTo(-89.5, -176);
	resultIntersect.LineTo(-8, -176);
	resultIntersect.LineTo(-8, -111);
	resultIntersect.LineTo(53, -111);
	resultIntersect.LineTo(53, -29.5);
	resultIntersect.LineTo(-8, -29.5);
	resultIntersect.LineTo(-8, 24);
	resultIntersect.LineTo(-89.5, 24);
	resultIntersect.LineTo(-89.5, -29.5);
	resultIntersect.LineTo(-147, -29.5);
	resultIntersect.LineTo(-147, -111);
	resultIntersect.LineTo(-89.5, -111);
	resultIntersect.LineTo(-89.5, -176);
	resultIntersect.CloseFigure();

	resultUnite.StartFigure();
	resultUnite.MoveTo(-147, -176);
	resultUnite.LineTo(-89.5, -176);
	resultUnite.LineTo(-89.5, -192.5);
	resultUnite.LineTo(-8, -192.5);
	resultUnite.LineTo(-8, -176);
	resultUnite.LineTo(53, -176);
	resultUnite.LineTo(53, -111);
	resultUnite.LineTo(73.5, -111);
	resultUnite.LineTo(73.5, -29.5);
	resultUnite.LineTo(53, -29.5);
	resultUnite.LineTo(53, 24);
	resultUnite.LineTo(-8, 24);
	resultUnite.LineTo(-8, 52);
	resultUnite.LineTo(-89.5, 52);
	resultUnite.LineTo(-89.5, 24);
	resultUnite.LineTo(-147, 24);
	resultUnite.LineTo(-147, -29.5);
	resultUnite.LineTo(-171, -29.5);
	resultUnite.LineTo(-171, -111);
	resultUnite.LineTo(-147, -111);
	resultUnite.LineTo(-147, -176);
	resultUnite.CloseFigure();

	resultSubtract.StartFigure();
	resultSubtract.MoveTo(-147, -176);
	resultSubtract.LineTo(-89.5, -176);
	resultSubtract.LineTo(-89.5, -111);
	resultSubtract.LineTo(-147, -111);
	resultSubtract.LineTo(-147, -176);
	resultSubtract.MoveTo(-8, -176);
	resultSubtract.LineTo(53, -176);
	resultSubtract.LineTo(53, -111);
	resultSubtract.LineTo(-8, -111);
	resultSubtract.LineTo(-8, -176);
	resultSubtract.MoveTo(53, -29.5);
	resultSubtract.LineTo(53, 24);
	resultSubtract.LineTo(-8, 24);
	resultSubtract.LineTo(-8, -29.5);
	resultSubtract.LineTo(53, -29.5);
	resultSubtract.MoveTo(-89.5, 24);
	resultSubtract.LineTo(-147, 24);
	resultSubtract.LineTo(-147, -29.5);
	resultSubtract.LineTo(-89.5, -29.5);
	resultSubtract.LineTo(-89.5, 24);
	resultSubtract.CloseFigure();

	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Intersection) == resultIntersect);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Union) == resultUnite);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Subtraction) == resultSubtract);
}

TEST(BooleanOperations, CrossIntersTriangle)
{
	Aggplus::CGraphicsPath path1, path2, resultIntersect, resultUnite, resultSubtract;

	path1.StartFigure();
	path1.MoveTo(-93, -87);
	path1.LineTo(-2.5, -87);
	path1.LineTo(-2.5, -177.5);
	path1.LineTo(88, -177.5);
	path1.LineTo(88, -87);
	path1.LineTo(178.5, -87);
	path1.LineTo(178.5, 3.5);
	path1.LineTo(88, 3.5);
	path1.LineTo(88, 94);
	path1.LineTo(-2.5, 94);
	path1.LineTo(-2.5, 3.5);
	path1.LineTo(-93, 3.5);
	path1.LineTo(-93, -87);
	path1.CloseFigure();

	path2.StartFigure();
	path2.MoveTo(36, -193);
	path2.LineTo(181, 97);
	path2.LineTo(-109, -48);
	path2.LineTo(36, -193);
	path2.CloseFigure();

	resultIntersect.StartFigure();
	resultIntersect.MoveTo(-70, -87);
	resultIntersect.LineTo(-2.5, -87);
	resultIntersect.LineTo(-2.5, -154.5);
	resultIntersect.LineTo(20.5, -177.5);
	resultIntersect.LineTo(43.75, -177.5);
	resultIntersect.LineTo(88, -89);
	resultIntersect.LineTo(88, -87);
	resultIntersect.LineTo(89, -87);
	resultIntersect.LineTo(134.25, 3.5);
	resultIntersect.LineTo(88, 3.5);
	resultIntersect.LineTo(88, 50.5);
	resultIntersect.LineTo(-2.5, 5.25);
	resultIntersect.LineTo(-2.5, 3.5);
	resultIntersect.LineTo(-6, 3.5);
	resultIntersect.LineTo(-93, -40);
	resultIntersect.LineTo(-93, -64);
	resultIntersect.LineTo(-70, -87);
	resultIntersect.CloseFigure();

	resultUnite.StartFigure();
	resultUnite.MoveTo(-93, -87);
	resultUnite.LineTo(-70, -87);
	resultUnite.LineTo(-2.5, -154.5);
	resultUnite.LineTo(-2.5, -177.5);
	resultUnite.LineTo(20.5, -177.5);
	resultUnite.LineTo(36, -193);
	resultUnite.LineTo(43.75, -177.5);
	resultUnite.LineTo(88, -177.5);
	resultUnite.LineTo(88, -89);
	resultUnite.LineTo(89, -87);
	resultUnite.LineTo(178.5, -87);
	resultUnite.LineTo(178.5, 3.5);
	resultUnite.LineTo(134.25, 3.5);
	resultUnite.LineTo(181, 97);
	resultUnite.LineTo(88, 50.5);
	resultUnite.LineTo(88, 94);
	resultUnite.LineTo(-2.5, 94);
	resultUnite.LineTo(-2.5, 5.25);
	resultUnite.LineTo(-6, 3.5);
	resultUnite.LineTo(-93, 3.5);
	resultUnite.LineTo(-93, -40);
	resultUnite.LineTo(-109, -48);
	resultUnite.LineTo(-93, -64);
	resultUnite.LineTo(-93, -87);
	resultUnite.CloseFigure();

	resultSubtract.StartFigure();
	resultSubtract.MoveTo(-93, -87);
	resultSubtract.LineTo(-70, -87);
	resultSubtract.LineTo(-93, -64);
	resultSubtract.LineTo(-93, -87);
	resultSubtract.MoveTo(-2.5, -154.5);
	resultSubtract.LineTo(-2.5, -177.5);
	resultSubtract.LineTo(20.5, -177.5);
	resultSubtract.LineTo(-2.5, -154.5);
	resultSubtract.MoveTo(43.75, -177.5);
	resultSubtract.LineTo(88, -177.5);
	resultSubtract.LineTo(88, -89);
	resultSubtract.LineTo(43.75, -177.5);
	resultSubtract.MoveTo(89, -87);
	resultSubtract.LineTo(178.5, -87);
	resultSubtract.LineTo(178.5, 3.5);
	resultSubtract.LineTo(134.25, 3.5);
	resultSubtract.LineTo(89, -87);
	resultSubtract.MoveTo(88, 50.5);
	resultSubtract.LineTo(88, 94);
	resultSubtract.LineTo(-2.5, 94);
	resultSubtract.LineTo(-2.5, 5.25);
	resultSubtract.LineTo(88, 50.5);
	resultSubtract.MoveTo(-6, 3.5);
	resultSubtract.LineTo(-93, 3.5);
	resultSubtract.LineTo(-93, -40);
	resultSubtract.LineTo(-6, 3.5);
	resultSubtract.CloseFigure();

	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Intersection) == resultIntersect);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Union) == resultUnite);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Subtraction) == resultSubtract);
}

TEST(BooleanOperations, CrossIntersCross)
{
	Aggplus::CGraphicsPath path1, path2, resultIntersect, resultUnite, resultSubtract;

	path1.StartFigure();
	path1.MoveTo(-161, -129);
	path1.LineTo(-99, -129);
	path1.LineTo(-99, -191);
	path1.LineTo(-37, -191);
	path1.LineTo(-37, -129);
	path1.LineTo(25, -129);
	path1.LineTo(25, -67);
	path1.LineTo(-37, -67);
	path1.LineTo(-37, -5);
	path1.LineTo(-99, -5);
	path1.LineTo(-99, -67);
	path1.LineTo(-161, -67);
	path1.LineTo(-161, -129);
	path1.CloseFigure();

	path2.StartFigure();
	path2.MoveTo(-172, -191);
	path2.LineTo(-122, -191);
	path2.LineTo(-122, -241);
	path2.LineTo(-72, -241);
	path2.LineTo(-72, -191);
	path2.LineTo(-22, -191);
	path2.LineTo(-22, -141);
	path2.LineTo(-72, -141);
	path2.LineTo(-72, -91);
	path2.LineTo(-122, -91);
	path2.LineTo(-122, -141);
	path2.LineTo(-172, -141);
	path2.LineTo(-172, -191);
	path2.CloseFigure();

	resultIntersect.StartFigure();
	resultIntersect.MoveTo(-122, -129);
	resultIntersect.LineTo(-99, -129);
	resultIntersect.LineTo(-99, -191);
	resultIntersect.LineTo(-72, -191);
	resultIntersect.LineTo(-37, -191);
	resultIntersect.LineTo(-37, -141);
	resultIntersect.LineTo(-72, -141);
	resultIntersect.LineTo(-72, -91);
	resultIntersect.LineTo(-122, -91);
	resultIntersect.LineTo(-122, -129);
	resultIntersect.CloseFigure();

	resultUnite.StartFigure();
	resultUnite.MoveTo(-161, -129);
	resultUnite.LineTo(-122, -129);
	resultUnite.LineTo(-122, -141);
	resultUnite.LineTo(-172, -141);
	resultUnite.LineTo(-172, -191);
	resultUnite.LineTo(-122, -191);
	resultUnite.LineTo(-122, -241);
	resultUnite.LineTo(-72, -241);
	resultUnite.LineTo(-72, -191);
	resultUnite.LineTo(-37, -191);
	resultUnite.LineTo(-22, -191);
	resultUnite.LineTo(-22, -141);
	resultUnite.LineTo(-37, -141);
	resultUnite.LineTo(-37, -129);
	resultUnite.LineTo(25, -129);
	resultUnite.LineTo(25, -67);
	resultUnite.LineTo(-37, -67);
	resultUnite.LineTo(-37, -5);
	resultUnite.LineTo(-99, -5);
	resultUnite.LineTo(-99, -67);
	resultUnite.LineTo(-161, -67);
	resultUnite.LineTo(-161, -129);
	resultUnite.CloseFigure();

	resultSubtract.StartFigure();
	resultSubtract.MoveTo(-161, -129);
	resultSubtract.LineTo(-122, -129);
	resultSubtract.LineTo(-122, -91);
	resultSubtract.LineTo(-72, -91);
	resultSubtract.LineTo(-72, -141);
	resultSubtract.LineTo(-37, -141);
	resultSubtract.LineTo(-37, -129);
	resultSubtract.LineTo(25, -129);
	resultSubtract.LineTo(25, -67);
	resultSubtract.LineTo(-37, -67);
	resultSubtract.LineTo(-37, -5);
	resultSubtract.LineTo(-99, -5);
	resultSubtract.LineTo(-99, -67);
	resultSubtract.LineTo(-161, -67);
	resultSubtract.LineTo(-161, -129);
	resultSubtract.MoveTo(-72, -191);
	resultSubtract.LineTo(-37, -191);
	resultSubtract.LineTo(-72, -191);
	resultSubtract.CloseFigure();

	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Intersection) == resultIntersect);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Union) == resultUnite);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Subtraction) == resultSubtract);
}

TEST(BooleanOperations, EllipseTouchEllipse)
{
	Aggplus::CGraphicsPath path1, path2, resultIntersect, resultUnite, resultSubtract;

	path1.AddEllipse(43, 88, 200, 200);

	path2.AddEllipse(45, 117, 120, 120);

	resultIntersect.StartFigure();
	resultIntersect.MoveTo(165, 177);
	resultIntersect.CurveTo(165, 210.137, 138.137, 237, 105, 237);
	resultIntersect.CurveTo(71.8629, 237, 45, 210.137, 45, 177);
	resultIntersect.CurveTo(45, 143.863, 71.8629, 117, 105, 117);
	resultIntersect.CurveTo(138.137, 117, 165, 143.863, 165, 177);
	resultIntersect.CloseFigure();

	resultUnite.StartFigure();
	resultUnite.MoveTo(243, 188);
	resultUnite.CurveTo(243, 243.228, 198.228, 288, 143, 288);
	resultUnite.CurveTo(87.7715, 288, 43, 243.228, 43, 188);
	resultUnite.CurveTo(43, 132.772, 87.7715, 88, 143, 88);
	resultUnite.CurveTo(198.228, 88, 243, 132.772, 243, 188);
	resultUnite.CloseFigure();

	resultSubtract.StartFigure();
	resultSubtract.MoveTo(243, 188);
	resultSubtract.CurveTo(243, 243.228, 198.228, 288, 143, 288);
	resultSubtract.CurveTo(87.7715, 288, 43, 243.228, 43, 188);
	resultSubtract.CurveTo(43, 132.772, 87.7715, 88, 143, 88);
	resultSubtract.CurveTo(198.228, 88, 243, 132.772, 243, 188);
	resultSubtract.MoveTo(165, 177);
	resultSubtract.CurveTo(165, 143.863, 138.137, 117, 105, 117);
	resultSubtract.CurveTo(71.8629, 117, 45, 143.863, 45, 177);
	resultSubtract.CurveTo(45, 210.137, 71.8629, 237, 105, 237);
	resultSubtract.CurveTo(138.137, 237, 165, 210.137, 165, 177);
	resultSubtract.CloseFigure();

	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Intersection) == resultIntersect);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Union) == resultUnite);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Subtraction) == resultSubtract);
}

TEST(BooleanOperations, TriangleOverlapTriangle)
{
	Aggplus::CGraphicsPath path1, path2, resultIntersect, resultUnite, resultSubtract;

	path1.StartFigure();
	path1.MoveTo(-200, -300);
	path1.LineTo(-100, -100);
	path1.LineTo(-300, -200);
	path1.LineTo(-200, -300);
	path1.CloseFigure();

	path2.StartFigure();
	path2.MoveTo(-181, -291);
	path2.LineTo(-81, -91);
	path2.LineTo(-281, -191);
	path2.LineTo(-181, -291);
	path2.CloseFigure();

	resultIntersect.StartFigure();
	resultIntersect.MoveTo(-190.667, -281.333);
	resultIntersect.LineTo(-100.333, -100.667);
	resultIntersect.LineTo(-281, -191);
	resultIntersect.LineTo(-190.667, -281.333);
	resultIntersect.CloseFigure();

	resultUnite.StartFigure();
	resultUnite.MoveTo(-200, -300);
	resultUnite.LineTo(-190.667, -281.333);
	resultUnite.LineTo(-181, -291);
	resultUnite.LineTo(-81, -91);
	resultUnite.LineTo(-100.333, -100.667);
	resultUnite.LineTo(-100, -100);
	resultUnite.LineTo(-300, -200);
	resultUnite.LineTo(-200, -300);
	resultUnite.CloseFigure();

	resultSubtract.StartFigure();
	resultSubtract.MoveTo(-200, -300);
	resultSubtract.LineTo(-190.667, -281.333);
	resultSubtract.LineTo(-281, -191);
	resultSubtract.LineTo(-100.333, -100.667);
	resultSubtract.LineTo(-100, -100);
	resultSubtract.LineTo(-300, -200);
	resultSubtract.LineTo(-200, -300);
	resultSubtract.CloseFigure();

	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Intersection) == resultIntersect);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Union) == resultUnite);
	EXPECT_TRUE(Aggplus::CalcBooleanOperation(path1, path2, Aggplus::Subtraction) == resultSubtract);
}
