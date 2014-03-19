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
	office_table_ = elm; 
}

void ods_table_state::set_name(std::wstring name)
{
	table_table* table = dynamic_cast<table_table*>(office_table_.get());
	if (table == NULL)return;

	tableName_ = name;
	table->table_table_attlist_.table_name_ = name;
}

void ods_table_state::convert()
{
	if (office_table_ == NULL)return;

	//??? 

}

void ods_table_state::set_table_style(office_element_ptr & elm)
{	
	office_table_style_ = dynamic_cast<style*>(elm.get());

	if (!office_table_style_)return;

	table_table* table = dynamic_cast<table_table*>(office_table_.get());
	if (table == NULL)return;
	
	table->table_table_attlist_.table_style_name_ = office_table_style_->style_name_;
	//потом в принципе и по имени можно будет связать(найти)


}
void ods_table_state::add_column(office_element_ptr & elm, int repeated, const std::wstring & style_name)
{
	office_table_->add_child_element(elm);

	ods_column_state state = {elm, repeated,style_name};
    columns_count_ += repeated;
    columns_.push_back(state);

///
	table_table_column* column = dynamic_cast<table_table_column*>(columns_.back().elm.get());
	if (column == NULL)return;

	column->table_table_column_attlist_.table_style_name_ = style_ref(style_name);
	column->table_table_column_attlist_.table_number_columns_repeated_ = repeated;
	
	
}

unsigned int ods_table_state::columns_count() const
{
    return columns_count_;
}



}
}
