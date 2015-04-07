#pragma once

#include "NamespaceOwn.h"
#include "../../../Common/DocxFormat/Source/XML/xmlutils.h"

#if defined(_WIN32) || defined (_WIN64)
#include "atlstr.h"
#endif

namespace PPTX
{
#define WritingElement_AdditionConstructors(Class) \
	Class(XmlUtils::CXmlNode& oNode)\
	{\
		fromXML( oNode );\
	}\
	const Class& operator =(const XmlUtils::CXmlNode &oNode)\
	{\
		fromXML( (XmlUtils::CXmlNode &)oNode );\
		return *this;\
	}

#define WritingElement_ReadNode( oRootNode, oChildNode, sNodeName, oValue ) \
	if ( oRootNode.GetNode( sNodeName, oChildNode ) )\
		oValue = oChildNode;

#define WritingElement_WriteNode_1( sStartNodeString, oValue ) \
	if ( oValue.IsInit() )\
	{\
		sResult += sStartNodeString;\
		sResult += oValue->ToString();\
		sResult += _T("/>");\
	}

#define WritingElement_WriteNode_2( oValue ) \
	if ( oValue.IsInit() )\
		sResult += oValue->toXML();

	enum EElementType
	{
		et_Unknown,

		et_Default, // <Default>
		et_Override, // <Override>

		et_a_graphic, // <a:graphic>

		et_ds_schemaRef, // <ds:shemeRef>
		et_ds_schemaRefs, // <ds:schemaRefs>

		et_p_pic, // <p:pic>

		et_w_abstractNum, // <w:abstractNum>
		et_w_annotationRef, // <w:annotationRef>
		et_w_background, // <w:background>
		et_w_bdo, // <w:bdo>
		et_w_bookmarkEnd, // <w:bookmarkEnd>
		et_w_bookmarkStart, // <w:bookmarkStart>
		et_w_br, // <w:br>
		et_w_checkBox, // <w:checkBox>
		et_w_cols, // <w:cols>
		et_w_comboBox, // <w:comboBox>
		et_w_commentRangeEnd, // <w:commentRangeEnd>
		et_w_commentRangeStart, // <w:commentRangeStart>
		et_w_commentReference, // <w:commentReference>
		et_w_contentPart, // <w:contentPart>
		et_w_continuationSeparator, // <w:continuationSeparator>
		et_w_customXmlDelRangeEnd, // <w:customXmlDelRangeEnd>
		et_w_customXmlDelRangeStart, // <w:customXmlDelRangeStart>
		et_w_customXmlInsRangeEnd, // <w:customXmlInsRangeEnd>
		et_w_customXmlInsRangeStart, // <w:customXmlInsRangeStart>
		et_w_customXmlMoveFromRangeEnd, // <w:customXmlMoveFromRangeEnd>
		et_w_customXmlMoveFromRangeStart, // <w:customXmlMoveFromRangeStart>
		et_w_customXmlMoveToRangeEnd, // <w:customXmlMoveToRangeEnd>
		et_w_customXmlMoveToRangeStart, // <w:customXmlMoveToRangeStart>
		et_w_cr, // <w:cr>
		et_w_date, // <w:date>
		et_w_dayLong, // <w:dayLong>
		et_w_dayShort, // <w:dayShort>
		et_w_ddList, // <w:ddList>
		et_w_delInstrText, // <w:delInstrText>
		et_w_delText, // <w:delText>
		et_w_docDefaults, // <w:docDefaults>
		et_w_docPartList, // <w:docPartList>
		et_w_dropDownList, // <w:dropDownList>
		et_w_endnote, // <w:endnote>
		et_w_endnotePr, // <w:endnotePr>
		et_w_endnoteRef, // <w:endnoteRef>
		et_w_endnoteReference, // <w:endnoteReference>
		et_w_ffData, // <w:ffData>
		et_w_fldChar, // <w:fldChar>
		et_w_fldSimple, // <w:fldSimple>
		et_w_font, // <w:font>
		et_w_footnote, // <w:footnote>
		et_w_footnotePr, // <w:footnotePr>
		et_w_footnoteRef, // <w:footnoteRef>
		et_w_footnoteReference, // <w:footnoteReference>
		et_w_ftr, // <w:ftr>
		et_w_hdr, // <w:hdr>
		et_w_headers, // <w:headers>
		et_w_hyperlink, // <w:hyperlink>
		et_w_instrText, // <w:instrText>
		et_w_latentStyles, // <w:latentStyles>
		et_w_lastRenderedPageBreak, // <w:lastRenderedPageBreak>
		et_w_lvl, // <w:lvl>
		et_w_lvlOverride, // <w:lvlOverride>
		et_w_monthLong, // <w:monthLong>
		et_w_monthShort, // <w:monthShort>
		et_w_moveFromRangeEnd, // <w:moveFromRangeEnd>
		et_w_moveFromRangeStart, // <w:moveFromRangeStart>
		et_w_moveToRangeEnd, // <w:moveToRangeEnd>
		et_w_moveToRangeStart, // <w:moveToRangeStart>
		et_w_num, // <w:num>
		et_w_numPr, // <w:numPr>
		et_w_nonBreakHyphen, // <w:nonBreakHyphen>
		et_w_object, // <w:object>
		et_w_p, // <w:p>
		et_w_pBdr, // <w:pBdr>
		et_w_permEnd, // <w:permEnd>
		et_w_permStart, // <w:permStart>
		et_w_pgBorders, // <w:pgBorders>
		et_w_pgNum, // <w:pgNum>
		et_w_placeholder, // <w:placeholder>
		et_w_pPr, // <w:pPr>
		et_w_pPrChange, // <w:pPrChange>
		et_w_proofErr, // <w:proofErr>
		et_w_ptab, // <w:ptab>
		et_w_r, // <w:r>
		et_w_ruby, // <w:ruby>
		et_w_rPr, // <w:rPr>
		et_w_rPrChange, // <w:rPrChange>
		et_w_sdt, // <w:sdt>
		et_w_sdtContent, // <w:sdtContent>
		et_w_sdtEndPr, // <w:sdtEndPr>
		et_w_sdtPr, // <w:sdtPr>
		et_w_sectPr, // <w:sectPr>
		et_w_sectPrChange, // <w:sectPrChange>
		et_w_separator, // <w:separator>
		et_w_softHyphen, // <w:softHyphen>
		et_w_style, // <w:style>
		et_w_sym, // <w:sym>
		et_w_t, // <w:t>
		et_w_tab, // <w:tab>
		et_w_tabs, // <w:tabs>
		et_w_tbl, // <w:tbl>
		et_w_tblBorders, // <w:tblBorders>
		et_w_tblCellMar, // <w:tblCellMar>
		et_w_tblGrid, // <w:tblGrid>
		et_w_tblGridChange, // <w:tblGridChange>
		et_w_tblPr, // <w:tblPr>
		et_w_tblPrChange, // <w:tblPrChange>
		et_w_tblPrEx, // <w:tblPrEx>
		et_w_tblPrExChange, // <w:tblPrExChange>
		et_w_tblStylePr, // <w:tblStylePr>
		et_w_tc, // <w:tc>
		et_w_tcBorders, // <w:tcBorders>
		et_w_tcMar, // <w:tcMar>
		et_w_tcPr, // <w:tcPr>
		et_w_tcPrChange, // <w:tcPrChange>
		et_w_textInput, // <w:textInput>
		et_w_tr, // <w:tr>
		et_w_trPr, // <w:trPr>
		et_w_trPrChange, // <w:trPrChange>
		et_w_yearLong, // <w:yearLong>
		et_w_yearShort, // <w:yearShort>

		et_wp_docPr, // <wp:docPr>
        et_wp_effectExtent, // <wp:effectExtent>
		et_wp_extent, // <wp:extent>
        et_wp_wrapPolygon, // <wp:wrapPolygon>
	};

	class WritingElement
	{
	public:
		WritingElement(){}
		virtual ~WritingElement() {}

		virtual void         fromXML(XmlUtils::CXmlNode& node) = 0;
        virtual CString      toXML() const                     = 0;
		virtual EElementType getType() const
		{
			return PPTX::et_Unknown;
		}
	};
}
