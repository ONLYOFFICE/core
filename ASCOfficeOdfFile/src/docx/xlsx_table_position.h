#ifndef CPDOCCORE_XLSX_TABLE_POSITION_
#define CPDOCCORE_XLSX_TABLE_POSITION_

namespace cpdoccore {
namespace oox {

struct xlsx_table_position
{
    size_t col;    
    double colOff;
    size_t row;
    double rowOff;
};


}
}

#endif
