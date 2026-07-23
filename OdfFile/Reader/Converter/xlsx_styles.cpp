/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#include "xlsxconversioncontext.h"
#include "xlsx_fonts.h"
#include "xlsx_xf.h"
#include "xlsx_dxfs.h"
#include "xlsx_borders.h"
#include "xlsx_fills.h"
#include "xlsx_cell_format.h"
#include "xlsx_cell_styles.h"
#include "xlsx_numFmts.h"

#include "../Format/style_text_properties.h"
#include "../Format/style_paragraph_properties.h"
#include "../Format/style_graphic_properties.h"

#include "../Format/odf_document.h"
#include "../Format/odfcontext.h"

#include <boost/unordered_set.hpp>
#include <boost/functional.hpp>

namespace cpdoccore {
namespace oox {

class xlsx_style_manager::Impl
{
public:
    typedef boost::unordered_set<xlsx_xf, boost::hash<xlsx_xf> > xlsx_xf_array;

    Impl(xlsx_conversion_context *context);
   
	size_t size() const;
   
	size_t xfId(	const odf_reader::text_format_properties_ptr &textProp,
					const odf_reader::paragraph_format_properties			* parProp,
					const odf_reader::style_table_cell_properties_attlist	* cellProp,
					const xlsx_cell_format * xlxsCellFormat,
					const std::wstring &num_format, odf_types::office_value_type::type num_format_type, bool  default_set, bool & is_visible);
	
	size_t dxfId(	const odf_reader::text_format_properties_ptr &textProp,
					const odf_reader::graphic_format_properties_ptr &graphProp,
					const odf_reader::style_table_cell_properties_attlist	*cellProp);

    void serialize		(std::wostream & _Wostream);
    void serialize_xf	(std::wostream & _Wostream, const xlsx_xf_array & xfArray, const std::wstring & nodeName);

private:
    xlsx_fonts				fonts_;
    xlsx_borders			borders_;
    xlsx_fills				fills_;
    xlsx_num_fmts			numFmts_;
    
	xlsx_xf_array			cellXfs_;

    xlsx_cell_styles		cellStyles_;
    xlsx_xf_array			cellStyleXfs_;
    
	xlsx_dxfs				dxfs_;

//-------------------------------------------------
	xlsx_conversion_context		*context;
    size_t						next_index_;

    void insert(xlsx_xf const & xf)
    {
        xf.index = next_index_++;
        cellXfs_.insert(xf);
    } 
};


xlsx_style_manager::Impl::Impl(xlsx_conversion_context *context_) : next_index_(0), context(context_), 
	fonts_(context_->root()->odf_context().fontContainer()), dxfs_(context_->root()->odf_context().fontContainer())
{
    xlsx_xf xfRecord;
    xfRecord.applyNumberForm = true;
	xfRecord.numFmtId = 0;
    
	cellStyleXfs_.insert(xfRecord);
}

size_t xlsx_style_manager::Impl::size() const
{
    return cellXfs_.size();
}

size_t xlsx_style_manager::Impl::dxfId(	const odf_reader::text_format_properties_ptr &textProp,
										const odf_reader::graphic_format_properties_ptr &graphProp,
										const odf_reader::style_table_cell_properties_attlist	* cellProp)
{
	return dxfs_.dxfId(textProp, graphProp, cellProp);
}

size_t xlsx_style_manager::Impl::xfId(const odf_reader::text_format_properties_ptr &textProp,
                                      const odf_reader::paragraph_format_properties			* parProp,
                                      const odf_reader::style_table_cell_properties_attlist * cellProp,
                                      const xlsx_cell_format								* xlxsCellFormat,

                                      const std::wstring &num_format, odf_types::office_value_type::type num_format_type, 
									  
									  bool  default_set, bool & is_visible )
{
	bool is_visible_set = is_visible;
    const size_t fontId = fonts_.fontId(textProp, parProp, cellProp, default_set);
    is_visible = false;
    
	bool default_border = false;
    const size_t borderId = borders_.borderId(cellProp, default_border);

    bool default_fill = false;
    const size_t fillId = fills_.fillId(textProp, parProp, cellProp, default_set, default_fill);
 
    if (!default_border || !default_fill || is_visible_set/* || (fillId > 2 && default_set != default_fill)*/)
		is_visible = true;

    xlsx_alignment alignment = OdfProperties2XlsxAlignment(context, textProp, parProp, cellProp);
	xlsx_protection protection = OdfProperties2XlsxProtection( cellProp);
	
	const unsigned int id = next_index_;//static_cast<unsigned int>(cellXfs_.size());    

    xlsx_xf xfRecord;
   
	xfRecord.applyAlignment = !is_default(alignment);
	xfRecord.alignment = alignment;

    xfRecord.applyBorder = true;
    xfRecord.borderId = borderId;

    xfRecord.applyFill = true;
    xfRecord.fillId = fillId;
    
    xfRecord.applyFont = true;
    xfRecord.fontId = fontId;
    
    xfRecord.applyProtection = !is_default(protection);
	xfRecord.protection = protection;

    if (false == num_format.empty())
    {
        xfRecord.applyNumberForm = true;
        xfRecord.numFmtId = numFmts_.add_or_find(num_format, num_format_type);
    }
    else
		if (xlxsCellFormat && 
			xlxsCellFormat->get_cell_type() != XlsxCellType::null)
	{
		xfRecord.applyNumberForm = true;
		xfRecord.numFmtId = xlxsCellFormat->get_num_format();
	}

    xfRecord.xfId = 0;

    xlsx_xf_array::const_iterator i = cellXfs_.find(xfRecord);
    if (i != cellXfs_.end())
    {
        const std::size_t dbgId = i->index;
        return dbgId;
    }
    else
    {
        insert(xfRecord);
    }
    
    return id;
}

void xlsx_style_manager::Impl::serialize(std::wostream & _Wostream)
{
    _Wostream << L"<styleSheet";
		_Wostream << L" xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\"";
		_Wostream << L" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\""; 
		_Wostream << L" mc:Ignorable=\"x14ac\"";
		_Wostream << L" xmlns:x14ac=\"http://schemas.microsoft.com/office/spreadsheetml/2009/9/ac\"";
	_Wostream << L">";

	numFmts_.serialize	(_Wostream);
    fonts_.serialize	(_Wostream);
	fills_.serialize	(_Wostream);
	borders_.serialize	(_Wostream);

    serialize_xf(_Wostream, cellStyleXfs_	, L"cellStyleXfs");
    serialize_xf(_Wostream, cellXfs_		, L"cellXfs");
    
	cellStyles_.serialize(_Wostream);

	dxfs_.serialize(_Wostream);
    _Wostream << L"</styleSheet>";
}

namespace 
{
    struct compare_xlsx_xf
    {
        bool operator() (xlsx_xf const & x1, xlsx_xf const & x2)
        {
            return x1.index < x2.index;
        }
    };
}

void xlsx_style_manager::Impl::serialize_xf(std::wostream & _Wostream, const xlsx_xf_array & xfArray, const std::wstring & nodeName)
{
    std::vector<xlsx_xf> xfs_;
   
	for (boost::unordered_set<xlsx_xf, boost::hash<xlsx_xf>>::iterator it = xfArray.begin(); it != xfArray.end(); ++it)
	{
        xfs_.push_back(*it);
	}

    std::sort(xfs_.begin(), xfs_.end(), compare_xlsx_xf());

    _Wostream << L"<" << nodeName << L" count=\"" << xfs_.size() << L"\">";
	for (size_t i = 0; i < xfs_.size(); i++)
    {
        cpdoccore::oox::xlsx_serialize(_Wostream, xfs_[i]);
    }
    _Wostream << L"</" << nodeName << L">";    
}

//------------------------------------------------------------------------------------------

xlsx_style_manager::xlsx_style_manager(xlsx_conversion_context *context) : impl_(new xlsx_style_manager::Impl(context) )
{}

size_t xlsx_style_manager::size() const
{
    return impl_->size();
}

size_t xlsx_style_manager::xfId(const odf_reader::text_format_properties_ptr &textProp,
                                const odf_reader::paragraph_format_properties			*parProp,
                                const odf_reader::style_table_cell_properties_attlist	*cellProp,
                                const xlsx_cell_format * xlxsCellFormat,
                                const std::wstring &num_format, char num_format_type, bool  default_set)
{
    bool is_visible;
    return impl_->xfId(textProp, parProp, cellProp, xlxsCellFormat, num_format, (odf_types::office_value_type::type) num_format_type, default_set, is_visible);
}

size_t xlsx_style_manager::xfId(const odf_reader::text_format_properties_ptr &textProp,
                                const odf_reader::paragraph_format_properties			*parProp,
                                const odf_reader::style_table_cell_properties_attlist	*cellProp,
                                const xlsx_cell_format * xlxsCellFormat,
                                const std::wstring &num_format, char num_format_type, bool  default_set, bool & is_visible)
{
    return impl_->xfId(textProp, parProp, cellProp, xlxsCellFormat, num_format, (odf_types::office_value_type::type)num_format_type, default_set,is_visible);
}
size_t xlsx_style_manager::dxfId(const odf_reader::text_format_properties_ptr &textProp,
								 const odf_reader::graphic_format_properties_ptr &graphProp,
								 const odf_reader::style_table_cell_properties_attlist	*cellProp)
{
	return impl_->dxfId(textProp, graphProp, cellProp);
}

void xlsx_style_manager::xlsx_serialize(std::wostream & _Wostream)
{
    return impl_->serialize(_Wostream);
}

xlsx_style_manager::~xlsx_style_manager()
{
}

}
}
