
#include "RtfDocument.h"
#include "RtfReader.h"

#include "Writer/OOXNumberingWriter.h"
#include "Writer/OOXFontTableWriter.h"
#include "Writer/OOXStylesWriter.h"
#include "Writer/OOXSettingsWriter.h"
#include "Writer/OOXThemeWriter.h"
#include "Writer/OOXDocPropAppWriter.h"
#include "Writer/OOXDocPropCoreWriter.h"
#include "Writer/OOXFootnoteWriter.h"

RtfDocument::RtfDocument()
{
	m_aArray.push_back( RtfSectionPtr( new RtfSection() ) );
	m_oProperty.SetDefaultOOX();
	m_oDefaultCharProp.SetDefaultRtf();
	m_oDefaultParagraphProp.SetDefaultRtf();
}
int RtfDocument::GetType()
	{
		return TYPE_RTF_DOCUMENT;
	}

CString RtfDocument::RenderToRtf(RenderParameter oRenderParameter)
	{
		return _T("");
	}
CString RtfDocument::RenderToOOX(RenderParameter oRenderParameter)
	{
		return _T("");
	}