#include "stdafx.h"
#include "RtfDocument.h"
#include "RtfReader.h"
#include "OOXNumberingWriter.h"
#include "OOXFontTableWriter.h"
#include "OOXStylesWriter.h"
#include "OOXSettingsWriter.h"
#include "OOXThemeWriter.h"
#include "OOXDocPropAppWriter.h"
#include "OOXDocPropCoreWriter.h"
#include "OOXFootnoteWriter.h"

RtfDocument::RtfDocument()
{
	m_aArray.Add( RtfSectionPtr( new RtfSection() ) );
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