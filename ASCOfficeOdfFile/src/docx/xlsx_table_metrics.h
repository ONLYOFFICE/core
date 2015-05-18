#pragma once

#include <boost/noncopyable.hpp>
#include <cpdoccore/CPSharedPtr.h>

#include "xlsx_table_position.h"

namespace cpdoccore {
namespace oox 
{

/// \class  xlsx_table_metrics
class xlsx_table_metrics
{
public:
    xlsx_table_metrics();
    ~xlsx_table_metrics();

public:
    xlsx_table_position calc(double x_pt, double y_pt);
    xlsx_table_position calc(size_t offset_col,size_t offset_row,double x_pt, double y_pt);
	xlsx_table_position calc(size_t last_col,size_t last_row);

	void update_pt(size_t offset_col,size_t offset_row,double &x_pt, double &y_pt);

    void add_cols(size_t count, double widht_pt);
    void add_rows(size_t count, double height_pt);

private:
    class Impl;
    _CP_PTR(Impl) impl_;
    
};

}
}
