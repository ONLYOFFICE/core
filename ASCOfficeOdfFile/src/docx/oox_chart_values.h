#pragma once 

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
//#include <cpdoccore/CPScopedPtr.h>

                 
namespace cpdoccore {
namespace oox {

struct _oox_numLit
{
	bool present;

};
//extLst (Chart Extensibility) §21.2.2.64
//formatCode (Format Code) §21.2.2.71
//pt (Numeric Point) §21.2.2.150
//ptCount (Point Count)

//extLst (Chart Extensibility) §21.2.2.64
//formatCode (Format Code) §21.2.2.71
//pt (Numeric Point) §21.2.2.150
//ptCount (Point Count)
//struct _xlsx_num_cash
//{
//	int formatCode;
//	std::vector<std::wstring>num_cash;
//};
struct _oox_strRef
{
	bool present;

	std::wstring formula;
	int str_cash_count;
	std::vector<std::wstring>str_cash;

};
struct _oox_numRef
{
	bool present;

	std::wstring formula;
	std::wstring formatCode;
	int num_cash_count;
	std::vector<std::wstring>num_cash;

};
//extLst (Chart Extensibility) §21.2.2.64
//f (Formula) §21.2.2.65
//numCache (Number Cache)

struct _oox_chart_values
{
public:
	_oox_chart_values()
	{
		numLit_.present =false;
		numRef_.present=false;
		numRef_.formatCode = L"General";
		numRef_.num_cash_count=0;

		strRef_.present=false;
		strRef_.str_cash_count=0;

		present = false;
	}

	bool present;
	std::wstring type;
	
	_oox_numLit numLit_;
	_oox_numRef numRef_;
	_oox_strRef strRef_;

};
//numLit (Number Literal) §21.2.2.122
//numRef (Number Reference) §21.2.2.123
}
}