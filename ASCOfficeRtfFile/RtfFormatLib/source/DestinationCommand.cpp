/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "DestinationCommand.h"
#include "RtfOle.h"
#include "Ole1FormatReader.h"

#include "ConvertationManager.h"

HRESULT ConvertOle1ToOle2(BYTE *pData, int nSize, std::wstring sOle2Name)
{
	HRESULT hr = S_FALSE;

	Ole1FormatReader ole1Reader(pData, nSize);

	NSFile::CFileBinary file;
	file.CreateFileW(sOle2Name);
	file.WriteFile(ole1Reader.NativeData, ole1Reader.NativeDataSize);
	file.CloseFile();
	return S_FALSE;
}
//-----------------------------------------------------------------------------------------

bool RtfDocumentCommand::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, CString sCommand, bool hasParameter, int parameter)
{
	if( _T("ansi") == sCommand )
		oDocument.m_oProperty.m_eCodePage = RtfDocumentProperty::cp_ansi;
	else if( _T("mac") == sCommand )
		oDocument.m_oProperty.m_eCodePage = RtfDocumentProperty::cp_mac;
	else if( _T("pc") == sCommand )
		oDocument.m_oProperty.m_eCodePage = RtfDocumentProperty::cp_pc;
	else if( _T("pca") == sCommand )
		oDocument.m_oProperty.m_eCodePage = RtfDocumentProperty::cp_pca;
	else if( _T("ansicpg") == sCommand )
	{
		oDocument.m_oProperty.m_eCodePage = RtfDocumentProperty::cp_ansi;

        if( true == hasParameter )
        {
			oDocument.m_oProperty.m_nAnsiCodePage = parameter;
        }
	}
	else if( _T("deff") == sCommand )
	{
		if( true == hasParameter )
		{
			oDocument.m_oProperty.m_nDeffFont = parameter;
			oReader.m_nDefFont = parameter;
		}
	}
	COMMAND_RTF_INT( _T("themelang"),	oDocument.m_oProperty.m_nThemelang, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("themelangfe"),	oDocument.m_oProperty.m_nThemelangfe, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("themelangcs"),	oDocument.m_oProperty.m_nThemelangcs, sCommand, hasParameter, parameter )
	
	COMMAND_RTF_BOOL( _T("rtlgutter"),  oDocument.m_oProperty.m_bRtlGutter, sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( _T("rtldoc"),		oDocument.m_oProperty.m_bRtl, sCommand, hasParameter, parameter)

	COMMAND_RTF_BOOL( _T("hyphcaps"),	oDocument.m_oProperty.m_bHypCaps, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("hyphauto"),	oDocument.m_oProperty.m_bAutoHyp, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("hyphconsec"),	oDocument.m_oProperty.m_nMaxHypen, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("hyphhotz"),	oDocument.m_oProperty.m_nHyphenationRight, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("deftab"),		oDocument.m_oProperty.m_nTabWidth, sCommand, hasParameter, parameter )

//Page Borders
	COMMAND_RTF_BOOL( _T("pgbrdrhead"), oDocument.m_oProperty.m_bDorderSurroundHeader, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("pgbrdrfoot"), oDocument.m_oProperty.m_bDorderSurroundFotter, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("pgbrdrsnap"), oDocument.m_oProperty.m_bAlignBordersAndEdges, sCommand, hasParameter, parameter )

//Page Information
	else if( _T("paperw") == sCommand )
	{
		if( true == hasParameter )
		{
			oDocument.m_oProperty.m_nPaperWidth = parameter;
			oReader.m_oCurSectionProp.m_nPageWidth = parameter;
		}
	}
	else if( _T("paperh") == sCommand )
	{
		if( true == hasParameter )
		{
			oDocument.m_oProperty.m_nPaperHeight = parameter;
			oReader.m_oCurSectionProp.m_nPageHeight = parameter;
		}
	}
	else if( _T("margl") == sCommand )
	{
		if( true == hasParameter )
		{
			oDocument.m_oProperty.m_nMarginLeft = parameter;
			oReader.m_oCurSectionProp.m_nMarginLeft = parameter;
		}
	}
	else if( _T("margr") == sCommand )
	{
		if( true == hasParameter )
		{
			oDocument.m_oProperty.m_nMarginRight = parameter;
			oReader.m_oCurSectionProp.m_nMarginRight = parameter;
		}
	}
	else if( _T("margt") == sCommand )
	{
		if( true == hasParameter )
		{
			oDocument.m_oProperty.m_nMarginTop = parameter;
			oReader.m_oCurSectionProp.m_nMarginTop = parameter;
		}
	}
	else if( _T("margb") == sCommand )
	{
		if( true == hasParameter )
		{
			oDocument.m_oProperty.m_nMarginBottom = parameter;
			oReader.m_oCurSectionProp.m_nMarginBottom = parameter;
		}
	}
	else if( _T("facingp") == sCommand )
	{
		if( true == hasParameter && 0 == parameter )
		{
			oDocument.m_oProperty.m_bFacingPage = 0;
			oReader.m_oCurSectionProp.m_bSwitchMargin = 0;
		}
		else
		{
			oDocument.m_oProperty.m_bFacingPage = 1;
			oReader.m_oCurSectionProp.m_bSwitchMargin = 1;
		}
	}
	else if( _T("gutter") == sCommand )
	{
		if( true == hasParameter )
		{
			oDocument.m_oProperty.m_nGutterWidth = parameter;
			oReader.m_oCurSectionProp.m_nGutterMarginWidth = parameter;
		}
	}
	else if( _T("ogutter") == sCommand )
	{
		if( true == hasParameter )
			oDocument.m_oProperty.m_nGutterWidthOutside = parameter;
	}
	else if( _T("margmirror") == sCommand )
	{
		if( true == hasParameter && 0 == parameter )
		{
			oDocument.m_oProperty.m_bSwitchMargins = 0;
			oReader.m_oCurSectionProp.m_bLandscapeFormat = 0;
		}
		else
		{
			oDocument.m_oProperty.m_bSwitchMargins = 1;
			oReader.m_oCurSectionProp.m_bLandscapeFormat = 1;
		}
	}
	else if( _T("landscape") == sCommand )
	{
		if( true == hasParameter && 0 == parameter )
			oDocument.m_oProperty.m_bLandScape = 0;
		else
			oDocument.m_oProperty.m_bLandScape = 1;
	}
	COMMAND_RTF_BOOL( _T("gutterprl"), oDocument.m_oProperty.m_bGutterAtTop, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("viewbksp"), oDocument.m_oProperty.m_nDisplayBackground, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("htmautsp"), oDocument.m_oProperty.m_bHtmlAutoSpace, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("useltbaln"), oDocument.m_oProperty.m_bUseTabAlignment, sCommand, hasParameter, parameter )
	//Footnotes and Endnotes
	COMMAND_RTF_INT( _T("endnotes"), oDocument.m_oProperty.m_eFootnotePlacement, sCommand, true, RtfDocumentProperty::fp_EndSection )
	COMMAND_RTF_INT( _T("enddoc"), oDocument.m_oProperty.m_eFootnotePlacement, sCommand, true, RtfDocumentProperty::fp_EndDocument )
	COMMAND_RTF_INT( _T("ftntj"), oDocument.m_oProperty.m_eFootnotePlacement, sCommand, true, RtfDocumentProperty::fp_BeneathText )
	COMMAND_RTF_INT( _T("ftnbj"), oDocument.m_oProperty.m_eFootnotePlacement, sCommand, true, RtfDocumentProperty::fp_BottomPage )

	COMMAND_RTF_INT( _T("aendnotes"), oDocument.m_oProperty.m_eEndnotePlacement, sCommand, true, RtfDocumentProperty::ep_EndSection )
	COMMAND_RTF_INT( _T("aenddoc"), oDocument.m_oProperty.m_eEndnotePlacement, sCommand, true, RtfDocumentProperty::ep_EndDocument )
	COMMAND_RTF_INT( _T("aftnbj"), oDocument.m_oProperty.m_eEndnotePlacement, sCommand, true, RtfDocumentProperty::ep_BeneathText )
	COMMAND_RTF_INT( _T("aftntj"), oDocument.m_oProperty.m_eEndnotePlacement, sCommand, true, RtfDocumentProperty::ep_BottomPage )

	COMMAND_RTF_INT( _T("ftnstart"), oDocument.m_oProperty.m_nFootnoteStart, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("aftnstart"), oDocument.m_oProperty.m_nEndnoteStart, sCommand, hasParameter, parameter )

	COMMAND_RTF_INT( _T("ftnrstpg"), oDocument.m_oProperty.m_eFootnoteRestart, sCommand, true, RtfDocumentProperty::fr_EachPage )
	COMMAND_RTF_INT( _T("ftnrestart"), oDocument.m_oProperty.m_eFootnoteRestart, sCommand, true, RtfDocumentProperty::fr_EachSection )
	COMMAND_RTF_INT( _T("ftnrstcont"), oDocument.m_oProperty.m_eFootnoteRestart, sCommand, true, RtfDocumentProperty::fr_Continuous )
	COMMAND_RTF_INT( _T("aftnrestart"), oDocument.m_oProperty.m_eEndnoteRestart, sCommand, true, RtfDocumentProperty::er_EachSection )
	COMMAND_RTF_INT( _T("aftnrstcont"), oDocument.m_oProperty.m_eEndnoteRestart, sCommand, true, RtfDocumentProperty::er_Continuous )

	COMMAND_RTF_INT( _T("ftnnar"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 0 )
	COMMAND_RTF_INT( _T("ftnnalc"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 4 )
	COMMAND_RTF_INT( _T("ftnnauc"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 3 )
	COMMAND_RTF_INT( _T("ftnnrlc"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 2 )
	COMMAND_RTF_INT( _T("ftnnruc"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 1 )
	COMMAND_RTF_INT( _T("ftnnchi"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 70 )
	COMMAND_RTF_INT( _T("ftnnchi"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 25 )
	COMMAND_RTF_INT( _T("ftnncnum"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 18 )
	COMMAND_RTF_INT( _T("ftnndbnum"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 10 )
	COMMAND_RTF_INT( _T("ftnndbnumd"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 11 )
	COMMAND_RTF_INT( _T("ftnndbnumt"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 16 )
	COMMAND_RTF_INT( _T("ftnndbnumk"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 17 )
	COMMAND_RTF_INT( _T("ftnndbar"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 20 )
	COMMAND_RTF_INT( _T("ftnnganada"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 24 )
	COMMAND_RTF_INT( _T("ftnngbnum"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 26 )
	COMMAND_RTF_INT( _T("ftnngbnumd"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 27 )
	COMMAND_RTF_INT( _T("ftnngbnuml"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 28 )
	COMMAND_RTF_INT( _T("ftnngbnumk"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 29 )
	COMMAND_RTF_INT( _T("ftnnzodiac"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 30 )
	COMMAND_RTF_INT( _T("ftnnzodiacd"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 31 )
	COMMAND_RTF_INT( _T("ftnnzodiacl"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 32 )

	COMMAND_RTF_INT( _T("aftnnar"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 0 )
	COMMAND_RTF_INT( _T("aftnnalc"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 4 )
	COMMAND_RTF_INT( _T("aftnnauc"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 3 )
	COMMAND_RTF_INT( _T("aftnnrlc"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 2 )
	COMMAND_RTF_INT( _T("aftnnruc"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 1 )
	COMMAND_RTF_INT( _T("aftnnchi"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 70 )
	COMMAND_RTF_INT( _T("aftnnchi"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 25 )
	COMMAND_RTF_INT( _T("aftnncnum"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 18 )
	COMMAND_RTF_INT( _T("aftnndbnum"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 10 )
	COMMAND_RTF_INT( _T("aftnndbnumd"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 11 )
	COMMAND_RTF_INT( _T("aftnndbnumt"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 16 )
	COMMAND_RTF_INT( _T("aftnndbnumk"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 17 )
	COMMAND_RTF_INT( _T("aftnndbar"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 20 )
	COMMAND_RTF_INT( _T("aftnnganada"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 24 )
	COMMAND_RTF_INT( _T("aftnngbnum"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 26 )
	COMMAND_RTF_INT( _T("aftnngbnumd"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 27 )
	COMMAND_RTF_INT( _T("aftnngbnuml"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 28 )
	COMMAND_RTF_INT( _T("aftnngbnumk"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 29 )
	COMMAND_RTF_INT( _T("aftnnzodiac"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 30 )
	COMMAND_RTF_INT( _T("aftnnzodiacd"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 31 )
	COMMAND_RTF_INT( _T("aftnnzodiacl"), oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 32 )
	else
		return false;
	return true;
}


bool RtfNormalReader::ExecuteCommand( RtfDocument& oDocument, RtfReader& oReader, CString sCommand, bool hasParameter, int parameter )
 {
	if( _T("colortbl") == sCommand )
	{
		RtfColorTableReader oColorTableReader;
		return StartSubReader( oColorTableReader, oDocument, oReader );
	}
	else if( _T("fonttbl") == sCommand )
	{
		RtfFontTableReader oFontTableReader;
		return StartSubReader( oFontTableReader, oDocument, oReader );
	}
	else if( _T("stylesheet") == sCommand )
	{
		StyleTableReader oStyleReader;
		return StartSubReader( oStyleReader, oDocument, oReader );
	}
	//else if( _T("latentstyles") == sCommand )
	//{
	//	LatentstyleTableReader oLatentstyleReader;
	//	return StartSubReader( oLatentstyleReader, oDocument, oReader );
	//}
	else if( _T("listtable") == sCommand )
	{
		ListTableReader oListTableReader;
		return StartSubReader( oListTableReader, oDocument, oReader );

	}
	else if( _T("listoverridetable") == sCommand )
	{
		ListOverrideTableReader oListOverrideReader;
		return StartSubReader( oListOverrideReader, oDocument, oReader );
	}
	else if( _T("info") == sCommand )
	{
		RtfInfoReader oInfoReader;
		return StartSubReader( oInfoReader, oDocument, oReader );			
	}
	//else if( _T("colorschememapping") == sCommand )
	//{
	//	RtfColorSchemeReader oSchemeReader;
	//	return StartSubReader( oSchemeReader, oDocument, oReader );		
	//}
	//else if( _T("themedata") == sCommand )
	//{
	//	RtfThemeDataReader oThemeDataReader;
	//	return StartSubReader( oThemeDataReader, oDocument, oReader );	
	//}
	//else if( _T("defchp") == sCommand )
	//{
	//	RtfDefCharPropReader oDefCharPropReader( oDocument.m_oDefaultCharProp );
	//	return StartSubReader( oDefCharPropReader, oDocument, oReader );				}
	//else if( _T("defpap") == sCommand )
	//{
	//	RtfDefParPropReader oDefParPropReader;
	//	return StartSubReader( oDefParPropReader, oDocument, oReader );
	//}
	else if( _T("mmathPr") == sCommand )
	{
		RtfMathReader oMathPropReader(oDocument.m_oMathProp);
		return StartSubReader( oMathPropReader, oDocument, oReader );
	}
    else if( _T("ftnsep") == sCommand || _T("ftnsepc") == sCommand ||
            _T("aftnsep")== sCommand || _T("aftnsepc") == sCommand )
	{
		ParagraphReader oParagraphReader(sCommand, oReader);
		StartSubReader( oParagraphReader, oDocument, oReader );
		if( _T("ftnsep") == sCommand )
		{
			oDocument.m_oFootnoteSep = oParagraphReader.m_oParPropDest.m_oTextItems;
			oDocument.m_oProperty.m_aSpecialFootnotes.push_back( 0 );
		}
		else if( _T("ftnsepc") == sCommand )
		{
			oDocument.m_oFootnoteCon = oParagraphReader.m_oParPropDest.m_oTextItems;
			oDocument.m_oProperty.m_aSpecialFootnotes.push_back( 1 );
		}
		else if( _T("aftnsep") == sCommand )
		{
			oDocument.m_oEndnoteSep = oParagraphReader.m_oParPropDest.m_oTextItems;
			oDocument.m_oProperty.m_aSpecialEndnotes.push_back( 0 );
		}
		else if( _T("aftnsepc") == sCommand )
		{
			oDocument.m_oEndnoteCon = oParagraphReader.m_oParPropDest.m_oTextItems;
			oDocument.m_oProperty.m_aSpecialEndnotes.push_back( 1 );
		}
	}
	else if( _T("sect") == sCommand )
	{
		RtfSectionPtr oCurSection;
		if(true == oDocument.GetItem( oCurSection ) )
		{
			oCurSection->m_oProperty = oReader.m_oCurSectionProp;
			oCurSection->m_bFinalize = true;
		}
			
		oParagraphReaderDestination.Finalize( oReader/* , oCurSection*/);
		
		RtfSectionPtr oNewSection = RtfSectionPtr( new RtfSection() );
		oParagraphReaderDestination.m_oTextItems = oNewSection;
		oDocument.AddItem( oNewSection );

		//вручную обнуляем footer, т.к. sectd может встретиться и после field
		///?????
		oReader.m_oCurSectionProp.m_oHeaderLeft		= TextItemContainerPtr();
		oReader.m_oCurSectionProp.m_oHeaderFirst	= TextItemContainerPtr();
		oReader.m_oCurSectionProp.m_oHeaderRight	= TextItemContainerPtr();
		oReader.m_oCurSectionProp.m_oFooterLeft		= TextItemContainerPtr();
		oReader.m_oCurSectionProp.m_oFooterFirst	= TextItemContainerPtr();
		oReader.m_oCurSectionProp.m_oFooterRight	= TextItemContainerPtr();
	}
	else if(  _T("sectd") == sCommand  )
	{
		SectDef( oDocument, oReader );
	}
	else
	{
		bool bResult = false;
		bResult = oParagraphReaderDestination.ExecuteCommand( oDocument, oReader, (*this), sCommand, hasParameter, parameter );
		if( true == bResult )
			return true;
		bResult = RtfDocumentCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter );
		if( true == bResult )
			return true;
		bResult = oRtfSectionCommand.ExecuteCommand( oDocument, oReader, (*this),sCommand, hasParameter, parameter );
		if( true == bResult )
			return true;
		bResult = RtfDocumentCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter );
		if( true == bResult )
			return true;
		return false;
	}
	return true;
 }
bool RtfSectionCommand::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, RtfAbstractReader& oAbstrReader, CString sCommand, bool hasParameter, int parameter)
{
	if( _T("11111111") == sCommand )
		;
	COMMAND_RTF_BOOL( _T("rtlsect"), oReader.m_oCurSectionProp.m_bBidi, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( _T("binfsxn"), oReader.m_oCurSectionProp.m_nPaperSourceFirst, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( _T("binsxn"), oReader.m_oCurSectionProp.m_nPaperSourceOther, sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( _T("rtlgutter"), oReader.m_oCurSectionProp.m_bRtlGutter, sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( _T("endnhere"), oReader.m_oCurSectionProp.m_bEndnotes, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( _T("ds"), oReader.m_oCurSectionProp.m_nStyle, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( _T("sbknone"), oReader.m_oCurSectionProp.m_eSectionBreak, sCommand, true, RtfSectionProperty::sb_sbknone)
	COMMAND_RTF_INT( _T("sbkcol"), oReader.m_oCurSectionProp.m_eSectionBreak, sCommand, true, RtfSectionProperty::sb_sbkcol)
	COMMAND_RTF_INT( _T("sbkpage"), oReader.m_oCurSectionProp.m_eSectionBreak, sCommand, true, RtfSectionProperty::sb_sbkpage)
	COMMAND_RTF_INT( _T("sbkeven"), oReader.m_oCurSectionProp.m_eSectionBreak, sCommand, true, RtfSectionProperty::sb_sbkeven)
	COMMAND_RTF_INT( _T("sbkodd"), oReader.m_oCurSectionProp.m_eSectionBreak, sCommand, true, RtfSectionProperty::sb_sbkodd)
	else if( _T("cols") == sCommand )
	{
		if( true == hasParameter )
		{
			oReader.m_oCurSectionProp.m_nColumnNumber = parameter;
			oReader.m_oCurSectionProp.m_oCollumnProperty.m_aCollumnProperty.resize( parameter );
		}
	}
	COMMAND_RTF_INT( _T("colsx"), oReader.m_oCurSectionProp.m_nColumnSpace, sCommand, hasParameter, parameter)
	else if( _T("colno") == sCommand )
	{
		if( true == hasParameter )
			nCurCollumnNumber = parameter - 1;
	}
	else if( _T("colsr") == sCommand )
	{
		if( true == hasParameter )
			if( nCurCollumnNumber >= 0 && nCurCollumnNumber < (int)oReader.m_oCurSectionProp.m_oCollumnProperty.m_aCollumnProperty.size() )
				oReader.m_oCurSectionProp.m_oCollumnProperty.m_aCollumnProperty[ nCurCollumnNumber ].m_nColumnSpaceToRightOfCol = parameter;
	}
	else if( _T("colw") == sCommand )
	{
		if( true == hasParameter )
			if( nCurCollumnNumber >= 0 && nCurCollumnNumber < (int)oReader.m_oCurSectionProp.m_oCollumnProperty.m_aCollumnProperty.size() )
				oReader.m_oCurSectionProp.m_oCollumnProperty.m_aCollumnProperty[ nCurCollumnNumber ].m_nColumnWidth = parameter;
	}
	COMMAND_RTF_BOOL( _T("linebetcol"), oReader.m_oCurSectionProp.m_bColumnLineBetween, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( _T("sftntj"), oReader.m_oCurSectionProp.m_eFootnotesJust, sCommand, true, RtfSectionProperty::fj_sftntj)
	COMMAND_RTF_INT( _T("sftnbj"), oReader.m_oCurSectionProp.m_eFootnotesJust, sCommand, true, RtfSectionProperty::fj_sftnbj)
	//Footnotes and Endnotes
	COMMAND_RTF_INT( _T("sftnstart"), oReader.m_oCurSectionProp.m_nFootnotesStart, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( _T("sftnrstpg"), oReader.m_oCurSectionProp.m_eFootnotesRestart, sCommand, true, RtfSectionProperty::fr_sftnrstpg)
	COMMAND_RTF_INT( _T("sftnrestart"), oReader.m_oCurSectionProp.m_eFootnotesRestart, sCommand, true, RtfSectionProperty::fr_sftnrestart)
	COMMAND_RTF_INT( _T("sftnrstcont"), oReader.m_oCurSectionProp.m_eFootnotesRestart, sCommand, true, RtfSectionProperty::fr_sftnrstcont)
	COMMAND_RTF_INT( _T("sftnnar"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnnar)
	COMMAND_RTF_INT( _T("sftnnalc"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnnalc)
	COMMAND_RTF_INT( _T("sftnnauc"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnnauc)
	COMMAND_RTF_INT( _T("sftnnrlc"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnnrlc)
	COMMAND_RTF_INT( _T("sftnnruc"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnnruc)
	COMMAND_RTF_INT( _T("sftnnchi"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnnchi)
	COMMAND_RTF_INT( _T("sftnnchosung"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnnchosung)
	COMMAND_RTF_INT( _T("sftnncnum"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnncnum)
	COMMAND_RTF_INT( _T("sftnndbnum"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnndbnum)
	COMMAND_RTF_INT( _T("sftnndbnumd"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnndbnumd)
	COMMAND_RTF_INT( _T("sftnndbnumt"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnndbnumt)
	COMMAND_RTF_INT( _T("sftnndbnumk"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnndbnumk)
	COMMAND_RTF_INT( _T("sftnndbar"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnndbar)
	COMMAND_RTF_INT( _T("sftnnganada"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnnganada)
	COMMAND_RTF_INT( _T("sftnngbnum"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnngbnum)
	COMMAND_RTF_INT( _T("sftnngbnumd"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnngbnumd)
	COMMAND_RTF_INT( _T("sftnngbnuml"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnngbnuml)
	COMMAND_RTF_INT( _T("sftnngbnumk"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnngbnumk)
	COMMAND_RTF_INT( _T("sftnnzodiac"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnnzodiac)
	COMMAND_RTF_INT( _T("sftnnzodiacd"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnnzodiacd)
	COMMAND_RTF_INT( _T("sftnnzodiacl"), oReader.m_oCurSectionProp.m_eFootnotesFormat, sCommand, true, RtfSectionProperty::ff_sftnnzodiacl)
						
	COMMAND_RTF_INT( _T("saftnstart"), oReader.m_oCurSectionProp.m_nEndnotesStart, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( _T("saftnrestart"), oReader.m_oCurSectionProp.m_eEndnotesRestart, sCommand, true, RtfSectionProperty::er_saftnrestart)
	COMMAND_RTF_INT( _T("saftnrstcont"), oReader.m_oCurSectionProp.m_eEndnotesRestart, sCommand, true, RtfSectionProperty::er_saftnrstcont)
	COMMAND_RTF_INT( _T("saftnnar"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnnar)
	COMMAND_RTF_INT( _T("saftnnalc"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnnalc)
	COMMAND_RTF_INT( _T("saftnnauc"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnnauc)
	COMMAND_RTF_INT( _T("saftnnrlc"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnnrlc)
	COMMAND_RTF_INT( _T("saftnnruc"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnnruc)
	COMMAND_RTF_INT( _T("saftnnchi"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnnchi)
	COMMAND_RTF_INT( _T("saftnnchosung"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnnchosung)
	COMMAND_RTF_INT( _T("saftnncnum"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnncnum)
	COMMAND_RTF_INT( _T("saftnndbnum"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnndbnum)
	COMMAND_RTF_INT( _T("saftnndbnumd"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnndbnumd)
	COMMAND_RTF_INT( _T("saftnndbnumt"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnndbnumt)
	COMMAND_RTF_INT( _T("saftnndbnumk"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnndbnumk)
	COMMAND_RTF_INT( _T("saftnndbar"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnndbar)
	COMMAND_RTF_INT( _T("saftnnganada"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnnganada)
	COMMAND_RTF_INT( _T("saftnngbnum"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnngbnum)
	COMMAND_RTF_INT( _T("saftnngbnumd"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnngbnumd)
	COMMAND_RTF_INT( _T("saftnngbnuml"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnngbnuml)
	COMMAND_RTF_INT( _T("saftnngbnumk"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnngbnumk)
	COMMAND_RTF_INT( _T("saftnnzodiac"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnnzodiac)
	COMMAND_RTF_INT( _T("saftnnzodiacd"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnnzodiacd)
	COMMAND_RTF_INT( _T("saftnnzodiacl"), oReader.m_oCurSectionProp.m_eEndnotesFormat, sCommand, true, RtfSectionProperty::ef_saftnnzodiacl)
	//Line Numbering
	COMMAND_RTF_INT( _T("linemod"), oReader.m_oCurSectionProp.m_nLineModulus, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( _T("linex"), oReader.m_oCurSectionProp.m_nLineX, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( _T("linestarts"), oReader.m_oCurSectionProp.m_nLineStart, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( _T("linerestart"), oReader.m_oCurSectionProp.m_eLineNumberRestart, sCommand, true, RtfSectionProperty::lnr_linerestart)
	COMMAND_RTF_INT( _T("lineppage"), oReader.m_oCurSectionProp.m_eLineNumberRestart, sCommand, true, RtfSectionProperty::lnr_lineppage)
	COMMAND_RTF_INT( _T("linecont"), oReader.m_oCurSectionProp.m_eLineNumberRestart, sCommand, true, RtfSectionProperty::lnr_linecont)
	//Page Information
	COMMAND_RTF_INT( _T("pgwsxn"), oReader.m_oCurSectionProp.m_nPageWidth, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( _T("pghsxn"), oReader.m_oCurSectionProp.m_nPageHeight, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( _T("marglsxn"), oReader.m_oCurSectionProp.m_nMarginLeft, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( _T("margrsxn"), oReader.m_oCurSectionProp.m_nMarginRight, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( _T("margtsxn"), oReader.m_oCurSectionProp.m_nMarginTop, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( _T("margbsxn"), oReader.m_oCurSectionProp.m_nMarginBottom, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( _T("guttersxn"), oReader.m_oCurSectionProp.m_nGutterMarginWidth, sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( _T("margmirsxn"), oReader.m_oCurSectionProp.m_bSwitchMargin, sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( _T("lndscpsxn"), oReader.m_oCurSectionProp.m_bLandscapeFormat, sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( _T("titlepg"), oReader.m_oCurSectionProp.m_bTitlePage, sCommand, hasParameter, parameter)
	//else if( _T("headery") == sCommand )
	//{
	//	static int nCount = 0;
	//	nCount++;
	//	ATLTRACE( "count %d\n", nCount );
	//}
	COMMAND_RTF_INT( _T("headery"), oReader.m_oCurSectionProp.m_nHeaderTop, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( _T("footery"), oReader.m_oCurSectionProp.m_nFooterBottom, sCommand, hasParameter, parameter)
	//Page Numbers
	COMMAND_RTF_INT( _T("pgnstarts"), oReader.m_oCurSectionProp.m_nPageNumberStart, sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( _T("pgncont"), oReader.m_oCurSectionProp.m_bPageNumberContinuos, sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( _T("pgnrestart"), oReader.m_oCurSectionProp.m_bPageNumberRestart, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( _T("pgnx"), oReader.m_oCurSectionProp.m_nPageNumberX, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( _T("pgny"), oReader.m_oCurSectionProp.m_nPageNumberY, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( _T("pgndec"), oReader.m_oCurSectionProp.m_ePageNumberFormat, sCommand, true, RtfSectionProperty::pnf_pgndec)
	COMMAND_RTF_INT( _T("pgnucrm"), oReader.m_oCurSectionProp.m_ePageNumberFormat, sCommand, true, RtfSectionProperty::pnf_pgnucrm)
	COMMAND_RTF_INT( _T("pgnlcrm"), oReader.m_oCurSectionProp.m_ePageNumberFormat, sCommand, true, RtfSectionProperty::pnf_pgnlcrm)
	COMMAND_RTF_INT( _T("vertalt"), oReader.m_oCurSectionProp.m_eVerticalAlignment, sCommand, true, RtfSectionProperty::va_vertalt)
	COMMAND_RTF_INT( _T("vertalb"), oReader.m_oCurSectionProp.m_eVerticalAlignment, sCommand, true, RtfSectionProperty::va_vertalb)
	COMMAND_RTF_INT( _T("vertalc"), oReader.m_oCurSectionProp.m_eVerticalAlignment, sCommand, true, RtfSectionProperty::va_vertalc)
	COMMAND_RTF_INT( _T("vertalj"), oReader.m_oCurSectionProp.m_eVerticalAlignment, sCommand, true, RtfSectionProperty::va_vertalj)
	//Text Flow
	COMMAND_RTF_INT( _T("stextflow"), oReader.m_oCurSectionProp.m_nTextFollow, sCommand, hasParameter, parameter)

	//Page Borders
	else if( _T("pgbrdrl") == sCommand )
		m_eInternalState = is_border_left;
	else if( _T("pgbrdrt") == sCommand )
		m_eInternalState = is_border_top;
	else if( _T("pgbrdrr") == sCommand )
		m_eInternalState = is_border_right;
	else if( _T("pgbrdrb") == sCommand )
		m_eInternalState = is_border_bottom;
	COMMAND_RTF_INT( _T("brdrart"), oReader.m_oCurSectionProp.m_nBorderArt, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( _T("pgbrdropt"), oReader.m_oCurSectionProp.m_nBorderMeasure, sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( _T("pgbrdrsna"), oReader.m_oCurSectionProp.m_nBorderAlign, sCommand, hasParameter, parameter)
    else if( _T("header") == sCommand || _T("footer") == sCommand ||  _T("headerl") == sCommand ||
             _T("headerr") == sCommand ||  _T("headerf") == sCommand ||  _T("footerl") == sCommand ||
             _T("footerr") == sCommand ||  _T("footerf") == sCommand )
	{
		ParagraphReader oParagraphReader(sCommand, oReader);
		oAbstrReader.StartSubReader( oParagraphReader, oDocument, oReader );
		
		oParagraphReader.m_oParPropDest.Finalize(oReader/*, RtfSectionPtr()*/);
		
		TextItemContainerPtr oNewFooterHeader = oParagraphReader.m_oParPropDest.m_oTextItems; 

		if( _T("header") == sCommand )
			oReader.m_oCurSectionProp.m_oHeaderRight = oNewFooterHeader;
		else if( _T("footer") == sCommand )
			oReader.m_oCurSectionProp.m_oFooterRight = oNewFooterHeader;
		else if( _T("headerl") == sCommand )
			oReader.m_oCurSectionProp.m_oHeaderLeft = oNewFooterHeader;
		else if( _T("headerr") == sCommand )
			oReader.m_oCurSectionProp.m_oHeaderRight = oNewFooterHeader;
		else if( _T("headerf") == sCommand )
			oReader.m_oCurSectionProp.m_oHeaderFirst = oNewFooterHeader;
		else if( _T("footerl") == sCommand )
			oReader.m_oCurSectionProp.m_oFooterLeft = oNewFooterHeader;
		else if( _T("footerr") == sCommand )
			oReader.m_oCurSectionProp.m_oFooterRight = oNewFooterHeader;
		else if( _T("footerf") == sCommand )
			oReader.m_oCurSectionProp.m_oFooterFirst = oNewFooterHeader;
	}
	else
	{
		bool bRes = false;
		if( is_border_left == m_eInternalState )
			bRes = RtfBorderCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, oReader.m_oCurSectionProp.m_oBorderLeft );
		else if( is_border_top == m_eInternalState )
			bRes = RtfBorderCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, oReader.m_oCurSectionProp.m_oBorderTop );
		else if( is_border_right == m_eInternalState )
			bRes = RtfBorderCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, oReader.m_oCurSectionProp.m_oBorderRight );
		else if( is_border_bottom == m_eInternalState )
			bRes = RtfBorderCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, oReader.m_oCurSectionProp.m_oBorderBottom );

		if( true == bRes )
			return true;
		return false;
	}

	return true;
}


bool RtfShadingCommand::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, CString sCommand, bool hasParameter, int parameter, RtfShading& oOutput )
{
	if( _T("bghoriz") == sCommand )
		oOutput.m_eType = RtfShading::st_chbghoriz;
	else if( _T("bgvert") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgvert;
	else if( _T("bgfdiag") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgfdiag;
	else if( _T("bgbdiag") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgbdiag;
	else if( _T("bgcross") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgcross;
	else if( _T("bgdcross") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdcross;
	else if( _T("bgdkhoriz") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkhoriz;
	else if( _T("bgdkvert") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkvert;
	else if( _T("bgdkfdiag") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkfdiag;
	else if( _T("bgdkbdiag") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkbdiag;
	else if( _T("bgdkcross") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkcross;
	else if( _T("bgdkdcross") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkdcross;
	else if( _T("cfpat") == sCommand )
	{
		if( true == hasParameter )
			oOutput.m_nForeColor = parameter;
	}
	else if( _T("cbpat") == sCommand )
	{
		if( true == hasParameter )
			oOutput.m_nBackColor = parameter;
	}
	else if( _T("shading") == sCommand )
	{
		if( true == hasParameter )
			oOutput.m_nValue = parameter;
	}
	else
		return false;
	return true;
}
bool ShapeReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
{
	if( _T("shp") == sCommand )
		return true;
	else if( _T("shpinst") == sCommand )
		return true;
	else if( _T("shprslt") == sCommand )
		return false;
	else if( _T("picprop") == sCommand )
		return true;
	else if( _T("shptxt") == sCommand )
	{
		if( PROP_DEF == m_oShape.m_nShapeType )
			m_oShape.m_nShapeType = 202;//Text box
		
		ParagraphReader oParagraphReader(_T("shptxt"), oReader);
		StartSubReader( oParagraphReader, oDocument, oReader );
		m_oShape.m_aTextItems = oParagraphReader.m_oParPropDest.m_oTextItems;
	}
	else if( _T("sp") == sCommand )
	{
		ShapePropertyReader oShapePropReader(m_oShape);
		StartSubReader(oShapePropReader, oDocument, oReader);
	}
	else if( _T("shpleft") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_nLeft = parameter;
	}
	else if( _T("shptop") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_nTop = parameter;
	}
	else if( _T("shpbottom") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_nBottom = parameter;
	}
	else if( _T("shpright") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_nRight = parameter;
	}
	else if( _T("shplid") == sCommand )
	{
		if( true == hasParameter )
		{
			m_oShape.m_nID = parameter;
			oDocument.SetShapeId( parameter );
		}
	}
	else if( _T("shpz") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_nZOrder = parameter;
	}
	else if( _T("shpfhdr") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_nHeader = parameter;
	}
	//else if( _T("shpbxignore") == sCommand )
	//	m_oShape.m_eXAnchor = RtfShape::ax_ignore;
	else if( _T("shpbxpage") == sCommand )
		m_oShape.m_eXAnchor = RtfShape::ax_page;
	else if( _T("shpbxmargin") == sCommand )
		m_oShape.m_eXAnchor = RtfShape::ax_margin;
	else if( _T("shpbxcolumn") == sCommand )
		m_oShape.m_eXAnchor = RtfShape::ax_column;
	//else if( _T("shpbyignore") == sCommand )
	//	m_oShape.m_eYAnchor = RtfShape::ay_ignore;
	else if( _T("shpbypage") == sCommand )
		m_oShape.m_eYAnchor = RtfShape::ay_page;
	else if( _T("shpbymargin") == sCommand )
		m_oShape.m_eYAnchor = RtfShape::ay_margin;
	else if( _T("shpbypara") == sCommand )
		m_oShape.m_eYAnchor = RtfShape::ay_Para;
	else if( _T("shpwr") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_nWrapType = parameter;
	}
	else if( _T("shpwrk") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_nWrapSideType = parameter;
	}
	else if( _T("shpfblwtxt") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_nZOrderRelative = parameter;
	}
	else if( _T("shplockanchor") == sCommand )
		m_oShape.m_bLockAnchor = true;
	else
		return false;
	return true;
}
bool RtfShadingCellCommand::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter, RtfShading& oOutput)
{
	//для свойст таблицы и для стилей таблицы
	if( _T("clshdrawnil") == sCommand )
		oOutput.m_eType = RtfShading::st_clshdrawnil;
	else if( _T("rawclbgvert") == sCommand || _T("clbgvert") == sCommand || _T("tsbgvert") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgvert;
	else if( _T("rawclbgfdiag") == sCommand || _T("clbgfdiag") == sCommand || _T("tsbgfdiag") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgfdiag;
	else if( _T("rawclbgbdiag") == sCommand || _T("clbgbdiag") == sCommand || _T("tsbgbdiag") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgbdiag;
	else if( _T("rawclbgcross") == sCommand || _T("clbgcross") == sCommand || _T("tsbgcross") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgcross;
	else if( _T("rawclbgdcross") == sCommand || _T("clbgdcross") == sCommand || _T("tsbgdcross") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdcross;
	else if( _T("rawclbgdkhor") == sCommand || _T("clbgdkhor") == sCommand || _T("tsbgdkhor") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkhoriz;
	else if( _T("rawclbgdkvert") == sCommand || _T("clbgdkvert") == sCommand || _T("tsbgdkvert") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkvert;
	else if( _T("rawclbgdkfdiag") == sCommand || _T("clbgdkfdiag") == sCommand || _T("tsbgdkfdiag") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkfdiag;
	else if( _T("rawclbgdkbdiag") == sCommand || _T("clbgdkbdiag") == sCommand || _T("tsbgdkbdiag") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkbdiag;
	else if( _T("rawclbgdkcross") == sCommand || _T("clbgdkcross") == sCommand || _T("tsbgdkcross") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkcross;
	else if( _T("rawclbgdkdcross") == sCommand || _T("clbgdkdcross") == sCommand || _T("tsbgdkdcross") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkdcross;
	COMMAND_RTF_INT( _T("clcfpat"),			oOutput.m_nForeColor, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clcbpat"), oOutput.m_nBackColor, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clcfpatraw"), oOutput.m_nForeColor, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clcbpatraw"), oOutput.m_nBackColor, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tscellcfpat"), oOutput.m_nForeColor, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tscellcbpat"), oOutput.m_nBackColor, sCommand, hasParameter, parameter )

	COMMAND_RTF_INT( _T("clshdng"), oOutput.m_nValue, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clshdngraw"), oOutput.m_nValue, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tscellpct"), oOutput.m_nValue, sCommand, hasParameter, parameter )
	else
		return false;

	if (sCommand == _T("clshdng"))		oReader.m_oState->m_oCellProperty.m_nShadingPctFrom = 1;
	if (sCommand == _T("clshdngraw"))	oReader.m_oState->m_oCellProperty.m_nShadingPctFrom = 2;
	return true;
}

bool RtfShadingCharCommand::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter, RtfShading& oOutput )
{
	if( _T("chbghoriz") == sCommand )
		oOutput.m_eType = RtfShading::st_chbghoriz;
	else if( _T("chbgvert") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgvert;
	else if( _T("chbgfdiag") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgfdiag;
	else if( _T("chbgbdiag") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgbdiag;
	else if( _T("chbgcross") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgcross;
	else if( _T("chbgdcross") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdcross;
	else if( _T("chbgdkhoriz") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkhoriz;
	else if( _T("chbgdkvert") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkvert;
	else if(  _T("chbgdkfdiag") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkfdiag;
	else if( _T("chbgdkbdiag") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkbdiag;
	else if(  _T("chbgdkcross") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkcross;
	else if( _T("chbgdkdcross") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkdcross;
	else if( _T("chcfpat") == sCommand )
	{
		if( true == hasParameter )
			oOutput.m_nForeColor = parameter;
	}
	else if( _T("chcbpat") == sCommand )
	{
		if( true == hasParameter )
			oOutput.m_nBackColor = parameter;
	}
	else if( _T("chshdng") == sCommand )
	{
		if( true == hasParameter )
			oOutput.m_nValue = parameter;
	}
	else
		return false;
	return true;
}

bool RtfShadingRowCommand::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter, RtfShading& oOutput)
{
	if( _T("trbghoriz") == sCommand )
		oOutput.m_eType = RtfShading::st_chbghoriz;
	else if( _T("trbgvert") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgvert;
	else if( _T("trbgfdiag") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgfdiag;
	else if( _T("trbgbdiag") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgbdiag;
	else if( _T("trbgcross") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgcross;
	else if( _T("trbgdcross") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdcross;
	else if( _T("trbgdkhor") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkhoriz;
	else if( _T("trbgdkvert") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkvert;
	else if( _T("trbgdkfdiag") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkfdiag;
	else if( _T("trbgdkbdiag") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkbdiag;
	else if( _T("trbgdkcross") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkcross;
	else if( _T("trbgdkdcross") == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkdcross;
	else if( _T("trcfpat") == sCommand )
	{
		if( true == hasParameter )
			oOutput.m_nForeColor = parameter;
	}
	else if( _T("trcbpat") == sCommand )
	{
		if( true == hasParameter )
			oOutput.m_nBackColor = parameter;
	}
	else if( _T("trshdng") == sCommand )
	{
		if( true == hasParameter )
			oOutput.m_nValue = parameter;
	}
	else
		return false;
	return true;
}

bool RtfCharPropCommand::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter, RtfCharProperty & ChartProperty, bool bLookOnBorder)
{
	if( _T("plain") == sCommand )
		oReader.m_oState->m_oCharProp.SetDefaultRtf();
	
	COMMAND_RTF_INT( _T("animtext")	, ChartProperty.m_nAnimated, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( _T("uc")		, oReader.m_oState->m_nUD, sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( _T("b")		, ChartProperty.m_bBold, sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( _T("caps")	, ChartProperty.m_bCaps, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( _T("charscalex"), ChartProperty.m_nScalex, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( _T("cs")		, ChartProperty.m_nCharStyle, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( _T("down")		, ChartProperty.m_nDown, sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( _T("embo")	, ChartProperty.m_bEmbo, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( _T("expndtw")	, ChartProperty.m_nCharacterSpacing, sCommand, hasParameter, parameter)
	else if( _T("expnd") == sCommand )
	{
		if( true == hasParameter )
			ChartProperty.m_nCharacterSpacing  = 5 * parameter; //quater -points
	}
	COMMAND_RTF_INT( _T("fittext")	, ChartProperty.m_nFitText, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( _T("f")		, ChartProperty.m_nFont, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( _T("fs")		, ChartProperty.m_nFontSize, sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( _T("i")		, ChartProperty.m_bItalic, sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( _T("impr")	, ChartProperty.m_bImprint, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( _T("kerning")	, ChartProperty.m_nKerning, sCommand, hasParameter, parameter)
	
	else if( _T("ltrch") == sCommand )
	{
		if( false == hasParameter || 0 != parameter ) 
			ChartProperty.m_bRightToLeft  = 0;
		else
			ChartProperty.m_bRightToLeft  = 1;
	}
	COMMAND_RTF_BOOL( _T("rtlch"),		ChartProperty.m_bRightToLeft, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( _T("lang"),		ChartProperty.m_nLanguage	, sCommand, hasParameter, parameter)
	
	COMMAND_RTF_BOOL( _T("cs"),			ChartProperty.m_nComplexScript, sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( _T("outl"),		ChartProperty.m_bOutline	, sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( _T("scaps"),		ChartProperty.m_bScaps		, sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( _T("shad"),		ChartProperty.m_bShadow		, sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( _T("strike"),		ChartProperty.m_bStrike		, sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( _T("striked"),	ChartProperty.m_nStriked	, sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( _T("sub"),		ChartProperty.m_bSub		, sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( _T("super"),		ChartProperty.m_bSuper		, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( _T("highlight"),	ChartProperty.m_nHightlited	, sCommand, hasParameter, parameter)
	else if( _T("cf") == sCommand )
	{
		if( true == hasParameter )
			ChartProperty.m_nForeColor = parameter;
		else
			ChartProperty.m_nForeColor= PROP_DEF;
	}
	else if( _T("ul") == sCommand )
	{
		if( true == hasParameter && 0 == parameter)
			ChartProperty.m_eUnderStyle = RtfCharProperty::uls_none;
		else
			ChartProperty.m_eUnderStyle = RtfCharProperty::uls_Single;
	}
	//COMMAND_RTF_BOOL( _T("ul"), ChartProperty.m_bUnderline, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( _T("ulc"), ChartProperty.m_nUnderlineColor, sCommand, hasParameter, parameter)


	COMMAND_RTF_INT( _T("uld"),		ChartProperty.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_Dotted)
	COMMAND_RTF_INT( _T("uldash"),	ChartProperty.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_Dashed)
	COMMAND_RTF_INT( _T("uldashd"), ChartProperty.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_Dash_dotted)
	COMMAND_RTF_INT( _T("uldashdd"), ChartProperty.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_Dash_dot_dotted)
	COMMAND_RTF_INT( _T("uldb"),	ChartProperty.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_Double)
	COMMAND_RTF_INT( _T("ulhwave"), ChartProperty.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_Heavy_wave)
	COMMAND_RTF_INT( _T("ulldash"), ChartProperty.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_Long_dashe)
	COMMAND_RTF_INT( _T("ulnone"), ChartProperty.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_none)
	COMMAND_RTF_INT( _T("ulth"),	ChartProperty.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_Thick)
	COMMAND_RTF_INT( _T("ulthd"),	ChartProperty.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_Thick_dotted)
	COMMAND_RTF_INT( _T("ulthdash"), ChartProperty.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_Thick_dashed)
	COMMAND_RTF_INT( _T("ulthdashd"), ChartProperty.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_Thick_dash_dotted)
	COMMAND_RTF_INT( _T("ulthdashdd"), ChartProperty.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_Thick_dash_dot_dotted)
	COMMAND_RTF_INT( _T("ulthldash"), ChartProperty.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_Thick_long_dashed)
	COMMAND_RTF_INT( _T("ululdbwave"), ChartProperty.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_Double_wave)
	COMMAND_RTF_INT( _T("ulw"), ChartProperty.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_Word)
	COMMAND_RTF_INT( _T("ulwave"), ChartProperty.m_eUnderStyle, sCommand, true, RtfCharProperty::uls_Wave)
	COMMAND_RTF_INT( _T("up"), ChartProperty.m_nUp, sCommand, hasParameter, parameter)
	else if( _T("nosupersub") == sCommand )
	{
		ChartProperty.m_bSub  = 0;
		ChartProperty.m_bSuper  = 0;
	}
	else if( _T("nosectexpand") == sCommand )
	{
		ChartProperty.m_nCharacterSpacing  = PROP_DEF;
	}
	else
	{
		bool bResult ;
		bResult = RtfShadingCharCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, ChartProperty.m_poShading);
		if( true == bResult )
				return true;
		if( true == bLookOnBorder )
		{
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, ChartProperty.m_poBorder);
			if( true == bResult )
					return true;
		}
		return false;
	}
	return true;
}
bool PictureReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
{
	if( _T("pict") == sCommand )
		return true;
	else if( _T("picprop") == sCommand )
	{
		ShapeReader oShapeReader(m_oShape);
		StartSubReader( oShapeReader, oDocument, oReader );
	}
	else if( _T("emfblip") == sCommand )
		m_oShape.m_oPicture->eDataType = RtfPicture::dt_emf;
	else if( _T("pngblip") == sCommand )
		m_oShape.m_oPicture->eDataType = RtfPicture::dt_png;
	else if( _T("jpegblip") == sCommand )
		m_oShape.m_oPicture->eDataType = RtfPicture::dt_jpg;
	else if( _T("wmetafile") == sCommand )
	{
		if( true == hasParameter && 8 == parameter )
		m_oShape.m_oPicture->eDataType = RtfPicture::dt_wmf;
	}
	else if( _T("picw") == sCommand )
	{
		if( true == hasParameter )
		{
			m_oShape.m_oPicture->m_nWidth = parameter;
		}
	}
	else if( _T("pich") == sCommand )
	{
		if( true == hasParameter )
		{
			m_oShape.m_oPicture->m_nHeight = parameter;
		}
	}
	else if( _T("picwgoal") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_oPicture->m_nWidthGoal = parameter;
	}	
	else if( _T("pichgoal") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_oPicture->m_nHeightGoal = parameter;
	}
	else if( _T("picscalex") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_oPicture->m_dScaleX = parameter;
	}
	else if( _T("picscaley") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_oPicture->m_dScaleY = parameter;
	}
	else if( _T("picscaled") == sCommand )
			m_oShape.m_oPicture->m_bScaled = 1;
	else if( _T("piccropl") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_oPicture->m_nCropL = parameter;
	}
	else if( _T("piccropt") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_oPicture->m_nCropT = parameter;
	}
	else if( _T("piccropr") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_oPicture->m_nCropR = parameter;
	}
	else if( _T("piccropb") == sCommand )
	{
		if( true == hasParameter )
			m_oShape.m_oPicture->m_nCropB = parameter;
	}
	else if( _T("bin") == sCommand)
	{
		if( true == hasParameter )
		{
			//читаем картинку как бинарник длиной parameter
			m_bBin = true;
			m_nBinLength = parameter;
			oReader.m_oLex.ReadBytes( parameter, &m_pbBin );//читаем сразу байты, потому что если между ними и был пробел, то он пропустится в RtfLex::parseKeyword
		}
	}
	else
		return false;
	return true;
}
bool FieldReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
{
	if( _T("field") == sCommand )
		return true;
	else if( _T("flddirty") == sCommand )
		m_oField.m_eMode = RtfField::fm_flddirty;
	else if( _T("fldedit") == sCommand )
		m_oField.m_eMode = RtfField::fm_fldedit;
	else if( _T("fldlock") == sCommand )
		m_oField.m_eMode = RtfField::fm_fldlock;
	else if( _T("fldpriv") == sCommand )
		m_oField.m_eMode = RtfField::fm_fldpriv;
	else if( _T("fldalt") == sCommand )
		m_oField.m_bReferenceToEndnote = true;

	else if( _T("fldinst") == sCommand )
	{
		ParagraphReader oParagraphReader( _T("fldinst"), oReader );
		StartSubReader( oParagraphReader, oDocument, oReader );
		m_oField.m_oInsert = oParagraphReader.m_oParPropDest.m_oTextItems;
	}
	else if( _T("datafield") == sCommand )
		Skip( oDocument, oReader );
	else if( _T("fldrslt") == sCommand )
	{
		ParagraphReader oParagraphReader( _T("fldrslt"), oReader );
		StartSubReader( oParagraphReader, oDocument, oReader );
		m_oField.m_oResult = oParagraphReader.m_oParPropDest.m_oTextItems;
		oReader.m_oLex.putString( "}{" );//чтобы не терять после fldrslt
		//{\field{\*\fldinst...}{\*\fldrslt...} ??? }
		//{\field{\*\fldinst...}{\*\fldrslt...}}{ ??? }
	}
	else
		return false;
	return true;
}
bool OleReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
{
	if( _T("object") == sCommand )
		return true;
	
	COMMAND_RTF_INT( _T("objw"), m_oOle.m_nWidth, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("objh"), m_oOle.m_nHeight, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("objemb"), m_oOle.m_eOleType, sCommand, true, RtfOle::ot_emb )
	COMMAND_RTF_INT( _T("objlink"), m_oOle.m_eOleType, sCommand, true, RtfOle::ot_link )
	else if( _T("objclass") == sCommand )
	{
		TextReader oTextReader( m_oOle.m_sOleClass, false );
		StartSubReader( oTextReader, oDocument, oReader );
	}
	else if( _T("objdata") == sCommand )
	{
		CString sOleData;
		TextReader oTextReader( sOleData, false );
		StartSubReader( oTextReader, oDocument, oReader );

		BYTE *pData = NULL;
		long nSize = 0;
		RtfUtility::WriteDataToBinary( sOleData, &pData, nSize );
		if( 0 != nSize  && pData)
		{
			HRESULT hRes = S_FALSE;

			//конвертация Ole1 в Ole2
#if 0//defined(_WIN32) || defined(_WIN64)
			RtfOle1ToOle2Stream oStream;

			oStream.lpstbl = new OLESTREAMVTBL();
			oStream.lpstbl->Get = &OleGet1;
			oStream.lpstbl->Put = &OlePut1;
			oStream.pBuffer = pData;
			oStream.nBufferSize = nSize;
			oStream.nCurPos = 0;

			CString sOleStorageName = Utils::CreateTempFile( oReader.m_sTempFolder );

			IStorage* piMSStorage = NULL;
			if( SUCCEEDED( StgCreateDocfile( sOleStorageName, STGM_READWRITE | STGM_CREATE | STGM_SHARE_EXCLUSIVE | STGM_TRANSACTED/* | STGM_DELETEONRELEASE*/, NULL, &piMSStorage) ) )
			{
				hRes = OleConvertOLESTREAMToIStorage( &oStream, piMSStorage, NULL );
				piMSStorage->Commit( STGC_DEFAULT );
				RELEASEINTERFACE( piMSStorage );
			}			
			delete oStream.lpstbl;
#else
			std::wstring sOleStorageName = FileSystem::Directory::CreateTempFileWithUniqueName(oReader.m_sTempFolder, _T("img"));

			hRes = ConvertOle1ToOle2(pData, nSize, sOleStorageName);
			
#endif
			delete[] pData;
			
			POLE::Storage * piRootStorage = new POLE::Storage(sOleStorageName.c_str());			
			if( piRootStorage)
			{
				m_oOle.SetFilename( sOleStorageName.c_str() );
				m_oOle.SetOle( piRootStorage );
				hRes = S_OK;
			}

            if(  hRes != S_OK )
				Utils::RemoveDirOrFile( sOleStorageName.c_str() );
		}
	}
	else if( _T("result") == sCommand )
	{
		RtfShapePtr oNewShape = RtfShapePtr( new RtfShape() );
		AllPictReader oAllPictReader( *oNewShape );
		StartSubReader( oAllPictReader, oDocument, oReader );
		m_oOle.m_oResultPic = oNewShape;
	}
	else
		return false;
	return true;
}

bool RtfOldListReader::ExecuteCommand( RtfDocument& oDocument, RtfReader& oReader, CString sKey, bool bHasPar, int nPar )
 {
	if( _T("pn") == sKey )
		return true;
	else if( _T("pnlvlblt") == sKey )
	{
		m_oTarget.m_eLevelType = RtfOldList::lt_blt;
		m_oTarget.m_nIlvl = 0;
	}
	COMMAND_RTF_INT( _T("pnf"), oReader.m_oState->m_oCharProp.m_nFont, sKey, bHasPar, nPar )
	COMMAND_RTF_INT( _T("pnfs"), oReader.m_oState->m_oCharProp.m_nFontSize, sKey, bHasPar, nPar )
	COMMAND_RTF_BOOL( _T("pnb"), oReader.m_oState->m_oCharProp.m_bBold, sKey, bHasPar, nPar )
	COMMAND_RTF_BOOL( _T("pni"), oReader.m_oState->m_oCharProp.m_bItalic, sKey, bHasPar, nPar )
	COMMAND_RTF_BOOL( _T("pncaps"), oReader.m_oState->m_oCharProp.m_bCaps, sKey, bHasPar, nPar )
	COMMAND_RTF_BOOL( _T("pnstrike"), oReader.m_oState->m_oCharProp.m_bStrike, sKey, bHasPar, nPar )
	COMMAND_RTF_INT( _T("pnul"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_Single)
	COMMAND_RTF_INT( _T("pnuld"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_Dashed)
	COMMAND_RTF_INT( _T("pnuldash"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_Dash_dotted)
	COMMAND_RTF_INT( _T("pnuldashdd"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_Dash_dot_dotted)
	COMMAND_RTF_INT( _T("pnulth"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_Thick)
	COMMAND_RTF_INT( _T("pnulwave"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_Wave)
	COMMAND_RTF_INT( _T("pnuldb"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_Double)
	COMMAND_RTF_INT( _T("pnulnone"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_none)
	COMMAND_RTF_INT( _T("pnulnone"), oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_Word)

	COMMAND_RTF_INT( _T("pnindent"), m_oTarget.m_oLevelText->m_oProperty.m_nIndLeft, sKey, bHasPar, nPar )
	COMMAND_RTF_INT( _T("pnsp"), m_oTarget.m_oLevelText->m_oProperty.m_nIndLeft, sKey, bHasPar, nPar )
	COMMAND_RTF_INT( _T("pnstart"), m_oTarget.m_oLevelText->m_oProperty.m_nIndLeft, sKey, bHasPar, nPar )
	COMMAND_RTF_INT( _T("pnhang"), m_oTarget.m_oLevelText->m_oProperty.m_nIndLeft, sKey, bHasPar, nPar )

	COMMAND_RTF_INT( _T("pnqc"), m_oTarget.m_eLevelJust, sKey, true, RtfOldList::lj_center )
	COMMAND_RTF_INT( _T("pnql"), m_oTarget.m_eLevelJust, sKey, true, RtfOldList::lj_left )
	COMMAND_RTF_INT( _T("pnqr"), m_oTarget.m_eLevelJust, sKey, true, RtfOldList::lj_right )

	else if( _T("pntxtb") == sKey )
	{
		CString sNumberingText;
		TextReader oTextReader( sNumberingText, false );
		StartSubReader( oTextReader, oDocument, oReader );
		RtfCharPtr oNewChar = RtfCharPtr( new RtfChar() );
		oNewChar->setText( sNumberingText );
		m_oTarget.m_oLevelText->AddItem( oNewChar );
	}
	else
		return false;
	return true;
}

bool ParagraphPropDestination::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,RtfAbstractReader& oAbstrReader,CString sCommand, bool hasParameter, int parameter)
{
	m_oReader = &oReader;
	m_bPar = false;

	bool bContinue = false;
	//PARAGRAPH Property
	if( _T("pard") == sCommand )
	{
		oReader.m_oState->m_oParagraphProp.SetDefaultRtf();
		oReader.m_oState->m_oCurOldList.SetDefault();
	}
	else if( _T("trowd") == sCommand )
		oReader.m_oState->m_oRowProperty.SetDefaultRtf();
	else if(  _T("tcelld") == sCommand  )
		oReader.m_oState->m_oCellProperty.SetDefaultRtf();
	else if( _T("par") == sCommand )
	{
		m_bPar = true;
		m_oCurParagraph->m_oProperty = oReader.m_oState->m_oParagraphProp;
		m_oCurParagraph->m_oOldList = RtfOldListPtr( new RtfOldList() );
		*m_oCurParagraph->m_oOldList = oReader.m_oState->m_oCurOldList;
		m_oCurParagraph->m_oProperty.m_oCharProperty = oReader.m_oState->m_oCharProp;
		AddItem( m_oCurParagraph, oReader, false, false );
		m_oCurParagraph = RtfParagraphPtr(new RtfParagraph());

	}
	else if( _T("cell") == sCommand  || _T("nestcell") == sCommand )
	{
		m_oCurParagraph->m_oProperty = oReader.m_oState->m_oParagraphProp;
		m_oCurParagraph->m_oOldList = RtfOldListPtr( new RtfOldList() );
		*m_oCurParagraph->m_oOldList = oReader.m_oState->m_oCurOldList;
		m_oCurParagraph->m_oProperty.m_oCharProperty = oReader.m_oState->m_oCharProp;
		AddItem( m_oCurParagraph, oReader, true, false );
		m_oCurParagraph = RtfParagraphPtr(new RtfParagraph());
	}
    else if( _T("row") == sCommand || _T("nestrow") == sCommand)
	{
		m_oCurParagraph->m_oProperty = oReader.m_oState->m_oParagraphProp;
		m_oCurParagraph->m_oOldList = RtfOldListPtr( new RtfOldList() );
		*m_oCurParagraph->m_oOldList = oReader.m_oState->m_oCurOldList;
		m_oCurParagraph->m_oProperty.m_oCharProperty = oReader.m_oState->m_oCharProp;
		AddItem( m_oCurParagraph, oReader, false, true );
		m_oCurParagraph = RtfParagraphPtr(new RtfParagraph());
	}
	else if( _T("nesttableprops") == sCommand )
	{
		oReader.m_oState->m_oRowProperty.SetDefaultRtf();
	}
	else if( _T("nonesttables") == sCommand )
	{
		oAbstrReader.Skip( oDocument, oReader );
	}
	else if( _T("listtext") == sCommand )
	{
		oAbstrReader.Skip( oDocument, oReader );
	}
	else if( _T("pntext") == sCommand )
	{
		//пропускаем списки office 95, если есть списки office 2007
		if( oDocument.m_oListTabel.GetCount() > 0 )
			oAbstrReader.Skip( oDocument, oReader );
		else
		{
			ParagraphReader oParagraphReader( _T("pntext"), oReader );
			oAbstrReader.StartSubReader( oParagraphReader, oDocument, oReader );
			if( NULL != oParagraphReader.m_oParPropDest.m_oTextItems && oParagraphReader.m_oParPropDest.m_oTextItems->GetCount() )
			{
				ITextItemPtr piCurContainer;
				oParagraphReader.m_oParPropDest.m_oTextItems->GetItem( piCurContainer, 0 );
				if( NULL != piCurContainer && TYPE_RTF_PARAGRAPH == piCurContainer->GetType() )
                    oReader.m_oState->m_oCurOldList.m_oText = boost::static_pointer_cast<RtfParagraph, ITextItem>( piCurContainer ) ;
			}
		}
	}
	else if( _T("pn") == sCommand )
	{
		if( oDocument.m_oListTabel.GetCount() > 0 )
			oAbstrReader.Skip( oDocument, oReader );
		else
		{
			RtfOldList oOldList;
			RtfOldListReader oOldListReader( oOldList );
			oAbstrReader.StartSubReader( oOldListReader, oDocument, oReader );

			oReader.m_oState->m_oCurOldList = oOldList;
			bool bExist = false;
			for( int i = 0; i < (int)oDocument.m_aOldLists.size(); i++ )
				if( oReader.m_oState->m_oCurOldList == *oDocument.m_aOldLists[i] )
				{
					bExist = true;
					oReader.m_oState->m_oCurOldList.m_nLs = oDocument.m_aOldLists[i]->m_nLs;
					oReader.m_oState->m_oCurOldList.m_nIlvl = oDocument.m_aOldLists[i]->m_nIlvl;
					break;
				}
			if( false == bExist )
			{
				oReader.m_oState->m_oCurOldList.m_nLs = oDocument.m_oIdGenerator.Generate_PnId();
				RtfOldListPtr oNewOldList = RtfOldListPtr( new RtfOldList() );
				*oNewOldList = oReader.m_oState->m_oCurOldList;
				oDocument.m_aOldLists.push_back( oNewOldList );
			}
		}
	}
	COMMAND_RTF_BOOL( _T("hyphpar"), oReader.m_oState->m_oParagraphProp.m_bAutoHyphenation, sCommand, hasParameter, parameter )
	else if( _T("intbl") == sCommand )
	{
		if( true == hasParameter && 0 == parameter )
		{
			oReader.m_oState->m_oParagraphProp.m_bInTable = 0;
			if( PROP_DEF == oReader.m_oState->m_oParagraphProp.m_nItap )
				oReader.m_oState->m_oParagraphProp.m_nItap = 0;
		}
		else
		{
			oReader.m_oState->m_oParagraphProp.m_bInTable = 1;
			if( PROP_DEF == oReader.m_oState->m_oParagraphProp.m_nItap )
				oReader.m_oState->m_oParagraphProp.m_nItap = 1;
		}
	}
	COMMAND_RTF_BOOL( _T("intbl"), oReader.m_oState->m_oParagraphProp.m_bInTable, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("itap"), oReader.m_oState->m_oParagraphProp.m_nItap, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("keep"), oReader.m_oState->m_oParagraphProp.m_bKeep, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("keepn"), oReader.m_oState->m_oParagraphProp.m_bKeepNext, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("pagebb"), oReader.m_oState->m_oParagraphProp.m_bPageBB, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("outlinelevel"), oReader.m_oState->m_oParagraphProp.m_nOutlinelevel, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("s"), oReader.m_oState->m_oParagraphProp.m_nStyle, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("qc"), oReader.m_oState->m_oParagraphProp.m_eAlign, sCommand, true, RtfParagraphProperty::pa_qc )
	COMMAND_RTF_INT( _T("qj"), oReader.m_oState->m_oParagraphProp.m_eAlign, sCommand, true, RtfParagraphProperty::pa_qj )
	COMMAND_RTF_INT( _T("ql"), oReader.m_oState->m_oParagraphProp.m_eAlign, sCommand, true, RtfParagraphProperty::pa_ql )
	COMMAND_RTF_INT( _T("qr"), oReader.m_oState->m_oParagraphProp.m_eAlign, sCommand, true, RtfParagraphProperty::pa_qr )
	COMMAND_RTF_INT( _T("qd"), oReader.m_oState->m_oParagraphProp.m_eAlign, sCommand, true, RtfParagraphProperty::pa_qd )
	else if( _T("qk0") == sCommand )
	{
		switch( parameter )
		{
			case 0:oReader.m_oState->m_oParagraphProp.m_eAlign = RtfParagraphProperty::pa_qk0;break;
			case 10:oReader.m_oState->m_oParagraphProp.m_eAlign = RtfParagraphProperty::pa_qk10;break;
			case 20:oReader.m_oState->m_oParagraphProp.m_eAlign = RtfParagraphProperty::pa_qk20;break;

		}
	}
	COMMAND_RTF_INT( _T("faauto"), oReader.m_oState->m_oParagraphProp.m_eFontAlign, sCommand, true, RtfParagraphProperty::fa_faauto )
	COMMAND_RTF_INT( _T("fahang"), oReader.m_oState->m_oParagraphProp.m_eFontAlign, sCommand, true, RtfParagraphProperty::fa_fahang )
	COMMAND_RTF_INT( _T("facenter"), oReader.m_oState->m_oParagraphProp.m_eFontAlign, sCommand, true, RtfParagraphProperty::fa_facenter )
	COMMAND_RTF_INT( _T("faroman"), oReader.m_oState->m_oParagraphProp.m_eFontAlign, sCommand, true, RtfParagraphProperty::fa_faroman )
	COMMAND_RTF_INT( _T("favar"), oReader.m_oState->m_oParagraphProp.m_eFontAlign, sCommand, true, RtfParagraphProperty::fa_favar )
	COMMAND_RTF_INT( _T("fafixed"), oReader.m_oState->m_oParagraphProp.m_eFontAlign, sCommand, true, RtfParagraphProperty::fa_fafixed )
	COMMAND_RTF_INT( _T("fi"), oReader.m_oState->m_oParagraphProp.m_nIndFirstLine, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("li"), oReader.m_oState->m_oParagraphProp.m_nIndLeft, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("ri"), oReader.m_oState->m_oParagraphProp.m_nIndRight, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("lin"), oReader.m_oState->m_oParagraphProp.m_nIndStart, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("rin"), oReader.m_oState->m_oParagraphProp.m_nIndEnd, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("adjustright"), oReader.m_oState->m_oParagraphProp.m_bIndRightAuto, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("indmirror"), oReader.m_oState->m_oParagraphProp.m_bIndMirror, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("sb"), oReader.m_oState->m_oParagraphProp.m_nSpaceBefore, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("sa"), oReader.m_oState->m_oParagraphProp.m_nSpaceAfter, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("sbauto"), oReader.m_oState->m_oParagraphProp.m_nSpaceBeforeAuto, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("saauto"), oReader.m_oState->m_oParagraphProp.m_nSpaceAfterAuto, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("lisb"), oReader.m_oState->m_oParagraphProp.m_nSpaceBeforeLine, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("lisa"), oReader.m_oState->m_oParagraphProp.m_nSpaceAfterLine, sCommand, hasParameter, parameter )
	else if( _T("sl") == sCommand )
	{
		if( true == hasParameter )
		{
			oReader.m_oState->m_oParagraphProp.m_nSpaceBetween = parameter;
			if( PROP_DEF == oReader.m_oState->m_oParagraphProp.m_nSpaceMultiLine )
				oReader.m_oState->m_oParagraphProp.m_nSpaceMultiLine = 0;
		}
	}
	COMMAND_RTF_INT( _T("slmult"), oReader.m_oState->m_oParagraphProp.m_nSpaceMultiLine, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("contextualspace"), oReader.m_oState->m_oParagraphProp.m_bContextualSpacing, sCommand, hasParameter, parameter )
	else if( _T("rtlpar") == sCommand )
		oReader.m_oState->m_oParagraphProp.m_bRtl = 1;
	else if( _T("ltrpar") == sCommand )
		oReader.m_oState->m_oParagraphProp.m_bRtl = 0;
	COMMAND_RTF_BOOL( _T("nowwrap"), oReader.m_oState->m_oParagraphProp.m_bNoWordWrap, sCommand, hasParameter, parameter )
	else if( _T("nowwrap") == sCommand )
	{
		if( true == hasParameter && 0 == parameter)
			oReader.m_oState->m_oParagraphProp.m_bSnapToGrid = 1;
		else
			oReader.m_oState->m_oParagraphProp.m_bSnapToGrid = 0;
	}
	else if( _T("ls") == sCommand )
	{
		if( true == hasParameter )
		{
			oReader.m_oState->m_oParagraphProp.m_nListId = parameter;
			if( PROP_DEF == oReader.m_oState->m_oParagraphProp.m_nListLevel )
				oReader.m_oState->m_oParagraphProp.m_nListLevel = 0;
		}
	}
	COMMAND_RTF_INT( _T("ilvl"), oReader.m_oState->m_oParagraphProp.m_nListLevel, sCommand, hasParameter, parameter )

	else if( _T("chbrdr") == sCommand )
		m_eInternalState = is_charBorder;
	else if( _T("brdrt") == sCommand )
		m_eInternalState = is_borderTop;
	else if( _T("brdrb") == sCommand )
		m_eInternalState = is_borderBottom;
	else if( _T("brdrl") == sCommand )
		m_eInternalState = is_borderLeft;
	else if( _T("brdrr") == sCommand )
		m_eInternalState = is_borderRight;
	//else if( _T("brdrbtw") == sCommand )
	//	m_eInternalState = is_borderRight;
	else if( _T("brdrbar") == sCommand )
		m_eInternalState = is_borderBar;
	else if( _T("box") == sCommand )
		m_eInternalState = is_borderBox;
	//Frame
	COMMAND_RTF_INT( _T("absw"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_nWidth, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("absh"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_nHeight, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("phmrg"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eHRef, sCommand, true, RtfFrame::hr_phmrg )
	COMMAND_RTF_INT( _T("phpg"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eHRef, sCommand, true, RtfFrame::hr_phpg )
	COMMAND_RTF_INT( _T("phcol"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eHRef, sCommand, true, RtfFrame::hr_phcol )
	COMMAND_RTF_INT( _T("posx"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_nHPos, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("posnegx"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_nHPos, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("posxc"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eHPos, sCommand, true, RtfFrame::hp_posxc )
	COMMAND_RTF_INT( _T("posxi"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eHPos, sCommand, true, RtfFrame::hp_posxi )
	COMMAND_RTF_INT( _T("posxo"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eHPos, sCommand, true, RtfFrame::hp_posxo )
	COMMAND_RTF_INT( _T("posxl"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eHPos, sCommand, true, RtfFrame::hp_posxl )
	COMMAND_RTF_INT( _T("posxr"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eHPos, sCommand, true, RtfFrame::hp_posxr )
	COMMAND_RTF_INT( _T("pvmrg"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eVRef, sCommand, true, RtfFrame::vr_pvmrg )
	COMMAND_RTF_INT( _T("pvpg"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eVRef, sCommand, true, RtfFrame::vr_pvpg )
	COMMAND_RTF_INT( _T("pvpara"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eVRef, sCommand, true, RtfFrame::vr_pvpara )
	COMMAND_RTF_INT( _T("posy"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_nVPos, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("posnegy"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_nVPos, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("posyt"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eVPos, sCommand, true, RtfFrame::vp_posyt )
	COMMAND_RTF_INT( _T("posyil"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eVPos, sCommand, true, RtfFrame::vp_posyil )
	COMMAND_RTF_INT( _T("posyb"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eVPos, sCommand, true, RtfFrame::vp_posyb )
	COMMAND_RTF_INT( _T("posyc"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eVPos, sCommand, true, RtfFrame::vp_posyc )
	COMMAND_RTF_INT( _T("posyin"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eVPos, sCommand, true, RtfFrame::vp_posyin )
	COMMAND_RTF_INT( _T("posyout"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eVPos, sCommand, true, RtfFrame::vp_posyout )
	COMMAND_RTF_BOOL( _T("abslock"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_bLockAnchor, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("wrapdefault"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eWrap, sCommand, true, RtfFrame::tw_wrapdefault )
	COMMAND_RTF_INT( _T("wraparound"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eWrap, sCommand, true, RtfFrame::tw_wraparound )
	COMMAND_RTF_INT( _T("wraptight"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eWrap, sCommand, true, RtfFrame::tw_wraptight )
	COMMAND_RTF_INT( _T("wrapthrough"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_eWrap, sCommand, true, RtfFrame::tw_wrapthrough )
	COMMAND_RTF_INT( _T("dropcapt"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_DropcapType, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("dropcapli"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_DropcapLines, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("dxfrtext"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_nAllSpace, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("dfrmtxtx"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_nHorSpace, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("dfrmtxty"), oReader.m_oState->m_oParagraphProp.m_oFrame.m_nVerSpace, sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( _T("absnoovrlp"), oReader.m_oState->m_oParagraphProp.m_bOverlap, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("frmtxlrtb"), oReader.m_oState->m_oParagraphProp.m_eTextFollow, sCommand, true, RtfParagraphProperty::tf_frmtxlrtb )
	COMMAND_RTF_INT( _T("frmtxtbrl"), oReader.m_oState->m_oParagraphProp.m_eTextFollow, sCommand, true, RtfParagraphProperty::tf_frmtxtbrl )
	COMMAND_RTF_INT( _T("frmtxbtlr"), oReader.m_oState->m_oParagraphProp.m_eTextFollow, sCommand, true, RtfParagraphProperty::tf_frmtxbtlr )
	COMMAND_RTF_INT( _T("frmtxlrtbv"), oReader.m_oState->m_oParagraphProp.m_eTextFollow, sCommand, true, RtfParagraphProperty::tf_frmtxlrtbv )
	COMMAND_RTF_INT( _T("frmtxtbrlv"), oReader.m_oState->m_oParagraphProp.m_eTextFollow, sCommand, true, RtfParagraphProperty::tf_frmtxtbrlv )
	//RtfTab
	
	COMMAND_RTF_INT( _T("tldot"), m_oCurTab.m_eLeader, sCommand, true, RtfTab::tl_dot )
	COMMAND_RTF_INT( _T("tlmdot"), m_oCurTab.m_eLeader, sCommand, true, RtfTab::tl_mdot )
	COMMAND_RTF_INT( _T("tlhyph"), m_oCurTab.m_eLeader, sCommand, true, RtfTab::tl_hyph )
	COMMAND_RTF_INT( _T("tlul"), m_oCurTab.m_eLeader, sCommand, true, RtfTab::tl_ul )
	COMMAND_RTF_INT( _T("tlth"), m_oCurTab.m_eLeader, sCommand, true, RtfTab::tl_ul )
	COMMAND_RTF_INT( _T("tqr"), m_oCurTab.m_eKind , sCommand, true, RtfTab::tk_tqr )
	COMMAND_RTF_INT( _T("tqc"), m_oCurTab.m_eKind , sCommand, true, RtfTab::tk_tqc )
	COMMAND_RTF_INT( _T("tqdec"), m_oCurTab.m_eKind , sCommand, true, RtfTab::tk_tqdec )
	else if( _T("tb") == sCommand )
	{
		if( true == hasParameter )
		{
			m_oCurTab.m_eKind = RtfTab::tk_tqbar;
			m_oCurTab.m_nTab = parameter;
			oReader.m_oState->m_oParagraphProp.m_oTabs.m_aTabs.push_back( m_oCurTab );
			m_oCurTab.SetDefault();
		}
	}
	else if( _T("tx") == sCommand )
	{
		if( true == hasParameter )
		{
			m_oCurTab.m_nTab = parameter;
			oReader.m_oState->m_oParagraphProp.m_oTabs.m_aTabs.push_back( m_oCurTab );
			m_oCurTab.SetDefault();
		}
	}
	else 
		bContinue = true;

	if( false == bContinue )
		return true;
	bContinue = false;
	//tableStyleProp
	if( _T("*") == sCommand )
		;
	COMMAND_RTF_INT( _T("yts"), oReader.m_oState->m_oParagraphProp.m_nTableStyle, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscfirstrow"), oReader.m_oState->m_oParagraphProp.m_bStyleFirstRow, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscfirstrow"), oReader.m_oState->m_oCellProperty.m_bStyleFirstRow, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscfirstrow"), oReader.m_oState->m_oRowProperty.m_bStyleFirstRow, sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( _T("tsclastrow"), oReader.m_oState->m_oParagraphProp.m_bStyleLastRow, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tsclastrow"), oReader.m_oState->m_oCellProperty.m_bStyleLastRow, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tsclastrow"), oReader.m_oState->m_oRowProperty.m_bStyleLastRow, sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( _T("tscfirstcol"), oReader.m_oState->m_oParagraphProp.m_bStyleFirstCollumn, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscfirstcol"), oReader.m_oState->m_oCellProperty.m_bStyleFirstCol, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscfirstcol"), oReader.m_oState->m_oRowProperty.m_bStyleFirstCol, sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( _T("tsclastcol"), oReader.m_oState->m_oParagraphProp.m_bStyleLastCollumn, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tsclastcol"), oReader.m_oState->m_oCellProperty.m_bStyleLastCol, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tsclastcol"), oReader.m_oState->m_oRowProperty.m_bStyleLastCol, sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( _T("tscbandhorzodd"), oReader.m_oState->m_oParagraphProp.m_bStyleOddRowBand, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscbandhorzodd"), oReader.m_oState->m_oCellProperty.m_bStyleOddRowBand, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscbandhorzodd"), oReader.m_oState->m_oRowProperty.m_bStyleOddRowBand, sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( _T("tscbandhorzeven"), oReader.m_oState->m_oParagraphProp.m_bStyleEvenRowBand, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscbandhorzeven"), oReader.m_oState->m_oCellProperty.m_bStyleEvenRowBand, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscbandhorzeven"), oReader.m_oState->m_oRowProperty.m_bStyleEvenRowBand, sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( _T("tscbandvertodd"), oReader.m_oState->m_oParagraphProp.m_bStyleOddColBand, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscbandvertodd"), oReader.m_oState->m_oCellProperty.m_bStyleOddColBand, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscbandvertodd"), oReader.m_oState->m_oRowProperty.m_bStyleOddColBand, sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( _T("tscbandverteven"), oReader.m_oState->m_oParagraphProp.m_bStyleEvenColBand, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscbandverteven"), oReader.m_oState->m_oCellProperty.m_bStyleEvenColBand, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscbandverteven"), oReader.m_oState->m_oRowProperty.m_bStyleEvenColBand, sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( _T("tscnwcell"), oReader.m_oState->m_oParagraphProp.m_bStyleNWCell, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscnwcell"), oReader.m_oState->m_oCellProperty.m_bStyleNWCell, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscnwcell"), oReader.m_oState->m_oRowProperty.m_bStyleNWCell, sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( _T("tscnecell"), oReader.m_oState->m_oParagraphProp.m_bStyleNECell, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscnecell"), oReader.m_oState->m_oCellProperty.m_bStyleNECell, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscnecell"), oReader.m_oState->m_oRowProperty.m_bStyleNECell, sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( _T("tscswcell"), oReader.m_oState->m_oParagraphProp.m_bStyleSWCell, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscswcell"), oReader.m_oState->m_oCellProperty.m_bStyleSWCell, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscswcell"), oReader.m_oState->m_oRowProperty.m_bStyleSWCell, sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( _T("tscsecell"), oReader.m_oState->m_oParagraphProp.m_bStyleSECell, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscsecell"), oReader.m_oState->m_oCellProperty.m_bStyleSECell, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tscsecell"), oReader.m_oState->m_oRowProperty.m_bStyleSECell, sCommand, hasParameter, parameter )

	else 
		bContinue = true;

	if( false == bContinue )
		return true;
	bContinue = false;
	//CELL Property
	if( _T("*") == sCommand )
		;
	COMMAND_RTF_BOOL( _T("clmgf"), oReader.m_oState->m_oCellProperty.m_bMergeFirst, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("clmrg"), oReader.m_oState->m_oCellProperty.m_bMerge, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("clvmgf"), oReader.m_oState->m_oCellProperty.m_bMergeFirstVertical, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("clvmrg"), oReader.m_oState->m_oCellProperty.m_bMergeVertical, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("clFitText"), oReader.m_oState->m_oCellProperty.m_bFitText, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("clNoWrap"), oReader.m_oState->m_oCellProperty.m_bNoWrap, sCommand, hasParameter, parameter )		

	COMMAND_RTF_INT( _T("clpadfl"), oReader.m_oState->m_oCellProperty.m_nIsPaddingLeft, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clpadl"), oReader.m_oState->m_oCellProperty.m_nPaddingLeft, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clpadft"), oReader.m_oState->m_oCellProperty.m_nIsPaddingTop, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clpadt"), oReader.m_oState->m_oCellProperty.m_nPaddingTop, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clpadfr"), oReader.m_oState->m_oCellProperty.m_nIsPaddingRight, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clpadr"), oReader.m_oState->m_oCellProperty.m_nPaddingRight, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clpadfb"), oReader.m_oState->m_oCellProperty.m_nIsPaddingBottom, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clpadb"), oReader.m_oState->m_oCellProperty.m_nPaddingBottom, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clspfl"), oReader.m_oState->m_oCellProperty.m_nIsSpacingLeft, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clspl"), oReader.m_oState->m_oCellProperty.m_nSpacingLeft, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clspft"), oReader.m_oState->m_oCellProperty.m_nIsSpacingTop, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clspt"), oReader.m_oState->m_oCellProperty.m_nSpacingTop, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clspfr"), oReader.m_oState->m_oCellProperty.m_nIsSpacingRight, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clspr"), oReader.m_oState->m_oCellProperty.m_nSpacingRight, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clspfb"), oReader.m_oState->m_oCellProperty.m_nIsSpacingBottom, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("clspb"), oReader.m_oState->m_oCellProperty.m_nSpacingBottom, sCommand, hasParameter, parameter )
	else if( _T("clftsWidth") == sCommand  )
	{
		if( true == hasParameter )
		{
			switch( parameter )
			{
				case 0:oReader.m_oState->m_oCellProperty.m_eWidthUnits = mu_none;break;
				case 1:oReader.m_oState->m_oCellProperty.m_eWidthUnits = mu_Auto;break;
				case 2:oReader.m_oState->m_oCellProperty.m_eWidthUnits = mu_Percent;break;
				case 3:oReader.m_oState->m_oCellProperty.m_eWidthUnits = mu_Twips;break;
			}
		}
	}
	COMMAND_RTF_INT( _T("clwWidth"), oReader.m_oState->m_oCellProperty.m_nWidth, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("clhidemark"), oReader.m_oState->m_oCellProperty.m_bHideMark, sCommand, hasParameter, parameter )
	else if( _T("cldglu") == sCommand )
		m_eInternalState = is_borderCellLR;
	else if( _T("cldgll") == sCommand )
		m_eInternalState = is_borderCellRL;
	else if( _T("clbrdrl") == sCommand )
		m_eInternalState = is_borderCellLeft;
	else if( _T("clbrdrt") == sCommand )
		m_eInternalState = is_borderCellTop;
	else if( _T("clbrdrr") == sCommand )
		m_eInternalState = is_borderCellRight;
	else if( _T("clbrdrb") == sCommand )
		m_eInternalState = is_borderCellBottom;

	//table Style
	else if( _T("tsbrdrdgl") == sCommand )
		m_eInternalState = is_borderCellLR;
	else if( _T("tsbrdrdgr") == sCommand )
		m_eInternalState = is_borderCellRL;

	COMMAND_RTF_INT( _T("clvertalt"), oReader.m_oState->m_oCellProperty.m_eAlign, sCommand, true, RtfCellProperty::ca_Top )
	COMMAND_RTF_INT( _T("clvertalc"), oReader.m_oState->m_oCellProperty.m_eAlign, sCommand, true, RtfCellProperty::ca_Center )
	COMMAND_RTF_INT( _T("clvertalb"), oReader.m_oState->m_oCellProperty.m_eAlign, sCommand, true, RtfCellProperty::ca_Bottom )
	COMMAND_RTF_INT( _T("cltxlrtb"), oReader.m_oState->m_oCellProperty.m_oCellFlow, sCommand, true, RtfCellProperty::cf_lrtb )
	COMMAND_RTF_INT( _T("cltxtbrl"), oReader.m_oState->m_oCellProperty.m_oCellFlow, sCommand, true, RtfCellProperty::cf_tbrl )
	COMMAND_RTF_INT( _T("cltxbtlr"), oReader.m_oState->m_oCellProperty.m_oCellFlow, sCommand, true, RtfCellProperty::cf_btlr )
	COMMAND_RTF_INT( _T("cltxlrtbv"), oReader.m_oState->m_oCellProperty.m_oCellFlow, sCommand, true, RtfCellProperty::cf_lrtbv )
	COMMAND_RTF_INT( _T("cltxtbrlv"), oReader.m_oState->m_oCellProperty.m_oCellFlow, sCommand, true, RtfCellProperty::cf_tbrlv )
	else if( _T("cellx") == sCommand )
	{
		if( true == hasParameter)
		{
			oReader.m_oState->m_oCellProperty.m_nCellx = parameter;
			oReader.m_oState->m_oRowProperty.AddItem( oReader.m_oState->m_oCellProperty );
			oReader.m_oState->m_oCellProperty.SetDefaultRtf();
		}
	}
	//table style
	COMMAND_RTF_INT( _T("tscellpaddfl"), oReader.m_oState->m_oCellProperty.m_nIsPaddingLeft, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tscellpaddl"), oReader.m_oState->m_oCellProperty.m_nPaddingLeft, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tscellpaddft"), oReader.m_oState->m_oCellProperty.m_nIsPaddingTop, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tscellpaddt"), oReader.m_oState->m_oCellProperty.m_nPaddingTop, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tscellpaddfr"), oReader.m_oState->m_oCellProperty.m_nIsPaddingRight, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tscellpaddr"), oReader.m_oState->m_oCellProperty.m_nPaddingRight, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tscellpaddfb"), oReader.m_oState->m_oCellProperty.m_nIsPaddingBottom, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tscellpaddb"), oReader.m_oState->m_oCellProperty.m_nPaddingBottom, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tsnowrap"), oReader.m_oState->m_oCellProperty.m_bNoWrap, sCommand, hasParameter, parameter )		
	COMMAND_RTF_INT( _T("tsvertalt"), oReader.m_oState->m_oCellProperty.m_eAlign, sCommand, true, RtfCellProperty::ca_Top )
	COMMAND_RTF_INT( _T("tsvertalc"), oReader.m_oState->m_oCellProperty.m_eAlign, sCommand, true, RtfCellProperty::ca_Center )
	COMMAND_RTF_INT( _T("tsvertalb"), oReader.m_oState->m_oCellProperty.m_eAlign, sCommand, true, RtfCellProperty::ca_Bottom )

	else 
		bContinue = true;

	if( false == bContinue )
		return true;
	bContinue = false;
	//ROW Property
	if( _T("*") == sCommand )
		;
	COMMAND_RTF_INT( _T("irow"), oReader.m_oState->m_oRowProperty.m_nIndex, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("irowband"), oReader.m_oState->m_oRowProperty.m_nBandIndex, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("lastrow"), oReader.m_oState->m_oRowProperty.m_bLastRow, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("trhdr"), oReader.m_oState->m_oRowProperty.m_bIsHeader, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("trkeep"), oReader.m_oState->m_oRowProperty.m_bKeep, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("trkeepfollow"), oReader.m_oState->m_oRowProperty.m_bKeep, sCommand, hasParameter, parameter )

	COMMAND_RTF_INT( _T("trql"), oReader.m_oState->m_oRowProperty.m_eJust, sCommand, true, RtfRowProperty::rj_trql )
	COMMAND_RTF_INT( _T("trqr"), oReader.m_oState->m_oRowProperty.m_eJust, sCommand, true, RtfRowProperty::rj_trqr )
	COMMAND_RTF_INT( _T("trqc"), oReader.m_oState->m_oRowProperty.m_eJust, sCommand, true, RtfRowProperty::rj_trqc )

	COMMAND_RTF_INT( _T("trrh"), oReader.m_oState->m_oRowProperty.m_nHeight, sCommand, hasParameter, parameter )

	else if( _T("trftsWidth") == sCommand )
	{
		if( true == hasParameter )
		{
			switch( parameter )
			{
			case 0:oReader.m_oState->m_oRowProperty.m_eMUWidth = mu_none;break;
			case 1:oReader.m_oState->m_oRowProperty.m_eMUWidth = mu_Auto;break;
			case 2:oReader.m_oState->m_oRowProperty.m_eMUWidth = mu_Percent;break;
			case 3:oReader.m_oState->m_oRowProperty.m_eMUWidth = mu_Twips;break;
			}
		}
	}
	COMMAND_RTF_INT( _T("trwWidth"), oReader.m_oState->m_oRowProperty.m_nWidth, sCommand, hasParameter, parameter )
	else if( _T("trftsWidthB") == sCommand )
	{
		if( true == hasParameter )
		{
			switch( parameter )
			{
			case 0:oReader.m_oState->m_oRowProperty.m_eMUStartInvCell = mu_none;break;
			case 1:oReader.m_oState->m_oRowProperty.m_eMUStartInvCell = mu_Auto;break;
			case 2:oReader.m_oState->m_oRowProperty.m_eMUStartInvCell = mu_Percent;break;
			case 3:oReader.m_oState->m_oRowProperty.m_eMUStartInvCell = mu_Twips;break;
			}
		}
	}
	COMMAND_RTF_INT( _T("trwWidthB"), oReader.m_oState->m_oRowProperty.m_nWidthStartInvCell, sCommand, hasParameter, parameter )
	else if( _T("trftsWidthA") == sCommand )
	{
		if( true == hasParameter )
		{
			switch( parameter )
			{
			case 0:oReader.m_oState->m_oRowProperty.m_eMUEndInvCell = mu_none;break;
			case 1:oReader.m_oState->m_oRowProperty.m_eMUEndInvCell = mu_Auto;break;
			case 2:oReader.m_oState->m_oRowProperty.m_eMUEndInvCell = mu_Percent;break;
			case 3:oReader.m_oState->m_oRowProperty.m_eMUEndInvCell = mu_Twips;break;
			}
		}
	}
	COMMAND_RTF_INT( _T("trwWidthA"), oReader.m_oState->m_oRowProperty.m_nWidthEndInvCell, sCommand, hasParameter, parameter )
	else 
		bContinue = true;

	if( false == bContinue )
		return true;
	bContinue = false;
	//TABLE property
	if( _T("*") == sCommand )
		;
	COMMAND_RTF_BOOL( _T("taprtl"), oReader.m_oState->m_oRowProperty.m_bBidi, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trautofit"), oReader.m_oState->m_oRowProperty.m_nAutoFit, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trgaph"), oReader.m_oState->m_oRowProperty.m_nGraph, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tblind"), oReader.m_oState->m_oRowProperty.nTableIndent, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tblindtype"), oReader.m_oState->m_oRowProperty.nTableIndentUnits, sCommand, hasParameter, parameter )

	COMMAND_RTF_INT( _T("tdfrmtxtLeft"), oReader.m_oState->m_oRowProperty.m_nWrapLeft, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tdfrmtxtRight"), oReader.m_oState->m_oRowProperty.m_nWrapRight, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tdfrmtxtTop"), oReader.m_oState->m_oRowProperty.m_nWrapTop, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tdfrmtxtBottom"), oReader.m_oState->m_oRowProperty.m_nWrapBottom, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( _T("tabsnoovrlp"), oReader.m_oState->m_oRowProperty.m_bOverlap, sCommand, hasParameter, parameter )

	COMMAND_RTF_INT( _T("tphmrg"), oReader.m_oState->m_oRowProperty.m_eHRef, sCommand, true, RtfTableProperty::hr_phmrg )
	COMMAND_RTF_INT( _T("tphpg"), oReader.m_oState->m_oRowProperty.m_eHRef, sCommand, true, RtfTableProperty::hr_phpg )
	COMMAND_RTF_INT( _T("tphcol"), oReader.m_oState->m_oRowProperty.m_eHRef, sCommand, true, RtfTableProperty::hr_phcol )
	COMMAND_RTF_INT( _T("tposx"), oReader.m_oState->m_oRowProperty.m_nHPos, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tposnegx"), oReader.m_oState->m_oRowProperty.m_nHPos, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tposxc"), oReader.m_oState->m_oRowProperty.m_eHPos, sCommand, true, RtfTableProperty::hp_posxc )
	COMMAND_RTF_INT( _T("tposxi"), oReader.m_oState->m_oRowProperty.m_eHPos, sCommand, true, RtfTableProperty::hp_posxi )
	COMMAND_RTF_INT( _T("tposxo"), oReader.m_oState->m_oRowProperty.m_eHPos, sCommand, true, RtfTableProperty::hp_posxo )
	COMMAND_RTF_INT( _T("tposxl"), oReader.m_oState->m_oRowProperty.m_eHPos, sCommand, true, RtfTableProperty::hp_posxl )
	COMMAND_RTF_INT( _T("tposxr"), oReader.m_oState->m_oRowProperty.m_eHPos, sCommand, true, RtfTableProperty::hp_posxr )

	COMMAND_RTF_INT( _T("tpvmrg"), oReader.m_oState->m_oRowProperty.m_eVRef, sCommand, true, RtfTableProperty::vr_pvmrg )
	COMMAND_RTF_INT( _T("tpvpg"), oReader.m_oState->m_oRowProperty.m_eVRef, sCommand, true, RtfTableProperty::vr_pvpg )
	COMMAND_RTF_INT( _T("tpvpara"), oReader.m_oState->m_oRowProperty.m_eVRef, sCommand, true, RtfTableProperty::vr_pvpara )
	COMMAND_RTF_INT( _T("tposy"), oReader.m_oState->m_oRowProperty.m_nVPos, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tposnegy"), oReader.m_oState->m_oRowProperty.m_nVPos, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tposyt"), oReader.m_oState->m_oRowProperty.m_eVPos, sCommand, true, RtfTableProperty::vp_posyt )
	COMMAND_RTF_INT( _T("tposyil"), oReader.m_oState->m_oRowProperty.m_eVPos, sCommand, true, RtfTableProperty::vp_posyil )
	COMMAND_RTF_INT( _T("tposyb"), oReader.m_oState->m_oRowProperty.m_eVPos, sCommand, true, RtfTableProperty::vp_posyb )
	COMMAND_RTF_INT( _T("tposyc"), oReader.m_oState->m_oRowProperty.m_eVPos, sCommand, true, RtfTableProperty::vp_posyc )
	COMMAND_RTF_INT( _T("tposyin"), oReader.m_oState->m_oRowProperty.m_eVPos, sCommand, true, RtfTableProperty::vp_posyin )
	COMMAND_RTF_INT( _T("tposyout"), oReader.m_oState->m_oRowProperty.m_eVPos, sCommand, true, RtfTableProperty::vp_posyout )

	else if( _T("trleft") == sCommand )
	{
		if( true == hasParameter )
		{
			oReader.m_oState->m_oRowProperty.m_nLeft = parameter;
			if( PROP_DEF == oReader.m_oState->m_oRowProperty.nTableIndent )
			{
				oReader.m_oState->m_oRowProperty.nTableIndent = parameter;
				oReader.m_oState->m_oRowProperty.nTableIndentUnits = 3;
			}
		}
	}
	COMMAND_RTF_INT( _T("trwWidth"), oReader.m_oState->m_oRowProperty.m_nWidth, sCommand, hasParameter, parameter )
	else if( _T("trleft") == sCommand )
	{
		if( true == hasParameter )
		{
			switch( parameter )
			{
			case 0: oReader.m_oState->m_oRowProperty.m_eMUWidth = mu_none;break;
			case 1: oReader.m_oState->m_oRowProperty.m_eMUWidth = mu_Auto;break;
			case 2: oReader.m_oState->m_oRowProperty.m_eMUWidth = mu_Percent;break;
			case 3: oReader.m_oState->m_oRowProperty.m_eMUWidth = mu_Twips;break;
			}
		}
	}
	COMMAND_RTF_INT( _T("trpaddb"), oReader.m_oState->m_oRowProperty.m_nDefCellMarBottom, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trpaddl"), oReader.m_oState->m_oRowProperty.m_nDefCellMarLeft, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trpaddr"), oReader.m_oState->m_oRowProperty.m_nDefCellMarRight, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trpaddt"), oReader.m_oState->m_oRowProperty.m_nDefCellMarTop, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trpaddfb"), oReader.m_oState->m_oRowProperty.m_nDefCellMarBottomUnits, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trpaddfl"), oReader.m_oState->m_oRowProperty.m_nDefCellMarLeftUnits, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trpaddfr"), oReader.m_oState->m_oRowProperty.m_nDefCellMarRightUnits, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trpaddft"), oReader.m_oState->m_oRowProperty.m_nDefCellMarTopUnits, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trspdb"), oReader.m_oState->m_oRowProperty.m_nDefCellSpBottom, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trspdl"), oReader.m_oState->m_oRowProperty.m_nDefCellSpLeft, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trspdr"), oReader.m_oState->m_oRowProperty.m_nDefCellSpRight, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trspdt"), oReader.m_oState->m_oRowProperty.m_nDefCellSpTop, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trspdfb"), oReader.m_oState->m_oRowProperty.m_nDefCellSpBottomUnits, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trspdfl"), oReader.m_oState->m_oRowProperty.m_nDefCellSpLeftUnits, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trspdfr"), oReader.m_oState->m_oRowProperty.m_nDefCellSpRightUnits, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("trspdft"), oReader.m_oState->m_oRowProperty.m_nDefCellSpTopUnits, sCommand, hasParameter, parameter )

	else if( _T("trbrdrl") == sCommand  )
		m_eInternalState = is_borderRowLeft;
	else if( _T("trbrdrr") == sCommand  )
		m_eInternalState = is_borderRowRight;
	else if( _T("trbrdrt") == sCommand  )
		m_eInternalState = is_borderRowTop;
	else if( _T("trbrdrb") == sCommand  )
		m_eInternalState = is_borderRowBottom;
	else if( _T("trbrdrv") == sCommand  )
		m_eInternalState = is_borderRowVer;
	else if( _T("trbrdrh") == sCommand  )
		m_eInternalState = is_borderRowHor;
	//table Style
	else if( _T("tsbrdrh") == sCommand ) //диагональные из свойств cell
		m_eInternalState = is_borderRowHor;
	else if( _T("tsbrdrv") == sCommand )
		m_eInternalState = is_borderRowVer;
	else if( _T("tsbrdrl") == sCommand )
		m_eInternalState = is_borderRowLeft;
	else if( _T("tsbrdrt") == sCommand )
		m_eInternalState = is_borderRowTop;
	else if( _T("tsbrdrr") == sCommand )
		m_eInternalState = is_borderRowRight;
	else if( _T("tsbrdrb") == sCommand )
		m_eInternalState = is_borderRowBottom;

	COMMAND_RTF_INT( _T("ts"), oReader.m_oState->m_oRowProperty.m_nStyle, sCommand, hasParameter, parameter )

	COMMAND_RTF_INT( _T("tbllkhdrrows"), oReader.m_oState->m_oRowProperty.m_bAutoFirstRow, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tbllklastrow"), oReader.m_oState->m_oRowProperty.m_bAutoLastRow, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tbllkhdrcols"), oReader.m_oState->m_oRowProperty.m_bAutoFirstCol, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tbllklastcol"), oReader.m_oState->m_oRowProperty.m_bAutoLastCol, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tbllknorowband"), oReader.m_oState->m_oRowProperty.m_bAutoNoRowBand, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tbllknocolband"), oReader.m_oState->m_oRowProperty.m_bAutoNoColBand, sCommand, hasParameter, parameter )

	COMMAND_RTF_INT( _T("tscbandsh"), oReader.m_oState->m_oRowProperty.m_nRowBandSize, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( _T("tscbandsv"), oReader.m_oState->m_oRowProperty.m_nColBandSize, sCommand, hasParameter, parameter )

	//Math
	else if( _T("mmath") == sCommand )
	{
		RtfMathPtr oNewMath = RtfMathPtr( new RtfMath() );
		RtfMathReader oMathReader( *oNewMath );
		oAbstrReader.StartSubReader( oMathReader, oDocument, oReader );
		m_oCurParagraph->AddItem( oNewMath );
	}
	else if( _T("shp") == sCommand )
	{
		RtfShapePtr oNewShape( new RtfShape() );
		ShapeReader oShapeReader( *oNewShape );
		oAbstrReader.StartSubReader( oShapeReader, oDocument, oReader );
		if( true == oNewShape->IsValid() )
			m_oCurParagraph->AddItem( oNewShape );
	}
	else if( _T("shppict") == sCommand )
	{
		RtfShapePtr oNewShape( new RtfShape() );
		ShppictReader oShppictReader( *oNewShape );
		oAbstrReader.StartSubReader( oShppictReader, oDocument, oReader );
		if( true == oNewShape->IsValid() )
			m_oCurParagraph->AddItem( oNewShape );
	}
	else if( _T("pict") == sCommand )
	{
		RtfShapePtr oNewShape( new RtfShape() );
		oNewShape->m_nShapeType = 75;
		oNewShape->m_nWrapType = 3; // none
		oNewShape->m_nPositionHRelative = 3;//TCHAR
		oNewShape->m_nPositionVRelative = 3;//line
		oNewShape->m_nPositionH = 0;//absolute
		oNewShape->m_nPositionV = 0;//absolute
		oNewShape->m_oPicture = RtfPicturePtr( new RtfPicture() );
		PictureReader oPictureReader( oReader, *oNewShape);
		oAbstrReader.StartSubReader( oPictureReader, oDocument, oReader );
		if( true == oNewShape->IsValid() )
			m_oCurParagraph->AddItem( oNewShape );
	}
	else if( _T("shpgrp") == sCommand )
	{
		RtfShapeGroupPtr oNewShape( new RtfShapeGroup() );
		ShapeGroupReader oShapeGroupReader( *oNewShape );
		oAbstrReader.StartSubReader( oShapeGroupReader, oDocument, oReader );
		if( true == oNewShape->IsValid() )
			m_oCurParagraph->AddItem( oNewShape );
	}
	else if( _T("nonshppict") == sCommand )
		oAbstrReader.Skip( oDocument, oReader );
	else if( _T("field") == sCommand )
	{
		RtfFieldPtr oNewField = RtfFieldPtr(new RtfField());
		FieldReader oFieldReader( *oNewField );
		oAbstrReader.StartSubReader( oFieldReader, oDocument, oReader );
		if( true == oNewField->IsValid() )
			m_oCurParagraph->AddItem( oNewField );
	}
	else if( _T("bkmkstart") == sCommand )
	{
		RtfBookmarkStartPtr oNewBookmarkStart = RtfBookmarkStartPtr( new RtfBookmarkStart() );
		BookmarkStartReader oBookmarkStartReader( *oNewBookmarkStart );
		oAbstrReader.StartSubReader( oBookmarkStartReader, oDocument, oReader );
		if( true == oNewBookmarkStart->IsValid() )
			m_oCurParagraph->AddItem( oNewBookmarkStart );
	}
	else if( _T("bkmkend") == sCommand )
	{
		RtfBookmarkEndPtr oNewBookmarkEnd = RtfBookmarkEndPtr( new RtfBookmarkEnd() );
		BookmarkEndReader oBookmarkEndReader( *oNewBookmarkEnd );
		oAbstrReader.StartSubReader( oBookmarkEndReader, oDocument, oReader );
		if( true == oNewBookmarkEnd->IsValid() )
			m_oCurParagraph->AddItem( oNewBookmarkEnd );
	}
	else if( _T("footnote") == sCommand )
	{
		RtfFootnotePtr oNewFootnote = RtfFootnotePtr( new RtfFootnote() );
		oNewFootnote->m_oCharProp = oReader.m_oState->m_oCharProp;
		FootnoteReader oFootnoteReader( *oNewFootnote );
		oAbstrReader.StartSubReader( oFootnoteReader, oDocument, oReader );
		if( true == oNewFootnote->IsValid() )
			m_oCurParagraph->AddItem( oNewFootnote );
	}
	else if( _T("chftn") == sCommand )
	{
		if( 1 == oReader.m_nFootnote )
		{
			RtfCharSpecialPtr oNewChar( new RtfCharSpecial() );
			oNewChar->m_oProperty = oReader.m_oState->m_oCharProp;
			oNewChar->m_eType = RtfCharSpecial::rsc_chftn;
			m_oCurParagraph->AddItem( oNewChar );
		}
		else if( 2 == oReader.m_nFootnote )
		{
			RtfCharSpecialPtr oNewChar( new RtfCharSpecial() );
			oNewChar->m_oProperty = oReader.m_oState->m_oCharProp;
			oNewChar->m_eType = RtfCharSpecial::rsc_chftnEnd;
			m_oCurParagraph->AddItem( oNewChar );
		}
	}
	else if( _T("chftnsep") == sCommand || _T("chftnsepc") == sCommand )
	{
		RtfCharSpecialPtr oNewChar( new RtfCharSpecial() );
		oNewChar->m_oProperty = oReader.m_oState->m_oCharProp;
		m_oCurParagraph->AddItem( oNewChar );
		if( _T("chftnsep") == sCommand )
			oNewChar->m_eType = RtfCharSpecial::rsc_chftnsep;
		else if( _T("chftnsepc") == sCommand )
			oNewChar->m_eType = RtfCharSpecial::rsc_chftnsepc;
	}//specialChars
	else if( _T("page") == sCommand  )
	{
		RtfCharSpecialPtr oNewChar = RtfCharSpecialPtr( new RtfCharSpecial() );
		oNewChar->m_eType = RtfCharSpecial::rsc_page;
		oNewChar->m_oProperty = oReader.m_oState->m_oCharProp;
		m_oCurParagraph->AddItem( oNewChar );
//Microsoft (Demo-Hayden-Management-v2).docx 
//после разрыва могут быть и графические элементы .. их надо оставить в ЭТОМ же параграфе
//разрыв страницы со следующего параграфа .. 
		//m_oCurParagraph->m_oProperty = oReader.m_oState->m_oParagraphProp;
		//m_oCurParagraph->m_oOldList = RtfOldListPtr( new RtfOldList() );
		//*m_oCurParagraph->m_oOldList = oReader.m_oState->m_oCurOldList;
		//m_oCurParagraph->m_oProperty.m_oCharProperty = oReader.m_oState->m_oCharProp;
		//AddItem( m_oCurParagraph, oReader, false, false );
		//m_oCurParagraph = RtfParagraphPtr(new RtfParagraph());
	}
	COMMAND_RTF_SPECIAL_CHAR( _T("column"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_column )
	COMMAND_RTF_SPECIAL_CHAR( _T("line"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_line )
	COMMAND_RTF_SPECIAL_CHAR( _T("line"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_line )
	else if( _T("lbr") == sCommand )
	{
		if( true == hasParameter )
		{
			RtfCharSpecialPtr oNewChar = RtfCharSpecialPtr( new RtfCharSpecial() );
			oNewChar->m_nTextWrapBreak = parameter;
			oNewChar->m_oProperty = oReader.m_oState->m_oCharProp;
			m_oCurParagraph->AddItem( oNewChar );
		}
	}
	COMMAND_RTF_SPECIAL_CHAR( _T("softpage"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_softpage )
	COMMAND_RTF_SPECIAL_CHAR( _T("softcol"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_softcol )
	COMMAND_RTF_SPECIAL_CHAR( _T("softline"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_softline )
	else if( _T("softlheight") == sCommand )
	{
		if( true == hasParameter )
		{
			RtfCharSpecialPtr oNewChar = RtfCharSpecialPtr( new RtfCharSpecial() );
			oNewChar->m_nSoftHeight = parameter;
			oNewChar->m_oProperty = oReader.m_oState->m_oCharProp;
			m_oCurParagraph->AddItem( oNewChar );
		}
	}
	COMMAND_RTF_SPECIAL_CHAR( _T("tab"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_tab )
	else if( _T("emdash") == sCommand )
		ExecuteNumberChar( oDocument, oReader, oAbstrReader, 151, 0xD0 ); //\bullet Word for Windows - 151	; Apple Macintosh - 0xD0
	else if( _T("endash") == sCommand )
		ExecuteNumberChar( oDocument, oReader, oAbstrReader, 150, 0xD1 ); //\bullet Word for Windows - 150	; Apple Macintosh - 0xD1
	COMMAND_RTF_SPECIAL_CHAR( _T("emspace"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_emspace )
	COMMAND_RTF_SPECIAL_CHAR( _T("enspace"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_enspace )
	COMMAND_RTF_SPECIAL_CHAR( _T("qmspace"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_qmspace )
	else if( _T("bullet") == sCommand )
		ExecuteNumberChar( oDocument, oReader, oAbstrReader, 149, 0xA5 ); //\bullet Word for Windows - 149	; Apple Macintosh - 0xA5
	else if( _T("lquote") == sCommand )
		ExecuteNumberChar( oDocument, oReader, oAbstrReader, 145, 0xD4 ); //\bullet Word for Windows - 145	; Apple Macintosh - 0xD4
	else if( _T("rquote") == sCommand )
		ExecuteNumberChar( oDocument, oReader, oAbstrReader, 146, 0xD5 ); //\bullet Word for Windows - 146	; Apple Macintosh - 0xD5
	else if( _T("ldblquote") == sCommand )
		ExecuteNumberChar( oDocument, oReader, oAbstrReader, 147, 0xD2 ); //\bullet Word for Windows - 147	; Apple Macintosh - 0xD2
	else if( _T("rdblquote") == sCommand )
		ExecuteNumberChar( oDocument, oReader, oAbstrReader, 148, 0xD3 ); //\bullet Word for Windows - 148	; Apple Macintosh - 0xD3
	COMMAND_RTF_SPECIAL_CHAR( _T("|"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_Formula )
	COMMAND_RTF_SPECIAL_CHAR( _T("~"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_NonBrSpace )
	COMMAND_RTF_SPECIAL_CHAR( _T("-"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_OptHyphen )
	COMMAND_RTF_SPECIAL_CHAR( _T("_"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_NonBrHyphen )
	COMMAND_RTF_SPECIAL_CHAR( _T(":"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_SubEntry )
	COMMAND_RTF_SPECIAL_CHAR( _T("zwbo"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_zwbo )
	COMMAND_RTF_SPECIAL_CHAR( _T("zwnbo"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_zwnbo )
	COMMAND_RTF_SPECIAL_CHAR( _T("zwj"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_zwj )
	COMMAND_RTF_SPECIAL_CHAR( _T("zwnj"), m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_zwnj )
	else if( _T("object") == sCommand )
	{
		RtfOlePtr oNewOleObject = RtfOlePtr( new RtfOle() );
		OleReader oOleReader( *oNewOleObject );
		oAbstrReader.StartSubReader( oOleReader, oDocument, oReader );
		if( true == oNewOleObject->IsValid() )
			m_oCurParagraph->AddItem( oNewOleObject );
	}
	else
	{
		bool bResult = false;


		bResult = RtfShadingCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oParagraphProp.m_oShading );
		if( true == bResult )
			return true;

		if( is_borderBar == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oParagraphProp.m_oBorderBar );
		else if( is_borderBottom == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oParagraphProp.m_oBorderBottom );
		else if( is_borderBox == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oParagraphProp.m_oBorderBox );
		else if( is_borderLeft == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oParagraphProp.m_oBorderLeft );
		else if( is_borderRight == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oParagraphProp.m_oBorderRight );
		else if( is_borderTop == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oParagraphProp.m_oBorderTop );

		if( true == bResult )
			return true;

		if( is_borderCellBottom == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oCellProperty.m_oBorderBottom );
		else if( is_borderCellLeft == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter,oReader.m_oState->m_oCellProperty.m_oBorderLeft );
		else if( is_borderCellRight == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oCellProperty.m_oBorderRight );
		else if( is_borderCellTop == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oCellProperty.m_oBorderTop );
		else if( is_borderCellLR == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oCellProperty.m_oBorderDiagonalLR );
		else if( is_borderCellRL == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oCellProperty.m_oBorderDiagonalRL );

		if( true == bResult )
			return true;

		bResult = RtfShadingCellCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oCellProperty.m_oShading );
		if( true == bResult )
			return true;

		if( is_borderRowBottom == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oRowProperty.m_oBorderBottom );
		else if( is_borderRowHor == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter,oReader.m_oState->m_oRowProperty.m_oBorderHor );
		else if( is_borderRowLeft == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oRowProperty.m_oBorderLeft );
		else if( is_borderRowRight == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oRowProperty.m_oBorderRight );
		else if( is_borderRowTop == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oRowProperty.m_oBorderTop );
		else if( is_borderRowVer == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oRowProperty.m_oBorderVert );

		if( true == bResult )
			return true;

		bResult = RtfShadingRowCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oRowProperty.m_oShading );
		if( true == bResult )
			return true;

		if( is_charBorder == m_eInternalState )
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter,  oReader.m_oState->m_oCharProp.m_poBorder);

		if( true == bResult )
			return true;

		bResult = RtfCharPropCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oCharProp  );
		if( true == bResult )
			return true;

		return false;
	}
	return true;
}

void ParagraphPropDestination::AddItem( RtfParagraphPtr oItem, RtfReader& oReader, bool bEndCell, bool bEndRow )
{
	 // 1 != oItem->m_oProperty.m_bInTable - параграф не в таблице
	 // PROP_DEF != nTargetItap && oItem->m_oProperty.m_nItap <= nTargetItap - выставлено свойство,что вложенность - nTargetItap - это не таблица( Нужно для чтения параграфов в таблицах )
	if( 1 != oItem->m_oProperty.m_bInTable || ( PROP_DEF != nTargetItap && oItem->m_oProperty.m_nItap <= nTargetItap ) )
	{
		if( nCurItap > 0 ) //Если до этого были только параграфы в таблицах - завершаем таблицу
		{
			RtfTablePtr oNewTable( new RtfTable() );
			oNewTable->m_oProperty = oCurRowProperty;

			for( int k = (int)aRows.size() - 1; k >= 0 ; k-- )
			{
				if( aRowItaps[k] == nCurItap )
				{
					oNewTable->InsertItem( aRows[k], 0 );

					aRows.erase(aRows.begin() + k);
					aRowItaps.erase(aRowItaps.begin() + k);
				}
				else
					break;
			}
			//вычисляем свойства для OOX
			oNewTable->CalculateGridProp();
			//удаляем временные единицы
			aCellRenderables.clear();
			aItaps.clear();
			aCells.clear();
			aCellItaps.clear();
			aRows.clear();
			aRowItaps.clear();

			//добавляем таблицу
			m_oTextItems->AddItem( oNewTable );

			if (oReader.m_convertationManager)
			{
                oReader.m_convertationManager->OnCompleteItemRtf();
			}
		}
		//добавляем параграф
		m_oTextItems->AddItem(  oItem );
		if (oReader.m_convertationManager)
		{
			oReader.m_convertationManager->OnCompleteItemRtf();
		}
		//запоминаем nCurItap
		nCurItap = oItem->m_oProperty.m_nItap;
	}
	else
	{
		//если вложенность текущего параграфа меньше - завершаем внутреннюю таблицу
		if( nCurItap > oItem->m_oProperty.m_nItap )
		{
			RtfTablePtr oNewTable( new RtfTable() );
			oNewTable->m_oProperty = oCurRowProperty;
			
			for( int k = (int)aRows.size() - 1; k >= 0 ; k-- )
			{
				if( aRowItaps[k] == nCurItap )
				{
					oNewTable->InsertItem( aRows[k], 0 );
					
					aRows.erase(aRows.begin() + k);
					aRowItaps.erase(aRowItaps.begin() + k);
				}
				else
					break;
			}
			//вычисляем свойства для OOX
			oNewTable->CalculateGridProp();
			//добавляем таблицу
			aCellRenderables.push_back( oNewTable );
			aItaps.push_back( oItem->m_oProperty.m_nItap );
		}
		nCurItap = oItem->m_oProperty.m_nItap;
		//закончилась строка
		if( true == bEndRow )
		{
			RtfTableRowPtr oNewTableRow( new RtfTableRow() );
			oNewTableRow->m_oProperty = oReader.m_oState->m_oRowProperty;

			for( int k = (int)aCells.size() - 1; k >= 0 ; k-- )
			{
				if( aCellItaps[k] == nCurItap )
				{
					oNewTableRow->InsertItem( aCells[k], 0 );
					
					aCells.erase(aCells.begin() + k);
					aCellItaps.erase(aCellItaps.begin() + k);
				}
				else
					break;
			}
			//для каждого cell в row добавляем их свойства
			for( int i = 0; i < (int)oNewTableRow->GetCount() && i < oNewTableRow->m_oProperty.GetCount() ; i++ )
				oNewTableRow->operator [](i)->m_oProperty = oNewTableRow->m_oProperty[i];
			//Добавляем временный row
			aRows.push_back( oNewTableRow );
			aRowItaps.push_back( nCurItap );
		}
		else
		{
			//добавляем параграф во временные cell
			aCellRenderables.push_back( oItem ); //содержит все параграфы, не разложенные по cell
			aItaps.push_back( nCurItap ); //содержит все номера вложенности параграфов

			if( true == bEndCell )
			{
				RtfTableCellPtr oNewTableCell( new RtfTableCell() );
				for( int k = (int)aCellRenderables.size() - 1; k >= 0 ; k-- )
				{
					if( aItaps[k] == nCurItap )
					{
						oNewTableCell->InsertItem(aCellRenderables[k], 0 );
						
						aCellRenderables.erase(aCellRenderables.begin() + k);
						aItaps.erase(aItaps.begin() + k);
					}
					else
						break;
				}
				aCells.push_back( oNewTableCell );
				aCellItaps.push_back( nCurItap );
			}
		}
		oCurRowProperty = oReader.m_oState->m_oRowProperty;
	}
 }

void ParagraphPropDestination::Finalize( RtfReader& oReader/*, RtfSectionPtr pSection*/) 
{
	if( false == m_bPar )// потому что это не reader и нужно как-то загонять последний параграф
	{
		m_oCurParagraph->m_oProperty			= oReader.m_oState->m_oParagraphProp;
		//m_oCurParagraph->m_oProperty.m_pSection = pSection;
		
		m_oCurParagraph->m_oOldList				= RtfOldListPtr( new RtfOldList() );
		*m_oCurParagraph->m_oOldList			= oReader.m_oState->m_oCurOldList;
		
		m_oCurParagraph->m_oProperty.m_oCharProperty = oReader.m_oState->m_oCharProp;

		AddItem( m_oCurParagraph, oReader, false, false );
		m_oCurParagraph = RtfParagraphPtr(new RtfParagraph());
	}
	else
	{
		//if (pSection)
		//	m_oCurParagraph->m_oProperty.m_pSection = pSection;
	}
}

 void ShapeReader::ShapePropertyReader::ShapePropertyValueReader::PopState( RtfDocument& oDocument, RtfReader& oReader )
{
	RtfAbstractReader::PopState( oDocument, oReader );
	if( _T("") == m_sPropValue ) 
		return;
	CString sValue = m_sPropValue;

	if( _T("wzName") == m_sPropName )
	{
		m_oShape.m_sName = sValue;
		return;
	}
	else if( _T("pibName") == m_sPropName )
	{
		return;
	}
	else if( _T("wzDescription") == m_sPropName )
	{
		m_oShape.m_sDescription = sValue;
		return;
	}
	else if( _T("gtextUNICODE") == m_sPropName )
	{
		m_oShape.m_sGtextUNICODE = sValue;
		return;
	}
	else if( _T("gtextFont") == m_sPropName )
	{
		m_oShape.m_sGtextFont = sValue;
		return;
	}
//числовые
	int nValue = 0;

	try
	{
		nValue = Strings::ToInteger( sValue );
	}catch(...)
	{
	}

	if( _T("shapeType") == m_sPropName )
		m_oShape.m_nShapeType = nValue;
//Position absolute
	else if( _T("posh") == m_sPropName )
		m_oShape.m_nPositionH = nValue;
	else if( _T("posrelh") == m_sPropName )
		m_oShape.m_nPositionHRelative = nValue;
	else if( _T("posv") == m_sPropName )
		m_oShape.m_nPositionV = nValue;
	else if( _T("posrelv") == m_sPropName )
		m_oShape.m_nPositionVRelative = nValue;
	else if( _T("fLayoutInCell") == m_sPropName )
		m_oShape.m_bLayoutInCell = nValue;
	else if( _T("fAllowOverlap") == m_sPropName )
		m_oShape.m_bAllowOverlap = nValue;
//Position relative
	else if( _T("pctHorizPos") == m_sPropName )
		m_oShape.m_nPositionHPct = nValue;
	else if( _T("pctVertPos") == m_sPropName )
		m_oShape.m_nPositionVPct = nValue;
	else if( _T("pctHoriz") == m_sPropName )
		m_oShape.m_nPctWidth = nValue;
	else if( _T("pctVert") == m_sPropName )
		m_oShape.m_nPctHeight = nValue;
	else if( _T("sizerelh") == m_sPropName )
		m_oShape.m_nPctWidthRelative = nValue;
	else if( _T("sizerelv") == m_sPropName )
		m_oShape.m_nPctHeightRelative = nValue;
	else if( _T("colStart") == m_sPropName )
		m_oShape.m_nColStart = nValue;
	//Rehydration
	else if( _T("metroBlob") == m_sPropName )
		m_oShape.m_sMetroBlob = sValue;
	//Object Type
	else if( _T("fIsBullet") == m_sPropName )
		m_oShape.m_bIsBullet = nValue;
	else if( _T("rotation") == m_sPropName )
		m_oShape.m_nRotation = nValue;
	else if( _T("fFlipV") == m_sPropName )
		m_oShape.m_bFlipV = nValue;
	else if( _T("fFlipH") == m_sPropName )
		m_oShape.m_bFlipH = nValue;
	else if( _T("shapeType") == m_sPropName )
		m_oShape.m_nShapeType = nValue;
	else if( _T("pWrapPolygonVertices") == m_sPropName )
	{
		CString sWrapPair;
		int nPosition = 0;
		int nCount=0;
		while(true)
		{
			sWrapPair = sValue.Tokenize(_T(";"),nPosition);
			if( _T("") == sWrapPair )
				break;
			nCount++;
			if( nCount > 2 )
			{
				int nSubPos = 0;
				CString sWrapPoint = sWrapPair.Tokenize( _T(","), nSubPos);
				sWrapPoint.Remove('(');
				int nWrapPoint = Strings::ToInteger(sWrapPoint);
				m_oShape.m_aWrapPoints.push_back( nWrapPoint );
				sWrapPoint = sWrapPair.Tokenize( _T(","), nSubPos);
				sWrapPoint.Remove(')');
                 try
                {
                    nWrapPoint = Strings::ToInteger(sWrapPoint);
                    //(-90,-() - file_1_(1).rtf
                }
                catch(...)
                {
                    nWrapPoint = 0;
                }

				m_oShape.m_aWrapPoints.push_back( nWrapPoint );
			}
		}

	}
	else if( _T("dxWrapDistLeft") == m_sPropName )
		m_oShape.m_nWrapDistLeft =  RtfUtility::Emu2Twips( nValue );
	else if( _T("dyWrapDistTop") == m_sPropName )
		m_oShape.m_nWrapDistTop = RtfUtility::Emu2Twips( nValue );
	else if( _T("dxWrapDistRight") == m_sPropName )
		m_oShape.m_nWrapDistRight = RtfUtility::Emu2Twips( nValue );
	else if( _T("dyWrapDistBottom") == m_sPropName )
		m_oShape.m_nWrapDistBottom = RtfUtility::Emu2Twips( nValue );
	else if( _T("fBehindDocument") == m_sPropName )
		m_oShape.m_nZOrderRelative = nValue;
	else if( _T("fHidden") == m_sPropName )
		m_oShape.m_bHidden = nValue;
//Text box
	else if( _T("dxTextLeft") == m_sPropName )
		m_oShape.m_nTexpLeft = nValue;
	else if( _T("dyTextTop") == m_sPropName )
		m_oShape.m_nTexpTop = nValue;
	else if( _T("dxTextRight") == m_sPropName )
		m_oShape.m_nTexpRight = nValue;
	else if( _T("dyTextBottom") == m_sPropName )
		m_oShape.m_nTexpBottom = nValue;
	else if( _T("anchorText") == m_sPropName )
		m_oShape.m_nAnchorText = nValue;

//Geometry
	else if( _T("adjustValue") == m_sPropName )
		m_oShape.m_nAdjustValue = nValue;
	else if( _T("adjust2Value") == m_sPropName )
		m_oShape.m_nAdjustValue2 = nValue;
	else if( _T("adjust3Value") == m_sPropName )
		m_oShape.m_nAdjustValue3 = nValue;
	else if( _T("adjust4Value") == m_sPropName )
		m_oShape.m_nAdjustValue4 = nValue;
	else if( _T("adjust5Value") == m_sPropName )
		m_oShape.m_nAdjustValue5 = nValue;
	else if( _T("adjust6Value") == m_sPropName )
		m_oShape.m_nAdjustValue6 = nValue;
	else if( _T("adjust7Value") == m_sPropName )
		m_oShape.m_nAdjustValue7 = nValue;
	else if( _T("adjust8Value") == m_sPropName )
		m_oShape.m_nAdjustValue8 = nValue;
	else if( _T("adjust9Value") == m_sPropName )
		m_oShape.m_nAdjustValue9 = nValue;
	else if( _T("adjust10Value") == m_sPropName )
		m_oShape.m_nAdjustValue10 = nValue;
//WordArt Effects
	else if( _T("cropFromTop") == m_sPropName )
		m_oShape.m_nCropFromTop = nValue;
	else if( _T("cropFromBottom") == m_sPropName )
		m_oShape.m_nCropFromBottom = nValue;
	else if( _T("cropFromLeft") == m_sPropName )
		m_oShape.m_nCropFromRight = nValue;
	else if( _T("cropFromRight") == m_sPropName )
		m_oShape.m_nCropFromTop = nValue;
//Grouped Shapes
	else if( _T("groupBottom") == m_sPropName )
		m_oShape.m_nGroupBottom = nValue;
	else if( _T("groupLeft") == m_sPropName )
		m_oShape.m_nGroupLeft = nValue;
	else if( _T("groupRight") == m_sPropName )
		m_oShape.m_nGroupRight = nValue;
	else if( _T("groupTop") == m_sPropName )
		m_oShape.m_nGroupTop = nValue;
	else if( _T("relBottom") == m_sPropName )
		m_oShape.m_nRelBottom = nValue;
	else if( _T("relLeft") == m_sPropName )
		m_oShape.m_nRelLeft = nValue;
	else if( _T("relRight") == m_sPropName )
		m_oShape.m_nRelRight = nValue;
	else if( _T("relTop") == m_sPropName )
		m_oShape.m_nRelTop = nValue;
	else if( _T("relRotation") == m_sPropName )
		m_oShape.m_nRelRotation = nValue;
	else if( _T("dhgt") == m_sPropName )
		m_oShape.m_nRelZOrder = nValue;
//Fill
	else if( _T("fFilled") == m_sPropName )
		m_oShape.m_bFilled = (0 == nValue ? false : true );
	else if( _T("fillType") == m_sPropName )
		m_oShape.m_nFillType = nValue;
	else if( _T("fillColor") == m_sPropName )
		m_oShape.m_nFillColor = nValue;
	else if( _T("fillBackColor") == m_sPropName )
		m_oShape.m_nFillColor2 = nValue;
	else if( _T("fillOpacity") == m_sPropName )
		m_oShape.m_nFillOpacity = nValue * 100 / 65536;
	else if( _T("fillAngle") == m_sPropName )
		m_oShape.m_nFillAngle = nValue / 65536;
	else if( _T("fillFocus") == m_sPropName )
		m_oShape.m_nFillFocus = nValue;

	else if( _T("fGtext") == m_sPropName )
		m_oShape.m_bGtext = nValue;
	else if( _T("gtextSize") == m_sPropName )
		m_oShape.m_nGtextSize = nValue;

//Line
	else if( _T("fLine") == m_sPropName )
		m_oShape.m_bLine = ( 0 == nValue ? false : true );
	else if( _T("lineStartArrowhead") == m_sPropName )
		m_oShape.m_nLineStartArrow = nValue;
	else if( _T("lineColor") == m_sPropName )
		m_oShape.m_nLineColor = nValue;
	else if( _T("lineStartArrowWidth") == m_sPropName )
		m_oShape.m_nLineStartArrowWidth = nValue;
	else if( _T("lineStartArrowLength") == m_sPropName )
		m_oShape.m_nLineStartArrowLength = nValue;
	else if( _T("lineEndArrowhead") == m_sPropName )
		m_oShape.m_nLineEndArrow = nValue;
	else if( _T("lineEndArrowWidth") == m_sPropName )
		m_oShape.m_nLineEndArrowWidth = nValue;
	else if( _T("lineEndArrowLength") == m_sPropName )
		m_oShape.m_nLineEndArrowLength = nValue;
	else if( _T("lineWidth") == m_sPropName )
		m_oShape.m_nLineWidth = nValue;
	else if( _T("lineDashing") == m_sPropName )
		m_oShape.m_nLineDashing = nValue;
	else
	{
		std::wstring name	= m_sPropName.GetBuffer();
		int val				= nValue;
	}
}
