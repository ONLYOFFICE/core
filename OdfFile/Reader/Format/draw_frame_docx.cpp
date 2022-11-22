/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "office_forms.h"
#include "draw_frame.h"

#include <ostream>
#include <sstream>
#include <string>

#include <boost/regex.hpp>

#include "odf_document.h"
#include <xml/utils.h>

#include "serialize_elements.h"

#include "odfcontext.h"
#include "odf_document_impl.h"

#include "draw_common.h"

#include "../../Reader/Converter/docx_drawing.h"
#include "../../Reader/Converter/xlsx_package.h"
#include "../../Reader/Converter/oox_conversion_context.h"

#include "chart_build_oox.h"

#include "calcs_styles.h"

#include "../../DataTypes/length.h"
#include "../../DataTypes/borderstyle.h"

#include "../../../OOXML/Base/Unit.h"
#include "../../../OfficeUtils/src/OfficeUtils.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {


namespace {
bool IsExistProperty(std::vector<_property> Heap,const std::wstring Name)
{
    for (size_t i = 0; i < Heap.size(); i++)
	{
		int res = Heap[i].name_.find(Name);
		if (res>=0)
		{
			return true;
		}
	}
	return false;			
}

_CP_OPT(length) CalcResultLength(const _CP_OPT(length_or_percent) & Value, const _CP_OPT(length) & Base)
{
    if (Value && Value->get_type() == length_or_percent::Percent && Base)
    {
        return length(Base->get_value() * Value->get_percent().get_value() / 100.0, Base->get_unit());
    }
    else if (Value)
        return Value->get_length();
    else
        return _CP_OPT(length)();
}

_CP_OPT(length) GetOnlyLength(const _CP_OPT(length_or_percent) & Value)
{
    if (Value && Value->get_type() == length_or_percent::Length)
        return Value->get_length();
    else
        return _CP_OPT(length)();
}



length ComputeContextWidth(const style_page_layout_properties			* pagePropertiesNode,
                           const style_page_layout_properties_attlist	& pageProperties,
                           const union_common_draw_attlists				& attlists_,
                           const _CP_OPT(horizontal_rel)				& horizontalRel,
                           const _CP_OPT(horizontal_pos)				& horizontalPos,
                           const _CP_OPT(length)						& pageWidth,
                           const _CP_OPT(length)						& pageMarginLeft,
                           const _CP_OPT(length)						& pageMarginRight
                           )
{
    if (!horizontalRel)
        return length(0, length::pt);

    switch(horizontalRel->get_type())
    {
    case horizontal_rel::Page:
    case horizontal_rel::PageContent:
    case horizontal_rel::PageStartMargin:
        return length(
            pageWidth.get_value_or(length(0, length::pt)).get_value_unit(length::pt) - 
            pageMarginRight.get_value_or(length(0, length::pt)).get_value_unit(length::pt), length::pt
            );           
    case horizontal_rel::PageEndMargin:
        if (horizontalPos)
        {
            switch(horizontalPos->get_type())
            {
            case horizontal_pos::Left:
            case horizontal_pos::Inside:
                return length(
                    pageWidth.get_value_or(length(0, length::pt)).get_value_unit(length::pt) - 
                    pageMarginRight.get_value_or(length(0, length::pt)).get_value_unit(length::pt), length::pt
                    );  
            case horizontal_pos::Right:
            case horizontal_pos::Outside:
                return length(
                    pageWidth.get_value_or(length(0, length::pt)).get_value_unit(length::pt) - 
                    pageMarginRight.get_value_or(length(0, length::pt)).get_value_unit(length::pt) -
                    pageMarginRight.get_value_or(length(0, length::pt)).get_value_unit(length::pt), length::pt
                    );  
            }
        }
        break;
    case horizontal_rel::Paragraph:
    case horizontal_rel::ParagraphContent:
    case horizontal_rel::ParagraphStartMargin:
    case horizontal_rel::ParagraphEndMargin:
        {
            unsigned int columnsCount = 1;
            length columnGap(0, length::pt);
            if (pagePropertiesNode)
            {
                if (const style_columns * styleColumns 
                    = dynamic_cast<const style_columns*>( pagePropertiesNode->style_columns_.get()))
                {
                    columnsCount = styleColumns->fo_column_count_.get_value_or(1);
                    if (!columnsCount)
                        columnsCount = 1;
                    columnGap = styleColumns->fo_column_gap_.get_value_or( length(0, length::pt ));
                }
            }
			//$pageWidth - $pageLeftMargin - $pageRightMargin - $columnGap
            return length( (pageWidth.get_value_or(length(0, length::pt)).get_value_unit(length::pt) - 
				pageMarginRight.get_value_or(length(0, length::pt)).get_value_unit(length::pt) -
                    pageMarginRight.get_value_or(length(0, length::pt)).get_value_unit(length::pt) - 
                    columnGap.get_value_unit(length::pt)) / columnsCount
                    );
        }
        break;
    }
    return length(0, length::pt);
}

int ComputeMarginX(const style_page_layout_properties				* pagePropertiesNode,
                      const style_page_layout_properties_attlist	& pageProperties,
                      const union_common_draw_attlists				& attlists_,
                      const graphic_format_properties				& graphicProperties,
					  const std::vector<odf_reader::_property>		& additional)
{

    const _CP_OPT(anchor_type) anchor = attlists_.shape_with_text_and_styles_.common_text_anchor_attlist_.type_;

    _CP_OPT(horizontal_rel) styleHorizontalRel = graphicProperties.common_horizontal_rel_attlist_.style_horizontal_rel_;
    _CP_OPT(horizontal_pos) styleHorizontalPos = graphicProperties.common_horizontal_pos_attlist_.style_horizontal_pos_;

	_CP_OPT(double) dVal;	
	GetProperty(additional, L"svg:translate_x", dVal);
 	
	const _CP_OPT(length) translation		= length(dVal ? *dVal : 0, length::pt);
	const _CP_OPT(length) pageWidth			= pageProperties.fo_page_width_;
    const _CP_OPT(length) pageMarginLeft	= CalcResultLength(pageProperties.common_horizontal_margin_attlist_.fo_margin_left_, pageWidth);
    const _CP_OPT(length) pageMarginRight	= CalcResultLength(pageProperties.common_horizontal_margin_attlist_.fo_margin_right_, pageWidth);

	length contextWidth = ComputeContextWidth(pagePropertiesNode, pageProperties, attlists_,
												styleHorizontalRel, styleHorizontalPos, pageWidth, pageMarginLeft,pageMarginRight);

    _CP_OPT(length)		contextSubstractedValue = length(0., length::pt);
    _CP_OPT(style_wrap) styleWrap = graphicProperties.style_wrap_;
   
	if (!styleWrap || 
        styleWrap->get_type() == style_wrap::None ||
        styleWrap->get_type() == style_wrap::RunThrough)
    {
        // TODO contextSubstractedValue
    }

    const _CP_OPT(length) frameMarginLeft	= GetOnlyLength(graphicProperties.common_horizontal_margin_attlist_.fo_margin_left_);
    const _CP_OPT(length) frameMarginRight	= GetOnlyLength(graphicProperties.common_horizontal_margin_attlist_.fo_margin_right_);

	const _CP_OPT(length) frameWidth	= attlists_.rel_size_.common_draw_size_attlist_.svg_width_;
    const _CP_OPT(length) frameHeight	= attlists_.rel_size_.common_draw_size_attlist_.svg_height_;

    const _CP_OPT(length) fromLeft = (styleHorizontalPos && styleHorizontalPos->get_type() == horizontal_pos::FromLeft) ?
										attlists_.position_.svg_x_ : length(0, length::pt);
    
     _CP_OPT(length) svgX;

    if (styleHorizontalPos)
    {
        const horizontal_pos::type horPos = styleHorizontalPos->get_type();
        
        if (horPos == horizontal_pos::FromLeft ||
            horPos == horizontal_pos::FromInside )
        {
            svgX = length(0, length::pt);
            if (styleHorizontalRel)
            {
                const horizontal_rel::type horRel = styleHorizontalRel->get_type();

                if (horRel == horizontal_rel::Page ||
                    horRel == horizontal_rel::PageContent ||
                    horRel == horizontal_rel::PageStartMargin)
                {
                    svgX = length (fromLeft.get_value_or(length(0, length::pt)).get_value_unit(length::pt)
									+ translation->get_value_unit(length::pt)
									, length::pt); // + translation
                }
                else if (horRel == horizontal_rel::PageEndMargin)
                {
                    svgX = length (contextWidth.get_value_unit(length::pt) 
									+ fromLeft.get_value_or(length(0, length::pt)).get_value_unit(length::pt)
									+ translation->get_value_unit(length::pt)
									, length::pt); // + translation
                }
                else if (anchor && anchor->get_type() == anchor_type::Page)
                {
                    svgX = length (fromLeft.get_value_or(length(0, length::pt)).get_value_unit(length::pt)
									+ translation->get_value_unit(length::pt)
									, length::pt); // + translation
                }
                else if (horRel == horizontal_rel::Paragraph ||
                    horRel == horizontal_rel::ParagraphContent ||
                    horRel == horizontal_rel::ParagraphStartMargin)
                {
                    double paragraphLeftIndent = 0;
                    svgX = length (paragraphLeftIndent
									- contextSubstractedValue.get_value_or(length(0, length::pt)).get_value_unit(length::pt)
									+ fromLeft.get_value_or(length(0, length::pt)).get_value_unit(length::pt)
									+ translation->get_value_unit(length::pt)
									, length::pt); // + translation
                                        
                }
                else if (horRel == horizontal_rel::ParagraphEndMargin)
                {
                    double paragraphRightIndent = 0;
                    
					svgX = length ( contextWidth.get_value_unit(length::pt) - paragraphRightIndent
									- contextSubstractedValue.get_value_or(length(0, length::pt)).get_value_unit(length::pt)
									+ fromLeft.get_value_or(length(0, length::pt)).get_value_unit(length::pt)
									+ translation->get_value_unit(length::pt)
									, length::pt); // + translation
                }
                else if (horRel == horizontal_rel::Frame ||
                    horRel == horizontal_rel::FrameContent ||
                    horRel == horizontal_rel::FrameStartMargin ||
                    horRel == horizontal_rel::FrameEndMargin)
                {
                    svgX = length (fromLeft.get_value_or(length(0, length::pt)).get_value_unit(length::pt)
									+ translation->get_value_unit(length::pt)
									, length::pt); // + translation
                }
                else if (horRel == horizontal_rel::Char)
                {
                    svgX = length (fromLeft.get_value_or(length(0, length::pt)).get_value_unit(length::pt)
									+ translation->get_value_unit(length::pt)
									, length::pt); // + translation
                }
            }
            return get_value_emu(svgX);
        }
        else if ( horPos == horizontal_pos::Left ||
                    horPos == horizontal_pos::Inside )
        {
            if (styleHorizontalRel)
            {
                const horizontal_rel::type horRel = styleHorizontalRel->get_type();

                if (horRel == horizontal_rel::Page || 
                    horRel == horizontal_rel::PageContent || 
                    horRel == horizontal_rel::PageStartMargin)
                {
                    svgX = length ( frameMarginLeft.get_value_or( length(0, length::pt )).get_value_unit(length::pt) 
										+ translation->get_value_unit(length::pt)
										, length::pt);
                }
                else if (horRel == horizontal_rel::PageEndMargin)
                {
                    if (frameWidth.get_value_or( length(0, length::pt )).get_value_unit(length::pt) <
                        pageMarginRight.get_value_or( length(0, length::pt )).get_value_unit(length::pt))
                    {
                        svgX = length (contextWidth.get_value_unit(length::pt) // - contextSubstractedValue
										+ frameMarginLeft.get_value_or( length(0, length::pt )).get_value_unit(length::pt)
										, length::pt);
                    }
                    else
                    {
                        svgX = length (pageWidth.get_value_or( length(0, length::pt )).get_value_unit(length::pt) 
										- frameWidth.get_value_or( length(0, length::pt )).get_value_unit(length::pt)
										- frameMarginLeft.get_value_or( length(0, length::pt )).get_value_unit(length::pt)
										+ translation->get_value_unit(length::pt)
										, length::pt);
                        //$pageWidth -$frameWidth - $frameMarginLeft + $translation
                    }
                } 
                else if (anchor && anchor->get_type() == anchor_type::Page)
                {
                    svgX = frameMarginLeft.get_value_or( length(0, length::pt ));
                } 
                else if (horRel == horizontal_rel::Paragraph ||
                    horRel == horizontal_rel::ParagraphContent ||
                    horRel == horizontal_rel::ParagraphStartMargin)
                {
                    // TODO paragraphLeftIndent
                    // TODO contextSubstractedValue
                    //$paragraphLeftIndent + $frameMarginLeft - $contextSubstractedValue + $translation"                        
                    svgX = length ( frameMarginLeft.get_value_or( length(0, length::pt )).get_value_unit(length::pt)
										+ translation->get_value_unit(length::pt)
										, length::pt );
                } 
                else if (horRel == horizontal_rel::ParagraphEndMargin)
                {
                    // TODO paragraphRightIndent
                    // TODO contextSubstractedValue
                    // select="$contextWidth - $paragraphRightIndent + $frameMarginLeft - $contextSubstractedValue + $translation"
                    svgX = length (contextWidth.get_value_unit(length::pt)
									+ frameMarginLeft.get_value_or( length(0, length::pt )).get_value_unit(length::pt)
									+ translation->get_value_unit(length::pt)
									, length::pt );
                }
                else if (horRel == horizontal_rel::Frame ||
                    horRel == horizontal_rel::FrameContent ||
                    horRel == horizontal_rel::FrameStartMargin ||
                    horRel == horizontal_rel::FrameEndMargin)
                {
                    svgX = frameMarginLeft.get_value_or( length(0, length::pt ));
                }
                else if (horRel == horizontal_rel::Char)
                {
                    svgX = frameMarginLeft.get_value_or( length(0, length::pt ));
                }
                else
                {
                    svgX = length(0, length::pt);                    
                }
            }            
        }
        else if ( (horPos == horizontal_pos::Right ||
            horPos == horizontal_pos::Outside ) && frameMarginRight)
        {
            if (styleHorizontalRel)
            {
                 const horizontal_rel::type horRel = styleHorizontalRel->get_type();
                 
                 if (horRel == horizontal_rel::Page || 
                     horRel == horizontal_rel::PageEndMargin)
                 {
                     // "$pageWidth - $frameWidth - $frameMarginRight + $translation
                     svgX = length (pageWidth.get_value_or( length(0, length::pt )).get_value_unit(length::pt)
										- frameWidth.get_value_or( length(0, length::pt )).get_value_unit(length::pt) 
										- frameMarginRight.get_value_or( length(0, length::pt )).get_value_unit(length::pt)
										+ translation->get_value_unit(length::pt)
										, length::pt);
                 }
                 else if (horRel == horizontal_rel::PageStartMargin)
                 {
                     if (frameWidth.get_value_or( length(0, length::pt )).get_value_unit(length::pt) < 
								pageMarginLeft.get_value_or( length(0, length::pt )).get_value_unit(length::pt))                                         
                     {
                         svgX = length (pageMarginLeft.get_value_or( length(0, length::pt )).get_value_unit(length::pt) 
										 - frameWidth.get_value_or( length(0, length::pt )).get_value_unit(length::pt) 
										 - frameMarginRight.get_value_or( length(0, length::pt )).get_value_unit(length::pt)
										 , length::pt);                        
                     }
                     else
                         svgX = length(0, length::pt);

                 }
                 else if (anchor && anchor->get_type() == anchor_type::Page)
                 {
                     svgX = length( pageWidth.get_value_or( length(0, length::pt )).get_value_unit(length::pt)
									 - frameWidth.get_value_or( length(0, length::pt )).get_value_unit(length::pt) 
									 - frameMarginRight.get_value_or( length(0, length::pt )).get_value_unit(length::pt));                    
                 } 
                 else if (horRel == horizontal_rel::PageContent)
                 {
                     // $contextWidth - $frameWidth - $frameMarginRight - $contextSubstractedValue + $translation
                     svgX = length (contextWidth.get_value_unit(length::pt) 
										- frameWidth.get_value_or( length(0, length::pt )).get_value_unit(length::pt)
										- frameMarginRight.get_value_or( length(0, length::pt )).get_value_unit(length::pt)
										- contextSubstractedValue.get_value_or( length(0, length::pt )).get_value_unit(length::pt)
										+ translation->get_value_unit(length::pt)
										, length::pt);                                                              
                 }
                 else if (horRel == horizontal_rel::Paragraph ||
                     horRel == horizontal_rel::ParagraphContent ||
                     horRel == horizontal_rel::ParagraphEndMargin)
                 {
                     // $contextWidth - $paragraphRightIndent -$frameWidth - $frameMarginRight - $contextSubstractedValue + $translation
                     length paragraphRightIndent(0, length::pt);
                     svgX = length ( contextWidth.get_value_unit(length::pt)
										- paragraphRightIndent.get_value_unit(length::pt)
										- frameWidth.get_value_or( length(0, length::pt )).get_value_unit(length::pt)
										- frameMarginRight.get_value_or( length(0, length::pt )).get_value_unit(length::pt)
										- contextSubstractedValue.get_value_or( length(0, length::pt )).get_value_unit(length::pt)
										+ translation->get_value_unit(length::pt)
										, length::pt); 

                 }     
                 else if (horRel == horizontal_rel::ParagraphStartMargin)
                 {
                     // $paragraphLeftIndent - $frameMarginRight - $contextSubstractedValue + $translation
                     length paragraphLeftIndent(0, length::pt);
                     svgX = length (paragraphLeftIndent.get_value_unit(length::pt)
										- frameMarginRight.get_value_or( length(0, length::pt )).get_value_unit(length::pt)
										- contextSubstractedValue.get_value_or( length(0, length::pt )).get_value_unit(length::pt)
										+ translation->get_value_unit(length::pt)
										, length::pt); 
                 } 
                 else if (horRel == horizontal_rel::Frame ||
                     horRel == horizontal_rel::FrameContent ||
                     horRel == horizontal_rel::FrameStartMargin ||
                     horRel == horizontal_rel::FrameEndMargin)
                 {
                     // $pageWidth - $frameMarginRight + $translation

                     svgX = length (pageWidth.get_value_or( length(0, length::pt )).get_value_unit(length::pt)
										- frameMarginRight.get_value_or( length(0, length::pt )).get_value_unit(length::pt)                         
										+ translation->get_value_unit(length::pt)
										, length::pt); 

                 }
                 else if (horRel == horizontal_rel::Char)
                 {
                     svgX = length ( pageWidth.get_value_or( length(0, length::pt )).get_value_unit(length::pt) 
									 - frameMarginRight.get_value_or( length(0, length::pt )).get_value_unit(length::pt)                         
									 , length::pt);                   
                 }
                 else
                 {
                    svgX = length(0, length::pt);
                 }
            }
        }
        else if (horPos == horizontal_pos::Center)
        {
            if (styleHorizontalRel)
            {
                const horizontal_rel::type horRel = styleHorizontalRel->get_type();

                if (horRel == horizontal_rel::PageStartMargin)
                {
                    if (frameWidth.get_value_or(length(0, length::pt)).get_value_unit(length::pt) 
                         < pageMarginLeft.get_value_or(length(0, length::pt)).get_value_unit(length::pt) )
                    {
                        svgX = length(
                            (pageMarginLeft.get_value_or(length(0, length::pt)).get_value_unit(length::pt) -
                            frameWidth.get_value_or(length(0, length::pt)).get_value_unit(length::pt)) / 2.0 ,
                            length::pt
                            );                                    
                    }
                    else
                        svgX = length(0, length::pt);

                }
                else if (horRel == horizontal_rel::PageEndMargin)
                {
                    if (frameWidth.get_value_or(length(0, length::pt)).get_value_unit(length::pt) 
                         < pageMarginRight.get_value_or(length(0, length::pt)).get_value_unit(length::pt) )
                    {
                        svgX = length( pageWidth.get_value_or( length(0, length::pt ) ).get_value_unit(length::pt) - 
                            (pageMarginRight.get_value_or(length(0, length::pt)).get_value_unit(length::pt) -
                            frameWidth.get_value_or(length(0, length::pt)).get_value_unit(length::pt)) / 2.0 ,
                            length::pt
                            );                                    
                    }
                    else
                    {
                        svgX = length( pageWidth.get_value_or( length(0, length::pt ) ).get_value_unit(length::pt) - 
                            frameWidth.get_value_or(length(0, length::pt)).get_value_unit(length::pt),
                            length::pt
                            );              
                    }
                }        
                else if (horRel == horizontal_rel::ParagraphStartMargin)
                {
                    // TODO paragraphLeftIndent
                    length paragraphLeftIndent(0, length::pt);
                    if (frameWidth.get_value_or(length(0, length::pt)).get_value_unit(length::pt) < 
                        paragraphLeftIndent.get_value_unit(length::pt))
                    {
                        svgX = length(
                        (paragraphLeftIndent.get_value_unit(length::pt) - frameWidth.get_value_or(length(0, length::pt)).get_value_unit(length::pt)) / 2.0
                        ,length::pt);                                                                    
                    }
                    else
                        svgX = length(0, length::pt);
                }
                else if (horRel == horizontal_rel::ParagraphEndMargin)
                {
                    // TODO paragraphRightIndent
                    length paragraphRightIndent(0, length::pt);
                    if (frameWidth.get_value_or(length(0, length::pt)).get_value_unit(length::pt) < 
                        paragraphRightIndent.get_value_unit(length::pt))
                    {
                        svgX = length(
                            contextWidth.get_value_unit(length::pt) - 
                        (paragraphRightIndent.get_value_unit(length::pt) - frameWidth.get_value_or(length(0, length::pt)).get_value_unit(length::pt)) / 2.0
                        ,length::pt);                                                                    
                    }
                    else
                    {
                        svgX = length(
                            contextWidth.get_value_unit(length::pt) - 
                            frameWidth.get_value_or(length(0, length::pt)).get_value_unit(length::pt)
                        ,length::pt);                                                                    
                    }
                }
                else if (horRel == horizontal_rel::Frame ||
                    horRel == horizontal_rel::FrameContent ||
                    horRel == horizontal_rel::FrameStartMargin ||
                    horRel == horizontal_rel::FrameEndMargin)
                {
                    svgX = length(
                        pageWidth.get_value_or(length(0, length::pt)).get_value_unit(length::pt) - 
                        frameWidth.get_value_or(length(0, length::pt)).get_value_unit(length::pt) / 2.0
                        ,length::pt
                        );
                }
                else
                {
                    svgX = length(0, length::pt);
                }
            }
        }
        else
        {
            if (attlists_.position_.svg_x_)
                svgX = *attlists_.position_.svg_x_;
        }
    }
	return get_value_emu (svgX);
}

int ComputeMarginY(const style_page_layout_properties_attlist		& pageProperties,
                      const union_common_draw_attlists				& attlists_,
                      const graphic_format_properties				& graphicProperties,
					  const std::vector<odf_reader::_property>		& additional)
{
    const _CP_OPT(anchor_type) anchor = attlists_.shape_with_text_and_styles_.common_text_anchor_attlist_.type_;

	//todooo пока не ясно как привязать к определеной странице в документе ...
	//const _CP_OPT(unsigned int) anchor_page_number = 
	//	attlists_.shape_with_text_and_styles_.
	//	common_text_anchor_attlist_.
	//	page_number_;

	_CP_OPT(vertical_rel) styleVerticalRel  = graphicProperties.common_vertical_rel_attlist_.style_vertical_rel_;
    _CP_OPT(vertical_pos) styleVerticallPos = graphicProperties.common_vertical_pos_attlist_.style_vertical_pos_;

	if (!styleVerticalRel && anchor)
	{
		switch(anchor->get_type())
		{
		case anchor_type::Paragraph:	styleVerticalRel = vertical_rel::Paragraph;	break;
		case anchor_type::Page:			styleVerticalRel = vertical_rel::Page;		break;
		}
	}

	_CP_OPT(double) dVal;	
	GetProperty(additional, L"svg:translate_y", dVal);
 	
	const _CP_OPT(length) translation		= length(dVal ? *dVal : 0, length::pt);
    const _CP_OPT(length) pageHeight		= pageProperties.fo_page_height_;        
    // TODO : проверить, значения в процентах что именно означают
    const _CP_OPT(length) pageMarginTop		= CalcResultLength(pageProperties.common_vertical_margin_attlist_.fo_margin_top_, pageHeight);
    const _CP_OPT(length) pageMarginBottom	= CalcResultLength(pageProperties.common_vertical_margin_attlist_.fo_margin_bottom_, pageHeight);

    const _CP_OPT(length) frameMarginTop = GetOnlyLength(graphicProperties.common_vertical_margin_attlist_.fo_margin_top_);
    const _CP_OPT(length) frameMarginBottom = GetOnlyLength(graphicProperties.common_vertical_margin_attlist_.fo_margin_bottom_);

    const _CP_OPT(length) frameWidth = attlists_.rel_size_.common_draw_size_attlist_.svg_width_;
    const _CP_OPT(length) frameHeight = attlists_.rel_size_.common_draw_size_attlist_.svg_height_;

    
    const _CP_OPT(length) fromTop = (styleVerticallPos && styleVerticallPos->get_type() == vertical_pos::FromTop) ?
        attlists_.position_.svg_y_ : length(0, length::pt);


    _CP_OPT(length) svgY;

    if (styleVerticallPos && 
        styleVerticallPos->get_type() == vertical_pos::FromTop)
    {
        svgY = length(0, length::pt);
        if (styleVerticalRel)
        {
            switch(styleVerticalRel->get_type())
            {
            case vertical_rel::Page:
            case vertical_rel::PageContent:
            case vertical_rel::Paragraph:
            case vertical_rel::Line:
                svgY = length (fromTop.get_value_or(length(0, length::pt)).get_value_unit(length::pt)
									+ translation->get_value_unit(length::pt)
									, length::pt); 
                break;        
            case vertical_rel::ParagraphContent:
                // TODO:  get spacing property of current paragraph
                svgY = length (fromTop.get_value_or(length(0, length::pt)).get_value_unit(length::pt)
					+ translation->get_value_unit(length::pt)
					, length::pt); 
				/*+paragraphTopSpacing*/;
                break;
            case vertical_rel::Frame:
            case vertical_rel::FrameContent:
                svgY = length (fromTop.get_value_or(length(0, length::pt)).get_value_unit(length::pt)
									+ translation->get_value_unit(length::pt)
									, length::pt); 
                break;
            default:
                break;
            }
        }
    } 
    else if (styleVerticallPos &&
        styleVerticallPos->get_type() == vertical_pos::Top && 
        pageMarginTop)
    {
        svgY = length(0, length::pt);
        if (styleVerticalRel)
        {
            switch(styleVerticalRel->get_type())
            {
            case vertical_rel::Page:
            case vertical_rel::PageContent:
            case vertical_rel::Paragraph:
                svgY = frameMarginTop;
                break;                
            case vertical_rel::ParagraphContent:
                // TODO:  get spacing property of current paragraph
                svgY = frameMarginTop /*+paragraphTopSpacing*/;
                break;
            case vertical_rel::Frame:
            case vertical_rel::FrameContent:
                svgY = frameMarginTop;
            default:
                break;
            }
        }

    } 
    else if (styleVerticallPos &&
        styleVerticallPos->get_type() == vertical_pos::Bottom && 
        pageMarginBottom)
    {
        svgY = length(0, length::pt);
        if (styleVerticallPos)
        {
            switch(styleVerticallPos->get_type())
            {
            case vertical_rel::Page:
                {
                    if (pageHeight)
                    {
                        svgY = length (pageHeight->get_value_unit(length::pt) 
										- frameHeight.get_value_or( length(0, length::pt) ).get_value_unit(length::pt)
										- frameMarginBottom.get_value_or( length(0, length::pt) ).get_value_unit(length::pt)
										, length::pt);
                    }
                }
                break;
            case vertical_rel::PageContent:
                {
                    if (pageHeight)
                    {
                        svgY = length (pageHeight->get_value_unit(length::pt)
										- pageMarginTop.get_value_or( length(0, length::pt) ).get_value_unit(length::pt)
										- pageMarginBottom.get_value_or( length(0, length::pt) ).get_value_unit(length::pt)
										- frameHeight.get_value_or( length(0, length::pt) ).get_value_unit(length::pt)
										- frameMarginBottom.get_value_or( length(0, length::pt) ).get_value_unit(length::pt)
										, length::pt);                        
                    }
                }
                break;
            case vertical_rel::Paragraph:
            case vertical_rel::ParagraphContent:
                {
                    double paragraphBottomSpacing = 0.0; // TODO
                    if (pageHeight)
                    {
                        svgY = length (pageHeight->get_value_unit(length::pt)
										- pageMarginTop.get_value_or( length(0, length::pt) ).get_value_unit(length::pt)
										- pageMarginBottom.get_value_or( length(0, length::pt) ).get_value_unit(length::pt)
										- paragraphBottomSpacing
										- frameHeight.get_value_or( length(0, length::pt) ).get_value_unit(length::pt)
										- frameMarginBottom.get_value_or( length(0, length::pt) ).get_value_unit(length::pt)
										, length::pt);                        
                    }
                }
                break;
            case vertical_rel::Frame:
            case vertical_rel::FrameContent:
                {
                    // $pageHeight - $frameHeight - $frameMarginBottom + $translation"
                    if (pageHeight)
                    {
                        svgY = length (pageHeight->get_value_unit(length::pt)
										- frameHeight.get_value_or( length(0, length::pt) ).get_value_unit(length::pt)
										- frameMarginBottom.get_value_or( length(0, length::pt) ).get_value_unit(length::pt)
										+ translation->get_value_unit(length::pt)
										, length::pt);                        
                    }
                }
                break;
            default:
                break;
            }
        }
    }
    else
    {
        if (attlists_.position_.svg_y_)
            svgY = *attlists_.position_.svg_y_;
    }

	//if (anchor_page_number && pageHeight)....так нельзя .. только в пределах текущей страницы :(
	//{
	//	svgY = length(svgY->get_value_unit(length::pt) + pageHeight->get_value_unit(length::pt) * (*anchor_page_number - 1), length::pt );
	//}
	//_CP_OPT(double) dVal;
	//if (GetProperty(additional, L"svg:translate_y", dVal))
	//{
	//	if (svgY) svgY = *svgY + *dVal;
	//	else svgY = length(*dVal, length::pt);
	//}	
	return get_value_emu(svgY);
}

}

void common_draw_docx_convert(oox::docx_conversion_context & Context, union_common_draw_attlists & attlists_, oox::_docx_drawing *drawing) 
{
	const std::wstring styleName = attlists_.shape_with_text_and_styles_.
					common_shape_draw_attlist_.draw_style_name_.get_value_or(L"");

	std::vector<const odf_reader::style_instance *> instances;
	odf_reader::style_instance* styleInst = Context.root()->odf_context().styleContainer().style_by_name(styleName, odf_types::style_family::Graphic,Context.process_headers_footers_);
	if (styleInst)
	{
		if (drawing->sub_type > 1) //without text-box
		{
			style_instance * defaultStyle = Context.root()->odf_context().styleContainer().style_default_by_type(odf_types::style_family::Graphic);
			if (defaultStyle)instances.push_back(defaultStyle);
		}
		else if (styleInst->content())
		{
			style_paragraph_properties *para_props = styleInst->content()->get_style_paragraph_properties();
			if ((para_props) && (para_props->content_.style_writing_mode_))
			{
				switch(para_props->content_.style_writing_mode_->get_type())
				{
					case writing_mode::TbLr:
						drawing->additional.push_back(odf_reader::_property(L"text_vert", 2)); break;
					case writing_mode::TbRl:
						drawing->additional.push_back(odf_reader::_property(L"text_vert", 1)); break;
				}
			}
		}
		
		instances.push_back(styleInst);
	}
	graphic_format_properties graphicProperties = calc_graphic_properties_content(instances);	

    const std::wstring pagePropertiesName = Context.get_page_properties();
    
    const style_page_layout_properties * pagePropertiesNode = NULL;
	const page_layout_instance * pageLayoutInst = Context.root()->odf_context().pageLayoutContainer().page_layout_by_name(pagePropertiesName);
    
	if (pageLayoutInst)  pagePropertiesNode = pageLayoutInst->properties();

    style_page_layout_properties_attlist	emptyPageProperties;
    
	const style_page_layout_properties_attlist &	pageProperties =
		pagePropertiesNode ? pagePropertiesNode->attlist_ : emptyPageProperties;

/////////////////////////////////////////////////////////////////////////////////////////////

    drawing->styleWrap	= graphicProperties.style_wrap_;

    if (drawing->styleWrap && drawing->styleWrap->get_type() == style_wrap::Parallel)
	{
        if (graphicProperties.style_number_wrapped_paragraphs_)
            drawing->parallel = graphicProperties.style_number_wrapped_paragraphs_->get_value();
	}

	_CP_OPT(run_through)	styleRunThrough	= graphicProperties.style_run_through_;
    _CP_OPT(anchor_type)	anchor			= attlists_.shape_with_text_and_styles_.common_text_anchor_attlist_.type_;
	
	drawing->styleHorizontalRel	= graphicProperties.common_horizontal_rel_attlist_.style_horizontal_rel_;
    drawing->styleHorizontalPos	= graphicProperties.common_horizontal_pos_attlist_.style_horizontal_pos_;
    drawing->styleVerticalPos	= graphicProperties.common_vertical_pos_attlist_.style_vertical_pos_;
    drawing->styleVerticalRel	= graphicProperties.common_vertical_rel_attlist_.style_vertical_rel_;
	
	if (!drawing->styleVerticalRel && anchor)
	{
		switch(anchor->get_type())
		{
		case anchor_type::Paragraph:	drawing->styleVerticalRel = vertical_rel::Paragraph;	break;
		case anchor_type::Page:			drawing->styleVerticalRel = vertical_rel::Page;			break;
		case anchor_type::Char:			drawing->styleVerticalRel = vertical_rel::Char;			break;
		}
	}
	if (!drawing->styleHorizontalRel && anchor)
	{
		switch(anchor->get_type())
		{
		case anchor_type::Paragraph:	drawing->styleHorizontalRel = horizontal_rel::Paragraph;	break;
		case anchor_type::Page:			drawing->styleHorizontalRel = horizontal_rel::Page;			break;
		case anchor_type::Char:			drawing->styleHorizontalRel = horizontal_rel::Char;			break;
		}
	}
	int level_drawing = Context.get_drawing_context().get_current_level();

    if (drawing->parallel == 1 || anchor && anchor->get_type() == anchor_type::AsChar || level_drawing >1 )
    {
        drawing->isInline = true;
    }
	if (attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_transform_)
	{
		std::wstring transformStr = attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_transform_.get();
		docx_convert_transforms(transformStr, drawing->additional);
	} 
	if (false == drawing->isInline)
    {
		drawing->relativeHeight	= L"2";
        drawing->behindDoc		= L"0";

		if (((drawing->styleWrap && drawing->styleWrap->get_type() == style_wrap::RunThrough) || !drawing->styleWrap) 
			&& ((styleRunThrough && styleRunThrough->get_type() == run_through::Background) || !styleRunThrough))
        {
           drawing->behindDoc = L"1";  
		   if (!drawing->styleWrap)
			   drawing->styleWrap = style_wrap(style_wrap::RunThrough);

        }
		if (!drawing->styleWrap)
			drawing->styleWrap = style_wrap(style_wrap::Parallel);//у опен офис и мс разные дефолты

        _CP_OPT(unsigned int) zIndex = attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_z_index_;
       
		if (zIndex)//порядок отрисовки объектов
        {
            if (*zIndex < 0) 
                drawing->relativeHeight = L"0";
            else
				drawing->relativeHeight = std::to_wstring( 2 + *zIndex );
        }
        drawing->margin_rect[0] = GetMargin(graphicProperties, sideLeft);
        drawing->margin_rect[1] = GetMargin(graphicProperties, sideTop);
        drawing->margin_rect[2] = GetMargin(graphicProperties, sideRight);
        drawing->margin_rect[3] = GetMargin(graphicProperties, sideBottom);

        drawing->posOffsetH = ComputeMarginX(pagePropertiesNode, pageProperties, attlists_, graphicProperties, drawing->additional);
        drawing->posOffsetV = ComputeMarginY(					pageProperties, attlists_, graphicProperties, drawing->additional);

		if (attlists_.rel_size_.style_rel_width_)
		{
			int type = attlists_.rel_size_.style_rel_width_->get_type();

			if (type == odf_types::percent_or_scale::Percent)
				drawing->pctWidth	= (int)attlists_.rel_size_.style_rel_width_->get_percent().get_value();
		}
		if (attlists_.rel_size_.style_rel_height_ )
		{
			int type = attlists_.rel_size_.style_rel_height_->get_type();

			if (type == odf_types::percent_or_scale::Percent)
				drawing->pctHeight	= (int)attlists_.rel_size_.style_rel_height_->get_percent().get_value();
		}

    }
	drawing->number_wrapped_paragraphs = graphicProperties.style_number_wrapped_paragraphs_.
									get_value_or( integer_or_nolimit( integer_or_nolimit::NoLimit) ).get_value();
	if (anchor && anchor->get_type() == anchor_type::AsChar && drawing->posOffsetV < 0)
	{
		drawing->posOffsetV = (int)(length(0.01, length::cm).get_value_unit(length::emu));
	}
//----------------------------------------------------
	graphicProperties.apply_to(drawing->additional);
//----------------------------------------------------
	bool bTxbx = (drawing->sub_type == 1);

	Compute_GraphicFill(graphicProperties.common_draw_fill_attlist_, graphicProperties.style_background_image_, Context.root()->odf_context().drawStyles(),drawing->fill, bTxbx);	

	if ((drawing->fill.bitmap) && (drawing->fill.bitmap->rId.empty()))
	{
		std::wstring href = drawing->fill.bitmap->xlink_href_;
		drawing->fill.bitmap->rId = Context.get_mediaitems()->add_or_find(href, oox::typeImage, drawing->fill.bitmap->isInternal, href, Context.get_type_place());
	}

//----------------------------------------------------
	drawing->additional.push_back(odf_reader::_property(L"border_width_left",		Compute_BorderWidth(graphicProperties, sideLeft)));
	drawing->additional.push_back(odf_reader::_property(L"border_width_top",		Compute_BorderWidth(graphicProperties, sideTop)));
	drawing->additional.push_back(odf_reader::_property(L"border_width_right",		Compute_BorderWidth(graphicProperties, sideRight)));
	drawing->additional.push_back(odf_reader::_property(L"border_width_bottom",		Compute_BorderWidth(graphicProperties, sideBottom))); 
	
	if (graphicProperties.common_border_attlist_.fo_border_)
	{
		if (graphicProperties.common_border_attlist_.fo_border_->is_none() == false)
		{
			drawing->additional.push_back(_property(L"stroke-color",	graphicProperties.common_border_attlist_.fo_border_->get_color().get_hex_value() ));
			drawing->additional.push_back(_property(L"stroke-width",	graphicProperties.common_border_attlist_.fo_border_->get_length().get_value_unit(odf_types::length::pt) ));

		}
	}
//----------------------------------------------------
//----------------------------------------------------
	if (attlists_.rel_size_.common_draw_size_attlist_.svg_width_)
	{
		double w_shape = attlists_.rel_size_.common_draw_size_attlist_.svg_width_->get_value_unit(length::pt);
		if (w_shape < 1) attlists_.rel_size_.common_draw_size_attlist_.svg_width_ = length(1, length::pt);
	}
	if (attlists_.rel_size_.common_draw_size_attlist_.svg_height_)
	{
		double h_shape = attlists_.rel_size_.common_draw_size_attlist_.svg_height_->get_value_unit(length::pt);
		if (h_shape < 1) attlists_.rel_size_.common_draw_size_attlist_.svg_height_ = length(1, length::pt);
	}
	drawing->x = get_value_emu(attlists_.position_.svg_x_);
    drawing->y = get_value_emu(attlists_.position_.svg_y_);

	drawing->cx = get_value_emu(attlists_.rel_size_.common_draw_size_attlist_.svg_width_);
    drawing->cy = get_value_emu(attlists_.rel_size_.common_draw_size_attlist_.svg_height_);

	_CP_OPT(double) dVal;
	
	GetProperty(drawing->additional, L"svg:rotate", dVal);
	if (dVal)
	{
		double new_x = (drawing->cx / 2 * cos(-(*dVal)) - drawing->cy / 2 * sin(-(*dVal)) ) - drawing->cx / 2;
		double new_y = (drawing->cx / 2 * sin(-(*dVal)) + drawing->cy / 2 * cos(-(*dVal)) ) - drawing->cy / 2;
		
		drawing->x += new_x;
		drawing->y += new_y;
	}	

	if (Context.process_headers_footers_ && drawing->posOffsetH < 0)
	{//p7офис_Альт.odt
		const _CP_OPT(length) pageMarginLeft = CalcResultLength(pageProperties.common_horizontal_margin_attlist_.fo_margin_left_, pageProperties.fo_page_width_);

		if (pageMarginLeft)
		{
			double val = pageMarginLeft->get_value_unit(length::emu);

			if (drawing->posOffsetH < - val)
				drawing->posOffsetH = -val;
		}
	}
	
	GetProperty(drawing->additional, L"svg:scale_x", dVal);
	if (dVal)drawing->cx = (int)(0.5 + drawing->cx * dVal.get());
	
	GetProperty(drawing->additional, L"svg:scale_y", dVal);
	if (dVal)drawing->cy = (int)(0.5 + drawing->cy * dVal.get());

	GetProperty(drawing->additional, L"svg:translate_x", dVal);
	if (dVal)
	{
		drawing->x += get_value_emu(dVal.get());
	}
	GetProperty(drawing->additional, L"svg:translate_y", dVal);
	if (dVal)
	{
		drawing->y += get_value_emu(dVal.get());
	}

	if (drawing->cx < 0)	//frame textbox int WORD_EXAMPLE.odt = 45 inch !!!!
	{
		drawing->cx = -drawing->cx;
		drawing->additional.push_back(_property(L"fit-to-size",	true));
	}
 
////////////////////////////////////////////////////////////////////////////////////////////////////
	if ((drawing->styleWrap) && (drawing->styleWrap->get_type() == style_wrap::Dynamic))	//автоподбор
	{
		int max_width	= get_value_emu(pageProperties.fo_page_width_);
		int max_height	= get_value_emu(pageProperties.fo_page_height_);
		
		//это бред(типо подгонка) автоподбор под размер текста ... 
		//if (Context.process_headers_footers_ && pageLayoutInst)
		//{
		//	style_header_style * headerStyle = dynamic_cast<style_header_style *>(pageLayoutInst->style_page_layout_->style_header_style_.get());
		//	style_footer_style * footerStyle = dynamic_cast<style_footer_style *>(pageLayoutInst->style_page_layout_->style_footer_style_.get());

		//	style_header_footer_properties * headerProp = headerStyle ? dynamic_cast<style_header_footer_properties *>(headerStyle->style_header_footer_properties_.get()) : NULL;
		//	style_header_footer_properties * footerProp = footerStyle ? dynamic_cast<style_header_footer_properties *>(footerStyle->style_header_footer_properties_.get()) : NULL;
		//	
		//	if (headerProp)
		//	{
		//		size_t		height	= get_value_emu(headerProp->style_header_footer_properties_attlist_.svg_height_);
		//		if (height<1)height	= get_value_emu(headerProp->style_header_footer_properties_attlist_.fo_min_height_);
		//		
		//		if (height >0 && height < max_height)
		//			max_height = height;
		//	}
		//	if (footerProp)
		//	{
		//		size_t		height	= get_value_emu(footerProp->style_header_footer_properties_attlist_.svg_height_);
		//		if (height<1)height	= get_value_emu(footerProp->style_header_footer_properties_attlist_.fo_min_height_);
		//		
		//		if (height >0 && height < max_height)
		//			max_height = height;

		//	}
		//}

		//if (drawing->cx<1 && max_width >0)
		//{
		//	drawing->cx = std::min(762000,max_width);
		//}
		//if (drawing->cy <1 && max_height >0)
		//{
		//	drawing->cy = std::min(142875,max_height);
		//}
	}
////////////////////////////////////////////////////////////////////////////////////////////////////
	if (drawing->inGroup && drawing->type != oox::typeGroupShape)
	{
        _INT32 x_group_offset, y_group_offset;
		Context.get_drawing_context().get_position_group(x_group_offset, y_group_offset);

		drawing->x -= x_group_offset;
		drawing->y -= y_group_offset;
	}

}
void draw_shape::docx_convert(oox::docx_conversion_context & Context)
{
//--------------------------------------------------------------------------------------------------
	oox::_docx_drawing drawing = oox::_docx_drawing();

	drawing.type		= oox::typeShape;
	drawing.id			= Context.get_drawing_context().get_current_shape_id();
	drawing.name		= Context.get_drawing_context().get_current_object_name();
	drawing.inGroup		= Context.get_drawing_context().in_group();
	drawing.lined		= lined_shape_;
	drawing.connector	= connector_;

	drawing.sub_type	= sub_type_;
	drawing.additional	= additional_;//сюда могут добавиться свойства ...

	if (drawing.lined == false)
	{
		drawing.additional.push_back(_property(L"text-content", Context.get_drawing_context().get_text_stream_shape()));
	}

	Context.get_drawing_context().clear_stream_shape();

/////////
	common_draw_docx_convert(Context, common_draw_attlists_, &drawing);
/////////

	if (bad_shape_ && drawing.fill.bitmap) // CV_Kucheruk_Maria(rus).odt - картинка по дебильному 
	{
		drawing.sub_type = 1;
		bad_shape_ = false;
	}

	if (drawing.fill.type < 1 && !IsExistProperty(drawing.additional, L"stroke"))//бывает что и не определено ничего 
	{
		drawing.fill.solid = oox::oox_solid_fill::create();
		drawing.fill.solid->color = L"729FCF";
		drawing.fill.type = 1;
	}

	if (bad_shape_)
	{
		Context.get_drawing_context().stop_shape();
		return;
	}

    std::wostream & strm = Context.output_stream();

	bool runState	= Context.get_run_state();
	bool paraState	= Context.get_paragraph_state();

	Context.reset_context_state();
	Context.set_run_state		(runState);	
	//Context.set_paragraph_state	(false);	

	bool new_run = false;
	
	if ((paraState == false && Context.get_drawing_context().get_current_level() == 1) || (Context.get_drawing_context().in_group()))
	{
	}
	else
	{
		if (!Context.get_drawing_context().in_group() && !runState)
		{
			if (!paraState)
			{
				Context.start_paragraph();
			}
			Context.add_new_run(L"");

			new_run = true;
		}
	}

	drawing.serialize(strm/*, Context.get_drawing_state_content()*/);

	if (new_run)
	{
		Context.finish_run();
		if (!paraState)
		{
			Context.finish_paragraph();
		}
	}

	//Context.set_paragraph_state(paraState);		
	Context.back_context_state();

	Context.get_drawing_context().stop_shape();
}

void draw_image::docx_convert(oox::docx_conversion_context & Context)
{
	const draw_frame *frame = Context.get_drawing_context().get_current_frame();//owner
	
	if (!frame && draw_frame_ptr)
	{
		draw_frame *frame = dynamic_cast<draw_frame *>(draw_frame_ptr.get());
		if (frame)
		{
			office_element_ptr elm = office_element_ptr(new draw_image(*this));
			draw_image *image = dynamic_cast<draw_image *>(elm.get());
			image->draw_frame_ptr = office_element_ptr();

			frame->content_.push_back(elm);
			frame->docx_convert(Context);
		}
		return;
	}
//-----------------------------------------------------------------------------------------------	
	if (!frame)
		return;

	if (!xlink_attlist_.href_)
		return;

	std::wstring href = xlink_attlist_.href_.get_value_or(L"");

	oox::_docx_drawing * drawing = dynamic_cast<oox::_docx_drawing *>(frame->oox_drawing_.get()); 
	if (!drawing) 
		return;

	size_t pos_replaicement	= href.find(L"ObjectReplacements"); 
	if (pos_replaicement != std::wstring::npos)
	{
		if (!Context.get_drawing_context().get_use_image_replace())
			return; //skip replacement image (math, chart, ...)  - возможно записать как альтернативный контент - todooo ???
		if (href.length() - (pos_replaicement + 18) < 2)
			return; //href="./ObjectReplacements/"
	}
	if (href[0] == L'#') href = href.substr(1);

	if (drawing->type == oox::typeUnknown)
		drawing->type = oox::typeImage;

	oox::StreamsManPtr prev = Context.get_stream_man();
	
	std::wstringstream temp_stream(Context.get_drawing_context().get_text_stream_frame());
	Context.set_stream_man( boost::shared_ptr<oox::streams_man>( new oox::streams_man(temp_stream) ));
  
	Context.reset_context_state();

	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->docx_convert(Context);
    }

	Context.back_context_state();

	Context.get_drawing_context().get_text_stream_frame() = temp_stream.str();
	Context.set_stream_man(prev);
	
//--------------------------------------------------
	oox::hyperlinks::_ref hyperlink = Context.last_hyperlink();
	//нужно еще систему конроля - могут придте уже "использованные" линки с картинок - из колонтитулов (но на них уже использовали релсы)
	//дыра осталась если картинка в картинке - линк продублируется с внутренней на внешнюю 
	
	if (hyperlink.drawing == true && hyperlink.used_rels == false)
	{//link from object
		drawing->action.enabled = true;
		drawing->action.hId		= hyperlink.id;
		drawing->action.hRef	= hyperlink.href;
		drawing->action.typeRels= oox::typeHyperlink;
	}
/////////
	drawing->fill.bitmap = oox::oox_bitmap_fill::create();
	drawing->fill.type = 2;
	drawing->fill.bitmap->isInternal = false;
    drawing->fill.bitmap->rId = Context.get_mediaitems()->add_or_find(href, oox::typeImage, drawing->fill.bitmap->isInternal, href, Context.get_type_place());
	drawing->fill.bitmap->bStretch = true;

    const std::wstring styleName = frame->common_draw_attlists_.shape_with_text_and_styles_.
									common_shape_draw_attlist_.draw_style_name_.get_value_or(L"");

	odf_reader::style_instance* styleInst = Context.root()->odf_context().styleContainer().style_by_name(styleName, odf_types::style_family::Graphic,Context.process_headers_footers_);
	
	odf_reader::graphic_format_properties *properties = NULL;
	if (styleInst) properties = styleInst->content()->get_graphic_properties();
////////////////
	if (properties)
	{
		if (properties->fo_clip_ && drawing->fill.bitmap)
		{
			std::wstring strRectClip = properties->fo_clip_.get();
			strRectClip = strRectClip.substr(5, strRectClip.length() - 6);
			
			std::wstring fileName = Context.root()->get_folder() + FILE_SEPARATOR_STR + xlink_attlist_.href_.get_value_or(L"");
			
			drawing->fill.bitmap->bCrop = parse_clipping(strRectClip, fileName, drawing->fill.bitmap->cropRect, Context.get_mediaitems()->applicationFonts());
		}
		if (properties->common_draw_fill_attlist_.draw_luminance_)
		{
			drawing->fill.bitmap->luminance = properties->common_draw_fill_attlist_.draw_luminance_->get_value();
		}
		if (properties->common_draw_fill_attlist_.draw_contrast_)
		{
			drawing->fill.bitmap->contrast = properties->common_draw_fill_attlist_.draw_contrast_->get_value();
		}
	}
}

void draw_text_box::docx_convert(oox::docx_conversion_context & Context)
{
	const draw_frame *frame = Context.get_drawing_context().get_current_frame();//owner

	if (!frame && draw_frame_ptr)
	{
		draw_frame *frame = dynamic_cast<draw_frame *>(draw_frame_ptr.get());
		if (frame)
		{
			office_element_ptr elm = office_element_ptr(new draw_text_box(*this));
			draw_text_box *text_box = dynamic_cast<draw_text_box *>(elm.get());
			text_box->draw_frame_ptr = office_element_ptr();

			frame->content_.push_back(elm);
			frame->docx_convert(Context);

		}
		return;
	}
//---------------------------------------------------------------------------------------------------------------
	//тут может быть не только текст , но и таблицы, другие объекты ...
 	oox::StreamsManPtr prev = Context.get_stream_man();
	
	std::wstringstream temp_stream(Context.get_drawing_context().get_text_stream_frame());
	Context.set_stream_man( boost::shared_ptr<oox::streams_man>( new oox::streams_man(temp_stream) ));	
	
	Context.reset_context_state();
	
	bool drState = Context.get_drawing_state_content();
	
	Context.set_drawing_state_content(true);
	for (size_t i = 0; i < content_.size(); i++)
    {
		ElementType type = content_[i]->get_type();
        content_[i]->docx_convert(Context);
    }
	
	Context.get_drawing_context().get_text_stream_frame() = temp_stream.str();
	Context.set_stream_man(prev);
	
	Context.set_drawing_state_content	(drState);
	Context.back_context_state();

//---------------------------------------------------------------------------------------------------------
	if (!frame)
		return;

	oox::_docx_drawing * drawing = dynamic_cast<oox::_docx_drawing *>(frame->oox_drawing_.get()); 
	if (!drawing) 
		return;

	drawing->type		= oox::typeShape;
	drawing->sub_type	= 1;	//textBox

	const std::wstring & content = Context.get_drawing_context().get_text_stream_frame();
	
	drawing->additional.push_back(_property(L"text-content", content));
	Context.get_drawing_context().clear_stream_frame();

/////////
	bool auto_fit_text = false;
	bool auto_fit_shape = false;

	if (!draw_text_box_attlist_.fo_min_height_)		draw_text_box_attlist_.fo_min_height_	= frame->draw_frame_attlist_.fo_min_height_;
	if (!draw_text_box_attlist_.fo_min_width_)		draw_text_box_attlist_.fo_min_width_	= frame->draw_frame_attlist_.fo_min_width_;

	if ((draw_text_box_attlist_.fo_min_height_) && (draw_text_box_attlist_.fo_min_height_->get_type()==length_or_percent::Length))
	{
		size_t min_y = get_value_emu(draw_text_box_attlist_.fo_min_height_->get_length());
		if (drawing->cy < min_y) 
		{
			drawing->cy = min_y;
		}
		//if (drawing->cy < 36000)
		//{
		//	auto_fit_shape = true;
		//}
		auto_fit_shape = true;
	}
	else if ((frame->draw_frame_attlist_.fo_min_height_) && (draw_text_box_attlist_.fo_min_height_->get_type()==length_or_percent::Length))
	{
		size_t min_y = get_value_emu(frame->draw_frame_attlist_.fo_min_height_->get_length());
		if (drawing->cy < min_y) 
		{
			drawing->cy = min_y;
		}
		auto_fit_shape = true;
	}
	else if ((frame->common_draw_attlists_.rel_size_.style_rel_height_) && (frame->common_draw_attlists_.rel_size_.style_rel_height_->get_type() == percent_or_scale::ScaleMin))
	{
		auto_fit_shape = true;
	}
	
	if ((draw_text_box_attlist_.fo_min_width_) && (draw_text_box_attlist_.fo_min_width_->get_type()==length_or_percent::Length))
	{
		size_t min_x = get_value_emu(draw_text_box_attlist_.fo_min_width_->get_length());
		if (drawing->cx < min_x)
		{
			auto_fit_text = true;
			drawing->cx = min_x;
		}
		if (drawing->cx < 36000)
		{
			auto_fit_text = false;
			auto_fit_shape = true;
		}
	}


	if ((draw_text_box_attlist_.fo_max_height_) && (draw_text_box_attlist_.fo_max_height_->get_type()==length_or_percent::Length))
	{
		size_t max_y = get_value_emu(draw_text_box_attlist_.fo_max_height_->get_length());
		if (drawing->cy > max_y) drawing->cy = max_y;
	}
	if ((draw_text_box_attlist_.fo_max_width_) && (draw_text_box_attlist_.fo_max_width_->get_type()==length_or_percent::Length))
	{
		size_t max_x = get_value_emu(draw_text_box_attlist_.fo_max_width_->get_length());
		if (drawing->cx > max_x) drawing->cy = max_x;
	}
	if (auto_fit_shape)
	{
		drawing->additional.push_back(_property(L"text-wrap"	, 0));
		drawing->additional.push_back(_property(L"auto-grow-height", auto_fit_shape));
	}
	else if (auto_fit_text)
		drawing->additional.push_back(_property(L"fit-to-size",	auto_fit_text));

	if (drawing->cx < 1 && drawing->cy < 1)
	{
		drawing->cx = 10;
		drawing->cy = 10;
	}
}
void draw_g::docx_convert(oox::docx_conversion_context & Context)
{
	if ((!Context.get_paragraph_state() && !Context.get_drawing_context().in_group()) && !Context.delayed_converting_)
    {
        Context.add_delayed_element(this);
        return;
    }

	if (object_index >= 0) //только в документах нельзя объект объединять с шейпами в группы (
	{
		draw_frame *frame = dynamic_cast<draw_frame*>(content_[object_index].get());

		frame->common_draw_attlists_.shape_with_text_and_styles_.common_text_anchor_attlist_.type_ =
				common_draw_attlists_.shape_with_text_and_styles_.common_text_anchor_attlist_.type_;
		
		for (size_t i = 0; i < frame->content_.size(); i++)
		{
			draw_object *object = dynamic_cast<draw_object*>(frame->content_[i].get());
			if (!object)continue;

			for (size_t j = 0; j < content_.size(); j++)
			{
				if (j == object_index) continue;

				object->content_.push_back(content_[j]);

			}	
			break;
		}
		frame->docx_convert(Context);

		return;
	}
	
	oox::_docx_drawing drawing = oox::_docx_drawing();
	
	drawing.inGroup	= Context.get_drawing_context().in_group();
	drawing.type	= oox::typeGroupShape;
	
	Context.get_drawing_context().start_group();
	
    const _CP_OPT(std::wstring) name = 
        common_draw_attlists_.shape_with_text_and_styles_.
        common_shape_draw_attlist_.draw_name_;
	
	Context.get_drawing_context().add_name_object(name.get_value_or(L"Group"));

	drawing.id		= Context.get_drawing_context().get_current_shape_id();
	drawing.name	= Context.get_drawing_context().get_current_object_name();

//--------------------------------------------------
	common_draw_docx_convert(Context, common_draw_attlists_, &drawing);
//--------------------------------------------------
	oox::StreamsManPtr prev = Context.get_stream_man();
	
	std::wstringstream temp_stream(drawing.content_group_);
	Context.set_stream_man( boost::shared_ptr<oox::streams_man>( new oox::streams_man(temp_stream) ));	
	
	Context.reset_context_state();
		
	if (position_child_x1 != 0x7fffffff && position_child_y1 != 0x7fffffff )
	{
		Context.get_drawing_context().set_position_child_group	(position_child_x1, position_child_y1);

		if (position_child_x2 != 0x7fffffff && position_child_y2 != 0x7fffffff )
		{
			Context.get_drawing_context().set_size_child_group	(position_child_x2 - position_child_x1, position_child_y2 - position_child_y1);
		}
	}

	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->docx_convert(Context);
    }
	drawing.content_group_ = temp_stream.str();
	
	Context.set_stream_man(prev);	
	Context.back_context_state();

//--------------------------------------------------
	Context.get_drawing_context().get_size_group	(drawing.cx	, drawing.cy);
	Context.get_drawing_context().get_position_group(drawing.x	, drawing.y);

	Context.get_drawing_context().stop_group();    	
	
	if (drawing.inGroup)
	{
        _INT32 x_group_offset, y_group_offset;
		Context.get_drawing_context().get_position_group(x_group_offset, y_group_offset);

		drawing.x -= x_group_offset;
		drawing.y -= y_group_offset;
	}
	else
	{
		drawing.posOffsetH = drawing.x;
		drawing.posOffsetV = drawing.y;

		drawing.x = drawing.y = 0;
	}
	
//--------------------------------------------------
    std::wostream & strm = Context.output_stream();

	bool runState	= Context.get_run_state();	
	bool pState		= Context.get_paragraph_state();

	if (!Context.get_drawing_context().in_group() && !pState)
	{
		Context.start_paragraph();
		Context.set_paragraph_keep(true);
		pState	= Context.get_paragraph_state();
	}

	Context.set_paragraph_state(false);

	if (!Context.get_drawing_context().in_group())
		Context.add_new_run(_T(""));
	
	drawing.serialize(strm/*, Context.get_drawing_state_content()*/);
 	
	if (!Context.get_drawing_context().in_group())
		Context.finish_run();

	Context.set_paragraph_state(pState);
//--------------------------------------------------
}
void draw_frame::docx_convert(oox::docx_conversion_context & Context)
{
	bool bImage = false;
	if (content_.empty() == false)
	{
		if (content_[0]->get_type() == typeDrawImage || content_[0]->get_type() == typeDrawObject || content_[0]->get_type() == typeDrawObjectOle )
			bImage = true;
	}
	if (Context.get_drawing_context().get_current_level() > 0 && !Context.get_drawing_context().in_group() && !bImage)
    {
        Context.add_delayed_element(this);
        return;
    }

	oox_drawing_ = oox_drawing_ptr(new oox::_docx_drawing());

    Context.get_drawing_context().start_frame(this);
    
    const _CP_OPT(std::wstring) name = 
        common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_name_;
	
	Context.get_drawing_context().add_name_object(name.get_value_or(L"Object"));
	
	oox::_docx_drawing* drawing = dynamic_cast<oox::_docx_drawing *>(oox_drawing_.get()); 
	
	drawing->id			= Context.get_drawing_context().get_current_frame_id();
	drawing->name		= Context.get_drawing_context().get_current_object_name();
	drawing->inGroup	= Context.get_drawing_context().in_group();
	
	common_draw_docx_convert(Context, common_draw_attlists_, drawing);
//-----------------------------------------------------------------------------------------------------

	for (size_t i = 0 ; i < content_.size(); i++)
    {
		content_[i]->docx_convert(Context);
    }

//-----------------------------------------------------------------------------------------------------
	bool runState	= Context.get_run_state();
	bool paraState	= Context.get_paragraph_state();

	Context.reset_context_state();

	if (!Context.get_drawing_context().in_group() && !runState)
	{
		if (!paraState)//0115GS3-KeyboardShortcuts.odt
		{
			Context.start_paragraph();
		}
		Context.add_new_run(L"");
	}

	drawing->serialize(Context.output_stream()/*, Context.get_drawing_state_content()*/);

	if (!Context.get_drawing_context().in_group() && !runState)
	{
		Context.finish_run();
		if (!paraState)//0115GS3-KeyboardShortcuts.odt
		{
			Context.finish_paragraph();
		}
	}

	Context.back_context_state();

	Context.get_drawing_context().stop_frame();
}

void draw_object::docx_convert(oox::docx_conversion_context & Context)
{
	const draw_frame *frame = Context.get_drawing_context().get_current_frame();//owner

	if (!frame && draw_frame_ptr)
	{
		draw_frame *frame = dynamic_cast<draw_frame *>(draw_frame_ptr.get());
		if (frame)
		{
			office_element_ptr elm = office_element_ptr(new draw_object(*this));
			draw_object *object = dynamic_cast<draw_object *>(elm.get());
			object->draw_frame_ptr = office_element_ptr();

			frame->content_.push_back(elm);
			frame->docx_convert(Context);
		}
		return;
	}
//-----------------------------------------------------------------------------------------------	
	if (!frame)
		return;

	oox::_docx_drawing *drawing =  dynamic_cast<oox::_docx_drawing *>(frame->oox_drawing_.get());
	try
	{
        std::wstring href		= xlink_attlist_.href_.get_value_or(L"");
		std::wstring tempPath	= Context.root()->get_temp_folder();
		std::wstring odfPath	= Context.root()->get_folder();

		if (!odf_document_ && false == href.empty())
		{
			if (href[0] == L'#') href = href.substr(1);
			std::wstring objectPath = odfPath + FILE_SEPARATOR_STR + href;

			// normalize path ???? todooo
			XmlUtils::replace_all( objectPath, FILE_SEPARATOR_STR + std::wstring(L"./"), FILE_SEPARATOR_STR);

            odf_document_ = odf_document_ptr(new odf_document(objectPath, tempPath, L""));
		}
//---------------------------------------------------------------------------------------------------------------------
		office_element* contentSubDoc	= odf_document_ ? odf_document_->get_impl()->get_content() : NULL;
		
		object_odf_context	objectBuild (href);
		if (contentSubDoc)
		{
			process_build_object process_build_object_(objectBuild, odf_document_->odf_context());
			contentSubDoc->accept(process_build_object_); 

			if (objectBuild.table_table_)
			{
				oox::xlsx_conversion_context xlsx_context(odf_document_.get());
				cpdoccore::oox::package::xlsx_document outputXlsx;
	
				xlsx_context.set_output_document (&outputXlsx);

				xlsx_context.start_document();
					objectBuild.table_table_->xlsx_convert(xlsx_context);
				xlsx_context.end_document();
				
				std::wstring href_folder = tempPath + FILE_SEPARATOR_STR + L"temp_xlsx";
				NSDirectory::CreateDirectory(href_folder);
				outputXlsx.write(href_folder);

				std::wstring href = L"Microsoft_Excel_Worksheet_" + std::to_wstring(Context.get_mediaitems()->count_object + 1) + L".xlsx";
				
				COfficeUtils oCOfficeUtils(NULL);
				if (S_OK == oCOfficeUtils.CompressFileOrDirectory(href_folder, odfPath + FILE_SEPARATOR_STR + href, true))
				{				
					objectBuild.embeddedData = href;
				}
			}

			if (false == content_.empty())
			{
				oox::xlsx_conversion_context xlsx_context(Context.root());

				xlsx_context.set_drawing_context_handle(Context.get_chart_drawing_handle());
				xlsx_context.set_mediaitems(Context.get_mediaitems());

				xlsx_context.get_table_context().start_table(L"", L"", 0);

				for (size_t i = 0; i < content_.size(); ++i)
				{
					xlsx_context.get_drawing_context().set_rel_anchor(drawing->cx, drawing->cy);
					content_[i]->xlsx_convert(xlsx_context);
				}
				xlsx_context.get_table_context().end_table();

				xlsx_context.get_drawing_context().process_objects(xlsx_context.get_table_metrics());
				
				std::wstringstream strm;
				xlsx_context.get_drawing_context().serialize(strm, L"cdr");
		        
				const std::pair<std::wstring, std::wstring> drawingName =
					xlsx_context.get_drawing_context_handle()->add_drawing_xml(strm.str(), xlsx_context.get_drawing_context().get_drawings(), oox::typeChartUserShapes );

				objectBuild.userShapes = drawingName;
			}

			objectBuild.docx_convert(Context);		
		}		
//------------------------------------------------------------------------------------------------------------
		if (!frame || !drawing)
		{
			objectBuild.object_type_ = 0;
		}

		if (objectBuild.object_type_ == 1) //диаграмма
		{	
			drawing->type = oox::typeChart;
			
			bool isMediaInternal = true;        
			drawing->objectId = Context.get_mediaitems()->add_or_find(href, drawing->type, isMediaInternal, href, Context.get_type_place());
		}
		else if (objectBuild.object_type_ == 2 ) //embedded text
		{	
			//text in text not support
		}
		else if (objectBuild.object_type_ == 3) //мат формулы
		{
			const std::wstring & content = Context.get_drawing_context().get_text_stream_frame();

			bool in_frame	= !drawing->isInline;
			
			bool runState	= Context.get_run_state();
			bool pState		= Context.get_paragraph_state();
			
			if (drawing->fill.type > 0)
				in_frame = true;
			
			if (in_frame)
			{
				drawing->type	= oox::typeShape;		
				
				drawing->additional.push_back(_property(L"fit-to-size",	true));		
				drawing->additional.push_back(_property(L"text-content",	std::wstring(L"<w:p><m:oMathPara><m:oMathParaPr/>") + 
																	content + std::wstring(L"</m:oMathPara></w:p>")));
			}
			else
			{//in text			
				drawing->type = oox::typeUnknown;	 //not drawing	
				
				if (runState) Context.finish_run();
				//if (pState == false)
				{
					Context.output_stream() << L"<m:oMathPara>";
					Context.output_stream() << L"<m:oMathParaPr/>";
				}
				Context.output_stream() << content;

				//if (pState == false)
				{
					Context.output_stream() << L"</m:oMathPara>";
				}
				if (runState) Context.add_new_run(_T(""));
			}
			Context.get_drawing_context().clear_stream_frame();						
		}
		else if (objectBuild.object_type_ == 4) //embedded sheet
		{	
			bool & use_image_replace = Context.get_drawing_context().get_use_image_replace();
			use_image_replace = true;

			std::wstring href_new = office_convert(odf_document_, 2);

			if (!href_new.empty())
			{
				drawing->type = oox::typeMsObject;
				bool isMediaInternal = true;        
				
				href += FILE_SEPARATOR_STR + href_new;
				drawing->objectId		= Context.get_mediaitems()->add_or_find(href, drawing->type, isMediaInternal, href, Context.get_type_place());
				drawing->objectProgId	= L"Excel.Sheet.12";
			}
		}
		else
		{
			//замещающая картинка(если она конечно присутствует)
			bool & use_image_replace = Context.get_drawing_context().get_use_image_replace();
			use_image_replace = true;
		}
	}
    catch(...)
    {
        _CP_LOG << "[error] : convert draw::object error" << std::endl;
    }
}

void draw_object_ole::docx_convert(oox::docx_conversion_context & Context)
{
	bool & use_image_replace = Context.get_drawing_context().get_use_image_replace();
	use_image_replace = true;

//------------------------------------------------
	std::wstring href		= xlink_attlist_.href_.get_value_or(L"");
	std::wstring folderPath = Context.root()->get_folder();
	std::wstring objectPath = folderPath + FILE_SEPARATOR_STR + href;

	if (href.empty()) return;

	draw_frame*	frame = Context.get_drawing_context().get_current_frame();		//owner
	if (!frame) return;
	
	oox::_docx_drawing * drawing = dynamic_cast<oox::_docx_drawing *>(frame->oox_drawing_.get());
	if (!drawing) return;
			
	std::wstring extension;
	detectObject(objectPath, drawing->objectProgId, extension, drawing->type);

	NSFile::CFileBinary::Copy(objectPath, objectPath + extension);

	bool isMediaInternal	= true;
	drawing->objectId = Context.get_mediaitems()->add_or_find(href + extension, drawing->type, isMediaInternal, href, Context.get_type_place());

}
void draw_control::docx_convert(oox::docx_conversion_context & Context)
{
	if (!control_id_) return;

	oox::forms_context::_state & state = Context.get_forms_context().get_state_element(*control_id_);
	if (state.id.empty()) return;

	if ((state.type == 6 || state.type == 4) && state.element)
	{
		return state.element->docx_convert_sdt(Context, this);
	}

	Context.get_drawing_context().start_shape(NULL);    
	Context.get_drawing_context().add_name_object(state.name.empty() ? L"Control" : state.name);

//--------------------------------------------------------------------------------------------------
	oox::_docx_drawing drawing = oox::_docx_drawing();

	drawing.type	= oox::typeShape;
	drawing.id		= Context.get_drawing_context().get_current_shape_id();
	drawing.name	= Context.get_drawing_context().get_current_object_name();
	drawing.inGroup	= Context.get_drawing_context().in_group();
	drawing.sub_type = 1;
	
//---------------------------------------------------------------------------------------------------------
 		oox::StreamsManPtr prev = Context.get_stream_man();
		
		std::wstringstream temp_stream(Context.get_drawing_context().get_text_stream_shape());
		Context.set_stream_man( boost::shared_ptr<oox::streams_man>( new oox::streams_man(temp_stream) ));	
		
		Context.reset_context_state();

		bool drState = Context.get_drawing_state_content();
		
		Context.set_drawing_state_content(true);

		Context.start_paragraph(false);
		
		if (common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_text_style_name_)
		{	
			text::paragraph_attrs attrs_;
			attrs_.text_style_name_ = *common_draw_attlists_.shape_with_text_and_styles_.common_shape_draw_attlist_.draw_text_style_name_;

			int textStyle = Context.process_paragraph_attr(&attrs_);
		}	
		
		if (state.element)
		{
			state.element->docx_convert_sdt(Context, this);
			//state.element->docx_convert_field(Context, this);
		}
		else
		{	
			std::wstring text;
			if (!state.label.empty())		text =  state.label;
			else if (!state.value.empty())	text =  state.value;

			Context.add_new_run(L"");
				Context.output_stream() << L"<w:t xml:space=\"preserve\">";
				Context.output_stream() << XmlUtils::EncodeXmlString( text, true );
				Context.output_stream() << L"</w:t>";
			Context.finish_run();
		}
		Context.finish_paragraph();
		
		Context.get_drawing_context().get_text_stream_shape() = temp_stream.str();
		Context.set_stream_man(prev);
		
		Context.set_drawing_state_content(drState);
		
		Context.back_context_state();

		const std::wstring & content = Context.get_drawing_context().get_text_stream_shape();
		
		drawing.additional.push_back(_property(L"text-content",content));
		Context.get_drawing_context().clear_stream_shape();

		drawing.additional.push_back(_property(L"text-padding-left", 0.));
		drawing.additional.push_back(_property(L"text-padding-top", 0.));
		drawing.additional.push_back(_property(L"text-padding-right", 0.));
		drawing.additional.push_back(_property(L"text-padding-bottom", 0.));
	
//---------------------------------------------------------------------------------------------------------

/////////
	common_draw_docx_convert(Context, common_draw_attlists_, &drawing);
/////////

    std::wostream & strm = Context.output_stream();

	bool pState		= Context.get_paragraph_state();
	bool runState	= Context.get_run_state();
	bool keepState	= Context.get_paragraph_keep();

	//Context.set_run_state		(false);	
	Context.set_paragraph_state	(false);	

	bool new_run = false;
	
	if ((pState == false && Context.get_drawing_context().get_current_level() == 1) || (Context.get_drawing_context().in_group()))
	{
	}
	else
	{
		if (!Context.get_drawing_context().in_group() && !runState)
		{
			if (!pState)
			{
				Context.start_paragraph();
			}
			Context.add_new_run(L"");

			new_run = true;
		}
	}

	drawing.serialize(strm/*, Context.get_drawing_state_content()*/);

	if (new_run)
	{
		Context.finish_run();
		if (!pState)
		{
			Context.finish_paragraph();
		}
	}

	Context.set_paragraph_state(pState);	

	Context.get_drawing_context().stop_shape();
}
void draw_param::docx_convert(oox::docx_conversion_context & Context)
{
	if (!draw_name_ && !draw_value_) return;

	//Context.get_drawing_context().set_media_param(*draw_name_, *draw_value_);
}

void draw_plugin::docx_convert(oox::docx_conversion_context & Context)
{
	bool & use_image_replace = Context.get_drawing_context().get_use_image_replace();
	use_image_replace = true;

//------------------------------------------------
	std::wstring href		= xlink_attlist_.href_.get_value_or(L"");
	std::wstring folderPath = Context.root()->get_folder();
	std::wstring objectPath = folderPath + FILE_SEPARATOR_STR + href;

	if (href.empty()) return;

	draw_frame*	frame = Context.get_drawing_context().get_current_frame();		//owner
	if (!frame) return;
	
	oox::_docx_drawing * drawing = dynamic_cast<oox::_docx_drawing *>(frame->oox_drawing_.get());
	if (!drawing) return;
	
	drawing->type = Context.get_mediaitems()->detectMediaType(objectPath); //reset from Media to Audio, Video, ... QuickTime? AudioCD? ... 
	//drawing->action.enabled = true;
	//drawing->action.action	= L"ppaction://media";

	bool isMediaInternal = true;
	drawing->objectId = Context.get_mediaitems()->add_or_find(href, drawing->type, isMediaInternal, href, Context.get_type_place());

	drawing->extId = L"ext" + drawing->objectId;
	Context.get_mediaitems()->add_rels(isMediaInternal, drawing->extId, href, oox::typeMedia, Context.get_type_place());
	drawing->extExternal = !isMediaInternal;
	
	if (!drawing->fill.bitmap)
	{
		drawing->fill.type = 2;
		drawing->fill.bitmap = oox::oox_bitmap_fill::create();
		drawing->fill.bitmap->xlink_href_ = L"zero.png";
		
		_image_file_::GenerateZeroImage(folderPath + FILE_SEPARATOR_STR + L"zero.png");
	}
	std::wstring ref_image;
	bool isMediaInternal_image = true;
	drawing->fill.bitmap->rId = Context.get_mediaitems()->add_or_find(drawing->fill.bitmap->xlink_href_, oox::typeImage, isMediaInternal_image, ref_image, Context.get_type_place());		
	
//params	
	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->docx_convert(Context);
    }
}
}
}
