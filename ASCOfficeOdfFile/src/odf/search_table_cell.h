#ifndef _CPDOCCORE_SEARCH_TABLE_CELL_H_
#define _CPDOCCORE_SEARCH_TABLE_CELL_H_

#include <string>

namespace cpdoccore { 
namespace odf {

class office_element;

const office_element * search_table_cell(const office_element * base, std::wstring const & tableName, std::wstring const & ref,
                                   std::wstring & defaultColumnStyle, std::wstring & defaultRowStyle);

}
}

#endif


