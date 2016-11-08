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
#include "../../../ASCOfficePPTXFile/Editor/Drawing/Enums.h"

#include <boost/algorithm/string.hpp>

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
	if		( L"ansi"	== sCommand )	oDocument.m_oProperty.m_eCodePage = RtfDocumentProperty::cp_ansi;
	else if ( L"mac"	== sCommand )	oDocument.m_oProperty.m_eCodePage = RtfDocumentProperty::cp_mac;
	else if ( L"pc"		== sCommand )	oDocument.m_oProperty.m_eCodePage = RtfDocumentProperty::cp_pc;
	else if ( L"pca"	== sCommand )	oDocument.m_oProperty.m_eCodePage = RtfDocumentProperty::cp_pca;
	else if ( L"ansicpg"== sCommand )
	{
		oDocument.m_oProperty.m_eCodePage = RtfDocumentProperty::cp_ansi;

        if ( hasParameter )
        {
			oDocument.m_oProperty.m_nAnsiCodePage = parameter;
        }
	}
	else if ( L"deff" == sCommand )
	{
		if ( hasParameter )
		{
			oDocument.m_oProperty.m_nDeffFont = parameter;
			oReader.m_nDefFont = parameter;
		}
	}
	COMMAND_RTF_INT	( L"themelang",		oDocument.m_oProperty.m_nThemelang,		sCommand, hasParameter, parameter )
	COMMAND_RTF_INT	( L"themelangfe",	oDocument.m_oProperty.m_nThemelangfe,	sCommand, hasParameter, parameter )
	COMMAND_RTF_INT	( L"themelangcs",	oDocument.m_oProperty.m_nThemelangcs,	sCommand, hasParameter, parameter )
	
	COMMAND_RTF_BOOL( L"rtlgutter",		oDocument.m_oProperty.m_bRtlGutter, sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( L"rtldoc",			oDocument.m_oProperty.m_bRtl,		sCommand, hasParameter, parameter)

	COMMAND_RTF_BOOL( L"hyphcaps",	oDocument.m_oProperty.m_bHypCaps,	sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"hyphauto",	oDocument.m_oProperty.m_bAutoHyp,	sCommand, hasParameter, parameter )
	COMMAND_RTF_INT	( L"hyphconsec",	oDocument.m_oProperty.m_nMaxHypen,	sCommand, hasParameter, parameter )
	COMMAND_RTF_INT	( L"hyphhotz",	oDocument.m_oProperty.m_nHyphenationRight, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT	( L"deftab",		oDocument.m_oProperty.m_nTabWidth,	sCommand, hasParameter, parameter )

//Page Borders
	COMMAND_RTF_BOOL( L"pgbrdrhead", oDocument.m_oProperty.m_bDorderSurroundHeader, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"pgbrdrfoot", oDocument.m_oProperty.m_bDorderSurroundFotter, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"pgbrdrsnap", oDocument.m_oProperty.m_bAlignBordersAndEdges, sCommand, hasParameter, parameter )

//Page Information
	else if ( L"paperw" == sCommand )
	{
		if ( hasParameter )
		{
			oDocument.m_oProperty.m_nPaperWidth		= parameter;
			oReader.m_oCurSectionProp.m_nPageWidth	= parameter;
		}
	}
	else if ( L"paperh" == sCommand )
	{
		if ( hasParameter )
		{
			oDocument.m_oProperty.m_nPaperHeight	= parameter;
			oReader.m_oCurSectionProp.m_nPageHeight = parameter;
		}
	}
	else if ( L"margl" == sCommand )
	{
		if ( hasParameter )
		{
			oDocument.m_oProperty.m_nMarginLeft		= parameter;
			oReader.m_oCurSectionProp.m_nMarginLeft = parameter;
		}
	}
	else if ( L"margr" == sCommand )
	{
		if ( hasParameter )
		{
			oDocument.m_oProperty.m_nMarginRight		= parameter;
			oReader.m_oCurSectionProp.m_nMarginRight	= parameter;
		}
	}
	else if ( L"margt" == sCommand )
	{
		if ( hasParameter )
		{
			oDocument.m_oProperty.m_nMarginTop		= parameter;
			oReader.m_oCurSectionProp.m_nMarginTop	= parameter;
		}
	}
	else if ( L"margb" == sCommand )
	{
		if ( hasParameter )
		{
			oDocument.m_oProperty.m_nMarginBottom		= parameter;
			oReader.m_oCurSectionProp.m_nMarginBottom	= parameter;
		}
	}
	else if ( L"facingp" == sCommand )
	{
		if ( hasParameter && 0 == parameter )
		{
			oDocument.m_oProperty.m_bFacingPage			= 0;
			oReader.m_oCurSectionProp.m_bSwitchMargin	= 0;
		}
		else
		{
			oDocument.m_oProperty.m_bFacingPage			= 1;
			oReader.m_oCurSectionProp.m_bSwitchMargin	= 1;
		}
	}
	else if ( L"gutter" == sCommand )
	{
		if ( hasParameter )
		{
			oDocument.m_oProperty.m_nGutterWidth = parameter;
			oReader.m_oCurSectionProp.m_nGutterMarginWidth = parameter;
		}
	}
	else if ( L"ogutter" == sCommand )
	{
		if ( hasParameter )
			oDocument.m_oProperty.m_nGutterWidthOutside = parameter;
	}
	else if ( L"margmirror" == sCommand )
	{
		if ( hasParameter && 0 == parameter )
		{
			oDocument.m_oProperty.m_bSwitchMargins			= 0;
			oReader.m_oCurSectionProp.m_bLandscapeFormat	= 0;
		}
		else
		{
			oDocument.m_oProperty.m_bSwitchMargins			= 1;
			oReader.m_oCurSectionProp.m_bLandscapeFormat	= 1;
		}
	}
	else if ( L"landscape" == sCommand )
	{
		if ( hasParameter && 0 == parameter )
			oDocument.m_oProperty.m_bLandScape = 0;
		else
			oDocument.m_oProperty.m_bLandScape = 1;
	}
	COMMAND_RTF_BOOL( L"gutterprl",		oDocument.m_oProperty.m_bGutterAtTop,		sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"viewbksp",		oDocument.m_oProperty.m_nDisplayBackground, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"htmautsp",		oDocument.m_oProperty.m_bHtmlAutoSpace,		sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"useltbaln",		oDocument.m_oProperty.m_bUseTabAlignment,	sCommand, hasParameter, parameter )
	//Footnotes and Endnotes
	COMMAND_RTF_INT	( L"endnotes",		oDocument.m_oProperty.m_eFootnotePlacement, sCommand, true, RtfDocumentProperty::fp_EndSection )
	COMMAND_RTF_INT	( L"enddoc",		oDocument.m_oProperty.m_eFootnotePlacement, sCommand, true, RtfDocumentProperty::fp_EndDocument )
	COMMAND_RTF_INT	( L"ftntj",			oDocument.m_oProperty.m_eFootnotePlacement, sCommand, true, RtfDocumentProperty::fp_BeneathText )
	COMMAND_RTF_INT	( L"ftnbj",			oDocument.m_oProperty.m_eFootnotePlacement, sCommand, true, RtfDocumentProperty::fp_BottomPage )

	COMMAND_RTF_INT( L"aendnotes",		oDocument.m_oProperty.m_eEndnotePlacement,	sCommand, true, RtfDocumentProperty::ep_EndSection )
	COMMAND_RTF_INT( L"aenddoc",		oDocument.m_oProperty.m_eEndnotePlacement,	sCommand, true, RtfDocumentProperty::ep_EndDocument )
	COMMAND_RTF_INT( L"aftnbj",			oDocument.m_oProperty.m_eEndnotePlacement,	sCommand, true, RtfDocumentProperty::ep_BeneathText )
	COMMAND_RTF_INT( L"aftntj",			oDocument.m_oProperty.m_eEndnotePlacement,	sCommand, true, RtfDocumentProperty::ep_BottomPage )

	COMMAND_RTF_INT( L"ftnstart",		oDocument.m_oProperty.m_nFootnoteStart,		sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"aftnstart",		oDocument.m_oProperty.m_nEndnoteStart,		sCommand, hasParameter, parameter )

	COMMAND_RTF_INT( L"ftnrstpg",		oDocument.m_oProperty.m_eFootnoteRestart,	sCommand, true, RtfDocumentProperty::fr_EachPage )
	COMMAND_RTF_INT( L"ftnrestart",		oDocument.m_oProperty.m_eFootnoteRestart,	sCommand, true, RtfDocumentProperty::fr_EachSection )
	COMMAND_RTF_INT( L"ftnrstcont",		oDocument.m_oProperty.m_eFootnoteRestart,	sCommand, true, RtfDocumentProperty::fr_Continuous )
	COMMAND_RTF_INT( L"aftnrestart",	oDocument.m_oProperty.m_eEndnoteRestart,	sCommand, true, RtfDocumentProperty::er_EachSection )
	COMMAND_RTF_INT( L"aftnrstcont",	oDocument.m_oProperty.m_eEndnoteRestart,	sCommand, true, RtfDocumentProperty::er_Continuous )

	COMMAND_RTF_INT( L"ftnnar",			oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 0 )
	COMMAND_RTF_INT( L"ftnnalc",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 4 )
	COMMAND_RTF_INT( L"ftnnauc",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 3 )
	COMMAND_RTF_INT( L"ftnnrlc",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 2 )
	COMMAND_RTF_INT( L"ftnnruc",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 1 )
	COMMAND_RTF_INT( L"ftnnchi",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 70 )
	COMMAND_RTF_INT( L"ftnnchi",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 25 )
	COMMAND_RTF_INT( L"ftnncnum",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 18 )
	COMMAND_RTF_INT( L"ftnndbnum",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 10 )
	COMMAND_RTF_INT( L"ftnndbnumd",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 11 )
	COMMAND_RTF_INT( L"ftnndbnumt",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 16 )
	COMMAND_RTF_INT( L"ftnndbnumk",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 17 )
	COMMAND_RTF_INT( L"ftnndbar",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 20 )
	COMMAND_RTF_INT( L"ftnnganada",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 24 )
	COMMAND_RTF_INT( L"ftnngbnum",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 26 )
	COMMAND_RTF_INT( L"ftnngbnumd",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 27 )
	COMMAND_RTF_INT( L"ftnngbnuml",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 28 )
	COMMAND_RTF_INT( L"ftnngbnumk",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 29 )
	COMMAND_RTF_INT( L"ftnnzodiac",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 30 )
	COMMAND_RTF_INT( L"ftnnzodiacd",	oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 31 )
	COMMAND_RTF_INT( L"ftnnzodiacl",	oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 32 )

	COMMAND_RTF_INT( L"aftnnar",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 0 )
	COMMAND_RTF_INT( L"aftnnalc",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 4 )
	COMMAND_RTF_INT( L"aftnnauc",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 3 )
	COMMAND_RTF_INT( L"aftnnrlc",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 2 )
	COMMAND_RTF_INT( L"aftnnruc",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 1 )
	COMMAND_RTF_INT( L"aftnnchi",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 70 )
	COMMAND_RTF_INT( L"aftnnchi",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 25 )
	COMMAND_RTF_INT( L"aftnncnum",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 18 )
	COMMAND_RTF_INT( L"aftnndbnum",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 10 )
	COMMAND_RTF_INT( L"aftnndbnumd",	oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 11 )
	COMMAND_RTF_INT( L"aftnndbnumt",	oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 16 )
	COMMAND_RTF_INT( L"aftnndbnumk",	oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 17 )
	COMMAND_RTF_INT( L"aftnndbar",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 20 )
	COMMAND_RTF_INT( L"aftnnganada",	oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 24 )
	COMMAND_RTF_INT( L"aftnngbnum",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 26 )
	COMMAND_RTF_INT( L"aftnngbnumd",	oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 27 )
	COMMAND_RTF_INT( L"aftnngbnuml",	oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 28 )
	COMMAND_RTF_INT( L"aftnngbnumk",	oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 29 )
	COMMAND_RTF_INT( L"aftnnzodiac",	oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 30 )
	COMMAND_RTF_INT( L"aftnnzodiacd",	oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 31 )
	COMMAND_RTF_INT( L"aftnnzodiacl",	oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 32 )
	else
		return false;
	return true;
}


bool RtfNormalReader::ExecuteCommand( RtfDocument& oDocument, RtfReader& oReader, CString sCommand, bool hasParameter, int parameter )
 {
	if ( L"colortbl" == sCommand )
	{
		RtfColorTableReader oColorTableReader;
		return StartSubReader( oColorTableReader, oDocument, oReader );
	}
	else if ( L"fonttbl" == sCommand )
	{
		RtfFontTableReader oFontTableReader;
		return StartSubReader( oFontTableReader, oDocument, oReader );
	}
	else if ( L"stylesheet" == sCommand )
	{
		StyleTableReader oStyleReader;
		return StartSubReader( oStyleReader, oDocument, oReader );
	}
	//else if ( L"latentstyles" == sCommand )
	//{
	//	LatentstyleTableReader oLatentstyleReader;
	//	return StartSubReader( oLatentstyleReader, oDocument, oReader );
	//}
	else if ( L"listtable" == sCommand )
	{
		ListTableReader oListTableReader;
		return StartSubReader( oListTableReader, oDocument, oReader );

	}
	else if ( L"listoverridetable" == sCommand )
	{
		ListOverrideTableReader oListOverrideReader;
		return StartSubReader( oListOverrideReader, oDocument, oReader );
	}
	else if ( L"revtbl" == sCommand )
	{
		RtfRevisionTableReader oRevisionReader;
		return StartSubReader( oRevisionReader, oDocument, oReader );
	}
	else if ( L"rsidtable" == sCommand )
	{
		//RSIDTableReader oRSIDReader;
		//return StartSubReader( oRSIDReader, oDocument, oReader );
	}
	else if ( L"info" == sCommand )
	{
		RtfInfoReader oInfoReader;
		return StartSubReader( oInfoReader, oDocument, oReader );			
	}
	//else if ( L"colorschememapping" == sCommand )
	//{
	//	RtfColorSchemeReader oSchemeReader;
	//	return StartSubReader( oSchemeReader, oDocument, oReader );		
	//}
	//else if ( L"themedata" == sCommand )
	//{
	//	RtfThemeDataReader oThemeDataReader;
	//	return StartSubReader( oThemeDataReader, oDocument, oReader );	
	//}
	//else if ( L"defchp" == sCommand )
	//{
	//	RtfDefCharPropReader oDefCharPropReader( oDocument.m_oDefaultCharProp );
	//	return StartSubReader( oDefCharPropReader, oDocument, oReader );				}
	//else if ( L"defpap" == sCommand )
	//{
	//	RtfDefParPropReader oDefParPropReader;
	//	return StartSubReader( oDefParPropReader, oDocument, oReader );
	//}
	else if ( L"mmathPr" == sCommand )
	{
		RtfMathReader oMathPropReader(oDocument.m_oMathProp);
		return StartSubReader( oMathPropReader, oDocument, oReader );
	}
    else if ( L"ftnsep" == sCommand || L"ftnsepc" == sCommand ||
            L"aftnsep"== sCommand || L"aftnsepc" == sCommand )
	{
		ParagraphReader oParagraphReader(sCommand, oReader);
		StartSubReader( oParagraphReader, oDocument, oReader );
		if ( L"ftnsep" == sCommand )
		{
			oDocument.m_oFootnoteSep = oParagraphReader.m_oParPropDest.m_oTextItems;
			oDocument.m_oProperty.m_aSpecialFootnotes.push_back( 0 );
		}
		else if ( L"ftnsepc" == sCommand )
		{
			oDocument.m_oFootnoteCon = oParagraphReader.m_oParPropDest.m_oTextItems;
			oDocument.m_oProperty.m_aSpecialFootnotes.push_back( 1 );
		}
		else if ( L"aftnsep" == sCommand )
		{
			oDocument.m_oEndnoteSep = oParagraphReader.m_oParPropDest.m_oTextItems;
			oDocument.m_oProperty.m_aSpecialEndnotes.push_back( 0 );
		}
		else if ( L"aftnsepc" == sCommand )
		{
			oDocument.m_oEndnoteCon = oParagraphReader.m_oParPropDest.m_oTextItems;
			oDocument.m_oProperty.m_aSpecialEndnotes.push_back( 1 );
		}
	}
	else if ( L"sect" == sCommand )
	{
		oParagraphReaderDestination.Finalize( oReader );

		_section section;
		if (oDocument.GetItem( section ) )
		{
			section.props->m_bFinalize = true;
			section.props->m_oProperty = oReader.m_oCurSectionProp;
		}
					
		RtfSectionPtr oNewSection = RtfSectionPtr( new RtfSection() );
		
		_section new_section(oNewSection);
		oDocument.AddItem( new_section );
		
		oParagraphReaderDestination.m_oTextItems	= oNewSection;

		//вручную обнуляем footer, т.к. sectd может встретиться и после field
		///?????
		oReader.m_oCurSectionProp.m_oHeaderLeft		= TextItemContainerPtr();
		oReader.m_oCurSectionProp.m_oHeaderFirst	= TextItemContainerPtr();
		oReader.m_oCurSectionProp.m_oHeaderRight	= TextItemContainerPtr();
		oReader.m_oCurSectionProp.m_oFooterLeft		= TextItemContainerPtr();
		oReader.m_oCurSectionProp.m_oFooterFirst	= TextItemContainerPtr();
		oReader.m_oCurSectionProp.m_oFooterRight	= TextItemContainerPtr();
	}
	else if (  L"sectd" == sCommand  )
	{
		//default set to curent section
		SectDef( oDocument, oReader );
	}
	else
	{
		bool bResult = false;
		bResult = oParagraphReaderDestination.ExecuteCommand( oDocument, oReader, (*this), sCommand, hasParameter, parameter );
		if ( bResult )
			return true;
		bResult = RtfDocumentCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter );
		if ( bResult )
			return true;
		bResult = oRtfSectionCommand.ExecuteCommand( oDocument, oReader, (*this),sCommand, hasParameter, parameter );
		if ( bResult )
			return true;
		bResult = RtfDocumentCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter );
		if ( bResult )
			return true;
		return false;
	}
	return true;
 }
bool RtfBorderCommand::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter,RtfBorder& oOutput)
{
		 if( L"brdrs"			== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrs;
	else if( L"brdrth"			== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrth;
	else if( L"brdrsh"			== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrsh;
	else if( L"brdrdb"			== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrdb;
	else if( L"brdrdot"			== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrdot;
	else if( L"brdrdash"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrdash;
	else if( L"brdrhair"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrhair;
	else if( L"brdrdashsm"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrdashsm;
	else if( L"brdrdashd"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrdashd;
	else if( L"brdrdashdd"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrdashdd;
	else if( L"brdrdashdot"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrdashdot;
	else if( L"brdrdashdotdot"	== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrdashdot;
	else if( L"brdrtriple"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrtriple;
	else if( L"brdrtnthsg"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrtnthsg;
	else if( L"brdrthtnsg"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrthtnsg;
	else if( L"brdrtnthtnsg"	== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrtnthtnsg;
	else if( L"brdrtnthmg"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrtnthmg;
	else if( L"brdrthtnmg"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrthtnmg;
	else if( L"brdrtnthtnmg"	== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrtnthtnmg;
	else if( L"brdrtnthlg"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrtnthlg;
	else if( L"brdrthtnlg"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrthtnlg;
	else if( L"brdrtnthtnlg"	== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrtnthtnlg;
	else if( L"brdrwavy"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrwavy;
	else if( L"brdrwavydb"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrwavydb;
	else if( L"brdrdashdotstr"	== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrdashdotstr;
	else if( L"brdremboss"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdremboss;
	else if( L"brdrengrave"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrengrave;
	else if( L"brdroutset"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdroutset;
	else if( L"brdrinset"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrinset;
	else if( L"brdrnone"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrnone;
	else if( L"brdrw"			== sCommand )
	{
		if( true == hasParameter )
		{
			oOutput.m_nWidth = parameter;
			if( RtfBorder::bt_none == oOutput.m_eType )
				oOutput.m_eType = RtfBorder::bt_brdrs;
		}
	}
	else if( L"brsp" == sCommand )
	{
		if( true == hasParameter )
		{
			oOutput.m_nSpace = parameter;
			if( RtfBorder::bt_none == oOutput.m_eType )
				oOutput.m_eType = RtfBorder::bt_brdrs;
		}
	}
	else if( L"brdrcf" == sCommand )
	{
		if( true == hasParameter )
		{
			oOutput.m_nColor = parameter;
			if( RtfBorder::bt_none == oOutput.m_eType )
				oOutput.m_eType = RtfBorder::bt_brdrs;
		}
	}
	else
		return false;
	return true;
}
bool RtfSectionCommand::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, RtfAbstractReader& oAbstrReader, CString sCommand, bool hasParameter, int parameter)
{
	if ( L"11111111" == sCommand )
		;
	COMMAND_RTF_BOOL( L"rtlsect",	oReader.m_oCurSectionProp.m_bBidi,				sCommand, hasParameter, parameter)
	COMMAND_RTF_INT	( L"binfsxn",	oReader.m_oCurSectionProp.m_nPaperSourceFirst,	sCommand, hasParameter, parameter)
	COMMAND_RTF_INT	( L"binsxn",	oReader.m_oCurSectionProp.m_nPaperSourceOther,	sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( L"rtlgutter",	oReader.m_oCurSectionProp.m_bRtlGutter,			sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( L"endnhere",	oReader.m_oCurSectionProp.m_bEndnotes,			sCommand, hasParameter, parameter)
	COMMAND_RTF_INT	( L"ds",		oReader.m_oCurSectionProp.m_nStyle,				sCommand, hasParameter, parameter)
	COMMAND_RTF_INT	( L"sbknone",	oReader.m_oCurSectionProp.m_eSectionBreak,		sCommand, true, RtfSectionProperty::sb_sbknone)
	COMMAND_RTF_INT	( L"sbkcol",	oReader.m_oCurSectionProp.m_eSectionBreak,		sCommand, true, RtfSectionProperty::sb_sbkcol)
	COMMAND_RTF_INT	( L"sbkpage",	oReader.m_oCurSectionProp.m_eSectionBreak,		sCommand, true, RtfSectionProperty::sb_sbkpage)
	COMMAND_RTF_INT	( L"sbkeven",	oReader.m_oCurSectionProp.m_eSectionBreak,		sCommand, true, RtfSectionProperty::sb_sbkeven)
	COMMAND_RTF_INT	( L"sbkodd",	oReader.m_oCurSectionProp.m_eSectionBreak,		sCommand, true, RtfSectionProperty::sb_sbkodd)
	else if ( L"cols" == sCommand )
	{
		if ( hasParameter )
		{
			oReader.m_oCurSectionProp.m_nColumnNumber = parameter;
			oReader.m_oCurSectionProp.m_oCollumnProperty.m_aCollumnProperty.resize( parameter );
		}
	}
	COMMAND_RTF_INT( L"colsx", oReader.m_oCurSectionProp.m_nColumnSpace, sCommand, hasParameter, parameter)
	else if ( L"colno" == sCommand )
	{
		if ( hasParameter )
			nCurCollumnNumber = parameter - 1;
	}
	else if ( L"colsr" == sCommand )
	{
		if ( hasParameter )
			if ( nCurCollumnNumber >= 0 && nCurCollumnNumber < (int)oReader.m_oCurSectionProp.m_oCollumnProperty.m_aCollumnProperty.size() )
				oReader.m_oCurSectionProp.m_oCollumnProperty.m_aCollumnProperty[ nCurCollumnNumber ].m_nColumnSpaceToRightOfCol = parameter;
	}
	else if ( L"colw" == sCommand )
	{
		if ( hasParameter )
			if ( nCurCollumnNumber >= 0 && nCurCollumnNumber < (int)oReader.m_oCurSectionProp.m_oCollumnProperty.m_aCollumnProperty.size() )
				oReader.m_oCurSectionProp.m_oCollumnProperty.m_aCollumnProperty[ nCurCollumnNumber ].m_nColumnWidth = parameter;
	}
	COMMAND_RTF_BOOL( L"linebetcol",	oReader.m_oCurSectionProp.m_bColumnLineBetween, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT	( L"sftntj",		oReader.m_oCurSectionProp.m_eFootnotesJust,		sCommand, true, RtfSectionProperty::fj_sftntj)
	COMMAND_RTF_INT	( L"sftnbj",		oReader.m_oCurSectionProp.m_eFootnotesJust,		sCommand, true, RtfSectionProperty::fj_sftnbj)
//Footnotes and Endnotes
	COMMAND_RTF_INT	( L"sftnstart",		oReader.m_oCurSectionProp.m_nFootnotesStart,	sCommand, hasParameter, parameter)
	COMMAND_RTF_INT	( L"sftnrstpg",		oReader.m_oCurSectionProp.m_eFootnotesRestart,	sCommand, true, RtfSectionProperty::fr_sftnrstpg)
	COMMAND_RTF_INT	( L"sftnrestart",	oReader.m_oCurSectionProp.m_eFootnotesRestart,	sCommand, true, RtfSectionProperty::fr_sftnrestart)
	COMMAND_RTF_INT	( L"sftnrstcont",	oReader.m_oCurSectionProp.m_eFootnotesRestart,	sCommand, true, RtfSectionProperty::fr_sftnrstcont)
	COMMAND_RTF_INT	( L"sftnnar",		oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnnar)
	COMMAND_RTF_INT	( L"sftnnalc",		oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnnalc)
	COMMAND_RTF_INT	( L"sftnnauc",		oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnnauc)
	COMMAND_RTF_INT	( L"sftnnrlc",		oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnnrlc)
	COMMAND_RTF_INT	( L"sftnnruc",		oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnnruc)
	COMMAND_RTF_INT	( L"sftnnchi",		oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnnchi)
	COMMAND_RTF_INT	( L"sftnnchosung",	oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnnchosung)
	COMMAND_RTF_INT	( L"sftnncnum",		oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnncnum)
	COMMAND_RTF_INT	( L"sftnndbnum",	oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnndbnum)
	COMMAND_RTF_INT	( L"sftnndbnumd",	oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnndbnumd)
	COMMAND_RTF_INT	( L"sftnndbnumt",	oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnndbnumt)
	COMMAND_RTF_INT	( L"sftnndbnumk",	oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnndbnumk)
	COMMAND_RTF_INT	( L"sftnndbar",		oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnndbar)
	COMMAND_RTF_INT	( L"sftnnganada",	oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnnganada)
	COMMAND_RTF_INT	( L"sftnngbnum",	oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnngbnum)
	COMMAND_RTF_INT	( L"sftnngbnumd",	oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnngbnumd)
	COMMAND_RTF_INT	( L"sftnngbnuml",	oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnngbnuml)
	COMMAND_RTF_INT	( L"sftnngbnumk",	oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnngbnumk)
	COMMAND_RTF_INT	( L"sftnnzodiac",	oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnnzodiac)
	COMMAND_RTF_INT	( L"sftnnzodiacd",	oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnnzodiacd)
	COMMAND_RTF_INT	( L"sftnnzodiacl",	oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnnzodiacl)
						
	COMMAND_RTF_INT	( L"saftnstart",	oReader.m_oCurSectionProp.m_nEndnotesStart,		sCommand, hasParameter, parameter)
	COMMAND_RTF_INT	( L"saftnrestart",	oReader.m_oCurSectionProp.m_eEndnotesRestart,	sCommand, true, RtfSectionProperty::er_saftnrestart)
	COMMAND_RTF_INT	( L"saftnrstcont",	oReader.m_oCurSectionProp.m_eEndnotesRestart,	sCommand, true, RtfSectionProperty::er_saftnrstcont)
	COMMAND_RTF_INT	( L"saftnnar",		oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnnar)
	COMMAND_RTF_INT	( L"saftnnalc",		oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnnalc)
	COMMAND_RTF_INT	( L"saftnnauc",		oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnnauc)
	COMMAND_RTF_INT	( L"saftnnrlc",		oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnnrlc)
	COMMAND_RTF_INT	( L"saftnnruc",		oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnnruc)
	COMMAND_RTF_INT	( L"saftnnchi",		oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnnchi)
	COMMAND_RTF_INT	( L"saftnnchosung",	oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnnchosung)
	COMMAND_RTF_INT	( L"saftnncnum",	oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnncnum)
	COMMAND_RTF_INT	( L"saftnndbnum",	oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnndbnum)
	COMMAND_RTF_INT	( L"saftnndbnumd",	oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnndbnumd)
	COMMAND_RTF_INT	( L"saftnndbnumt",	oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnndbnumt)
	COMMAND_RTF_INT	( L"saftnndbnumk",	oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnndbnumk)
	COMMAND_RTF_INT	( L"saftnndbar",	oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnndbar)
	COMMAND_RTF_INT	( L"saftnnganada",	oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnnganada)
	COMMAND_RTF_INT	( L"saftnngbnum",	oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnngbnum)
	COMMAND_RTF_INT	( L"saftnngbnumd",	oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnngbnumd)
	COMMAND_RTF_INT	( L"saftnngbnuml",	oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnngbnuml)
	COMMAND_RTF_INT	( L"saftnngbnumk",	oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnngbnumk)
	COMMAND_RTF_INT	( L"saftnnzodiac",	oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnnzodiac)
	COMMAND_RTF_INT	( L"saftnnzodiacd",	oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnnzodiacd)
	COMMAND_RTF_INT	( L"saftnnzodiacl",	oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnnzodiacl)
//Line Numbering
	COMMAND_RTF_INT	( L"linemod",		oReader.m_oCurSectionProp.m_nLineModulus,		sCommand, hasParameter, parameter)
	COMMAND_RTF_INT	( L"linex",			oReader.m_oCurSectionProp.m_nLineX,				sCommand, hasParameter, parameter)
	COMMAND_RTF_INT	( L"linestarts",	oReader.m_oCurSectionProp.m_nLineStart,			sCommand, hasParameter, parameter)
	COMMAND_RTF_INT	( L"linerestart",	oReader.m_oCurSectionProp.m_eLineNumberRestart, sCommand, true, RtfSectionProperty::lnr_linerestart)
	COMMAND_RTF_INT	( L"lineppage",		oReader.m_oCurSectionProp.m_eLineNumberRestart, sCommand, true, RtfSectionProperty::lnr_lineppage)
	COMMAND_RTF_INT	( L"linecont",		oReader.m_oCurSectionProp.m_eLineNumberRestart, sCommand, true, RtfSectionProperty::lnr_linecont)
//Page Information
	COMMAND_RTF_INT	( L"pgwsxn",		oReader.m_oCurSectionProp.m_nPageWidth,			sCommand, hasParameter, parameter)
	COMMAND_RTF_INT	( L"pghsxn",		oReader.m_oCurSectionProp.m_nPageHeight,		sCommand, hasParameter, parameter)
	COMMAND_RTF_INT	( L"marglsxn",		oReader.m_oCurSectionProp.m_nMarginLeft,		sCommand, hasParameter, parameter)
	COMMAND_RTF_INT	( L"margrsxn",		oReader.m_oCurSectionProp.m_nMarginRight,		sCommand, hasParameter, parameter)
	COMMAND_RTF_INT	( L"margtsxn",		oReader.m_oCurSectionProp.m_nMarginTop,			sCommand, hasParameter, parameter)
	COMMAND_RTF_INT	( L"margbsxn",		oReader.m_oCurSectionProp.m_nMarginBottom,		sCommand, hasParameter, parameter)
	COMMAND_RTF_INT	( L"guttersxn",		oReader.m_oCurSectionProp.m_nGutterMarginWidth, sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( L"margmirsxn",	oReader.m_oCurSectionProp.m_bSwitchMargin,		sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( L"lndscpsxn",		oReader.m_oCurSectionProp.m_bLandscapeFormat,	sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( L"titlepg",		oReader.m_oCurSectionProp.m_bTitlePage,			sCommand, hasParameter, parameter)
	//else if ( L"headery" == sCommand )
	//{
	//	static int nCount = 0;
	//	nCount++;
	//	ATLTRACE( "count %d\n", nCount );
	//}
	COMMAND_RTF_INT	( L"headery",	oReader.m_oCurSectionProp.m_nHeaderTop,			sCommand, hasParameter, parameter)
	COMMAND_RTF_INT	( L"footery",	oReader.m_oCurSectionProp.m_nFooterBottom,		sCommand, hasParameter, parameter)
	//Page Numbers
	COMMAND_RTF_INT	( L"pgnstarts",	oReader.m_oCurSectionProp.m_nPageNumberStart,	sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( L"pgncont",	oReader.m_oCurSectionProp.m_bPageNumberContinuos, sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( L"pgnrestart", oReader.m_oCurSectionProp.m_bPageNumberRestart, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT	( L"pgnx",		oReader.m_oCurSectionProp.m_nPageNumberX,		sCommand, hasParameter, parameter)
	COMMAND_RTF_INT	( L"pgny",		oReader.m_oCurSectionProp.m_nPageNumberY,		sCommand, hasParameter, parameter)
	COMMAND_RTF_INT	( L"pgndec",	oReader.m_oCurSectionProp.m_ePageNumberFormat,	sCommand, true, RtfSectionProperty::pnf_pgndec)
	COMMAND_RTF_INT	( L"pgnucrm",	oReader.m_oCurSectionProp.m_ePageNumberFormat,	sCommand, true, RtfSectionProperty::pnf_pgnucrm)
	COMMAND_RTF_INT	( L"pgnlcrm",	oReader.m_oCurSectionProp.m_ePageNumberFormat,	sCommand, true, RtfSectionProperty::pnf_pgnlcrm)
	COMMAND_RTF_INT	( L"vertalt",	oReader.m_oCurSectionProp.m_eVerticalAlignment, sCommand, true, RtfSectionProperty::va_vertalt)
	COMMAND_RTF_INT	( L"vertalb",	oReader.m_oCurSectionProp.m_eVerticalAlignment, sCommand, true, RtfSectionProperty::va_vertalb)
	COMMAND_RTF_INT	( L"vertalc",	oReader.m_oCurSectionProp.m_eVerticalAlignment, sCommand, true, RtfSectionProperty::va_vertalc)
	COMMAND_RTF_INT	( L"vertalj",	oReader.m_oCurSectionProp.m_eVerticalAlignment, sCommand, true, RtfSectionProperty::va_vertalj)
//Text Flow
	COMMAND_RTF_INT	( L"stextflow",	oReader.m_oCurSectionProp.m_nTextFollow,		sCommand, hasParameter, parameter)

//Page Borders
	else if ( L"pgbrdrl" == sCommand )	m_eInternalState = is_border_left;
	else if ( L"pgbrdrt" == sCommand )	m_eInternalState = is_border_top;
	else if ( L"pgbrdrr" == sCommand )	m_eInternalState = is_border_right;
	else if ( L"pgbrdrb" == sCommand )	m_eInternalState = is_border_bottom;

	COMMAND_RTF_INT	( L"brdrart",	oReader.m_oCurSectionProp.m_nBorderArt,		sCommand, hasParameter, parameter)
	COMMAND_RTF_INT	( L"pgbrdropt",	oReader.m_oCurSectionProp.m_nBorderMeasure, sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( L"pgbrdrsna",	oReader.m_oCurSectionProp.m_nBorderAlign,	sCommand, hasParameter, parameter)
   
	else if ( L"header" == sCommand ||	L"footer" == sCommand ||	L"headerl" == sCommand ||
             L"headerr" == sCommand ||  L"headerf" == sCommand ||	L"footerl" == sCommand ||
             L"footerr" == sCommand ||  L"footerf" == sCommand )
	{
		ParagraphReader oParagraphReader(sCommand, oReader);
		oAbstrReader.StartSubReader( oParagraphReader, oDocument, oReader );
		
		oParagraphReader.m_oParPropDest.Finalize(oReader/*, RtfSectionPtr()*/);
		
		TextItemContainerPtr oNewFooterHeader = oParagraphReader.m_oParPropDest.m_oTextItems; 

		if		( L"header"		== sCommand )	oReader.m_oCurSectionProp.m_oHeaderRight = oNewFooterHeader;
		else if ( L"footer"		== sCommand )	oReader.m_oCurSectionProp.m_oFooterRight = oNewFooterHeader;
		else if ( L"headerl"	== sCommand )	oReader.m_oCurSectionProp.m_oHeaderLeft = oNewFooterHeader;
		else if ( L"headerr"	== sCommand )	oReader.m_oCurSectionProp.m_oHeaderRight = oNewFooterHeader;
		else if ( L"headerf"	== sCommand )	oReader.m_oCurSectionProp.m_oHeaderFirst = oNewFooterHeader;
		else if ( L"footerl"	== sCommand )	oReader.m_oCurSectionProp.m_oFooterLeft = oNewFooterHeader;
		else if ( L"footerr"	== sCommand )	oReader.m_oCurSectionProp.m_oFooterRight = oNewFooterHeader;
		else if ( L"footerf"	== sCommand )	oReader.m_oCurSectionProp.m_oFooterFirst = oNewFooterHeader;
	}
	else
	{
		bool bRes = false;
		if ( is_border_left == m_eInternalState )
			bRes = RtfBorderCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, oReader.m_oCurSectionProp.m_oBorderLeft );
		else if ( is_border_top == m_eInternalState )
			bRes = RtfBorderCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, oReader.m_oCurSectionProp.m_oBorderTop );
		else if ( is_border_right == m_eInternalState )
			bRes = RtfBorderCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, oReader.m_oCurSectionProp.m_oBorderRight );
		else if ( is_border_bottom == m_eInternalState )
			bRes = RtfBorderCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, oReader.m_oCurSectionProp.m_oBorderBottom );

		if ( bRes )
			return true;
		return false;
	}

	return true;
}


bool RtfShadingCommand::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, CString sCommand, bool hasParameter, int parameter, RtfShading& oOutput )
{
	if		( L"bghoriz"	== sCommand )	oOutput.m_eType = RtfShading::st_chbghoriz;
	else if ( L"bgvert"		== sCommand )	oOutput.m_eType = RtfShading::st_chbgvert;
	else if ( L"bgfdiag"	== sCommand )	oOutput.m_eType = RtfShading::st_chbgfdiag;
	else if ( L"bgbdiag"	== sCommand )	oOutput.m_eType = RtfShading::st_chbgbdiag;
	else if ( L"bgcross"	== sCommand )	oOutput.m_eType = RtfShading::st_chbgcross;
	else if ( L"bgdcross"	== sCommand )	oOutput.m_eType = RtfShading::st_chbgdcross;
	else if ( L"bgdkhoriz"	== sCommand )	oOutput.m_eType = RtfShading::st_chbgdkhoriz;
	else if ( L"bgdkvert"	== sCommand )	oOutput.m_eType = RtfShading::st_chbgdkvert;
	else if ( L"bgdkfdiag"	== sCommand )	oOutput.m_eType = RtfShading::st_chbgdkfdiag;
	else if ( L"bgdkbdiag"	== sCommand )	oOutput.m_eType = RtfShading::st_chbgdkbdiag;
	else if ( L"bgdkcross"	== sCommand )	oOutput.m_eType = RtfShading::st_chbgdkcross;
	else if ( L"bgdkdcross"	== sCommand )	oOutput.m_eType = RtfShading::st_chbgdkdcross;
	else if ( L"cfpat" == sCommand )
	{
		if ( hasParameter )
			oOutput.m_nForeColor = parameter;
	}
	else if ( L"cbpat" == sCommand )
	{
		if ( hasParameter )
			oOutput.m_nBackColor = parameter;
	}
	else if ( L"shading" == sCommand )
	{
		if ( hasParameter )
			oOutput.m_nValue = parameter;
	}
	else
		return false;
	return true;
}
bool RtfShadingCellCommand::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter, RtfShading& oOutput)
{
	//для свойст таблицы и для стилей таблицы
	if ( L"clshdrawnil" == sCommand )
		oOutput.m_eType = RtfShading::st_clshdrawnil;
	else if ( L"rawclbgvert" == sCommand || L"clbgvert" == sCommand || L"tsbgvert" == sCommand )
		oOutput.m_eType = RtfShading::st_chbgvert;
	else if ( L"rawclbgfdiag" == sCommand || L"clbgfdiag" == sCommand || L"tsbgfdiag" == sCommand )
		oOutput.m_eType = RtfShading::st_chbgfdiag;
	else if ( L"rawclbgbdiag" == sCommand || L"clbgbdiag" == sCommand || L"tsbgbdiag" == sCommand )
		oOutput.m_eType = RtfShading::st_chbgbdiag;
	else if ( L"rawclbgcross" == sCommand || L"clbgcross" == sCommand || L"tsbgcross" == sCommand )
		oOutput.m_eType = RtfShading::st_chbgcross;
	else if ( L"rawclbgdcross" == sCommand || L"clbgdcross" == sCommand || L"tsbgdcross" == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdcross;
	else if ( L"rawclbgdkhor" == sCommand || L"clbgdkhor" == sCommand || L"tsbgdkhor" == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkhoriz;
	else if ( L"rawclbgdkvert" == sCommand || L"clbgdkvert" == sCommand || L"tsbgdkvert" == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkvert;
	else if ( L"rawclbgdkfdiag" == sCommand || L"clbgdkfdiag" == sCommand || L"tsbgdkfdiag" == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkfdiag;
	else if ( L"rawclbgdkbdiag" == sCommand || L"clbgdkbdiag" == sCommand || L"tsbgdkbdiag" == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkbdiag;
	else if ( L"rawclbgdkcross" == sCommand || L"clbgdkcross" == sCommand || L"tsbgdkcross" == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkcross;
	else if ( L"rawclbgdkdcross" == sCommand || L"clbgdkdcross" == sCommand || L"tsbgdkdcross" == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkdcross;
	COMMAND_RTF_INT( L"clcfpat",		oOutput.m_nForeColor, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"clcbpat",		oOutput.m_nBackColor, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"clcfpatraw",		oOutput.m_nForeColor, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"clcbpatraw",		oOutput.m_nBackColor, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"tscellcfpat",	oOutput.m_nForeColor, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"tscellcbpat",	oOutput.m_nBackColor, sCommand, hasParameter, parameter )

	COMMAND_RTF_INT( L"clshdng",		oOutput.m_nValue, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"clshdngraw",		oOutput.m_nValue, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"tscellpct",		oOutput.m_nValue, sCommand, hasParameter, parameter )
	else
		return false;

	if (sCommand == L"clshdng")		oReader.m_oState->m_oCellProperty.m_nShadingPctFrom = 1;
	if (sCommand == L"clshdngraw")	oReader.m_oState->m_oCellProperty.m_nShadingPctFrom = 2;
	return true;
}

bool RtfShadingCharCommand::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter, RtfShading& oOutput )
{
	if		( L"chbghoriz"		== sCommand )		oOutput.m_eType = RtfShading::st_chbghoriz;
	else if ( L"chbgvert"		== sCommand )		oOutput.m_eType = RtfShading::st_chbgvert;
	else if ( L"chbgfdiag"		== sCommand )		oOutput.m_eType = RtfShading::st_chbgfdiag;
	else if ( L"chbgbdiag"		== sCommand )		oOutput.m_eType = RtfShading::st_chbgbdiag;
	else if ( L"chbgcross"		== sCommand )		oOutput.m_eType = RtfShading::st_chbgcross;
	else if ( L"chbgdcross"		== sCommand )		oOutput.m_eType = RtfShading::st_chbgdcross;
	else if ( L"chbgdkhoriz"	== sCommand )		oOutput.m_eType = RtfShading::st_chbgdkhoriz;
	else if ( L"chbgdkvert"		== sCommand )		oOutput.m_eType = RtfShading::st_chbgdkvert;
	else if (  L"chbgdkfdiag"	== sCommand )		oOutput.m_eType = RtfShading::st_chbgdkfdiag;
	else if ( L"chbgdkbdiag"	== sCommand )		oOutput.m_eType = RtfShading::st_chbgdkbdiag;
	else if (  L"chbgdkcross"	== sCommand )		oOutput.m_eType = RtfShading::st_chbgdkcross;
	else if ( L"chbgdkdcross"	== sCommand )		oOutput.m_eType = RtfShading::st_chbgdkdcross;
	else if ( L"chcfpat"		== sCommand )	{
		if ( hasParameter )
			oOutput.m_nForeColor = parameter;
	}
	else if ( L"chcbpat" == sCommand )
	{
		if ( hasParameter )
			oOutput.m_nBackColor = parameter;
	}
	else if ( L"chshdng" == sCommand )
	{
		if ( hasParameter )
			oOutput.m_nValue = parameter;
	}
	else
		return false;
	return true;
}

bool RtfShadingRowCommand::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter, RtfShading& oOutput)
{
	if		( L"trbghoriz"		== sCommand )		oOutput.m_eType = RtfShading::st_chbghoriz;
	else if ( L"trbgvert"		== sCommand )		oOutput.m_eType = RtfShading::st_chbgvert;
	else if	( L"trbgfdiag"		== sCommand )		oOutput.m_eType = RtfShading::st_chbgfdiag;
	else if	( L"trbgbdiag"		== sCommand )		oOutput.m_eType = RtfShading::st_chbgbdiag;
	else if	( L"trbgcross"		== sCommand )		oOutput.m_eType = RtfShading::st_chbgcross;
	else if	( L"trbgdcross"		== sCommand )		oOutput.m_eType = RtfShading::st_chbgdcross;
	else if	( L"trbgdkhor"		== sCommand )		oOutput.m_eType = RtfShading::st_chbgdkhoriz;
	else if	( L"trbgdkvert"		== sCommand )		oOutput.m_eType = RtfShading::st_chbgdkvert;
	else if	( L"trbgdkfdiag"	== sCommand )		oOutput.m_eType = RtfShading::st_chbgdkfdiag;
	else if	( L"trbgdkbdiag"	== sCommand )		oOutput.m_eType = RtfShading::st_chbgdkbdiag;
	else if	( L"trbgdkcross"	== sCommand )		oOutput.m_eType = RtfShading::st_chbgdkcross;
	else if	( L"trbgdkdcross"	== sCommand )		oOutput.m_eType = RtfShading::st_chbgdkdcross;
	else if	( L"trcfpat"		== sCommand )
	{
		if ( hasParameter )
			oOutput.m_nForeColor = parameter;
	}
	else if ( L"trcbpat" == sCommand )
	{
		if ( hasParameter )
			oOutput.m_nBackColor = parameter;
	}
	else if ( L"trshdng" == sCommand )
	{
		if ( hasParameter )
			oOutput.m_nValue = parameter;
	}
	else
		return false;
	return true;
}

bool RtfCharPropsCommand::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, CString sCommand, bool hasParameter, int parameter, RtfCharProperty * charProps, bool bLookOnBorder)
{
	if (!charProps) return false;
		
	if ( L"plain" == sCommand )
		charProps->SetDefaultRtf();

	COMMAND_RTF_INT	( L"animtext"	,	charProps->m_nAnimated,			sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( L"b"			,	charProps->m_bBold,				sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( L"caps"		,	charProps->m_bCaps,				sCommand, hasParameter, parameter)
	COMMAND_RTF_INT	( L"charscalex"	,	charProps->m_nScalex,			sCommand, hasParameter, parameter)
	COMMAND_RTF_INT	( L"cs"			,	charProps->m_nCharStyle,		sCommand, hasParameter, parameter)
	COMMAND_RTF_INT	( L"down"		,	charProps->m_nDown,				sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( L"embo"		,	charProps->m_bEmbo,				sCommand, hasParameter, parameter)
	COMMAND_RTF_INT	( L"expndtw"	,	charProps->m_nCharacterSpacing,	sCommand, hasParameter, parameter)
	
	else if ( L"expnd" == sCommand )
	{
		if ( hasParameter )
			charProps->m_nCharacterSpacing  = 5 * parameter; //quater -points
	}
	COMMAND_RTF_INT	( L"fittext"	, charProps->m_nFitText, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT	( L"f"			, charProps->m_nFont, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT	( L"fs"			, charProps->m_nFontSize, sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( L"i"			, charProps->m_bItalic, sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( L"impr"		, charProps->m_bImprint, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT	( L"kerning"	, charProps->m_nKerning, sCommand, hasParameter, parameter)
	
	else if ( L"ltrch" == sCommand )
	{
		if ( false == hasParameter || 0 != parameter ) 
			charProps->m_bRightToLeft  = 0;
		else
			charProps->m_bRightToLeft  = 1;
	}
	COMMAND_RTF_BOOL( L"rtlch",		charProps->m_bRightToLeft,	sCommand, hasParameter, parameter)
	COMMAND_RTF_INT	( L"lang",		charProps->m_nLanguage,		sCommand, hasParameter, parameter)
	
	COMMAND_RTF_BOOL( L"cs",		charProps->m_nComplexScript, sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( L"outl",		charProps->m_bOutline,		sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( L"scaps",		charProps->m_bScaps,		sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( L"shad",		charProps->m_bShadow,		sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( L"strike",	charProps->m_bStrike,		sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( L"striked",	charProps->m_nStriked,		sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( L"sub",		charProps->m_bSub,			sCommand, hasParameter, parameter)
	COMMAND_RTF_BOOL( L"super",		charProps->m_bSuper,		sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( L"highlight",	charProps->m_nHightlited,	sCommand, hasParameter, parameter)
	else if ( L"cf" == sCommand )
	{
		if ( hasParameter )
			charProps->m_nForeColor = parameter;
		else
			charProps->m_nForeColor= PROP_DEF;
	}
	else if ( L"ul" == sCommand )
	{
		if ( hasParameter && 0 == parameter)
			charProps->m_eUnderStyle = RtfCharProperty::uls_none;
		else
			charProps->m_eUnderStyle = RtfCharProperty::uls_Single;
	}
	//COMMAND_RTF_BOOL( L"ul", charProps->m_bUnderline, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( L"ulc", charProps->m_nUnderlineColor, sCommand, hasParameter, parameter)

	COMMAND_RTF_INT( L"uld",		charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_Dotted)
	COMMAND_RTF_INT( L"uldash",		charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_Dashed)
	COMMAND_RTF_INT( L"uldashd",	charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_Dash_dotted)
	COMMAND_RTF_INT( L"uldashdd",	charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_Dash_dot_dotted)
	COMMAND_RTF_INT( L"uldb",		charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_Double)
	COMMAND_RTF_INT( L"ulhwave",	charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_Heavy_wave)
	COMMAND_RTF_INT( L"ulldash",	charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_Long_dashe)
	COMMAND_RTF_INT( L"ulnone",		charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_none)
	COMMAND_RTF_INT( L"ulth",		charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_Thick)
	COMMAND_RTF_INT( L"ulthd",		charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_Thick_dotted)
	COMMAND_RTF_INT( L"ulthdash",	charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_Thick_dashed)
	COMMAND_RTF_INT( L"ulthdashd",	charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_Thick_dash_dotted)
	COMMAND_RTF_INT( L"ulthdashdd",	charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_Thick_dash_dot_dotted)
	COMMAND_RTF_INT( L"ulthldash",	charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_Thick_long_dashed)
	COMMAND_RTF_INT( L"ululdbwave",	charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_Double_wave)
	COMMAND_RTF_INT( L"ulw",		charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_Word)
	COMMAND_RTF_INT( L"ulwave",		charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_Wave)
	
	COMMAND_RTF_INT( L"up",			charProps->m_nUp,			sCommand, hasParameter, parameter)

	COMMAND_RTF_INT( L"crauth",		charProps->m_nCrAuth,		sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( L"crdate",		charProps->m_nCrDate,		sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( L"revaut",		charProps->m_nRevauth,		sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( L"revdttm",	charProps->m_nRevdttm,		sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( L"revauthdel",	charProps->m_nRevauthDel,	sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( L"revdttmdel",	charProps->m_nRevdttmDel,	sCommand, hasParameter, parameter)
	COMMAND_RTF_INT( L"insrsid",	charProps->m_nInsrsid,		sCommand, hasParameter, parameter)
	
	else if ( L"revised" == sCommand )
	{
		charProps->m_nRevised = 1;
	}	
	else if ( L"deleted" == sCommand )
	{
		charProps->m_nDeleted = 1;
	}	
	else if ( L"nosupersub" == sCommand )
	{
		charProps->m_bSub	= 0;
		charProps->m_bSuper  = 0;
	}
	else if ( L"nosectexpand" == sCommand )
	{
		charProps->m_nCharacterSpacing  = PROP_DEF;
	}
	else
	{
		if (RtfShadingCharCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, charProps->m_poShading))
			return true;
		
		if ( bLookOnBorder )
		{
			if (RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, charProps->m_poBorder))
				return true;
		}
		
		return false;
	}
	return true;
}
bool RtfParagraphPropsCommand::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, CString sCommand, bool hasParameter, int parameter, RtfParagraphProperty * paragraphProps)
{
	if (!paragraphProps) return false;

	if ( L"pard" == sCommand )
	{
		paragraphProps->SetDefaultRtf();
	}
	COMMAND_RTF_INT	( L"outlinelevel",		paragraphProps->m_nOutlinelevel,		sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"hyphpar",			paragraphProps->m_bAutoHyphenation,		sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"contextualspace",	paragraphProps->m_bContextualSpacing,	sCommand, hasParameter, parameter )
	else if ( L"intbl" == sCommand )
	{
		if ( hasParameter && 0 == parameter )
		{
			paragraphProps->m_bInTable = 0;
			if ( PROP_DEF == paragraphProps->m_nItap )
				paragraphProps->m_nItap = 0;
		}
		else
		{
			paragraphProps->m_bInTable = 1;
			if ( PROP_DEF == paragraphProps->m_nItap )
				paragraphProps->m_nItap = 1;
		}
	}
	COMMAND_RTF_BOOL( L"intbl", paragraphProps->m_bInTable,	sCommand, hasParameter, parameter )
	COMMAND_RTF_INT	( L"itap",	paragraphProps->m_nItap,	sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"keep",	paragraphProps->m_bKeep,	sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"keepn", paragraphProps->m_bKeepNext, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"pagebb", paragraphProps->m_bPageBB,	sCommand, hasParameter, parameter )
	COMMAND_RTF_INT	( L"s",		paragraphProps->m_nStyle,	sCommand, hasParameter, parameter )
	COMMAND_RTF_INT	( L"qc",	paragraphProps->m_eAlign,	sCommand, true, RtfParagraphProperty::pa_qc )
	COMMAND_RTF_INT	( L"qj",	paragraphProps->m_eAlign,	sCommand, true, RtfParagraphProperty::pa_qj )
	COMMAND_RTF_INT	( L"ql",	paragraphProps->m_eAlign,	sCommand, true, RtfParagraphProperty::pa_ql )
	COMMAND_RTF_INT	( L"qr",	paragraphProps->m_eAlign,	sCommand, true, RtfParagraphProperty::pa_qr )
	COMMAND_RTF_INT	( L"qd",	paragraphProps->m_eAlign,	sCommand, true, RtfParagraphProperty::pa_qd )
	
	else if ( L"qk0" == sCommand )
	{
		switch( parameter )
		{
			case 0:		paragraphProps->m_eAlign= RtfParagraphProperty::pa_qk0;		break;
			case 10:	paragraphProps->m_eAlign = RtfParagraphProperty::pa_qk10;	break;
			case 20:	paragraphProps->m_eAlign = RtfParagraphProperty::pa_qk20;	break;

		}
	}
	COMMAND_RTF_INT( L"faauto",		paragraphProps->m_eFontAlign,		sCommand, true, RtfParagraphProperty::fa_faauto )
	COMMAND_RTF_INT( L"fahang",		paragraphProps->m_eFontAlign,		sCommand, true, RtfParagraphProperty::fa_fahang )
	COMMAND_RTF_INT( L"facenter",	paragraphProps->m_eFontAlign,		sCommand, true, RtfParagraphProperty::fa_facenter )
	COMMAND_RTF_INT( L"faroman",	paragraphProps->m_eFontAlign,		sCommand, true, RtfParagraphProperty::fa_faroman )
	COMMAND_RTF_INT( L"favar",		paragraphProps->m_eFontAlign,		sCommand, true, RtfParagraphProperty::fa_favar )
	COMMAND_RTF_INT( L"fafixed",	paragraphProps->m_eFontAlign,		sCommand, true, RtfParagraphProperty::fa_fafixed )
	COMMAND_RTF_INT( L"fi",			paragraphProps->m_nIndFirstLine,	sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"li",			paragraphProps->m_nIndLeft,			sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"ri",			paragraphProps->m_nIndRight,		sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"lin",		paragraphProps->m_nIndStart,		sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"rin",		paragraphProps->m_nIndEnd,			sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"adjustright", paragraphProps->m_bIndRightAuto,	sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"indmirror", paragraphProps->m_bIndMirror,		sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"sb",			paragraphProps->m_nSpaceBefore,		sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"sa",			paragraphProps->m_nSpaceAfter,		sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"sbauto",		paragraphProps->m_nSpaceBeforeAuto,	sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"saauto",		paragraphProps->m_nSpaceAfterAuto,	sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"lisb",		paragraphProps->m_nSpaceBeforeLine,	sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"lisa",		paragraphProps->m_nSpaceAfterLine,	sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"slmult",		paragraphProps->m_nSpaceMultiLine,	sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"ilvl",		paragraphProps->m_nListLevel,		sCommand, hasParameter, parameter )
	else if ( L"sl" == sCommand )
	{
		if ( hasParameter )
		{
			paragraphProps->m_nSpaceBetween = parameter;
			if ( PROP_DEF == paragraphProps->m_nSpaceMultiLine )
				paragraphProps->m_nSpaceMultiLine = 0;
		}
	}
	else if ( L"rtlpar" == sCommand )		paragraphProps->m_bRtl = 1;
	else if ( L"ltrpar" == sCommand )		paragraphProps->m_bRtl = 0;
	COMMAND_RTF_BOOL( L"nowwrap", paragraphProps->m_bNoWordWrap, sCommand, hasParameter, parameter )
	else if ( L"nowwrap" == sCommand )
	{
		if ( hasParameter && 0 == parameter)
			paragraphProps->m_bSnapToGrid = 1;
		else
			paragraphProps->m_bSnapToGrid = 0;
	}
	else if ( L"ls" == sCommand )
	{
		if ( hasParameter )
		{
			paragraphProps->m_nListId = parameter;
			if ( PROP_DEF == paragraphProps->m_nListLevel )
				paragraphProps->m_nListLevel = 0;
		}
	}
//Frame
	COMMAND_RTF_INT( L"absw",		paragraphProps->m_oFrame.m_nWidth, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"absh",		paragraphProps->m_oFrame.m_nHeight, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"phmrg",		paragraphProps->m_oFrame.m_eHRef, sCommand, true, RtfFrame::hr_phmrg )
	COMMAND_RTF_INT( L"phpg",		paragraphProps->m_oFrame.m_eHRef, sCommand, true, RtfFrame::hr_phpg )
	COMMAND_RTF_INT( L"phcol",		paragraphProps->m_oFrame.m_eHRef, sCommand, true, RtfFrame::hr_phcol )
	COMMAND_RTF_INT( L"posx",		paragraphProps->m_oFrame.m_nHPos, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"posnegx",	paragraphProps->m_oFrame.m_nHPos, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"posxc",		paragraphProps->m_oFrame.m_eHPos, sCommand, true, RtfFrame::hp_posxc )
	COMMAND_RTF_INT( L"posxi",		paragraphProps->m_oFrame.m_eHPos, sCommand, true, RtfFrame::hp_posxi )
	COMMAND_RTF_INT( L"posxo",		paragraphProps->m_oFrame.m_eHPos, sCommand, true, RtfFrame::hp_posxo )
	COMMAND_RTF_INT( L"posxl",		paragraphProps->m_oFrame.m_eHPos, sCommand, true, RtfFrame::hp_posxl )
	COMMAND_RTF_INT( L"posxr",		paragraphProps->m_oFrame.m_eHPos, sCommand, true, RtfFrame::hp_posxr )
	COMMAND_RTF_INT( L"pvmrg",		paragraphProps->m_oFrame.m_eVRef, sCommand, true, RtfFrame::vr_pvmrg )
	COMMAND_RTF_INT( L"pvpg",		paragraphProps->m_oFrame.m_eVRef, sCommand, true, RtfFrame::vr_pvpg )
	COMMAND_RTF_INT( L"pvpara",		paragraphProps->m_oFrame.m_eVRef, sCommand, true, RtfFrame::vr_pvpara )
	COMMAND_RTF_INT( L"posy",		paragraphProps->m_oFrame.m_nVPos, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"posnegy",	paragraphProps->m_oFrame.m_nVPos, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"posyt",		paragraphProps->m_oFrame.m_eVPos, sCommand, true, RtfFrame::vp_posyt )
	COMMAND_RTF_INT( L"posyil",		paragraphProps->m_oFrame.m_eVPos, sCommand, true, RtfFrame::vp_posyil )
	COMMAND_RTF_INT( L"posyb",		paragraphProps->m_oFrame.m_eVPos, sCommand, true, RtfFrame::vp_posyb )
	COMMAND_RTF_INT( L"posyc",		paragraphProps->m_oFrame.m_eVPos, sCommand, true, RtfFrame::vp_posyc )
	COMMAND_RTF_INT( L"posyin",		paragraphProps->m_oFrame.m_eVPos, sCommand, true, RtfFrame::vp_posyin )
	COMMAND_RTF_INT( L"posyout",	paragraphProps->m_oFrame.m_eVPos, sCommand, true, RtfFrame::vp_posyout )
	COMMAND_RTF_BOOL( L"abslock",	paragraphProps->m_oFrame.m_bLockAnchor, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"wrapdefault", paragraphProps->m_oFrame.m_eWrap,		sCommand, true, RtfFrame::tw_wrapdefault )
	COMMAND_RTF_INT( L"wraparound",	paragraphProps->m_oFrame.m_eWrap,		sCommand, true, RtfFrame::tw_wraparound )
	COMMAND_RTF_INT( L"wraptight",	paragraphProps->m_oFrame.m_eWrap,		sCommand, true, RtfFrame::tw_wraptight )
	COMMAND_RTF_INT( L"wrapthrough", paragraphProps->m_oFrame.m_eWrap,		sCommand, true, RtfFrame::tw_wrapthrough )
	COMMAND_RTF_INT( L"dropcapt",	paragraphProps->m_oFrame.m_DropcapType, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"dropcapli",	paragraphProps->m_oFrame.m_DropcapLines, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"dxfrtext",	paragraphProps->m_oFrame.m_nAllSpace,	sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"dfrmtxtx",	paragraphProps->m_oFrame.m_nHorSpace,	sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"dfrmtxty",	paragraphProps->m_oFrame.m_nVerSpace,	sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( L"absnoovrlp", paragraphProps->m_bOverlap,	sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"frmtxlrtb",	paragraphProps->m_eTextFollow,	sCommand, true, RtfParagraphProperty::tf_frmtxlrtb )
	COMMAND_RTF_INT( L"frmtxtbrl",	paragraphProps->m_eTextFollow,	sCommand, true, RtfParagraphProperty::tf_frmtxtbrl )
	COMMAND_RTF_INT( L"frmtxbtlr",	paragraphProps->m_eTextFollow,	sCommand, true, RtfParagraphProperty::tf_frmtxbtlr )
	COMMAND_RTF_INT( L"frmtxlrtbv", paragraphProps->m_eTextFollow,	sCommand, true, RtfParagraphProperty::tf_frmtxlrtbv )
	COMMAND_RTF_INT( L"frmtxtbrlv", paragraphProps->m_eTextFollow,	sCommand, true, RtfParagraphProperty::tf_frmtxtbrlv )
	
	else
	{
		if (RtfShadingCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, oReader.m_oState->m_oParagraphProp.m_oShading ))
			return true;

		return false;
	}
	return true;
}

bool RtfTableCellPropsCommand::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, CString sCommand, bool hasParameter, int parameter, RtfCellProperty * cellProps)
{
	if (!cellProps) return false;

	COMMAND_RTF_BOOL( L"clmgf",		cellProps->m_bMergeFirst,			sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"clmrg",		cellProps->m_bMerge,				sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"clvmgf",	cellProps->m_bMergeFirstVertical,	sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"clvmrg",	cellProps->m_bMergeVertical,		sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"clFitText", cellProps->m_bFitText,				sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"clNoWrap",	cellProps->m_bNoWrap,				sCommand, hasParameter, parameter )		

	COMMAND_RTF_INT( L"clpadfl",	cellProps->m_nIsPaddingLeft,		sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"clpadl",		cellProps->m_nPaddingLeft,			sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"clpadft",	cellProps->m_nIsPaddingTop,			sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"clpadt",		cellProps->m_nPaddingTop,			sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"clpadfr",	cellProps->m_nIsPaddingRight,		sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"clpadr",		cellProps->m_nPaddingRight,			sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"clpadfb",	cellProps->m_nIsPaddingBottom,		sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"clpadb",		cellProps->m_nPaddingBottom,		sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"clspfl",		cellProps->m_nIsSpacingLeft,		sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"clspl",		cellProps->m_nSpacingLeft,			sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"clspft",		cellProps->m_nIsSpacingTop,			sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"clspt",		cellProps->m_nSpacingTop,			sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"clspfr",		cellProps->m_nIsSpacingRight,		sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"clspr",		cellProps->m_nSpacingRight,			sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"clspfb",		cellProps->m_nIsSpacingBottom,		sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"clspb",		cellProps->m_nSpacingBottom,		sCommand, hasParameter, parameter )
	
	else if ( L"clftsWidth" == sCommand  )
	{
		if ( hasParameter )
		{
			switch( parameter )
			{
				case 0:cellProps->m_eWidthUnits = mu_none;break;
				case 1:cellProps->m_eWidthUnits = mu_Auto;break;
				case 2:cellProps->m_eWidthUnits = mu_Percent;break;
				case 3:cellProps->m_eWidthUnits = mu_Twips;break;
			}
		}
	}
	COMMAND_RTF_INT( L"clwWidth",	cellProps->m_nWidth,		sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"clhidemark",cellProps->m_bHideMark,		sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"clvertalt",	cellProps->m_eAlign,		sCommand, true, RtfCellProperty::ca_Top )
	COMMAND_RTF_INT( L"clvertalc",	cellProps->m_eAlign,		sCommand, true, RtfCellProperty::ca_Center )
	COMMAND_RTF_INT( L"clvertalb",	cellProps->m_eAlign,		sCommand, true, RtfCellProperty::ca_Bottom )
	COMMAND_RTF_INT( L"cltxlrtb",	cellProps->m_oCellFlow,		sCommand, true, RtfCellProperty::cf_lrtb )
	COMMAND_RTF_INT( L"cltxtbrl",	cellProps->m_oCellFlow,		sCommand, true, RtfCellProperty::cf_tbrl )
	COMMAND_RTF_INT( L"cltxbtlr",	cellProps->m_oCellFlow,		sCommand, true, RtfCellProperty::cf_btlr )
	COMMAND_RTF_INT( L"cltxlrtbv",	cellProps->m_oCellFlow,		sCommand, true, RtfCellProperty::cf_lrtbv )
	COMMAND_RTF_INT( L"cltxtbrlv",	cellProps->m_oCellFlow,		sCommand, true, RtfCellProperty::cf_tbrlv )

//table style
	COMMAND_RTF_INT( L"tscellpaddfl",	cellProps->m_nIsPaddingLeft,	sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"tscellpaddl",	cellProps->m_nPaddingLeft,		sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"tscellpaddft",	cellProps->m_nIsPaddingTop,		sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"tscellpaddt",	cellProps->m_nPaddingTop,		sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"tscellpaddfr",	cellProps->m_nIsPaddingRight,	sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"tscellpaddr",	cellProps->m_nPaddingRight,		sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"tscellpaddfb",	cellProps->m_nIsPaddingBottom,	sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"tscellpaddb",	cellProps->m_nPaddingBottom,	sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"tsnowrap",		cellProps->m_bNoWrap,			sCommand, hasParameter, parameter )		
	COMMAND_RTF_INT( L"tsvertalt",		cellProps->m_eAlign,			sCommand, true, RtfCellProperty::ca_Top )
	COMMAND_RTF_INT( L"tsvertalc",		cellProps->m_eAlign,			sCommand, true, RtfCellProperty::ca_Center )
	COMMAND_RTF_INT( L"tsvertalb",		cellProps->m_eAlign,			sCommand, true, RtfCellProperty::ca_Bottom )
	else
	{
		if (RtfShadingCellCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, cellProps->m_oShading ))
			return true;
		
		return false;
	}
	return true;
}

bool RtfTableRowPropsCommand::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, CString sCommand, bool hasParameter, int parameter, RtfRowProperty * rowProps)
{
	if (!rowProps) return false;
	
	COMMAND_RTF_INT	( L"irow",			rowProps->m_nIndex,		sCommand, hasParameter, parameter )
	COMMAND_RTF_INT	( L"irowband",		rowProps->m_nBandIndex,	sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"lastrow",		rowProps->m_bLastRow,	sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"trhdr",			rowProps->m_bIsHeader,	sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"trkeep",		rowProps->m_bKeep,		sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"trkeepfollow",	rowProps->m_bKeep,		sCommand, hasParameter, parameter )

	COMMAND_RTF_INT	( L"trql",			rowProps->m_eJust,		sCommand, true, RtfRowProperty::rj_trql )
	COMMAND_RTF_INT	( L"trqr",			rowProps->m_eJust,		sCommand, true, RtfRowProperty::rj_trqr )
	COMMAND_RTF_INT	( L"trqc",			rowProps->m_eJust,		sCommand, true, RtfRowProperty::rj_trqc )

	COMMAND_RTF_INT	( L"trrh",			rowProps->m_nHeight,	sCommand, hasParameter, parameter )

	else if ( L"trftsWidth" == sCommand )
	{
		if ( hasParameter )
		{
			switch( parameter )
			{
			case 0:	rowProps->m_eMUWidth = mu_none;		break;
			case 1:	rowProps->m_eMUWidth = mu_Auto;		break;
			case 2:	rowProps->m_eMUWidth = mu_Percent;	break;
			case 3:	rowProps->m_eMUWidth = mu_Twips;	break;
			}
		}
	}
	COMMAND_RTF_INT( L"trwWidth", rowProps->m_nWidth, sCommand, hasParameter, parameter )
	else if ( L"trftsWidthB" == sCommand )
	{
		if ( hasParameter )
		{
			switch( parameter )
			{
			case 0:	rowProps->m_eMUStartInvCell = mu_none;		break;
			case 1:	rowProps->m_eMUStartInvCell = mu_Auto;		break;
			case 2:	rowProps->m_eMUStartInvCell = mu_Percent;	break;
			case 3:	rowProps->m_eMUStartInvCell = mu_Twips;		break;
			}
		}
	}
	COMMAND_RTF_INT( L"trwWidthB", rowProps->m_nWidthStartInvCell, sCommand, hasParameter, parameter )
	else if ( L"trftsWidthA" == sCommand )
	{
		if ( hasParameter )
		{
			switch( parameter )
			{
			case 0:	rowProps->m_eMUEndInvCell = mu_none;		break;
			case 1:	rowProps->m_eMUEndInvCell = mu_Auto;		break;
			case 2:	rowProps->m_eMUEndInvCell = mu_Percent;	break;
			case 3:	rowProps->m_eMUEndInvCell = mu_Twips;	break;
			}
		}
	}
	COMMAND_RTF_INT( L"trwWidthA",		rowProps->m_nWidthEndInvCell,	sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"taprtl",		rowProps->m_bBidi,				sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"trautofit",		rowProps->m_nAutoFit,			sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"trgaph",			rowProps->m_nGraph,				sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"tblind",			rowProps->nTableIndent,			sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"tblindtype",		rowProps->nTableIndentUnits,	sCommand, hasParameter, parameter )

	COMMAND_RTF_INT( L"tdfrmtxtLeft",	rowProps->m_nWrapLeft,			sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"tdfrmtxtRight",	rowProps->m_nWrapRight,			sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"tdfrmtxtTop",	rowProps->m_nWrapTop,			sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"tdfrmtxtBottom", rowProps->m_nWrapBottom,		sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"tabsnoovrlp",	rowProps->m_bOverlap,			sCommand, hasParameter, parameter )

	COMMAND_RTF_INT( L"tphmrg",			rowProps->m_eHRef, sCommand, true, RtfTableProperty::hr_phmrg )
	COMMAND_RTF_INT( L"tphpg",			rowProps->m_eHRef, sCommand, true, RtfTableProperty::hr_phpg )
	COMMAND_RTF_INT( L"tphcol",			rowProps->m_eHRef, sCommand, true, RtfTableProperty::hr_phcol )
	COMMAND_RTF_INT( L"tposx",			rowProps->m_nHPos, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"tposnegx",		rowProps->m_nHPos, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"tposxc",			rowProps->m_eHPos, sCommand, true, RtfTableProperty::hp_posxc )
	COMMAND_RTF_INT( L"tposxi",			rowProps->m_eHPos, sCommand, true, RtfTableProperty::hp_posxi )
	COMMAND_RTF_INT( L"tposxo",			rowProps->m_eHPos, sCommand, true, RtfTableProperty::hp_posxo )
	COMMAND_RTF_INT( L"tposxl",			rowProps->m_eHPos, sCommand, true, RtfTableProperty::hp_posxl )
	COMMAND_RTF_INT( L"tposxr",			rowProps->m_eHPos, sCommand, true, RtfTableProperty::hp_posxr )

	COMMAND_RTF_INT( L"tpvmrg",			rowProps->m_eVRef, sCommand, true, RtfTableProperty::vr_pvmrg )
	COMMAND_RTF_INT( L"tpvpg",			rowProps->m_eVRef, sCommand, true, RtfTableProperty::vr_pvpg )
	COMMAND_RTF_INT( L"tpvpara",		rowProps->m_eVRef, sCommand, true, RtfTableProperty::vr_pvpara )
	COMMAND_RTF_INT( L"tposy",			rowProps->m_nVPos, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"tposnegy",		rowProps->m_nVPos, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"tposyt",			rowProps->m_eVPos, sCommand, true, RtfTableProperty::vp_posyt )
	COMMAND_RTF_INT( L"tposyil",		rowProps->m_eVPos, sCommand, true, RtfTableProperty::vp_posyil )
	COMMAND_RTF_INT( L"tposyb",			rowProps->m_eVPos, sCommand, true, RtfTableProperty::vp_posyb )
	COMMAND_RTF_INT( L"tposyc",			rowProps->m_eVPos, sCommand, true, RtfTableProperty::vp_posyc )
	COMMAND_RTF_INT( L"tposyin",		rowProps->m_eVPos, sCommand, true, RtfTableProperty::vp_posyin )
	COMMAND_RTF_INT( L"tposyout",		rowProps->m_eVPos, sCommand, true, RtfTableProperty::vp_posyout )

	else if ( L"trleft" == sCommand )
	{
		if ( hasParameter )
		{
			rowProps->m_nLeft = parameter;
			if ( PROP_DEF == rowProps->nTableIndent )
			{
				rowProps->nTableIndent = parameter;
				rowProps->nTableIndentUnits = 3;
			}
		}
	}
	COMMAND_RTF_INT( L"trwWidth", rowProps->m_nWidth, sCommand, hasParameter, parameter )
	else if ( L"trleft" == sCommand )
	{
		if ( hasParameter )
		{
			switch( parameter )
			{
			case 0: rowProps->m_eMUWidth = mu_none;		break;
			case 1: rowProps->m_eMUWidth = mu_Auto;		break;
			case 2: rowProps->m_eMUWidth = mu_Percent;	break;
			case 3: rowProps->m_eMUWidth = mu_Twips;	break;
			}
		}
	}
	COMMAND_RTF_INT( L"trpaddb",	rowProps->m_nDefCellMarBottom,		sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"trpaddl",	rowProps->m_nDefCellMarLeft,		sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"trpaddr",	rowProps->m_nDefCellMarRight,		sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"trpaddt",	rowProps->m_nDefCellMarTop,			sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"trpaddfb",	rowProps->m_nDefCellMarBottomUnits,	sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"trpaddfl",	rowProps->m_nDefCellMarLeftUnits,	sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"trpaddfr",	rowProps->m_nDefCellMarRightUnits,	sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"trpaddft",	rowProps->m_nDefCellMarTopUnits,	sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"trspdb",		rowProps->m_nDefCellSpBottom,		sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"trspdl",		rowProps->m_nDefCellSpLeft,			sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"trspdr",		rowProps->m_nDefCellSpRight,		sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"trspdt",		rowProps->m_nDefCellSpTop,			sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"trspdfb",	rowProps->m_nDefCellSpBottomUnits,	sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"trspdfl",	rowProps->m_nDefCellSpLeftUnits,	sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"trspdfr",	rowProps->m_nDefCellSpRightUnits,	sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"trspdft",	rowProps->m_nDefCellSpTopUnits,		sCommand, hasParameter, parameter )
	
	COMMAND_RTF_INT( L"ts",			rowProps->m_nStyle,					sCommand, hasParameter, parameter )

	COMMAND_RTF_INT( L"tbllkhdrrows",	rowProps->m_bAutoFirstRow,		sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"tbllklastrow",	rowProps->m_bAutoLastRow,		sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"tbllkhdrcols",	rowProps->m_bAutoFirstCol,		sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"tbllklastcol",	rowProps->m_bAutoLastCol,		sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"tbllknorowband", rowProps->m_bAutoNoRowBand,		sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"tbllknocolband", rowProps->m_bAutoNoColBand,		sCommand, hasParameter, parameter )

	COMMAND_RTF_INT( L"tscbandsh",		rowProps->m_nRowBandSize,		sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"tscbandsv",		rowProps->m_nColBandSize,		sCommand, hasParameter, parameter )	
	else
		return false;
	
	return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------
bool OldShapeReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
{
	if ( L"do" == sCommand )
		return true;
	else if ( L"doinst" == sCommand )
		return true;
	else if ( L"dorslt" == sCommand )
		return false;
	else if ( L"picprop" == sCommand )
		return true;
//-------------------------------------------------------- type primitives
	else if ( L"dprect"		== sCommand)	m_oShape.m_nShapeType = NSOfficeDrawing::sptRectangle;
	else if ( L"dpline"		== sCommand)	m_oShape.m_nShapeType = NSOfficeDrawing::sptLine;
	else if ( L"dpellipse"	== sCommand)	m_oShape.m_nShapeType = NSOfficeDrawing::sptEllipse;
	else if ( L"dparc"		== sCommand)	m_oShape.m_nShapeType = NSOfficeDrawing::sptArc;
	else if ( L"dppolyline" == sCommand)	m_oShape.m_nShapeType = NSOfficeDrawing::sptNotPrimitive;
	else if ( L"dppolygon"	== sCommand)	m_oShape.m_nShapeType = NSOfficeDrawing::sptNotPrimitive;
	else if ( L"dpcallout"	== sCommand)	m_oShape.m_nShapeType = NSOfficeDrawing::sptTextBox;
	else if ( L"dptxbx"		== sCommand)	m_oShape.m_nShapeType = NSOfficeDrawing::sptTextBox;
	else if ( L"dproundr"	== sCommand)	m_oShape.m_nShapeType = NSOfficeDrawing::sptRoundRectangle;
	else if ( L"dptxbxtext" == sCommand )
	{
		if ( PROP_DEF == m_oShape.m_nShapeType )
			m_oShape.m_nShapeType = NSOfficeDrawing::sptTextBox;
		
		ParagraphReader oParagraphReader(L"shptxt", oReader);
		StartSubReader( oParagraphReader, oDocument, oReader );
		m_oShape.m_aTextItems = oParagraphReader.m_oParPropDest.m_oTextItems;
	}
	//else if ( L"shpbxignore"	== sCommand )	m_oShape.m_eXAnchor		= RtfShape::ax_ignore;
	else if ( L"dobxpage"		== sCommand )	m_oShape.m_eXAnchor		= RtfShape::ax_page;
	else if ( L"dobxmargin"		== sCommand )	m_oShape.m_eXAnchor		= RtfShape::ax_margin;
	else if ( L"dobxcolumn"		== sCommand )	m_oShape.m_eXAnchor		= RtfShape::ax_column;
	//else if ( L"shpbyignore"	== sCommand )	m_oShape.m_eYAnchor		= RtfShape::ay_ignore;
	else if ( L"dobypage"		== sCommand )	m_oShape.m_eYAnchor		= RtfShape::ay_page;
	else if ( L"dobymargin"		== sCommand )	m_oShape.m_eYAnchor		= RtfShape::ay_margin;
	else if ( L"dobypara"		== sCommand )	m_oShape.m_eYAnchor		= RtfShape::ay_Para;
	else if ( L"dolockanchor"	== sCommand )	m_oShape.m_bLockAnchor	= true;
	else if ( L"dplinehollow"	== sCommand )	m_oShape.m_bLine		= false;

	else if ( hasParameter)
	{
		if		( L"dpx"		== sCommand )	m_oShape.m_nLeft			= parameter;
		else if ( L"dpx"		== sCommand )	m_oShape.m_nLeft			= parameter;
		else if ( L"dpy"		== sCommand )	m_oShape.m_nTop				= parameter;
		else if ( L"dpysize"	== sCommand )	m_oShape.m_nBottom			= parameter + m_oShape.m_nTop;
		else if ( L"dpxsize"	== sCommand )	m_oShape.m_nRight			= parameter + m_oShape.m_nLeft;
		else if ( L"doz"		== sCommand )	m_oShape.m_nZOrder			= parameter;
		else if ( L"dofhdr"		== sCommand )	m_oShape.m_nHeader			= parameter;
		else if ( L"dowr"		== sCommand )	m_oShape.m_nWrapType		= parameter;
		else if ( L"dowrk"		== sCommand )	m_oShape.m_nWrapSideType	= parameter;
		else if ( L"dofblwtxt"	== sCommand)	m_oShape.m_nZOrderRelative	= parameter;		
		else if ( L"dplinew"	== sCommand )	m_oShape.m_nLineWidth		= parameter;	
		else if ( L"dodhgt"		== sCommand )	m_oShape.m_nZOrder			= parameter;	
		else if ( L"dpfillpat"	== sCommand )
		{
			m_oShape.m_nFillType = parameter;	
			if (m_oShape.m_nFillType == 0) m_oShape.m_bFilled = false;
		}
	}
	else
	{
		return false;
	}
	return true;
}
bool ShapeReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
{
	if ( L"shp" == sCommand )
		return true;
	else if ( L"shpinst" == sCommand )
		return true;
	else if ( L"shprslt" == sCommand )
		return false;
	else if ( L"picprop" == sCommand )
		return true;
	else if ( L"shptxt" == sCommand )
	{
		if ( PROP_DEF == m_oShape.m_nShapeType )
			m_oShape.m_nShapeType = NSOfficeDrawing::sptTextBox;
		
		ParagraphReader oParagraphReader(L"shptxt", oReader);
		StartSubReader( oParagraphReader, oDocument, oReader );
		m_oShape.m_aTextItems = oParagraphReader.m_oParPropDest.m_oTextItems;
	}
	else if ( L"sp" == sCommand )
	{
		ShapePropertyReader oShapePropReader(m_oShape);
		StartSubReader(oShapePropReader, oDocument, oReader);
	}
	else if ( L"shpleft" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_nLeft = parameter;
	}
	else if ( L"shptop" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_nTop = parameter;
	}
	else if ( L"shpbottom" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_nBottom = parameter;
	}
	else if ( L"shpright" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_nRight = parameter;
	}
	else if ( L"shplid" == sCommand )
	{
		if ( hasParameter )
		{
			m_oShape.m_nID = parameter;
			oDocument.SetShapeId( parameter );
		}
	}
	else if ( L"shpz" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_nZOrder = parameter;
	}
	else if ( L"shpfhdr" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_nHeader = parameter;
	}
	//else if ( L"shpbxignore" == sCommand )	m_oShape.m_eXAnchor = RtfShape::ax_ignore;
	else if ( L"shpbxpage" == sCommand )		m_oShape.m_eXAnchor = RtfShape::ax_page;
	else if ( L"shpbxmargin" == sCommand )		m_oShape.m_eXAnchor = RtfShape::ax_margin;
	else if ( L"shpbxcolumn" == sCommand )		m_oShape.m_eXAnchor = RtfShape::ax_column;
	//else if ( L"shpbyignore" == sCommand )	m_oShape.m_eYAnchor = RtfShape::ay_ignore;
	else if ( L"shpbypage" == sCommand )		m_oShape.m_eYAnchor = RtfShape::ay_page;
	else if ( L"shpbymargin" == sCommand )		m_oShape.m_eYAnchor = RtfShape::ay_margin;
	else if ( L"shpbypara" == sCommand )		m_oShape.m_eYAnchor = RtfShape::ay_Para;
	else if ( L"shpwr" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_nWrapType = parameter;
	}
	else if ( L"shpwrk" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_nWrapSideType = parameter;
	}
	else if ( L"shpfblwtxt" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_nZOrderRelative = parameter;
	}
	else if ( L"shplockanchor" == sCommand )
		m_oShape.m_bLockAnchor = true;
	else
		return false;
	return true;
}
bool PictureReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
{
	if ( L"pict" == sCommand )
		return true;
	else if ( L"picprop" == sCommand )
	{
		ShapeReader oShapeReader(m_oShape);
		StartSubReader( oShapeReader, oDocument, oReader );
	}
	else if ( L"emfblip" == sCommand )	m_oShape.m_oPicture->eDataType = RtfPicture::dt_emf;
	else if ( L"pngblip" == sCommand )	m_oShape.m_oPicture->eDataType = RtfPicture::dt_png;
	else if ( L"jpegblip" == sCommand )	m_oShape.m_oPicture->eDataType = RtfPicture::dt_jpg;
	else if ( L"macpict" == sCommand )	m_oShape.m_oPicture->eDataType = RtfPicture::dt_macpict;

	else if ( L"wmetafile" == sCommand )
	{
		if ( hasParameter && 8 == parameter )
		m_oShape.m_oPicture->eDataType = RtfPicture::dt_wmf;
	}
	else if ( L"picw" == sCommand )
	{
		if ( hasParameter )
		{
			m_oShape.m_oPicture->m_nWidth = parameter;
		}
	}
	else if ( L"pich" == sCommand )
	{
		if ( hasParameter )
		{
			m_oShape.m_oPicture->m_nHeight = parameter;
		}
	}
	else if ( L"picwgoal" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_oPicture->m_nWidthGoal = parameter;
	}	
	else if ( L"pichgoal" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_oPicture->m_nHeightGoal = parameter;
	}
	else if ( L"picscalex" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_oPicture->m_dScaleX = parameter;
	}
	else if ( L"picscaley" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_oPicture->m_dScaleY = parameter;
	}
	else if ( L"picscaled" == sCommand )
			m_oShape.m_oPicture->m_bScaled = 1;
	else if ( L"piccropl" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_oPicture->m_nCropL = parameter;
	}
	else if ( L"piccropt" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_oPicture->m_nCropT = parameter;
	}
	else if ( L"piccropr" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_oPicture->m_nCropR = parameter;
	}
	else if ( L"piccropb" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_oPicture->m_nCropB = parameter;
	}
	else if ( L"bin" == sCommand)
	{
		if ( hasParameter )
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
	if ( L"field" == sCommand )
		return true;
	else if ( L"flddirty" == sCommand )
		m_oField.m_eMode = RtfField::fm_flddirty;
	else if ( L"fldedit" == sCommand )
		m_oField.m_eMode = RtfField::fm_fldedit;
	else if ( L"fldlock" == sCommand )
		m_oField.m_eMode = RtfField::fm_fldlock;
	else if ( L"fldpriv" == sCommand )
		m_oField.m_eMode = RtfField::fm_fldpriv;
	else if ( L"fldalt" == sCommand )
		m_oField.m_bReferenceToEndnote = true;

	else if ( L"fldinst" == sCommand )
	{
		ParagraphReader oParagraphReader( L"fldinst", oReader );
		StartSubReader( oParagraphReader, oDocument, oReader );
		m_oField.m_oInsert = oParagraphReader.m_oParPropDest.m_oTextItems;
	}
	else if ( L"datafield" == sCommand )
		Skip( oDocument, oReader );
	else if ( L"fldrslt" == sCommand )
	{
		ParagraphReader oParagraphReader( L"fldrslt", oReader );
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
	if ( L"object" == sCommand )
		return true;
	
	COMMAND_RTF_INT( L"objw", m_oOle.m_nWidth, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"objh", m_oOle.m_nHeight, sCommand, hasParameter, parameter )
	COMMAND_RTF_INT( L"objemb", m_oOle.m_eOleType, sCommand, true, RtfOle::ot_emb )
	COMMAND_RTF_INT( L"objlink", m_oOle.m_eOleType, sCommand, true, RtfOle::ot_link )
	else if ( L"objclass" == sCommand )
	{
		TextReader oTextReader( m_oOle.m_sOleClass, false );
		StartSubReader( oTextReader, oDocument, oReader );
	}
	else if ( L"objdata" == sCommand )
	{
		CString sOleData;
		TextReader oTextReader( sOleData, false );
		StartSubReader( oTextReader, oDocument, oReader );

		BYTE *pData = NULL;
		long nSize = 0;
		RtfUtility::WriteDataToBinary( sOleData, &pData, nSize );
		if ( 0 != nSize  && pData)
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
			if ( SUCCEEDED( StgCreateDocfile( sOleStorageName, STGM_READWRITE | STGM_CREATE | STGM_SHARE_EXCLUSIVE | STGM_TRANSACTED/* | STGM_DELETEONRELEASE*/, NULL, &piMSStorage) ) )
			{
				hRes = OleConvertOLESTREAMToIStorage( &oStream, piMSStorage, NULL );
				piMSStorage->Commit( STGC_DEFAULT );
				RELEASEINTERFACE( piMSStorage );
			}			
			delete oStream.lpstbl;
#else
			std::wstring sOleStorageName = FileSystem::Directory::CreateTempFileWithUniqueName(oReader.m_sTempFolder, L"img");

			hRes = ConvertOle1ToOle2(pData, nSize, sOleStorageName);
			
#endif
			delete[] pData;
			
			POLE::Storage * piRootStorage = new POLE::Storage(sOleStorageName.c_str());			
			if ( piRootStorage)
			{
				m_oOle.SetFilename( sOleStorageName.c_str() );
				m_oOle.SetOle( piRootStorage );
				hRes = S_OK;
			}

            if (  hRes != S_OK )
				Utils::RemoveDirOrFile( sOleStorageName.c_str() );
		}
	}
	else if ( L"result" == sCommand )
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
void ShapeReader::ShapePropertyReader::ShapePropertyValueReader::PopState( RtfDocument& oDocument, RtfReader& oReader )
{
	RtfAbstractReader::PopState( oDocument, oReader );
	if ( L"" == m_sPropValue ) 
		return;
	CString sValue = m_sPropValue;

	if ( L"wzName" == m_sPropName )
	{
		m_oShape.m_sName = sValue;
		return;
	}
	else if ( L"pibName" == m_sPropName )
	{
		return;
	}
	else if ( L"wzDescription" == m_sPropName )
	{
		m_oShape.m_sDescription = sValue;
		return;
	}
	else if ( L"gtextUNICODE" == m_sPropName )
	{
		m_oShape.m_sGtextUNICODE = sValue;
		return;
	}
	else if ( L"gtextFont" == m_sPropName )
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

	if ( L"shapeType"				== m_sPropName ) m_oShape.m_nShapeType			= nValue;
//Position absolute
	else if ( L"posh"				== m_sPropName ) m_oShape.m_nPositionH			= nValue;
	else if ( L"posrelh"			== m_sPropName ) m_oShape.m_nPositionHRelative	= nValue;
	else if ( L"posv"				== m_sPropName ) m_oShape.m_nPositionV			= nValue;
	else if ( L"posrelv"			== m_sPropName ) m_oShape.m_nPositionVRelative	= nValue;
	else if ( L"fLayoutInCell"		== m_sPropName ) m_oShape.m_bLayoutInCell		= nValue;
	else if ( L"fAllowOverlap"		== m_sPropName ) m_oShape.m_bAllowOverlap		= nValue;
//Position relative
	else if ( L"pctHorizPos"			== m_sPropName ) m_oShape.m_nPositionHPct		= nValue;
	else if ( L"pctVertPos"			== m_sPropName ) m_oShape.m_nPositionVPct		= nValue;
	else if ( L"pctHoriz"			== m_sPropName ) m_oShape.m_nPctWidth			= nValue;
	else if ( L"pctVert"			== m_sPropName ) m_oShape.m_nPctHeight			= nValue;
	else if ( L"sizerelh"			== m_sPropName ) m_oShape.m_nPctWidthRelative	= nValue;
	else if ( L"sizerelv"			== m_sPropName ) m_oShape.m_nPctHeightRelative	= nValue;
	else if ( L"colStart"			== m_sPropName ) m_oShape.m_nColStart			= nValue;
//Rehydration
	else if ( L"metroBlob"			== m_sPropName ) m_oShape.m_sMetroBlob			= sValue;
//Object Type
	else if ( L"fIsBullet"			== m_sPropName ) m_oShape.m_bIsBullet			= nValue;
	else if ( L"rotation"				== m_sPropName ) m_oShape.m_nRotation			= nValue;
	else if ( L"fFlipV"				== m_sPropName ) m_oShape.m_bFlipV				= nValue;
	else if ( L"fFlipH"				== m_sPropName ) m_oShape.m_bFlipH				= nValue;
	else if ( L"shapeType"			== m_sPropName ) m_oShape.m_nShapeType			= nValue;
//custom
	else if ( L"shapePath"			== m_sPropName ) m_oShape.m_nShapePath			= nValue;
	else if ( L"pWrapPolygonVertices" == m_sPropName )
	{
		std::wstring sValue_(sValue.GetBuffer());
		std::vector< std::wstring > splitted;
	    
		boost::algorithm::split(splitted, sValue_, boost::algorithm::is_any_of(L";"), boost::algorithm::token_compress_on);
		
		for (int i = 2 ; i < splitted.size(); i++)
		{
			boost::algorithm::replace_all(splitted[i], L")", L"");
			boost::algorithm::replace_all(splitted[i], L"(", L"");
			int pos = splitted[i].find(L",");

            int x = 0, y = 0;
            try
            {
                x = _wtoi(splitted[i].substr(0, pos).c_str());
            }
            catch(...){}
            try
            {
                y = _wtoi(splitted[i].substr(pos + 1, splitted[i].length() - 1).c_str());
            }
            catch(...){}

			m_oShape.m_aWrapPoints.push_back( std::pair<int, int>(x, y) );
		}
	}
	else if ( L"pVerticies"		== m_sPropName )
	{
		std::wstring sValue_(sValue.GetBuffer());
		std::vector< std::wstring > splitted;
	    
		boost::algorithm::split(splitted, sValue_, boost::algorithm::is_any_of(L";"), boost::algorithm::token_compress_on);
		
		for (int i = 2 ; i < splitted.size(); i++)
		{
			boost::algorithm::replace_all(splitted[i], L")", L"");
			boost::algorithm::replace_all(splitted[i], L"(", L"");
			int pos = splitted[i].find(L",");

            int x = 0, y = 0;
            try
            {
                x = _wtoi(splitted[i].substr(0, pos).c_str());
            }
            catch(...){}
            try
            {
                y = _wtoi(splitted[i].substr(pos + 1, splitted[i].length() - 1).c_str());
            }
            catch(...){}

			m_oShape.m_aPVerticles.push_back( std::pair<int, int>(x, y) );
		}
	}	
	else if ( L"pSegmentInfo" == m_sPropName )
	{
		std::wstring sValue_(sValue.GetBuffer());
		std::vector< std::wstring > splitted;
	    
		boost::algorithm::split(splitted, sValue_, boost::algorithm::is_any_of(L";"), boost::algorithm::token_compress_on);
		
		for (int i = 2 ; i < splitted.size(); i++)
		{
            int val = 0;
            try
            {
                val = _wtoi(splitted[i].c_str());
            }catch(...){}

			m_oShape.m_aPSegmentInfo.push_back( val );
		}
	}	
	else if ( L"geoBottom"		== m_sPropName ) m_oShape.m_nGeoBottom		= nValue;
	else if ( L"geoLeft"		== m_sPropName ) m_oShape.m_nGeoLeft		= nValue;
	else if ( L"geoRight"		== m_sPropName ) m_oShape.m_nGeoRight		= nValue;
	else if ( L"geoTop"			== m_sPropName ) m_oShape.m_nGeoTop			= nValue;
//
	else if ( L"dxWrapDistLeft"	== m_sPropName ) m_oShape.m_nWrapDistLeft	= RtfUtility::Emu2Twips( nValue );
	else if ( L"dyWrapDistTop"	== m_sPropName ) m_oShape.m_nWrapDistTop	= RtfUtility::Emu2Twips( nValue );
	else if ( L"dxWrapDistRight"== m_sPropName ) m_oShape.m_nWrapDistRight	= RtfUtility::Emu2Twips( nValue );
	else if ( L"dyWrapDistBottom" == m_sPropName ) m_oShape.m_nWrapDistBottom = RtfUtility::Emu2Twips( nValue );
	else if ( L"fBehindDocument"== m_sPropName ) m_oShape.m_nZOrderRelative = nValue;
	else if ( L"fHidden"		== m_sPropName ) m_oShape.m_bHidden			= nValue;
//Text box
	else if ( L"dxTextLeft"		== m_sPropName ) m_oShape.m_nTexpLeft		= nValue;
	else if ( L"dyTextTop"		== m_sPropName ) m_oShape.m_nTexpTop		= nValue;
	else if ( L"dxTextRight"	== m_sPropName ) m_oShape.m_nTexpRight		= nValue;
	else if ( L"dyTextBottom"	== m_sPropName ) m_oShape.m_nTexpBottom		= nValue;
	else if ( L"anchorText"		== m_sPropName ) m_oShape.m_nAnchorText		= nValue;

//Geometry
	else if ( L"adjustValue"	== m_sPropName ) m_oShape.m_nAdjustValue[0]	= nValue;
	else if ( L"adjust2Value"	== m_sPropName ) m_oShape.m_nAdjustValue[1]	= nValue;
	else if ( L"adjust3Value"	== m_sPropName ) m_oShape.m_nAdjustValue[2]	= nValue;
	else if ( L"adjust4Value"	== m_sPropName ) m_oShape.m_nAdjustValue[3]	= nValue;
	else if ( L"adjust5Value"	== m_sPropName ) m_oShape.m_nAdjustValue[4]	= nValue;
	else if ( L"adjust6Value"	== m_sPropName ) m_oShape.m_nAdjustValue[5]	= nValue;
	else if ( L"adjust7Value"	== m_sPropName ) m_oShape.m_nAdjustValue[6]	= nValue;
	else if ( L"adjust8Value"	== m_sPropName ) m_oShape.m_nAdjustValue[7]	= nValue;
	else if ( L"adjust9Value"	== m_sPropName ) m_oShape.m_nAdjustValue[8]	= nValue;
	else if ( L"adjust10Value"	== m_sPropName ) m_oShape.m_nAdjustValue[9]	= nValue;
//WordArt Effects
	else if ( L"cropFromTop"	== m_sPropName ) m_oShape.m_nCropFromTop	= nValue;
	else if ( L"cropFromBottom"	== m_sPropName ) m_oShape.m_nCropFromBottom = nValue;
	else if ( L"cropFromLeft"	== m_sPropName ) m_oShape.m_nCropFromRight	= nValue;
	else if ( L"cropFromRight"	== m_sPropName ) m_oShape.m_nCropFromTop	= nValue;
//Grouped Shapes
	else if ( L"groupBottom"	== m_sPropName ) m_oShape.m_nGroupBottom	= nValue;
	else if ( L"groupLeft"		== m_sPropName ) m_oShape.m_nGroupLeft		= nValue;
	else if ( L"groupRight"		== m_sPropName ) m_oShape.m_nGroupRight		= nValue;
	else if ( L"groupTop"		== m_sPropName ) m_oShape.m_nGroupTop		= nValue;

	else if ( L"relBottom"		== m_sPropName ) m_oShape.m_nRelBottom		= nValue;
	else if ( L"relLeft"		== m_sPropName ) m_oShape.m_nRelLeft		= nValue;
	else if ( L"relRight"		== m_sPropName ) m_oShape.m_nRelRight		= nValue;
	else if ( L"relTop"			== m_sPropName ) m_oShape.m_nRelTop			= nValue;
	else if ( L"relRotation"	== m_sPropName ) m_oShape.m_nRelRotation	= nValue;

	else if ( L"dhgt"			== m_sPropName ) m_oShape.m_nRelZOrder		= nValue;
//Fill
	else if ( L"fFilled"		== m_sPropName ) m_oShape.m_bFilled			= (0 == nValue ? false : true );
	else if ( L"fillType"		== m_sPropName ) m_oShape.m_nFillType		= nValue;
	else if ( L"fillColor"		== m_sPropName ) m_oShape.m_nFillColor		= nValue;
	else if ( L"fillBackColor"	== m_sPropName ) m_oShape.m_nFillColor2		= nValue;
	else if ( L"fillOpacity"	== m_sPropName ) m_oShape.m_nFillOpacity	= nValue * 100 / 65536;
	else if ( L"fillAngle"		== m_sPropName ) m_oShape.m_nFillAngle		= nValue / 65536;
	else if ( L"fillFocus"		== m_sPropName ) m_oShape.m_nFillFocus		= nValue;

	else if ( L"fGtext"			== m_sPropName ) m_oShape.m_bGtext			= nValue;
	else if ( L"gtextSize"		== m_sPropName ) m_oShape.m_nGtextSize		= nValue;

//Line
	else if ( L"fLine"					== m_sPropName ) m_oShape.m_bLine					= ( 0 == nValue ? false : true );
	else if ( L"lineStartArrowhead"		== m_sPropName ) m_oShape.m_nLineStartArrow			= nValue;
	else if ( L"lineColor"				== m_sPropName ) m_oShape.m_nLineColor				= nValue;
	else if ( L"lineStartArrowWidth"	== m_sPropName ) m_oShape.m_nLineStartArrowWidth= nValue;
	else if ( L"lineStartArrowLength"	== m_sPropName ) m_oShape.m_nLineStartArrowLength	= nValue;
	else if ( L"lineEndArrowhead"		== m_sPropName ) m_oShape.m_nLineEndArrow		= nValue;
	else if ( L"lineEndArrowWidth"		== m_sPropName ) m_oShape.m_nLineEndArrowWidth		= nValue;
	else if ( L"lineEndArrowLength"		== m_sPropName ) m_oShape.m_nLineEndArrowLength		= nValue;
	else if ( L"lineWidth"				== m_sPropName ) m_oShape.m_nLineWidth				= nValue;
	else if ( L"lineDashing"			== m_sPropName ) m_oShape.m_nLineDashing		= nValue;
	else if (L"cxstyle"					== m_sPropName ) m_oShape.m_nConnectorStyle			= nValue;
	else if (L"cxk"						== m_sPropName ) m_oShape.m_nConnectionType			= nValue;
	else
	{
		std::wstring name	= m_sPropName.GetBuffer();
		int val				= nValue;
	}
}

bool TrackerChangesReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, CString sCommand, bool hasParameter, int parameter)
{
	if ( L"oldcprops" == sCommand )
		return true;
	if ( L"oldpprops" == sCommand )
		return true;
	if ( L"oldsprops" == sCommand )
		return true;
	if ( L"oldtpprops" == sCommand )
		return true;
	
	else if ( L"chbrdr"	== sCommand )		m_eInternalState = is_charBorder;
	else if ( L"brdrt"	== sCommand )		m_eInternalState = is_borderTop;
	else if ( L"brdrb"	== sCommand )		m_eInternalState = is_borderBottom;
	else if ( L"brdrl"	== sCommand )		m_eInternalState = is_borderLeft;
	else if ( L"brdrr"	== sCommand )		m_eInternalState = is_borderRight;
	//else if ( L"brdrbtw" == sCommand )	m_eInternalState = is_borderRight;
	else if ( L"brdrbar" == sCommand )		m_eInternalState = is_borderBar;
	else if ( L"box"	== sCommand )		m_eInternalState = is_borderBox;

	else if ( L"cldglu" == sCommand )		m_eInternalState = is_borderCellLR;
	else if ( L"cldgll" == sCommand )		m_eInternalState = is_borderCellRL;
	else if ( L"clbrdrl" == sCommand )		m_eInternalState = is_borderCellLeft;
	else if ( L"clbrdrt" == sCommand )		m_eInternalState = is_borderCellTop;
	else if ( L"clbrdrr" == sCommand )		m_eInternalState = is_borderCellRight;
	else if ( L"clbrdrb" == sCommand )		m_eInternalState = is_borderCellBottom;

	else if ( L"tsbrdrdgl" == sCommand )	m_eInternalState = is_borderCellLR;
	else if ( L"tsbrdrdgr" == sCommand )	m_eInternalState = is_borderCellRL;

	else if ( L"trbrdrl" == sCommand )		m_eInternalState = is_borderRowLeft;
	else if ( L"trbrdrr" == sCommand )		m_eInternalState = is_borderRowRight;
	else if ( L"trbrdrt" == sCommand )		m_eInternalState = is_borderRowTop;
	else if ( L"trbrdrb" == sCommand )		m_eInternalState = is_borderRowBottom;
	else if ( L"trbrdrv" == sCommand )		m_eInternalState = is_borderRowVer;
	else if ( L"trbrdrh" == sCommand )		m_eInternalState = is_borderRowHor;
	else if ( L"tsbrdrh" == sCommand ) 		m_eInternalState = is_borderRowHor;
	else if ( L"tsbrdrv" == sCommand )		m_eInternalState = is_borderRowVer;
	else if ( L"tsbrdrl" == sCommand )		m_eInternalState = is_borderRowLeft;
	else if ( L"tsbrdrt" == sCommand )		m_eInternalState = is_borderRowTop;
	else if ( L"tsbrdrr" == sCommand )		m_eInternalState = is_borderRowRight;
	else if ( L"tsbrdrb" == sCommand )		m_eInternalState = is_borderRowBottom;
	else
	{
		if (m_pCharProps)
		{
			return RtfCharPropsCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, m_pCharProps.get() );
		}
		
		if (m_pParagraphProps)
		{
			bool bResult = false;
			switch(m_eInternalState)
			{
			case is_borderBar:
				bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, m_pParagraphProps->m_oBorderBar );
				break;
			case is_borderBottom:
				bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, m_pParagraphProps->m_oBorderBottom );
				break;
			case is_borderBox:
				bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, m_pParagraphProps->m_oBorderBox );
				break;
			case is_borderLeft:
				bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, m_pParagraphProps->m_oBorderLeft );
				break;
			case is_borderRight:
				bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, m_pParagraphProps->m_oBorderRight );
				break;
			case is_borderTop:
				bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, m_pParagraphProps->m_oBorderTop );
				break;
			}
			if (bResult) return true;

			return RtfParagraphPropsCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, m_pParagraphProps.get() );
		}
			
		//if (m_pTableCellProps)
		//	return RtfTableCellPropsCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, m_pTableCellProps.get() );

		if (m_pTableRowProps)
		{
			bool bResult = false;
			switch(m_eInternalState)
			{
			case is_borderRowBottom :
				bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, m_pTableRowProps->m_oBorderBottom );
				break;
			case is_borderRowHor :
				bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter,m_pTableRowProps->m_oBorderHor );
				break;
			case is_borderRowLeft :
				bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, m_pTableRowProps->m_oBorderLeft );
				break;
			case is_borderRowRight :
				bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, m_pTableRowProps->m_oBorderRight );
				break;
			case is_borderRowTop :
				bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, m_pTableRowProps->m_oBorderTop );
				break;
			case is_borderRowVer :
				bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, m_pTableRowProps->m_oBorderVert );
				break;
			}
			if ( bResult )	return true;
			return RtfTableRowPropsCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, m_pTableRowProps.get() );
		}
	}
	return true;
}
bool RtfOldListReader::ExecuteCommand( RtfDocument& oDocument, RtfReader& oReader, CString sKey, bool bHasPar, int nPar )
{
	if ( L"pn" == sKey )
		return true;
	else if ( L"pnlvlblt" == sKey )
	{
		m_oTarget.m_eLevelType = RtfOldList::lt_blt;
		m_oTarget.m_nIlvl = 0;
	}
	COMMAND_RTF_INT	( L"pnf",		oReader.m_oState->m_oCharProp.m_nFont,		sKey, bHasPar, nPar )
	COMMAND_RTF_INT	( L"pnfs",		oReader.m_oState->m_oCharProp.m_nFontSize,	sKey, bHasPar, nPar )
	COMMAND_RTF_BOOL( L"pnb",		oReader.m_oState->m_oCharProp.m_bBold,		sKey, bHasPar, nPar )
	COMMAND_RTF_BOOL( L"pni",		oReader.m_oState->m_oCharProp.m_bItalic,	sKey, bHasPar, nPar )
	COMMAND_RTF_BOOL( L"pncaps",	oReader.m_oState->m_oCharProp.m_bCaps,		sKey, bHasPar, nPar )
	COMMAND_RTF_BOOL( L"pnstrike",	oReader.m_oState->m_oCharProp.m_bStrike,	sKey, bHasPar, nPar )
	COMMAND_RTF_INT	( L"pnul",		oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_Single)
	COMMAND_RTF_INT	( L"pnuld",		oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_Dashed)
	COMMAND_RTF_INT	( L"pnuldash",	oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_Dash_dotted)
	COMMAND_RTF_INT	( L"pnuldashdd", oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_Dash_dot_dotted)
	COMMAND_RTF_INT	( L"pnulth",	oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_Thick)
	COMMAND_RTF_INT	( L"pnulwave",	oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_Wave)
	COMMAND_RTF_INT	( L"pnuldb",	oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_Double)
	COMMAND_RTF_INT	( L"pnulnone",	oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_none)
	COMMAND_RTF_INT	( L"pnulnone",	oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_Word)

	COMMAND_RTF_INT( L"pnindent",	m_oTarget.m_oLevelText->m_oProperty.m_nIndLeft, sKey, bHasPar, nPar )
	COMMAND_RTF_INT( L"pnsp",		m_oTarget.m_oLevelText->m_oProperty.m_nIndLeft, sKey, bHasPar, nPar )
	COMMAND_RTF_INT( L"pnstart",	m_oTarget.m_oLevelText->m_oProperty.m_nIndLeft, sKey, bHasPar, nPar )
	COMMAND_RTF_INT( L"pnhang",		m_oTarget.m_oLevelText->m_oProperty.m_nIndLeft, sKey, bHasPar, nPar )

	COMMAND_RTF_INT( L"pnqc",	m_oTarget.m_eLevelJust, sKey, true, RtfOldList::lj_center )
	COMMAND_RTF_INT( L"pnql",	m_oTarget.m_eLevelJust, sKey, true, RtfOldList::lj_left )
	COMMAND_RTF_INT( L"pnqr",	m_oTarget.m_eLevelJust, sKey, true, RtfOldList::lj_right )

	else if ( L"pntxtb" == sKey )
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

//-------------------------------------------------------------------------------------------------------------------------------------------
void ParagraphPropDestination::AddItem( RtfParagraphPtr oItem, RtfReader& oReader, bool bEndCell, bool bEndRow )
{
	 // 1 != oItem->m_oProperty.m_bInTable - параграф не в таблице
	 // PROP_DEF != nTargetItap && oItem->m_oProperty.m_nItap <= nTargetItap - выставлено свойство,что вложенность - nTargetItap - это не таблица( Нужно для чтения параграфов в таблицах )
	if ( 1 != oItem->m_oProperty.m_bInTable || ( PROP_DEF != nTargetItap && oItem->m_oProperty.m_nItap <= nTargetItap ) )
	{
		if ( nCurItap > 0 ) //Если до этого были только параграфы в таблицах - завершаем таблицу
		{
			RtfTablePtr oNewTable( new RtfTable() );
			oNewTable->m_oProperty = oCurRowProperty;

			for( int k = (int)aRows.size() - 1; k >= 0 ; k-- )
			{
				if ( aRowItaps[k] == nCurItap )
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
		if ( nCurItap > oItem->m_oProperty.m_nItap )
		{
			RtfTablePtr oNewTable( new RtfTable() );
			oNewTable->m_oProperty = oCurRowProperty;
			
			for( int k = (int)aRows.size() - 1; k >= 0 ; k-- )
			{
				if ( aRowItaps[k] == nCurItap )
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
		if ( bEndRow )
		{
			RtfTableRowPtr oNewTableRow( new RtfTableRow() );
			oNewTableRow->m_oProperty = oReader.m_oState->m_oRowProperty;

			for( int k = (int)aCells.size() - 1; k >= 0 ; k-- )
			{
				if ( aCellItaps[k] == nCurItap )
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

			if ( bEndCell )
			{
				RtfTableCellPtr oNewTableCell( new RtfTableCell() );
				for( int k = (int)aCellRenderables.size() - 1; k >= 0 ; k-- )
				{
					if ( aItaps[k] == nCurItap )
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
	if ( false == m_bPar )// потому что это не reader и нужно как-то загонять последний параграф
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


bool ParagraphPropDestination::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, RtfAbstractReader& oAbstrReader, CString sCommand, bool hasParameter, int parameter)
{
	m_oReader		= &oReader;
	m_bPar			= false;
	bool bContinue	= false;

		 if ( L"pard"			== sCommand )	oReader.m_oState->m_oCurOldList.SetDefault();
	else if ( L"trowd"			== sCommand )	oReader.m_oState->m_oRowProperty.SetDefaultRtf();
	else if ( L"tcelld"			== sCommand )	oReader.m_oState->m_oCellProperty.SetDefaultRtf();
	else if ( L"nesttableprops" == sCommand )	oReader.m_oState->m_oRowProperty.SetDefaultRtf();

//----------------------------------------------------------------------------------
	if ( L"par" == sCommand )
	{
		m_bPar = true;
		m_oCurParagraph->m_oProperty	= oReader.m_oState->m_oParagraphProp;
		m_oCurParagraph->m_oOldList		= RtfOldListPtr( new RtfOldList() );
		*m_oCurParagraph->m_oOldList	= oReader.m_oState->m_oCurOldList;
		m_oCurParagraph->m_oProperty.m_oCharProperty = oReader.m_oState->m_oCharProp;

		AddItem( m_oCurParagraph, oReader, false, false );
		m_oCurParagraph = RtfParagraphPtr(new RtfParagraph());
	}
	else if ( L"cell" == sCommand  || L"nestcell" == sCommand )
	{
		m_oCurParagraph->m_oProperty	= oReader.m_oState->m_oParagraphProp;
		m_oCurParagraph->m_oOldList		= RtfOldListPtr( new RtfOldList() );
		*m_oCurParagraph->m_oOldList	= oReader.m_oState->m_oCurOldList;

		m_oCurParagraph->m_oProperty.m_oCharProperty = oReader.m_oState->m_oCharProp;
		AddItem( m_oCurParagraph, oReader, true, false );
		m_oCurParagraph = RtfParagraphPtr(new RtfParagraph());
	}
    else if ( L"row" == sCommand || L"nestrow" == sCommand)
	{
		m_oCurParagraph->m_oProperty	= oReader.m_oState->m_oParagraphProp;
		m_oCurParagraph->m_oOldList		= RtfOldListPtr( new RtfOldList() );
		*m_oCurParagraph->m_oOldList	= oReader.m_oState->m_oCurOldList;

		m_oCurParagraph->m_oProperty.m_oCharProperty = oReader.m_oState->m_oCharProp;
		AddItem( m_oCurParagraph, oReader, false, true );
		m_oCurParagraph = RtfParagraphPtr(new RtfParagraph());
	}
	else if ( L"cellx" == sCommand )
	{
		if ( hasParameter)
		{
			oReader.m_oState->m_oCellProperty.m_nCellx = parameter;
			oReader.m_oState->m_oRowProperty.AddItem( oReader.m_oState->m_oCellProperty );
			oReader.m_oState->m_oCellProperty.SetDefaultRtf();
		}
	}
	else if ( L"nonesttables" == sCommand )
	{
		oAbstrReader.Skip( oDocument, oReader );
	}
	else if ( L"listtext" == sCommand )
	{
		oAbstrReader.Skip( oDocument, oReader );
	}
	else if ( L"pntext" == sCommand )
	{
		//пропускаем списки office 95, если есть списки office 2007
		if ( oDocument.m_oListTabel.GetCount() > 0 )
			oAbstrReader.Skip( oDocument, oReader );
		else
		{
			ParagraphReader oParagraphReader( L"pntext", oReader );
			oAbstrReader.StartSubReader( oParagraphReader, oDocument, oReader );
			if ( NULL != oParagraphReader.m_oParPropDest.m_oTextItems && oParagraphReader.m_oParPropDest.m_oTextItems->GetCount() )
			{
				ITextItemPtr piCurContainer;
				oParagraphReader.m_oParPropDest.m_oTextItems->GetItem( piCurContainer, 0 );
				if ( NULL != piCurContainer && TYPE_RTF_PARAGRAPH == piCurContainer->GetType() )
                    oReader.m_oState->m_oCurOldList.m_oText = boost::static_pointer_cast<RtfParagraph, ITextItem>( piCurContainer ) ;
			}
		}
	}
	else if ( L"pn" == sCommand )
	{
		if ( oDocument.m_oListTabel.GetCount() > 0 )
			oAbstrReader.Skip( oDocument, oReader );
		else
		{
			RtfOldList oOldList;
			RtfOldListReader oOldListReader( oOldList );
			oAbstrReader.StartSubReader( oOldListReader, oDocument, oReader );

			oReader.m_oState->m_oCurOldList = oOldList;
			bool bExist = false;
			for( int i = 0; i < (int)oDocument.m_aOldLists.size(); i++ )
				if ( oReader.m_oState->m_oCurOldList == *oDocument.m_aOldLists[i] )
				{
					bExist = true;
					oReader.m_oState->m_oCurOldList.m_nLs	= oDocument.m_aOldLists[i]->m_nLs;
					oReader.m_oState->m_oCurOldList.m_nIlvl = oDocument.m_aOldLists[i]->m_nIlvl;
					break;
				}
			if ( false == bExist )
			{
				oReader.m_oState->m_oCurOldList.m_nLs = oDocument.m_oIdGenerator.Generate_PnId();
				RtfOldListPtr oNewOldList = RtfOldListPtr( new RtfOldList() );
				*oNewOldList = oReader.m_oState->m_oCurOldList;
				oDocument.m_aOldLists.push_back( oNewOldList );
			}
		}
	}

	COMMAND_RTF_INT	( L"uc",	oReader.m_oState->m_nUD, sCommand, hasParameter, parameter)
//Tab todoooo перенести в ParagrProps (trackchanges)	
	COMMAND_RTF_INT( L"tldot",	m_oCurTab.m_eLeader,	sCommand, true, RtfTab::tl_dot )
	COMMAND_RTF_INT( L"tlmdot", m_oCurTab.m_eLeader,	sCommand, true, RtfTab::tl_mdot )
	COMMAND_RTF_INT( L"tlhyph", m_oCurTab.m_eLeader,	sCommand, true, RtfTab::tl_hyph )
	COMMAND_RTF_INT( L"tlul",	m_oCurTab.m_eLeader,	sCommand, true, RtfTab::tl_ul )
	COMMAND_RTF_INT( L"tlth",	m_oCurTab.m_eLeader,	sCommand, true, RtfTab::tl_ul )
	COMMAND_RTF_INT( L"tqr",	m_oCurTab.m_eKind ,		sCommand, true, RtfTab::tk_tqr )
	COMMAND_RTF_INT( L"tqc",	m_oCurTab.m_eKind ,		sCommand, true, RtfTab::tk_tqc )
	COMMAND_RTF_INT( L"tqdec",	m_oCurTab.m_eKind ,		sCommand, true, RtfTab::tk_tqdec )
	else if ( L"tb" == sCommand )
	{
		if ( hasParameter )
		{
			m_oCurTab.m_eKind	= RtfTab::tk_tqbar;
			m_oCurTab.m_nTab	= parameter;
			oReader.m_oState->m_oParagraphProp.m_oTabs.m_aTabs.push_back( m_oCurTab );
			m_oCurTab.SetDefault();
		}
	}
	else if ( L"tx" == sCommand )
	{
		if ( hasParameter )
		{
			m_oCurTab.m_nTab = parameter;
			oReader.m_oState->m_oParagraphProp.m_oTabs.m_aTabs.push_back( m_oCurTab );
			m_oCurTab.SetDefault();
		}
	}
	else 
		bContinue = true;

	if ( false == bContinue ) // compiler limit : blocks nested too deeply
		return true;
	bContinue = false;
//tableStyleProp
	if ( L"*" == sCommand )
		;
	COMMAND_RTF_INT( L"yts",			oReader.m_oState->m_oParagraphProp.m_nTableStyle,		sCommand, hasParameter, parameter )
	
	COMMAND_RTF_BOOL( L"tscfirstrow",	oReader.m_oState->m_oParagraphProp.m_bStyleFirstRow,	sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"tscfirstrow",	oReader.m_oState->m_oCellProperty.m_bStyleFirstRow,		sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"tscfirstrow",	oReader.m_oState->m_oRowProperty.m_bStyleFirstRow,		sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( L"tsclastrow",	oReader.m_oState->m_oParagraphProp.m_bStyleLastRow,		sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"tsclastrow",	oReader.m_oState->m_oCellProperty.m_bStyleLastRow,		sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"tsclastrow",	oReader.m_oState->m_oRowProperty.m_bStyleLastRow,		sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( L"tscfirstcol",	oReader.m_oState->m_oParagraphProp.m_bStyleFirstCollumn, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"tscfirstcol",	oReader.m_oState->m_oCellProperty.m_bStyleFirstCol,		sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"tscfirstcol",	oReader.m_oState->m_oRowProperty.m_bStyleFirstCol,		sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( L"tsclastcol",	oReader.m_oState->m_oParagraphProp.m_bStyleLastCollumn, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"tsclastcol",	oReader.m_oState->m_oCellProperty.m_bStyleLastCol,		sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"tsclastcol",	oReader.m_oState->m_oRowProperty.m_bStyleLastCol,		sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( L"tscbandhorzodd", oReader.m_oState->m_oParagraphProp.m_bStyleOddRowBand, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"tscbandhorzodd", oReader.m_oState->m_oCellProperty.m_bStyleOddRowBand,	sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"tscbandhorzodd", oReader.m_oState->m_oRowProperty.m_bStyleOddRowBand,	sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( L"tscbandhorzeven", oReader.m_oState->m_oParagraphProp.m_bStyleEvenRowBand,	sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"tscbandhorzeven", oReader.m_oState->m_oCellProperty.m_bStyleEvenRowBand,	sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"tscbandhorzeven", oReader.m_oState->m_oRowProperty.m_bStyleEvenRowBand,		sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( L"tscbandvertodd", oReader.m_oState->m_oParagraphProp.m_bStyleOddColBand, sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"tscbandvertodd", oReader.m_oState->m_oCellProperty.m_bStyleOddColBand,	sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"tscbandvertodd", oReader.m_oState->m_oRowProperty.m_bStyleOddColBand,	sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( L"tscbandverteven", oReader.m_oState->m_oParagraphProp.m_bStyleEvenColBand,	sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"tscbandverteven", oReader.m_oState->m_oCellProperty.m_bStyleEvenColBand,	sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"tscbandverteven", oReader.m_oState->m_oRowProperty.m_bStyleEvenColBand,		sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( L"tscnwcell", oReader.m_oState->m_oParagraphProp.m_bStyleNWCell,	sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"tscnwcell", oReader.m_oState->m_oCellProperty.m_bStyleNWCell,	sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"tscnwcell", oReader.m_oState->m_oRowProperty.m_bStyleNWCell,	sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( L"tscnecell", oReader.m_oState->m_oParagraphProp.m_bStyleNECell,	sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"tscnecell", oReader.m_oState->m_oCellProperty.m_bStyleNECell,	sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"tscnecell", oReader.m_oState->m_oRowProperty.m_bStyleNECell,	sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( L"tscswcell", oReader.m_oState->m_oParagraphProp.m_bStyleSWCell,	sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"tscswcell", oReader.m_oState->m_oCellProperty.m_bStyleSWCell,	sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"tscswcell", oReader.m_oState->m_oRowProperty.m_bStyleSWCell,	sCommand, hasParameter, parameter )

	COMMAND_RTF_BOOL( L"tscsecell", oReader.m_oState->m_oParagraphProp.m_bStyleSECell,	sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"tscsecell", oReader.m_oState->m_oCellProperty.m_bStyleSECell,	sCommand, hasParameter, parameter )
	COMMAND_RTF_BOOL( L"tscsecell", oReader.m_oState->m_oRowProperty.m_bStyleSECell,	sCommand, hasParameter, parameter )
//Math
	else if ( L"mmath" == sCommand )
	{
		RtfMathPtr oNewMath = RtfMathPtr( new RtfMath() );
		RtfMathReader oMathReader( *oNewMath );
		oAbstrReader.StartSubReader( oMathReader, oDocument, oReader );
		m_oCurParagraph->AddItem( oNewMath );
	}
//Drawing
	else if ( L"shp" == sCommand )
	{
		RtfShapePtr oNewShape( new RtfShape() );
		ShapeReader oShapeReader( *oNewShape );
		oAbstrReader.StartSubReader( oShapeReader, oDocument, oReader );
		if ( oNewShape->IsValid() )
			m_oCurParagraph->AddItem( oNewShape );
	}
	else if ( L"do" == sCommand )
	{
		RtfShapePtr oNewShape( new RtfShape() );
		OldShapeReader oShapeReader( *oNewShape );
		oAbstrReader.StartSubReader( oShapeReader, oDocument, oReader );
		if ( oNewShape->IsValid() )
			m_oCurParagraph->AddItem( oNewShape );
	}
	else if ( L"shppict" == sCommand )
	{
		RtfShapePtr oNewShape( new RtfShape() );
		ShppictReader oShppictReader( *oNewShape );
		oAbstrReader.StartSubReader( oShppictReader, oDocument, oReader );
		if ( oNewShape->IsValid() )
			m_oCurParagraph->AddItem( oNewShape );
	}
	else if ( L"pict" == sCommand )
	{
		RtfShapePtr oNewShape( new RtfShape() );
		oNewShape->m_nShapeType			= NSOfficeDrawing::sptPictureFrame;
		oNewShape->m_nWrapType			= 3; // none
		oNewShape->m_nPositionHRelative = 3;//TCHAR
		oNewShape->m_nPositionVRelative = 3;//line
		oNewShape->m_nPositionH			= 0;//absolute
		oNewShape->m_nPositionV			= 0;//absolute

		oNewShape->m_oPicture = RtfPicturePtr( new RtfPicture() );
		PictureReader oPictureReader( oReader, *oNewShape);
		oAbstrReader.StartSubReader( oPictureReader, oDocument, oReader );

		if ( oNewShape->IsValid() )
			m_oCurParagraph->AddItem( oNewShape );
	}
	else if ( L"shpgrp" == sCommand )
	{
		RtfShapeGroupPtr oNewShape( new RtfShapeGroup() );
		ShapeGroupReader oShapeGroupReader( *oNewShape );
		oAbstrReader.StartSubReader( oShapeGroupReader, oDocument, oReader );
		if ( oNewShape->IsValid() )
			m_oCurParagraph->AddItem( oNewShape );
	}
	else if ( L"nonshppict" == sCommand )
	{
		oAbstrReader.Skip( oDocument, oReader );
	}
	else if ( L"field" == sCommand )
	{
		RtfFieldPtr oNewField = RtfFieldPtr(new RtfField());
		FieldReader oFieldReader( *oNewField );
		oAbstrReader.StartSubReader( oFieldReader, oDocument, oReader );
		if ( oNewField->IsValid() )
			m_oCurParagraph->AddItem( oNewField );
	}
	else if ( L"bkmkstart" == sCommand )
	{
		RtfBookmarkStartPtr oNewBookmarkStart = RtfBookmarkStartPtr( new RtfBookmarkStart() );
		BookmarkStartReader oBookmarkStartReader( *oNewBookmarkStart );
		oAbstrReader.StartSubReader( oBookmarkStartReader, oDocument, oReader );
		if ( oNewBookmarkStart->IsValid() )
			m_oCurParagraph->AddItem( oNewBookmarkStart );
	}
	else if ( L"bkmkend" == sCommand )
	{
		RtfBookmarkEndPtr oNewBookmarkEnd = RtfBookmarkEndPtr( new RtfBookmarkEnd() );
		BookmarkEndReader oBookmarkEndReader( *oNewBookmarkEnd );
		oAbstrReader.StartSubReader( oBookmarkEndReader, oDocument, oReader );
		if ( oNewBookmarkEnd->IsValid() )
			m_oCurParagraph->AddItem( oNewBookmarkEnd );
	}
	else if ( L"footnote" == sCommand )
	{
		RtfFootnotePtr oNewFootnote = RtfFootnotePtr( new RtfFootnote() );
		oNewFootnote->m_oCharProp = oReader.m_oState->m_oCharProp;
		FootnoteReader oFootnoteReader( *oNewFootnote );
		oAbstrReader.StartSubReader( oFootnoteReader, oDocument, oReader );
		if ( oNewFootnote->IsValid() )
			m_oCurParagraph->AddItem( oNewFootnote );
	}
	else if ( L"chftn" == sCommand )
	{
		if ( 1 == oReader.m_nFootnote )
		{
			RtfCharSpecialPtr oNewChar( new RtfCharSpecial() );
			oNewChar->m_oProperty = oReader.m_oState->m_oCharProp;
			oNewChar->m_eType = RtfCharSpecial::rsc_chftn;
			m_oCurParagraph->AddItem( oNewChar );
		}
		else if ( 2 == oReader.m_nFootnote )
		{
			RtfCharSpecialPtr oNewChar( new RtfCharSpecial() );
			oNewChar->m_oProperty = oReader.m_oState->m_oCharProp;
			oNewChar->m_eType = RtfCharSpecial::rsc_chftnEnd;
			m_oCurParagraph->AddItem( oNewChar );
		}
	}
	else if ( L"chftnsep" == sCommand || L"chftnsepc" == sCommand )
	{
		RtfCharSpecialPtr oNewChar( new RtfCharSpecial() );
		oNewChar->m_oProperty = oReader.m_oState->m_oCharProp;
		m_oCurParagraph->AddItem( oNewChar );
		
		if		( L"chftnsep"	== sCommand )	oNewChar->m_eType = RtfCharSpecial::rsc_chftnsep;
		else if ( L"chftnsepc"	== sCommand )	oNewChar->m_eType = RtfCharSpecial::rsc_chftnsepc;
	}//specialChars
	else if ( L"page" == sCommand  )
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
	COMMAND_RTF_SPECIAL_CHAR( L"column",	m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_column )
	COMMAND_RTF_SPECIAL_CHAR( L"line",		m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_line )
	COMMAND_RTF_SPECIAL_CHAR( L"line",		m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_line )
	else if ( L"lbr" == sCommand )
	{
		if ( hasParameter )
		{
			RtfCharSpecialPtr oNewChar	= RtfCharSpecialPtr( new RtfCharSpecial() );
			oNewChar->m_nTextWrapBreak	= parameter;
			oNewChar->m_oProperty		= oReader.m_oState->m_oCharProp;
			m_oCurParagraph->AddItem( oNewChar );
		}
	}
	COMMAND_RTF_SPECIAL_CHAR( L"softpage",	m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_softpage )
	COMMAND_RTF_SPECIAL_CHAR( L"softcol",	m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_softcol )
	COMMAND_RTF_SPECIAL_CHAR( L"softline",	m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_softline )
	
	else if ( L"softlheight" == sCommand )
	{
		if ( hasParameter )
		{
			RtfCharSpecialPtr oNewChar	= RtfCharSpecialPtr( new RtfCharSpecial() );
			oNewChar->m_nSoftHeight		= parameter;
			oNewChar->m_oProperty		= oReader.m_oState->m_oCharProp;
			m_oCurParagraph->AddItem( oNewChar );
		}
	}
	COMMAND_RTF_SPECIAL_CHAR( L"tab", m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_tab )
	
	else if ( L"emdash" == sCommand )
		ExecuteNumberChar( oDocument, oReader, oAbstrReader, 151, 0xD0 ); //\bullet Word for Windows - 151	; Apple Macintosh - 0xD0
	else if ( L"endash" == sCommand )
		ExecuteNumberChar( oDocument, oReader, oAbstrReader, 150, 0xD1 ); //\bullet Word for Windows - 150	; Apple Macintosh - 0xD1
	
	COMMAND_RTF_SPECIAL_CHAR( L"emspace", m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_emspace )
	COMMAND_RTF_SPECIAL_CHAR( L"enspace", m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_enspace )
	COMMAND_RTF_SPECIAL_CHAR( L"qmspace", m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_qmspace )
	
	else if ( L"bullet" == sCommand )
		ExecuteNumberChar( oDocument, oReader, oAbstrReader, 149, 0xA5 ); //\bullet Word for Windows - 149	; Apple Macintosh - 0xA5
	else if ( L"lquote" == sCommand )
		ExecuteNumberChar( oDocument, oReader, oAbstrReader, 145, 0xD4 ); //\bullet Word for Windows - 145	; Apple Macintosh - 0xD4
	else if ( L"rquote" == sCommand )
		ExecuteNumberChar( oDocument, oReader, oAbstrReader, 146, 0xD5 ); //\bullet Word for Windows - 146	; Apple Macintosh - 0xD5
	else if ( L"ldblquote" == sCommand )
		ExecuteNumberChar( oDocument, oReader, oAbstrReader, 147, 0xD2 ); //\bullet Word for Windows - 147	; Apple Macintosh - 0xD2
	else if ( L"rdblquote" == sCommand )
		ExecuteNumberChar( oDocument, oReader, oAbstrReader, 148, 0xD3 ); //\bullet Word for Windows - 148	; Apple Macintosh - 0xD3
	
	COMMAND_RTF_SPECIAL_CHAR( L"|", m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_Formula )
	COMMAND_RTF_SPECIAL_CHAR( L"~", m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_NonBrSpace )
	COMMAND_RTF_SPECIAL_CHAR( L"-", m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_OptHyphen )
	COMMAND_RTF_SPECIAL_CHAR( L"_", m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_NonBrHyphen )
	COMMAND_RTF_SPECIAL_CHAR( L":", m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_SubEntry )
	COMMAND_RTF_SPECIAL_CHAR( L"zwbo", m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_zwbo )
	COMMAND_RTF_SPECIAL_CHAR( L"zwnbo", m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_zwnbo )
	COMMAND_RTF_SPECIAL_CHAR( L"zwj", m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_zwj )
	COMMAND_RTF_SPECIAL_CHAR( L"zwnj", m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_zwnj )
	else if ( L"object" == sCommand )
	{
		RtfOlePtr oNewOleObject = RtfOlePtr( new RtfOle() );
		OleReader oOleReader( *oNewOleObject );
		oAbstrReader.StartSubReader( oOleReader, oDocument, oReader );
		if ( oNewOleObject->IsValid() )
			m_oCurParagraph->AddItem( oNewOleObject );
	}
	else if ( L"oldcprops"  == sCommand )
	{
		RtfCharPropertyPtr props = RtfCharPropertyPtr( new RtfCharProperty() );
		TrackerChangesReader oOldPropReader(props);
		
		if (oAbstrReader.StartSubReader( oOldPropReader, oDocument, oReader ))
		{
			oReader.m_oState->m_oCharProp.m_pOldCharProp = props;
		}
	}
	else if ( L"oldpprops"  == sCommand )
	{
		RtfParagraphPropertyPtr props =  RtfParagraphPropertyPtr( new RtfParagraphProperty() );
		TrackerChangesReader oOldPropReader(props);
		
		if (oAbstrReader.StartSubReader( oOldPropReader, oDocument, oReader ))
		{
			oReader.m_oState->m_oParagraphProp.m_pOldParagraphProp = props;
		}
	}
	else if ( L"oldsprops"  == sCommand )
	{
		RtfSectionPropertyPtr props = RtfSectionPropertyPtr( new RtfSectionProperty() );
		TrackerChangesReader oOldPropReader(props);
		
		if (oAbstrReader.StartSubReader( oOldPropReader, oDocument, oReader ))
		{
			//oReader.m_oState->m_oCellProp.m_pOldCellProp = props;
		}
	}
	else if ( L"oldtprops"  == sCommand )
	{
		RtfRowPropertyPtr props = RtfRowPropertyPtr( new RtfRowProperty() );
		TrackerChangesReader oOldPropReader(props);
		
		if (oAbstrReader.StartSubReader( oOldPropReader, oDocument, oReader ))
		{
			oReader.m_oState->m_oRowProperty.m_pOldRowProperty = props;
		}
	}
	else if ( L"chbrdr"	== sCommand )		m_eInternalState = is_charBorder;
	else if ( L"brdrt"	== sCommand )		m_eInternalState = is_borderTop;
	else if ( L"brdrb"	== sCommand )		m_eInternalState = is_borderBottom;
	else if ( L"brdrl"	== sCommand )		m_eInternalState = is_borderLeft;
	else if ( L"brdrr"	== sCommand )		m_eInternalState = is_borderRight;
	//else if ( L"brdrbtw" == sCommand )	m_eInternalState = is_borderRight;
	else if ( L"brdrbar" == sCommand )		m_eInternalState = is_borderBar;
	else if ( L"box"	== sCommand )		m_eInternalState = is_borderBox;

	else if ( L"cldglu" == sCommand )		m_eInternalState = is_borderCellLR;
	else if ( L"cldgll" == sCommand )		m_eInternalState = is_borderCellRL;
	else if ( L"clbrdrl" == sCommand )		m_eInternalState = is_borderCellLeft;
	else if ( L"clbrdrt" == sCommand )		m_eInternalState = is_borderCellTop;
	else if ( L"clbrdrr" == sCommand )		m_eInternalState = is_borderCellRight;
	else if ( L"clbrdrb" == sCommand )		m_eInternalState = is_borderCellBottom;

	else if ( L"tsbrdrdgl" == sCommand )	m_eInternalState = is_borderCellLR;
	else if ( L"tsbrdrdgr" == sCommand )	m_eInternalState = is_borderCellRL;

	else if ( L"trbrdrl" == sCommand )		m_eInternalState = is_borderRowLeft;
	else if ( L"trbrdrr" == sCommand )		m_eInternalState = is_borderRowRight;
	else if ( L"trbrdrt" == sCommand )		m_eInternalState = is_borderRowTop;
	else if ( L"trbrdrb" == sCommand )		m_eInternalState = is_borderRowBottom;
	else if ( L"trbrdrv" == sCommand )		m_eInternalState = is_borderRowVer;
	else if ( L"trbrdrh" == sCommand )		m_eInternalState = is_borderRowHor;
	else if ( L"tsbrdrh" == sCommand ) 		m_eInternalState = is_borderRowHor;
	else if ( L"tsbrdrv" == sCommand )		m_eInternalState = is_borderRowVer;
	else if ( L"tsbrdrl" == sCommand )		m_eInternalState = is_borderRowLeft;
	else if ( L"tsbrdrt" == sCommand )		m_eInternalState = is_borderRowTop;
	else if ( L"tsbrdrr" == sCommand )		m_eInternalState = is_borderRowRight;
	else if ( L"tsbrdrb" == sCommand )		m_eInternalState = is_borderRowBottom;
	else
	{
		bool bResult = false;

		switch(m_eInternalState)
		{
		case is_borderBar:
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, oReader.m_oState->m_oParagraphProp.m_oBorderBar );
			break;
		case is_borderBottom:
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, oReader.m_oState->m_oParagraphProp.m_oBorderBottom );
			break;
		case is_borderBox:
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, oReader.m_oState->m_oParagraphProp.m_oBorderBox );
			break;
		case is_borderLeft:
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, oReader.m_oState->m_oParagraphProp.m_oBorderLeft );
			break;
		case is_borderRight:
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, oReader.m_oState->m_oParagraphProp.m_oBorderRight );
			break;
		case is_borderTop:
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, oReader.m_oState->m_oParagraphProp.m_oBorderTop );
			break;
//----------------
		case is_borderCellBottom:
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oCellProperty.m_oBorderBottom );
			break;
		case is_borderCellLeft:
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter,oReader.m_oState->m_oCellProperty.m_oBorderLeft );
			break;
		case is_borderCellRight:
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oCellProperty.m_oBorderRight );
			break;
		case is_borderCellTop:
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oCellProperty.m_oBorderTop );
			break;
		case is_borderCellLR:
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oCellProperty.m_oBorderDiagonalLR );
			break;
		case is_borderCellRL:
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oCellProperty.m_oBorderDiagonalRL );
			break;
//----------
		case is_borderRowBottom :
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oRowProperty.m_oBorderBottom );
			break;
		case is_borderRowHor :
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter,oReader.m_oState->m_oRowProperty.m_oBorderHor );
			break;
		case is_borderRowLeft :
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oRowProperty.m_oBorderLeft );
			break;
		case is_borderRowRight :
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oRowProperty.m_oBorderRight );
			break;
		case is_borderRowTop :
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oRowProperty.m_oBorderTop );
			break;
		case is_borderRowVer :
			bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oRowProperty.m_oBorderVert );
			break;
		}
		if ( bResult )	return true;

		if (RtfShadingRowCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oRowProperty.m_oShading ))
			return true;
		
		if ( is_charBorder == m_eInternalState )
			if (RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter,  oReader.m_oState->m_oCharProp.m_poBorder))
				return true;

		if (RtfParagraphPropsCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, &oReader.m_oState->m_oParagraphProp ))
			return true;
		
		if (RtfTableRowPropsCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, &oReader.m_oState->m_oRowProperty ))
			return true;
		
		if (RtfTableCellPropsCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, &oReader.m_oState->m_oCellProperty ))
			return true;

		if (RtfCharPropsCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, &oReader.m_oState->m_oCharProp ))
			return true;

		return false;
	}
	return true;
}

