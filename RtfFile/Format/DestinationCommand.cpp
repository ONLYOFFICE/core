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

#include "DestinationCommand.h"
#include "RtfOle.h"
#include "Ole1FormatReader.h"

#include "ConvertationManager.h"

#include <boost/algorithm/string.hpp>
#include "../../OfficeUtils/src/OfficeUtils.h"
#include "../../DesktopEditor/xml/include/xmlutils.h"

TextReader::TextReader( std::wstring& sName, bool bErease  ) : m_sName(sName), m_bErease(bErease)
{
}
void TextReader::ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText)
{
	if( true == m_bErease )
		XmlUtils::replace_all(sText, L";", L"");
	m_sName += sText;
}

void ConvertOle1ToOle2(BYTE *pData, int nSize, std::wstring sOle2Name)
{
	Ole1FormatReaderWriter ole1Reader(pData, nSize);

	bool bResult = false;
	if (ole1Reader.NativeDataSize > 0)
	{
		NSFile::CFileBinary file;
		
		file.CreateFileW(sOle2Name);
		file.WriteFile(ole1Reader.NativeData, ole1Reader.NativeDataSize);
		file.CloseFile();
		
		POLE::Storage * storageIn = new POLE::Storage(sOle2Name.c_str());
		if ( (storageIn) && (storageIn->open(false, false))) //storage in storage
		{
			POLE::Stream stream(storageIn, L"Package");
			if (false == stream.fail())
			{//test package stream??? xls ole -> xlsx ole

				POLE::uint64 size = stream.size();
				unsigned char* data = new unsigned char[size];
				stream.read(data, size);
				storageIn->close();

				NSFile::CFileBinary file;
				
				file.CreateFileW(sOle2Name);
				file.WriteFile(data, (DWORD)size);
				file.CloseFile();

				delete []data;
				bResult = true;
			}
			else
			{//уже ole2
				storageIn->close();
				bResult = true;
			}
		}
		delete storageIn;
		
		if (!bResult)
		{
			POLE::Storage * storageOut = new POLE::Storage(sOle2Name.c_str());
			if ( (storageOut) && (storageOut->open(true, true)))
			{
				_UINT32 tmp = 0;
				std::string name = ole1Reader.Header.ClassName.val;
				_UINT32 name_size = (_UINT32)name.length() + 1;
				//Ole
				BYTE dataOleInfo[] = {0x01,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
				POLE::Stream oStream3(storageOut, L"\001Ole", true, 20);
				oStream3.write(dataOleInfo, 20);
				oStream3.flush();
				//CompObj
				BYTE dataCompObjHeader[28] = {0x01,0x00,0xfe,0xff,0x03,0x0a,0x00,0x00,0xff,0xff,0xff,0xff,0x0a,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x46};
				POLE::Stream oStream1(storageOut, L"\001CompObj", true, 28 + (name_size + 5) + 2 * (ole1Reader.Header.ClassName.size + 4) + 4 * 4);
				oStream1.write(dataCompObjHeader, 28);

				oStream1.write((BYTE*)&name_size, 4);
				oStream1.write((BYTE*)name.c_str(), name_size);

				//oStream1.write((BYTE*)&ole1Reader.Header.ClassName.size, 4);
				//oStream1.write((BYTE*)ole1Reader.Header.ClassName.val.c_str(), ole1Reader.Header.ClassName.size);

				oStream1.write((BYTE*)&ole1Reader.Header.ClassName.size, 4);
				oStream1.write((BYTE*)ole1Reader.Header.ClassName.val.c_str(), ole1Reader.Header.ClassName.size);
				oStream1.write((BYTE*)&ole1Reader.Header.ClassName.size, 4);
				oStream1.write((BYTE*)ole1Reader.Header.ClassName.val.c_str(), ole1Reader.Header.ClassName.size);

				tmp = 0x71B239F4;
				oStream1.write((BYTE*)&tmp, 4); // UnicodeMarker

				tmp = 0;
				oStream1.write((BYTE*)&tmp, 4); // UnicodeUserType
				oStream1.write((BYTE*)&tmp, 4); // UnicodeClipboardFormat
				oStream1.write((BYTE*)&tmp, 4); //
				oStream1.flush();

				//ObjInfo
				BYTE dataObjInfo[] = {0x00,0x00,0x03,0x00,0x04,0x00};
				POLE::Stream oStream2(storageOut, L"\003ObjInfo", true, 6);
				oStream2.write(dataObjInfo, 6);
				oStream2.flush();
				//Ole10Native
				POLE::Stream streamData(storageOut, L"\001Ole10Native", true, ole1Reader.NativeDataSize + 4);
				streamData.write((BYTE*)&ole1Reader.NativeDataSize, 4);

				_UINT32 sz_write = 0;
				_UINT32 sz = 4096;
				while (sz_write < ole1Reader.NativeDataSize)
				{
					if (sz_write + sz > ole1Reader.NativeDataSize)
						sz = ole1Reader.NativeDataSize - sz_write;
					streamData.write(ole1Reader.NativeData + sz_write, sz);
					sz_write += sz;
				}
				streamData.flush();

				storageOut->close();
				delete storageOut;

				bResult = true;
			}
		}
	}
	if (!bResult) //conv_NI38P7GBIpw1aD84H3k.rtf
	{
		NSFile::CFileBinary file;
		
		file.CreateFileW(sOle2Name);
		file.WriteFile(pData, nSize);
		file.CloseFile();
		
		bResult = true;
	}
}
//-----------------------------------------------------------------------------------------
bool RtfDocumentCommand::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,  std::string sCommand, bool hasParameter, int parameter)
{
	if		( "ansi"	== sCommand )	oDocument.m_oProperty.m_eCodePage = RtfDocumentProperty::cp_ansi;
	else if ( "mac"		== sCommand )	oDocument.m_oProperty.m_eCodePage = RtfDocumentProperty::cp_mac;
	else if ( "pc"		== sCommand )	oDocument.m_oProperty.m_eCodePage = RtfDocumentProperty::cp_pc;
	else if ( "pca"		== sCommand )	oDocument.m_oProperty.m_eCodePage = RtfDocumentProperty::cp_pca;
	else if ( "ansicpg"	== sCommand )
	{
		oDocument.m_oProperty.m_eCodePage = RtfDocumentProperty::cp_ansi;

		if ( hasParameter )
		{
			oDocument.m_oProperty.m_nAnsiCodePage = parameter;
		}
	}
	else if ( "deff" == sCommand )
	{
		if ( hasParameter )
		{
			oDocument.m_oProperty.m_nDeffFont = parameter;
			oReader.m_nDefFont = parameter;
		}
	}
	else if ( "dgmargin" == sCommand )//Drawing grid to follow margins.
	{

	}
	else if ( "dgsnap" == sCommand )//Snap to drawing grid.
	{

	}
	COMMAND_RTF_INT	( "themelang",		oDocument.m_oProperty.m_nThemelang,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT	( "themelangfe",	oDocument.m_oProperty.m_nThemelangfe,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT	( "themelangcs",	oDocument.m_oProperty.m_nThemelangcs,	sCommand, hasParameter, parameter )

			COMMAND_RTF_BOOL( "rtlgutter",		oDocument.m_oProperty.m_bRtlGutter, sCommand, hasParameter, parameter)
			COMMAND_RTF_BOOL( "rtldoc",			oDocument.m_oProperty.m_bRtl,		sCommand, hasParameter, parameter)

			COMMAND_RTF_BOOL( "hyphcaps",	oDocument.m_oProperty.m_bHypCaps,	sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "hyphauto",	oDocument.m_oProperty.m_bAutoHyp,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT	( "hyphconsec",	oDocument.m_oProperty.m_nMaxHypen,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT	( "hyphhotz",	oDocument.m_oProperty.m_nHyphenationRight, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT	( "deftab",		oDocument.m_oProperty.m_nTabWidth,	sCommand, hasParameter, parameter )

			//Page Borders
			COMMAND_RTF_BOOL( "pgbrdrhead", oDocument.m_oProperty.m_bDorderSurroundHeader, sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "pgbrdrfoot", oDocument.m_oProperty.m_bDorderSurroundFotter, sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "pgbrdrsnap", oDocument.m_oProperty.m_bAlignBordersAndEdges, sCommand, hasParameter, parameter )

			COMMAND_RTF_INT	( "dghspace",		oDocument.m_oProperty.m_nDrawingGridHorizontalSpacing,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT	( "dgvspace",		oDocument.m_oProperty.m_nDrawingGridVerticalSpacing,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT	( "dghorigin",		oDocument.m_oProperty.m_nDrawingGridHorizontalOrigin,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT	( "dgvorigin",		oDocument.m_oProperty.m_nDrawingGridVerticalOrigin,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT	( "dghshow",		oDocument.m_oProperty.m_nDisplayHorizontalDrawingGridEvery,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT	( "dgvshow",		oDocument.m_oProperty.m_nDisplayVerticalDrawingGridEvery,	sCommand, hasParameter, parameter )

			COMMAND_RTF_INT	( "viewscale",		oDocument.m_oProperty.m_nZoom,	sCommand, hasParameter, parameter )

			//Page Informationf
			else if ( "paperw" == sCommand )
	{
		if ( hasParameter )
		{
			oDocument.m_oProperty.m_nPaperWidth		= parameter;
			oReader.m_oCurSectionProp.m_nPageWidth	= parameter;
		}
	}
	else if ( "paperh" == sCommand )
	{
		if ( hasParameter )
		{
			oDocument.m_oProperty.m_nPaperHeight	= parameter;
			oReader.m_oCurSectionProp.m_nPageHeight = parameter;
		}
	}
	else if ( "margl" == sCommand )
	{
		if ( hasParameter )
		{
			oDocument.m_oProperty.m_nMarginLeft		= parameter;
			oReader.m_oCurSectionProp.m_nMarginLeft = parameter;
		}
	}
	else if ( "margr" == sCommand )
	{
		if ( hasParameter )
		{
			oDocument.m_oProperty.m_nMarginRight		= parameter;
			oReader.m_oCurSectionProp.m_nMarginRight	= parameter;
		}
	}
	else if ( "margt" == sCommand )
	{
		if ( hasParameter )
		{
			oDocument.m_oProperty.m_nMarginTop		= parameter;
			oReader.m_oCurSectionProp.m_nMarginTop	= parameter;
		}
	}
	else if ( "margb" == sCommand )
	{
		if ( hasParameter )
		{
			oDocument.m_oProperty.m_nMarginBottom		= parameter;
			oReader.m_oCurSectionProp.m_nMarginBottom	= parameter;
		}
	}
	else if ( "facingp" == sCommand )
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
	else if ( "gutter" == sCommand )
	{
		if ( hasParameter )
		{
			oDocument.m_oProperty.m_nGutterWidth = parameter;
			oReader.m_oCurSectionProp.m_nGutterMarginWidth = parameter;
		}
	}
	else if ( "ogutter" == sCommand )
	{
		if ( hasParameter )
			oDocument.m_oProperty.m_nGutterWidthOutside = parameter;
	}
	else if ( "margmirror" == sCommand )
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
	else if ( "landscape" == sCommand )
	{
		if ( hasParameter && 0 == parameter )
			oDocument.m_oProperty.m_bLandScape = 0;
		else
			oDocument.m_oProperty.m_bLandScape = 1;
	}
	COMMAND_RTF_BOOL( "gutterprl",		oDocument.m_oProperty.m_bGutterAtTop,		sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "viewbksp",		oDocument.m_oProperty.m_nDisplayBackground, sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "htmautsp",		oDocument.m_oProperty.m_bHtmlAutoSpace,		sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "useltbaln",		oDocument.m_oProperty.m_bUseTabAlignment,	sCommand, hasParameter, parameter )
			//Footnotes and Endnotes
			COMMAND_RTF_INT	( "endnotes",		oDocument.m_oProperty.m_eFootnotePlacement, sCommand, true, RtfDocumentProperty::fp_EndSection )
			COMMAND_RTF_INT	( "enddoc",			oDocument.m_oProperty.m_eFootnotePlacement, sCommand, true, RtfDocumentProperty::fp_EndDocument )
			COMMAND_RTF_INT	( "ftntj",			oDocument.m_oProperty.m_eFootnotePlacement, sCommand, true, RtfDocumentProperty::fp_BeneathText )
			COMMAND_RTF_INT	( "ftnbj",			oDocument.m_oProperty.m_eFootnotePlacement, sCommand, true, RtfDocumentProperty::fp_BottomPage )

			COMMAND_RTF_INT ( "aendnotes",		oDocument.m_oProperty.m_eEndnotePlacement,	sCommand, true, RtfDocumentProperty::ep_EndSection )
			COMMAND_RTF_INT ( "aenddoc",		oDocument.m_oProperty.m_eEndnotePlacement,	sCommand, true, RtfDocumentProperty::ep_EndDocument )
			COMMAND_RTF_INT ( "aftnbj",			oDocument.m_oProperty.m_eEndnotePlacement,	sCommand, true, RtfDocumentProperty::ep_BeneathText )
			COMMAND_RTF_INT ( "aftntj",			oDocument.m_oProperty.m_eEndnotePlacement,	sCommand, true, RtfDocumentProperty::ep_BottomPage )

			COMMAND_RTF_INT ( "ftnstart",		oDocument.m_oProperty.m_nFootnoteStart,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "aftnstart",		oDocument.m_oProperty.m_nEndnoteStart,		sCommand, hasParameter, parameter )

			COMMAND_RTF_INT ( "ftnrstpg",		oDocument.m_oProperty.m_eFootnoteRestart,	sCommand, true, RtfDocumentProperty::fr_EachPage )
			COMMAND_RTF_INT ( "ftnrestart",		oDocument.m_oProperty.m_eFootnoteRestart,	sCommand, true, RtfDocumentProperty::fr_EachSection )
			COMMAND_RTF_INT ( "ftnrstcont",		oDocument.m_oProperty.m_eFootnoteRestart,	sCommand, true, RtfDocumentProperty::fr_Continuous )
			COMMAND_RTF_INT ( "aftnrestart",	oDocument.m_oProperty.m_eEndnoteRestart,	sCommand, true, RtfDocumentProperty::er_EachSection )
			COMMAND_RTF_INT ( "aftnrstcont",	oDocument.m_oProperty.m_eEndnoteRestart,	sCommand, true, RtfDocumentProperty::er_Continuous )

			COMMAND_RTF_INT ( "ftnnar",			oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 0 )
			COMMAND_RTF_INT ( "ftnnalc",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 4 )
			COMMAND_RTF_INT ( "ftnnauc",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 3 )
			COMMAND_RTF_INT ( "ftnnrlc",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 2 )
			COMMAND_RTF_INT ( "ftnnruc",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 1 )
			COMMAND_RTF_INT ( "ftnnchi",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 70 )
			COMMAND_RTF_INT ( "ftnnchi",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 25 )
			COMMAND_RTF_INT ( "ftnncnum",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 18 )
			COMMAND_RTF_INT ( "ftnndbnum",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 10 )
			COMMAND_RTF_INT ( "ftnndbnumd",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 11 )
			COMMAND_RTF_INT ( "ftnndbnumt",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 16 )
			COMMAND_RTF_INT ( "ftnndbnumk",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 17 )
			COMMAND_RTF_INT ( "ftnndbar",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 20 )
			COMMAND_RTF_INT ( "ftnnganada",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 24 )
			COMMAND_RTF_INT ( "ftnngbnum",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 26 )
			COMMAND_RTF_INT ( "ftnngbnumd",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 27 )
			COMMAND_RTF_INT ( "ftnngbnuml",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 28 )
			COMMAND_RTF_INT ( "ftnngbnumk",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 29 )
			COMMAND_RTF_INT ( "ftnnzodiac",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 30 )
			COMMAND_RTF_INT ( "ftnnzodiacd",	oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 31 )
			COMMAND_RTF_INT ( "ftnnzodiacl",	oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 32 )

			COMMAND_RTF_INT ( "aftnnar",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 0 )
			COMMAND_RTF_INT ( "aftnnalc",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 4 )
			COMMAND_RTF_INT ( "aftnnauc",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 3 )
			COMMAND_RTF_INT ( "aftnnrlc",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 2 )
			COMMAND_RTF_INT ( "aftnnruc",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 1 )
			COMMAND_RTF_INT ( "aftnnchi",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 70 )
			COMMAND_RTF_INT ( "aftnnchi",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 25 )
			COMMAND_RTF_INT ( "aftnncnum",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 18 )
			COMMAND_RTF_INT ( "aftnndbnum",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 10 )
			COMMAND_RTF_INT ( "aftnndbnumd",	oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 11 )
			COMMAND_RTF_INT ( "aftnndbnumt",	oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 16 )
			COMMAND_RTF_INT ( "aftnndbnumk",	oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 17 )
			COMMAND_RTF_INT ( "aftnndbar",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 20 )
			COMMAND_RTF_INT ( "aftnnganada",	oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 24 )
			COMMAND_RTF_INT ( "aftnngbnum",		oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 26 )
			COMMAND_RTF_INT ( "aftnngbnumd",	oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 27 )
			COMMAND_RTF_INT ( "aftnngbnuml",	oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 28 )
			COMMAND_RTF_INT ( "aftnngbnumk",	oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 29 )
			COMMAND_RTF_INT ( "aftnnzodiac",	oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 30 )
			COMMAND_RTF_INT ( "aftnnzodiacd",	oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 31 )
			COMMAND_RTF_INT ( "aftnnzodiacl",	oDocument.m_oProperty.m_nFootnoteNumberingFormat, sCommand, true, 32 )
			else
			return false;
	return true;
}

RtfNormalReader::RtfNormalReader( RtfDocument& oDocument, RtfReader& oReader )
{
	SectDef( oDocument, oReader );

	_section section;
	if(true == oDocument.GetItem( section ) )
	{
		section.props->m_oProperty					= oReader.m_oCurSectionProp;
		oParagraphReaderDestination.m_oTextItems	= section.props;
	}
	m_nCurGroups = 0;
}
void RtfNormalReader::ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText)
{
	oParagraphReaderDestination.ExecuteText( oDocument, oReader, sText );
}
void RtfNormalReader::ExitReader(RtfDocument& oDocument, RtfReader& oReader)
{
	oParagraphReaderDestination.m_bPar = m_bStopReader ? false : true;
	oParagraphReaderDestination.Finalize( oReader );

	_section section;
	if(true == oDocument.GetItem( section) )
	{
		section.props->m_oProperty = oReader.m_oCurSectionProp;
	}

	if( NULL == oDocument.m_oFootnoteCon )
	{
		oDocument.m_oFootnoteCon		= TextItemContainerPtr	( new TextItemContainer() );
		RtfParagraphPtr		pNewPar	( new RtfParagraph() );
		RtfCharSpecialPtr	pNewChar( new RtfCharSpecial() );
		pNewChar->m_eType				= RtfCharSpecial::rsc_chftnsepc;

		pNewPar->AddItem( pNewChar );
		oDocument.m_oFootnoteCon->AddItem( pNewPar );
		oDocument.m_oProperty.m_aSpecialFootnotes.push_back( 1 );
	}
	if( NULL == oDocument.m_oFootnoteSep )
	{
		oDocument.m_oFootnoteSep		= TextItemContainerPtr( new TextItemContainer() );
		RtfParagraphPtr		pNewPar	( new RtfParagraph() );
		RtfCharSpecialPtr	pNewChar( new RtfCharSpecial() );
		pNewChar->m_eType				= RtfCharSpecial::rsc_chftnsep;

		pNewPar->AddItem( pNewChar );
		oDocument.m_oFootnoteSep->AddItem( pNewPar );
		oDocument.m_oProperty.m_aSpecialFootnotes.push_back( 0 );
	}
	if( NULL == oDocument.m_oEndnoteCon )
	{
		oDocument.m_oEndnoteCon			= TextItemContainerPtr( new TextItemContainer() );
		RtfParagraphPtr		pNewPar	( new RtfParagraph() );
		RtfCharSpecialPtr	pNewChar( new RtfCharSpecial() );
		pNewChar->m_eType				= RtfCharSpecial::rsc_chftnsepc;

		pNewPar->AddItem( pNewChar );
		oDocument.m_oEndnoteCon->AddItem( pNewPar );
		oDocument.m_oProperty.m_aSpecialEndnotes.push_back( 1 );
	}
	if( NULL == oDocument.m_oEndnoteSep )
	{
		oDocument.m_oEndnoteSep			= TextItemContainerPtr( new TextItemContainer() );
		RtfParagraphPtr		pNewPar	( new RtfParagraph() );
		RtfCharSpecialPtr	pNewChar( new RtfCharSpecial() );
		pNewChar->m_eType				= RtfCharSpecial::rsc_chftnsep;

		pNewPar->AddItem( pNewChar );
		oDocument.m_oEndnoteSep->AddItem( pNewPar );
		oDocument.m_oProperty.m_aSpecialEndnotes.push_back( 0 );
	}
}
void RtfNormalReader::SectDef( RtfDocument& oDocument, RtfReader& oReader )
{
	oReader.m_oCurSectionProp.SetDefaultRtf();

	//в соответствии с документацией ставим Page Information свойства как у документа
	oReader.m_oCurSectionProp.m_nPageWidth			= oDocument.m_oProperty.m_nPaperWidth;
	oReader.m_oCurSectionProp.m_nPageHeight			= oDocument.m_oProperty.m_nPaperHeight;
	oReader.m_oCurSectionProp.m_nMarginLeft			= oDocument.m_oProperty.m_nMarginLeft;
	oReader.m_oCurSectionProp.m_nMarginRight		= oDocument.m_oProperty.m_nMarginRight;
	oReader.m_oCurSectionProp.m_nMarginTop			= oDocument.m_oProperty.m_nMarginTop;
	oReader.m_oCurSectionProp.m_nMarginBottom		= oDocument.m_oProperty.m_nMarginBottom;
	oReader.m_oCurSectionProp.m_nGutterMarginWidth	= oDocument.m_oProperty.m_nGutterWidth;
	oReader.m_oCurSectionProp.m_bSwitchMargin		= oDocument.m_oProperty.m_bFacingPage;
	oReader.m_oCurSectionProp.m_bLandscapeFormat	= oDocument.m_oProperty.m_bLandScape;
}
bool RtfNormalReader::ExecuteCommand( RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter )
{
	if ("rtf" == sCommand) //open-rtf-document-image-error.rtf
	{
		if (oDocument.m_bStartRead)
		{
			RtfNormalReader oRtfReader(oDocument, oReader);
			return StartSubReader(oRtfReader, oDocument, oReader);
		}
		oDocument.m_bStartRead = true;
	}
	else if ( "colortbl" == sCommand )
	{
		RtfColorTableReader oColorTableReader;
		return StartSubReader( oColorTableReader, oDocument, oReader );
	}
	else if ( "fonttbl" == sCommand )
	{
		RtfFontTableReader oFontTableReader;
		return StartSubReader( oFontTableReader, oDocument, oReader );
	}
	else if ( "stylesheet" == sCommand )
	{
		RtfStyleTableReader oStyleReader;
		return StartSubReader( oStyleReader, oDocument, oReader );
	}
	//else if ( "latentstyles" == sCommand )
	//{
	//	LatentstyleTableReader oLatentstyleReader;
	//	return StartSubReader( oLatentstyleReader, oDocument, oReader );
	//}
	else if ( "listtable" == sCommand )
	{
		RtfListTableReader oListTableReader;
		return StartSubReader( oListTableReader, oDocument, oReader );

	}
	else if ( "listoverridetable" == sCommand )
	{
		RtfListOverrideTableReader oListOverrideReader;
		return StartSubReader( oListOverrideReader, oDocument, oReader );
	}
	else if ( "revtbl" == sCommand )
	{
		RtfRevisionTableReader oRevisionReader;
		return StartSubReader( oRevisionReader, oDocument, oReader );
	}
	else if ( "rsidtable" == sCommand )
	{
		//RSIDTableReader oRSIDReader;
		//return StartSubReader( oRSIDReader, oDocument, oReader );
	}
	else if ( "info" == sCommand )
	{
		RtfInfoReader oInfoReader;
		return StartSubReader( oInfoReader, oDocument, oReader );
	}
	else if ( "background" == sCommand )
	{
		oDocument.m_pBackground = RtfShapePtr( new RtfShape() );
		
		oDocument.m_pBackground->m_bBackground = true;

		RtfBackgroundReader oBackgroundReader( *oDocument.m_pBackground );
		return StartSubReader( oBackgroundReader, oDocument, oReader );
	}
	else if ( "colorschememapping" == sCommand )
	{
		RtfColorSchemeReader oSchemeReader;
		return StartSubReader( oSchemeReader, oDocument, oReader );
	}
	else if ( "themedata" == sCommand )
	{
		RtfThemeDataReader oThemeDataReader;
		bool res = StartSubReader( oThemeDataReader, oDocument, oReader );
		if (res)
		{

		}
	}
	else if ("datastore" == sCommand)
	{
		RtfDataStoreReader oDataStoreReader;
		bool res = StartSubReader(oDataStoreReader, oDocument, oReader);
		if (res)
		{

		}
	}
	else if ( "mmathPr" == sCommand )
	{
		oDocument.m_pMathProp = RtfMathPtr(new RtfMath());
		RtfMathReader oMathPropReader(oDocument.m_pMathProp);
		return StartSubReader( oMathPropReader, oDocument, oReader );
	}
	else if ( "ftnsep" == sCommand || "ftnsepc" == sCommand ||
			  "aftnsep"== sCommand || "aftnsepc" == sCommand )
	{
		RtfParagraphReader oParagraphReader(sCommand, oReader);
		StartSubReader( oParagraphReader, oDocument, oReader );
		if ( "ftnsep" == sCommand )
		{
			oDocument.m_oFootnoteSep = oParagraphReader.m_oParPropDest.m_oTextItems;
			oDocument.m_oProperty.m_aSpecialFootnotes.push_back( 0 );
		}
		else if ( "ftnsepc" == sCommand )
		{
			oDocument.m_oFootnoteCon = oParagraphReader.m_oParPropDest.m_oTextItems;
			oDocument.m_oProperty.m_aSpecialFootnotes.push_back( 1 );
		}
		else if ( "aftnsep" == sCommand )
		{
			oDocument.m_oEndnoteSep = oParagraphReader.m_oParPropDest.m_oTextItems;
			oDocument.m_oProperty.m_aSpecialEndnotes.push_back( 0 );
		}
		else if ( "aftnsepc" == sCommand )
		{
			oDocument.m_oEndnoteCon = oParagraphReader.m_oParPropDest.m_oTextItems;
			oDocument.m_oProperty.m_aSpecialEndnotes.push_back( 1 );
		}
	}
	else if ( "sect" == sCommand )
	{
		oParagraphReaderDestination.Finalize( oReader );

		_section section;
		if (oDocument.GetItem( section ) )
		{
			section.props->m_bFinalize = true;
			section.props->m_oProperty = oReader.m_oCurSectionProp;
		}
		_section new_section;
		new_section.props = RtfSectionPtr( new RtfSection() );

		oDocument.AddItem( new_section );
		
		oParagraphReaderDestination.m_oTextItems = new_section.props;

		//вручную обнуляем footer, т.к. sectd может встретиться и после field
		///?????
		oReader.m_oCurSectionProp.m_oHeaderLeft		= TextItemContainerPtr();
		oReader.m_oCurSectionProp.m_oHeaderFirst	= TextItemContainerPtr();
		oReader.m_oCurSectionProp.m_oHeaderRight	= TextItemContainerPtr();
		oReader.m_oCurSectionProp.m_oFooterLeft		= TextItemContainerPtr();
		oReader.m_oCurSectionProp.m_oFooterFirst	= TextItemContainerPtr();
		oReader.m_oCurSectionProp.m_oFooterRight	= TextItemContainerPtr();
	}
	else if ( "sectd" == sCommand  )
	{
		//default set to curent section
		SectDef( oDocument, oReader );
	}
	else if ("defchp" == sCommand)
	{
		RtfDefCharPropReader oDefCharPropReader;
		return StartSubReader(oDefCharPropReader, oDocument, oReader);
	}
	else if ("defpap" == sCommand)
	{
		RtfDefParPropReader oDefParPropReader;
		return StartSubReader(oDefParPropReader, oDocument, oReader);
	}
	else
	{
		bool bResult = false;
		bResult = oParagraphReaderDestination.ExecuteCommand( oDocument, oReader, (*this), sCommand, hasParameter, parameter );
		if (bResult)
			return true;
		bResult = RtfDocumentCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter );
		if ( bResult )
			return true;
		bResult = oRtfSectionCommand.ExecuteCommand( oDocument, oReader, (*this), sCommand, hasParameter, parameter );
		if ( bResult )
			return true;
		bResult = RtfDocumentCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter );
		if ( bResult )
			return true;
		return false;
	}
	return true;
}

RtfParagraphReader::RtfParagraphReader(std::string sHeader, RtfReader& oReader) : m_sHeader(sHeader)
{
	if (PROP_DEF != oReader.m_oState->m_oParagraphProp.m_nItap)
		m_oParPropDest.nTargetItap = oReader.m_oState->m_oParagraphProp.m_nItap;
}
bool RtfParagraphReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
{
	if (m_sHeader == sCommand)
		return true;
	else if ("rtf" == sCommand)
	{
		RtfNormalReader oRtfReader(oDocument, oReader);
		oDocument.m_bStartRead = false;
		oRtfReader.oParagraphReaderDestination = m_oParPropDest;
		StartSubReader(oRtfReader, oDocument, oReader);

		m_oParPropDest = oRtfReader.oParagraphReaderDestination;
		return true;
	}
	else
		return m_oParPropDest.ExecuteCommand(oDocument, oReader, (*this), sCommand, hasParameter, parameter);
}
void RtfParagraphReader::ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText)
{
	m_oParPropDest.ExecuteText(oDocument, oReader, sText);
}
void RtfParagraphReader::ExitReader(RtfDocument& oDocument, RtfReader& oReader)
{
	m_oParPropDest.Finalize(oReader);
}

bool RtfBorderCommand::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter, RtfBorder& oOutput)
{
	if( "brdrs"			== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrs;
	else if( "brdrth"			== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrth;
	else if( "brdrsh"			== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrsh;
	else if( "brdrdb"			== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrdb;
	else if( "brdrdot"			== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrdot;
	else if( "brdrdash"			== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrdash;
	else if( "brdrhair"			== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrhair;
	else if( "brdrdashsm"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrdashsm;
	else if( "brdrdashd"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrdashd;
	else if( "brdrdashdd"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrdashdd;
	else if( "brdrdashdot"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrdashdot;
	else if( "brdrdashdotdot"	== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrdashdot;
	else if( "brdrtriple"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrtriple;
	else if( "brdrtnthsg"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrtnthsg;
	else if( "brdrthtnsg"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrthtnsg;
	else if( "brdrtnthtnsg"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrtnthtnsg;
	else if( "brdrtnthmg"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrtnthmg;
	else if( "brdrthtnmg"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrthtnmg;
	else if( "brdrtnthtnmg"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrtnthtnmg;
	else if( "brdrtnthlg"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrtnthlg;
	else if( "brdrthtnlg"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrthtnlg;
	else if( "brdrtnthtnlg"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrtnthtnlg;
	else if( "brdrwavy"			== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrwavy;
	else if( "brdrwavydb"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrwavydb;
	else if( "brdrdashdotstr"	== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrdashdotstr;
	else if( "brdremboss"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdremboss;
	else if( "brdrengrave"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrengrave;
	else if( "brdroutset"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdroutset;
	else if( "brdrinset"		== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrinset;
	else if( "brdrnone"			== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrnone;
	else if( "brdrnil"			== sCommand )	oOutput.m_eType = RtfBorder::bt_none;
	else if( "brdrn"			== sCommand )	oOutput.m_eType = RtfBorder::bt_brdrnone;
	else if( "brdrw"			== sCommand )
	{
		if( true == hasParameter )
		{
			oOutput.m_nWidth = parameter;
			if( RtfBorder::bt_none == oOutput.m_eType )
				oOutput.m_eType = RtfBorder::bt_brdrs;
		}
	}
	else if( "brsp" == sCommand )
	{
		if( true == hasParameter )
		{
			oOutput.m_nSpace = parameter;
			if( RtfBorder::bt_none == oOutput.m_eType )
				oOutput.m_eType = RtfBorder::bt_brdrs;
		}
	}
	else if( "brdrcf" == sCommand )
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

RtfSectionCommand::RtfSectionCommand()
{
	m_eInternalState	= is_none;
	nCurCollumnNumber	= PROP_DEF;
}
bool RtfSectionCommand::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, RtfAbstractReader& oAbstrReader,  std::string sCommand, bool hasParameter, int parameter)
{
	if ( "11111111" == sCommand )
		;
	COMMAND_RTF_BOOL( "rtlsect",	oReader.m_oCurSectionProp.m_bBidi,				sCommand, hasParameter, parameter)
			COMMAND_RTF_INT	( "binfsxn",	oReader.m_oCurSectionProp.m_nPaperSourceFirst,	sCommand, hasParameter, parameter)
			COMMAND_RTF_INT	( "binsxn",	oReader.m_oCurSectionProp.m_nPaperSourceOther,	sCommand, hasParameter, parameter)
			COMMAND_RTF_BOOL( "rtlgutter",	oReader.m_oCurSectionProp.m_bRtlGutter,			sCommand, hasParameter, parameter)
			COMMAND_RTF_BOOL( "endnhere",	oReader.m_oCurSectionProp.m_bEndnotes,			sCommand, hasParameter, parameter)
			COMMAND_RTF_INT	( "ds",		oReader.m_oCurSectionProp.m_nStyle,				sCommand, hasParameter, parameter)
			COMMAND_RTF_INT	( "sbknone",	oReader.m_oCurSectionProp.m_eSectionBreak,		sCommand, true, RtfSectionProperty::sb_sbknone)
			COMMAND_RTF_INT	( "sbkcol",	oReader.m_oCurSectionProp.m_eSectionBreak,		sCommand, true, RtfSectionProperty::sb_sbkcol)
			COMMAND_RTF_INT	( "sbkpage",	oReader.m_oCurSectionProp.m_eSectionBreak,		sCommand, true, RtfSectionProperty::sb_sbkpage)
			COMMAND_RTF_INT	( "sbkeven",	oReader.m_oCurSectionProp.m_eSectionBreak,		sCommand, true, RtfSectionProperty::sb_sbkeven)
			COMMAND_RTF_INT	( "sbkodd",	oReader.m_oCurSectionProp.m_eSectionBreak,		sCommand, true, RtfSectionProperty::sb_sbkodd)
			else if ( "cols" == sCommand )
	{
		if ( hasParameter )
		{
			oReader.m_oCurSectionProp.m_nColumnNumber = parameter;
			oReader.m_oCurSectionProp.m_oCollumnProperty.m_aCollumnProperty.resize( parameter );
		}
	}
	COMMAND_RTF_INT ( "colsx", oReader.m_oCurSectionProp.m_nColumnSpace, sCommand, hasParameter, parameter)
			else if ( "colno" == sCommand )
	{
		if ( hasParameter )
			nCurCollumnNumber = parameter - 1;
	}
	else if ( "colsr" == sCommand )
	{
		if ( hasParameter )
			if ( nCurCollumnNumber >= 0 && nCurCollumnNumber < (int)oReader.m_oCurSectionProp.m_oCollumnProperty.m_aCollumnProperty.size() )
				oReader.m_oCurSectionProp.m_oCollumnProperty.m_aCollumnProperty[ nCurCollumnNumber ].m_nColumnSpaceToRightOfCol = parameter;
	}
	else if ( "colw" == sCommand )
	{
		if ( hasParameter )
			if ( nCurCollumnNumber >= 0 && nCurCollumnNumber < (int)oReader.m_oCurSectionProp.m_oCollumnProperty.m_aCollumnProperty.size() )
				oReader.m_oCurSectionProp.m_oCollumnProperty.m_aCollumnProperty[ nCurCollumnNumber ].m_nColumnWidth = parameter;
	}
	COMMAND_RTF_BOOL( "linebetcol",	oReader.m_oCurSectionProp.m_bColumnLineBetween, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT	( "sftntj",		oReader.m_oCurSectionProp.m_eFootnotesJust,		sCommand, true, RtfSectionProperty::fj_sftntj)
			COMMAND_RTF_INT	( "sftnbj",		oReader.m_oCurSectionProp.m_eFootnotesJust,		sCommand, true, RtfSectionProperty::fj_sftnbj)
			//Footnotes and Endnotes
			COMMAND_RTF_INT	( "sftnstart",		oReader.m_oCurSectionProp.m_nFootnotesStart,	sCommand, hasParameter, parameter)
			COMMAND_RTF_INT	( "sftnrstpg",		oReader.m_oCurSectionProp.m_eFootnotesRestart,	sCommand, true, RtfSectionProperty::fr_sftnrstpg)
			COMMAND_RTF_INT	( "sftnrestart",	oReader.m_oCurSectionProp.m_eFootnotesRestart,	sCommand, true, RtfSectionProperty::fr_sftnrestart)
			COMMAND_RTF_INT	( "sftnrstcont",	oReader.m_oCurSectionProp.m_eFootnotesRestart,	sCommand, true, RtfSectionProperty::fr_sftnrstcont)
			COMMAND_RTF_INT	( "sftnnar",		oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnnar)
			COMMAND_RTF_INT	( "sftnnalc",		oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnnalc)
			COMMAND_RTF_INT	( "sftnnauc",		oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnnauc)
			COMMAND_RTF_INT	( "sftnnrlc",		oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnnrlc)
			COMMAND_RTF_INT	( "sftnnruc",		oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnnruc)
			COMMAND_RTF_INT	( "sftnnchi",		oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnnchi)
			COMMAND_RTF_INT	( "sftnnchosung",	oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnnchosung)
			COMMAND_RTF_INT	( "sftnncnum",		oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnncnum)
			COMMAND_RTF_INT	( "sftnndbnum",	oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnndbnum)
			COMMAND_RTF_INT	( "sftnndbnumd",	oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnndbnumd)
			COMMAND_RTF_INT	( "sftnndbnumt",	oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnndbnumt)
			COMMAND_RTF_INT	( "sftnndbnumk",	oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnndbnumk)
			COMMAND_RTF_INT	( "sftnndbar",		oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnndbar)
			COMMAND_RTF_INT	( "sftnnganada",	oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnnganada)
			COMMAND_RTF_INT	( "sftnngbnum",	oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnngbnum)
			COMMAND_RTF_INT	( "sftnngbnumd",	oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnngbnumd)
			COMMAND_RTF_INT	( "sftnngbnuml",	oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnngbnuml)
			COMMAND_RTF_INT	( "sftnngbnumk",	oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnngbnumk)
			COMMAND_RTF_INT	( "sftnnzodiac",	oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnnzodiac)
			COMMAND_RTF_INT	( "sftnnzodiacd",	oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnnzodiacd)
			COMMAND_RTF_INT	( "sftnnzodiacl",	oReader.m_oCurSectionProp.m_eFootnotesFormat,	sCommand, true, RtfSectionProperty::ff_sftnnzodiacl)

			COMMAND_RTF_INT	( "saftnstart",	oReader.m_oCurSectionProp.m_nEndnotesStart,		sCommand, hasParameter, parameter)
			COMMAND_RTF_INT	( "saftnrestart",	oReader.m_oCurSectionProp.m_eEndnotesRestart,	sCommand, true, RtfSectionProperty::er_saftnrestart)
			COMMAND_RTF_INT	( "saftnrstcont",	oReader.m_oCurSectionProp.m_eEndnotesRestart,	sCommand, true, RtfSectionProperty::er_saftnrstcont)
			COMMAND_RTF_INT	( "saftnnar",		oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnnar)
			COMMAND_RTF_INT	( "saftnnalc",		oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnnalc)
			COMMAND_RTF_INT	( "saftnnauc",		oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnnauc)
			COMMAND_RTF_INT	( "saftnnrlc",		oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnnrlc)
			COMMAND_RTF_INT	( "saftnnruc",		oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnnruc)
			COMMAND_RTF_INT	( "saftnnchi",		oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnnchi)
			COMMAND_RTF_INT	( "saftnnchosung",	oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnnchosung)
			COMMAND_RTF_INT	( "saftnncnum",	oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnncnum)
			COMMAND_RTF_INT	( "saftnndbnum",	oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnndbnum)
			COMMAND_RTF_INT	( "saftnndbnumd",	oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnndbnumd)
			COMMAND_RTF_INT	( "saftnndbnumt",	oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnndbnumt)
			COMMAND_RTF_INT	( "saftnndbnumk",	oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnndbnumk)
			COMMAND_RTF_INT	( "saftnndbar",	oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnndbar)
			COMMAND_RTF_INT	( "saftnnganada",	oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnnganada)
			COMMAND_RTF_INT	( "saftnngbnum",	oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnngbnum)
			COMMAND_RTF_INT	( "saftnngbnumd",	oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnngbnumd)
			COMMAND_RTF_INT	( "saftnngbnuml",	oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnngbnuml)
			COMMAND_RTF_INT	( "saftnngbnumk",	oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnngbnumk)
			COMMAND_RTF_INT	( "saftnnzodiac",	oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnnzodiac)
			COMMAND_RTF_INT	( "saftnnzodiacd",	oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnnzodiacd)
			COMMAND_RTF_INT	( "saftnnzodiacl",	oReader.m_oCurSectionProp.m_eEndnotesFormat,	sCommand, true, RtfSectionProperty::ef_saftnnzodiacl)
			//Line Numbering
			COMMAND_RTF_INT	( "linemod",		oReader.m_oCurSectionProp.m_nLineModulus,		sCommand, hasParameter, parameter)
			COMMAND_RTF_INT	( "linex",			oReader.m_oCurSectionProp.m_nLineX,				sCommand, hasParameter, parameter)
			COMMAND_RTF_INT	( "linestarts",	oReader.m_oCurSectionProp.m_nLineStart,			sCommand, hasParameter, parameter)
			COMMAND_RTF_INT	( "linerestart",	oReader.m_oCurSectionProp.m_eLineNumberRestart, sCommand, true, RtfSectionProperty::lnr_linerestart)
			COMMAND_RTF_INT	( "lineppage",		oReader.m_oCurSectionProp.m_eLineNumberRestart, sCommand, true, RtfSectionProperty::lnr_lineppage)
			COMMAND_RTF_INT	( "linecont",		oReader.m_oCurSectionProp.m_eLineNumberRestart, sCommand, true, RtfSectionProperty::lnr_linecont)
			//Page Information
			COMMAND_RTF_INT	( "pgwsxn",		oReader.m_oCurSectionProp.m_nPageWidth,			sCommand, hasParameter, parameter)
			COMMAND_RTF_INT	( "pghsxn",		oReader.m_oCurSectionProp.m_nPageHeight,		sCommand, hasParameter, parameter)
			COMMAND_RTF_INT	( "marglsxn",		oReader.m_oCurSectionProp.m_nMarginLeft,		sCommand, hasParameter, parameter)
			COMMAND_RTF_INT	( "margrsxn",		oReader.m_oCurSectionProp.m_nMarginRight,		sCommand, hasParameter, parameter)
			COMMAND_RTF_INT	( "margtsxn",		oReader.m_oCurSectionProp.m_nMarginTop,			sCommand, hasParameter, parameter)
			COMMAND_RTF_INT	( "margbsxn",		oReader.m_oCurSectionProp.m_nMarginBottom,		sCommand, hasParameter, parameter)
			COMMAND_RTF_INT	( "guttersxn",		oReader.m_oCurSectionProp.m_nGutterMarginWidth, sCommand, hasParameter, parameter)
			COMMAND_RTF_BOOL( "margmirsxn",	oReader.m_oCurSectionProp.m_bSwitchMargin,		sCommand, hasParameter, parameter)
			COMMAND_RTF_BOOL( "lndscpsxn",		oReader.m_oCurSectionProp.m_bLandscapeFormat,	sCommand, hasParameter, parameter)
			COMMAND_RTF_BOOL( "titlepg",		oReader.m_oCurSectionProp.m_bTitlePage,			sCommand, hasParameter, parameter)
			//else if ( "headery" == sCommand )
			//{
			//	static int nCount = 0;
			//	nCount++;
			//}
			COMMAND_RTF_INT	( "headery",	oReader.m_oCurSectionProp.m_nHeaderTop,			sCommand, hasParameter, parameter)
			COMMAND_RTF_INT	( "footery",	oReader.m_oCurSectionProp.m_nFooterBottom,		sCommand, hasParameter, parameter)
			//Page Numbers
			COMMAND_RTF_INT	( "pgnstarts",	oReader.m_oCurSectionProp.m_nPageNumberStart,	sCommand, hasParameter, parameter)
			COMMAND_RTF_BOOL( "pgncont",	oReader.m_oCurSectionProp.m_bPageNumberContinuos, sCommand, hasParameter, parameter)
			COMMAND_RTF_BOOL( "pgnrestart", oReader.m_oCurSectionProp.m_bPageNumberRestart, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT	( "pgnx",		oReader.m_oCurSectionProp.m_nPageNumberX,		sCommand, hasParameter, parameter)
			COMMAND_RTF_INT	( "pgny",		oReader.m_oCurSectionProp.m_nPageNumberY,		sCommand, hasParameter, parameter)
			COMMAND_RTF_INT	( "pgndec",	oReader.m_oCurSectionProp.m_ePageNumberFormat,	sCommand, true, RtfSectionProperty::pnf_pgndec)
			COMMAND_RTF_INT	( "pgnucrm",	oReader.m_oCurSectionProp.m_ePageNumberFormat,	sCommand, true, RtfSectionProperty::pnf_pgnucrm)
			COMMAND_RTF_INT	( "pgnlcrm",	oReader.m_oCurSectionProp.m_ePageNumberFormat,	sCommand, true, RtfSectionProperty::pnf_pgnlcrm)
			COMMAND_RTF_INT	( "vertalt",	oReader.m_oCurSectionProp.m_eVerticalAlignment, sCommand, true, RtfSectionProperty::va_vertalt)
			COMMAND_RTF_INT	( "vertalb",	oReader.m_oCurSectionProp.m_eVerticalAlignment, sCommand, true, RtfSectionProperty::va_vertalb)
			COMMAND_RTF_INT	( "vertalc",	oReader.m_oCurSectionProp.m_eVerticalAlignment, sCommand, true, RtfSectionProperty::va_vertalc)
			COMMAND_RTF_INT	( "vertalj",	oReader.m_oCurSectionProp.m_eVerticalAlignment, sCommand, true, RtfSectionProperty::va_vertalj)
			//Text Flow
			COMMAND_RTF_INT	( "stextflow",	oReader.m_oCurSectionProp.m_nTextFollow,		sCommand, hasParameter, parameter)

			//Page Borders
			else if ( "pgbrdrl" == sCommand )	m_eInternalState = is_border_left;
	else if ( "pgbrdrt" == sCommand )	m_eInternalState = is_border_top;
	else if ( "pgbrdrr" == sCommand )	m_eInternalState = is_border_right;
	else if ( "pgbrdrb" == sCommand )	m_eInternalState = is_border_bottom;

	COMMAND_RTF_INT	( "brdrart",	oReader.m_oCurSectionProp.m_nBorderArt,		sCommand, hasParameter, parameter)
			COMMAND_RTF_INT	( "pgbrdropt",	oReader.m_oCurSectionProp.m_nBorderMeasure, sCommand, hasParameter, parameter)
			COMMAND_RTF_BOOL( "pgbrdrsna",	oReader.m_oCurSectionProp.m_nBorderAlign,	sCommand, hasParameter, parameter)

			COMMAND_RTF_INT	( "srauth",	oReader.m_oCurSectionProp.m_nSrAuth, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT	( "srdate",	oReader.m_oCurSectionProp.m_nSrDate, sCommand, hasParameter, parameter)

			else if (   "header" == sCommand ||   "footer" == sCommand ||	"headerl" == sCommand ||
						"headerr" == sCommand ||  "headerf" == sCommand ||	"footerl" == sCommand ||
						"footerr" == sCommand ||  "footerf" == sCommand )
	{
		RtfParagraphReader oParagraphReader(sCommand, oReader);
		oAbstrReader.StartSubReader( oParagraphReader, oDocument, oReader );
		
		oParagraphReader.m_oParPropDest.m_bPar = true;
		oParagraphReader.m_oParPropDest.Finalize( oReader );
		
		TextItemContainerPtr oNewFooterHeader = oParagraphReader.m_oParPropDest.m_oTextItems;

		if		( "header"		== sCommand )	oReader.m_oCurSectionProp.m_oHeaderRight = oNewFooterHeader;
		else if ( "footer"		== sCommand )	oReader.m_oCurSectionProp.m_oFooterRight = oNewFooterHeader;
		else if ( "headerl"	== sCommand )	oReader.m_oCurSectionProp.m_oHeaderLeft = oNewFooterHeader;
		else if ( "headerr"	== sCommand )	oReader.m_oCurSectionProp.m_oHeaderRight = oNewFooterHeader;
		else if ( "headerf"	== sCommand )	oReader.m_oCurSectionProp.m_oHeaderFirst = oNewFooterHeader;
		else if ( "footerl"	== sCommand )	oReader.m_oCurSectionProp.m_oFooterLeft = oNewFooterHeader;
		else if ( "footerr"	== sCommand )	oReader.m_oCurSectionProp.m_oFooterRight = oNewFooterHeader;
		else if ( "footerf"	== sCommand )	oReader.m_oCurSectionProp.m_oFooterFirst = oNewFooterHeader;
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


bool RtfShadingCommand::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,  std::string sCommand, bool hasParameter, int parameter, RtfShading& oOutput )
{
	if		( "bghoriz"	== sCommand )	oOutput.m_eType = RtfShading::st_chbghoriz;
	else if ( "bgvert"		== sCommand )	oOutput.m_eType = RtfShading::st_chbgvert;
	else if ( "bgfdiag"	== sCommand )	oOutput.m_eType = RtfShading::st_chbgfdiag;
	else if ( "bgbdiag"	== sCommand )	oOutput.m_eType = RtfShading::st_chbgbdiag;
	else if ( "bgcross"	== sCommand )	oOutput.m_eType = RtfShading::st_chbgcross;
	else if ( "bgdcross"	== sCommand )	oOutput.m_eType = RtfShading::st_chbgdcross;
	else if ( "bgdkhoriz"	== sCommand )	oOutput.m_eType = RtfShading::st_chbgdkhoriz;
	else if ( "bgdkvert"	== sCommand )	oOutput.m_eType = RtfShading::st_chbgdkvert;
	else if ( "bgdkfdiag"	== sCommand )	oOutput.m_eType = RtfShading::st_chbgdkfdiag;
	else if ( "bgdkbdiag"	== sCommand )	oOutput.m_eType = RtfShading::st_chbgdkbdiag;
	else if ( "bgdkcross"	== sCommand )	oOutput.m_eType = RtfShading::st_chbgdkcross;
	else if ( "bgdkdcross"	== sCommand )	oOutput.m_eType = RtfShading::st_chbgdkdcross;
	else if ( "cfpat"		== sCommand )
	{
		if ( hasParameter )
			oOutput.m_nForeColor = parameter;
	}
	else if ( "cbpat" == sCommand )
	{
		if ( hasParameter )
			oOutput.m_nBackColor = parameter;
	}
	else if ( "shading" == sCommand )
	{
		if ( hasParameter )
			oOutput.m_nValue = parameter;
	}
	else
		return false;
	return true;
}
bool RtfShadingCellCommand::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter, RtfShading& oOutput)
{
	//для свойст таблицы и для стилей таблицы
	if ( "clshdrawnil" == sCommand )
		oOutput.m_eType = RtfShading::st_clshdrawnil;
	else if ( "rawclbgvert" == sCommand || "clbgvert" == sCommand || "tsbgvert" == sCommand )
		oOutput.m_eType = RtfShading::st_chbgvert;
	else if ( "rawclbgfdiag" == sCommand || "clbgfdiag" == sCommand || "tsbgfdiag" == sCommand )
		oOutput.m_eType = RtfShading::st_chbgfdiag;
	else if ( "rawclbgbdiag" == sCommand || "clbgbdiag" == sCommand || "tsbgbdiag" == sCommand )
		oOutput.m_eType = RtfShading::st_chbgbdiag;
	else if ( "rawclbgcross" == sCommand || "clbgcross" == sCommand || "tsbgcross" == sCommand )
		oOutput.m_eType = RtfShading::st_chbgcross;
	else if ( "rawclbgdcross" == sCommand || "clbgdcross" == sCommand || "tsbgdcross" == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdcross;
	else if ( "rawclbgdkhor" == sCommand || "clbgdkhor" == sCommand || "tsbgdkhor" == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkhoriz;
	else if ( "rawclbgdkvert" == sCommand || "clbgdkvert" == sCommand || "tsbgdkvert" == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkvert;
	else if ( "rawclbgdkfdiag" == sCommand || "clbgdkfdiag" == sCommand || "tsbgdkfdiag" == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkfdiag;
	else if ( "rawclbgdkbdiag" == sCommand || "clbgdkbdiag" == sCommand || "tsbgdkbdiag" == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkbdiag;
	else if ( "rawclbgdkcross" == sCommand || "clbgdkcross" == sCommand || "tsbgdkcross" == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkcross;
	else if ( "rawclbgdkdcross" == sCommand || "clbgdkdcross" == sCommand || "tsbgdkdcross" == sCommand )
		oOutput.m_eType = RtfShading::st_chbgdkdcross;
	
	COMMAND_RTF_INT ( "clcfpat",		oOutput.m_nForeColor, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "clcbpat",		oOutput.m_nBackColor, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "clcfpatraw",	oOutput.m_nForeColor, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "clcbpatraw",	oOutput.m_nBackColor, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "tscellcfpat",	oOutput.m_nForeColor, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "tscellcbpat",	oOutput.m_nBackColor, sCommand, hasParameter, parameter )

			COMMAND_RTF_INT ( "clshdng",		oOutput.m_nValue, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "clshdngraw",	oOutput.m_nValue, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "tscellpct",		oOutput.m_nValue, sCommand, hasParameter, parameter )
			else
			return false;

	if (sCommand == "clshdng")		oReader.m_oState->m_oCellProperty.m_nShadingPctFrom = 1;
	if (sCommand == "clshdngraw")	oReader.m_oState->m_oCellProperty.m_nShadingPctFrom = 2;
	return true;
}

bool RtfShadingCharCommand::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter, RtfShading& oOutput )
{
	if		( "chbghoriz"		== sCommand )		oOutput.m_eType = RtfShading::st_chbghoriz;
	else if ( "chbgvert"		== sCommand )		oOutput.m_eType = RtfShading::st_chbgvert;
	else if ( "chbgfdiag"		== sCommand )		oOutput.m_eType = RtfShading::st_chbgfdiag;
	else if ( "chbgbdiag"		== sCommand )		oOutput.m_eType = RtfShading::st_chbgbdiag;
	else if ( "chbgcross"		== sCommand )		oOutput.m_eType = RtfShading::st_chbgcross;
	else if ( "chbgdcross"		== sCommand )		oOutput.m_eType = RtfShading::st_chbgdcross;
	else if ( "chbgdkhoriz"	== sCommand )		oOutput.m_eType = RtfShading::st_chbgdkhoriz;
	else if ( "chbgdkvert"		== sCommand )		oOutput.m_eType = RtfShading::st_chbgdkvert;
	else if ( "chbgdkfdiag"	== sCommand )		oOutput.m_eType = RtfShading::st_chbgdkfdiag;
	else if ( "chbgdkbdiag"	== sCommand )		oOutput.m_eType = RtfShading::st_chbgdkbdiag;
	else if ( "chbgdkcross"	== sCommand )		oOutput.m_eType = RtfShading::st_chbgdkcross;
	else if ( "chbgdkdcross"	== sCommand )		oOutput.m_eType = RtfShading::st_chbgdkdcross;
	else if ( "chcfpat"		== sCommand )
	{
		if ( hasParameter )
			oOutput.m_nForeColor = parameter;
	}
	else if ( "chcbpat" == sCommand )
	{
		if ( hasParameter )
			oOutput.m_nBackColor = parameter;
	}
	else if ( "chshdng" == sCommand )
	{
		if ( hasParameter )
			oOutput.m_nValue = parameter;
	}
	else
		return false;
	return true;
}

bool RtfShadingRowCommand::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter, RtfShading& oOutput)
{
	if		( "trbghoriz"	== sCommand )		oOutput.m_eType = RtfShading::st_chbghoriz;
	else if ( "trbgvert"	== sCommand )		oOutput.m_eType = RtfShading::st_chbgvert;
	else if	( "trbgfdiag"	== sCommand )		oOutput.m_eType = RtfShading::st_chbgfdiag;
	else if	( "trbgbdiag"	== sCommand )		oOutput.m_eType = RtfShading::st_chbgbdiag;
	else if	( "trbgcross"	== sCommand )		oOutput.m_eType = RtfShading::st_chbgcross;
	else if	( "trbgdcross"	== sCommand )		oOutput.m_eType = RtfShading::st_chbgdcross;
	else if	( "trbgdkhor"	== sCommand )		oOutput.m_eType = RtfShading::st_chbgdkhoriz;
	else if	( "trbgdkvert"	== sCommand )		oOutput.m_eType = RtfShading::st_chbgdkvert;
	else if	( "trbgdkfdiag"	== sCommand )		oOutput.m_eType = RtfShading::st_chbgdkfdiag;
	else if	( "trbgdkbdiag"	== sCommand )		oOutput.m_eType = RtfShading::st_chbgdkbdiag;
	else if	( "trbgdkcross"	== sCommand )		oOutput.m_eType = RtfShading::st_chbgdkcross;
	else if	( "trbgdkdcross"== sCommand )		oOutput.m_eType = RtfShading::st_chbgdkdcross;
	else if	( "trcfpat"		== sCommand )
	{
		if ( hasParameter )
			oOutput.m_nForeColor = parameter;
	}
	else if ( "trcbpat" == sCommand )
	{
		if ( hasParameter )
			oOutput.m_nBackColor = parameter;
	}
	else if ( "trshdng" == sCommand )
	{
		if ( hasParameter )
			oOutput.m_nValue = parameter;
	}
	else
		return false;
	return true;
}	
bool RtfCharPropsCommand::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter, RtfCharProperty * charProps, bool bLookOnBorder)
{
	if (!charProps) return false;

	if ( "plain" == sCommand )
		charProps->SetDefaultRtf();

	COMMAND_RTF_INT	( "animtext"	,	charProps->m_nAnimated,			sCommand, hasParameter, parameter)
			COMMAND_RTF_BOOL( "b"			,	charProps->m_bBold,				sCommand, hasParameter, parameter)
			COMMAND_RTF_BOOL( "caps"		,	charProps->m_bCaps,				sCommand, hasParameter, parameter)
			COMMAND_RTF_INT	( "charscalex"	,	charProps->m_nScalex,			sCommand, hasParameter, parameter)
			COMMAND_RTF_INT	( "cs"			,	charProps->m_nCharStyle,		sCommand, hasParameter, parameter)
			COMMAND_RTF_INT	( "down"		,	charProps->m_nDown,				sCommand, hasParameter, parameter)
			COMMAND_RTF_BOOL( "embo"		,	charProps->m_bEmbo,				sCommand, hasParameter, parameter)
			COMMAND_RTF_INT	( "expndtw"	,	charProps->m_nCharacterSpacing,	sCommand, hasParameter, parameter)

			else if ( "expnd" == sCommand )
	{
		if ( hasParameter )
			charProps->m_nCharacterSpacing  = 5 * parameter; //quater -points
	}
	COMMAND_RTF_INT	( "fittext"		, charProps->m_nFitText, sCommand, hasParameter, parameter)
			else if ("f" == sCommand && hasParameter)
	{
		charProps->m_nFont = parameter;
		oReader.m_nDefFont = charProps->m_nFont; //reset
	}
	COMMAND_RTF_INT	( "fs"			, charProps->m_nFontSize, sCommand, hasParameter, parameter)
			COMMAND_RTF_BOOL( "i"			, charProps->m_bItalic, sCommand, hasParameter, parameter)
			COMMAND_RTF_BOOL( "impr"		, charProps->m_bImprint, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT	( "kerning"		, charProps->m_nKerning, sCommand, hasParameter, parameter)
			else if ( "ltrch" == sCommand )
	{
		if ( false == hasParameter || 0 != parameter )
			charProps->m_bRightToLeft  = 0;
		else
			charProps->m_bRightToLeft  = 1;
	}
	COMMAND_RTF_BOOL( "rtlch",		charProps->m_bRightToLeft,	sCommand, hasParameter, parameter)
			COMMAND_RTF_INT	( "lang",		charProps->m_nLanguage,		sCommand, hasParameter, parameter)
			COMMAND_RTF_INT	( "langfe",		charProps->m_nLanguageAsian,sCommand, hasParameter, parameter)

			COMMAND_RTF_BOOL( "cs",		charProps->m_nComplexScript, sCommand, hasParameter, parameter)
			COMMAND_RTF_BOOL( "outl",		charProps->m_bOutline,		sCommand, hasParameter, parameter)
			COMMAND_RTF_BOOL( "scaps",		charProps->m_bScaps,		sCommand, hasParameter, parameter)
			COMMAND_RTF_BOOL( "shad",		charProps->m_bShadow,		sCommand, hasParameter, parameter)
			COMMAND_RTF_BOOL( "strike",	charProps->m_bStrike,		sCommand, hasParameter, parameter)
			COMMAND_RTF_BOOL( "striked",	charProps->m_nStriked,		sCommand, hasParameter, parameter)
			COMMAND_RTF_BOOL( "sub",		charProps->m_bSub,			sCommand, hasParameter, parameter)
			COMMAND_RTF_BOOL( "super",		charProps->m_bSuper,		sCommand, hasParameter, parameter)
			COMMAND_RTF_INT ( "highlight",	charProps->m_nHightlited,	sCommand, hasParameter, parameter)
			else if ( "cf" == sCommand )
	{
		if ( hasParameter )
			charProps->m_nForeColor = parameter;
		else
			charProps->m_nForeColor= PROP_DEF;
	}
	else if ( "ul" == sCommand )
	{
		if ( hasParameter && 0 == parameter)
			charProps->m_eUnderStyle = RtfCharProperty::uls_none;
		else
			charProps->m_eUnderStyle = RtfCharProperty::uls_Single;
	}
	//COMMAND_RTF_BOOL( "ul", charProps->m_bUnderline, sCommand, hasParameter, parameter)
	COMMAND_RTF_INT ( "ulc", charProps->m_nUnderlineColor, sCommand, hasParameter, parameter)

			COMMAND_RTF_INT ( "uld",		charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_Dotted)
			COMMAND_RTF_INT ( "uldash",		charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_Dashed)
			COMMAND_RTF_INT ( "uldashd",	charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_Dash_dotted)
			COMMAND_RTF_INT ( "uldashdd",	charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_Dash_dot_dotted)
			COMMAND_RTF_INT ( "uldb",		charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_Double)
			COMMAND_RTF_INT ( "ulhwave",	charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_Heavy_wave)
			COMMAND_RTF_INT ( "ulldash",	charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_Long_dashe)
			COMMAND_RTF_INT ( "ulnone",		charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_none)
			COMMAND_RTF_INT ( "ulth",		charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_Thick)
			COMMAND_RTF_INT ( "ulthd",		charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_Thick_dotted)
			COMMAND_RTF_INT ( "ulthdash",	charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_Thick_dashed)
			COMMAND_RTF_INT ( "ulthdashd",	charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_Thick_dash_dotted)
			COMMAND_RTF_INT ( "ulthdashdd",	charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_Thick_dash_dot_dotted)
			COMMAND_RTF_INT ( "ulthldash",	charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_Thick_long_dashed)
			COMMAND_RTF_INT ( "ululdbwave",	charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_Double_wave)
			COMMAND_RTF_INT ( "ulw",		charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_Word)
			COMMAND_RTF_INT ( "ulwave",		charProps->m_eUnderStyle,	sCommand, true, RtfCharProperty::uls_Wave)

			COMMAND_RTF_INT ( "up",			charProps->m_nUp,			sCommand, hasParameter, parameter)

			COMMAND_RTF_INT ( "crauth",		charProps->m_nCrAuth,		sCommand, hasParameter, parameter)
			COMMAND_RTF_INT ( "crdate",		charProps->m_nCrDate,		sCommand, hasParameter, parameter)
			COMMAND_RTF_INT ( "insrsid",	charProps->m_nInsrsid,		sCommand, hasParameter, parameter)

			COMMAND_RTF_INT ( "revauth",	charProps->m_nRevauth,		sCommand, hasParameter, parameter)
			COMMAND_RTF_INT ( "revdttm",	charProps->m_nRevdttm,		sCommand, hasParameter, parameter)
			COMMAND_RTF_INT ( "revauthdel",	charProps->m_nRevauthDel,	sCommand, hasParameter, parameter)
			COMMAND_RTF_INT ( "revdttmdel",	charProps->m_nRevdttmDel,	sCommand, hasParameter, parameter)

			else if ( "revised" == sCommand )
	{
		charProps->m_nRevised = 1;
	}
	else if ( "v" == sCommand )
	{
		charProps->m_bHidden = 1;
	}
	else if ( "deleted" == sCommand )
	{
		charProps->m_nDeleted = 1;
	}
	else if ( "nosupersub" == sCommand )
	{
		charProps->m_bSub	= 0;
		charProps->m_bSuper  = 0;
	}
	else if ( "nosectexpand" == sCommand )
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
bool RtfParagraphPropsCommand::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,  std::string sCommand, bool hasParameter, int parameter, RtfParagraphProperty * paragraphProps)
{
	if (!paragraphProps) return false;

	if ( "pard" == sCommand )
	{
		paragraphProps->SetDefaultRtf();
	}
	COMMAND_RTF_INT	( "outlinelevel",		paragraphProps->m_nOutlinelevel,		sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "hyphpar",			paragraphProps->m_bAutoHyphenation,		sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "contextualspace",	paragraphProps->m_bContextualSpacing,	sCommand, hasParameter, parameter )
			else if ( "intbl" == sCommand )
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
			if ( PROP_DEF == paragraphProps->m_nItap)
				paragraphProps->m_nItap = 1;
		}
	}
	else if ( "itap" == sCommand && hasParameter)
	{
		//if (parameter == 0 && paragraphProps->m_bInTable && paragraphProps->m_nItap > 0)
		//{
		//
		////	paragraphProps->m_bInTable = 0;
		//}
		//else
		paragraphProps->m_nItap = parameter;
	}
	COMMAND_RTF_BOOL( "keep",	paragraphProps->m_bKeep,	sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "keepn", paragraphProps->m_bKeepNext, sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "pagebb", paragraphProps->m_bPageBB,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT	( "s",		paragraphProps->m_nStyle,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT	( "qc",	paragraphProps->m_eAlign,	sCommand, true, RtfParagraphProperty::pa_qc )
			COMMAND_RTF_INT	( "qj",	paragraphProps->m_eAlign,	sCommand, true, RtfParagraphProperty::pa_qj )
			COMMAND_RTF_INT	( "ql",	paragraphProps->m_eAlign,	sCommand, true, RtfParagraphProperty::pa_ql )
			COMMAND_RTF_INT	( "qr",	paragraphProps->m_eAlign,	sCommand, true, RtfParagraphProperty::pa_qr )
			COMMAND_RTF_INT	( "qd",	paragraphProps->m_eAlign,	sCommand, true, RtfParagraphProperty::pa_qd )

			else if ( "qk0" == sCommand )
	{
		switch( parameter )
		{
		case 0:		paragraphProps->m_eAlign= RtfParagraphProperty::pa_qk0;		break;
		case 10:	paragraphProps->m_eAlign = RtfParagraphProperty::pa_qk10;	break;
		case 20:	paragraphProps->m_eAlign = RtfParagraphProperty::pa_qk20;	break;

		}
	}
	COMMAND_RTF_INT ( "faauto",		paragraphProps->m_eFontAlign,		sCommand, true, RtfParagraphProperty::fa_faauto )
			COMMAND_RTF_INT ( "fahang",		paragraphProps->m_eFontAlign,		sCommand, true, RtfParagraphProperty::fa_fahang )
			COMMAND_RTF_INT ( "facenter",	paragraphProps->m_eFontAlign,		sCommand, true, RtfParagraphProperty::fa_facenter )
			COMMAND_RTF_INT ( "faroman",	paragraphProps->m_eFontAlign,		sCommand, true, RtfParagraphProperty::fa_faroman )
			COMMAND_RTF_INT ( "favar",		paragraphProps->m_eFontAlign,		sCommand, true, RtfParagraphProperty::fa_favar )
			COMMAND_RTF_INT ( "fafixed",	paragraphProps->m_eFontAlign,		sCommand, true, RtfParagraphProperty::fa_fafixed )
			COMMAND_RTF_INT ( "fi",			paragraphProps->m_nIndFirstLine,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "li",			paragraphProps->m_nIndLeft,			sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "ri",			paragraphProps->m_nIndRight,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "lin",		paragraphProps->m_nIndStart,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "rin",		paragraphProps->m_nIndEnd,			sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "adjustright",paragraphProps->m_bIndRightAuto,	sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "indmirror",	paragraphProps->m_bIndMirror,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "sb",			paragraphProps->m_nSpaceBefore,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "sa",			paragraphProps->m_nSpaceAfter,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "sbauto",		paragraphProps->m_nSpaceBeforeAuto,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "saauto",		paragraphProps->m_nSpaceAfterAuto,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "lisb",		paragraphProps->m_nSpaceBeforeLine,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "lisa",		paragraphProps->m_nSpaceAfterLine,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "slmult",		paragraphProps->m_nSpaceMultiLine,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "ilvl",		paragraphProps->m_nListLevel,		sCommand, hasParameter, parameter )

			COMMAND_RTF_BOOL( "absnoovrlp",	paragraphProps->m_bOverlap,			sCommand, hasParameter, parameter )

			//changes
			COMMAND_RTF_INT ( "prdate",		paragraphProps->m_nPrDate,			sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "prauth",		paragraphProps->m_nPrAuth,			sCommand, hasParameter, parameter )

			else if ( "sl" == sCommand )
	{
		if ( hasParameter )
		{
			paragraphProps->m_nSpaceBetween = parameter;
			if ( PROP_DEF == paragraphProps->m_nSpaceMultiLine )
				paragraphProps->m_nSpaceMultiLine = 0;
		}
	}
	else if ( "rtlpar" == sCommand )		paragraphProps->m_bRtl = 1;
	else if ( "ltrpar" == sCommand )		paragraphProps->m_bRtl = 0;
	COMMAND_RTF_BOOL( "nowwrap", paragraphProps->m_bNoWordWrap, sCommand, hasParameter, parameter )
			else if ( "nowwrap" == sCommand )
	{
		if ( hasParameter && 0 == parameter)
			paragraphProps->m_bSnapToGrid = 1;
		else
			paragraphProps->m_bSnapToGrid = 0;
	}
	else if ( "ls" == sCommand )
	{
		if ( hasParameter )
		{
			paragraphProps->m_nListId = parameter;
			if ( PROP_DEF == paragraphProps->m_nListLevel )
				paragraphProps->m_nListLevel = 0;
		}
	}
	//Frame
	COMMAND_RTF_INT ( "absw",			paragraphProps->m_oFrame.m_nWidth,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "absh",			paragraphProps->m_oFrame.m_nHeight,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "phmrg",			paragraphProps->m_oFrame.m_eHRef,		sCommand, true, RtfFrame::hr_phmrg )
			COMMAND_RTF_INT ( "phpg",			paragraphProps->m_oFrame.m_eHRef,		sCommand, true, RtfFrame::hr_phpg )
			COMMAND_RTF_INT ( "phcol",			paragraphProps->m_oFrame.m_eHRef,		sCommand, true, RtfFrame::hr_phcol )
			COMMAND_RTF_INT ( "posx",			paragraphProps->m_oFrame.m_nHPos,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "posnegx",		paragraphProps->m_oFrame.m_nHPos,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "posxc",			paragraphProps->m_oFrame.m_eHPos,		sCommand, true, RtfFrame::hp_posxc )
			COMMAND_RTF_INT ( "posxi",			paragraphProps->m_oFrame.m_eHPos,		sCommand, true, RtfFrame::hp_posxi )
			COMMAND_RTF_INT ( "posxo",			paragraphProps->m_oFrame.m_eHPos,		sCommand, true, RtfFrame::hp_posxo )
			COMMAND_RTF_INT ( "posxl",			paragraphProps->m_oFrame.m_eHPos,		sCommand, true, RtfFrame::hp_posxl )
			COMMAND_RTF_INT ( "posxr",			paragraphProps->m_oFrame.m_eHPos,		sCommand, true, RtfFrame::hp_posxr )
			COMMAND_RTF_INT ( "pvmrg",			paragraphProps->m_oFrame.m_eVRef,		sCommand, true, RtfFrame::vr_pvmrg )
			COMMAND_RTF_INT ( "pvpg",			paragraphProps->m_oFrame.m_eVRef,		sCommand, true, RtfFrame::vr_pvpg )
			COMMAND_RTF_INT ( "pvpara",			paragraphProps->m_oFrame.m_eVRef,		sCommand, true, RtfFrame::vr_pvpara )
			COMMAND_RTF_INT ( "posy",			paragraphProps->m_oFrame.m_nVPos,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "posnegy",		paragraphProps->m_oFrame.m_nVPos,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "posyt",			paragraphProps->m_oFrame.m_eVPos,		sCommand, true, RtfFrame::vp_posyt )
			COMMAND_RTF_INT ( "posyil",			paragraphProps->m_oFrame.m_eVPos,		sCommand, true, RtfFrame::vp_posyil )
			COMMAND_RTF_INT ( "posyb",			paragraphProps->m_oFrame.m_eVPos,		sCommand, true, RtfFrame::vp_posyb )
			COMMAND_RTF_INT ( "posyc",			paragraphProps->m_oFrame.m_eVPos,		sCommand, true, RtfFrame::vp_posyc )
			COMMAND_RTF_INT ( "posyin",			paragraphProps->m_oFrame.m_eVPos,		sCommand, true, RtfFrame::vp_posyin )
			COMMAND_RTF_INT ( "posyout",		paragraphProps->m_oFrame.m_eVPos,		sCommand, true, RtfFrame::vp_posyout )
			COMMAND_RTF_BOOL( "abslock",		paragraphProps->m_oFrame.m_bLockAnchor, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "wrapdefault",	paragraphProps->m_oFrame.m_eWrap,		sCommand, true, RtfFrame::tw_wrapdefault )
			COMMAND_RTF_INT ( "wraparound",		paragraphProps->m_oFrame.m_eWrap,		sCommand, true, RtfFrame::tw_wraparound )
			COMMAND_RTF_INT ( "wraptight",		paragraphProps->m_oFrame.m_eWrap,		sCommand, true, RtfFrame::tw_wraptight )
			COMMAND_RTF_INT ( "wrapthrough",	paragraphProps->m_oFrame.m_eWrap,		sCommand, true, RtfFrame::tw_wrapthrough )
			COMMAND_RTF_INT ( "dropcapt",		paragraphProps->m_oFrame.m_DropcapType, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "dropcapli",		paragraphProps->m_oFrame.m_DropcapLines, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "dxfrtext",		paragraphProps->m_oFrame.m_nAllSpace,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "dfrmtxtx",		paragraphProps->m_oFrame.m_nHorSpace,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "dfrmtxty",		paragraphProps->m_oFrame.m_nVerSpace,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "frmtxlrtb",		paragraphProps->m_eTextFollow,			sCommand, true, RtfParagraphProperty::tf_frmtxlrtb )
			COMMAND_RTF_INT ( "frmtxtbrl",		paragraphProps->m_eTextFollow,			sCommand, true, RtfParagraphProperty::tf_frmtxtbrl )
			COMMAND_RTF_INT ( "frmtxbtlr",		paragraphProps->m_eTextFollow,			sCommand, true, RtfParagraphProperty::tf_frmtxbtlr )
			COMMAND_RTF_INT ( "frmtxlrtbv",		paragraphProps->m_eTextFollow,			sCommand, true, RtfParagraphProperty::tf_frmtxlrtbv )
			COMMAND_RTF_INT ( "frmtxtbrlv",		paragraphProps->m_eTextFollow,			sCommand, true, RtfParagraphProperty::tf_frmtxtbrlv )

			else
	{
		if (RtfShadingCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, oReader.m_oState->m_oParagraphProp.m_oShading ))
			return true;

		return false;
	}
	return true;
}

bool RtfTableCellPropsCommand::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,  std::string sCommand, bool hasParameter, int parameter, RtfCellProperty * cellProps)
{
	if (!cellProps) return false;

	COMMAND_RTF_BOOL( "clmgf",		cellProps->m_bMergeFirst,			sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "clmrg",		cellProps->m_bMerge,				sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "clvmgf",		cellProps->m_bMergeFirstVertical,	sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "clvmrg",		cellProps->m_bMergeVertical,		sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "clFitText",	cellProps->m_bFitText,				sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "clNoWrap",	cellProps->m_bNoWrap,				sCommand, hasParameter, parameter )

			//https://www.office-forums.com/threads/rtf-file-weirdness-clpadt-vs-clpadl.2163500/
			COMMAND_RTF_INT ( "clpadft",	cellProps->m_ePaddingLeftUnit,		sCommand, hasParameter, parameter )	//перепутаны top & left
			COMMAND_RTF_INT ( "clpadt",		cellProps->m_nPaddingLeft,			sCommand, hasParameter, parameter )	//перепутаны top & left
			COMMAND_RTF_INT ( "clpadfl",	cellProps->m_ePaddingTopUnit,		sCommand, hasParameter, parameter )	//перепутаны top & left
			COMMAND_RTF_INT ( "clpadl",		cellProps->m_nPaddingTop,			sCommand, hasParameter, parameter )	//перепутаны top & left
			COMMAND_RTF_INT ( "clpadfr",	cellProps->m_ePaddingRightUnit,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "clpadr",		cellProps->m_nPaddingRight,			sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "clpadfb",	cellProps->m_ePaddingBottomUnit,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "clpadb",		cellProps->m_nPaddingBottom,		sCommand, hasParameter, parameter )

			COMMAND_RTF_INT ( "clspfl",		cellProps->m_eSpacingLeftUnit,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "clspl",		cellProps->m_nSpacingLeft,			sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "clspft",		cellProps->m_eSpacingTopUnit,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "clspt",		cellProps->m_nSpacingTop,			sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "clspfr",		cellProps->m_eSpacingRightUnit,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "clspr",		cellProps->m_nSpacingRight,			sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "clspfb",		cellProps->m_eSpacingBottomUnit,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "clspb",		cellProps->m_nSpacingBottom,		sCommand, hasParameter, parameter )

			else if ( "clftsWidth" == sCommand  )
	{
		if ( hasParameter )
		{
			switch( parameter )
			{
			case 0:	cellProps->m_eWidthUnit = mu_none;		break;
			case 1:	cellProps->m_eWidthUnit = mu_Auto;		break;
			case 2:	cellProps->m_eWidthUnit = mu_Percent;	break;
			case 3:	cellProps->m_eWidthUnit = mu_Twips;		break;
			default:
				break;
			}
		}
	}
	COMMAND_RTF_INT ( "clwWidth",	cellProps->m_nWidth,		sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "clhidemark",	cellProps->m_bHideMark,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "clvertalt",	cellProps->m_eAlign,		sCommand, true, RtfCellProperty::ca_Top )
			COMMAND_RTF_INT ( "clvertalc",	cellProps->m_eAlign,		sCommand, true, RtfCellProperty::ca_Center )
			COMMAND_RTF_INT ( "clvertalb",	cellProps->m_eAlign,		sCommand, true, RtfCellProperty::ca_Bottom )
			COMMAND_RTF_INT ( "cltxlrtb",	cellProps->m_oCellFlow,		sCommand, true, RtfCellProperty::cf_lrtb )
			COMMAND_RTF_INT ( "cltxtbrl",	cellProps->m_oCellFlow,		sCommand, true, RtfCellProperty::cf_tbrl )
			COMMAND_RTF_INT ( "cltxbtlr",	cellProps->m_oCellFlow,		sCommand, true, RtfCellProperty::cf_btlr )
			COMMAND_RTF_INT ( "cltxlrtbv",	cellProps->m_oCellFlow,		sCommand, true, RtfCellProperty::cf_lrtbv )
			COMMAND_RTF_INT ( "cltxtbrlv",	cellProps->m_oCellFlow,		sCommand, true, RtfCellProperty::cf_tbrlv )

			//table style
			COMMAND_RTF_INT ( "tscellpaddfl",	cellProps->m_ePaddingLeftUnit,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "tscellpaddl",	cellProps->m_nPaddingLeft,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "tscellpaddft",	cellProps->m_ePaddingTopUnit,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "tscellpaddt",	cellProps->m_nPaddingTop,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "tscellpaddfr",	cellProps->m_ePaddingRightUnit,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "tscellpaddr",	cellProps->m_nPaddingRight,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "tscellpaddfb",	cellProps->m_ePaddingBottomUnit,sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "tscellpaddb",	cellProps->m_nPaddingBottom,	sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "tsnowrap",		cellProps->m_bNoWrap,			sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "tsvertalt",		cellProps->m_eAlign,			sCommand, true, RtfCellProperty::ca_Top )
			COMMAND_RTF_INT ( "tsvertalc",		cellProps->m_eAlign,			sCommand, true, RtfCellProperty::ca_Center )
			COMMAND_RTF_INT ( "tsvertalb",		cellProps->m_eAlign,			sCommand, true, RtfCellProperty::ca_Bottom )
			else
	{
		if (RtfShadingCellCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, cellProps->m_oShading ))
			return true;
		
		return false;
	}
	return true;
}

bool RtfTableRowPropsCommand::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,  std::string sCommand, bool hasParameter, int parameter, RtfRowProperty * rowProps)
{
	if (!rowProps) return false;
	
	if ( "trowd" == sCommand )
	{
		rowProps->SetDefaultRtf();
	}
	else if ( "nesttableprops" == sCommand )
	{
		rowProps->SetDefaultRtf();
	}
	
	COMMAND_RTF_INT	( "irow",			rowProps->m_nIndex,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT	( "irowband",		rowProps->m_nBandIndex,	sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "lastrow",		rowProps->m_bLastRow,	sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "trhdr",			rowProps->m_bIsHeader,	sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "trkeep",			rowProps->m_bKeep,		sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "trkeepfollow",	rowProps->m_bKeep,		sCommand, hasParameter, parameter )

			COMMAND_RTF_INT	( "trql",			rowProps->m_eJust,		sCommand, true, RtfRowProperty::rj_trql )
			COMMAND_RTF_INT	( "trqr",			rowProps->m_eJust,		sCommand, true, RtfRowProperty::rj_trqr )
			COMMAND_RTF_INT	( "trqc",			rowProps->m_eJust,		sCommand, true, RtfRowProperty::rj_trqc )

			COMMAND_RTF_INT	( "trrh",			rowProps->m_nHeight,				sCommand, hasParameter, parameter )

			COMMAND_RTF_INT ( "trftsWidth",		rowProps->m_eWidthUnit,				sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "trwWidth",		rowProps->m_nWidth,					sCommand, hasParameter, parameter )

			COMMAND_RTF_INT ( "trftsWidthB",	rowProps->m_eWidthStartInvCellUnit, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "trwWidthB",		rowProps->m_nWidthStartInvCell,		sCommand, hasParameter, parameter )

			COMMAND_RTF_INT ( "trftsWidthA",	rowProps->m_eWidthEndInvCellUnit,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "trwWidthA",		rowProps->m_nWidthEndInvCell,		sCommand, hasParameter, parameter )

			COMMAND_RTF_BOOL( "taprtl",			rowProps->m_bBidi,				sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "trautofit",		rowProps->m_nAutoFit,			sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "trgaph",			rowProps->m_nGraph,				sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "tblind",			rowProps->nTableIndent,			sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "tblindtype",		rowProps->eTableIndentUnit,		sCommand, hasParameter, parameter )

			COMMAND_RTF_INT ( "tdfrmtxtLeft",	rowProps->m_nWrapLeft,			sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "tdfrmtxtRight",	rowProps->m_nWrapRight,			sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "tdfrmtxtTop",	rowProps->m_nWrapTop,			sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "tdfrmtxtBottom", rowProps->m_nWrapBottom,		sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "tabsnoovrlp",	rowProps->m_bOverlap,			sCommand, hasParameter, parameter )

			COMMAND_RTF_INT ( "tphmrg",			rowProps->m_eHRef, sCommand, true, RtfTableProperty::hr_phmrg )
			COMMAND_RTF_INT ( "tphpg",			rowProps->m_eHRef, sCommand, true, RtfTableProperty::hr_phpg )
			COMMAND_RTF_INT ( "tphcol",			rowProps->m_eHRef, sCommand, true, RtfTableProperty::hr_phcol )
			COMMAND_RTF_INT ( "tposx",			rowProps->m_nHPos, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "tposnegx",		rowProps->m_nHPos, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "tposxc",			rowProps->m_eHPos, sCommand, true, RtfTableProperty::hp_posxc )
			COMMAND_RTF_INT ( "tposxi",			rowProps->m_eHPos, sCommand, true, RtfTableProperty::hp_posxi )
			COMMAND_RTF_INT ( "tposxo",			rowProps->m_eHPos, sCommand, true, RtfTableProperty::hp_posxo )
			COMMAND_RTF_INT ( "tposxl",			rowProps->m_eHPos, sCommand, true, RtfTableProperty::hp_posxl )
			COMMAND_RTF_INT ( "tposxr",			rowProps->m_eHPos, sCommand, true, RtfTableProperty::hp_posxr )

			COMMAND_RTF_INT ( "tpvmrg",			rowProps->m_eVRef, sCommand, true, RtfTableProperty::vr_pvmrg )
			COMMAND_RTF_INT ( "tpvpg",			rowProps->m_eVRef, sCommand, true, RtfTableProperty::vr_pvpg )
			COMMAND_RTF_INT ( "tpvpara",		rowProps->m_eVRef, sCommand, true, RtfTableProperty::vr_pvpara )
			COMMAND_RTF_INT ( "tposy",			rowProps->m_nVPos, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "tposnegy",		rowProps->m_nVPos, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "tposyt",			rowProps->m_eVPos, sCommand, true, RtfTableProperty::vp_posyt )
			COMMAND_RTF_INT ( "tposyil",		rowProps->m_eVPos, sCommand, true, RtfTableProperty::vp_posyil )
			COMMAND_RTF_INT ( "tposyb",			rowProps->m_eVPos, sCommand, true, RtfTableProperty::vp_posyb )
			COMMAND_RTF_INT ( "tposyc",			rowProps->m_eVPos, sCommand, true, RtfTableProperty::vp_posyc )
			COMMAND_RTF_INT ( "tposyin",		rowProps->m_eVPos, sCommand, true, RtfTableProperty::vp_posyin )
			COMMAND_RTF_INT ( "tposyout",		rowProps->m_eVPos, sCommand, true, RtfTableProperty::vp_posyout )

			else if ( "trleft" == sCommand )
	{
		if ( hasParameter )
		{
			rowProps->m_nLeft = parameter;
			if ( PROP_DEF == rowProps->nTableIndent )
			{
				rowProps->nTableIndent = parameter;
				
				if ( PROP_DEF == rowProps->eTableIndentUnit )
					rowProps->eTableIndentUnit = 3;
			}
		}
	}
	COMMAND_RTF_INT ( "trwWidth", rowProps->m_nWidth, sCommand, hasParameter, parameter )
			else if ( "trftsWidth" == sCommand )
	{
		if ( hasParameter )
		{
			switch( parameter )
			{
			case 0: rowProps->m_eWidthUnit = mu_none;		break;
			case 1: rowProps->m_eWidthUnit = mu_Auto;		break;
			case 2: rowProps->m_eWidthUnit = mu_Percent;	break;
			case 3: rowProps->m_eWidthUnit = mu_Twips;		break;
			default:
				break;
			}
		}
	}
	COMMAND_RTF_INT ( "trpaddb",	rowProps->m_nDefCellMarBottom,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "trpaddl",	rowProps->m_nDefCellMarLeft,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "trpaddr",	rowProps->m_nDefCellMarRight,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "trpaddt",	rowProps->m_nDefCellMarTop,			sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "trpaddfb",	rowProps->m_eDefCellMarBottomUnit,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "trpaddfl",	rowProps->m_eDefCellMarLeftUnit,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "trpaddfr",	rowProps->m_eDefCellMarRightUnit,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "trpaddft",	rowProps->m_eDefCellMarTopUnit,		sCommand, hasParameter, parameter )

			COMMAND_RTF_INT ( "trspdb",		rowProps->m_nDefCellSpBottom,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "trspdl",		rowProps->m_nDefCellSpLeft,			sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "trspdr",		rowProps->m_nDefCellSpRight,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "trspdt",		rowProps->m_nDefCellSpTop,			sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "trspdfb",	rowProps->m_eDefCellSpBottomUnit,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "trspdfl",	rowProps->m_eDefCellSpLeftUnit,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "trspdfr",	rowProps->m_eDefCellSpRightUnit,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "trspdft",	rowProps->m_eDefCellSpTopUnit,		sCommand, hasParameter, parameter )

			COMMAND_RTF_INT ( "ts",			rowProps->m_nStyle,					sCommand, hasParameter, parameter )

			COMMAND_RTF_INT ( "tbllkhdrrows",	rowProps->m_bAutoFirstRow,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "tbllklastrow",	rowProps->m_bAutoLastRow,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "tbllkhdrcols",	rowProps->m_bAutoFirstCol,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "tbllklastcol",	rowProps->m_bAutoLastCol,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "tbllknorowband",	rowProps->m_bAutoNoRowBand,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "tbllknocolband",	rowProps->m_bAutoNoColBand,		sCommand, hasParameter, parameter )

			COMMAND_RTF_INT ( "tscbandsh",		rowProps->m_nRowBandSize,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "tscbandsv",		rowProps->m_nColBandSize,	sCommand, hasParameter, parameter )

			COMMAND_RTF_INT ( "trdate",			rowProps->m_nTrDate,		sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "trauth",			rowProps->m_nTrAuth,		sCommand, hasParameter, parameter )

			else if ( "rtlrow" == sCommand ) rowProps->m_nRightToLeft = 1;
	else if ( "ltrrow" == sCommand ) rowProps->m_nRightToLeft = 0;

	else
		return false;
	
	return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------

RtfOldShapeReader::RtfOldShapeReader( RtfShape& oShape ):m_oShape(oShape)
{
}
void RtfOldShapeReader::ExitReader( RtfDocument& oDocument, RtfReader& oReader )
{
	//если задан поворот, то надо повернуть и исходный rect, если угол от 45 до 135 и от 225 до 315
	if( (PROP_DEF != m_oShape.m_nRotation || PROP_DEF != m_oShape.m_nRelRotation) &&
			(( PROP_DEF != m_oShape.m_nLeft && PROP_DEF != m_oShape.m_nTop &&
			   PROP_DEF != m_oShape.m_nBottom && PROP_DEF != m_oShape.m_nRight ) ||
			 ( PROP_DEF != m_oShape.m_nRelLeft && PROP_DEF != m_oShape.m_nRelTop &&
			   PROP_DEF != m_oShape.m_nRelRight && PROP_DEF != m_oShape.m_nRelBottom ) )
			)
	{
		int nAngel = 0; // в градусах
		if( PROP_DEF != m_oShape.m_nRotation )
			nAngel = m_oShape.m_nRotation / 65536;
		else
			nAngel = m_oShape.m_nRelRotation / 65536;
		int nSourceAngel = nAngel; // в градусах
		bool bRel = false;
		int nLeft;
		int nRight;
		int nTop;
		int nBottom;
		if( PROP_DEF != m_oShape.m_nLeft )
		{
			nLeft = m_oShape.m_nLeft;
			nRight = m_oShape.m_nRight;
			nTop = m_oShape.m_nTop;
			nBottom = m_oShape.m_nBottom;
		}
		else
		{
			bRel = true;
			nLeft = m_oShape.m_nRelLeft;
			nRight = m_oShape.m_nRelRight;
			nTop = m_oShape.m_nRelTop;
			nBottom = m_oShape.m_nRelBottom;
		}
		//поворачиваем на 45 градусов
		nAngel -= 45;
		//делаем угол от 0 до 360
		nAngel = nAngel % 360;
		if( nAngel < 0 )
			nAngel += 360;
		int nQuater = nAngel / 90; // определяем четверть
		if( 0 == nQuater || 2 == nQuater )
		{
			//поворачиваем относительно центра на 90 градусов обратно
			int nCenterX = ( nLeft + nRight ) / 2;
			int nCenterY = ( nTop + nBottom ) / 2;
			int nWidth = nRight - nLeft;
			int nHeight = nBottom - nTop;
			if( true == bRel )
			{
				m_oShape.m_nRelLeft = nCenterX - nHeight / 2;
				m_oShape.m_nRelRight = nCenterX + nHeight / 2;
				m_oShape.m_nRelTop = nCenterY - nWidth / 2;
				m_oShape.m_nRelBottom = nCenterY + nWidth / 2;
			}
			else
			{
				m_oShape.m_nLeft = nCenterX - nHeight / 2;
				m_oShape.m_nRight = nCenterX + nHeight / 2;
				m_oShape.m_nTop = nCenterY - nWidth / 2;
				m_oShape.m_nBottom = nCenterY + nWidth / 2;
			}
		}

	}
}
bool RtfOldShapeReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
{
	if ( "do" == sCommand )
		return true;
	else if ( "doinst" == sCommand )
		return true;
	else if ( "dorslt" == sCommand )
		return false;
	else if ( "picprop" == sCommand )
		return true;
	//-------------------------------------------------------- type primitives
	else if ( "dprect"		== sCommand)	m_oShape.m_nShapeType = ODRAW::sptRectangle;
	else if ( "dpline"		== sCommand)	m_oShape.m_nShapeType = ODRAW::sptLine;
	else if ( "dpellipse"	== sCommand)	m_oShape.m_nShapeType = ODRAW::sptEllipse;
	else if ( "dparc"		== sCommand)	m_oShape.m_nShapeType = ODRAW::sptArc;
	else if ( "dppolyline"	== sCommand)	m_oShape.m_nShapeType = ODRAW::sptNotPrimitive;
	else if ( "dppolygon"	== sCommand)	m_oShape.m_nShapeType = ODRAW::sptNotPrimitive;
	else if ( "dpcallout"	== sCommand)	m_oShape.m_nShapeType = ODRAW::sptTextBox;
	else if ( "dptxbx"		== sCommand)	m_oShape.m_nShapeType = ODRAW::sptTextBox;
	else if ( "dproundr"	== sCommand)	m_oShape.m_nShapeType = ODRAW::sptRoundRectangle;
	else if ( "dptxbxtext" == sCommand )
	{
		if ( PROP_DEF == m_oShape.m_nShapeType )
			m_oShape.m_nShapeType = ODRAW::sptTextBox;
		
		RtfParagraphReader oParagraphReader("shptxt", oReader);
		StartSubReader( oParagraphReader, oDocument, oReader );
		m_oShape.m_aTextItems = oParagraphReader.m_oParPropDest.m_oTextItems;
	}
	else if ( "dobxpage"		== sCommand )	m_oShape.m_eXAnchor		= RtfShape::ax_page;
	else if ( "dobxmargin"		== sCommand )	m_oShape.m_eXAnchor		= RtfShape::ax_margin;
	else if ( "dobxcolumn"		== sCommand )	m_oShape.m_eXAnchor		= RtfShape::ax_column;
	else if ( "dobypage"		== sCommand )	m_oShape.m_eYAnchor		= RtfShape::ay_page;
	else if ( "dobymargin"		== sCommand )	m_oShape.m_eYAnchor		= RtfShape::ay_margin;
	else if ( "dobypara"		== sCommand )	m_oShape.m_eYAnchor		= RtfShape::ay_Para;
	else if ( "dolockanchor"	== sCommand )	m_oShape.m_bLockAnchor	= true;
	else if ( "dplinehollow"	== sCommand )	m_oShape.m_bLine		= false;

	else if ( hasParameter)
	{
		if		( "dpx"			== sCommand )	m_oShape.m_nLeft			= parameter;
		else if ( "dpx"			== sCommand )	m_oShape.m_nLeft			= parameter;
		else if ( "dpy"			== sCommand )	m_oShape.m_nTop				= parameter;
		else if ( "dpysize"		== sCommand )	m_oShape.m_nBottom			= parameter + m_oShape.m_nTop;
		else if ( "dpxsize"		== sCommand )	m_oShape.m_nRight			= parameter + m_oShape.m_nLeft;
		else if ( "doz"			== sCommand )	m_oShape.m_nZOrder			= parameter;
		else if ( "dofhdr"		== sCommand )	m_oShape.m_nHeader			= parameter;
		else if ( "dowr"		== sCommand )	m_oShape.m_nWrapType		= parameter;
		else if ( "dowrk"		== sCommand )	m_oShape.m_nWrapSideType	= parameter;
		else if ( "dofblwtxt"	== sCommand	)	m_oShape.m_nZOrderRelative	= parameter;
		else if ( "dplinew"		== sCommand )	m_oShape.m_nLineWidth		= parameter;
		else if ( "dodhgt"		== sCommand )	m_oShape.m_nZOrder			= parameter;
		else if ( "dptxbxmar"	== sCommand )
		{
			m_oShape.m_nTexpLeft = m_oShape.m_nTexpTop = m_oShape.m_nTexpRight = m_oShape.m_nTexpBottom = RtfUtility::Twips2Emu(parameter);
		}
		else if ( "dpfillpat"	== sCommand )
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

RtfShppictReader::RtfShppictReader( RtfShape& oShape ):m_oShape(oShape)
{
}
bool RtfShppictReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
{
	if( "shppict" == sCommand )
		return true;
	else if( "pict" == sCommand )
	{
		m_oShape.m_eAnchorTypeShape		= RtfShape::st_inline;
		m_oShape.m_nShapeType			= ODRAW::sptPictureFrame;
		m_oShape.m_nWrapType			= 3; // none
		m_oShape.m_nPositionHRelative	= 3;//TCHAR
		m_oShape.m_nPositionVRelative	= 3;//line
		m_oShape.m_nPositionH			= 0;//absolute
		m_oShape.m_nPositionV			= 0;//absolute

		m_oShape.m_oPicture = RtfPicturePtr( new RtfPicture() );
		RtfPictureReader oPictureReader( oReader, m_oShape);
		StartSubReader( oPictureReader, oDocument, oReader );
	}
	else
		return false;
	return true;
}

RtfAllPictReader::RtfAllPictReader( RtfShape& oShape ) : m_oShape(oShape)
{
}
bool RtfAllPictReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
{
	if( "shp" == sCommand )
	{
		RtfShapeReader oShapeReader(m_oShape);
		StartSubReader( oShapeReader, oDocument, oReader );
	}
	else if( "shppict" == sCommand )
	{
		RtfShppictReader oShppictReader(m_oShape);
		StartSubReader( oShppictReader, oDocument, oReader );
	}
	else if( "nonshppict" == sCommand )
	{
		Skip( oDocument, oReader );
	}
	else if( "pict" == sCommand )
	{
		m_oShape.m_eAnchorTypeShape		= RtfShape::st_inline;
		m_oShape.m_nShapeType			= ODRAW::sptPictureFrame;
		m_oShape.m_nWrapType			= 3;	// none
		m_oShape.m_nPositionHRelative	= 3;	//TCHAR
		m_oShape.m_nPositionVRelative	= 3;	//line
		m_oShape.m_nPositionH			= 0;	//absolute
		m_oShape.m_nPositionV			= 0;	//absolute
		m_oShape.m_oPicture				= RtfPicturePtr( new RtfPicture() );

		RtfPictureReader oPictureReader( oReader, m_oShape);
		StartSubReader( oPictureReader, oDocument, oReader );
	}
	else
	{
		bool res = RtfCharPropsCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, &m_oShape.m_oCharProperty );
		if (res) return true;

		return false;
	}
	return true;
}

RtfMathReader::RtfMathReader(RtfMathPtr& pMath): m_pMath(pMath)
{
	m_eParAlign = RtfParagraphProperty::pa_none;
	m_oCharProp.SetDefaultRtf();
}
bool RtfMathReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
{
	if( "mmathPict" == sCommand  )
		;
	else if( "shppict" == sCommand )
	{
		m_pMath->m_oPicture = RtfShapePtr( new RtfShape() );

		RtfShppictReader oShppictReader( *m_pMath->m_oPicture );
		StartSubReader( oShppictReader, oDocument, oReader );
	}
	else if( "nonshppict" == sCommand )
		Skip( oDocument, oReader );
	else
	{
		bool isBoolMath = m_pMath ? m_pMath->IsRtfControlPropertyBool(sCommand) : false;
		bool isValMath	= isBoolMath ? false : (m_pMath ? m_pMath->IsRtfControlProperty(sCommand) : false);
		bool isMath		= (isValMath || isBoolMath) ? false : (m_pMath ? m_pMath->IsRtfControlWord(sCommand) : false);

		if( isMath || isValMath || isBoolMath)
		{
			if( true == m_pMath->m_bHeader )
			{
				m_pMath->m_bHeader = false;
				if (m_pMath->IsEmpty())
					m_pMath->SetRtfName( sCommand );

			}
			else
			{
				RtfMathPtr pNewMath ( new RtfMath() );
				pNewMath->SetRtfName( sCommand );

				pNewMath->m_bIsVal	= isValMath;
				pNewMath->m_bIsBool	= isBoolMath;

				RtfMathReader oSubMathReader(pNewMath);
				bool resParseSub = StartSubReader( oSubMathReader, oDocument, oReader );

				if (resParseSub && "mctrlPr" == sCommand)
				{
					RtfCharPropertyPtr oNewCharProp ( new RtfCharProperty() );
					oNewCharProp->Merge(oSubMathReader.m_oCharProp);
					pNewMath->AddItem( oNewCharProp );
				}

				if(pNewMath->IsValid() == true )
				{
					if ((pNewMath->m_bIsVal || pNewMath->m_bIsBool) && hasParameter)
					{
						RtfCharPtr oChar = RtfCharPtr(new RtfChar);
						std::wstring s = ExecuteMathProp(oDocument, sCommand, parameter);
						oChar->setText( s);
						pNewMath->m_oVal.AddItem( oChar );
					}

					m_pMath->AddItem(pNewMath);
				}
			}
		}
		else if( RtfCharPropsCommand::ExecuteCommand(oDocument, oReader, sCommand, hasParameter, parameter, &m_oCharProp ) == true )
		{
		}
		else
			return false;

		return true;
	}

	return true;
}
void RtfMathReader::ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText)
{
	RtfCharPtr pNewChar ( new RtfChar() );
	pNewChar->m_oProperty.Merge(m_oCharProp);

	pNewChar->setText( sText );
	m_pMath->AddItem( pNewChar );
}
std::wstring RtfMathReader::ExecuteMathProp(RtfDocument& oDocument, std::string sCommand, int parameter)
{//rtf math properties (int) to oox math properties (string)

	std::wstring strProp;

	if( "mbrkBin" == sCommand )
	{
		switch( parameter )
		{
		case 0:	strProp = L"before";	break;
		case 1:	strProp = L"after";		break;
		case 2:	strProp = L"repeat";	break;
		}
	}
	else if( "mbrkBinSub" == sCommand )
	{
		switch( parameter )
		{
		case 0:	strProp = L"--";	break;
		case 1:	strProp = L"+-";	break;
		case 2:	strProp = L"-+";	break;
		}
	}
	else if( "mdefJc" == sCommand )
	{
		switch( parameter )
		{
		case 0:	strProp = L"centerGroup";	break;
		case 1:	strProp = L"center";		break;
		case 2:	strProp = L"left";			break;
		case 3:	strProp = L"right";			break;
		}
	}
	else if( "mnaryLim" == sCommand || "mintLim" == sCommand  || "mLim" == sCommand)
	{
		switch( parameter )
		{
		case 0:	strProp = L"subSup";	break;
		case 1:	strProp = L"undOvr";	break;
		}
	}
	else if ( "mmathFont" == sCommand )
	{
		if (oDocument.m_oProperty.m_nDeffMathFont == PROP_DEF)
			oDocument.m_oProperty.m_nDeffMathFont = parameter;

		RtfFont oFont;
		if( true == oDocument.m_oFontTable.GetFont(parameter, oFont) )
			strProp = oFont.m_sName;
	}
	else
	{
		strProp = std::to_wstring( parameter);
	}

	return strProp;
}
void RtfMathReader::ExitReader2( RtfDocument& oDocument, RtfReader& oReader )
{
	if( RtfParagraphProperty::pa_none != m_eParAlign )
		oReader.m_oState->m_oParagraphProp.m_eAlign = m_eParAlign;
}

RtfShapeReader::RtfShapeReader( RtfShape& oShape ):m_oShape(oShape)
{
}
void RtfShapeReader::ExitReader( RtfDocument& oDocument, RtfReader& oReader )
{
	//если задан поворот, то надо повернуть и исходный rect, если угол от 45 до 135 и от 225 до 315
	if( (PROP_DEF != m_oShape.m_nRotation || PROP_DEF != m_oShape.m_nRelRotation) &&
			(( PROP_DEF != m_oShape.m_nLeft && PROP_DEF != m_oShape.m_nTop &&
			   PROP_DEF != m_oShape.m_nBottom && PROP_DEF != m_oShape.m_nRight ) ||
			 ( PROP_DEF != m_oShape.m_nRelLeft && PROP_DEF != m_oShape.m_nRelTop &&
			   PROP_DEF != m_oShape.m_nRelRight && PROP_DEF != m_oShape.m_nRelBottom ) )
			)
	{
		int nAngel = 0; // в градусах
		if( PROP_DEF != m_oShape.m_nRotation )
			nAngel = m_oShape.m_nRotation / 65536;
		else
			nAngel = m_oShape.m_nRelRotation / 65536;
		int nSourceAngel = nAngel; // в градусах
		bool bRel = false;
		int nLeft;
		int nRight;
		int nTop;
		int nBottom;
		if( PROP_DEF != m_oShape.m_nLeft )
		{
			nLeft = m_oShape.m_nLeft;
			nRight = m_oShape.m_nRight;
			nTop = m_oShape.m_nTop;
			nBottom = m_oShape.m_nBottom;
		}
		else
		{
			bRel = true;
			nLeft = m_oShape.m_nRelLeft;
			nRight = m_oShape.m_nRelRight;
			nTop = m_oShape.m_nRelTop;
			nBottom = m_oShape.m_nRelBottom;
		}
		//поворачиваем на 45 градусов
		nAngel -= 45;
		//делаем угол от 0 до 360
		nAngel = nAngel % 360;
		if( nAngel < 0 )
			nAngel += 360;
		int nQuater = nAngel / 90; // определяем четверть
		if( 0 == nQuater || 2 == nQuater )
		{
			//поворачиваем относительно центра на 90 градусов обратно
			int nCenterX = ( nLeft + nRight ) / 2;
			int nCenterY = ( nTop + nBottom ) / 2;
			int nWidth = nRight - nLeft;
			int nHeight = nBottom - nTop;
			if( true == bRel )
			{
				m_oShape.m_nRelLeft = nCenterX - nHeight / 2;
				m_oShape.m_nRelRight = nCenterX + nHeight / 2;
				m_oShape.m_nRelTop = nCenterY - nWidth / 2;
				m_oShape.m_nRelBottom = nCenterY + nWidth / 2;
			}
			else
			{
				m_oShape.m_nLeft = nCenterX - nHeight / 2;
				m_oShape.m_nRight = nCenterX + nHeight / 2;
				m_oShape.m_nTop = nCenterY - nWidth / 2;
				m_oShape.m_nBottom = nCenterY + nWidth / 2;
			}
		}

	}
}
bool RtfShapeReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
{
	if ( "shp" == sCommand )
		return true;
	else if ( "shpinst" == sCommand )
		return true;
	else if ( "shprslt" == sCommand )
		return false;
	else if ( "picprop" == sCommand )
		return true;
	else if ( "shptxt" == sCommand )
	{
		if ( PROP_DEF == m_oShape.m_nShapeType )
			m_oShape.m_nShapeType = ODRAW::sptTextBox;
		
		RtfParagraphReader oParagraphReader("shptxt", oReader);
		StartSubReader( oParagraphReader, oDocument, oReader );
		m_oShape.m_aTextItems = oParagraphReader.m_oParPropDest.m_oTextItems;
	}
	else if ( "sp" == sCommand )
	{
		ShapePropertyReader oShapePropReader(m_oShape);
		StartSubReader(oShapePropReader, oDocument, oReader);
	}
	else if ( "shpleft" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_nLeft = parameter;
	}
	else if ( "shptop" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_nTop = parameter;
	}
	else if ( "shpbottom" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_nBottom = parameter;
	}
	else if ( "shpright" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_nRight = parameter;
	}
	else if ( "shplid" == sCommand )
	{
		if ( hasParameter )
		{
			m_oShape.m_nID = parameter;
			oDocument.SetShapeId( parameter );
		}
	}
	else if ( "shpz" == sCommand )
	{
		if ( hasParameter )
		{
			m_oShape.m_nZOrder = parameter;
			
			oDocument.SetZIndex(abs(m_oShape.m_nZOrder));
		}
	}
	else if ( "shpfhdr" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_nHeader = parameter;
	}
	else if ( "shpbxignore" == sCommand )	m_oShape.m_eXAnchor = RtfShape::ax_none; // PHy2.rtf
	else if ( "shpbxpage" == sCommand )		m_oShape.m_eXAnchor = RtfShape::ax_page;
	else if ( "shpbxmargin" == sCommand )	m_oShape.m_eXAnchor = RtfShape::ax_margin;
	else if ( "shpbxcolumn" == sCommand )	m_oShape.m_eXAnchor = RtfShape::ax_column;
	else if ( "shpbyignore" == sCommand )	m_oShape.m_eYAnchor = RtfShape::ay_none; // PHy2.rtf
	else if ( "shpbypage" == sCommand )		m_oShape.m_eYAnchor = RtfShape::ay_page;
	else if ( "shpbymargin" == sCommand )	m_oShape.m_eYAnchor = RtfShape::ay_margin;
	else if ( "shpbypara" == sCommand )		m_oShape.m_eYAnchor = RtfShape::ay_Para;
	else if ( "shpwr" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_nWrapType = parameter;
	}
	else if ( "shpwrk" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_nWrapSideType = parameter;
	}
	else if ( "shpfblwtxt" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_nZOrderRelative = parameter;
	}
	else if ( "shplockanchor" == sCommand )
		m_oShape.m_bLockAnchor = true;
	else
		return false;
	return true;
}

RtfShapeGroupReader::RtfShapeGroupReader( RtfShape& oShape ) : RtfShapeReader(oShape), m_oShapeGroup(oShape)
{
	m_bHeader = true;
}
bool RtfShapeGroupReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
{
	if( "shpgrp" == sCommand )
	{
		if( true == m_bHeader )
			m_bHeader = false;
		else
		{
			RtfShapePtr	pNewShape ( new RtfShape() );
			pNewShape->m_bInGroup	= true;
			pNewShape->m_bIsGroup	= true;
			pNewShape->m_nShapeType	= 1;

			RtfShapeGroupReader oShapeGroupReader ( *pNewShape );
			StartSubReader( oShapeGroupReader, oDocument, oReader );

			m_oShapeGroup.AddItem( pNewShape );
		}
	}
	else if( "shp" == sCommand )
	{
		RtfShapePtr pNewShape ( new RtfShape() );
		pNewShape->m_bInGroup = true;

		RtfShapeReader	oShapeReader( *pNewShape );
		StartSubReader( oShapeReader, oDocument, oReader );

		m_oShapeGroup.AddItem( pNewShape );
	}
	else
	{
		return RtfShapeReader::ExecuteCommand( oDocument,  oReader, sCommand, hasParameter, parameter);
	}
	return true;
}

RtfBackgroundReader::RtfBackgroundReader( RtfShape& oShape ) : RtfShapeReader(oShape), m_oShape(oShape)
{
}
bool RtfBackgroundReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
{
	if( "background" == sCommand )
	{
		return true;
	}
	else if( "shp" == sCommand )
	{
		RtfShapeReader	oShapeReader( m_oShape );

		StartSubReader( oShapeReader, oDocument, oReader );
	}
	else
		return RtfShapeReader::ExecuteCommand( oDocument,  oReader, sCommand, hasParameter, parameter);
	return true;
}

RtfFieldReader::RtfFieldReader( RtfField& oField ):m_oField(oField)
{
	m_eInternalState = is_normal;
}
void RtfFieldReader::ExitReader( RtfDocument& oDocument, RtfReader& oReader )
{
	//если результат пустой пытаемся его сгенерировать
	if( true == IsEmptyText( oDocument ) )
		TryToPepairResult( oDocument, oReader );
}
bool RtfFieldReader::IsEmptyText( RtfDocument& oDocument )
{
	if( NULL != m_oField.m_pResult )
	{
		OOXWriter oTempWriter( oDocument, L"" );
		OOXRelsWriter oTempRelsWriter( L"", oDocument );

		RenderParameter oRenderParameter;
		oRenderParameter.poDocument	= &oDocument;
		oRenderParameter.poRels		= &oTempRelsWriter;
		oRenderParameter.poWriter	= &oTempWriter;
		oRenderParameter.nType		= RENDER_TO_OOX_PARAM_PLAIN;

		std::wstring sResult = m_oField.m_pResult->RenderToOOX( oRenderParameter );
		if( L"" != sResult )
			return false;
	}
	return true;
}
void RtfFieldReader::TryToPepairResult( RtfDocument& oDocument, RtfReader& oReader )
{
	OOXWriter oTempWriter( oDocument, L"" );
	OOXRelsWriter oTempRelsWriter( L"", oDocument );

	RenderParameter oNewParametr;
	oNewParametr.poDocument = &oDocument;
	oNewParametr.poRels		= &oTempRelsWriter;
	oNewParametr.poWriter	= &oTempWriter;
	oNewParametr.nType		= RENDER_TO_OOX_PARAM_PLAIN;

	std::wstring sCharCode;
	std::wstring sCharFont;

	std::wstring sField = m_oField.m_pInsert->m_pTextItems->RenderToOOX(oNewParametr);
	size_t nStartTokenize = 0;

	int nCommand = 0; //0 - none; 1 - \f; 3 - other

	std::vector<std::wstring> arResult;
	boost::algorithm::split(arResult, sField, boost::algorithm::is_any_of(L" \""), boost::algorithm::token_compress_on);

	while (!arResult.empty())
	{
		if (arResult[0].empty())
			arResult.erase( arResult.begin(), arResult.begin() + 1 );
		else
			break;
	}

	if (arResult.empty())
		return;

	if (L"SYMBOL" != arResult[0] && L"HYPERLINK" != arResult[0])
		return;

	nStartTokenize = arResult[0].length();

	for (size_t i = 1 ; i < arResult.size(); i++)
	{
		std::wstring sResTokenize = arResult[i];

		size_t nTokenLen = sResTokenize.length();
		if( nTokenLen > 0 && sResTokenize[0] == '\"' && sResTokenize[nTokenLen - 1] != '\"' ) //текст в кавычках считается как один
		{
			//ищем следующую кавычку
			size_t nNextQuot = sField.find( '\"', nStartTokenize );
			if( std::wstring::npos != nNextQuot )
			{
				sResTokenize = sField.substr( nStartTokenize - nTokenLen, nNextQuot - nStartTokenize + nTokenLen );
				nStartTokenize = nNextQuot + 1;

				for (; i < arResult.size(); i++)
				{
					if ( std::wstring::npos != arResult[i].find( '\"'))
						break;
				}
			}
		}
		if( nTokenLen > 0 && sResTokenize[0] == '\"' && sResTokenize[nTokenLen - 1] == '\"' )
			sResTokenize = sResTokenize.substr( 1, nTokenLen - 2 );

		if( std::wstring::npos != sResTokenize.find( L"\\f" ) )
			nCommand = 1;
		else if( std::wstring::npos != sResTokenize.find( L"\\" ) )
			nCommand = 3;
		else if( 1 == nCommand )
		{
			sCharFont = sResTokenize;
			nCommand = 0;
		}
		else if( 0 == nCommand && L"" == sCharCode )
		{
			sCharCode = sResTokenize;
		}
		else
			nCommand = 0;

		nStartTokenize += sResTokenize.length();
	}

	std::wstring sResult;
	if ( L"SYMBOL" == arResult[0])
	{
		if ( L"" == sCharCode || L"" == sCharFont ) return;
		int nCharCode = Strings::ToInteger( sCharCode );
		std::string sCharA; sCharA += char(nCharCode );

		RtfFont oSymbolFont;
		if( true == oDocument.m_oFontTable.GetFont( sCharFont, oSymbolFont ) )
			oReader.m_oState->m_oCharProp.m_nFont = oSymbolFont.m_nID;
		else
		{
			oSymbolFont.m_sName = sCharFont;
			oSymbolFont.m_nID = oDocument.m_oFontTable.GetCount() + 1;
			oSymbolFont.m_nCodePage = CP_SYMBOL;
			oReader.m_oState->m_oCharProp.m_nFont = oSymbolFont.m_nID;
			oDocument.m_oFontTable.DirectAddItem( oSymbolFont );
		}

		int nSkipChar = 0;

		RtfAbstractReader reader;
		sResult	= reader.ExecuteTextInternal( oDocument, oReader, sCharA, false, 0, nSkipChar );
	}
	else if ( L"HYPERLINK" == arResult[0])
	{
		sResult = sCharCode;
	}

	if (sResult.empty()) return;

	RtfFieldInstPtr newResult = RtfFieldInstPtr ( new RtfFieldInst() );
	newResult->SetDefault();

	RtfParagraphPtr pNewPar	( new RtfParagraph() );
	RtfCharPtr		pNewChar( new RtfChar() );

	pNewChar->setText( sResult);
	pNewChar->m_oProperty = m_oField.m_pResult->m_oCharProperty;

	pNewPar->AddItem( pNewChar );
	newResult->m_pTextItems->AddItem( pNewPar );
	newResult->m_oCharProperty = m_oField.m_pResult->m_oCharProperty;

	m_oField.m_pResult = newResult;

	if ( L"SYMBOL" == arResult[0])
		m_oField.m_bTextOnly = true;
}
bool RtfFieldReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
{
	if ( "field" == sCommand )
		return true;
	else if ( "flddirty" == sCommand )
		m_oField.m_eMode = RtfField::fm_flddirty;
	else if ( "fldedit" == sCommand )
		m_oField.m_eMode = RtfField::fm_fldedit;
	else if ( "fldlock" == sCommand )
		m_oField.m_eMode = RtfField::fm_fldlock;
	else if ( "fldpriv" == sCommand )
		m_oField.m_eMode = RtfField::fm_fldpriv;
	else if ( "fldalt" == sCommand )
		m_oField.m_bReferenceToEndnote = true;

	else if ( "fldinst" == sCommand )
	{
		RtfFieldInstPtr oNewFieldInst = RtfFieldInstPtr(new RtfFieldInst());
		oNewFieldInst->m_oCharProperty = oReader.m_oState->m_oCharProp;
		
		RtfFieldInstReader oFieldInstReader( *oNewFieldInst );
		StartSubReader( oFieldInstReader, oDocument, oReader );
		
		if ( oNewFieldInst->IsValid() )
		{
			m_oField.m_pInsert = oNewFieldInst;
		}
	}
	else if ( "fldrslt" == sCommand )
	{
		RtfFieldInstPtr oNewFieldInst = RtfFieldInstPtr(new RtfFieldInst());
		oNewFieldInst->m_oCharProperty = oReader.m_oState->m_oCharProp;
		
		RtfFieldInstReader oFieldInstReader( *oNewFieldInst );
		StartSubReader( oFieldInstReader, oDocument, oReader );
		
		if ( oNewFieldInst->IsValid() )
		{
			m_oField.m_pResult = oNewFieldInst;
		}
		
		//oReader.m_oLex.putString( "}{" );//чтобы не терять после fldrslt
		//{\field{\*\fldinst...}{\*\fldrslt...} ??? }
		//{\field{\*\fldinst...}{\*\fldrslt...}}{ ??? }
	}
	else
	{
		return false;
	}
	return true;
}

RtfAnnotElemReader::RtfAnnotElemReader( RtfAnnotElem& oAnnot ) : m_oAnnot(oAnnot)
{
}
bool RtfAnnotElemReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
{
	if( "atrfstart" == sCommand )
		;
	else if( "atrfend" == sCommand )
		;
	else if( "atnref" == sCommand )
		;
	else if( "atndate" == sCommand )
		;
	else if( "atnid" == sCommand )
		;
	else if( "atnauthor" == sCommand )
		;
	else if ( "atnparent"  == sCommand )
		;
	else
		return false;
	return true;
}
void RtfAnnotElemReader::ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText)
{
	m_oAnnot.m_sValue += sText ;
}

RtfBookmarkStartReader::RtfBookmarkStartReader( RtfBookmarkStart& oBookmark ):m_oBookmarkStart(oBookmark)
{
}
bool RtfBookmarkStartReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
{
	if( "bkmkstart" == sCommand )
		;
	else if( "bkmkcolf" == sCommand )
	{
		if( true == hasParameter)
			m_oBookmarkStart.nFirstColumn = parameter;
	}
	else if( "bkmkcoll" == sCommand )
	{
		if( true == hasParameter)
			m_oBookmarkStart.nLastColumn = parameter;
	}
	else
		return false;
	return true;
}
void RtfBookmarkStartReader::ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText)
{
	m_oBookmarkStart.m_sName += sText ;
}

RtfBookmarkEndReader::RtfBookmarkEndReader( RtfBookmarkEnd& oBookmark ):m_oBookmarkEnd(oBookmark)
{
}
void RtfBookmarkEndReader::ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText)
{
	m_oBookmarkEnd.m_sName += sText;
}

RtfFieldInstReader::RtfFieldInstReader( RtfFieldInst& oFieldInst ) :  m_oFieldInst(oFieldInst) {}
void RtfFieldInstReader::ExecuteText( RtfDocument& oDocument, RtfReader& oReader, std::wstring sText )
{
	RtfParagraphPropDestination::ExecuteText( oDocument, oReader, sText );
}
void RtfFieldInstReader::ExitReader( RtfDocument& oDocument, RtfReader& oReader )
{
	RtfParagraphPropDestination::Finalize( oReader );

	m_oFieldInst.m_pTextItems	= m_oTextItems;
}
bool RtfFieldInstReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader , std::string sCommand, bool hasParameter, int parameter)
{
	if( "fldinst" == sCommand )
		return true;
	if( "fldrslt" == sCommand )
		return true;
	else if ("formfield" == sCommand)
	{
		RtfFormFieldPtr oNewFormField = RtfFormFieldPtr(new RtfFormField());

		RtfFormFieldReader oFormFieldReader(*oNewFormField.get());
		StartSubReader(oFormFieldReader, oDocument, oReader);

		if (oNewFormField->IsValid())
		{
			m_oFieldInst.m_pFormField = oNewFormField;
		}
	}
	else if ("ffdeftext" == sCommand)
		Skip(oDocument, oReader);
	else
	{
		RtfCharPropsCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, &m_oFieldInst.m_oCharProperty );
		
		return RtfParagraphPropDestination::ExecuteCommand( oDocument, oReader, (*this), sCommand, hasParameter, parameter );
	}
}

RtfFormFieldReader::RtfFormFieldReader(RtfFormField& oFormField) : m_oFormField(oFormField)
{
}
void RtfFormFieldReader::ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText)
{
	if (is_name == m_eInternalState)			m_oFormField.name += sText;
	else if (is_deftext == m_eInternalState)	m_oFormField.deftext += sText;
	else if (is_datafield == m_eInternalState)	m_oFormField.datafield += sText;
	else if (is_format == m_eInternalState)		m_oFormField.format += sText;
	else if (is_helptext == m_eInternalState)	m_oFormField.helptext += sText;
	else if (is_stattext == m_eInternalState)	m_oFormField.stattext += sText;
	else if (is_entrymcr == m_eInternalState)	m_oFormField.entrymcr += sText;
	else if (is_exitmcr == m_eInternalState)	m_oFormField.exitmcr += sText;
	else if (is_list == m_eInternalState)		m_oFormField.list.push_back(sText);
}
bool RtfFormFieldReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
{
	if ("formfield" == sCommand)
		return true;

	m_eInternalState = is_none;
	if ("datafield" == sCommand)		m_eInternalState = is_datafield;
	else if ("ffname" == sCommand)		m_eInternalState = is_name;
	else if ("ffdeftext" == sCommand)	m_eInternalState = is_deftext;
	else if ("ffformat" == sCommand)	m_eInternalState = is_format;
	else if ("ffhelptext" == sCommand)	m_eInternalState = is_helptext;
	else if ("ffstattext" == sCommand)	m_eInternalState = is_stattext;
	else if ("ffentrymcr" == sCommand)	m_eInternalState = is_entrymcr;
	else if ("ffexitmcr" == sCommand)	m_eInternalState = is_exitmcr;
	else if ("ffl" == sCommand)			m_eInternalState = is_list;
	COMMAND_RTF_INT("fftype", m_oFormField.type, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT("ffownhelp", m_oFormField.ownhelp, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT("ffownstat", m_oFormField.ownstat, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT("ffprot", m_oFormField.prot, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT("ffsize", m_oFormField.sizeCheckBox, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT("fftypetx", m_oFormField.typetx, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT("ffrecalc", m_oFormField.recalc, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT("ffhaslistbox", m_oFormField.haslistbox, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT("ffmaxlen", m_oFormField.maxlen, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT("ffhps", m_oFormField.hps, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT("ffdefres", m_oFormField.defres, sCommand, hasParameter, parameter)
			COMMAND_RTF_INT("ffres", m_oFormField.res, sCommand, hasParameter, parameter)

			return true;
}

RtfOleBinReader::RtfOleBinReader()
{
}
bool RtfOleBinReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
{
	if ( "objdata" == sCommand )
		return true;

	if ( "bin" == sCommand ) // from RtfOleReader - conv_NI38P7GBIpw1aD84H3k.rtf
	{
		int nDataSize = 0;
		if ( hasParameter )
			nDataSize  = parameter;

		BYTE *pData = NULL;

		oReader.m_oLex.ReadBytes( parameter, &pData );

		m_arData.push_back(std::string((char*)pData, nDataSize));

		RELEASEOBJECT(pData);
	}
	return true;
}
void RtfOleBinReader::ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText)
{
	m_arData.push_back(std::string(sText.begin(), sText.end()));
}
void RtfOleBinReader::GetData( BYTE** ppData, long& nSize)
{
	nSize = 0;
	size_t pos = 0, start = 0, nSizeRead = 0;

	if (m_arData.size() > 1)
	{
		nSizeRead = *((short*)m_arData[0].c_str());
		start = 1; // first content all size
	}
	for (size_t i = start; i < m_arData.size(); i++)
	{
		nSize += (long)m_arData[i].length();
	}
	
	(*ppData) = new BYTE[ nSize];
	
	for (size_t i = start; i < m_arData.size(); i++)
	{
		BYTE *buf = (BYTE*)m_arData[i].c_str();

		for (size_t j = 0; j < m_arData[i].length(); j += 2)
		{
			BYTE nByte = 0;
			
			nByte =		RtfUtility::ToByte(buf[ j ]) << 4;
			nByte |=	RtfUtility::ToByte(buf[ j + 1]);
			
			(*ppData)[pos++] = nByte;
		}
	}
	nSize = (long)pos;
}

RtfOleReader::RtfOleReader(RtfOle& oOle) : m_oOle(oOle)
{
}
bool RtfOleReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
{
	if ( "object" == sCommand )
		return true;
	
	COMMAND_RTF_INT ( "objw",		m_oOle.m_nWidth,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "objh",		m_oOle.m_nHeight,	sCommand, hasParameter, parameter )
			COMMAND_RTF_INT ( "objemb",		m_oOle.m_eOleType,	sCommand, true, RtfOle::ot_emb )
			COMMAND_RTF_INT ( "objlink",	m_oOle.m_eOleType,	sCommand, true, RtfOle::ot_link )

			else if ( "objclass" == sCommand )
	{
		TextReader oTextReader( m_oOle.m_sOleClass, false );
		StartSubReader( oTextReader, oDocument, oReader );

		size_t pos = m_oOle.m_sOleClass.find(L"asc.");
		if (std::wstring::npos != pos)
		{
			m_oOle.m_sOleClass = L"asc.{" + m_oOle.m_sOleClass.substr(pos + 4) + L"}";
		}
	}
	else if ( "objdata" == sCommand )
	{
		RtfOleBinReader oBinReader;

		StartSubReader( oBinReader, oDocument, oReader );

		BYTE *pData = NULL;
		long nSize = 0;
		
		oBinReader.GetData(&pData, nSize );

		if ( 0 != nSize  && pData)
		{
			boost::shared_array<BYTE> buffer(pData);
			m_oOle.m_oOle1Data = std::make_pair(buffer, nSize);

			std::wstring sOleStorageName = NSDirectory::CreateTempFileWithUniqueName(oReader.m_sTempFolder, L"img");

			//конвертация Ole1 в Ole2
			ConvertOle1ToOle2(pData, nSize, sOleStorageName);

			m_oOle.SetFilename( sOleStorageName.c_str() );
		}
	}
	else if ( "result" == sCommand )
	{
		RtfShapePtr oNewShape ( new RtfShape() );
		
		RtfAllPictReader oAllPictReader( *oNewShape );
		StartSubReader( oAllPictReader, oDocument, oReader );
		
		m_oOle.m_oResultShape = oNewShape;
	}
	else
		return false;
	return true;
}

RtfShapeReader::ShapePropertyReader::ShapePropertyValueReader::ShapePropertyValueReader(std::wstring& sPropName, RtfShape& oShape) : m_sPropName(sPropName), m_oShape(oShape)
{
}
bool RtfShapeReader::ShapePropertyReader::ShapePropertyValueReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,  std::string sCommand, bool hasParameter, int parameter)
{
	if( "sv" == sCommand )
		return true;
	else if( "pict" == sCommand && ( L"pib" == m_sPropName  || L"fillBlip" == m_sPropName))
	{
		m_oShape.m_oPicture = RtfPicturePtr ( new RtfPicture() );
		RtfPictureReader oPictureReader( oReader, m_oShape );
		StartSubReader( oPictureReader, oDocument, oReader );
	}
	else
		return false;
	return true;
}
void RtfShapeReader::ShapePropertyReader::ShapePropertyValueReader::ExecuteText( RtfDocument& oDocument, RtfReader& oReader, std::wstring oText)
{
	m_sPropValue += oText;
}
void RtfShapeReader::ShapePropertyReader::ShapePropertyValueReader::PopState( RtfDocument& oDocument, RtfReader& oReader )
{
	RtfAbstractReader::PopState( oDocument, oReader );
	if ( L"" == m_sPropValue )
		return;
	std::wstring sValue = m_sPropValue;

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
	else if ( L"wzSigSetupId" == m_sPropName )
	{
		m_oShape.m_sSigSetupId = sValue;
		return;
	}
	else if ( L"wzSigSetupProvId" == m_sPropName )
	{
		m_oShape.m_sSigSetupProvId = sValue;
		return;
	}
	else if ( L"wzSigSetupSuggSigner" == m_sPropName )
	{
		m_oShape.m_sSigSetupSuggSigner = sValue;
		return;
	}
	else if ( L"wzSigSetupSuggSigner2" == m_sPropName )
	{
		m_oShape.m_sSigSetupSuggSigner2 = sValue;
		return;
	}
	else if ( L"wzSigSetupSuggSignerEmail" == m_sPropName )
	{
		m_oShape.m_sSigSetupSuggSignerEmail = sValue;
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
	else if ( L"fLockPosition"		== m_sPropName ) m_oShape.m_nLockPosition		= nValue;
	else if ( L"fLockRotation"		== m_sPropName ) m_oShape.m_nLockRotation		= nValue;

	//Position relative
	else if ( L"pctHorizPos"		== m_sPropName ) m_oShape.m_nPositionHPct		= nValue;
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
	else if ( L"rotation"			== m_sPropName ) m_oShape.m_nRotation			= nValue;
	else if ( L"fFlipV"				== m_sPropName ) m_oShape.m_bFlipV				= nValue;
	else if ( L"fFlipH"				== m_sPropName ) m_oShape.m_bFlipH				= nValue;
	else if ( L"shapeType"			== m_sPropName ) m_oShape.m_nShapeType			= nValue;
	//custom
	else if ( L"shapePath"			== m_sPropName ) m_oShape.m_nShapePath			= nValue;
	else if ( L"pWrapPolygonVertices" == m_sPropName )
	{
		std::vector< std::wstring > splitted;

		boost::algorithm::split(splitted, sValue, boost::algorithm::is_any_of(L";"), boost::algorithm::token_compress_on);
		
		for (size_t i = 2 ; i < splitted.size(); i++)
		{
			XmlUtils::replace_all(splitted[i], L")", L"");
			XmlUtils::replace_all(splitted[i], L"(", L"");
			
			int pos = (int)splitted[i].find(L",");

			int x = 0, y = 0;
			try
			{
				x = XmlUtils::GetInteger(splitted[i].substr(0, pos));
			}
			catch(...){}
			try
			{
				y = XmlUtils::GetInteger(splitted[i].substr(pos + 1, splitted[i].length() - 1));
			}
			catch(...){}

			m_oShape.m_aWrapPoints.push_back( std::pair<int, int>(x, y) );
		}
	}
	else if ( L"pVerticies"		== m_sPropName )
	{
		std::vector< std::wstring > splitted;

		boost::algorithm::split(splitted, sValue, boost::algorithm::is_any_of(L";"), boost::algorithm::token_compress_on);
		
		for (size_t i = 2 ; i < splitted.size(); i++)
		{
			XmlUtils::replace_all(splitted[i], L")", L"");
			XmlUtils::replace_all(splitted[i], L"(", L"");
			
			int pos = (int)splitted[i].find(L",");

			int x = 0, y = 0;
			try
			{
				x = XmlUtils::GetInteger(splitted[i].substr(0, pos));
			}
			catch(...){}
			try
			{
				y = XmlUtils::GetInteger(splitted[i].substr(pos + 1, splitted[i].length() - 1));
			}
			catch(...){}

			m_oShape.m_aPVerticles.push_back( std::make_pair(x, y) );
		}
	}
	else if ( L"pSegmentInfo" == m_sPropName )
	{
		std::vector< std::wstring > splitted;

		boost::algorithm::split(splitted, sValue, boost::algorithm::is_any_of(L";"), boost::algorithm::token_compress_on);
		
		for (size_t i = 2 ; i < splitted.size(); i++)
		{
			int val = 0;
			try
			{
				val = XmlUtils::GetInteger(splitted[i]);
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
	else if ( L"WrapText"		== m_sPropName ) m_oShape.m_nWrapText		= nValue;
	else if ( L"txflTextFlow"	== m_sPropName ) m_oShape.m_nTxflTextFlow	= nValue;
	else if ( L"ccol"			== m_sPropName ) m_oShape.m_nCcol			= nValue;
	else if ( L"txdir"			== m_sPropName ) m_oShape.m_nTxdir			= nValue;
	else if ( L"fFitShapeToText"== m_sPropName ) m_oShape.m_bFitShapeToText	= nValue;
	else if ( L"fFitTextToShape"== m_sPropName ) m_oShape.m_bFitTextToShape	= nValue;
	else if ( L"fRotateText"	== m_sPropName ) m_oShape.m_fRotateText		= nValue;

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
	else if ( L"fillShadeType"	== m_sPropName ) m_oShape.m_nFillShadeType	= nValue;
	else if ( L"fillShadeColors"== m_sPropName )
	{
		std::vector< std::wstring > splitted;

		boost::algorithm::split(splitted, sValue, boost::algorithm::is_any_of(L";"), boost::algorithm::token_compress_on);
		
		for (size_t i = 2 ; i < splitted.size(); i++)
		{
			XmlUtils::replace_all(splitted[i], L")", L"");
			XmlUtils::replace_all(splitted[i], L"(", L"");
			
			int pos = (int)splitted[i].find(L",");

			int col = 0, pos_col = 0;
			try
			{
				col = XmlUtils::GetInteger(splitted[i].substr(0, pos));
			}
			catch(...){}
			try
			{
				pos_col = XmlUtils::GetInteger(splitted[i].substr(pos + 1, splitted[i].length() - 1)) * 100 / 65536;
			}
			catch(...){}

			m_oShape.m_aFillShadeColors.push_back( std::make_pair(col, pos_col) );
		}
	}

	else if ( L"fGtext"			== m_sPropName ) m_oShape.m_bGtext			= nValue;
	else if ( L"gtextSize"		== m_sPropName ) m_oShape.m_nGtextSize		= nValue;

	//Line
	else if ( L"fLine"					== m_sPropName ) m_oShape.m_bLine					= ( 0 == nValue ? false : true );
	else if ( L"lineStartArrowhead"		== m_sPropName ) m_oShape.m_nLineStartArrow			= nValue;
	else if ( L"lineColor"				== m_sPropName ) m_oShape.m_nLineColor				= nValue;
	else if ( L"lineStartArrowWidth"	== m_sPropName ) m_oShape.m_nLineStartArrowWidth	= nValue;
	else if ( L"lineStartArrowLength"	== m_sPropName ) m_oShape.m_nLineStartArrowLength	= nValue;
	else if ( L"lineEndArrowhead"		== m_sPropName ) m_oShape.m_nLineEndArrow			= nValue;
	else if ( L"lineEndArrowWidth"		== m_sPropName ) m_oShape.m_nLineEndArrowWidth		= nValue;
	else if ( L"lineEndArrowLength"		== m_sPropName ) m_oShape.m_nLineEndArrowLength		= nValue;
	else if ( L"lineWidth"				== m_sPropName ) m_oShape.m_nLineWidth				= nValue;
	else if ( L"lineDashing"			== m_sPropName ) m_oShape.m_nLineDashing			= nValue;
	else if ( L"cxstyle"				== m_sPropName ) m_oShape.m_nConnectorStyle			= nValue;
	else if ( L"cxk"					== m_sPropName ) m_oShape.m_nConnectionType			= nValue;
	//office signature
	else if ( L"fIsSignatureLine"		== m_sPropName ) m_oShape.m_bIsSignatureLine		= nValue;
	else if ( L"fSigSetupAllowComments"	== m_sPropName ) m_oShape.m_bSigSetupAllowComments	= nValue;
	else
	{
		std::wstring name	= m_sPropName;
		int val				= nValue;
	}
}

RtfShapeReader::ShapePropertyReader::ShapePropertyReader(RtfShape& oShape):m_oShape(oShape)
{
}
bool RtfShapeReader::ShapePropertyReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,  std::string sCommand, bool hasParameter, int parameter)
{
	if( "sp" == sCommand )
		return true;
	else if( "sn" == sCommand )
	{
		TextReader oTextReader(sPropName);
		StartSubReader( oTextReader, oDocument, oReader );
	}
	else if( "sv" == sCommand )
	{
		ShapePropertyValueReader oShPropValReader(sPropName, m_oShape);
		StartSubReader( oShPropValReader, oDocument, oReader );
	}
	else
		return false;
	return true;
}

bool RtfTrackerChangesReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,  std::string sCommand, bool hasParameter, int parameter)
{
	if ( "oldcprops" == sCommand )
		return true;
	if ( "oldpprops" == sCommand )
		return true;
	if ( "oldsprops" == sCommand )
		return true;
	if ( "oldtprops" == sCommand )
		return true;
	
	else if ( "chbrdr"	== sCommand )		m_eInternalState = is_charBorder;
	else if ( "brdrt"	== sCommand )		m_eInternalState = is_borderTop;
	else if ( "brdrb"	== sCommand )		m_eInternalState = is_borderBottom;
	else if ( "brdrl"	== sCommand )		m_eInternalState = is_borderLeft;
	else if ( "brdrr"	== sCommand )		m_eInternalState = is_borderRight;
	//else if ( "brdrbtw" == sCommand )	m_eInternalState = is_borderRight;
	else if ( "brdrbar" == sCommand )		m_eInternalState = is_borderBar;
	else if ( "box"		== sCommand )		m_eInternalState = is_borderBox;

	else if ( "cldglu" == sCommand )		m_eInternalState = is_borderCellLR;
	else if ( "cldgll" == sCommand )		m_eInternalState = is_borderCellRL;
	else if ( "clbrdrl" == sCommand )		m_eInternalState = is_borderCellLeft;
	else if ( "clbrdrt" == sCommand )		m_eInternalState = is_borderCellTop;
	else if ( "clbrdrr" == sCommand )		m_eInternalState = is_borderCellRight;
	else if ( "clbrdrb" == sCommand )		m_eInternalState = is_borderCellBottom;

	else if ( "tsbrdrdgl" == sCommand )		m_eInternalState = is_borderCellLR;
	else if ( "tsbrdrdgr" == sCommand )		m_eInternalState = is_borderCellRL;

	else if ( "trbrdrl" == sCommand )		m_eInternalState = is_borderRowLeft;
	else if ( "trbrdrr" == sCommand )		m_eInternalState = is_borderRowRight;
	else if ( "trbrdrt" == sCommand )		m_eInternalState = is_borderRowTop;
	else if ( "trbrdrb" == sCommand )		m_eInternalState = is_borderRowBottom;
	else if ( "trbrdrv" == sCommand )		m_eInternalState = is_borderRowVer;
	else if ( "trbrdrh" == sCommand )		m_eInternalState = is_borderRowHor;

	else if ( "tsbrdrh" == sCommand ) 		m_eInternalState = is_borderRowHor;
	else if ( "tsbrdrv" == sCommand )		m_eInternalState = is_borderRowVer;
	else if ( "tsbrdrl" == sCommand )		m_eInternalState = is_borderRowLeft;
	else if ( "tsbrdrt" == sCommand )		m_eInternalState = is_borderRowTop;
	else if ( "tsbrdrr" == sCommand )		m_eInternalState = is_borderRowRight;
	else if ( "tsbrdrb" == sCommand )		m_eInternalState = is_borderRowBottom;
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
			default:
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
			default:
				break;
			}
			if ( bResult )	return true;
			
			return RtfTableRowPropsCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, m_pTableRowProps.get() );
		}
	}
	return true;
}

RtfOldListReader::RtfOldListReader( RtfOldList& oTarget) : m_oTarget(oTarget)
{
}
void RtfOldListReader::ExitReader( RtfDocument& oDocument, RtfReader& oReader )
{
	m_oTarget.m_oLevelText->m_oProperty.m_oCharProperty = oReader.m_oState->m_oCharProp;
}
bool RtfOldListReader::ExecuteCommand( RtfDocument& oDocument, RtfReader& oReader, std::string sKey, bool bHasPar, int nPar )
{
	if ( "pn" == sKey )
		return true;
	else if ( "pnlvlblt" == sKey )
	{
		m_oTarget.m_eLevelType = RtfOldList::lt_blt;
		m_oTarget.m_nIlvl = 0;
	}
	COMMAND_RTF_INT	( "pnf",		oReader.m_oState->m_oCharProp.m_nFont,		sKey, bHasPar, nPar )
			COMMAND_RTF_INT	( "pnfs",		oReader.m_oState->m_oCharProp.m_nFontSize,	sKey, bHasPar, nPar )
			COMMAND_RTF_BOOL( "pnb",		oReader.m_oState->m_oCharProp.m_bBold,		sKey, bHasPar, nPar )
			COMMAND_RTF_BOOL( "pni",		oReader.m_oState->m_oCharProp.m_bItalic,	sKey, bHasPar, nPar )
			COMMAND_RTF_BOOL( "pncaps",		oReader.m_oState->m_oCharProp.m_bCaps,		sKey, bHasPar, nPar )
			COMMAND_RTF_BOOL( "pnstrike",	oReader.m_oState->m_oCharProp.m_bStrike,	sKey, bHasPar, nPar )
			COMMAND_RTF_INT	( "pnul",		oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_Single)
			COMMAND_RTF_INT	( "pnuld",		oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_Dashed)
			COMMAND_RTF_INT	( "pnuldash",	oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_Dash_dotted)
			COMMAND_RTF_INT	( "pnuldashdd", oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_Dash_dot_dotted)
			COMMAND_RTF_INT	( "pnulth",		oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_Thick)
			COMMAND_RTF_INT	( "pnulwave",	oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_Wave)
			COMMAND_RTF_INT	( "pnuldb",		oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_Double)
			COMMAND_RTF_INT	( "pnulnone",	oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_none)
			COMMAND_RTF_INT	( "pnulnone",	oReader.m_oState->m_oCharProp.m_eUnderStyle, sKey, true, RtfCharProperty::uls_Word)

			COMMAND_RTF_INT ( "pnindent",	m_oTarget.m_oLevelText->m_oProperty.m_nIndLeft, sKey, bHasPar, nPar )
			COMMAND_RTF_INT ( "pnsp",		m_oTarget.m_oLevelText->m_oProperty.m_nIndLeft, sKey, bHasPar, nPar )
			COMMAND_RTF_INT ( "pnstart",	m_oTarget.m_oLevelText->m_oProperty.m_nIndLeft, sKey, bHasPar, nPar )
			COMMAND_RTF_INT ( "pnhang",		m_oTarget.m_oLevelText->m_oProperty.m_nIndLeft, sKey, bHasPar, nPar )

			COMMAND_RTF_INT ( "pnqc",		m_oTarget.m_eLevelJust, sKey, true, RtfOldList::lj_center )
			COMMAND_RTF_INT ( "pnql",		m_oTarget.m_eLevelJust, sKey, true, RtfOldList::lj_left )
			COMMAND_RTF_INT ( "pnqr",		m_oTarget.m_eLevelJust, sKey, true, RtfOldList::lj_right )

			else if ( "pntxtb" == sKey )
	{
		std::wstring sNumberingText;
		TextReader oTextReader( sNumberingText, false );
		StartSubReader( oTextReader, oDocument, oReader );
		
		RtfCharPtr pNewChar ( new RtfChar() );
		pNewChar->setText( sNumberingText );
		m_oTarget.m_oLevelText->AddItem( pNewChar );
	}
	else
	return false;
	return true;
}

void RtfHEXStringReader::ExecuteTextInternal2(RtfDocument& oDocument, RtfReader& oReader, std::string & sKey, int& nSkipChars)
{
	if (oReader.m_oState->m_sCurText.empty()) return;
	sHexString += oReader.m_oState->m_sCurText;

	oReader.m_oState->m_sCurText.clear();
}
void RtfHEXStringReader::ExitReader(RtfDocument& oDocument, RtfReader& oReader)
{
	if (sHexString.empty()) return;

	int nDataSize = sHexString.size() / 2;
	unsigned char *pData = new unsigned char[nDataSize];
	RtfUtility::DecodeHexString(sHexString, pData);

	pDataArray = std::make_pair(boost::shared_array<unsigned char>(pData), nDataSize);
}

RtfFontTableReader::RtfFontTableReader()
{
	m_bUseGlobalCodepage    = true;

	m_eInternalState = is_normal;
	m_oFont.SetDefaultOOX();
}
bool RtfFontTableReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
{
	if( "fonttbl" == sCommand )
		;
	else if( "flomajor" == sCommand )	m_oFont.m_eFontTheme = RtfFont::ft_flomajor;
	else if( "fhimajor" == sCommand )	m_oFont.m_eFontTheme = RtfFont::ft_fhimajor;
	else if( "fdbmajor" == sCommand )	m_oFont.m_eFontTheme = RtfFont::ft_fdbmajor;
	else if( "fbimajor" == sCommand )	m_oFont.m_eFontTheme = RtfFont::ft_fbimajor;
	else if( "flominor" == sCommand )	m_oFont.m_eFontTheme = RtfFont::ft_flominor;
	else if( "fhiminor" == sCommand )	m_oFont.m_eFontTheme = RtfFont::ft_fhiminor;
	else if( "fdbminor" == sCommand )	m_oFont.m_eFontTheme = RtfFont::ft_fdbminor;
	else if( "fbiminor" == sCommand )	m_oFont.m_eFontTheme = RtfFont::ft_fbiminor;

	else if( "fnil"		== sCommand )	m_oFont.m_eFontFamily = RtfFont::ff_fnil;
	else if( "froman"	== sCommand )	m_oFont.m_eFontFamily = RtfFont::ff_froman;
	else if( "fswiss"	== sCommand )	m_oFont.m_eFontFamily = RtfFont::ff_fswiss;
	else if( "fmodern"	== sCommand )	m_oFont.m_eFontFamily = RtfFont::ff_fmodern;
	else if( "fscript"	== sCommand )	m_oFont.m_eFontFamily = RtfFont::ff_fscript;
	else if( "fdecor"	== sCommand )	m_oFont.m_eFontFamily = RtfFont::ff_fdecor;
	else if( "ftech"	== sCommand )	m_oFont.m_eFontFamily = RtfFont::ff_ftech;
	else if( "fbidi"	== sCommand )	m_oFont.m_eFontFamily = RtfFont::ff_fbidi;

	else if( "panose"	== sCommand )	m_eInternalState	= is_panose;
	else if( "falt"		== sCommand )
		m_eInternalState	= is_altname;

	else if( "f" == sCommand )
	{
		if( true == hasParameter )
			m_oFont.m_nID = parameter;
	}
	else if( "fcharset" == sCommand )
	{
		if( true == hasParameter )
		{
			m_oFont.m_nCharset = parameter;
		}
	}
	else if( "cpg" == sCommand )
	{
		if( true == hasParameter )
		{
			m_oFont.m_nCodePage = parameter;
		}
	}
	else if( "fprq" == sCommand )
	{
		if( true == hasParameter )
			m_oFont.m_nPitch = parameter;
	}
	else
	{
		return false;
	}
	return true;
}
void RtfFontTableReader::ExecuteTextInternal2( RtfDocument& oDocument, RtfReader& oReader, std::string & sKey, int& nSkipChars)
{
	if( oReader.m_oState->m_sCurText.empty() ) return;

	//для корректного отображения названий шрифта нужно использовать данные самого шрифта

	int nKeepGlobalCodepage = oDocument.m_oProperty.m_nAnsiCodePage;

	if (m_oFont.m_nCodePage > 0)
	{
		oDocument.m_oProperty.m_nAnsiCodePage = m_oFont.m_nCodePage;
	}

	if (m_oFont.m_nCharset > 2 && oDocument.m_oProperty.m_nAnsiCodePage == 0)
	{
		oDocument.m_oProperty.m_nAnsiCodePage = RtfUtility::CharsetToCodepage(m_oFont.m_nCharset);
	}

	RtfAbstractReader::ExecuteTextInternal2(oDocument, oReader, sKey, nSkipChars);

	oDocument.m_oProperty.m_nAnsiCodePage = nKeepGlobalCodepage;
}
std::wstring RtfFontTableReader::RemoveLastUnchar(std::wstring str)
{
	size_t i = 1;
	while(true)
	{
		if (i > str.length())
			break;
		if (str[str.length() - i] <= 0x20)
			str.erase(str.length() - i , 1);
		else
			break;
	}
	return str;
}
void RtfFontTableReader::ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText)
{
	if( is_panose == m_eInternalState )
		m_oFont.m_sPanose += sText;
	else if( is_altname == m_eInternalState )
		m_oFont.m_sAltName += sText;
	else if( is_normal == m_eInternalState && sText.length() > 0)
	{
		size_t pos = sText.find(';');
		if (std::wstring::npos != pos)
		{
			sText = sText.substr(0, pos);

			if( std::wstring::npos != sText.find('&') )//todooo выясниснить что значит &;
			{
				//sText.Remove('&'); //
				m_oFont.m_sName += RemoveLastUnchar(sText);
			}
			else
			{
				m_oFont.m_sName += sText;
			}

			//todooo при добавлении могут быть повторы - убрать нннадо - goldwingSetting.rtf
			oDocument.m_oFontTable.DirectAddItem( m_oFont );

			m_oFont.SetDefaultRtf();
		}
		else
		{
			boost::algorithm::trim(sText);
			m_oFont.m_sName += sText;
		}
	}
}
void RtfFontTableReader::PopState(RtfDocument& oDocument, RtfReader& oReader)
{
	RtfAbstractReader::PopState( oDocument, oReader );

	if( is_panose == m_eInternalState )
		m_eInternalState = is_normal;
	if( is_altname == m_eInternalState )
		m_eInternalState = is_normal;
}

RtfColorTableReader::RtfColorTableReader()
{
	oCurColor.SetDefaultRtf();
	m_bIsSet = false;
}
bool RtfColorTableReader::ExecuteCommand( RtfDocument& oDocument, RtfReader& oReader, std::string sKey, bool bHasPar, int nPar )
{
	if( "colortbl" == sKey )
		return true;
	else if( "cmaindarkone"	== sKey )					oCurColor.m_eTheme = RtfColor::cmaindarkone;
	else if( "cmainlightone"	== sKey )				oCurColor.m_eTheme = RtfColor::cmainlightone;
	else if( "cmaindarktwo"		== sKey )				oCurColor.m_eTheme = RtfColor::cmaindarktwo;
	else if( "cmainlighttwo"	== sKey )				oCurColor.m_eTheme = RtfColor::cmainlighttwo;
	else if( "caccentone"		== sKey )				oCurColor.m_eTheme = RtfColor::caccentone;
	else if( "caccenttwo"		== sKey )				oCurColor.m_eTheme = RtfColor::caccenttwo;
	else if( "caccentthree"		== sKey )				oCurColor.m_eTheme = RtfColor::caccentthree;
	else if( "caccentfour"		== sKey )				oCurColor.m_eTheme = RtfColor::caccentfour;
	else if( "caccentfive"		== sKey )				oCurColor.m_eTheme = RtfColor::caccentfive;
	else if( "caccentsix"		== sKey )				oCurColor.m_eTheme = RtfColor::caccentsix;
	else if( "chyperlink"		== sKey )				oCurColor.m_eTheme = RtfColor::chyperlink;
	else if( "cfollowedhyperlink" == sKey )				oCurColor.m_eTheme = RtfColor::cfollowedhyperlink;
	else if( "cbackgroundone"	== sKey )				oCurColor.m_eTheme = RtfColor::cbackgroundone;
	else if( "ctextone"			== sKey )				oCurColor.m_eTheme = RtfColor::ctextone;
	else if( "cbackgroundtwo"	== sKey )				oCurColor.m_eTheme = RtfColor::cbackgroundtwo;
	else if( "ctexttwo"			== sKey )				oCurColor.m_eTheme = RtfColor::ctexttwo;

	else if( "ctint"	== sKey	&& true == bHasPar )	oCurColor.m_byteTint	= nPar;
	else if( "cshade"	== sKey	&& true == bHasPar )	oCurColor.m_byteShade	= nPar;
	else if( "red"		== sKey	&& true == bHasPar )	oCurColor.m_byteRed		= nPar;
	else if( "green"	== sKey	&& true == bHasPar )	oCurColor.m_byteGreen	= nPar;
	else if( "blue"		== sKey	&& true == bHasPar )	oCurColor.m_byteBlue	= nPar;
	else
	{
		return false;
	}
	m_bIsSet = true;
	return true;
}
void RtfColorTableReader::ExecuteText( RtfDocument& oDocument, RtfReader& oReader, std::wstring oText )
{
	//Romanization_Armenian.rtf
	//{\colortbl\red0\blue159\green82;\red0\blue0\green0;\red255\blue255\green255;\red0\blue156\green90;\red169\blue86\green0;}
	//{\colortbl;\red0\green0\blue0;\red0\green0\blue255;\red0\green255\blue255;\red0\green255\blue0;\red255\green0\blue255;
	size_t pos = oText.find(';');
	if( std::wstring::npos != pos)
	{
		if( true == m_bIsSet )
		{
			oDocument.m_oColorTable.DirectAddItem( oCurColor );
		}
		else
		{
			//Romanization_Armenian.rtf
			//{\colortbl\red0\blue159\green82;\red0\blue0\green0;\red255\blue255\green255;\red0\blue156\green90;\red169\blue86\green0;}
			//{\colortbl;\red0\green0\blue0;\red0\green0\blue255;\red0\green255\blue255;\red0\green255\blue0;\red255\green0\blue255;
			oCurColor.SetAuto();
			oDocument.m_oColorTable.DirectAddItem( oCurColor );
		}
		oCurColor.SetDefaultRtf();
		m_bIsSet = false;
	}
	else
	{

	}
}

RtfDefCharPropReader::RtfDefCharPropReader()
{
}
bool RtfDefCharPropReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
{
	if( "defchp" == sCommand )
		return true;
	else
		return RtfCharPropsCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, &oDocument.m_oDefaultCharProp);
}
void RtfDefCharPropReader::ExitReader( RtfDocument& oDocument, RtfReader& oReader )
{
}

RtfRevisionTableReader::RtfRevisionTableReader() {}
bool RtfRevisionTableReader::ExecuteCommand( RtfDocument& oDocument, RtfReader& oReader, std::string sKey, bool bHasPar, int nPar )
{
	if( "revtbl" == sKey )
		return true;

	return true;
}
void RtfRevisionTableReader::ExecuteText( RtfDocument& oDocument, RtfReader& oReader, std::wstring sText )
{
	if (std::wstring::npos != sText.find(L";"))
	{
		m_sCurrent += sText;
		XmlUtils::replace_all(m_sCurrent, L";", L"");
		oDocument.m_oRevisionTable.AddItem(m_sCurrent);

		m_sCurrent.clear();
	}
	else
	{
		m_sCurrent += sText;
	}
}

RtfInfoReader::RtfInfoReader()
{
	m_eInternalState = is_normal;
}
bool RtfInfoReader::ExecuteCommand( RtfDocument& oDocument, RtfReader& oReader,  std::string sCommand, bool hasParameter, int parameter)
{
	if( "info" == sCommand )
		return true;
	else if( "title"		== sCommand )		m_eInternalState = is_title;
	else if( "subject"		== sCommand )		m_eInternalState = is_subject;
	else if( "author"		== sCommand )		m_eInternalState = is_author;
	else if( "manager"		== sCommand )		m_eInternalState = is_manager;
	else if( "company"		== sCommand )		m_eInternalState = is_company;
	else if( "operator"		== sCommand )		m_eInternalState = is_operator;
	else if( "category"		== sCommand )		m_eInternalState = is_category;
	else if( "keywords"		== sCommand )		m_eInternalState = is_keywords;
	else if( "comment"		== sCommand )		m_eInternalState = is_comment;
	else if( "doccomm"		== sCommand )		m_eInternalState = is_doccomm;
	else if( "hlinkbase"	== sCommand )		m_eInternalState = is_hlinkbase;
	else if( "version"		== sCommand )
	{
		if( true == hasParameter )
			oDocument.m_oInformation.m_nVersion = parameter;
	}
	else if( "vern" == sCommand )
	{
		if( true == hasParameter )
			oDocument.m_oInformation.m_nInternalVersion = parameter;
	}
	else if( "edmins" == sCommand )
	{
		if( true == hasParameter )
			oDocument.m_oInformation.m_nEndingTime = parameter;
	}
	else if( "nofpages" == sCommand )
	{
		if( true == hasParameter )
			oDocument.m_oInformation.m_nNumberOfPages= parameter;
	}
	else if( "nofwords" == sCommand )
	{
		if( true == hasParameter )
			oDocument.m_oInformation.m_nNumberOfWords= parameter;
	}
	else if( "nofchars" == sCommand )
	{
		if( true == hasParameter )
			oDocument.m_oInformation.m_nNumberOfCharactersWithSpace= parameter;
	}
	else if( "nofcharsws" == sCommand )
	{
		if( true == hasParameter )
			oDocument.m_oInformation.m_nNumberOfCharactersWithoutSpace= parameter;
	}
	else if( "id" == sCommand )
	{
		if( true == hasParameter )
			oDocument.m_oInformation.m_nInternalId= parameter;
	}
	else if( "creatim" == sCommand )	m_eInternalState = is_creatim;
	else if( "revtim"	== sCommand )	m_eInternalState = is_revtim;
	else if( "printim" == sCommand )	m_eInternalState = is_printim;
	else if( "buptim"	== sCommand )	m_eInternalState = is_buptim;
	else if(  "yr"		== sCommand  )
	{
		if( true == hasParameter )
		{
			if( is_creatim		== m_eInternalState )	oDocument.m_oInformation.m_oCreateTime.m_nYear	= parameter;
			else if( is_revtim	== m_eInternalState )	oDocument.m_oInformation.m_oRevTime.m_nYear		= parameter;
			else if( is_printim == m_eInternalState )	oDocument.m_oInformation.m_oPrintTime.m_nYear	= parameter;
			else if( is_buptim	== m_eInternalState )	oDocument.m_oInformation.m_oBackupTime.m_nYear	= parameter;
		}
	}
	else if( "mo" == sCommand  )
	{
		if( true == hasParameter )
		{
			if( is_creatim		== m_eInternalState )	oDocument.m_oInformation.m_oCreateTime.m_nMonth = parameter;
			else if( is_revtim	== m_eInternalState )	oDocument.m_oInformation.m_oRevTime.m_nMonth	= parameter;
			else if( is_printim == m_eInternalState )	oDocument.m_oInformation.m_oPrintTime.m_nMonth	= parameter;
			else if( is_buptim	== m_eInternalState )	oDocument.m_oInformation.m_oBackupTime.m_nMonth	= parameter;
		}
	}
	else if ( "dy" == sCommand  )
	{
		if( true == hasParameter )
		{
			if( is_creatim		== m_eInternalState )	oDocument.m_oInformation.m_oCreateTime.m_nDay	= parameter;
			else if( is_revtim	== m_eInternalState )	oDocument.m_oInformation.m_oRevTime.m_nDay		= parameter;
			else if( is_printim == m_eInternalState )	oDocument.m_oInformation.m_oPrintTime.m_nDay	= parameter;
			else if( is_buptim	== m_eInternalState )	oDocument.m_oInformation.m_oBackupTime.m_nDay	= parameter;
		}
	}
	else if ( "hr" == sCommand  )
	{
		if( true == hasParameter )
		{
			if(		 is_creatim	== m_eInternalState )	oDocument.m_oInformation.m_oCreateTime.m_nHour	= parameter;
			else if( is_revtim	== m_eInternalState )	oDocument.m_oInformation.m_oRevTime.m_nHour		= parameter;
			else if( is_printim	== m_eInternalState )	oDocument.m_oInformation.m_oPrintTime.m_nHour	= parameter;
			else if( is_buptim	== m_eInternalState )	oDocument.m_oInformation.m_oBackupTime.m_nHour	= parameter;
		}
	}
	else if ( "min" == sCommand  )
	{
		if( true == hasParameter )
		{
			if( is_creatim == m_eInternalState )	oDocument.m_oInformation.m_oCreateTime.m_nMin	= parameter;
			else if( is_revtim	== m_eInternalState )	oDocument.m_oInformation.m_oRevTime.m_nMin		= parameter;
			else if( is_printim == m_eInternalState )	oDocument.m_oInformation.m_oPrintTime.m_nMin	= parameter;
			else if( is_buptim	== m_eInternalState )	oDocument.m_oInformation.m_oBackupTime.m_nMin	= parameter;
		}
	}
	else if ( "sec" == sCommand  )
	{
		if( true == hasParameter )
		{
			if(		 is_creatim		== m_eInternalState )	oDocument.m_oInformation.m_oCreateTime.m_nSecond	= parameter;
			else if( is_revtim		== m_eInternalState )	oDocument.m_oInformation.m_oRevTime.m_nSecond		= parameter;
			else if( is_printim		== m_eInternalState )	oDocument.m_oInformation.m_oPrintTime.m_nSecond		= parameter;
			else if( is_buptim		== m_eInternalState )	oDocument.m_oInformation.m_oBackupTime.m_nSecond	= parameter;
		}
	}
	else
	{
		return false;
	}
	return true;

}
void RtfInfoReader::ExecuteText( RtfDocument& oDocument, RtfReader& oReader, std::wstring sText )
{
	if		( is_title		== m_eInternalState )	oDocument.m_oInformation.m_sTitle		+= sText.c_str();
	else if	( is_subject	== m_eInternalState )	oDocument.m_oInformation.m_sSubject		+= sText.c_str();
	else if	( is_author		== m_eInternalState )	oDocument.m_oInformation.m_sAuthor		+= sText.c_str();
	else if	( is_manager	== m_eInternalState )	oDocument.m_oInformation.m_sManager		+= sText.c_str();
	else if	( is_company	== m_eInternalState )	oDocument.m_oInformation.m_sCompany		+= sText.c_str();
	else if	( is_operator	== m_eInternalState )	oDocument.m_oInformation.m_sOperator	+= sText.c_str();
	else if	( is_category	== m_eInternalState )	oDocument.m_oInformation.m_sCategory	+= sText.c_str();
	else if	( is_keywords	== m_eInternalState )	oDocument.m_oInformation.m_sKeywords	+= sText.c_str();
	else if( is_comment		== m_eInternalState )	oDocument.m_oInformation.m_sComment		+= sText.c_str();
	else if( is_doccomm		== m_eInternalState )	oDocument.m_oInformation.m_sDocCom		+= sText.c_str();
	else if( is_hlinkbase	== m_eInternalState )	oDocument.m_oInformation.m_sLinkBase	+= sText.c_str();
}

RtfPictureReader::PLACEABLEMETAHEADER::PLACEABLEMETAHEADER()
{
	Key = 0x9AC6CDD7;
	Handle = 0;
	Left = 0;
	Top = 0;
	Right = 0;
	Bottom = 0;
	Inch = 0x0902;
	Reserved = 0;
	Checksum = 0;
}
void RtfPictureReader::PLACEABLEMETAHEADER::CalculateChecksum()
{
	Checksum = 0;
	Checksum ^= (Key & 0x0000FFFFUL);
	Checksum ^= ((Key & 0xFFFF0000UL) >> 16);
	Checksum ^= Handle;
	Checksum ^= Left;
	Checksum ^= Top;
	Checksum ^= Right;
	Checksum ^= Bottom;
	Checksum ^= Inch;
	Checksum ^= (Reserved & 0x0000FFFFUL);
	Checksum ^= ((Reserved & 0xFFFF0000UL) >> 16);
}
std::wstring RtfPictureReader::PLACEABLEMETAHEADER::ToString()
{
	std::wstring sResult;
	sResult += ByteToString( (BYTE*)&Key, 4, true );
	sResult += ByteToString( (BYTE*)&Handle, 2, true );
	sResult += ByteToString( (BYTE*)&Left, 2, true );
	sResult += ByteToString( (BYTE*)&Top, 2, true );
	sResult += ByteToString( (BYTE*)&Right, 2, true );
	sResult += ByteToString( (BYTE*)&Bottom, 2, true );
	sResult += ByteToString( (BYTE*)&Inch, 2, true );
	sResult += ByteToString( (BYTE*)&Reserved, 4, true );
	sResult += ByteToString( (BYTE*)&Checksum, 2, true );
	return sResult;
}
std::wstring RtfPictureReader::PLACEABLEMETAHEADER::ByteToString( BYTE* pbData, int nSize, bool bLittleEnd )
{
	std::wstring sResult;
	if( true == bLittleEnd )
	{
		for( int i = 0; i < nSize; i++ )
		{
			BYTE byteVal = pbData[i];
			sResult += XmlUtils::ToString(byteVal, L"%02X");
		}
	}
	else
	{
		for( int i = nSize - 1 ; i >= 0; i-- )
		{
			BYTE byteVal = pbData[i];
			sResult += XmlUtils::ToString(byteVal, L"%02X");
		}
	}
	return sResult;
}

RtfPictureReader::RtfPictureReader( RtfReader& oReader, RtfShape& oShape ) : m_oShape(oShape)
{
	m_bBin = false;
	m_pbBin = NULL;
	m_nBinLength = 0;
}
RtfPictureReader::~RtfPictureReader()
{
	RELEASEARRAYOBJECTS(m_pbBin);
}
bool RtfPictureReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,  std::string sCommand, bool hasParameter, int parameter)
{
	if ( "pict" == sCommand )
		return true;
	else if ( "picprop" == sCommand )
	{
		RtfShapeReader oShapeReader(m_oShape);
		StartSubReader( oShapeReader, oDocument, oReader );
	}
	else if ( "emfblip" == sCommand )	m_oShape.m_oPicture->eDataType = RtfPicture::dt_emf;
	else if ( "pngblip" == sCommand )	m_oShape.m_oPicture->eDataType = RtfPicture::dt_png;
	else if ( "jpegblip" == sCommand )	m_oShape.m_oPicture->eDataType = RtfPicture::dt_jpg;
	else if ( "macpict" == sCommand )	m_oShape.m_oPicture->eDataType = RtfPicture::dt_macpict;
	else if ( "svgpict" == sCommand)	m_oShape.m_oPicture->eDataType = RtfPicture::dt_svg;

	else if ( "wmetafile" == sCommand )
	{
		if ( hasParameter && 8 == parameter )
			m_oShape.m_oPicture->eDataType = RtfPicture::dt_wmf;
	}
	else if ( "picw" == sCommand )
	{
		if ( hasParameter )
		{
			m_oShape.m_oPicture->m_nWidth = parameter;
		}
	}
	else if ( "pich" == sCommand )
	{
		if ( hasParameter )
		{
			m_oShape.m_oPicture->m_nHeight = parameter;
		}
	}
	else if ( "picwgoal" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_oPicture->m_nWidthGoal = parameter;
	}
	else if ( "pichgoal" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_oPicture->m_nHeightGoal = parameter;
	}
	else if ( "picscalex" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_oPicture->m_dScaleX = parameter;
	}
	else if ( "picscaley" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_oPicture->m_dScaleY = parameter;
	}
	else if ( "picscaled" == sCommand )
		m_oShape.m_oPicture->m_bScaled = 1;
	else if ( "piccropl" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_oPicture->m_nCropL = parameter;
	}
	else if ( "piccropt" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_oPicture->m_nCropT = parameter;
	}
	else if ( "piccropr" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_oPicture->m_nCropR = parameter;
	}
	else if ( "piccropb" == sCommand )
	{
		if ( hasParameter )
			m_oShape.m_oPicture->m_nCropB = parameter;
	}
	else if ( "bin" == sCommand)
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
void RtfPictureReader::ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText)
{
	m_sData += sText;
}
void RtfPictureReader::ExitReader( RtfDocument& oDocument, RtfReader& oReader )
{
	if( NULL != m_oShape.m_oPicture && RtfPicture::dt_wmf == m_oShape.m_oPicture->eDataType )
	{
		if( PROP_DEF != m_oShape.m_oPicture->m_nWidthGoal	&& PROP_DEF != (int)m_oShape.m_oPicture->m_dScaleX &&
				PROP_DEF != m_oShape.m_oPicture->m_nHeightGoal	&& PROP_DEF != (int)m_oShape.m_oPicture->m_dScaleY )
		{
			PLACEABLEMETAHEADER oPLACEABLEMETAHEADER;
			oPLACEABLEMETAHEADER.Right	= long( (m_oShape.m_oPicture->m_dScaleX / 100.0) * m_oShape.m_oPicture->m_nWidthGoal * ( 96.0 / 1440 ) ); //to pixel
			oPLACEABLEMETAHEADER.Bottom = long( (m_oShape.m_oPicture->m_dScaleY / 100.0) * m_oShape.m_oPicture->m_nHeightGoal * ( 96.0 / 1440 ) );
			oPLACEABLEMETAHEADER.CalculateChecksum();
			m_sData = oPLACEABLEMETAHEADER.ToString() + m_sData;
		}
	}

	std::wstring sTempFile = Utils::CreateTempFile( oReader.m_sTempFolder );
	if(m_bBin)
		RtfUtility::WriteDataToFileBinary( sTempFile, m_pbBin, m_nBinLength );
	else
		RtfUtility::WriteDataToFile( sTempFile, m_sData );

	if( RtfPicture::dt_none ==  m_oShape.m_oPicture->eDataType )
		m_oShape.m_oPicture->eDataType = RtfPicture::GetPictureType( sTempFile );

	m_oShape.m_oPicture->m_bIsCopy = true;
	m_oShape.m_oPicture->m_sPicFilename = sTempFile;
}

//----------------------------------------------------------------------------------------------------------
void RtfThemeDataReader::ExitReader(RtfDocument& oDocument, RtfReader& oReader)
{
	RtfHEXStringReader::ExitReader(oDocument, oReader);

	if (pDataArray.second < 1) return;

	HRESULT res = S_OK;
	COfficeUtils* pOfficeUtils = new COfficeUtils(NULL);

	if (pOfficeUtils)
	{
		std::wstring tempFileName = oReader.m_sTempFolder + FILE_SEPARATOR_STR + L"themeManager.zip";
		NSFile::CFileBinary file_test;
		file_test.CreateFileW(tempFileName);
		file_test.WriteFile(pDataArray.first.get(), pDataArray.second);
		file_test.CloseFile();

		BYTE *utf8Data = NULL;
		ULONG utf8DataSize = 0;
		pOfficeUtils->LoadFileFromArchive(tempFileName, L"theme/theme/theme1.xml", &utf8Data, utf8DataSize);

		if (utf8Data && utf8DataSize > 0)
		{
			oDocument.m_sThemeXml = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8(utf8Data, utf8DataSize);

			delete[]utf8Data;
		}
		delete pOfficeUtils;
		pOfficeUtils = NULL;
	}
}

//----------------------------------------------------------------------------------------------------------

void RtfColorSchemeReader::ExitReader(RtfDocument& oDocument, RtfReader& oReader)
{
	RtfHEXStringReader::ExitReader(oDocument, oReader);

	if (pDataArray.second < 1) return;

	XmlUtils::CXmlLiteReader oXmlReader;

	std::string sXml = std::string((char*)pDataArray.first.get(), pDataArray.second);
	if (false == oXmlReader.FromStringA(sXml)) return;
	
	if (!oXmlReader.ReadNextNode()) return; //

	std::string sName = oXmlReader.GetNameA();
	if ("a:clrMap" == sName)
	{
		if ((oXmlReader.GetAttributesCount()) && (oXmlReader.MoveToFirstAttribute()))
		{
			std::string sNameAttr = oXmlReader.GetNameA();
			while (!sNameAttr.empty())
			{
				//if ("bg1" == sNameAttr)
				//{
				//	oDocument. = oXmlReader.GetText();
				//}
				//else if ("tx1" == sNameAttr)
				//{
				//	oDocument. = oXmlReader.GetText();
				//}
				//else if ("bg2" == sNameAttr)
				//{
				//	oDocument. = oXmlReader.GetText();
				//}
				//else if ("tx2" == sNameAttr)
				//{
				//	oDocument. = oXmlReader.GetText();
				//}
				//else if ("accent1" == sNameAttr)
				//{
				//	oDocument. = oXmlReader.GetText();
				//}
				//else if ("hyperlink" == sNameAttr)
				//{
				//	oDocument. = oXmlReader.GetText();
				//}
				//else if ("followedHyperlink" == sNameAttr)
				//{
				//	oDocument. = sNameAttr.GetText();
				//}
				if (!oXmlReader.MoveToNextAttribute())
					break;
				sNameAttr = oXmlReader.GetNameA();
			}
			oXmlReader.MoveToElement();
		}
	}
}

//----------------------------------------------------------------------------------------------------------

void RtfDataStoreReader::ExitReader(RtfDocument& oDocument, RtfReader& oReader)
{
	RtfHEXStringReader::ExitReader(oDocument, oReader);
}

RtfParagraphPropDestination::RtfParagraphPropDestination( )
{
	nTargetItap			= PROP_DEF;
	m_bPar				= false;
	nCurItap			= 0;				//main document
	m_eInternalState	= is_normal;
	m_oCurParagraph		= RtfParagraphPtr		(new RtfParagraph());
	m_oTextItems		= TextItemContainerPtr	( new TextItemContainer() );
}
void RtfParagraphPropDestination::ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText)
{
	m_bPar = false;
	RtfCharPtr pNewChar ( new RtfChar() );
	pNewChar->m_oProperty = oReader.m_oState->m_oCharProp;
	pNewChar->setText( sText );
	m_oCurParagraph->AddItem( pNewChar );
}
void RtfParagraphPropDestination::ExecuteNumberChar( RtfDocument& oDocument, RtfReader& oReader, RtfAbstractReader& oAbstrReader, int nWinChar, int nMacChar )
{
	RtfFont oFont;
	if( true == oDocument.m_oFontTable.GetFont( oReader.m_oState->m_oCharProp.m_nFont, oFont ) )
	{
		bool bIsMac = false;
		if( PROP_DEF != oFont.m_nCharset )
		{
			if( true == RtfUtility::IsMacCharset(oFont.m_nCharset) )
				bIsMac = true;
		}
		else if( PROP_DEF != oFont.m_nCodePage )
		{
			if( true == RtfUtility::IsMacCodepage(oFont.m_nCodePage) )
				bIsMac = true;
		}
		std::string sBullet;
		if( true == bIsMac )
			sBullet += (char) nMacChar;
		else
			sBullet += (char)  nWinChar;
		int nSkip = 0;

		RtfAbstractReader reader;
		std::wstring sText = reader.ExecuteTextInternal( oDocument, oReader, sBullet, false, 0, nSkip );
		ExecuteText( oDocument, oReader, sText );
	}
}
void RtfParagraphPropDestination::EndRows(RtfReader& oReader)
{
	RtfTableRowPtr oNewTableRow ( new RtfTableRow() );
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
	for( int i = 0; i < oNewTableRow->GetCount() && i < oNewTableRow->m_oProperty.GetCount() ; i++ )
	{
		oNewTableRow->operator [](i)->m_oProperty = oNewTableRow->m_oProperty[i];
	}
	//Добавляем временный row
	aRows.push_back( oNewTableRow );
	aRowItaps.push_back( nCurItap );
}
void RtfParagraphPropDestination::AddItem( RtfParagraphPtr oItem, RtfReader& oReader, bool bEndCell, bool bEndRow )
{
	// 1 != oItem->m_oProperty.m_bInTable - параграф не в таблице
	// PROP_DEF != nTargetItap && oItem->m_oProperty.m_nItap <= nTargetItap - выставлено свойство,что вложенность - nTargetItap - это не таблица( Нужно для чтения параграфов в таблицах )
	if (	( 1 != oItem->m_oProperty.m_bInTable || 0 == oItem->m_oProperty.m_nItap ) //Платежное_поручение.rtf
			||	( PROP_DEF != nTargetItap && oItem->m_oProperty.m_nItap <= nTargetItap ) )
	{
		if ( nCurItap > 0 ) //Если до этого были только параграфы в таблицах - завершаем таблицу
		{
			if (bEndRow) EndRows(oReader); //ê¡ñ¿ó¿ñπá½∞¡á∩ »α«úαá¼¼á.rtf

			RtfTablePtr oNewTable ( new RtfTable() );
			oNewTable->m_oProperty = oCurRowProperty;

			for( int k = (int)aRows.size() - 1; k >= 0 ; k-- )
			{
				if ( aRowItaps[k] != nCurItap )
					break;
				
				oNewTable->InsertItem( aRows[k], 0 );

				aRows.erase(aRows.begin() + k);
				aRowItaps.erase(aRowItaps.begin() + k);
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
			RtfTablePtr oNewTable ( new RtfTable() );
			oNewTable->m_oProperty = oCurRowProperty;
			
			for( int k = (int)aRows.size() - 1; k >= 0 ; k-- )
			{
				if ( aRowItaps[k] != nCurItap )
					break;

				oNewTable->InsertItem( aRows[k], 0 );
				
				aRows.erase(aRows.begin() + k);
				aRowItaps.erase(aRowItaps.begin() + k);
			}
			//вычисляем свойства для OOX
			oNewTable->CalculateGridProp();
			//добавляем таблицу
			aCellRenderables.push_back( oNewTable );
			aItaps.push_back( oItem->m_oProperty.m_nItap );
		}
		nCurItap = oItem->m_oProperty.m_nItap;

		if ( bEndRow )
		{
			EndRows(oReader);
		}
		else
		{
			//добавляем параграф во временные cell
			aCellRenderables.push_back( oItem ); //содержит все параграфы, не разложенные по cell
			aItaps.push_back( nCurItap ); //содержит все номера вложенности параграфов
			
			if ( bEndCell )
			{
				RtfTableCellPtr oNewTableCell ( new RtfTableCell() );
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
void RtfParagraphPropDestination::Finalize( RtfReader& oReader/*, RtfSectionPtr pSection*/) 
{
	if ( false == m_bPar && m_oCurParagraph->IsValid())// потому что это не reader и нужно как-то загонять последний параграф
	{
		m_oCurParagraph->m_oProperty	= oReader.m_oState->m_oParagraphProp;
		//m_oCurParagraph->m_oProperty.m_pSection = pSection;
		
		m_oCurParagraph->m_oOldList		= RtfOldListPtr ( new RtfOldList() );
		*m_oCurParagraph->m_oOldList	= oReader.m_oState->m_oCurOldList;
		
		m_oCurParagraph->m_oProperty.m_oCharProperty = oReader.m_oState->m_oCharProp;
		m_oCurParagraph->m_oProperty.m_bInTable = PROP_DEF; // поле (гиперссылка) в таблице

		AddItem( m_oCurParagraph, oReader, false, false );
		m_oCurParagraph = RtfParagraphPtr(new RtfParagraph());
	}
	else// if (true == m_bPar) // bug 50434
	{
		if (false == aRows.empty() || false == aCells.empty()) // bug 39172
		{
			AddItem( m_oCurParagraph, oReader, false, false );
		}
		//if (pSection)
		//	m_oCurParagraph->m_oProperty.m_pSection = pSection;
	}
}
bool RtfParagraphPropDestination::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, RtfAbstractReader& oAbstrReader,  std::string sCommand, bool hasParameter, int parameter)
{
	m_oReader		= &oReader;
	m_bPar			= false;
	bool bContinue	= false;

	if ("pard" == sCommand)
	{
		oReader.m_oState->m_oCurOldList.SetDefault();
	}
	else if ( "tcelld" == sCommand)
	{
		oReader.m_oState->m_oCellProperty.SetDefaultRtf();
	}
	
	//----------------------------------------------------------------------------------
	if ( "par" == sCommand )
	{
		m_bPar = true;
		m_oCurParagraph->m_oProperty	= oReader.m_oState->m_oParagraphProp;
		m_oCurParagraph->m_oOldList		= RtfOldListPtr( new RtfOldList() );
		*m_oCurParagraph->m_oOldList	= oReader.m_oState->m_oCurOldList;
		m_oCurParagraph->m_oProperty.m_oCharProperty = oReader.m_oState->m_oCharProp;

		AddItem( m_oCurParagraph, oReader, false, false );
		m_oCurParagraph = RtfParagraphPtr(new RtfParagraph());

		return true;
	}
	else if ( "cell" == sCommand  || "nestcell" == sCommand )
	{
		//пример п 9 п 12.rtf
		//XXT_RV_VNP.rtf
		if (oReader.m_oState->m_oParagraphProp.m_bInTable == PROP_DEF)
			oReader.m_oState->m_oParagraphProp.m_bInTable = 1;
		if (oReader.m_oState->m_oParagraphProp.m_nItap == PROP_DEF)
			oReader.m_oState->m_oParagraphProp.m_nItap = 1;

		if (oReader.m_oState->m_oParagraphProp.m_bInTable == 1 && 0 == oReader.m_oState->m_oParagraphProp.m_nItap )//Платежное_поручение.rtf (ели по другому сбойная строка заменяется параграфами
			oReader.m_oState->m_oParagraphProp.m_nItap = 1;

		m_oCurParagraph->m_oProperty	= oReader.m_oState->m_oParagraphProp;
		m_oCurParagraph->m_oOldList		= RtfOldListPtr( new RtfOldList() );
		*m_oCurParagraph->m_oOldList	= oReader.m_oState->m_oCurOldList;

		m_oCurParagraph->m_oProperty.m_oCharProperty = oReader.m_oState->m_oCharProp;
		AddItem( m_oCurParagraph, oReader, true, false );
		m_oCurParagraph = RtfParagraphPtr(new RtfParagraph());

		return true;
	}
	else if ( "row" == sCommand || "nestrow" == sCommand)
	{
		m_oCurParagraph->m_oProperty	= oReader.m_oState->m_oParagraphProp;
		m_oCurParagraph->m_oOldList		= RtfOldListPtr( new RtfOldList() );
		*m_oCurParagraph->m_oOldList	= oReader.m_oState->m_oCurOldList;

		m_oCurParagraph->m_oProperty.m_oCharProperty = oReader.m_oState->m_oCharProp;
		AddItem( m_oCurParagraph, oReader, false, true );
		m_oCurParagraph = RtfParagraphPtr(new RtfParagraph());

		return true;
	}
	else if ( "cellx" == sCommand )
	{
		if ( hasParameter)
		{
			oReader.m_oState->m_oCellProperty.m_nCellx = parameter;
			oReader.m_oState->m_oRowProperty.AddItem( oReader.m_oState->m_oCellProperty );
			oReader.m_oState->m_oCellProperty.SetDefaultRtf();
		}
		//else
		//{
		//	if (oReader.m_oState->m_oRowProperty.GetCount() > 0)
		//	{
		//		oReader.m_oState->m_oCellProperty.m_nCellx = oReader.m_oState->m_oRowProperty[oReader.m_oState->m_oRowProperty.GetCount() - 1].m_nCellx;
		//		oReader.m_oState->m_oRowProperty.AddItem(oReader.m_oState->m_oCellProperty);
		//		oReader.m_oState->m_oCellProperty.SetDefaultRtf();
		//	}
		//}
	}
	else if ( "nonesttables" == sCommand )
	{
		oAbstrReader.Skip( oDocument, oReader );
	}
	else if ( "listtext" == sCommand )
	{
		oAbstrReader.Skip( oDocument, oReader );
	}
	else if ( "pntext" == sCommand )
	{
		//пропускаем списки office 95, если есть списки office 2007
		if ( oDocument.m_oListTable.GetCount() > 0 )
			oAbstrReader.Skip( oDocument, oReader );
		else
		{
			RtfParagraphReader oParagraphReader( "pntext", oReader );
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
	else if ( "pn" == sCommand )
	{
		if ( oDocument.m_oListTable.GetCount() > 0 )
			oAbstrReader.Skip( oDocument, oReader );
		else
		{
			RtfOldList oOldList;
			RtfOldListReader oOldListReader( oOldList );
			oAbstrReader.StartSubReader( oOldListReader, oDocument, oReader );

			oReader.m_oState->m_oCurOldList = oOldList;
			bool bExist = false;
			for (size_t i = 0; i < oDocument.m_aOldLists.size(); i++ )
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
				
				RtfOldListPtr oNewOldList ( new RtfOldList() );
				*oNewOldList = oReader.m_oState->m_oCurOldList;
				oDocument.m_aOldLists.push_back( oNewOldList );
			}
		}
	}

	COMMAND_RTF_INT	( "uc",	oReader.m_oState->m_nUnicodeClean, sCommand, hasParameter, parameter)
			//Tab todoooo перенести в ParagrProps (trackchanges)
			COMMAND_RTF_INT ( "tldot",		m_oCurTab.m_eLeader,	sCommand, true, RtfTab::tl_dot )
			COMMAND_RTF_INT ( "tlmdot",		m_oCurTab.m_eLeader,	sCommand, true, RtfTab::tl_mdot )
			COMMAND_RTF_INT ( "tlhyph",		m_oCurTab.m_eLeader,	sCommand, true, RtfTab::tl_hyph )
			COMMAND_RTF_INT ( "tlul",		m_oCurTab.m_eLeader,	sCommand, true, RtfTab::tl_ul )
			COMMAND_RTF_INT ( "tlth",		m_oCurTab.m_eLeader,	sCommand, true, RtfTab::tl_ul )
			COMMAND_RTF_INT ( "tqr",		m_oCurTab.m_eKind ,		sCommand, true, RtfTab::tk_tqr )
			COMMAND_RTF_INT ( "tqc",		m_oCurTab.m_eKind ,		sCommand, true, RtfTab::tk_tqc )
			COMMAND_RTF_INT ( "tqdec",		m_oCurTab.m_eKind ,		sCommand, true, RtfTab::tk_tqdec )
			else if ( "tb" == sCommand )
	{
		if ( hasParameter )
		{
			m_oCurTab.m_eKind	= RtfTab::tk_tqbar;
			m_oCurTab.m_nTab	= parameter;
			
			if (!oReader.m_oState->m_oParagraphProp.m_oTabs.m_aTabs.empty())
			{
				if (oReader.m_oState->m_oParagraphProp.m_oTabs.m_aTabs.back().m_nTab > m_oCurTab.m_nTab)
				{
					oReader.m_oState->m_oParagraphProp.m_oTabs.m_aTabs.clear();
				}
			}
			oReader.m_oState->m_oParagraphProp.m_oTabs.m_aTabs.push_back( m_oCurTab );
			m_oCurTab.SetDefault();
		}
	}
	else if ( "tx" == sCommand )
	{
		if ( hasParameter )
		{
			m_oCurTab.m_nTab = parameter;

			if (!oReader.m_oState->m_oParagraphProp.m_oTabs.m_aTabs.empty())
			{
				if (oReader.m_oState->m_oParagraphProp.m_oTabs.m_aTabs.back().m_nTab > m_oCurTab.m_nTab)
				{
					oReader.m_oState->m_oParagraphProp.m_oTabs.m_aTabs.clear();
				}
			}
			oReader.m_oState->m_oParagraphProp.m_oTabs.m_aTabs.push_back( m_oCurTab );
			m_oCurTab.SetDefault();
		}
	}
	else
		bContinue = true;

	if (false == bContinue) // compiler limit : blocks nested too deeply
	{
		m_oCurParagraph->SetValid(true);
		return true;
	}
	bContinue = false;
	//tableStyleProp
	if ( "*" == sCommand )
		;
	COMMAND_RTF_INT ( "yts",			oReader.m_oState->m_oParagraphProp.m_nTableStyle,		sCommand, hasParameter, parameter )

			COMMAND_RTF_BOOL( "tscfirstrow",	oReader.m_oState->m_oParagraphProp.m_bStyleFirstRow,	sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "tscfirstrow",	oReader.m_oState->m_oCellProperty.m_bStyleFirstRow,		sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "tscfirstrow",	oReader.m_oState->m_oRowProperty.m_bStyleFirstRow,		sCommand, hasParameter, parameter )

			COMMAND_RTF_BOOL( "tsclastrow",		oReader.m_oState->m_oParagraphProp.m_bStyleLastRow,		sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "tsclastrow",		oReader.m_oState->m_oCellProperty.m_bStyleLastRow,		sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "tsclastrow",		oReader.m_oState->m_oRowProperty.m_bStyleLastRow,		sCommand, hasParameter, parameter )

			COMMAND_RTF_BOOL( "tscfirstcol",	oReader.m_oState->m_oParagraphProp.m_bStyleFirstCollumn, sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "tscfirstcol",	oReader.m_oState->m_oCellProperty.m_bStyleFirstCol,		sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "tscfirstcol",	oReader.m_oState->m_oRowProperty.m_bStyleFirstCol,		sCommand, hasParameter, parameter )

			COMMAND_RTF_BOOL( "tsclastcol",		oReader.m_oState->m_oParagraphProp.m_bStyleLastCollumn, sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "tsclastcol",		oReader.m_oState->m_oCellProperty.m_bStyleLastCol,		sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "tsclastcol",		oReader.m_oState->m_oRowProperty.m_bStyleLastCol,		sCommand, hasParameter, parameter )

			COMMAND_RTF_BOOL( "tscbandhorzodd", oReader.m_oState->m_oParagraphProp.m_bStyleOddRowBand, sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "tscbandhorzodd", oReader.m_oState->m_oCellProperty.m_bStyleOddRowBand,	sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "tscbandhorzodd", oReader.m_oState->m_oRowProperty.m_bStyleOddRowBand,	sCommand, hasParameter, parameter )

			COMMAND_RTF_BOOL( "tscbandhorzeven", oReader.m_oState->m_oParagraphProp.m_bStyleEvenRowBand,	sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "tscbandhorzeven", oReader.m_oState->m_oCellProperty.m_bStyleEvenRowBand,	sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "tscbandhorzeven", oReader.m_oState->m_oRowProperty.m_bStyleEvenRowBand,		sCommand, hasParameter, parameter )

			COMMAND_RTF_BOOL( "tscbandvertodd", oReader.m_oState->m_oParagraphProp.m_bStyleOddColBand, sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "tscbandvertodd", oReader.m_oState->m_oCellProperty.m_bStyleOddColBand,	sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "tscbandvertodd", oReader.m_oState->m_oRowProperty.m_bStyleOddColBand,	sCommand, hasParameter, parameter )

			COMMAND_RTF_BOOL( "tscbandverteven", oReader.m_oState->m_oParagraphProp.m_bStyleEvenColBand,	sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "tscbandverteven", oReader.m_oState->m_oCellProperty.m_bStyleEvenColBand,	sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "tscbandverteven", oReader.m_oState->m_oRowProperty.m_bStyleEvenColBand,		sCommand, hasParameter, parameter )

			COMMAND_RTF_BOOL( "tscnwcell", oReader.m_oState->m_oParagraphProp.m_bStyleNWCell,	sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "tscnwcell", oReader.m_oState->m_oCellProperty.m_bStyleNWCell,	sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "tscnwcell", oReader.m_oState->m_oRowProperty.m_bStyleNWCell,	sCommand, hasParameter, parameter )

			COMMAND_RTF_BOOL( "tscnecell", oReader.m_oState->m_oParagraphProp.m_bStyleNECell,	sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "tscnecell", oReader.m_oState->m_oCellProperty.m_bStyleNECell,	sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "tscnecell", oReader.m_oState->m_oRowProperty.m_bStyleNECell,	sCommand, hasParameter, parameter )

			COMMAND_RTF_BOOL( "tscswcell", oReader.m_oState->m_oParagraphProp.m_bStyleSWCell,	sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "tscswcell", oReader.m_oState->m_oCellProperty.m_bStyleSWCell,	sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "tscswcell", oReader.m_oState->m_oRowProperty.m_bStyleSWCell,	sCommand, hasParameter, parameter )

			COMMAND_RTF_BOOL( "tscsecell", oReader.m_oState->m_oParagraphProp.m_bStyleSECell,	sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "tscsecell", oReader.m_oState->m_oCellProperty.m_bStyleSECell,	sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "tscsecell", oReader.m_oState->m_oRowProperty.m_bStyleSECell,	sCommand, hasParameter, parameter )
			//Math
			else if ( "mmath" == sCommand )
	{
		RtfMathPtr		pNewMath	( new RtfMath() );
		RtfMathReader	oMathReader	( pNewMath );
		
		oAbstrReader.StartSubReader( oMathReader, oDocument, oReader );
		m_oCurParagraph->AddItem(pNewMath);
	}
	//Drawing
	else if ( "shp" == sCommand )
	{
		RtfShapePtr oNewShape ( new RtfShape() );
		oNewShape->m_oCharProperty = oReader.m_oState->m_oCharProp;
		
		RtfShapeReader oShapeReader( *oNewShape );
		oAbstrReader.StartSubReader( oShapeReader, oDocument, oReader );
		
		if ( oNewShape->IsValid() )
			m_oCurParagraph->AddItem( oNewShape );
	}
	else if ( "do" == sCommand )
	{
		RtfShapePtr oNewShape ( new RtfShape() );
		oNewShape->m_oCharProperty = oReader.m_oState->m_oCharProp;
		
		RtfOldShapeReader oShapeReader( *oNewShape );
		oAbstrReader.StartSubReader( oShapeReader, oDocument, oReader );
		
		if ( oNewShape->IsValid() )
			m_oCurParagraph->AddItem( oNewShape );
	}
	else if ( "shppict" == sCommand )
	{
		RtfShapePtr oNewShape ( new RtfShape() );
		oNewShape->m_oCharProperty = oReader.m_oState->m_oCharProp;
		
		RtfShppictReader oShppictReader( *oNewShape );
		oAbstrReader.StartSubReader( oShppictReader, oDocument, oReader );

		if ( oNewShape->IsValid() )
			m_oCurParagraph->AddItem( oNewShape );
	}
	else if ( "pict" == sCommand )
	{
		RtfShapePtr oNewShape ( new RtfShape() );
		oNewShape->m_oCharProperty		= oReader.m_oState->m_oCharProp;
		oNewShape->m_nShapeType			= ODRAW::sptPictureFrame;
		oNewShape->m_nWrapType			= 3; // none
		oNewShape->m_nPositionHRelative = 3;//TCHAR
		oNewShape->m_nPositionVRelative = 3;//line
		oNewShape->m_nPositionH			= 0;//absolute
		oNewShape->m_nPositionV			= 0;//absolute
		oNewShape->m_oPicture			= RtfPicturePtr( new RtfPicture() );

		RtfPictureReader oPictureReader( oReader, *oNewShape);
		if (false == oAbstrReader.StartSubReader(oPictureReader, oDocument, oReader))
		{
			//open-rtf-document-image-error.rtf
			oPictureReader.Parse(oDocument, oReader);
		}

		if ( oNewShape->IsValid() )
			m_oCurParagraph->AddItem( oNewShape );
	}
	else if ( "shpgrp" == sCommand )
	{
		RtfShapePtr oNewShape ( new RtfShape() );
		
		oNewShape->m_bIsGroup		= true;
		oNewShape->m_nShapeType		= 1;
		oNewShape->m_oCharProperty	= oReader.m_oState->m_oCharProp;
		
		RtfShapeGroupReader oShapeGroupReader( *oNewShape );
		oAbstrReader.StartSubReader( oShapeGroupReader, oDocument, oReader );
		
		if ( oNewShape->IsValid() )
			m_oCurParagraph->AddItem( oNewShape );
	}
	else if ( "nonshppict" == sCommand )
	{
		oAbstrReader.Skip( oDocument, oReader );
	}
	else if ( "field" == sCommand )
	{
		RtfFieldPtr		oNewField		(new RtfField());
		RtfFieldReader	oFieldReader	( *oNewField );
		
		oAbstrReader.StartSubReader( oFieldReader, oDocument, oReader );
		
		if ( oNewField->IsValid() )
			m_oCurParagraph->AddItem( oNewField );
	}
	else if ( "object" == sCommand )
	{
		RtfOlePtr oNewOleObject = RtfOlePtr( new RtfOle() );
		oNewOleObject->m_oCharProperty = oReader.m_oState->m_oCharProp;
		
		RtfOleReader oOleReader( *oNewOleObject );
		oAbstrReader.StartSubReader( oOleReader, oDocument, oReader );
		
		if ( oNewOleObject->IsValid() )
			m_oCurParagraph->AddItem( oNewOleObject );
	}
	else if ( "bkmkstart" == sCommand )
	{
		RtfBookmarkStartPtr		pNewBookmarkStart	( new RtfBookmarkStart() );
		RtfBookmarkStartReader	oBookmarkStartReader( *pNewBookmarkStart );
		
		oAbstrReader.StartSubReader( oBookmarkStartReader, oDocument, oReader );
		if ( pNewBookmarkStart->IsValid() )
			m_oCurParagraph->AddItem( pNewBookmarkStart );
	}
	else if ( "bkmkend" == sCommand )
	{
		RtfBookmarkEndPtr		pNewBookmarkEnd		( new RtfBookmarkEnd() );
		RtfBookmarkEndReader	oBookmarkEndReader	( *pNewBookmarkEnd );

		oAbstrReader.StartSubReader( oBookmarkEndReader, oDocument, oReader );
		
		if ( pNewBookmarkEnd->IsValid() )
			m_oCurParagraph->AddItem( pNewBookmarkEnd );
	}
	else if ( "atrfstart" == sCommand )
	{
		RtfAnnotElemPtr		pNewAnnotElem		( new RtfAnnotElem(1) );
		RtfAnnotElemReader	oAnnotElemReader	( *pNewAnnotElem );

		oAbstrReader.StartSubReader( oAnnotElemReader, oDocument, oReader );
		
		if ( pNewAnnotElem->IsValid() )
			m_oCurParagraph->AddItem( pNewAnnotElem );
	}
	else if ( "atrfend" == sCommand )
	{
		RtfAnnotElemPtr		pNewAnnotElem		( new RtfAnnotElem(2) );
		RtfAnnotElemReader	oAnnotElemReader	( *pNewAnnotElem );

		oAbstrReader.StartSubReader( oAnnotElemReader, oDocument, oReader );
		
		if ( pNewAnnotElem->IsValid() )
			m_oCurParagraph->AddItem( pNewAnnotElem );

	}
	else if ( "annotation" == sCommand )
	{
		RtfAnnotationPtr	pNewAnnot		( new RtfAnnotation() );
		RtfAnnotationReader	oAnnotReader	( *pNewAnnot );
		
		oAbstrReader.StartSubReader( oAnnotReader, oDocument, oReader );
		
		if ( pNewAnnot->IsValid() )
			m_oCurParagraph->AddItem( pNewAnnot );
	}
	else if ( "atnid" == sCommand )
	{
		RtfAnnotElemPtr		pNewAnnotElem	( new RtfAnnotElem(5) );
		RtfAnnotElemReader	oAnnotElemReader( *pNewAnnotElem );
		
		oAbstrReader.StartSubReader( oAnnotElemReader, oDocument, oReader );
		
		if ( pNewAnnotElem->IsValid() )
			m_oCurParagraph->AddItem( pNewAnnotElem );
	}
	else if ( "atnauthor" == sCommand )
	{
		RtfAnnotElemPtr		pNewAnnotElem	( new RtfAnnotElem(4) );
		RtfAnnotElemReader	oAnnotElemReader( *pNewAnnotElem );
		
		oAbstrReader.StartSubReader( oAnnotElemReader, oDocument, oReader );
		
		if ( pNewAnnotElem->IsValid() )
			m_oCurParagraph->AddItem( pNewAnnotElem );
	}
	else if ( "atnref" == sCommand )
	{
		RtfAnnotElemPtr		pNewAnnotElem		( new RtfAnnotElem(3) );
		RtfAnnotElemReader	oAnnotElemReader	( *pNewAnnotElem );
		
		oAbstrReader.StartSubReader( oAnnotElemReader, oDocument, oReader );
		
		if ( pNewAnnotElem->IsValid() )
			m_oCurParagraph->AddItem( pNewAnnotElem );

	}
	else if ( "footnote" == sCommand )
	{
		RtfFootnotePtr pNewFootnote ( new RtfFootnote() );
		pNewFootnote->m_oCharProp = oReader.m_oState->m_oCharProp;
		
		RtfFootnoteReader oFootnoteReader( *pNewFootnote );
		oAbstrReader.StartSubReader( oFootnoteReader, oDocument, oReader );
		
		if ( pNewFootnote->IsValid() )
			m_oCurParagraph->AddItem( pNewFootnote );
	}
	//else if ( "chatn" == sCommand )
	//{
	//	RtfCharSpecialPtr pNewChar ( new RtfCharSpecial() );
	//
	//	pNewChar->m_oProperty = oReader.m_oState->m_oCharProp;
	//	pNewChar->m_eType = RtfCharSpecial::rsc_chatn;
	//	m_oCurParagraph->AddItem( pNewChar );
	//}
	else if ( "chpgn" == sCommand ) //todooo - other special
	{//header & footer
		RtfCharSpecialPtr pNewChar ( new RtfCharSpecial() );
		
		pNewChar->m_oProperty = oReader.m_oState->m_oCharProp;
		pNewChar->m_eType = RtfCharSpecial::rsc_chpgn;
		m_oCurParagraph->AddItem( pNewChar );
	}
	else if ( "chftn" == sCommand )
	{
		if ( 1 == oReader.m_nFootnote )
		{
			RtfCharSpecialPtr pNewChar ( new RtfCharSpecial() );
			
			pNewChar->m_oProperty = oReader.m_oState->m_oCharProp;
			pNewChar->m_eType = RtfCharSpecial::rsc_chftn;
			m_oCurParagraph->AddItem( pNewChar );
		}
		else if ( 2 == oReader.m_nFootnote )
		{
			RtfCharSpecialPtr pNewChar ( new RtfCharSpecial() );
			
			pNewChar->m_oProperty = oReader.m_oState->m_oCharProp;
			pNewChar->m_eType = RtfCharSpecial::rsc_chftnEnd;
			m_oCurParagraph->AddItem( pNewChar );
		}
	}
	else if ( "chftnsep" == sCommand || "chftnsepc" == sCommand )
	{
		RtfCharSpecialPtr pNewChar ( new RtfCharSpecial() );
		
		pNewChar->m_oProperty = oReader.m_oState->m_oCharProp;
		m_oCurParagraph->AddItem( pNewChar );
		
		if		( "chftnsep"	== sCommand )	pNewChar->m_eType = RtfCharSpecial::rsc_chftnsep;
		else if ( "chftnsepc"	== sCommand )	pNewChar->m_eType = RtfCharSpecial::rsc_chftnsepc;
	}//specialChars
	else if ( "page" == sCommand  )
	{
		RtfCharSpecialPtr pNewChar ( new RtfCharSpecial() );
		
		pNewChar->m_eType = RtfCharSpecial::rsc_page;
		pNewChar->m_oProperty = oReader.m_oState->m_oCharProp;
		m_oCurParagraph->AddItem( pNewChar );
		//Microsoft (Demo-Hayden-Management-v2).docx
		//после разрыва могут быть и графические элементы .. их надо оставить в ЭТОМ же параграфе
		//разрыв страницы со следующего параграфа ..
		//m_oCurParagraph->m_oProperty = oReader.m_oState->m_oParagraphProp;
		//m_oCurParagraph->m_oOldList ( new RtfOldList() );
		//*m_oCurParagraph->m_oOldList = oReader.m_oState->m_oCurOldList;
		//m_oCurParagraph->m_oProperty.m_oCharProperty = oReader.m_oState->m_oCharProp;
		//AddItem( m_oCurParagraph, oReader, false, false );
		//m_oCurParagraph = RtfParagraphPtr(new RtfParagraph());
	}
	COMMAND_RTF_SPECIAL_CHAR( "column",	m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_column )
			COMMAND_RTF_SPECIAL_CHAR( "line",	m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_line )
			else if ( "lbr" == sCommand )
	{
		if ( hasParameter )
		{
			RtfCharSpecialPtr pNewChar ( new RtfCharSpecial() );
			
			pNewChar->m_nTextWrapBreak	= parameter;
			pNewChar->m_oProperty		= oReader.m_oState->m_oCharProp;
			m_oCurParagraph->AddItem( pNewChar );
		}
	}
	COMMAND_RTF_SPECIAL_CHAR( "softpage",	m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_softpage )
			COMMAND_RTF_SPECIAL_CHAR( "softcol",	m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_softcol )
			COMMAND_RTF_SPECIAL_CHAR( "softline",	m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_softline )

			else if ( "softlheight" == sCommand )
	{
		if ( hasParameter )
		{
			RtfCharSpecialPtr pNewChar	= RtfCharSpecialPtr( new RtfCharSpecial() );
			pNewChar->m_nSoftHeight		= parameter;
			pNewChar->m_oProperty		= oReader.m_oState->m_oCharProp;
			m_oCurParagraph->AddItem( pNewChar );
		}
	}
	COMMAND_RTF_SPECIAL_CHAR( "tab", m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_tab )

			else if ( "emdash" == sCommand )
			ExecuteNumberChar( oDocument, oReader, oAbstrReader, 151, 0xD0 ); // bullet Word for Windows - 151	; Apple Macintosh - 0xD0
	else if ( "endash" == sCommand )
		ExecuteNumberChar( oDocument, oReader, oAbstrReader, 150, 0xD1 ); // bullet Word for Windows - 150	; Apple Macintosh - 0xD1
	
	COMMAND_RTF_SPECIAL_CHAR( "emspace", m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_emspace )
			COMMAND_RTF_SPECIAL_CHAR( "enspace", m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_enspace )
			COMMAND_RTF_SPECIAL_CHAR( "qmspace", m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_qmspace )

			else if ( "bullet" == sCommand )
			ExecuteNumberChar( oDocument, oReader, oAbstrReader, 149, 0xA5 ); // bullet Word for Windows - 149	; Apple Macintosh - 0xA5
	else if ( "lquote" == sCommand )
		ExecuteNumberChar( oDocument, oReader, oAbstrReader, 145, 0xD4 ); // bullet Word for Windows - 145	; Apple Macintosh - 0xD4
	else if ( "rquote" == sCommand )
		ExecuteNumberChar( oDocument, oReader, oAbstrReader, 146, 0xD5 ); // bullet Word for Windows - 146	; Apple Macintosh - 0xD5
	else if ( "ldblquote" == sCommand )
		ExecuteNumberChar( oDocument, oReader, oAbstrReader, 147, 0xD2 ); // bullet Word for Windows - 147	; Apple Macintosh - 0xD2
	else if ( "rdblquote" == sCommand )
		ExecuteNumberChar( oDocument, oReader, oAbstrReader, 148, 0xD3 ); // bullet Word for Windows - 148	; Apple Macintosh - 0xD3
	
	COMMAND_RTF_SPECIAL_CHAR( "|",		m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_Formula )
			COMMAND_RTF_SPECIAL_CHAR( "~",		m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_NonBrSpace )
			COMMAND_RTF_SPECIAL_CHAR( "-",		m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_OptHyphen )
			COMMAND_RTF_SPECIAL_CHAR( "_",		m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_NonBrHyphen )
			COMMAND_RTF_SPECIAL_CHAR( ":",		m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_SubEntry )
			COMMAND_RTF_SPECIAL_CHAR( "zwbo",	m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_zwbo )
			COMMAND_RTF_SPECIAL_CHAR( "zwnbo", m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_zwnbo )
			COMMAND_RTF_SPECIAL_CHAR( "zwj",	m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_zwj )
			COMMAND_RTF_SPECIAL_CHAR( "zwnj",	m_oCurParagraph, sCommand, hasParameter, RtfCharSpecial::rsc_zwnj )

			else if ( "oldcprops"  == sCommand )
	{
		RtfCharPropertyPtr props ( new RtfCharProperty() );
		RtfTrackerChangesReader oOldPropReader(props);
		
		if (oAbstrReader.StartSubReader( oOldPropReader, oDocument, oReader ))
		{
			oReader.m_oState->m_oCharProp.m_pOldCharProp = props;
		}
	}
	else if ( "oldpprops"  == sCommand )
	{
		RtfParagraphPropertyPtr props ( new RtfParagraphProperty() );
		RtfTrackerChangesReader oOldPropReader(props);
		
		if (oAbstrReader.StartSubReader( oOldPropReader, oDocument, oReader ))
		{
			oReader.m_oState->m_oParagraphProp.m_pOldParagraphProp = props;
		}
	}
	else if ( "oldsprops"  == sCommand )
	{
		RtfSectionPropertyPtr props ( new RtfSectionProperty() );
		RtfTrackerChangesReader oOldPropReader(props);
		
		if (oAbstrReader.StartSubReader( oOldPropReader, oDocument, oReader ))
		{
			oReader.m_oCurSectionProp.m_pOldSectionProp = props;
		}
	}
	else if ( "oldtprops"  == sCommand )
	{
		RtfRowPropertyPtr props ( new RtfRowProperty() );
		RtfTrackerChangesReader oOldPropReader(props);
		
		if (oAbstrReader.StartSubReader( oOldPropReader, oDocument, oReader ))
		{
			oReader.m_oState->m_oRowProperty.m_pOldRowProperty = props;
		}
	}
	else if ( "chbrdr"	== sCommand )		m_eInternalState = is_charBorder;
	else if ( "brdrt"	== sCommand )		m_eInternalState = is_borderTop;
	else if ( "brdrb"	== sCommand )		m_eInternalState = is_borderBottom;
	else if ( "brdrl"	== sCommand )		m_eInternalState = is_borderLeft;
	else if ( "brdrr"	== sCommand )		m_eInternalState = is_borderRight;
	//else if ( "brdrbtw" == sCommand )		m_eInternalState = is_borderRight;
	else if ( "brdrbar" == sCommand )		m_eInternalState = is_borderBar;
	else if ( "box"	== sCommand )			m_eInternalState = is_borderBox;

	else if ( "cldglu" == sCommand )		m_eInternalState = is_borderCellLR;
	else if ( "cldgll" == sCommand )		m_eInternalState = is_borderCellRL;
	else if ( "clbrdrl" == sCommand )		m_eInternalState = is_borderCellLeft;
	else if ( "clbrdrt" == sCommand )		m_eInternalState = is_borderCellTop;
	else if ( "clbrdrr" == sCommand )		m_eInternalState = is_borderCellRight;
	else if ( "clbrdrb" == sCommand )		m_eInternalState = is_borderCellBottom;

	else if ( "tsbrdrdgl" == sCommand )		m_eInternalState = is_borderCellLR;
	else if ( "tsbrdrdgr" == sCommand )		m_eInternalState = is_borderCellRL;

	else if ( "trbrdrl" == sCommand )		m_eInternalState = is_borderRowLeft;
	else if ( "trbrdrr" == sCommand )		m_eInternalState = is_borderRowRight;
	else if ( "trbrdrt" == sCommand )		m_eInternalState = is_borderRowTop;
	else if ( "trbrdrb" == sCommand )		m_eInternalState = is_borderRowBottom;
	else if ( "trbrdrv" == sCommand )		m_eInternalState = is_borderRowVer;
	else if ( "trbrdrh" == sCommand )		m_eInternalState = is_borderRowHor;

	else if ( "tsbrdrh" == sCommand ) 		m_eInternalState = is_borderRowHor;
	else if ( "tsbrdrv" == sCommand )		m_eInternalState = is_borderRowVer;
	else if ( "tsbrdrl" == sCommand )		m_eInternalState = is_borderRowLeft;
	else if ( "tsbrdrt" == sCommand )		m_eInternalState = is_borderRowTop;
	else if ( "tsbrdrr" == sCommand )		m_eInternalState = is_borderRowRight;
	else if ( "tsbrdrb" == sCommand )		m_eInternalState = is_borderRowBottom;
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
		default:
			break;
		}
		if ( bResult )	return true;

		if (RtfShadingRowCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oRowProperty.m_oShading ))
			return true;
		
		if ( is_charBorder == m_eInternalState )
			if (RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter,  oReader.m_oState->m_oCharProp.m_poBorder))
				return true;
		
		if (RtfTableRowPropsCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, &oReader.m_oState->m_oRowProperty ))
			return true;
		
		if (RtfTableCellPropsCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, &oReader.m_oState->m_oCellProperty ))
			return true;

		if (RtfParagraphPropsCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, &oReader.m_oState->m_oParagraphProp ))
			return true;
		
		if (RtfCharPropsCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, &oReader.m_oState->m_oCharProp ))
			return true;

		return false;
	}
	m_oCurParagraph->SetValid(true);
	return true;
}

RtfFootnoteReader::RtfFootnoteReader( RtfFootnote& oRtfFootnote ):m_oRtfFootnote(oRtfFootnote)
{
}
bool RtfFootnoteReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
{
	if( "footnote" == sCommand )
	{
		oReader.m_nFootnote = 1;
	}
	else if( "ftnalt" == sCommand )
	{
		m_oRtfFootnote.m_bEndNote = true;
		oReader.m_nFootnote = 2;
	}
	else
		return m_oParPropDest.ExecuteCommand( oDocument, oReader, (*this), sCommand, hasParameter, parameter );

	return true;
}
void RtfFootnoteReader::ExecuteText( RtfDocument& oDocument, RtfReader& oReader, std::wstring sText )
{
	m_oParPropDest.ExecuteText( oDocument, oReader, sText );
}
void RtfFootnoteReader::ExitReader( RtfDocument& oDocument, RtfReader& oReader )
{
	m_oParPropDest.Finalize( oReader );

	m_oRtfFootnote.m_oContent	= m_oParPropDest.m_oTextItems;
	oReader.m_nFootnote			= PROP_DEF;
}

RtfAnnotationReader::RtfAnnotationReader( RtfAnnotation& oRtfAnnotation ) : m_oRtfAnnotation(oRtfAnnotation)
{
}
bool RtfAnnotationReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
{
	if( "annotation" == sCommand )
	{
		return true;
	}
	else if( "atnref" == sCommand )
	{
		m_oRtfAnnotation.m_oRef = RtfAnnotElemPtr ( new RtfAnnotElem(3) );
		RtfAnnotElemReader	oAnnotReader	( *m_oRtfAnnotation.m_oRef );

		StartSubReader( oAnnotReader, oDocument, oReader );

	}
	else if( "atndate" == sCommand )
	{
		m_oRtfAnnotation.m_oDate = RtfAnnotElemPtr ( new RtfAnnotElem(6) );
		RtfAnnotElemReader	oAnnotReader	( *m_oRtfAnnotation.m_oDate );

		StartSubReader( oAnnotReader, oDocument, oReader );
	}
	else if( "atnparent" == sCommand )
	{
		m_oRtfAnnotation.m_oParent = RtfAnnotElemPtr ( new RtfAnnotElem(7) );
		RtfAnnotElemReader	oAnnotReader	( *m_oRtfAnnotation.m_oParent );

		StartSubReader( oAnnotReader, oDocument, oReader );
	}
	else
		return m_oParPropDest.ExecuteCommand( oDocument, oReader, (*this), sCommand, hasParameter, parameter );
	return true;
}
void RtfAnnotationReader::ExecuteText( RtfDocument& oDocument, RtfReader& oReader, std::wstring sText )
{
	m_oParPropDest.ExecuteText( oDocument, oReader, sText );
}
void RtfAnnotationReader::ExitReader( RtfDocument& oDocument, RtfReader& oReader )
{
	m_oParPropDest.Finalize( oReader );

	m_oRtfAnnotation.m_oContent = m_oParPropDest.m_oTextItems;
}

RtfDefParPropReader::RtfDefParPropReader( )
{
}
bool RtfDefParPropReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
{
	if( "defpap" == sCommand )
		return true;
	else
		return m_oParPropDest.ExecuteCommand( oDocument, oReader, (*this), sCommand, hasParameter, parameter);
}
void RtfDefParPropReader::ExitReader(RtfDocument& oDocument, RtfReader& oReader)
{
	oDocument.m_oDefaultParagraphProp = oReader.m_oState->m_oParagraphProp;
}

RtfStyleTableReader::RtfStyleReader::RtfStyleReader()
{
	m_eInternalState = is_normal;

	m_oCurStyle = RtfParagraphStylePtr ( new RtfParagraphStyle() );
	m_oCurStyle->m_nID = 0;
}
bool RtfStyleTableReader::RtfStyleReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
{
	if( "s" == sCommand )
	{
		if( true == hasParameter )
		{
			m_oCurStyle = RtfParagraphStylePtr ( new RtfParagraphStyle() );
			m_oCurStyle->m_nID = parameter;
		}
	}
	else if( "cs" == sCommand )
	{
		if( true == hasParameter )
		{
			m_oCurStyle = RtfCharStylePtr ( new RtfCharStyle() );
			m_oCurStyle->m_nID = parameter;
		}
	}
	else if( "ts" == sCommand )
	{
		if( true == hasParameter )
		{
			m_oCurStyle = RtfTableStylePtr ( new RtfTableStyle() );
			m_oCurStyle->m_nID = parameter;
		}
	}
	COMMAND_RTF_INT( "sbasedon",	m_oCurStyle->m_nBasedOn,			sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( "snext",		m_oCurStyle->m_nNext,				sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( "slink",		m_oCurStyle->m_nLink,				sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "sqformat",	m_oCurStyle->m_bQFormat,			sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( "spriority",	m_oCurStyle->m_nPriority,			sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "sunhideused", m_oCurStyle->m_bUnhiddenWhenUse,	sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "slocked",	m_oCurStyle->m_bLocked,				sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "shidden",	m_oCurStyle->m_bHidden,				sCommand, hasParameter, parameter )
			else if( "ssemihidden" == sCommand )
	{
		if( true == hasParameter && 0 == parameter)
			m_oCurStyle->m_nSemiHidden = 0;
		else
			m_oCurStyle->m_nSemiHidden = 1;
	}
	COMMAND_RTF_BOOL( "spersonal",	m_oCurStyle->m_bPersonal,	sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "scompose",	m_oCurStyle->m_bCompose,	sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "sreply",		m_oCurStyle->m_bReply,		sCommand, hasParameter, parameter )

			//tableStyleCommands
			//else if( "tscellpaddt" == sCommand )
			//{
			//	if( true == hasParameter )
			//		m_oTableStyleProperty.m_nPaddingTop = parameter;
			//}
			//else if( "tscellpaddl" == sCommand )
			//{
			//	if( true == hasParameter )
			//		m_oTableStyleProperty.m_nPaddingLeft = parameter;
			//}
			//else if( "tscellpaddr" == sCommand )
			//{
			//	if( true == hasParameter )
			//		m_oTableStyleProperty.m_nPaddingRight = parameter;
			//}
			//else if( "tscellpaddb" == sCommand )
			//{
			//	if( true == hasParameter )
			//		m_oTableStyleProperty.m_nPaddingBottom = parameter;
			//}
			//else if( "tscellpaddft" == sCommand )
			//{
			//	if( true == hasParameter )
			//		m_oTableStyleProperty.m_nIsPaddingTop = parameter;
			//}
			//else if( "tscellpaddfl" == sCommand )
			//{
			//	if( true == hasParameter )
			//		m_oTableStyleProperty.m_nIsPaddingLeft = parameter;
			//}
			//else if( "tscellpaddfr" == sCommand )
			//{
			//	if( true == hasParameter )
			//		m_oTableStyleProperty.m_nIsPaddingRight = parameter;
			//}
			//else if( "tscellpaddfb" == sCommand )
			//{
			//	if( true == hasParameter )
			//		m_oTableStyleProperty.m_nIsPaddingBottom = parameter;
			//}
			//else if( "tsvertalt" == sCommand )
			//	m_oTableStyleProperty.m_eAlign = RtfTableStyleProperty::av_tsvertalt;
			//else if( "tsvertalc" == sCommand )
			//	m_oTableStyleProperty.m_eAlign = RtfTableStyleProperty::av_tsvertalc;
			//else if( "tsvertalb" == sCommand )
			//	m_oTableStyleProperty.m_eAlign = RtfTableStyleProperty::av_tsvertalb;
			//else if( "tsnowrap" == sCommand )
			//	m_oTableStyleProperty.m_bNoCellWrap = 1;
			//else if( "tscbandsh" == sCommand )
			//{
			//	if( true == hasParameter )
			//		m_oTableStyleProperty.m_nRowBandNumber = parameter;
			//}
			//else if( "tscbandsv" == sCommand )
			//{
			//	if( true == hasParameter )
			//		m_oTableStyleProperty.m_nCellBandNumber = parameter;
			//}
			//else if(L"tsbghoriz" == sCommand )
			//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbghoriz;
			//else if( "tsbgvert" == sCommand )
			//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgvert;
			//else if( "tsbgfdiag" == sCommand )
			//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgfdiag;
			//else if( "tsbgbdiag" == sCommand )
			//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgbdiag;
			//else if( "tsbgcross" == sCommand )
			//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgcross;
			//else if( "tsbgdcross" == sCommand )
			//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgdcross;
			//else if( "tsbgdkhor" == sCommand )
			//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgdkhoriz;
			//else if( "tsbgdkvert" == sCommand )
			//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgdkvert;
			//else if( "tsbgdkfdiag" == sCommand )
			//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgdkfdiag;
			//else if( "tsbgdkbdiag" == sCommand )
			//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgdkbdiag;
			//else if( "tsbgdkcross" == sCommand )
			//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgdkcross;
			//else if( "tsbgdkdcross" == sCommand )
			//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgdkdcross;
			//else if( "tscellcfpat" == sCommand )
			//{
			//	if( true == hasParameter )
			//		m_oTableStyleProperty.m_oShading.m_nForeColor = parameter;
			//}
			//else if( "tscellcbpat" == sCommand )
			//{
			//	if( true == hasParameter )
			//		m_oTableStyleProperty.m_oShading.m_nBackColor = parameter;
			//}
			//else if( "tscellpct" == sCommand )
			//{
			//	if( true == hasParameter )
			//		m_oTableStyleProperty.m_oShading.m_nValue = parameter;
			//}
			//else if( "tsbrdrt" == sCommand )
			//	m_eInternalState = is_tsbrdrt;
			//else if( "tsbrdrb" == sCommand )
			//	m_eInternalState = is_tsbrdrb;
			//else if( "tsbrdrl" == sCommand )
			//	m_eInternalState = is_tsbrdrl;
			//else if( "tsbrdrr" == sCommand )
			//	m_eInternalState = is_tsbrdrr;
			//else if( "tsbrdrh" == sCommand )
			//	m_eInternalState = is_tsbrdrh;
			//else if( "tsbrdrv" == sCommand )
			//	m_eInternalState = is_tsbrdrv;
			//else if( "tscbandsh" == sCommand )
			//{
			//	if( true == hasParameter )
			//		m_oTableStyleProperty.m_nRowBandNumber = parameter;
			//}
			//else if( "tscbandsv" == sCommand )
			//{
			//	if( true == hasParameter )
			//		m_oTableStyleProperty.m_nCellBandNumber = parameter;
			//}
			else
	{
		bool bResult = false;
		//if( is_tsbrdrt == m_eInternalState )
		//	bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, m_oTableStyleProperty.m_oCellTopBorder );
		//else if( is_tsbrdrb == m_eInternalState )
		//	bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, m_oTableStyleProperty.m_oCellBottomBorder );
		//else if( is_tsbrdrl == m_eInternalState )
		//	bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, m_oTableStyleProperty.m_oCellLeftBorder );
		//else if( is_tsbrdrr == m_eInternalState )
		//	bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, m_oTableStyleProperty.m_oCellRightBorder );
		//else if( is_tsbrdrh == m_eInternalState )
		//	bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, m_oTableStyleProperty.m_oCellHorBorder );
		//else if( is_tsbrdrv == m_eInternalState )
		//	bResult = RtfBorderCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, m_oTableStyleProperty.m_oCellVerBorder );

		//if( true == bResult )
		//	return true;
		bResult = m_oParDest.ExecuteCommand( oDocument, oReader,(*this), sCommand, hasParameter, parameter );
		if( true == bResult )
			return true;
		bResult = RtfCharPropsCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, &oReader.m_oState->m_oCharProp );
		if( true == bResult )
			return true;

		return false;
	}
	return true;

}
void RtfStyleTableReader::RtfStyleReader::ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText)
{
	while (true)
	{
		size_t pos = sText.find(';');
		if (std::wstring::npos == pos) break;

		sText.erase(pos, 1);
	}

	m_oCurStyle->m_sName += sText;
}
void RtfStyleTableReader::RtfStyleReader::ExitReader(RtfDocument& oDocument, RtfReader& oReader)
{
	switch( m_oCurStyle->m_eType )
	{
	case RtfStyle::stCharacter:
	{
		RtfCharStylePtr m_oCurCharStyle = boost::static_pointer_cast< RtfCharStyle, RtfStyle >( m_oCurStyle );
		m_oCurCharStyle->m_oCharProp = oReader.m_oState->m_oCharProp;
		oDocument.m_oStyleTable.AddItem( m_oCurStyle );
		break;
	}
	case RtfStyle::stParagraph:
	{
		RtfParagraphStylePtr m_oCurParStyle = boost::static_pointer_cast< RtfParagraphStyle, RtfStyle >( m_oCurStyle );
		m_oCurParStyle->m_oCharProp = oReader.m_oState->m_oCharProp;
		m_oCurParStyle->m_oParProp = oReader.m_oState->m_oParagraphProp;
		oDocument.m_oStyleTable.AddItem( m_oCurStyle );
		break;
	}
	case RtfStyle::stTable:
	{
		RtfTableStylePtr m_oCurTableStyle = boost::static_pointer_cast< RtfTableStyle, RtfStyle >( m_oCurStyle );
		m_oCurTableStyle->m_oCharProp = oReader.m_oState->m_oCharProp;
		m_oCurTableStyle->m_oParProp = oReader.m_oState->m_oParagraphProp;

		if( PROP_DEF == m_oCurTableStyle->m_oParProp.m_nSpaceBetween )
			m_oCurTableStyle->m_oParProp.m_nSpaceBetween = 240;//интервал - единичный

		m_oCurTableStyle->m_oTableProp = oReader.m_oState->m_oRowProperty;
		m_oCurTableStyle->m_oRowProp = oReader.m_oState->m_oRowProperty;
		//m_oCurTableStyle->m_oTableStyleProperty = m_oTableStyleProperty;
		//надо определить базовый это стиль или например firstRow
		RtfStylePtr oStyle;
		if( true == oDocument.m_oStyleTable.GetStyle(m_oCurTableStyle->m_nID, oStyle) )
		{
			if( oStyle->m_eType == RtfStyle::stTable )
			{//определяем какой это conditionalFormating
				RtfTableStylePtr oStyleTable = boost::static_pointer_cast<RtfTableStyle, RtfStyle>(oStyle);
				if( 1 == m_oCurTableStyle->m_oParProp.m_bStyleFirstRow )
					oStyleTable->m_oFirstRow = m_oCurTableStyle;
				else if( 1 == m_oCurTableStyle->m_oParProp.m_bStyleLastRow )
					oStyleTable->m_oLastRow = m_oCurTableStyle;
				else if( 1 == m_oCurTableStyle->m_oParProp.m_bStyleFirstCollumn )
					oStyleTable->m_oFirstCol = m_oCurTableStyle;
				else if( 1 == m_oCurTableStyle->m_oParProp.m_bStyleLastCollumn )
					oStyleTable->m_oLastCol = m_oCurTableStyle;
				else if( 1 == m_oCurTableStyle->m_oParProp.m_bStyleOddRowBand )
					oStyleTable->m_oBandHorOdd = m_oCurTableStyle;
				else if( 1 == m_oCurTableStyle->m_oParProp.m_bStyleEvenRowBand )
					oStyleTable->m_oBandHorEven = m_oCurTableStyle;
				else if( 1 == m_oCurTableStyle->m_oParProp.m_bStyleOddColBand )
					oStyleTable->m_oBandVerOdd = m_oCurTableStyle;
				else if( 1 == m_oCurTableStyle->m_oParProp.m_bStyleEvenColBand )
					oStyleTable->m_oBandVerEven = m_oCurTableStyle;
				else if( 1 == m_oCurTableStyle->m_oParProp.m_bStyleNWCell )
					oStyleTable->m_oNWCell = m_oCurTableStyle;
				else if( 1 == m_oCurTableStyle->m_oParProp.m_bStyleNECell )
					oStyleTable->m_oNECell = m_oCurTableStyle;
				else if( 1 == m_oCurTableStyle->m_oParProp.m_bStyleSWCell )
					oStyleTable->m_oSWCell = m_oCurTableStyle;
				else if( 1 == m_oCurTableStyle->m_oParProp.m_bStyleSECell )
					oStyleTable->m_oSECell = m_oCurTableStyle;
			}
		}
		else
			oDocument.m_oStyleTable.AddItem( m_oCurStyle );
		break;
	}
	}
}

bool RtfStyleTableReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
{
	if( "stylesheet" == sCommand )
		return true;
	else
	{
		RtfStyleReader oStyleReader;
		return StartSubReader( oStyleReader, oDocument, oReader );
	}
}

RtfListTableReader::ListReader::ListLevelReader::ListLevelReader(RtfListLevelProperty & oListLevelProp):m_oListLevelProp(oListLevelProp)
{
}
bool RtfListTableReader::ListReader::ListLevelReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
{
	if( "listlevel" == sCommand )
		;
	COMMAND_RTF_INT( "levelnfc", m_oListLevelProp.m_nNumberType, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( "levelnfcn", m_oListLevelProp.m_nNumberType, sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "lvltentative", m_oListLevelProp.m_bTentative, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( "leveljc", m_oListLevelProp.m_nJustification, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( "leveljcn", m_oListLevelProp.m_nJustification, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( "levelfollow", m_oListLevelProp.m_nFollow, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( "levelstartat", m_oListLevelProp.m_nStart, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( "levelnorestart", m_oListLevelProp.m_nNoRestart, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( "levellegal", m_oListLevelProp.m_nLegal, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( "levelpicture", m_oListLevelProp.m_nPictureIndex, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( "levelspace", m_oListLevelProp.m_nSpace, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( "levelindent", m_oListLevelProp.m_nIndent, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( "li", m_oListLevelProp.m_nIndent, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( "lin", m_oListLevelProp.m_nIndentStart, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( "fi", m_oListLevelProp.m_nFirstIndent, sCommand, hasParameter, parameter )
			else if( "tx" == sCommand  )
	{
		if( true == hasParameter )
		{
			RtfTab oNewTab;
			oNewTab.m_nTab = parameter;
			m_oListLevelProp.m_oTabs.m_aTabs.push_back( oNewTab );
		}
	}
	else if( "leveltext" == sCommand  )
	{
		TextReader oLevelTextReader( m_oListLevelProp.m_sText );
		oLevelTextReader.m_bUseGlobalCodepage = true;
		return StartSubReader( oLevelTextReader, oDocument, oReader );
	}
	else if( "levelnumbers" == sCommand  )
	{
		TextReader oLevelNumberReader( m_oListLevelProp.m_sNumber );
		return StartSubReader( oLevelNumberReader, oDocument, oReader );
	}
	else
	{
		bool res = RtfCharPropsCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, &m_oListLevelProp.m_oCharProp);
		oReader.m_nDefFont = PROP_DEF;
	}
	return true;
}
void RtfListTableReader::ListReader::ListLevelReader::ExitReader( RtfDocument& oDocument, RtfReader& oReader )
{
	m_oListLevelProp.m_oCharProp = oReader.m_oState->m_oCharProp;
	//убираем shading и border (word тоже так делает)
	m_oListLevelProp.m_oCharProp.m_poBorder.SetDefaultRtf();
	m_oListLevelProp.m_oCharProp.m_poShading.SetDefaultRtf();

}

RtfListTableReader::ListReader::ListReader(RtfListProperty& oListProp):m_oListProp(oListProp)
{
}
bool RtfListTableReader::ListReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
{
	if( "list" == sCommand )
		;
	COMMAND_RTF_INT( "listid", m_oListProp.m_nID , sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( "listtemplateid", m_oListProp.m_nTemplateId , sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "listsimple", m_oListProp.m_nListSimple , sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( "listhybrid", m_oListProp.m_bListHybrid , sCommand, hasParameter, parameter )
			else if( "listname" == sCommand )
	{
		TextReader oListNameReader( m_oListProp.m_sName );
		return StartSubReader( oListNameReader, oDocument, oReader );
	}
	else if( "listlevel" == sCommand  )
	{
		RtfListLevelProperty oListLevelProp;
		ListLevelReader oListLevelReader( oListLevelProp );

		oReader.m_oState->m_oCharProp.m_bListLevel = true;
		oReader.m_oState->m_oCharProp.SetDefaultRtf();

		bool bResult = StartSubReader( oListLevelReader, oDocument, oReader );

		oReader.m_oState->m_oCharProp.m_bListLevel = false;
		if( true == bResult )
		{
			oListLevelProp.m_nLevel = m_oListProp.GetCount();
			m_oListProp.AddItem( oListLevelProp );
			return true;
		}
	}
	else
	{
		return false;
	}
	return true;
}

bool RtfListTableReader::ListPictureReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
{
	if( "listpicture" == sCommand )
	{
	}
	else if( "shppict" == sCommand )
	{
		RtfShapePtr oNewPicture = RtfShapePtr ( new RtfShape() );

		RtfShppictReader oShppictReader( *oNewPicture );
		StartSubReader( oShppictReader, oDocument, oReader );

		oDocument.m_oListTable.m_aPictureList.AddItem( oNewPicture );
	}
	else
		return false;
	return true;
}

RtfListTableReader::RtfListTableReader::RtfListTableReader()
{
}
bool RtfListTableReader::RtfListTableReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
{
	if( "listtable" == sCommand )
		return true;
	else if( "list" == sCommand )
	{
		RtfListProperty oListProp;

		ListReader oListReader(oListProp);

		oReader.m_oState->m_oCharProp.m_bListLevel = true;

		bool bResult = StartSubReader( oListReader, oDocument, oReader );

		oReader.m_oState->m_oCharProp.m_bListLevel = false;

		if( true == bResult )
		{
			oDocument.m_oListTable.AddItem( oListProp );
			return true;
		}
		else
			return false;
	}
	else if( "listpicture" == sCommand )
	{
		ListPictureReader oListPictureReader;
		StartSubReader( oListPictureReader, oDocument, oReader );
	}
	else
		return false;
	return true;

}

RtfListOverrideTableReader::ListOverrideReader::lfolevelReader::lfolevelReader( RtfListOverrideProperty::ListOverrideLevels::ListOverrideLevel& oOverrideLevel ):m_oOverrideLevel(oOverrideLevel)
{
}
bool RtfListOverrideTableReader::ListOverrideReader::lfolevelReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
{
	if( "lfolevel" == sCommand)
		return true;
	COMMAND_RTF_INT( "listoverrideformat", m_oOverrideLevel.m_nLevelIndex, sCommand, hasParameter, parameter )
			else if( "listoverridestartat" == sCommand )
	{
		m_oOverrideLevel.m_nStart = 0; //индикатор
		if( true == hasParameter )
			m_oOverrideLevel.m_nStart = parameter;
	}
	else if( "levelstartat" == sCommand && m_oOverrideLevel.m_nStart == 0 && hasParameter)
	{
		m_oOverrideLevel.m_nStart = parameter;
	}
	else if( "listlevel" == sCommand )
	{
		m_oOverrideLevel.m_oLevel.m_nLevel = m_oOverrideLevel.m_nLevelIndex;
		RtfListTableReader::ListReader::ListLevelReader oListLevelReader( m_oOverrideLevel.m_oLevel );
		StartSubReader( oListLevelReader, oDocument, oReader );
	}
	else if( "ls" == sCommand )
	{
		if( true == hasParameter )
			m_oOverrideLevel.m_nLevelIndex = parameter;
	}
	else
		return false;
	return true;
}

RtfListOverrideTableReader::ListOverrideReader::ListOverrideReader(RtfListOverrideProperty& oProperty):m_oProperty(oProperty)
{
}
bool RtfListOverrideTableReader::ListOverrideReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
{
	if( "listoverride" == sCommand)
		return true;
	else if( "listid" == sCommand )
	{
		if( true == hasParameter )
			m_oProperty.m_nListID = parameter;
	}
	else if( "ls" == sCommand )
	{
		if( true == hasParameter )
			m_oProperty.m_nIndex = parameter;
	}
	else if( "lfolevel" == sCommand )
	{
		RtfListOverrideProperty::ListOverrideLevels::ListOverrideLevel oOverrideLevel;

		lfolevelReader olfolevelReader( oOverrideLevel );
		StartSubReader( olfolevelReader, oDocument, oReader );

		m_oProperty.m_oOverrideLevels.m_aOverrideLevels.push_back( oOverrideLevel );
	}
	else
		return false;
	return true;
}

RtfListOverrideTableReader::RtfListOverrideTableReader()
{
}
bool RtfListOverrideTableReader::ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
{
	if( "listoverridetable" == sCommand )
		return true;
	else if( "listoverride" == sCommand)
	{
		RtfListOverrideProperty oProperty;
		ListOverrideReader oListOverrideReader( oProperty );
		bool bResult = StartSubReader( oListOverrideReader, oDocument, oReader );
		if( true == bResult )
		{
			oDocument.m_oListOverrideTable.AddItem(oProperty);
			return true;
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	return true;
}
