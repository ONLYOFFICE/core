#include "precompiled_cpodf.h"
#include "xlsx_table_state.h"
#include "logging.h"
#include "xlsxconversioncontext.h"

#include <cpdoccore/xml/simple_xml_writer.h>

namespace cpdoccore {
namespace oox {

xlsx_table_state::xlsx_table_state(xlsx_conversion_context & Context,
    const std::wstring & StyleName,
    const std::wstring & tableName)
     : context_(Context),
    table_style_(StyleName),
    tableName_(tableName),
    current_table_column_(-1),
    current_table_row_(-1),
    columns_spanned_num_(0),
    columns_count_(0),
    xlsx_drawing_context_(Context.get_drawing_context_handle()),
    xlsx_comments_context_(Context.get_comments_context_handle()),
    table_column_last_width_(0.0)
{        
	memset(&group_row_,0,sizeof(_group_row));
}
    
void xlsx_table_state::start_column(unsigned int repeated, const std::wstring & defaultCellStyleName)
{
    for (unsigned int i = 0; i <repeated; ++i)
        column_default_cell_style_name_.push_back(defaultCellStyleName);

    columns_count_ += repeated;
    columns_.push_back(repeated);
}

unsigned int xlsx_table_state::columns_count() const
{
    return columns_count_;
}

void xlsx_table_state::set_table_row_group(int count, bool collapsed, int level)
{
	group_row_.enabled = true;
	group_row_.count = count;
	group_row_.collapsed = collapsed;
	group_row_.level = level;
}

void xlsx_table_state::start_row(const std::wstring & StyleName, const std::wstring & defaultCellStyleName)
{
    empty_row_ = true;
    // reset column num, column spanned style
    current_table_column_ = -1; 
    current_table_row_++;
    columns_spanned_style_ = L"";
    row_default_cell_style_name_ = defaultCellStyleName;
        
    // set row style name
    table_row_style_ = StyleName;
}

void xlsx_table_state::non_empty_row()
{
    empty_row_= false;
}

bool xlsx_table_state::is_empty_row() const
{
    return empty_row_;
}

std::wstring xlsx_table_state::default_row_cell_style() const
{
    return row_default_cell_style_name_;
}

std::wstring xlsx_table_state::default_column_cell_style() const
{
	if (current_table_column_ < column_default_cell_style_name_.size())
		return column_default_cell_style_name_.at(current_table_column_);
	else
	{
		//непон€тна€ хрень!! - неправильно сформирован ods???
		return L"";
	}
}

void xlsx_table_state::end_row()
{
    table_row_style_ = L"";
}

std::wstring xlsx_table_state::current_row_style() const
{
    return table_row_style_;
}

void xlsx_table_state::start_cell(size_t columnsSpanned, size_t rowsSpanned)
{
    current_table_column_++;

    // в случае если объединение имеет место добавл€ем запись о нем
    if (columnsSpanned != 0 || rowsSpanned != 0)
        merge_cells_.add_merge(current_table_column_, current_table_row_, columnsSpanned, rowsSpanned);

    if ( current_columns_spaned() > 0 )
    {
        _CP_LOG(info) << L"[warning] current columns spanned > 0\n";
    }

    columns_spanned_num_ = static_cast<int>(columnsSpanned);

    // обновл€ем вектор, в котором хран€тс€ информаци€ об объединении строк
    // добавл€ем в него новый столбец

    
    for (size_t i = 0; i <= columns_spanned_num_; ++i)
    {
        if (current_table_column_+i >= (int)(rows_spanned_.size()))
            rows_spanned_.push_back(xlsx_row_spanned());   
    }

    if (rows_spanned_.size() <= current_table_column_)
    {
#ifdef _DEBUG
		_CP_LOG(info) << L"[warning] set_rows_spanned error\n";
#endif
    }
    else
    {
        if (rows_spanned_[current_table_column_].num() > 0)
        {
#ifdef _DEBUG
			_CP_LOG(info) << L"[warning] current rows spanned > 0\n";       
#endif
        }
        rows_spanned_[current_table_column_].num(static_cast<unsigned int>(rowsSpanned));
        rows_spanned_[current_table_column_].column_spanned(static_cast<unsigned int>(columnsSpanned));
        for (size_t i = 0; i <= columns_spanned_num_; ++i)
        {
            rows_spanned_[current_table_column_ + i].set_style_id(-1);
        }
    }
}

void xlsx_table_state::end_cell()
{}

void xlsx_table_state::set_current_cell_style_id(unsigned int xfId)
{
    for (size_t i = 0; i <= columns_spanned_num_; ++i)
    {
        rows_spanned_[current_table_column_ + i].set_style_id((int)xfId);
    }
}

int xlsx_table_state::get_current_cell_style_id()
{
    return rows_spanned_[current_table_column_].get_style_id();
}

void xlsx_table_state::start_covered_cell()
{
    current_table_column_++;

    // обновл€ем вектор, в котором хран€тс€ информаци€ об объединении строк
    // добавл€ем в него новый столбец

    if (current_table_column_ >= (int)(rows_spanned_.size()))
        rows_spanned_.push_back(xlsx_row_spanned());

    // использовали текущую €чейку, уменьшаем счетчики оставшихс€ объединенных €чеек
    // дл€ столбцов и строк

    if (columns_spanned_num_ > 0)
        columns_spanned_num_--;

    if (rows_spanned_[current_table_column_].num() > 0)
        rows_spanned_[current_table_column_].decrease();

    if (current_table_column_ > 0)
    {
        const unsigned int pred = rows_spanned_[current_table_column_ - 1].column_spanned();
        if (pred > 0)
        {
            rows_spanned_[current_table_column_].column_spanned(pred - 1);
            rows_spanned_[current_table_column_].num(rows_spanned_[current_table_column_ - 1].num());
        }
    }
}

void xlsx_table_state::end_covered_cell()
{
}

int xlsx_table_state::current_column() const
{
    return current_table_column_;
}

int xlsx_table_state::current_row() const
{
    return current_table_row_;
}

unsigned int xlsx_table_state::current_columns_spaned() const
{
    return columns_spanned_num_;
}

unsigned int xlsx_table_state::current_rows_spanned(unsigned int Column) const
{
    if (rows_spanned_.size() <= Column)
    {
        _CP_LOG(info) << L"[warning] current_rows_spanned error\n";        
        return 0;
    }
    else
    {
        return rows_spanned_[Column].num();
    }
}

double charsToSize(unsigned int charsCount, double maxDigitSize) 
{
	return 1.0 * int((maxDigitSize * charsCount + 5.0) / maxDigitSize * 256.0) / 256.0;
}

void xlsx_table_state::serialize_table_format(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE(L"sheetView")
		{
			//	-showGridLines
			//	-showRowColHeaders
			//	-rightToLeft
			//	-zoomScale
		} 
		CP_XML_NODE(L"sheetFormatPr")
		{
			double default_height=int (context_.getMaxDigitSize().second * 72. / getDefaultDpi() * 100.) /100.;//in point size.

			
			//odf::style_instance * rowStyle = odfContext.styleContainer().style_by_name(styleName, odf::style_family::TableRow,false/*false*/);
			//if (!rowStyle)
			//	break;

			//if (!rowStyle->content())
			//	break;

			//const odf::style_table_row_properties * prop = rowStyle->content()->get_style_table_row_properties();
			//if (!prop)
			//	break;
			//if (const _CP_OPT(odf::length) & height = prop->style_table_row_properties_attlist_.style_row_height_)
			//{
			//	row_height = height->get_value_unit(odf::length::pt);
			//	std::wstringstream ht_s;
			//	ht_s.precision(3);
			//	ht_s << std::fixed << row_height;
			//	ht = ht_s.str();
			//}

			//CP_XML_ATTR(L"defaultColWidth", merges_.size());
			CP_XML_ATTR(L"defaultRowHeight",default_height);
		}  
	}

}
void xlsx_table_state::serialize_merge_cells(std::wostream & _Wostream)
{
    return merge_cells_.xlsx_serialize(_Wostream);
}

void xlsx_table_state::serialize_hyperlinks(std::wostream & _Wostream)
{
    return xlsx_hyperlinks_.xlsx_serialize(_Wostream);
}

void xlsx_table_state::dump_rels_hyperlinks(rels & Rels)
{
    return xlsx_hyperlinks_.dump_rels(Rels);
}

void xlsx_table_state::start_hyperlink()
{
}

 std::wstring xlsx_table_state::end_hyperlink(std::wstring const & ref, std::wstring const & href, std::wstring const & display)
{
    return xlsx_hyperlinks_.add(ref, href, display);
}

}
}
