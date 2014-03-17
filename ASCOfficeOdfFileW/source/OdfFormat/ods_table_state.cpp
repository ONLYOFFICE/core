#include "precompiled_cpodf.h"
#include "logging.h"

#include "ods_table_state.h"
#include "ods_conversion_context.h"

#include "table.h"
#include "styles.h"

namespace cpdoccore {
namespace odf {

ods_table_state::ods_table_state(ods_conversion_context & Context, office_element_ptr & elm): context_(Context)   
{        
	office_table_ = (table_table*)dynamic_cast<table_table*>(elm.get());

}

void ods_table_state::set_name(std::wstring name)
{
	tableName_ = name;
	office_table_->table_table_attlist_.table_name_ = name;
}

void ods_table_state::convert()
{
	if (office_table_ == NULL)return;

	//??? 

}

void ods_table_state::set_table_style(office_element_ptr & elm)
{	
	office_table_style_ = (style*)dynamic_cast<style*>(elm.get());

	if (!office_table_style_)return;

	office_table_->table_table_attlist_.table_style_name_ = office_table_style_->style_name_;
	//потом в принципе и по имени можно будет связать(найти)


}
}
}
