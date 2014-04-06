#include "precompiled_cpodf.h"
#include "xlsx_cell_format.h"

#include <boost/foreach.hpp>

namespace cpdoccore {
namespace oox {

xlsx_cell_format::xlsx_cell_format() : cell_type_(XlsxCellType::null)
{}

XlsxCellType::type xlsx_cell_format::get_cell_type() const
{
    return cell_type_;
}

void xlsx_cell_format::set_cell_type(XlsxCellType::type type)
{
    cell_type_ = type;
}

int xlsx_cell_format::get_num_format() const
{
    return num_format_;
}

void xlsx_cell_format::set_num_format(int numFmt)
{
    num_format_ = numFmt;
}

namespace {

	int odf2buildin_map[] = {0,0,0,0,0,0,0,0};
//{
//    {L"", 0},
//    {L"float", 0},
//    {L"currency", 0},
//    {L"percentage", 10},
//    {L"date", 15},
//    {L"time", 21},
//    {L"boolean", 0},
//    {L"string", 49}
//};
//    
}

int odf_string_to_build_in(const int odf_type_value)
{
    return odf2buildin_map[odf_type_value];    
}

}
}
