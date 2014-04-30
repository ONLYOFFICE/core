#pragma once

#include "stdAfx.h"

#include "DocxConverter.h"

#include <DocxFormat\Docx.h>

#include <boost/foreach.hpp>

#include "odf_conversion_context.h"
//#include "odt_conversion_context.h"

using namespace cpdoccore;

namespace Oox2Odf
{
DocxConverter::DocxConverter(const std::wstring & path)
{
	output_document = new  odf::package::odf_document(L"text");

	const OOX::CPath oox_path(CString(path.c_str()));	
	docx_document = new OOX::CDocx(oox_path);	
}
void DocxConverter::write(const std::wstring & path)
{
	if (!output_document)return;

	output_document->write(path);

}
odf::odf_conversion_context* DocxConverter::odf_context()
{
	return NULL;//odt_context;
}
OOX::CTheme* DocxConverter::oox_theme()
{
	return NULL;
}

void DocxConverter::convertDocument()
{
	if (!docx_document)return;

	const OOX::CDocument* document = docx_document->GetDocument();

	if (!document)return;
	
	//odf_context_ = new odf::odf_conversion_context(output_document);

}
} 