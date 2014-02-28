#include "precompiled_cpodf.h"

#include "ods_table_context.h"
//#include "ods_textcontext.h"
#include "ods_conversion_context.h"
#include "logging.h"

#include <boost/foreach.hpp>
#include <iostream>

namespace cpdoccore {
namespace odf {

ods_table_context::ods_table_context(ods_conversion_context & Context/*, xlsx_text_context & textCotnext*/): context_(Context)
{        
}

ods_table_state & ods_table_context::state()
{
    return table_state_list_.back();
}

const ods_table_state & ods_table_context::state() const
{
    return table_state_list_.back();
}

void ods_table_context::start_table(office_element_ptr & elm)
{
	table_state_list_.push_back( ods_table_state(context_, elm) );
}

void ods_table_context::end_table()
{
}
}
}