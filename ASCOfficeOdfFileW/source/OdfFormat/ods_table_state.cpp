#include "precompiled_cpodf.h"
#include "logging.h"

#include "ods_table_state.h"
#include "ods_conversion_context.h"

#include "table.h"

namespace cpdoccore {
namespace odf {

ods_table_state::ods_table_state(ods_conversion_context & Context, office_element_ptr & elm): context_(Context)   
{        
	office_element_ = (table_table*)dynamic_cast<table_table*>(elm.get());

}

void ods_table_state::set_name(std::wstring name)
{
	tableName_ = name;
	office_element_->table_table_attlist_.table_name_ = name;
}

void ods_table_state::convert()
{
	if (office_element_ == NULL)return;

	//последние штрихи??? проверки .. согласования

}

}
}
