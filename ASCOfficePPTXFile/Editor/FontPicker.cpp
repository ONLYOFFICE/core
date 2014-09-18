//#include "stdafx.h"
#include "BinWriters.h"

HRESULT COfficeFontPicker::GetBinaryData(LONG lType, SAFEARRAY** ppBinaryArray)
{
	if (NULL == ppBinaryArray)
		return S_FALSE;

	*ppBinaryArray = NULL;
	if (FONT_PICKER_BINARYDATA_SAVEEMBEDDED == lType)
	{
		NSBinPptxRW::CBinaryFileWriter oWriter;
		m_oPicker.m_oEmbeddedFonts.CheckString(_T(".)abcdefghijklmnopqrstuvwxyz"));
		m_oPicker.m_oEmbeddedFonts.CheckFont(_T("Wingdings 3"), m_oPicker.m_pFontManager);
		m_oPicker.m_oEmbeddedFonts.CheckFont(_T("Arial"), m_oPicker.m_pFontManager);

		oWriter.StartRecord(NSBinPptxRW::NSMainTables::FontsEmbedded);
		m_oPicker.m_oEmbeddedFonts.WriteEmbeddedFonts(&oWriter);
		oWriter.EndRecord();

		*ppBinaryArray = oWriter.GetSafearray();
	}
	else if (FONT_PICKER_BINARYDATA_SAVEMAP == lType)
	{
	}
	return S_OK;
}