#include "cstarmathpars.h"

int main()
{
	std::wstring Temp = L"lim from color red bold {7 over 5 over 3} to 56 721 ";
	StarMath::CStarMathPars TempO;
	TempO.Pars(Temp);
}
