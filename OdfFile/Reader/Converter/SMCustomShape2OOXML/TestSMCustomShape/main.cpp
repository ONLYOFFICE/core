#include "gtest/gtest.h"
#include "../smcustomshapepars.h"

TEST(SMCustomShapeTest,Number)
{
	std::wstring wsString =L"45";
    OdfCustomShape::SMCustomShapePars oPars;
    OdfCustomShape::SMCustomShapeConversion oConvers;
	oPars.StartParsSMCustomShape(wsString);
	oConvers.StartConversion(oPars.GetVector(),L"gd35");
	std::wstring wsXmlString = L"<a:gd name=\"gd35\" fmla=\"val 45 \" />";
	EXPECT_EQ(oConvers.GetStringXml(),wsXmlString);
}
TEST(SMCustomShapeTest,Plus)
{
	std::wstring wsString =L"left+?f2";
    OdfCustomShape::SMCustomShapePars oPars;
    OdfCustomShape::SMCustomShapeConversion oConvers;
	oPars.StartParsSMCustomShape(wsString);
	oConvers.StartConversion(oPars.GetVector(),L"gd35");
	std::wstring wsXmlString = L"<a:gd name=\"gd35\" fmla=\"+/ 0 gd2 1 \" />";
	EXPECT_EQ(oConvers.GetStringXml(),wsXmlString);
}
TEST(SMCustomShapeTest,Minus)
{
	std::wstring wsString =L"bottom-?f2 ";
    OdfCustomShape::SMCustomShapePars oPars;
    OdfCustomShape::SMCustomShapeConversion oConvers;
	oPars.StartParsSMCustomShape(wsString);
	oConvers.StartConversion(oPars.GetVector(),L"gd35");
	std::wstring wsXmlString = L"<a:gd name=\"gd35\" fmla=\"+- 0 h gd2 \" />";
	EXPECT_EQ(oConvers.GetStringXml(),wsXmlString);
}
TEST(SMCustomShapeTest,Multi)
{
	std::wstring wsString =L"?f1 *3163/7636";
    OdfCustomShape::SMCustomShapePars oPars;
    OdfCustomShape::SMCustomShapeConversion oConvers;
	oPars.StartParsSMCustomShape(wsString);
	oConvers.StartConversion(oPars.GetVector(),L"gd35");
	std::wstring wsXmlString = L"<a:gd name=\"gd35\" fmla=\"*/ gd1 3163 7636 \" />";
	EXPECT_EQ(oConvers.GetStringXml(),wsXmlString);
}
TEST(SMCustomShapeTest,Divide)
{
	std::wstring wsString =L"10800/cos(pi/8)";
    OdfCustomShape::SMCustomShapePars oPars;
    OdfCustomShape::SMCustomShapeConversion oConvers;
	oPars.StartParsSMCustomShape(wsString);
	oConvers.StartConversion(oPars.GetVector(),L"gd35");
	std::wstring wsXmlString = L"<a:gd name=\"gd35.1.1.1\" fmla=\"+/ 0 314 100\" /><a:gd name=\"gd35.1.1\" fmla=\"+/ 0 gd35.1.1.1 8 \" /><a:gd name=\"gd35.1\" fmla=\"cos 1 gd35.1.1 \" /><a:gd name=\"gd35\" fmla=\"+/ 0 10800 gd35.1 \" />";
	EXPECT_EQ(oConvers.GetStringXml(),wsXmlString);
}
TEST(SMCustomShapeTest,Sqrt)
{
	std::wstring wsString =L"*sqrt(2)/2";
    OdfCustomShape::SMCustomShapePars oPars;
    OdfCustomShape::SMCustomShapeConversion oConvers;
	oPars.StartParsSMCustomShape(wsString);
	oConvers.StartConversion(oPars.GetVector(),L"gd35");
	std::wstring wsXmlString = L"<a:gd name=\"gd35.1\" fmla=\"sqrt 2 \" /><a:gd name=\"gd35\" fmla=\"*/ 0 gd35.1 2 \" />";
	EXPECT_EQ(oConvers.GetStringXml(),wsXmlString);
}
TEST(SMCustomShapeTest,DoubleBracket)
{
	std::wstring wsString =L"$0 *sin(?f0 *(pi/180))";
    OdfCustomShape::SMCustomShapePars oPars;
    OdfCustomShape::SMCustomShapeConversion oConvers;
	oPars.StartParsSMCustomShape(wsString);
	oConvers.StartConversion(oPars.GetVector(),L"gd35");
	std::wstring wsXmlString = L"<a:gd name=\"gd35.1.1.1.1\" fmla=\"+/ 0 314 100\" /><a:gd name=\"gd35.1.1.1\" fmla=\"+/ 0 gd35.1.1.1.1 180 \" /><a:gd name=\"gd35.1.1\" fmla=\"*/ gd0 gd35.1.1.1 1 \" /><a:gd name=\"gd35.1\" fmla=\"sin 1 gd35.1.1 \" /><a:gd name=\"gd35\" fmla=\"*/ $0 gd35.1 1 \" />";
	EXPECT_EQ(oConvers.GetStringXml(),wsXmlString);
}
TEST(SMCustomShapeTest,Min)
{
	std::wstring wsString =L"min(?f36+25,?f37/2)";
    OdfCustomShape::SMCustomShapePars oPars;
    OdfCustomShape::SMCustomShapeConversion oConvers;
	oPars.StartParsSMCustomShape(wsString);
	oConvers.StartConversion(oPars.GetVector(),L"gd35");
	std::wstring wsXmlString = L"<a:gd name=\"gd35.1\" fmla=\"+/ gd36 25 1 \" /><a:gd name=\"gd35.2\" fmla=\"+/ 0 gd37 2 \" /><a:gd name=\"gd35\" fmla=\"min gd35.1 gd35.2 \" />";
	EXPECT_EQ(oConvers.GetStringXml(),wsXmlString);
}
TEST(SMCustomShapeTest,Max)
{
	std::wstring wsString =L"max(?f36,2)";
    OdfCustomShape::SMCustomShapePars oPars;
    OdfCustomShape::SMCustomShapeConversion oConvers;
	oPars.StartParsSMCustomShape(wsString);
	oConvers.StartConversion(oPars.GetVector(),L"gd35");
	std::wstring wsXmlString = L"<a:gd name=\"gd35\" fmla=\"max gd36 2 \" />";
	EXPECT_EQ(oConvers.GetStringXml(),wsXmlString);
}
TEST(SMCustomShapeTest,Abs)
{
	std::wstring wsString =L"abs(?f2+?f3)";
    OdfCustomShape::SMCustomShapePars oPars;
    OdfCustomShape::SMCustomShapeConversion oConvers;
	oPars.StartParsSMCustomShape(wsString);
	oConvers.StartConversion(oPars.GetVector(),L"gd35");
	std::wstring wsXmlString = L"<a:gd name=\"gd35.1\" fmla=\"+/ gd2 gd3 1 \" /><a:gd name=\"gd35\" fmla=\"abs gd35.1 \" />";
	EXPECT_EQ(oConvers.GetStringXml(),wsXmlString);
}
TEST(SMCustomShapeTest,Sin)
{
	std::wstring wsString =L"sin(26500)";
    OdfCustomShape::SMCustomShapePars oPars;
    OdfCustomShape::SMCustomShapeConversion oConvers;
	oPars.StartParsSMCustomShape(wsString);
	oConvers.StartConversion(oPars.GetVector(),L"gd35");
	std::wstring wsXmlString = L"<a:gd name=\"gd35\" fmla=\"sin 1 26500 \" />";
	EXPECT_EQ(oConvers.GetStringXml(),wsXmlString);
}
TEST(SMCustomShapeTest,Cos)
{
	std::wstring wsString =L"cos(45/?f40)";
    OdfCustomShape::SMCustomShapePars oPars;
    OdfCustomShape::SMCustomShapeConversion oConvers;
	oPars.StartParsSMCustomShape(wsString);
	oConvers.StartConversion(oPars.GetVector(),L"gd35");
	std::wstring wsXmlString = L"<a:gd name=\"gd35.1\" fmla=\"+/ 0 45 gd40 \" /><a:gd name=\"gd35\" fmla=\"cos 1 gd35.1 \" />";
	EXPECT_EQ(oConvers.GetStringXml(),wsXmlString);
}
TEST(SMCustomShapeTest,IF)
{
	std::wstring wsString =L"if(2+?f1,26500/sqrt(2),min(?f35,?f37))";
    OdfCustomShape::SMCustomShapePars oPars;
    OdfCustomShape::SMCustomShapeConversion oConvers;
	oPars.StartParsSMCustomShape(wsString);
	oConvers.StartConversion(oPars.GetVector(),L"gd35");
	std::wstring wsXmlString = L"<a:gd name=\"gd35.1\" fmla=\"+/ 2 gd1 1 \" /><a:gd name=\"gd35.2.1\" fmla=\"sqrt 2 \" /><a:gd name=\"gd35.2\" fmla=\"+/ 0 26500 gd35.2.1 \" /><a:gd name=\"gd35.3\" fmla=\"min gd35 gd37 \" /><a:gd name=\"gd35\" fmla=\"?: gd35.1 gd35.2 gd35.3 \" />";
	EXPECT_EQ(oConvers.GetStringXml(),wsXmlString);
}
TEST(SMCustomShapeTest,Tan)
{
	std::wstring wsString =L"tan(?f40*?f41)";
    OdfCustomShape::SMCustomShapePars oPars;
    OdfCustomShape::SMCustomShapeConversion oConvers;
	oPars.StartParsSMCustomShape(wsString);
	oConvers.StartConversion(oPars.GetVector(),L"gd35");
	std::wstring wsXmlString = L"<a:gd name=\"gd35.1\" fmla=\"*/ gd40 gd41 1 \" /><a:gd name=\"gd35\" fmla=\"tan 1 gd35.1 \" />";
	EXPECT_EQ(oConvers.GetStringXml(),wsXmlString);
}
TEST(SMCustomShapeTest,ComplexEquation)
{
	std::wstring wsString =L"sqrt(?f36 * ?f36 + ?f39 * ?f39 + 0 * 0)";
    OdfCustomShape::SMCustomShapePars oPars;
    OdfCustomShape::SMCustomShapeConversion oConvers;
	oPars.StartParsSMCustomShape(wsString);
	oConvers.StartConversion(oPars.GetVector(),L"gd35");
	std::wstring wsXmlString = L"<a:gd name=\"gd35.1.1.1\" fmla=\"*/ gd36 gd36 1 \" /><a:gd name=\"gd35.1.1.2\" fmla=\"*/ gd39 gd39 1 \" /><a:gd name=\"gd35.1.1\" fmla=\"+/ gd35.1.1.1 gd35.1.1.2 1 \" /><a:gd name=\"gd35.1.2\" fmla=\"*/ 0 0 1 \" /><a:gd name=\"gd35.1\" fmla=\"+/ gd35.1.1 gd35.1.2 1 \" /><a:gd name=\"gd35\" fmla=\"sqrt gd35.1 \" />";
	EXPECT_EQ(oConvers.GetStringXml(),wsXmlString);
}
TEST(SMCustomShapeTest,Atan)
{
	std::wstring wsString =L"atan(25/5)";
    OdfCustomShape::SMCustomShapePars oPars;
    OdfCustomShape::SMCustomShapeConversion oConvers;
	oPars.StartParsSMCustomShape(wsString);
	oConvers.StartConversion(oPars.GetVector(),L"gd35");
	std::wstring wsXmlString = L"<a:gd name=\"gd35.1\" fmla=\"+/ 0 25 5 \" /><a:gd name=\"gd35\" fmla=\"at2 1 gd35.1 \" />";
	EXPECT_EQ(oConvers.GetStringXml(),wsXmlString);
}
TEST(SMCustomShapeTest,PlusSecondMinusEmpty)
{
	std::wstring wsString =L"+1-";
    OdfCustomShape::SMCustomShapePars oPars;
    OdfCustomShape::SMCustomShapeConversion oConvers;
	oPars.StartParsSMCustomShape(wsString);
	oConvers.StartConversion(oPars.GetVector(),L"gd35");
	std::wstring wsXmlString = L"<a:gd name=\"gd35\" fmla=\"+- 0 1 0 \" />";
	EXPECT_EQ(oConvers.GetStringXml(),wsXmlString);
}
TEST(SMCustomShapeTest,SinWithoutBracket)
{
	std::wstring wsString =L"sin";
    OdfCustomShape::SMCustomShapePars oPars;
    OdfCustomShape::SMCustomShapeConversion oConvers;
	oPars.StartParsSMCustomShape(wsString);
	oConvers.StartConversion(oPars.GetVector(),L"gd35");
	std::wstring wsXmlString = L"<a:gd name=\"gd35\" fmla=\"sqrt 0 \" />";
	EXPECT_EQ(oConvers.GetStringXml(),wsXmlString);
}
TEST(SMCustomShapeTest,SinEmpty)
{
	std::wstring wsString =L"sin()";
    OdfCustomShape::SMCustomShapePars oPars;
    OdfCustomShape::SMCustomShapeConversion oConvers;
	oPars.StartParsSMCustomShape(wsString);
	oConvers.StartConversion(oPars.GetVector(),L"gd35");
	std::wstring wsXmlString = L"<a:gd name=\"gd35\" fmla=\"sin 1 0 \" />";
	EXPECT_EQ(oConvers.GetStringXml(),wsXmlString);
}
TEST(SMCustomShapeTest,MaxEmpty)
{
	std::wstring wsString =L"max()";
    OdfCustomShape::SMCustomShapePars oPars;
    OdfCustomShape::SMCustomShapeConversion oConvers;
	oPars.StartParsSMCustomShape(wsString);
	oConvers.StartConversion(oPars.GetVector(),L"gd35");
	std::wstring wsXmlString = L"<a:gd name=\"gd35\" fmla=\"max 0 0 \" />";
	EXPECT_EQ(oConvers.GetStringXml(),wsXmlString);
}
TEST(SMCustomShapeTest,IfEmpty)
{
	std::wstring wsString =L"if()";
    OdfCustomShape::SMCustomShapePars oPars;
    OdfCustomShape::SMCustomShapeConversion oConvers;
	oPars.StartParsSMCustomShape(wsString);
	oConvers.StartConversion(oPars.GetVector(),L"gd35");
	std::wstring wsXmlString = L"<a:gd name=\"gd35\" fmla=\"?: 0 0 0 \" />";
	EXPECT_EQ(oConvers.GetStringXml(),wsXmlString);
}
TEST(SMCustomShapeTest,AbsEmpty)
{
	std::wstring wsString =L"abs()";
    OdfCustomShape::SMCustomShapePars oPars;
    OdfCustomShape::SMCustomShapeConversion oConvers;
	oPars.StartParsSMCustomShape(wsString);
	oConvers.StartConversion(oPars.GetVector(),L"gd35");
	std::wstring wsXmlString = L"<a:gd name=\"gd35\" fmla=\"abs 0 \" />";
	EXPECT_EQ(oConvers.GetStringXml(),wsXmlString);
}
TEST(SMCustomShapeTest,PlusFirstMinusEmpty)
{
	std::wstring wsString =L"1+-";
    OdfCustomShape::SMCustomShapePars oPars;
    OdfCustomShape::SMCustomShapeConversion oConvers;
	oPars.StartParsSMCustomShape(wsString);
	oConvers.StartConversion(oPars.GetVector(),L"gd35");
	std::wstring wsXmlString = L"<a:gd name=\"gd35\" fmla=\"+- 1 0 0 \" />";
	EXPECT_EQ(oConvers.GetStringXml(),wsXmlString);
}
