#pragma once

#include <cpdoccore/CPOptional.h>
#include <cpdoccore/CPScopedPtr.h>
#include <string>

namespace cpdoccore {
namespace oox {

class xlsx_conditionalFormatting_context
{
public:
    xlsx_conditionalFormatting_context();
    ~xlsx_conditionalFormatting_context();

	void add(std::wstring ref);

	void add_rule(int type);

	void set_formula(std::wstring f);
	void set_dataBar(_CP_OPT(int) min, _CP_OPT(int) max);

	void set_dxf	(int dxf_id);
	void set_showVal(bool val);

	void add_sfv	(int type, std::wstring value);
	void add_color	(std::wstring col);

    void serialize(std::wostream & _Wostream);
private:
    class Impl;
    _CP_SCOPED_PTR(Impl) impl_;
};

}
}
