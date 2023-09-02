#include "cstarmathpars.h"
#include "cconversionsmtoooxml.h"

int main()
{
	std::wstring Temp = L"2+1";
	StarMath::CStarMathPars TempO;
	TempO.Pars(Temp);
	ConversionSM2OOXML::CConversionSMtoOOXML m_oTest;
	m_oTest.StartConversion(TempO.GetVector());
}
