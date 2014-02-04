#include "precompiled_cpodf.h"
#include "draw_frame.h"

#include <ostream>
#include <sstream>
#include <string>

#include <boost/foreach.hpp>
#include <boost_string.h>
#include <regex.h>

#include "serialize_elements.h"

#include "length.h"
#include "borderstyle.h"
#include "odfcontext.h"
#include "odf_document_impl.h"
#include <cpdoccore/common/boost_filesystem_version.h>

#include <cpdoccore/odf/odf_document.h>

#include "draw_common.h"
#include "../docx/docx_drawing.h"
#include "chart_build_oox.h"

#include "calcs_styles.h"

namespace cpdoccore { 
namespace odf {

namespace fs = ::boost::filesystem;

namespace {
bool IsExistProperty(std::vector<_property> Heap,const std::wstring Name)
{
	BOOST_FOREACH(_property const & p, Heap)
	{
		int res = p.name_.find(Name);
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



length ComputeContextWidth(const style_page_layout_properties * pagePropertiesNode,
                           const style_page_layout_properties_attlist & pageProperties,
                           const union_common_draw_attlists  & attlists_,
                           const _CP_OPT(horizontal_rel) & horizontalRel,
                           const _CP_OPT(horizontal_pos) & horizontalPos,
                           const _CP_OPT(length) & pageWidth,
                           const _CP_OPT(length) & pageMarginLeft,
                           const _CP_OPT(length) & pageMarginRight
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
                    = dynamic_cast<const style_columns*>( pagePropertiesNode->style_page_layout_properties_elements_.style_columns_.get()))
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

int ComputeMarginX(const style_page_layout_properties * pagePropertiesNode,
                      const style_page_layout_properties_attlist & pageProperties,
                      const union_common_draw_attlists  & attlists_,
                      const graphic_format_properties & graphicProperties)
{
    const _CP_OPT(anchor_type) anchor = 
        attlists_.shape_with_text_and_styles_.
        common_draw_shape_with_styles_attlist_.
        common_text_spreadsheet_shape_attlist_.
        common_text_anchor_attlist_.
        type_;

    _CP_OPT(horizontal_rel) styleHorizontalRel = graphicProperties.common_horizontal_rel_attlist_.style_horizontal_rel_;
    _CP_OPT(horizontal_pos) styleHorizontalPos = graphicProperties.common_horizontal_pos_attlist_.style_horizontal_pos_;

    const _CP_OPT(length) pageWidth = pageProperties.fo_page_width_;
    const _CP_OPT(length) pageMarginLeft = CalcResultLength(pageProperties.common_horizontal_margin_attlist_.fo_margin_left_, pageWidth);
    const _CP_OPT(length) pageMarginRight = CalcResultLength(pageProperties.common_horizontal_margin_attlist_.fo_margin_right_, pageWidth);

	length contextWidth = ComputeContextWidth(pagePropertiesNode, pageProperties, attlists_,
                           styleHorizontalRel, styleHorizontalPos, pageWidth, pageMarginLeft,pageMarginRight);

    
    _CP_OPT(length) contextSubstractedValue(0, length::pt);
    _CP_OPT(style_wrap) styleWrap = graphicProperties.style_wrap_;
    if (!styleWrap || 
        styleWrap->get_type() == style_wrap::None ||
        styleWrap->get_type() == style_wrap::RunThrough)
    {
        // TODO contextSubstractedValue
    }

    const _CP_OPT(length) frameMarginLeft = GetOnlyLength(graphicProperties.common_horizontal_margin_attlist_.fo_margin_left_);
    const _CP_OPT(length) frameMarginRight = GetOnlyLength(graphicProperties.common_horizontal_margin_attlist_.fo_margin_right_);

	const _CP_OPT(length) frameWidth = attlists_.rel_size_.common_draw_size_attlist_.svg_width_;
    const _CP_OPT(length) frameHeight = attlists_.rel_size_.common_draw_size_attlist_.svg_height_;

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
                    svgX = fromLeft; // + translation
                }
                else if (horRel == horizontal_rel::PageEndMargin)
                {
                    svgX = length(contextWidth.get_value_unit(length::pt) + 
                        fromLeft.get_value_or(length(0, length::pt)).get_value_unit(length::pt), length::pt); // + translation
                }
                else if (anchor && anchor->get_type() == anchor_type::Page)
                {
                    svgX = fromLeft; // + translation
                }
                else if (horRel == horizontal_rel::Paragraph ||
                    horRel == horizontal_rel::ParagraphContent ||
                    horRel == horizontal_rel::ParagraphStartMargin)
                {
                    // TODO
                    double paragraphLeftIndent = 0;
                    svgX = length(paragraphLeftIndent - 
                        contextSubstractedValue.get_value_or(length(0, length::pt)).get_value_unit(length::pt) +
                        fromLeft.get_value_or(length(0, length::pt)).get_value_unit(length::pt)
                        ,length::pt); // + translation
                                        
                }
                else if (horRel == horizontal_rel::ParagraphEndMargin)
                {
                    // TODO
                    double paragraphRightIndent = 0;
                    svgX = length(contextWidth.get_value_unit(length::pt) - paragraphRightIndent - 
                        contextSubstractedValue.get_value_or(length(0, length::pt)).get_value_unit(length::pt) +
                        fromLeft.get_value_or(length(0, length::pt)).get_value_unit(length::pt)
                        ,length::pt); // + translation
                }
                else if (horRel == horizontal_rel::Frame ||
                    horRel == horizontal_rel::FrameContent ||
                    horRel == horizontal_rel::FrameStartMargin ||
                    horRel == horizontal_rel::FrameEndMargin)
                {
                    svgX = fromLeft; // + translation
                }
                else if (horRel == horizontal_rel::Char)
                {
                    svgX = fromLeft; // + translation                                    
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
                    svgX = frameMarginLeft.get_value_or( length(0, length::pt )); // + translation
                }
                else if (horRel == horizontal_rel::PageEndMargin)
                {
                    if (frameWidth.get_value_or( length(0, length::pt )).get_value_unit(length::pt) <
                        pageMarginRight.get_value_or( length(0, length::pt )).get_value_unit(length::pt))
                    {
                        // TODO
                        svgX = length(
                            contextWidth.get_value_unit(length::pt) // - contextSubstractedValue
                            + frameMarginLeft.get_value_or( length(0, length::pt )).get_value_unit(length::pt),
                            length::pt                                
                            );
                    }
                    else
                    {
                        svgX = length(
                            pageWidth.get_value_or( length(0, length::pt )).get_value_unit(length::pt) - 
                            frameWidth.get_value_or( length(0, length::pt )).get_value_unit(length::pt) -
                            frameMarginLeft.get_value_or( length(0, length::pt )).get_value_unit(length::pt),
                            length::pt);
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
                    svgX = length(
                        frameMarginLeft.get_value_or( length(0, length::pt )).get_value_unit(length::pt), length::pt                            
                        );
                } 
                else if (horRel == horizontal_rel::ParagraphEndMargin)
                {
                    // TODO paragraphRightIndent
                    // TODO contextSubstractedValue
                    // select="$contextWidth - $paragraphRightIndent + $frameMarginLeft - $contextSubstractedValue + $translation"
                    svgX = length(
                        contextWidth.get_value_unit(length::pt) +
                        frameMarginLeft.get_value_or( length(0, length::pt )).get_value_unit(length::pt)
                        ,length::pt                            
                        );
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
                     svgX = length(
                         pageWidth.get_value_or( length(0, length::pt )).get_value_unit(length::pt) - 
                         frameWidth.get_value_or( length(0, length::pt )).get_value_unit(length::pt) - 
                         frameMarginRight.get_value_or( length(0, length::pt )).get_value_unit(length::pt),
                         length::pt
                         );
                 }
                 else if (horRel == horizontal_rel::PageStartMargin)
                 {
                     if (frameWidth.get_value_or( length(0, length::pt )).get_value_unit(length::pt) < 
                         pageMarginLeft.get_value_or( length(0, length::pt )).get_value_unit(length::pt)
                         )                                         
                     {
                         // 
                         svgX = length(
                             pageMarginLeft.get_value_or( length(0, length::pt )).get_value_unit(length::pt) - 
                             frameWidth.get_value_or( length(0, length::pt )).get_value_unit(length::pt) - 
                             frameMarginRight.get_value_or( length(0, length::pt )).get_value_unit(length::pt),
                             length::pt
                             );                        
                     }
                     else
                         svgX = length(0, length::pt);

                 }
                 else if (anchor && anchor->get_type() == anchor_type::Page)
                 {
                     svgX = length( pageWidth.get_value_or( length(0, length::pt )).get_value_unit(length::pt) - 
                         frameWidth.get_value_or( length(0, length::pt )).get_value_unit(length::pt) -
                         frameMarginRight.get_value_or( length(0, length::pt )).get_value_unit(length::pt)
                         );                    
                 } 
                 else if (horRel == horizontal_rel::PageContent)
                 {
                     // $contextWidth - $frameWidth - $frameMarginRight - $contextSubstractedValue + $translation
                     svgX = length( 
                         contextWidth.get_value_unit(length::pt) - 
                         frameWidth.get_value_or( length(0, length::pt )).get_value_unit(length::pt) -
                         frameMarginRight.get_value_or( length(0, length::pt )).get_value_unit(length::pt) -
                         contextSubstractedValue.get_value_or( length(0, length::pt )).get_value_unit(length::pt)
                         , length::pt
                         );                                                              
                 }
                 else if (horRel == horizontal_rel::Paragraph ||
                     horRel == horizontal_rel::ParagraphContent ||
                     horRel == horizontal_rel::ParagraphEndMargin)
                 {
                     // $contextWidth - $paragraphRightIndent -$frameWidth - $frameMarginRight - $contextSubstractedValue + $translation
                     // TODO
                     length paragraphRightIndent(0, length::pt);
                     svgX = length( 
                         contextWidth.get_value_unit(length::pt) - 
                         paragraphRightIndent.get_value_unit(length::pt) - 
                         frameWidth.get_value_or( length(0, length::pt )).get_value_unit(length::pt) - 
                         frameMarginRight.get_value_or( length(0, length::pt )).get_value_unit(length::pt) -
                         contextSubstractedValue.get_value_or( length(0, length::pt )).get_value_unit(length::pt)
                         , length::pt
                         ); 

                 }     
                 else if (horRel == horizontal_rel::ParagraphStartMargin)
                 {
                     // $paragraphLeftIndent - $frameMarginRight - $contextSubstractedValue + $translation
                     // TODO
                     length paragraphLeftIndent(0, length::pt);
                     svgX = length( 
                         paragraphLeftIndent.get_value_unit(length::pt) - 
                         frameMarginRight.get_value_or( length(0, length::pt )).get_value_unit(length::pt) -
                         contextSubstractedValue.get_value_or( length(0, length::pt )).get_value_unit(length::pt)
                         , length::pt
                         ); 
                 } 
                 else if (horRel == horizontal_rel::Frame ||
                     horRel == horizontal_rel::FrameContent ||
                     horRel == horizontal_rel::FrameStartMargin ||
                     horRel == horizontal_rel::FrameEndMargin)
                 {
                     // $pageWidth - $frameMarginRight + $translation

                     svgX = length( 
                         pageWidth.get_value_or( length(0, length::pt )).get_value_unit(length::pt) - 
                         frameMarginRight.get_value_or( length(0, length::pt )).get_value_unit(length::pt)                         
                         , length::pt
                         ); 

                 }
                 else if (horRel == horizontal_rel::Char)
                 {
                     svgX = length( 
                         pageWidth.get_value_or( length(0, length::pt )).get_value_unit(length::pt) - 
                         frameMarginRight.get_value_or( length(0, length::pt )).get_value_unit(length::pt)                         
                         , length::pt
                         );                   
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
    return get_value_emu(svgX);
}

int ComputeMarginY(const style_page_layout_properties_attlist & pageProperties,
                      const union_common_draw_attlists & attlists_,
                      const graphic_format_properties & graphicProperties)
{
    // TODO : recursive result!!!
    const _CP_OPT(anchor_type) anchor = 
        attlists_.shape_with_text_and_styles_.
        common_draw_shape_with_styles_attlist_.
        common_text_spreadsheet_shape_attlist_.
        common_text_anchor_attlist_.
        type_;

    _CP_OPT(vertical_rel) styleVerticalRel  = graphicProperties.common_vertical_rel_attlist_.style_vertical_rel_;
    _CP_OPT(vertical_pos) styleVerticallPos = graphicProperties.common_vertical_pos_attlist_.style_vertical_pos_;

    const _CP_OPT(length) pageHeight = pageProperties.fo_page_height_;        
    // TODO : проверить, значения в процентах что именно означают
    const _CP_OPT(length) pageMarginTop = CalcResultLength(pageProperties.common_vertical_margin_attlist_.fo_margin_top_, pageHeight);
    const _CP_OPT(length) pageMarginBottom = CalcResultLength(pageProperties.common_vertical_margin_attlist_.fo_margin_bottom_, pageHeight);

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
                svgY = fromTop;
                break;        
            case vertical_rel::ParagraphContent:
                // TODO:  get spacing property of current paragraph
                svgY = fromTop /*+paragraphTopSpacing*/;
                break;
            case vertical_rel::Frame:
            case vertical_rel::FrameContent:
                svgY = fromTop;
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
                        svgY = length(
                            pageHeight->get_value_unit(length::pt) - 
                            frameHeight.get_value_or( length(0, length::pt) ).get_value_unit(length::pt) -
                            frameMarginBottom.get_value_or( length(0, length::pt) ).get_value_unit(length::pt),
                            length::pt
                            );
                    }
                }
                break;
            case vertical_rel::PageContent:
                {
                    if (pageHeight)
                    {
                        svgY = length(
                        pageHeight->get_value_unit(length::pt) -
                        pageMarginTop.get_value_or( length(0, length::pt) ).get_value_unit(length::pt) -
                        pageMarginBottom.get_value_or( length(0, length::pt) ).get_value_unit(length::pt) -
                        frameHeight.get_value_or( length(0, length::pt) ).get_value_unit(length::pt) -
                        frameMarginBottom.get_value_or( length(0, length::pt) ).get_value_unit(length::pt), length::pt);                        
                    }
                }
                break;
            case vertical_rel::Paragraph:
            case vertical_rel::ParagraphContent:
                {
                    double paragraphBottomSpacing = 0.0; // TODO
                    if (pageHeight)
                    {
                        svgY = length(
                        pageHeight->get_value_unit(length::pt) -
                        pageMarginTop.get_value_or( length(0, length::pt) ).get_value_unit(length::pt) -
                        pageMarginBottom.get_value_or( length(0, length::pt) ).get_value_unit(length::pt) -
                        paragraphBottomSpacing -
                        frameHeight.get_value_or( length(0, length::pt) ).get_value_unit(length::pt) -
                        frameMarginBottom.get_value_or( length(0, length::pt) ).get_value_unit(length::pt), length::pt);                        
                    }
                }
                break;
            case vertical_rel::Frame:
            case vertical_rel::FrameContent:
                {
                    // $pageHeight - $frameHeight - $frameMarginBottom + $translation"
                    if (pageHeight)
                    {
                        svgY = length(
                        pageHeight->get_value_unit(length::pt) -
                        frameHeight.get_value_or( length(0, length::pt) ).get_value_unit(length::pt) -
                        frameMarginBottom.get_value_or( length(0, length::pt) ).get_value_unit(length::pt), length::pt);                        
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
                
    return get_value_emu(svgY);
}

}

void common_draw_docx_convert(oox::docx_conversion_context & Context, const union_common_draw_attlists & attlists_,oox::_docx_drawing &drawing) 
{
	const _CP_OPT(style_ref) & styleRef = attlists_.shape_with_text_and_styles_.
        common_draw_shape_with_styles_attlist_.common_draw_style_name_attlist_.draw_style_name_;

    const std::wstring styleName = styleRef ? styleRef->style_name() : L"";

	std::vector<const odf::style_instance *> instances;
	odf::style_instance* styleInst = Context.root()->odf_context().styleContainer().style_by_name(styleName, odf::style_family::Graphic,Context.process_headers_footers_);
	if (styleInst)
	{
		style_instance * defaultStyle = Context.root()->odf_context().styleContainer().style_default_by_type(odf::style_family::Graphic);
		if (defaultStyle)instances.push_back(defaultStyle);
		instances.push_back(styleInst);
	}
	graphic_format_properties graphicProperties = calc_graphic_properties_content(instances);	

    const std::wstring pagePropertiesName = Context.get_page_properties();
    
    const style_page_layout_properties * pagePropertiesNode = NULL;
	const page_layout_instance * pageLayoutInst = Context.root()->odf_context().pageLayoutContainer().page_layout_by_name(pagePropertiesName);
    
	if (pageLayoutInst)  pagePropertiesNode = pageLayoutInst->properties();

    const style_page_layout_properties_attlist emptyPageProperties;
    const style_page_layout_properties_attlist & pageProperties =
        pagePropertiesNode ? pagePropertiesNode->get_style_page_layout_properties_attlist() : emptyPageProperties;

/////////////////////////////////////////////////////////////////////////////////////////////

    drawing.styleWrap	= graphicProperties.style_wrap_;

    if (drawing.styleWrap && drawing.styleWrap->get_type() == style_wrap::Parallel)
	{
        if (graphicProperties.style_number_wrapped_paragraphs_)
            drawing.parallel = graphicProperties.style_number_wrapped_paragraphs_->get_value();
	}

	_CP_OPT(run_through)	styleRunThrough	= graphicProperties.style_run_through_;
	
	drawing.styleHorizontalRel	= graphicProperties.common_horizontal_rel_attlist_.style_horizontal_rel_;
    drawing.styleHorizontalPos	= graphicProperties.common_horizontal_pos_attlist_.style_horizontal_pos_;
    drawing.styleVerticalPos	= graphicProperties.common_vertical_pos_attlist_.style_vertical_pos_;
    drawing.styleVerticalRel	= graphicProperties.common_vertical_rel_attlist_.style_vertical_rel_;

    _CP_OPT(anchor_type) anchor = 
        attlists_.shape_with_text_and_styles_.
        common_draw_shape_with_styles_attlist_.
        common_text_spreadsheet_shape_attlist_.
        common_text_anchor_attlist_.
        type_;

	int level_drawing = Context.get_drawing_context().get_current_level();

    if (drawing.parallel == 1 || anchor && anchor->get_type() == anchor_type::AsChar || level_drawing >1 )
    {
        drawing.isInline = true;
    }

	if (!drawing.isInline)
    {
        drawing.relativeHeight = L"2";
        drawing.behindDoc = L"0";

        _CP_OPT(int) zIndex = attlists_.shape_with_text_and_styles_.common_draw_shape_with_styles_attlist_.common_draw_z_index_attlist_.draw_z_index_;
       
		if (zIndex)//порядок отрисовки объектов
        {
            if (*zIndex < 0) 
                drawing.relativeHeight = L"0";
            else
                drawing.relativeHeight = boost::lexical_cast<std::wstring>( 2 + *zIndex );
        }

        if (drawing.styleWrap && drawing.styleWrap->get_type() == style_wrap::RunThrough 
            && styleRunThrough && styleRunThrough->get_type() == run_through::Background
            )
        {
           drawing. behindDoc = L"1";            
        }


        drawing.margin_rect[0] = GetMargin(graphicProperties, sideLeft);
        drawing.margin_rect[1] = GetMargin(graphicProperties, sideTop);
        drawing.margin_rect[2] = GetMargin(graphicProperties, sideRight);
        drawing.margin_rect[3] = GetMargin(graphicProperties, sideBottom);

        drawing.posOffsetH = ComputeMarginX(pagePropertiesNode, pageProperties, attlists_, graphicProperties);
        drawing.posOffsetV = ComputeMarginY(pageProperties, attlists_, graphicProperties);

    }
	drawing.number_wrapped_paragraphs=graphicProperties.style_number_wrapped_paragraphs_.
									get_value_or( integer_or_nolimit( integer_or_nolimit::NoLimit) ).get_value();
	if (anchor && anchor->get_type() == anchor_type::AsChar && drawing.posOffsetV< 0)
	{
		drawing.posOffsetV = (int)(length(0.01, length::cm).get_value_unit(length::emu));
	}
//////////////////////////////////////////////
	graphicProperties.apply_to(drawing.additional);
//////////////////////////////////////////
	Compute_GraphicFill(graphicProperties.common_draw_fill_attlist_, Context.root()->odf_context().drawStyles() ,drawing.fill);	
////////////////////////////////////////////////////
	drawing.additional.push_back(odf::_property(L"border_width_left",	Compute_BorderWidth(graphicProperties, sideLeft)));
	drawing.additional.push_back(odf::_property(L"border_width_top",	Compute_BorderWidth(graphicProperties, sideTop)));
	drawing.additional.push_back(odf::_property(L"border_width_right",	Compute_BorderWidth(graphicProperties, sideRight)));
	drawing.additional.push_back(odf::_property(L"border_width_bottom", Compute_BorderWidth(graphicProperties, sideBottom))); 
	
	if (const _CP_OPT(std::wstring) foBorder = graphicProperties.common_border_attlist_.fo_border_)
	{
		border_style borderStyle(*foBorder);
		if (borderStyle.initialized())
		{
			drawing.additional.push_back(_property(L"stroke-color",	borderStyle.get_color().get_hex_value() ));
			drawing.additional.push_back(_property(L"stroke-width",	borderStyle.get_length().get_value_unit(odf::length::pt) ));

		}
	}
///////////////////////////
	if (attlists_.shape_with_text_and_styles_.common_draw_shape_with_styles_attlist_.common_draw_transform_attlist_.draw_transform_)
	{
		std::wstring transformStr = attlists_.shape_with_text_and_styles_.common_draw_shape_with_styles_attlist_.common_draw_transform_attlist_.draw_transform_.get();
		oox_convert_transforms(transformStr,drawing.additional);
	}


	drawing.cx = get_value_emu(attlists_.rel_size_.common_draw_size_attlist_.svg_width_);
    drawing.cy = get_value_emu(attlists_.rel_size_.common_draw_size_attlist_.svg_height_);
////////////////////////////////////////////////////////////////////////////////////////////////////
	if ((drawing.styleWrap) && (drawing.styleWrap->get_type() == style_wrap::Dynamic))	//автоподбор
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

		//if (drawing.cx<1 && max_width >0)
		//{
		//	drawing.cx = std::min(762000,max_width);
		//}
		//if (drawing.cy <1 && max_height >0)
		//{
		//	drawing.cy = std::min(142875,max_height);
		//}
	}
////////////////////////////////////////////////////////////////////////////////////////////////////
	_CP_OPT(double) dVal;
	
	GetProperty(drawing.additional,L"svg:scale_x",dVal);
	if (dVal)drawing.cx = (int)(0.5 + drawing.cx * dVal.get());
	
	GetProperty(drawing.additional,L"svg:scale_y",dVal);
	if (dVal)drawing.cy = (int)(0.5 + drawing.cy * dVal.get());

	GetProperty(drawing.additional,L"svg:translate_x",dVal);
	if (dVal)drawing.x+=get_value_emu(dVal.get());

	GetProperty(drawing.additional,L"svg:translate_y",dVal);
	if (dVal)drawing.y+=get_value_emu(dVal.get());

}
void draw_shape::docx_convert(oox::docx_conversion_context & Context)
{
	/////...../////

	oox::_docx_drawing drawing = oox::_docx_drawing();

	drawing.type = oox::mediaitems::typeShape;

	drawing.id = Context.get_drawing_context().get_current_shape_id();
	drawing.name = Context.get_drawing_context().get_current_object_name();

	drawing.sub_type = sub_type_;
	drawing.additional=additional_;//сюда могут добавиться свойства ...

	drawing.additional.push_back(_property(L"text-content",Context.get_drawing_context().get_text_stream_shape()));

	Context.get_drawing_context().clear_stream_shape();
/////////
	common_draw_docx_convert(Context, common_draw_attlists_, drawing);
/////////

	if (drawing.fill.type < 1 && !IsExistProperty(drawing.additional,L"stroke"))//бывает что и не определено ничего 
	{
		drawing.fill.solid = oox::oox_solid_fill::create();
		drawing.fill.solid->color = L"729FCF";
		drawing.fill.type = 1;
	}

    std::wostream & strm = Context.output_stream();

	bool pState = Context.get_paragraph_state();
	Context.set_paragraph_state(false);		

	Context.add_new_run();
	docx_serialize(strm,drawing);
    Context.finish_run();

	Context.set_paragraph_state(pState);		

	Context.get_drawing_context().stop_shape();
}

void draw_image::docx_convert(oox::docx_conversion_context & Context)
{
	if (!common_xlink_attlist_.href_)
		return;
 
	std::wstring href		= common_xlink_attlist_.href_.get_value_or(L"");
	int pos_replaicement= href.find(L"ObjectReplacements"); 
	if (pos_replaicement >=0)
		return;//заменяемый объект

    const draw_frame * frame = Context.get_drawing_context().get_current_frame();//owner
	if (!frame)
		return;

	//тут может быть не только текст , но и таблицы, другие объекты ...
 	oox::docx_conversion_context::StreamsManPtr prev = Context.get_stream_man();
	
	std::wstringstream temp_stream(Context.get_drawing_context().get_text_stream_frame());
	Context.set_stream_man( boost::shared_ptr<oox::streams_man>( new oox::streams_man(temp_stream) ));
  
	bool runState = Context.get_run_state();
	Context.set_run_state(false);
	
	bool pState = Context.get_paragraph_state();
	Context.set_paragraph_state(false);		

	BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
        elm->docx_convert(Context);
    }

	Context.set_run_state(runState);
	Context.set_paragraph_state(pState);

	Context.get_drawing_context().get_text_stream_frame() = temp_stream.str();
	Context.set_stream_man(prev);

/////////////////////////////////////////////////////////////////////////////////////////////////////
	oox::_docx_drawing drawing = oox::_docx_drawing();
	drawing.type = oox::mediaitems::typeImage;

	drawing.id = Context.get_drawing_context().get_current_frame_id();
	drawing.name = Context.get_drawing_context().get_current_object_name();
//////////////////////////////////////////////////////////////////////////////////////////////////
	oox::hyperlinks::_ref hyperlink = Context.last_hyperlink();
	//нужно еще систему конроля - могут придте уже "использованные" линки с картинок - из колонтитулов (но на них уже использовали релсы)
	//дыра осталась если картинка в картинке - линк продублируется с внутренней на внешнюю 
	if (hyperlink.drawing == true && hyperlink.used_rels == false)
	{
		oox::_hlink_desc desc={hyperlink.id,hyperlink.href,true};
		drawing.hlinks.push_back(desc);
	}

	//if (Context.get_drawing_context().get_current_level() > 1)
	//	drawing.isInline = true;

/////////
	common_draw_docx_convert(Context, frame->common_draw_attlists_, drawing);
/////////
	drawing.fill.bitmap = oox::oox_bitmap_fill::create();
	drawing.fill.type = 2;
	drawing.fill.bitmap->isInternal = false;
    drawing.fill.bitmap->rId = Context.add_mediaitem(href, oox::mediaitems::typeImage,drawing.fill.bitmap->isInternal,href);
	drawing.fill.bitmap->bStretch = true;

    const _CP_OPT(style_ref) & styleRef = frame->common_draw_attlists_.shape_with_text_and_styles_.
        common_draw_shape_with_styles_attlist_.
        common_draw_style_name_attlist_.
        draw_style_name_;

    const std::wstring styleName = styleRef ? styleRef->style_name() : L"";

	odf::style_instance* styleInst = Context.root()->odf_context().styleContainer().style_by_name(styleName, odf::style_family::Graphic,Context.process_headers_footers_);
	
	odf::style_graphic_properties *properties = NULL;
	if (styleInst) properties = styleInst->content()->get_style_graphic_properties();
////////////////
	if (properties)
	{
		using boost::filesystem::wpath;
		if (properties->content().fo_clip_ && drawing.fill.bitmap)
		{
			std::wstring strRectClip = properties->content().fo_clip_.get();
			strRectClip = strRectClip.substr(5,strRectClip.length()-6);
			
			std::wstring fileName = BOOST_STRING_PATH(wpath(Context.root()->get_folder()) / href);
			
			drawing.fill.bitmap->bCrop = parse_clipping(strRectClip,fileName,drawing.fill.bitmap->cropRect);
		}        
	}
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    std::wostream & strm = Context.output_stream();

	pState = Context.get_paragraph_state();
	Context.set_paragraph_state(false);

	Context.add_new_run();
	docx_serialize(strm,drawing);
    Context.finish_run();

	Context.set_paragraph_state(pState);

}

void draw_text_box::docx_convert(oox::docx_conversion_context & Context)
{
 	if (Context.get_drawing_context().get_current_level() >1 )return;

	//тут может быть не только текст , но и таблицы, другие объекты ...
 	oox::docx_conversion_context::StreamsManPtr prev = Context.get_stream_man();
	
	std::wstringstream temp_stream(Context.get_drawing_context().get_text_stream_frame());
	Context.set_stream_man( boost::shared_ptr<oox::streams_man>( new oox::streams_man(temp_stream) ));	
	bool runState = Context.get_run_state();
	Context.set_run_state(false);

	bool pState = Context.get_paragraph_state();
	Context.set_paragraph_state(false);		
	
	bool graphic_parent=false;
	
	BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
		ElementType type = elm->get_type();
        elm->docx_convert(Context);
    }

	Context.get_drawing_context().get_text_stream_frame() = temp_stream.str();
	Context.set_stream_man(prev);
	Context.set_run_state(runState);
	Context.set_paragraph_state(pState);		

	/////...../////

	const draw_frame * frame = Context.get_drawing_context().get_current_frame();//owner
	if (!frame)
		return;
	oox::_docx_drawing drawing = oox::_docx_drawing();

	drawing.type = oox::mediaitems::typeShape;

	drawing.id = Context.get_drawing_context().get_current_frame_id();
	drawing.name = Context.get_drawing_context().get_current_object_name();

	drawing.sub_type = 1;	//textBox

	const std::wstring & content = Context.get_drawing_context().get_text_stream_frame();
	
	drawing.additional.push_back(_property(L"text-content",content));
	Context.get_drawing_context().clear_stream_frame();
/////////
	common_draw_docx_convert(Context, frame->common_draw_attlists_, drawing);
//+ локальные 
	if ((draw_text_box_attlist_.fo_min_height_) && (draw_text_box_attlist_.fo_min_height_->get_type()==length_or_percent::Length))
	{
		size_t min_y = get_value_emu(draw_text_box_attlist_.fo_min_height_->get_length());
		if (drawing.cy < min_y) drawing.cy = min_y;
	}
	if ((draw_text_box_attlist_.fo_min_width_) && (draw_text_box_attlist_.fo_min_width_->get_type()==length_or_percent::Length))
	{
		size_t min_x = get_value_emu(draw_text_box_attlist_.fo_min_width_->get_length());
		if (drawing.cx < min_x) drawing.cx = min_x;
	}
	if ((draw_text_box_attlist_.fo_max_height_) && (draw_text_box_attlist_.fo_max_height_->get_type()==length_or_percent::Length))
	{
		size_t max_y = get_value_emu(draw_text_box_attlist_.fo_max_height_->get_length());
		if (drawing.cy > max_y) drawing.cy = max_y;
	}
	if ((draw_text_box_attlist_.fo_max_width_) && (draw_text_box_attlist_.fo_max_width_->get_type()==length_or_percent::Length))
	{
		size_t max_x = get_value_emu(draw_text_box_attlist_.fo_max_width_->get_length());
		if (drawing.cx > max_x) drawing.cy = max_x;
	}

///////////////////////////////////////////////////////////////////

    std::wostream & strm = Context.output_stream();
		
	pState = Context.get_paragraph_state();
	Context.set_paragraph_state(false);		
   
	Context.add_new_run();    
	docx_serialize(strm,drawing);
	Context.finish_run();

	Context.set_paragraph_state(pState);		
}
void draw_g::docx_convert(oox::docx_conversion_context & Context)
{
    BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
		ElementType type = elm->get_type();
        elm->docx_convert(Context);
    }
}
void draw_frame::docx_convert(oox::docx_conversion_context & Context)
{
	if (!Context.get_paragraph_state() && !Context.delayed_converting_)
    {
        Context.add_delayed_element(this);
        return;
    }

    Context.get_drawing_context().start_frame(this);
    
    const _CP_OPT(std::wstring) name = 
        common_draw_attlists_.shape_with_text_and_styles_.
        common_draw_shape_with_styles_attlist_.
        common_draw_name_attlist_.draw_name_;

    
    BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
		ElementType type = elm->get_type();
        Context.get_drawing_context().add_name_object(name.get_value_or(L""));
        elm->docx_convert(Context);
    }

	Context.get_drawing_context().stop_frame();
}
void draw_object::docx_convert(oox::docx_conversion_context & Context)
{
    try 
	{
        std::wstring href		= common_xlink_attlist_.href_.get_value_or(L"");

        odf::odf_document * odf = Context.root();
        const std::wstring folder = odf->get_folder();

        fs::wpath folderPath(folder);
        fs::wpath objectPath = folderPath / href;

#ifdef BOOST_FILESYSTEM_LEGACY
       const std::wstring dbgObjectPathStr = objectPath.normalize().string();
#else
       const std::wstring dbgObjectPathStr = objectPath.normalize().wstring();
#endif

        cpdoccore::odf::odf_document objectSubDoc(dbgObjectPathStr);    
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//функциональная часть
		const office_element *contentSubDoc = objectSubDoc.get_impl()->get_content();
		if (!contentSubDoc)
			return;

		chart_build chartBuild;
		process_build_chart process_build_chart_(chartBuild,objectSubDoc.odf_context().styleContainer());
        contentSubDoc->accept(process_build_chart_); 

		chartBuild.docx_convert(Context);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//отображательная часть	

		const draw_frame * frame = Context.get_drawing_context().get_current_frame();//owner
		if (!frame)
			return;
		oox::_docx_drawing drawing = oox::_docx_drawing();

		drawing.type = oox::mediaitems::typeChart;
		
		drawing.id = Context.get_drawing_context().get_current_frame_id();
		drawing.name = Context.get_drawing_context().get_current_object_name();
		
        bool isMediaInternal = true;        
		drawing.chartId = Context.add_mediaitem(href, oox::mediaitems::typeChart, isMediaInternal, href);

		common_draw_docx_convert(Context, frame->common_draw_attlists_, drawing);
		
		bool runState = Context.get_run_state();
		Context.set_run_state(false);
		
		bool pState = Context.get_paragraph_state();
		Context.set_paragraph_state(false);		
		
		std::wostream & strm = Context.output_stream();
		
		Context.add_new_run();
		docx_serialize(strm,drawing);
		Context.finish_run();
		
		Context.set_run_state(runState);
		Context.set_paragraph_state(pState);		
	}
    catch(...)
    {
        _CP_LOG(error) << "convert draw::object error" << std::endl;        
    }
}


}
}
