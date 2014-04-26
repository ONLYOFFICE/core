#pragma once

#include "stdAfx.h"

#include "XlsxTextShapeConverter.h"

#include <boost/foreach.hpp>

#include "ods_conversion_context.h"

#include "odf_text_context.h"
#include "odf_drawing_context.h"

#include "styles.h"

#include "style_table_properties.h"
#include "style_text_properties.h"
#include "style_paragraph_properties.h"
#include "style_graphic_properties.h"

#include <Logic\TxBody.h>

using namespace cpdoccore;

namespace Oox2Odf
{


void XlsxTextShapeConverter::convert(CString &strParagraphs)
{
	if (strParagraphs.GetLength() < 1)return;
	
	CString xml_txBody;
	xml_txBody += _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	xml_txBody += _T("<xdr:txBody xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:xdr=\"http://schemas.openxmlformats.org/drawingml/2006/spreadsheetDrawing\">");
	xml_txBody += _T("<a:bodyPr/>");
	xml_txBody += strParagraphs;
	xml_txBody += _T("</xdr:txBody>");

	XmlUtils::CXmlNode oXmlNode;
	if (!oXmlNode.FromXmlString(xml_txBody))	return;

	PPTX::Logic::TxBody * txBody = new PPTX::Logic::TxBody(oXmlNode);


	
}

}