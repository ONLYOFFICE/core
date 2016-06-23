#pragma once 

#include <iosfwd>
#include <cpdoccore/CPOptional.h>

                 
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

struct _oox_strRef
{
	bool						present;

	std::wstring				formula;
	int							str_cache_count;
	std::vector<std::wstring>	str_cache;

};
struct _oox_numRef
{
	bool						present;

	std::wstring				formula;
	std::wstring				formatCode;
	bool						link_to_source;
	int							num_cache_count;
	std::vector<std::wstring>	num_cache;

};
//extLst (Chart Extensibility) §21.2.2.64
//f (Formula) §21.2.2.65
//numCache (Number Cache)

struct _oox_chart_values
{
public:
	_oox_chart_values()
	{
		numRef_.present			= false;
		numRef_.formatCode		= L"General";
		numRef_.num_cache_count	= 0;
		numRef_.link_to_source	= true;

		strRef_.present			= false;
		strRef_.str_cache_count	= 0;

		present					= false;
	}

	bool			present;
	std::wstring	type;
	
	_oox_numRef		numRef_;
	_oox_strRef		strRef_;

};
//numLit (Number Literal) §21.2.2.122
//numRef (Number Reference) §21.2.2.123
}
}