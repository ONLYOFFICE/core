// MathEquationTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "..\MathEquation.h"
#include "..\StringOutput.h"
using namespace MathEquation;

int main()
{
	CStringOutput oStringOutput;
	CEquationReader oReader(_T("..\\Examples\\oleObject60.bin"));
	oReader.SetOutputDev(&oStringOutput);
	oReader.Parse();

	return 0;
}
