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
#pragma once 

#include "RtfReader.h"
#include "RtfDocument.h"
#include "RtfChar.h"
#include "RtfField.h"
#include "RtfShape.h"
#include "RtfMath.h"
#include "RtfTable.h"
#include "RtfBookmark.h"
#include "RtfOle.h"

#include "Writer/OOXWriter.h"
#include "Writer/OOXRelsWriter.h"

#include "../../../ASCOfficePPTXFile/Editor/Drawing/Enums.h"

#define COMMAND_RTF_BOOL( pattern, target, command, hasParameter, parameter )\
		else if( pattern == command )\
		{\
			if( true == hasParameter && 0 == parameter)\
				target = 0;\
			else\
				target = 1;\
		}
#define COMMAND_RTF_INT( pattern, target, command, hasParameter, parameter )\
		else if( pattern == command )\
		{\
			if( true == hasParameter )\
				target = parameter;\
		}
#define COMMAND_RTF_SPECIAL_CHAR( pattern, target, command, hasParameter, parameter )\
		else if( pattern == command  )\
		{\
			RtfCharSpecialPtr oNewChar = RtfCharSpecialPtr( new RtfCharSpecial() );\
			oNewChar->m_eType = parameter;\
			oNewChar->m_oProperty = oReader.m_oState->m_oCharProp;\
			target->AddItem( oNewChar );\
		}
//Command не имеет состояний
#include "math.h"
class RtfBorderCommand
{
public:static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter,RtfBorder& oOutput)
		{
			if( _T("brdrs") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrs;
			else if( _T("brdrth") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrth;
			else if( _T("brdrsh") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrsh;
			else if( _T("brdrdb") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrdb;
			else if( _T("brdrdot") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrdot;
			else if( _T("brdrdash") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrdash;
			else if( _T("brdrhair") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrhair;
			else if( _T("brdrdashsm") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrdashsm;
			else if( _T("brdrdashd") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrdashd;
			else if( _T("brdrdashdd") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrdashdd;
			else if( _T("brdrdashdot") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrdashdot;
			else if( _T("brdrdashdotdot") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrdashdot;
			else if( _T("brdrtriple") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrtriple;
			else if( _T("brdrtnthsg") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrtnthsg;
			else if( _T("brdrthtnsg") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrthtnsg;
			else if( _T("brdrtnthtnsg") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrtnthtnsg;
			else if( _T("brdrtnthmg") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrtnthmg;
			else if( _T("brdrthtnmg") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrthtnmg;
			else if( _T("brdrtnthtnmg") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrtnthtnmg;
			else if( _T("brdrtnthlg") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrtnthlg;
			else if( _T("brdrthtnlg") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrthtnlg;
			else if( _T("brdrtnthtnlg") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrtnthtnlg;
			else if( _T("brdrwavy") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrwavy;
			else if( _T("brdrwavydb") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrwavydb;
			else if( _T("brdrdashdotstr") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrdashdotstr;
			else if( _T("brdremboss") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdremboss;
			else if( _T("brdrengrave") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrengrave;
			else if( _T("brdroutset") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdroutset;
			else if( _T("brdrinset") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrinset;

			else if( _T("brdrnone") == sCommand )
				oOutput.m_eType = RtfBorder::bt_brdrnone;
			else if( _T("brdrw") == sCommand )
			{
				if( true == hasParameter )
				{
					oOutput.m_nWidth = parameter;
					if( RtfBorder::bt_none == oOutput.m_eType )
						oOutput.m_eType = RtfBorder::bt_brdrs;
				}
			}
			else if( _T("brsp") == sCommand )
			{
				if( true == hasParameter )
				{
					oOutput.m_nSpace = parameter;
					if( RtfBorder::bt_none == oOutput.m_eType )
						oOutput.m_eType = RtfBorder::bt_brdrs;
				}
			}
			else if( _T("brdrcf") == sCommand )
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
};
class RtfShadingCommand
{
public:
	static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, CString sCommand, bool hasParameter, int parameter, RtfShading& oOutput );
};
class RtfShadingCellCommand
{
public: 
	static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter, RtfShading& oOutput);
};

class RtfShadingCharCommand
{
public: 
	static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter, RtfShading& oOutput );
};

class RtfShadingRowCommand
{
public: 
	static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter, RtfShading& oOutput);
};

class RtfFrameCommand
{
public:
	static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter, RtfFrame& oOutput )
	{
		return true;
	}
};
//class RtfOldList;
class TextReader: public RtfAbstractReader
{
private: 
	CString& m_sName;
	bool m_bErease;
public:
	TextReader( CString& sName, bool bErease = true ):m_sName(sName),m_bErease(bErease)
	{
	}
	void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, CString sText)
	{
		if( true == m_bErease )
			sText.Replace(_T(";"),_T(""));
		m_sName += sText;
	}
};
class RtfCharPropCommand
{
//bool bLookOnBorder; Надо ли читать свойства border( актуалЃEЃEдля ParagraphReader )
public:
	static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter, RtfCharProperty & ChartProperty, bool bLookOnBorder = true);
};
//Reader выражения в скобках
class RtfOldListReader : public RtfAbstractReader
{
private: 
	RtfOldList& m_oTarget;
public: 
	RtfOldListReader( RtfOldList& oTarget) : m_oTarget(oTarget)
	{
	}
	bool ExecuteCommand( RtfDocument& oDocument, RtfReader& oReader, CString sKey, bool bHasPar, int nPar );
	void ExitReader( RtfDocument& oDocument, RtfReader& oReader )
	{
		m_oTarget.m_oLevelText->m_oProperty.m_oCharProperty = oReader.m_oState->m_oCharProp;
	}
};
//class RtfColorSchemeReader: public RtfAbstractReader
//{
//private: CString sSchemeXml;
//public: void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, CString sText)
//		{
//			sSchemeXml += sText;
//		}
//		void ExitReader(RtfDocument& oDocument, RtfReader& oReader)
//		{
//			CString sXml = RtfUtility::EncodeHex(sSchemeXml);
//			XmlUtils::CXmlLiteReader oXmlReader;
//			oXmlReader.OpenFromXmlString(sXml);
//			oXmlReader.ReadRootNode(_T("a:clrMap"));
//			CString sValue;
//			sValue = oXmlReader.ReadNodeAttribute(_T("bg1"),_T(""));
//			if( _T("") != sValue )
//				oDocument.m_oColorSchemeMapping.bg1 = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(_T("tx1"),_T(""));
//			if( _T("") != sValue )
//				oDocument.m_oColorSchemeMapping.t1 = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(_T("bg2"),_T(""));
//			if( _T("") != sValue )
//				oDocument.m_oColorSchemeMapping.bg2 = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(_T("tx2"),_T(""));
//			if( _T("") != sValue )
//				oDocument.m_oColorSchemeMapping.t2 = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(_T("accent1"),_T(""));
//			if( _T("") != sValue )
//				oDocument.m_oColorSchemeMapping.accent1 = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(_T("accent2"),_T(""));
//			if( _T("") != sValue )
//				oDocument.m_oColorSchemeMapping.accent3 = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(_T("accent3"),_T(""));
//			if( _T("") != sValue )
//				oDocument.m_oColorSchemeMapping.accent4 = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(_T("accent4"),_T(""));
//			if( _T("") != sValue )
//				oDocument.m_oColorSchemeMapping.accent5 = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(_T("accent5"),_T(""));
//			if( _T("") != sValue )
//				oDocument.m_oColorSchemeMapping.accent6 = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(_T("accent6"),_T(""));
//			if( _T("") != sValue )
//				oDocument.m_oColorSchemeMapping.hyperlink = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(_T("hlink"),_T(""));
//			if( _T("") != sValue )
//				oDocument.m_oColorSchemeMapping.hyperlink = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(_T("folHlink"),_T(""));
//			if( _T("") != sValue )
//				oDocument.m_oColorSchemeMapping.followedHyperlink = sValue;
//		}
//};
//class RtfThemeDataReader: public RtfAbstractReader
//{
//public: void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, CString sText )
//		{
//			oDocument.m_sThemeData += sText;
//		}
//};
class RtfFontTableReader: public RtfAbstractReader
{
private: 
    typedef enum { is_normal, is_panose, is_altname } InternalState;

    RtfFont         m_oFont;
    InternalState   m_eInternalState;
public: 
	RtfFontTableReader()
	{
        m_bUseGlobalCodepage    = true;

		m_eInternalState = is_normal;
		m_oFont.SetDefaultOOX();
	}

    bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
    {
        if( _T("fonttbl") == sCommand )
            ;
        else if( _T("flomajor") == sCommand )
            m_oFont.m_eFontTheme = RtfFont::ft_flomajor;
        else if( _T("fhimajor") == sCommand )
            m_oFont.m_eFontTheme = RtfFont::ft_fhimajor;
        else if( _T("fdbmajor") == sCommand )
            m_oFont.m_eFontTheme = RtfFont::ft_fdbmajor;
        else if( _T("fbimajor") == sCommand )
            m_oFont.m_eFontTheme = RtfFont::ft_fbimajor;
        else if( _T("flominor") == sCommand )
            m_oFont.m_eFontTheme = RtfFont::ft_flominor;
        else if( _T("fhiminor") == sCommand )
            m_oFont.m_eFontTheme = RtfFont::ft_fhiminor;
        else if( _T("fdbminor") == sCommand )
            m_oFont.m_eFontTheme = RtfFont::ft_fdbminor;
        else if( _T("fbiminor") == sCommand )
            m_oFont.m_eFontTheme = RtfFont::ft_fbiminor;

        else if( _T("fnil") == sCommand )
            m_oFont.m_eFontFamily = RtfFont::ff_fnil;
        else if( _T("froman") == sCommand )
            m_oFont.m_eFontFamily = RtfFont::ff_froman;
        else if( _T("fswiss") == sCommand )
            m_oFont.m_eFontFamily = RtfFont::ff_fswiss;
        else if( _T("fmodern") == sCommand )
            m_oFont.m_eFontFamily = RtfFont::ff_fmodern;
        else if( _T("fscript") == sCommand )
            m_oFont.m_eFontFamily = RtfFont::ff_fscript;
        else if( _T("fdecor") == sCommand )
            m_oFont.m_eFontFamily = RtfFont::ff_fdecor;
        else if( _T("ftech") == sCommand )
            m_oFont.m_eFontFamily = RtfFont::ff_ftech;
        else if( _T("fbidi") == sCommand )
            m_oFont.m_eFontFamily = RtfFont::ff_fbidi;

        else if( _T("panose") == sCommand )
            m_eInternalState = is_panose;
        else if( _T("falt") == sCommand )
            m_eInternalState = is_altname;

        else if( _T("f") == sCommand )
        {
            if( true == hasParameter )
                m_oFont.m_nID = parameter;
        }
        else if( _T("fcharset") == sCommand )
        {
            if( true == hasParameter )
            {
                m_oFont.m_nCharset = parameter;
            }
        }
        else if( _T("cpg") == sCommand )
        {
            if( true == hasParameter )
            {
                m_oFont.m_nCodePage = parameter;
            }
        }
        else if( _T("fprq") == sCommand )
        {
            if( true == hasParameter )
                m_oFont.m_nPitch = parameter;
        }
        else
        {
            return false;
            //ATLASSERT( false );
        }
        return true;
    }
    void ExecuteTextInternal2( RtfDocument& oDocument, RtfReader& oReader, std::string & sKey, int& nSkipChars)
	{
		if(oReader.m_oState->m_sCurText.length() < 1 )return;

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

	CString RemoveLastUnchar(CString str)
	{
		int i = 1;
		while(true)
		{
			if (i > str.GetLength())
				break;
			if (str.GetAt(str.GetLength() - i) <= 0x20)
				str.Delete(str.GetLength() - i , 1);
			else
				break;
		}
		return str;
	}
    void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, CString sText)
    {
        if( is_panose == m_eInternalState )
            m_oFont.m_sPanose += sText;
        else if( is_altname == m_eInternalState )
            m_oFont.m_sAltName += sText;
        else if( is_normal == m_eInternalState && sText.GetLength() > 0)
        {
            if( sText.Find(';') != -1 )
            {
                sText.Remove(';');
				if( sText.Find('&') !=0  )//todooo выясниснить что значит &;
				{
					//sText.Remove('&'); // 
					m_oFont.m_sName += RemoveLastUnchar(sText);
				}
				
			//todooo при добавлении могут быть повторы - убрать нннадо - goldwingSetting.rtf
				oDocument.m_oFontTable.DirectAddItem( m_oFont );
	
                m_oFont.SetDefaultRtf();
            }
            else
            {
                m_oFont.m_sName += sText.Trim();
            }
        }
    }
    void PopState(RtfDocument& oDocument, RtfReader& oReader)
    {
        RtfAbstractReader::PopState( oDocument, oReader );

        if( is_panose == m_eInternalState )
            m_eInternalState = is_normal;
        if( is_altname == m_eInternalState )
            m_eInternalState = is_normal;
    }
};


class RtfColorTableReader : public RtfAbstractReader
{
private: 
	RtfColor oCurColor;
	bool m_bIsSet;

public: 
		RtfColorTableReader()
		{
			oCurColor.SetDefaultRtf();
			m_bIsSet = false;
		}
		bool ExecuteCommand( RtfDocument& oDocument, RtfReader& oReader, CString sKey, bool bHasPar, int nPar )
		 {
			if( _T("colortbl") == sKey )
				return true;
			else if( _T("cmaindarkone") == sKey )
				oCurColor.m_eTheme = RtfColor::cmaindarkone;
			else if( _T("cmainlightone") == sKey )
				oCurColor.m_eTheme = RtfColor::cmainlightone;
			else if( _T("cmaindarktwo") == sKey )
				oCurColor.m_eTheme = RtfColor::cmaindarktwo;
			else if( _T("cmainlighttwo") == sKey )
				oCurColor.m_eTheme = RtfColor::cmainlighttwo;
			else if( _T("caccentone") == sKey )
				oCurColor.m_eTheme = RtfColor::caccentone;
			else if( _T("caccenttwo") == sKey )
				oCurColor.m_eTheme = RtfColor::caccenttwo;
			else if( _T("caccentthree") == sKey )
				oCurColor.m_eTheme = RtfColor::caccentthree;
			else if( _T("caccentfour") == sKey )
				oCurColor.m_eTheme = RtfColor::caccentfour;
			else if( _T("caccentfive") == sKey )
				oCurColor.m_eTheme = RtfColor::caccentfive;
			else if( _T("caccentsix") == sKey )
				oCurColor.m_eTheme = RtfColor::caccentsix;
			else if( _T("chyperlink") == sKey )
				oCurColor.m_eTheme = RtfColor::chyperlink;
			else if( _T("cfollowedhyperlink") == sKey )
				oCurColor.m_eTheme = RtfColor::cfollowedhyperlink;
			else if( _T("cbackgroundone") == sKey )
				oCurColor.m_eTheme = RtfColor::cbackgroundone;
			else if( _T("ctextone") == sKey )
				oCurColor.m_eTheme = RtfColor::ctextone;
			else if( _T("cbackgroundtwo") == sKey )
				oCurColor.m_eTheme = RtfColor::cbackgroundtwo;
			else if( _T("ctexttwo") == sKey )
				oCurColor.m_eTheme = RtfColor::ctexttwo;
			else if( _T("ctint") == sKey && true == bHasPar )
				oCurColor.m_byteTint = nPar;
			else if( _T("cshade") == sKey && true == bHasPar )
				oCurColor.m_byteShade = nPar;
			else if( _T("red") == sKey && true == bHasPar )
				oCurColor.m_byteRed = nPar;
			else if( _T("green") == sKey && true == bHasPar )
				oCurColor.m_byteGreen = nPar;
			else if( _T("blue") == sKey && true == bHasPar )
				oCurColor.m_byteBlue = nPar;
			else
			{
				return false;
				//ATLASSERT( false );
			}	
			m_bIsSet = true;
			return true;
		 }
		void ExecuteText( RtfDocument& oDocument, RtfReader& oReader, CString oText )
		{
		//Romanization_Armenian.rtf
		//{\colortbl\red0\blue159\green82;\red0\blue0\green0;\red255\blue255\green255;\red0\blue156\green90;\red169\blue86\green0;}
		//{\colortbl;\red0\green0\blue0;\red0\green0\blue255;\red0\green255\blue255;\red0\green255\blue0;\red255\green0\blue255;
			if( oText.Find(';') != -1)
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
};
class RtfDefCharPropReader: public RtfAbstractReader
{
public: RtfCharProperty& m_oCharProp;
		RtfDefCharPropReader( RtfCharProperty& oOutput ):m_oCharProp(oOutput)
		{
		}
		bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
		{
			if( _T("defchp") == sCommand )
				return true;
			else 
				return RtfCharPropCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, oReader.m_oState->m_oCharProp );
		}
		void ExitReader( RtfDocument& oDocument, RtfReader& oReader )
		{
			m_oCharProp = oReader.m_oState->m_oCharProp;
		}
};

class RtfInfoReader: public RtfAbstractReader
{
private: typedef enum { is_normal, is_title, is_subject, is_author, is_manager, is_company, is_operator, is_category, is_keywords,
						is_comment, is_doccomm, is_hlinkbase, is_creatim, is_revtim, is_printim, is_buptim } InternalState;
		InternalState m_eInternalState;
public: RtfInfoReader()
		{
			m_eInternalState = is_normal;
		}

		bool ExecuteCommand( RtfDocument& oDocument, RtfReader& oReader, CString sCommand, bool hasParameter, int parameter)
		{
			if( _T("info") == sCommand )
				return true;
			else if( _T("title") == sCommand )
				m_eInternalState = is_title;
			else if( _T("subject") == sCommand )
				m_eInternalState = is_subject;
			else if( _T("author") == sCommand )
				m_eInternalState = is_author;
			else if( _T("manager") == sCommand )
				m_eInternalState = is_manager;
			else if( _T("company") == sCommand )
				m_eInternalState = is_company;
			else if( _T("operator") == sCommand )
				m_eInternalState = is_operator;
			else if( _T("category") == sCommand )
				m_eInternalState = is_category;
			else if( _T("keywords") == sCommand )
				m_eInternalState = is_keywords;
			else if( _T("comment") == sCommand )
				m_eInternalState = is_comment;
			else if( _T("doccomm") == sCommand )
				m_eInternalState = is_doccomm;
			else if( _T("hlinkbase") == sCommand )
				m_eInternalState = is_hlinkbase;
			else if( _T("version") == sCommand )
			{
				if( true == hasParameter )
					oDocument.m_oInformation.m_nVersion = parameter;
			}
			else if( _T("vern") == sCommand )
			{
				if( true == hasParameter )
					oDocument.m_oInformation.m_nInternalVersion = parameter;
			}
			else if( _T("edmins") == sCommand )
			{
				if( true == hasParameter )
					oDocument.m_oInformation.m_nEndingTime = parameter;
			}
			else if( _T("nofpages") == sCommand )
			{
				if( true == hasParameter )
					oDocument.m_oInformation.m_nNumberOfPages= parameter;
			}
			else if( _T("nofwords") == sCommand )
			{
				if( true == hasParameter )
					oDocument.m_oInformation.m_nNumberOfWords= parameter;
			}
			else if( _T("nofchars") == sCommand )
			{
				if( true == hasParameter )
					oDocument.m_oInformation.m_nNumberOfCharactersWithSpace= parameter;
			}
			else if( _T("nofcharsws") == sCommand )
			{
				if( true == hasParameter )
					oDocument.m_oInformation.m_nNumberOfCharactersWithoutSpace= parameter;
			}
			else if( _T("id") == sCommand )
			{
				if( true == hasParameter )
					oDocument.m_oInformation.m_nInternalId= parameter;
			}
			else if( _T("creatim") == sCommand )
				m_eInternalState = is_creatim;
			else if( _T("revtim") == sCommand )
				m_eInternalState = is_revtim;
			else if( _T("printim") == sCommand )
				m_eInternalState = is_printim;
			else if( _T("buptim") == sCommand )
				m_eInternalState = is_buptim;
			else if(  _T("yr") == sCommand  )
			{
				if( true == hasParameter )
				{
					if( is_creatim == m_eInternalState )
						oDocument.m_oInformation.m_oCreateTime.m_nYear = parameter;
					else if( is_revtim == m_eInternalState )
						oDocument.m_oInformation.m_oRevTime.m_nYear = parameter;
					else if( is_printim == m_eInternalState )
						oDocument.m_oInformation.m_oPrintTime.m_nYear = parameter;
					else if( is_buptim == m_eInternalState )
						oDocument.m_oInformation.m_oBackupTime.m_nYear = parameter;
				}
			}
			else if(  _T("mo") == sCommand  )
			{
				if( true == hasParameter )
				{
					if( is_creatim == m_eInternalState )
						oDocument.m_oInformation.m_oCreateTime.m_nMonth = parameter;
					else if( is_revtim == m_eInternalState )
						oDocument.m_oInformation.m_oRevTime.m_nMonth = parameter;
					else if( is_printim == m_eInternalState )
						oDocument.m_oInformation.m_oPrintTime.m_nMonth = parameter;
					else if( is_buptim == m_eInternalState )
						oDocument.m_oInformation.m_oBackupTime.m_nMonth = parameter;
				}
			}
			else if(  _T("dy") == sCommand  )
			{
				if( true == hasParameter )
				{
					if( is_creatim == m_eInternalState )
						oDocument.m_oInformation.m_oCreateTime.m_nDay = parameter;
					else if( is_revtim == m_eInternalState )
						oDocument.m_oInformation.m_oRevTime.m_nDay = parameter;
					else if( is_printim == m_eInternalState )
						oDocument.m_oInformation.m_oPrintTime.m_nDay = parameter;
					else if( is_buptim == m_eInternalState )
						oDocument.m_oInformation.m_oBackupTime.m_nDay = parameter;
				}
			}
			else if(  _T("hr") == sCommand  )
			{
				if( true == hasParameter )
				{
					if( is_creatim == m_eInternalState )
						oDocument.m_oInformation.m_oCreateTime.m_nHour = parameter;
					else if( is_revtim == m_eInternalState )
						oDocument.m_oInformation.m_oRevTime.m_nHour = parameter;
					else if( is_printim == m_eInternalState )
						oDocument.m_oInformation.m_oPrintTime.m_nHour = parameter;
					else if( is_buptim == m_eInternalState )
						oDocument.m_oInformation.m_oBackupTime.m_nHour = parameter;
				}
			}
			else if(  _T("min") == sCommand  )
			{
				if( true == hasParameter )
				{
					if( is_creatim == m_eInternalState )
						oDocument.m_oInformation.m_oCreateTime.m_nMin = parameter;
					else if( is_revtim == m_eInternalState )
						oDocument.m_oInformation.m_oRevTime.m_nMin = parameter;
					else if( is_printim == m_eInternalState )
						oDocument.m_oInformation.m_oPrintTime.m_nMin = parameter;
					else if( is_buptim == m_eInternalState )
						oDocument.m_oInformation.m_oBackupTime.m_nMin = parameter;
				}
			}
			else if(  _T("sec") == sCommand  )
			{
				if( true == hasParameter )
				{
					if( is_creatim == m_eInternalState )
						oDocument.m_oInformation.m_oCreateTime.m_nSecond = parameter;
					else if( is_revtim == m_eInternalState )
						oDocument.m_oInformation.m_oRevTime.m_nSecond = parameter;
					else if( is_printim == m_eInternalState )
						oDocument.m_oInformation.m_oPrintTime.m_nSecond = parameter;
					else if( is_buptim == m_eInternalState )
						oDocument.m_oInformation.m_oBackupTime.m_nSecond = parameter;
				}
			}
			else
			{
				return false;
			}
			return true;

		}
		void ExecuteText( RtfDocument& oDocument, RtfReader& oReader, CString sText )
		{
			if( is_title == m_eInternalState )
				oDocument.m_oInformation.m_sTitle += sText;
			else if( is_subject == m_eInternalState )
				oDocument.m_oInformation.m_sSubject += sText;
			else if( is_author == m_eInternalState )
				oDocument.m_oInformation.m_sAuthor += sText;
			else if( is_manager == m_eInternalState )
				oDocument.m_oInformation.m_sManager += sText;
			else if( is_company == m_eInternalState )
				oDocument.m_oInformation.m_sCompany += sText;
			else if( is_operator == m_eInternalState )
				oDocument.m_oInformation.m_sOperator += sText;
			else if( is_category == m_eInternalState )
				oDocument.m_oInformation.m_sCategory += sText;
			else if( is_keywords == m_eInternalState )
				oDocument.m_oInformation.m_sKeywords += sText;
			else if( is_comment == m_eInternalState )
				oDocument.m_oInformation.m_sComment += sText;
			else if( is_doccomm == m_eInternalState )
				oDocument.m_oInformation.m_sDocCom += sText;
			else if( is_hlinkbase == m_eInternalState )
				oDocument.m_oInformation.m_sLinkBase += sText;
		}
};
class PictureReader:  public RtfAbstractReader
{
	class PLACEABLEMETAHEADER //заголовок для wmf из rtf ( в rtf wmf не содержит размеров картинки )
	{
	public:
		DWORD Key;           /* Magic number (always 9AC6CDD7h) */
		WORD  Handle;        /* Metafile HANDLE number (always 0) */
		SHORT Left;          /* Left coordinate in metafile units */
		SHORT Top;           /* Top coordinate in metafile units */
		SHORT Right;         /* Right coordinate in metafile units */
		SHORT Bottom;        /* Bottom coordinate in metafile units */
		WORD  Inch;          /* Number of metafile units per inch */
		DWORD Reserved;      /* Reserved (always 0) */
		WORD  Checksum;      /* Checksum value for previous 10 WORDs */
	public:
		PLACEABLEMETAHEADER()
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
		void CalculateChecksum()
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
		CString ToString()
		{
			CString sResult;
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
			CString ByteToString( BYTE* pbData, int nSize, bool bLittleEnd = true )
			 {
				CString sResult;
				if( true == bLittleEnd )
				{
					for( int i = 0; i < nSize; i++ )
					{
						BYTE byteVal = pbData[i];
						if( byteVal < 0x10 )
							sResult.AppendFormat( _T("0%x"), byteVal );
						else
							sResult.AppendFormat( _T("%x"), byteVal );
					}
				}
				else
				{
					for( int i = nSize - 1 ; i >= 0; i-- )
					{
						BYTE byteVal = pbData[i];
						if( byteVal < 0x10 )
							sResult.AppendFormat( _T("0%x"), byteVal );
						else
							sResult.AppendFormat( _T("%x"), byteVal );
					}
				}
				return sResult;
			 }
	};

private: 
	RtfShape& m_oShape;
	CString m_sFile;
	CString m_sData;
	bool  m_bBin;
    BYTE* m_pbBin;
	int m_nBinLength;
public: 
	PictureReader( RtfReader& oReader, RtfShape& oShape ):m_oShape(oShape)
	{
		m_bBin = false;
		m_pbBin = NULL;

		//m_oFileWriter = NULL;

		//m_sFile = Utils::CreateTempFile( oReader.m_sTempFolder );
		//try{
		//	RELEASEOBJECT( m_oFileWriter );
		//	BSTR bstrFilename = m_sFile.AllocSysString();
		//	m_oFileWriter = new NFileWriter::CBufferedFileWriter( bstrFilename );
		//	SysFreeString( bstrFilename );
		//}
		//catch(...)
		//	m_oFileWriter = NULL;
	}
	~PictureReader()
	{
		RELEASEARRAYOBJECTS(m_pbBin);
		//RELEASEOBJECT( m_oFileWriter );
	}
	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter);
	void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, CString sText)
	{
		m_sData += sText;
	}
	void ExitReader( RtfDocument& oDocument, RtfReader& oReader )
	{
		//if( NULL != m_oFileWriter )
		//{
		//	//есЃE ыD задаЃEтиЃEзадаем егЃEсаЃE
		//	if( RtfPicture::dt_none ==  m_oShape.m_oPicture->eDataType )
		//		RtfPicture::DataType eDataType = RtfPicture::GetPictureType( m_sFile );
		//	m_oShape.m_oPicture->m_bIsCopy = true;
		//	m_oShape.m_oPicture->m_sPicFilename = m_sFile;
		//}
		if( NULL != m_oShape.m_oPicture && RtfPicture::dt_wmf == m_oShape.m_oPicture->eDataType )
		{
			if( PROP_DEF != m_oShape.m_oPicture->m_nWidthGoal && PROP_DEF != (int)m_oShape.m_oPicture->m_dScaleX &&
				PROP_DEF != m_oShape.m_oPicture->m_nHeightGoal && PROP_DEF != (int)m_oShape.m_oPicture->m_dScaleY )
			{
				PLACEABLEMETAHEADER oPLACEABLEMETAHEADER;
				oPLACEABLEMETAHEADER.Right = long( (m_oShape.m_oPicture->m_dScaleX / 100.0) * m_oShape.m_oPicture->m_nWidthGoal * ( 96.0 / 1440 ) ); //to pixel
				oPLACEABLEMETAHEADER.Bottom = long( (m_oShape.m_oPicture->m_dScaleY / 100.0) * m_oShape.m_oPicture->m_nHeightGoal * ( 96.0 / 1440 ) );
				oPLACEABLEMETAHEADER.CalculateChecksum();
				m_sData.Insert( 0, oPLACEABLEMETAHEADER.ToString() );
			}
		}

		CString sTempFile = Utils::CreateTempFile( oReader.m_sTempFolder );
		if(m_bBin)
			RtfUtility::WriteDataToFileBinary( sTempFile, m_pbBin, m_nBinLength );
		else
			RtfUtility::WriteDataToFile( sTempFile, m_sData );
		
		if( RtfPicture::dt_none ==  m_oShape.m_oPicture->eDataType )
			m_oShape.m_oPicture->eDataType = RtfPicture::GetPictureType( sTempFile );

		m_oShape.m_oPicture->m_bIsCopy = true;
		m_oShape.m_oPicture->m_sPicFilename = sTempFile;
	}
};

class OleReader:  public RtfAbstractReader
{
private: 
	RtfOle& m_oOle;
public: 
	OleReader(RtfOle& oOle) : m_oOle(oOle)
	{
	}
	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter);
};

class ShapeReader : public RtfAbstractReader
{
public:
	class ShapePropertyReader : public RtfAbstractReader
	{
	public:
		class ShapePropertyValueReader : public RtfAbstractReader
		{
			CString& m_sPropName;
			RtfShape& m_oShape;
		public: 
			CString m_sPropValue;
			ShapePropertyValueReader(CString& sPropName, RtfShape& oShape):m_sPropName(sPropName),m_oShape(oShape)
			{
			}
			bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
			{
				if( _T("sv") == sCommand )
					return true;
				else if( _T("pict") == sCommand && ( _T("pib") == m_sPropName  || _T("fillBlip") == m_sPropName))
				{
					m_oShape.m_oPicture = RtfPicturePtr( new RtfPicture() );
					PictureReader oPictureReader( oReader, m_oShape );
					StartSubReader( oPictureReader, oDocument, oReader );
				}
				return true;
			}
			void PopState( RtfDocument& oDocument, RtfReader& oReader );
			void ExecuteText( RtfDocument& oDocument, RtfReader& oReader, CString oText )
			{
				CString sValue;
				sValue = oText;
				m_sPropValue += sValue;
			}
		};
		private:
			CString sPropName;
			RtfShape& m_oShape;
		public: 
			ShapePropertyReader(RtfShape& oShape):m_oShape(oShape)
			{
			}
			bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
			{
				if( _T("sp") == sCommand )
					return true;
				else if( _T("sn") == sCommand )
				{
					TextReader oTextReader(sPropName);
					StartSubReader( oTextReader, oDocument, oReader );
				}
				else if( _T("sv") == sCommand )
				{
					ShapePropertyValueReader oShPropValReader(sPropName, m_oShape);
					StartSubReader( oShPropValReader, oDocument, oReader );
				}
				else
					return false;
				return true;
			}
	};
private: 
	RtfShape& m_oShape;
public: 
	ShapeReader( RtfShape& oShape ):m_oShape(oShape)
	{
	}
	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter);
	void ExitReader( RtfDocument& oDocument, RtfReader& oReader )
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
};

class ShapeGroupReader : public ShapeReader
{
public: 
	RtfShapeGroup& m_oShapeGroup;
	bool m_bHeader; //чтобы отличать заголовок от вложенных групп
	ShapeGroupReader( RtfShapeGroup& oShape ):ShapeReader(oShape),m_oShapeGroup(oShape)
	{
		m_bHeader = true;
	}
	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
	{
		if( _T("shpgrp") == sCommand )
		{
			if( true == m_bHeader )
				m_bHeader = false;
			else
			{
				RtfShapeGroupPtr oNewShape = RtfShapeGroupPtr( new RtfShapeGroup() );
				ShapeGroupReader oShapeGroupReader( *oNewShape );
				StartSubReader( oShapeGroupReader, oDocument, oReader );
				m_oShapeGroup.AddItem( oNewShape );
			}
		}
		else if( _T("shp") == sCommand )
		{
			RtfShapePtr oNewShape = RtfShapePtr( new RtfShape() );
			ShapeReader oShapeReader( *oNewShape );
			StartSubReader( oShapeReader, oDocument, oReader );
			m_oShapeGroup.AddItem( oNewShape );
		}
		else
			return ShapeReader::ExecuteCommand( oDocument,  oReader, sCommand, hasParameter, parameter);
		return true;
	}
};
class OldShapeReader : public RtfAbstractReader
{
private: 
	RtfShape& m_oShape;
public: 
	OldShapeReader( RtfShape& oShape ):m_oShape(oShape)
	{
	}
	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter);
	void ExitReader( RtfDocument& oDocument, RtfReader& oReader )
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
};

class ShppictReader : public RtfAbstractReader
{
private: 
	RtfShape& m_oShape;
public: 
	ShppictReader( RtfShape& oShape ):m_oShape(oShape)
	{
	}
	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
	{
		if( _T("shppict") == sCommand )
			return true;
		else if( _T("pict") == sCommand )
		{
			m_oShape.m_eAnchorTypeShape		= RtfShape::st_inline;
			m_oShape.m_nShapeType			= NSOfficeDrawing::sptPictureFrame;
			m_oShape.m_nWrapType			= 3; // none
			m_oShape.m_nPositionHRelative	= 3;//TCHAR
			m_oShape.m_nPositionVRelative	= 3;//line
			m_oShape.m_nPositionH			= 0;//absolute
			m_oShape.m_nPositionV			= 0;//absolute

			m_oShape.m_oPicture = RtfPicturePtr( new RtfPicture() );
			PictureReader oPictureReader( oReader, m_oShape);
			StartSubReader( oPictureReader, oDocument, oReader );
		}
		else
			return false;
		return true;
	}
};

class AllPictReader : public RtfAbstractReader
{
private: 
	RtfShape& m_oShape;
public: 
	AllPictReader( RtfShape& oShape ):m_oShape(oShape)
	{
	}
	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
	{
		if( _T("shp") == sCommand )
		{
			ShapeReader oShapeReader(m_oShape);
			StartSubReader( oShapeReader, oDocument, oReader );
		}
		if( _T("shppict") == sCommand )
		{
			ShppictReader oShppictReader(m_oShape);
			StartSubReader( oShppictReader, oDocument, oReader );
		}
		else if( _T("pict") == sCommand )
		{
			m_oShape.m_eAnchorTypeShape		= RtfShape::st_inline;
			m_oShape.m_nShapeType			= NSOfficeDrawing::sptPictureFrame;
			m_oShape.m_nWrapType			= 3; // none
			m_oShape.m_nPositionHRelative	= 3;//TCHAR
			m_oShape.m_nPositionVRelative	= 3;//line
			m_oShape.m_nPositionH			= 0;//absolute
			m_oShape.m_nPositionV			= 0;//absolute
			m_oShape.m_oPicture				= RtfPicturePtr( new RtfPicture() );

			PictureReader oPictureReader( oReader, m_oShape);
			StartSubReader( oPictureReader, oDocument, oReader );
		}
		else
			return false;
		return true;
	}
};

class RtfMathReader: public RtfAbstractReader
{
private: 
	RtfCharProperty m_oCharProp;
public: 
	RtfMath& m_oMath;
	RtfParagraphProperty::ParagraphAlign m_eParAlign;
	RtfMathReader(RtfMath& oMath): m_oMath(oMath)
	{
		m_eParAlign = RtfParagraphProperty::pa_none;
		m_oCharProp.SetDefaultRtf();
	}

	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
	{
		if( _T("mmathPict") == sCommand  )
			;
		else if( _T("shppict") == sCommand )
		{
			m_oMath.m_oPicture = RtfShapePtr( new RtfShape() );
			ShppictReader oShppictReader( *m_oMath.m_oPicture );
			StartSubReader( oShppictReader, oDocument, oReader );
		}
		else if( _T("nonshppict") == sCommand )
			Skip( oDocument, oReader );
		else 
		{
			bool isBoolMath = m_oMath.IsRtfControlPropertyBool(sCommand);
			bool isValMath	= isBoolMath ? false : m_oMath.IsRtfControlProperty(sCommand);
			bool isMath		= (isValMath || isBoolMath) ? false : m_oMath.IsRtfControlWord(sCommand);
			
			if( isMath || isValMath || isBoolMath)
			{
				if( true == m_oMath.m_bHeader )
				{
					m_oMath.m_bHeader = false;
					if (m_oMath.IsEmpty())
						m_oMath.SetRtfName( sCommand );
					
				}
				else
				{
					RtfMathPtr oNewMath( new RtfMath() );
					oNewMath->SetRtfName( sCommand );
					
					oNewMath->m_bIsVal	= isValMath;
					oNewMath->m_bIsBool	= isBoolMath;

					RtfMathReader oSubMathReader( *oNewMath );
					bool resParseSub = StartSubReader( oSubMathReader, oDocument, oReader );

					if (resParseSub && _T("mctrlPr") == sCommand)
					{
						RtfCharPropertyPtr oNewCharProp( new RtfCharProperty() );
						oNewCharProp->Merge(oSubMathReader.m_oCharProp);
						oNewMath->AddItem( oNewCharProp );
					}

					if( oNewMath->IsValid() == true )
					{
						if ((oNewMath->m_bIsVal || oNewMath->m_bIsBool) && hasParameter)
						{
							RtfCharPtr oChar = RtfCharPtr(new RtfChar);
							CString s = ExecuteMathProp(oDocument, sCommand, parameter);
							oChar->setText( s);
							oNewMath->m_oVal.AddItem( oChar );
						}

						m_oMath.AddItem( oNewMath );
					}
				}
			}
			else if( RtfCharPropCommand::ExecuteCommand(oDocument, oReader, sCommand, hasParameter, parameter, m_oCharProp ) == true )
			{
			}
			else
				return false;

			return true;
		}

		return true;
	}
	void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, CString sText)
	{
		RtfCharPtr oNewChar( new RtfChar() );
		oNewChar->m_oProperty.Merge(m_oCharProp);

		oNewChar->setText( sText ); 
		m_oMath.AddItem( oNewChar );
	}
	CString ExecuteMathProp(RtfDocument& oDocument, CString sCommand, int parameter)
	{//rtf math properties (int) to oox math properties (string)

		CString strProp;

		if( _T("mbrkBin") == sCommand )
		{
			switch( parameter )
			{
				case 0:	strProp = L"before";	break;
				case 1:	strProp = L"after";		break;
				case 2:	strProp = L"repeat";	break;
			}
		}
		else if( _T("mbrkBinSub") == sCommand )
		{
			switch( parameter )
			{
				case 0:	strProp = L"--";	break;
				case 1:	strProp = L"+-";	break;
				case 2:	strProp = L"-+";	break;
			}
		}
		else if( _T("mdefJc") == sCommand )
		{
			switch( parameter )
			{
				case 0:	strProp = L"centerGroup";	break;
				case 1:	strProp = L"center";		break;
				case 2:	strProp = L"left";			break;
				case 3:	strProp = L"right";			break;
			}
		}
		else if( _T("mnaryLim") == sCommand || _T("mintLim") == sCommand  || _T("mLim") == sCommand)
		{
			switch( parameter )
			{
				case 0:	strProp = L"subSup";	break;
				case 1:	strProp = L"undOvr";	break;
			}
		}
		else if ( _T("mmathFont") == sCommand )
		{
			if (oDocument.m_oProperty.m_nDeffMathFont == PROP_DEF)
				oDocument.m_oProperty.m_nDeffMathFont = parameter;
			
			RtfFont oFont;
			if( true == oDocument.m_oFontTable.GetFont(parameter, oFont) )
				strProp = oFont.m_sName;
		}
		else
		{
			strProp.Format(L"%d", parameter);
		}

		return strProp;
	}
	void ExitReader2( RtfDocument& oDocument, RtfReader& oReader )
	{
		if( RtfParagraphProperty::pa_none != m_eParAlign )
			oReader.m_oState->m_oParagraphProp.m_eAlign = m_eParAlign;
	}
};
class FieldReader: public RtfAbstractReader
{
private: 
	typedef enum { is_normal, is_insert, is_datafield, is_formfield, is_result } InternalState;
	InternalState m_eInternalState;
	RtfField& m_oField;
public: 
	FieldReader( RtfField& oField ):m_oField(oField)
	{
		m_eInternalState = is_normal;
	}
	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter);
	void ExitReader( RtfDocument& oDocument, RtfReader& oReader )
	{
		//если результат пустой пытаемся его сгенерировать
		if( true == IsEmptyText( oDocument ) )
			TryToPepairResult( oDocument, oReader );
	}
	bool IsEmptyText( RtfDocument& oDocument )
	{
		if( NULL != m_oField.m_oResult )
		{
			OOXWriter oTempWriter( oDocument, _T("") );
			OOXRelsWriter oTempRelsWriter( _T(""), oDocument );
			RenderParameter oRenderParameter;
			oRenderParameter.poDocument = &oDocument;
			oRenderParameter.poRels = &oTempRelsWriter;
			oRenderParameter.poWriter = &oTempWriter;
			oRenderParameter.nType = RENDER_TO_OOX_PARAM_PLAIN;

			CString sResult = m_oField.m_oResult->RenderToOOX( oRenderParameter );
			if( _T("") != sResult )
				return false;
		}
		return true;
	}
private: 
	void TryToPepairResult( RtfDocument& oDocument, RtfReader& oReader )
	 {
		OOXWriter oTempWriter( oDocument, _T("") );
		OOXRelsWriter oTempRelsWriter( _T(""), oDocument );

		RenderParameter oNewParametr;
		oNewParametr.poDocument = &oDocument;
		oNewParametr.poRels = &oTempRelsWriter;
		oNewParametr.poWriter = &oTempWriter;
		oNewParametr.nType = RENDER_TO_OOX_PARAM_PLAIN;

		CString sCharCode;
		CString sCharFont;

		CString sField = m_oField.m_oInsert->RenderToOOX(oNewParametr);
		int nStartTokenize = 0;
		CString sResTokenize = sField.Tokenize( _T(" "), nStartTokenize );
		if( _T("SYMBOL") != sResTokenize )
			return;
		sResTokenize = sField.Tokenize( _T(" \""), nStartTokenize );
		int nCommand = 0; //0 - none; 1 - \f; 3 - other

        bool bWaitRightBreak = false;
        while( sResTokenize != _T("") )
		{
			int nTokenLen = sResTokenize.GetLength();
			if( nTokenLen > 0 && sResTokenize[0] == '\"' && sResTokenize[nTokenLen - 1] != '\"' ) //текст в кавычках считается как один
			{
				//ищем следующую кавычку
				int nNextQuot = sField.Find( '\"', nStartTokenize );
				if( -1 != nNextQuot )
				{
					sResTokenize = sField.Mid( nStartTokenize - nTokenLen, nNextQuot - nStartTokenize + nTokenLen );
					nStartTokenize = nNextQuot + 1;
				}
			}
			if( nTokenLen > 0 && sResTokenize[0] == '\"' && sResTokenize[nTokenLen - 1] == '\"' )
				sResTokenize = sResTokenize.Mid( 1, nTokenLen - 2 );
			

			if( -1 != sResTokenize.Find( _T("\\f") ) )
				nCommand = 1;
			else if( -1 != sResTokenize.Find( _T("\\") ) )
				nCommand = 3;
			else if( 1 == nCommand )
			{
				sCharFont = sResTokenize;
				nCommand = 0;
			}
			else if( 0 == nCommand && _T("") == sCharCode )
			{
				sCharCode = sResTokenize;
			}
			else
				nCommand = 0;
			sResTokenize = sField.Tokenize( _T(" "), nStartTokenize );
		}
		if( _T("") == sCharCode || _T("") == sCharFont )
			return;
		
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
		CString sResultSymbol = reader.ExecuteTextInternal( oDocument, oReader, sCharA, false, 0, nSkipChar );
		m_oField.m_oResult = TextItemContainerPtr( new TextItemContainer() );
		RtfParagraphPtr oNewPar	= RtfParagraphPtr( new RtfParagraph() );
		RtfCharPtr oNewChar = RtfCharPtr( new RtfChar() );
		oNewChar->setText( sResultSymbol );
		oNewChar->m_oProperty = oReader.m_oState->m_oCharProp;

		oNewPar->AddItem( oNewChar );
		m_oField.m_oResult->AddItem( oNewPar );
		m_oField.m_bTextOnly = true;
	 }
};

class BookmarkStartReader: public RtfAbstractReader
{
public: 
	RtfBookmarkStart& m_oBookmarkStart;
	BookmarkStartReader( RtfBookmarkStart& oBookmark ):m_oBookmarkStart(oBookmark)
	{
	}
	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
	{
		if( _T("bkmkstart") == sCommand )
			;
		else if( _T("bkmkcolf") == sCommand )
		{
			if( true == hasParameter)
				m_oBookmarkStart.nFirstColumn = parameter;
		}
		else if( _T("bkmkcoll") == sCommand )
		{
			if( true == hasParameter)
				m_oBookmarkStart.nLastColumn = parameter;
		}
		else
			return false;
		return true;
	}
	void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, CString sText)
	{
		m_oBookmarkStart.m_sName += sText ;
	}
};

class BookmarkEndReader: public RtfAbstractReader
{
public: 
	RtfBookmarkEnd& m_oBookmarkEnd;
	BookmarkEndReader( RtfBookmarkEnd& oBookmark ):m_oBookmarkEnd(oBookmark)
	{
	}
	void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, CString sText)
	{
		m_oBookmarkEnd.m_sName += sText;
	}
};

class FootnoteReader;
//Destination имеет состояния
class ParagraphPropDestination // todo - последний параграф не обрабатывается
{
public:
	typedef enum{ is_normal,is_charBorder, is_borderTop, is_borderLeft, is_borderBottom, is_borderRight, is_borderBox, is_borderBar,
		is_borderCellLeft,is_borderCellTop,is_borderCellRight,is_borderCellBottom,is_borderCellLR,is_borderCellRL,
		is_borderRowLeft,is_borderRowTop,is_borderRowRight,is_borderRowBottom,is_borderRowVer,is_borderRowHor} InternalState;
//только для определения бордера
	
private: 
	InternalState m_eInternalState;
//		ItemContainer< RtfReaderParagraphPtr > m_aArray;
	RtfParagraphPtr m_oCurParagraph;

//реальные параграфы и таблицы
	std::vector< ITextItemPtr >		aCellRenderables;
	std::vector< int >				aItaps; //вложенность параграфов
	std::vector< RtfTableCellPtr >	aCells;
	std::vector< int >				aCellItaps; //вложенность cell
	std::vector< RtfTableRowPtr >	aRows;
	std::vector< int >				aRowItaps; //вложенность row
	RtfRowProperty oCurRowProperty;

	RtfReader* m_oReader;
	bool m_bPar;// если последняя команда была par, то не надо добавлять параграф
public: 
	TextItemContainerPtr m_oTextItems;	//для разбивки на TextItem
	int nTargetItap;		//уровень который считается не таблицей ( для того чтобы читать параграфы в таблицах )
	int nCurItap;
	RtfTab m_oCurTab;

	ParagraphPropDestination( )
	{
		nTargetItap			= PROP_DEF;
		m_bPar				= false;
		m_oTextItems		= TextItemContainerPtr( new TextItemContainer() );
		nCurItap			= 0;//main document
		m_eInternalState	= is_normal;
		m_oCurParagraph		= RtfParagraphPtr(new RtfParagraph());
	}
	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,RtfAbstractReader& oAbstrReader,CString sCommand, bool hasParameter, int parameter);
	void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, CString sText)
	{
		m_bPar = false;
		RtfCharPtr oNewChar( new RtfChar() );
		oNewChar->m_oProperty = oReader.m_oState->m_oCharProp;
		oNewChar->setText( sText );
		m_oCurParagraph->AddItem( oNewChar );
	}
	void AddItem( RtfParagraphPtr oItem, RtfReader& oReader, bool bEndCell, bool bEndRow );
	
	void Finalize( RtfReader& oReader/*, RtfSectionPtr pSection*/);

	void ExecuteNumberChar( RtfDocument& oDocument, RtfReader& oReader, RtfAbstractReader& oAbstrReader, int nWinChar, int nMacChar )
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
			CString sText = reader.ExecuteTextInternal( oDocument, oReader, sBullet, false, 0, nSkip );
			ExecuteText( oDocument, oReader, sText );
		 }
	 }
};
class FootnoteReader: public RtfAbstractReader
{
private: 
	ParagraphPropDestination m_oParPropDest;
public: 
	RtfFootnote& m_oRtfFootnote;
	FootnoteReader( RtfFootnote& oRtfFootnote ):m_oRtfFootnote(oRtfFootnote)
	{
	}
	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
	{
		if( _T("footnote") == sCommand )
		{
			oReader.m_nFootnote = 1;
		}
		else if( _T("ftnalt") == sCommand )
		{
			m_oRtfFootnote.m_bEndNote = true;
			oReader.m_nFootnote = 2;
		}
		else
			return m_oParPropDest.ExecuteCommand( oDocument, oReader, (*this), sCommand, hasParameter, parameter );
		return true;
	}
	void ExecuteText( RtfDocument& oDocument, RtfReader& oReader, CString sText )
	{
		m_oParPropDest.ExecuteText( oDocument, oReader, sText );
	}
	void ExitReader( RtfDocument& oDocument, RtfReader& oReader )
	{
		m_oParPropDest.Finalize( oReader );

		m_oRtfFootnote.m_oContent	= m_oParPropDest.m_oTextItems;
		oReader.m_nFootnote			= PROP_DEF;
	}
};
class RtfDefParPropReader: public RtfAbstractReader
{
private: ParagraphPropDestination m_oParPropDest;
public: RtfDefParPropReader( )
		{
		}

		bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
		{
			if( _T("defpap") == sCommand )
				return true;
			else
				return m_oParPropDest.ExecuteCommand( oDocument, oReader, (*this), sCommand, hasParameter, parameter);
		}
		void ExitReader(RtfDocument& oDocument, RtfReader& oReader)
		{
			oDocument.m_oDefaultParagraphProp = oReader.m_oState->m_oParagraphProp;
		}
};

//class LockedExeceptReader: public RtfAbstractReader
//{
//private: 
//	RtfStyleException m_oCurException;
//public:
//	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
//	{
//		if( _T("lsdlockedexcept") == sCommand )
//			return true;
//		else if( _T("lsdpriority") == sCommand )
//		{
//			if( true == hasParameter )
//				m_oCurException.m_nPriority = parameter;
//		}
//		else if( _T("lsdunhideused") == sCommand )
//		{
//			if( true == hasParameter )
//				m_oCurException.m_nHiddenWhenUse = parameter;
//		}
//		else if( _T("lsdqformat") == sCommand )
//		{
//			if( true == hasParameter )
//				m_oCurException.m_nQFormat = parameter;
//		}
//		else if( _T("lsdlocked") == sCommand )
//		{
//			if( true == hasParameter )
//				m_oCurException.m_nLocked = parameter;
//		}
//		else if( _T("lsdsemihidden") == sCommand )
//		{
//			if( true == hasParameter )
//				m_oCurException.m_nSemiHidden = parameter;
//		}
//		else
//		{
//			return false;
//		}
//		return true;
//	}
//	void ExecuteText( RtfDocument& oDocument, RtfReader& oReader, CString sText )
//	{
//		if( sText.Find(';') != -1 )
//		{
//			sText.Remove(';');
//			m_oCurException.m_sName += sText;
//			oDocument.m_oLatentStyleTable.AddItem( m_oCurException );
//			m_oCurException.SetDefaultRtf();
//		}
//		else
//		{
//			m_oCurException.m_sName += sText;
//		}
//	}
//};
//
//class LatentstyleTableReader: public RtfAbstractReader
//{
//public: bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
//		{
//			if( _T("latentstyles") == sCommand )
//				return true;
//			else if( _T("lsdstimax") == sCommand )
//			{
//				if( true == hasParameter )
//					oDocument.m_oLatentStyleTable.m_nCount = parameter;
//			}
//			else if( _T("lsdlockeddef") == sCommand )
//			{
//				if( true == hasParameter )
//					oDocument.m_oLatentStyleTable.m_nLocked = parameter;
//			}
//			else if( _T("lsdsemihiddendef") == sCommand )
//			{
//				if( true == hasParameter )
//					oDocument.m_oLatentStyleTable.m_nSemiHidden = parameter;
//			}
//			else if( _T("lsdunhideuseddef") == sCommand )
//			{
//				if( true == hasParameter )
//					oDocument.m_oLatentStyleTable.m_nUnHiddenWhenUse = parameter;
//			}
//			else if( _T("lsdqformatdef") == sCommand )
//			{
//				if( true == hasParameter )
//					oDocument.m_oLatentStyleTable.m_nQFormat = parameter;
//			}
//			else if( _T("lsdprioritydef") == sCommand )
//			{
//				if( true == hasParameter )
//					oDocument.m_oLatentStyleTable.m_nPriority = parameter;
//			}
//			else if( _T("lsdlockedexcept") == sCommand )
//			{
//				LockedExeceptReader oExceptionReader;
//				StartSubReader( oExceptionReader, oDocument, oReader );
//			}
//			else
//			{
//				return false;
//			}
//			return true;
//
//		}
//};
//

class StyleTableReader: public RtfAbstractReader
{
	class RtfStyleReader: public RtfAbstractReader
	{
	//только для определения бордеров
	private: 
		typedef enum { is_normal, is_tsbrdrt, is_tsbrdrb, is_tsbrdrl, is_tsbrdrr, is_tsbrdrh, is_tsbrdrv } InternalState;
		InternalState m_eInternalState;
		ParagraphPropDestination m_oParDest;
		RtfStylePtr m_oCurStyle;

//		RtfTableStyleProperty m_oTableStyleProperty;
	public: 
		RtfStyleReader()
		{
			m_oCurStyle = RtfParagraphStylePtr( new RtfParagraphStyle() );
			m_eInternalState = is_normal;
		}
			bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
			{
				if( _T("s") == sCommand )
				{
					if( true == hasParameter )
					{
						m_oCurStyle = RtfParagraphStylePtr( new RtfParagraphStyle() );
						m_oCurStyle->m_nID = parameter;
					}
				}
				else if( _T("cs") == sCommand )
				{
					if( true == hasParameter )
					{
						m_oCurStyle = RtfCharStylePtr( new RtfCharStyle() );
						m_oCurStyle->m_nID = parameter;
					}
				}
				else if( _T("ts") == sCommand )
				{
					if( true == hasParameter )
					{
						m_oCurStyle = RtfTableStylePtr( new RtfTableStyle() );
						m_oCurStyle->m_nID = parameter;
					}
				}
				COMMAND_RTF_INT( _T("sbasedon"), m_oCurStyle->m_nBasedOn, sCommand, hasParameter, parameter )
				COMMAND_RTF_INT( _T("snext"), m_oCurStyle->m_nNext, sCommand, hasParameter, parameter )
				COMMAND_RTF_INT( _T("slink"), m_oCurStyle->m_nLink, sCommand, hasParameter, parameter )
				COMMAND_RTF_BOOL( _T("sqformat"), m_oCurStyle->m_bQFormat, sCommand, hasParameter, parameter )
				COMMAND_RTF_INT( _T("spriority"), m_oCurStyle->m_nPriority, sCommand, hasParameter, parameter )
				COMMAND_RTF_BOOL( _T("sunhideused"), m_oCurStyle->m_bUnhiddenWhenUse, sCommand, hasParameter, parameter )
				COMMAND_RTF_BOOL( _T("slocked"), m_oCurStyle->m_bLocked, sCommand, hasParameter, parameter )
				COMMAND_RTF_BOOL( _T("shidden"), m_oCurStyle->m_bHidden, sCommand, hasParameter, parameter )
				else if( _T("ssemihidden") == sCommand )
				{
					if( true == hasParameter && 0 == parameter)
						m_oCurStyle->m_nSemiHidden = 0;
					else
						m_oCurStyle->m_nSemiHidden = 1;
				}
				COMMAND_RTF_BOOL( _T("spersonal"), m_oCurStyle->m_bPersonal, sCommand, hasParameter, parameter )
				COMMAND_RTF_BOOL( _T("scompose"), m_oCurStyle->m_bCompose, sCommand, hasParameter, parameter )
				COMMAND_RTF_BOOL( _T("sreply"), m_oCurStyle->m_bReply, sCommand, hasParameter, parameter )

				//tableStyleCommands
				//else if( _T("tscellpaddt") == sCommand )
				//{
				//	if( true == hasParameter )
				//		m_oTableStyleProperty.m_nPaddingTop = parameter;
				//}
				//else if( _T("tscellpaddl") == sCommand )
				//{
				//	if( true == hasParameter )
				//		m_oTableStyleProperty.m_nPaddingLeft = parameter;
				//}
				//else if( _T("tscellpaddr") == sCommand )
				//{
				//	if( true == hasParameter )
				//		m_oTableStyleProperty.m_nPaddingRight = parameter;
				//}
				//else if( _T("tscellpaddb") == sCommand )
				//{
				//	if( true == hasParameter )
				//		m_oTableStyleProperty.m_nPaddingBottom = parameter;
				//}
				//else if( _T("tscellpaddft") == sCommand )
				//{
				//	if( true == hasParameter )
				//		m_oTableStyleProperty.m_nIsPaddingTop = parameter;
				//}
				//else if( _T("tscellpaddfl") == sCommand )
				//{
				//	if( true == hasParameter )
				//		m_oTableStyleProperty.m_nIsPaddingLeft = parameter;
				//}
				//else if( _T("tscellpaddfr") == sCommand )
				//{
				//	if( true == hasParameter )
				//		m_oTableStyleProperty.m_nIsPaddingRight = parameter;
				//}
				//else if( _T("tscellpaddfb") == sCommand )
				//{
				//	if( true == hasParameter )
				//		m_oTableStyleProperty.m_nIsPaddingBottom = parameter;
				//}
				//else if( _T("tsvertalt") == sCommand )
				//	m_oTableStyleProperty.m_eAlign = RtfTableStyleProperty::av_tsvertalt;
				//else if( _T("tsvertalc") == sCommand )
				//	m_oTableStyleProperty.m_eAlign = RtfTableStyleProperty::av_tsvertalc;
				//else if( _T("tsvertalb") == sCommand )
				//	m_oTableStyleProperty.m_eAlign = RtfTableStyleProperty::av_tsvertalb;
				//else if( _T("tsnowrap") == sCommand )
				//	m_oTableStyleProperty.m_bNoCellWrap = 1;
				//else if( _T("tscbandsh") == sCommand )
				//{
				//	if( true == hasParameter )
				//		m_oTableStyleProperty.m_nRowBandNumber = parameter;
				//}
				//else if( _T("tscbandsv") == sCommand )
				//{
				//	if( true == hasParameter )
				//		m_oTableStyleProperty.m_nCellBandNumber = parameter;
				//}
				//else if(_T("tsbghoriz") == sCommand )
				//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbghoriz;
				//else if( _T("tsbgvert") == sCommand )
				//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgvert;
				//else if( _T("tsbgfdiag") == sCommand )
				//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgfdiag;
				//else if( _T("tsbgbdiag") == sCommand )
				//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgbdiag;
				//else if( _T("tsbgcross") == sCommand )
				//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgcross;
				//else if( _T("tsbgdcross") == sCommand )
				//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgdcross;
				//else if( _T("tsbgdkhor") == sCommand )
				//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgdkhoriz;
				//else if( _T("tsbgdkvert") == sCommand )
				//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgdkvert;
				//else if( _T("tsbgdkfdiag") == sCommand )
				//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgdkfdiag;
				//else if( _T("tsbgdkbdiag") == sCommand )
				//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgdkbdiag;
				//else if( _T("tsbgdkcross") == sCommand )
				//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgdkcross;
				//else if( _T("tsbgdkdcross") == sCommand )
				//	m_oTableStyleProperty.m_oShading.m_eType = RtfShading::st_chbgdkdcross;
				//else if( _T("tscellcfpat") == sCommand )
				//{
				//	if( true == hasParameter )
				//		m_oTableStyleProperty.m_oShading.m_nForeColor = parameter;
				//}
				//else if( _T("tscellcbpat") == sCommand )
				//{
				//	if( true == hasParameter )
				//		m_oTableStyleProperty.m_oShading.m_nBackColor = parameter;
				//}
				//else if( _T("tscellpct") == sCommand )
				//{
				//	if( true == hasParameter )
				//		m_oTableStyleProperty.m_oShading.m_nValue = parameter;
				//}
				//else if( _T("tsbrdrt") == sCommand )
				//	m_eInternalState = is_tsbrdrt;
				//else if( _T("tsbrdrb") == sCommand )
				//	m_eInternalState = is_tsbrdrb;
				//else if( _T("tsbrdrl") == sCommand )
				//	m_eInternalState = is_tsbrdrl;
				//else if( _T("tsbrdrr") == sCommand )
				//	m_eInternalState = is_tsbrdrr;
				//else if( _T("tsbrdrh") == sCommand )
				//	m_eInternalState = is_tsbrdrh;
				//else if( _T("tsbrdrv") == sCommand )
				//	m_eInternalState = is_tsbrdrv;
				//else if( _T("tscbandsh") == sCommand )
				//{
				//	if( true == hasParameter )
				//		m_oTableStyleProperty.m_nRowBandNumber = parameter;
				//}
				//else if( _T("tscbandsv") == sCommand )
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
					bResult = m_oParDest.ExecuteCommand( oDocument, oReader,(*this),sCommand, hasParameter, parameter );
					if( true == bResult )
						return true;
					bResult = RtfCharPropCommand::ExecuteCommand( oDocument, oReader,sCommand, hasParameter, parameter, oReader.m_oState->m_oCharProp );
					if( true == bResult )
						return true;

					return false;
				}
				return true;

			}

			void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, CString sText)
			{
				if( sText.Find(';') != -1 )
					sText.Remove(';');
				m_oCurStyle->m_sName += sText;
			}
			void ExitReader(RtfDocument& oDocument, RtfReader& oReader)
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
	};
	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
	{
		if( _T("stylesheet") == sCommand )
			return true;
		else
		{
			RtfStyleReader oStyleReader;
			return StartSubReader( oStyleReader, oDocument, oReader );
		}
	}
};

class ListTableReader: public RtfAbstractReader
{
public: 
	class ListReader: public RtfAbstractReader
	{
	public:
		class ListLevelReader: public RtfAbstractReader
		{
			private: RtfListLevelProperty & m_oListLevelProp;
			public: 
				ListLevelReader(RtfListLevelProperty & oListLevelProp):m_oListLevelProp(oListLevelProp)
				{
				}
		bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
		{
			if( _T("listlevel") == sCommand )
				;
			COMMAND_RTF_INT( _T("levelnfc"), m_oListLevelProp.m_nNumberType, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( _T("levelnfcn"), m_oListLevelProp.m_nNumberType, sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( _T("lvltentative"), m_oListLevelProp.m_bTentative, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( _T("leveljc"), m_oListLevelProp.m_nJustification, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( _T("leveljcn"), m_oListLevelProp.m_nJustification, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( _T("levelfollow"), m_oListLevelProp.m_nFollow, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( _T("levelstartat"), m_oListLevelProp.m_nStart, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( _T("levelnorestart"), m_oListLevelProp.m_nNoRestart, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( _T("levellegal"), m_oListLevelProp.m_nLegal, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( _T("levelpicture"), m_oListLevelProp.m_nPictureIndex, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( _T("li"), m_oListLevelProp.m_nIndent, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( _T("lin"), m_oListLevelProp.m_nIndentStart, sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( _T("fi"), m_oListLevelProp.m_nFirstIndent, sCommand, hasParameter, parameter )
			else if( _T("tx") == sCommand  )
			{
				if( true == hasParameter )
				{
					RtfTab oNewTab;
					oNewTab.m_nTab = parameter;
					m_oListLevelProp.m_oTabs.m_aTabs.push_back( oNewTab );
				}
			}
			else if( _T("leveltext") == sCommand  )
			{
				TextReader oLevelTextReader( m_oListLevelProp.m_sText );
				oLevelTextReader.m_bUseGlobalCodepage = true;
				return StartSubReader( oLevelTextReader, oDocument, oReader );
			}
			else if( _T("levelnumbers") == sCommand  )
			{
				TextReader oLevelNumberReader( m_oListLevelProp.m_sNumber );
				return StartSubReader( oLevelNumberReader, oDocument, oReader );
			}
			else
			{
				return RtfCharPropCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, oReader.m_oState->m_oCharProp  );
			}
			return true;
		}
		void ExitReader( RtfDocument& oDocument, RtfReader& oReader )
		{
			m_oListLevelProp.m_oCharProp = oReader.m_oState->m_oCharProp;
			//убираем shading и border (word тоже так делает)
			m_oListLevelProp.m_oCharProp.m_poBorder.SetDefaultRtf();
			m_oListLevelProp.m_oCharProp.m_poShading.SetDefaultRtf();

		}
};
private: RtfListProperty& m_oListProp;
public: 
		ListReader(RtfListProperty& oListProp):m_oListProp(oListProp)
		{
		}
		bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
		{
			if( _T("list") == sCommand )
				;
			COMMAND_RTF_INT( _T("listid"), m_oListProp.m_nID , sCommand, hasParameter, parameter )
			COMMAND_RTF_INT( _T("listtemplateid"), m_oListProp.m_nTemplateId , sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( _T("listsimple"), m_oListProp.m_nListSimple , sCommand, hasParameter, parameter )
			COMMAND_RTF_BOOL( _T("listhybrid"), m_oListProp.m_bListHybrid , sCommand, hasParameter, parameter )
			else if( _T("listname") == sCommand )
			{
				TextReader oListNameReader( m_oListProp.m_sName );
				return StartSubReader( oListNameReader, oDocument, oReader );
			}
			else if( _T("listlevel") == sCommand  )
			{
				RtfListLevelProperty oListLevelProp;
				ListLevelReader oListLevelReader( oListLevelProp );
				bool bResult = StartSubReader( oListLevelReader, oDocument, oReader );
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
};
class ListPictureReader: public RtfAbstractReader
{
public: bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
		{
			if( _T("listpicture") == sCommand )
			{
			}
			else if( _T("shppict") == sCommand )
			{
				RtfShapePtr oNewPicture = RtfShapePtr( new RtfShape() );
				ShppictReader oShppictReader( *oNewPicture );
				StartSubReader( oShppictReader, oDocument, oReader );
				oDocument.m_oListTabel.m_aPictureList.AddItem( oNewPicture );
			}
			else
				return false;
			return true;
		}
};
public: ListTableReader()
		{

		}
		bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
		{
			if( _T("listtable") == sCommand )
				return true;
			else if( _T("list") == sCommand )
			{
				RtfListProperty oListProp;
				ListReader oListReader(oListProp);
				bool bResult = StartSubReader( oListReader, oDocument, oReader );
				if( true == bResult )
				{
					oDocument.m_oListTabel.AddItem( oListProp );
					return true;
				}
				else
					return false;
			}
			else if( _T("listpicture") == sCommand )
			{
				ListPictureReader oListPictureReader;
				StartSubReader( oListPictureReader, oDocument, oReader );
			}
			else
				return false;
			return true;

		}
};
class ListOverrideTableReader: public RtfAbstractReader
{
private: 
	class ListOverrideReader : public RtfAbstractReader
	{
	private: 
		class lfolevelReader : public RtfAbstractReader
		{
		private: 
			RtfListOverrideProperty::ListOverrideLevels::ListOverrideLevel& m_oOverrideLevel;
		public: 
			lfolevelReader( RtfListOverrideProperty::ListOverrideLevels::ListOverrideLevel& oOverrideLevel ):m_oOverrideLevel(oOverrideLevel)
			{
			}
			bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
			{
			  if( _T("lfolevel") == sCommand)
				  return true;
			  COMMAND_RTF_INT( _T("listoverrideformat"), m_oOverrideLevel.m_nLevelIndex, sCommand, hasParameter, parameter )
			  COMMAND_RTF_INT( _T("listoverridestartat"), m_oOverrideLevel.m_nStart, sCommand, hasParameter, parameter )
			  else if( _T("listlevel") == sCommand )
			  {
				  m_oOverrideLevel.m_oLevel.m_nLevel = m_oOverrideLevel.m_nLevelIndex;
				  ListTableReader::ListReader::ListLevelReader oListLevelReader( m_oOverrideLevel.m_oLevel );
				  StartSubReader( oListLevelReader, oDocument, oReader );
			  }
			  else
				  return false;
			  return true;
			}
		};
		private: 
			RtfListOverrideProperty& m_oProperty;
		 public:
			ListOverrideReader(RtfListOverrideProperty& oProperty):m_oProperty(oProperty)
			{
			}
			bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
			{
				if( _T("listoverride") == sCommand)
					return true;
				else if( _T("listid") == sCommand )
				{
					if( true == hasParameter )
						m_oProperty.m_nListID = parameter;
				}
				else if( _T("ls") == sCommand )
				{
					if( true == hasParameter )
						m_oProperty.m_nIndex = parameter;
				}
				else if( _T("lfolevel") == sCommand )
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
		 };
public: 
	ListOverrideTableReader()
	{
	}
	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
	{
		if( _T("listoverridetable") == sCommand )
			return true;
		else if( _T("listoverride") == sCommand)
		{
			RtfListOverrideProperty oProperty;
			ListOverrideReader oListOverrideReader( oProperty );
			bool bResult = StartSubReader( oListOverrideReader, oDocument, oReader );
			if( true == bResult )
			{
				oDocument.m_oListOverrideTabel.AddItem(oProperty);
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
};

class ParagraphReader : public RtfAbstractReader
{
private: 
	CString m_sHeader;
public: 
	ParagraphPropDestination m_oParPropDest;

	ParagraphReader( CString sHeader, RtfReader& oReader ):m_sHeader(sHeader)
	{
		if( PROP_DEF != oReader.m_oState->m_oParagraphProp.m_nItap )
			m_oParPropDest.nTargetItap = oReader.m_oState->m_oParagraphProp.m_nItap;
	}
	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,CString sCommand, bool hasParameter, int parameter)
	{
		if( m_sHeader == sCommand )
			return true;
		else
			return m_oParPropDest.ExecuteCommand( oDocument, oReader, (*this), sCommand, hasParameter, parameter );
	}
	void ExecuteText( RtfDocument& oDocument, RtfReader& oReader, CString sText )
	{
		m_oParPropDest.ExecuteText( oDocument, oReader, sText );
	}
	void ExitReader( RtfDocument& oDocument, RtfReader& oReader )
	{
		m_oParPropDest.Finalize( oReader );
	}
};

class RtfDocumentCommand
{
public: 
	static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, CString sCommand, bool hasParameter, int parameter);
};
class RtfSectionCommand
{
private: 
	typedef enum{ is_none, is_border_left, is_border_top, is_border_right, is_border_bottom }InternalState;
	
	InternalState m_eInternalState;
	int nCurCollumnNumber;
public: 
	RtfSectionCommand()
	{
		m_eInternalState	= is_none;
		nCurCollumnNumber	= PROP_DEF;
	}
	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, RtfAbstractReader& oAbstrReader, CString sCommand, bool hasParameter, int parameter);
};
class RtfNormalReader : public RtfAbstractReader
{
public: 
	ParagraphPropDestination	oParagraphReaderDestination;
	RtfSectionCommand			oRtfSectionCommand;

	RtfNormalReader( RtfDocument& oDocument, RtfReader& oReader )
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
	bool ExecuteCommand( RtfDocument& oDocument, RtfReader& oReader, CString sCommand, bool hasParameter, int parameter );
	void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, CString sText)
	{
		oParagraphReaderDestination.ExecuteText( oDocument, oReader, sText );
	}
	void ExitReader(RtfDocument& oDocument, RtfReader& oReader)
	{
		oParagraphReaderDestination.Finalize( oReader );
		
		_section section;
		if(true == oDocument.GetItem( section) )
		{
			section.props->m_oProperty = oReader.m_oCurSectionProp;
		}

		if( NULL == oDocument.m_oFootnoteCon )
		{
			oDocument.m_oFootnoteCon		= TextItemContainerPtr( new TextItemContainer() );
			RtfParagraphPtr		oNewPar		= RtfParagraphPtr( new RtfParagraph() );
			RtfCharSpecialPtr	oNewChar	= RtfCharSpecialPtr( new RtfCharSpecial() );
			oNewChar->m_eType				= RtfCharSpecial::rsc_chftnsepc;
			
			oNewPar->AddItem( oNewChar );
			oDocument.m_oFootnoteCon->AddItem( oNewPar );
			oDocument.m_oProperty.m_aSpecialFootnotes.push_back( 1 );
		}
		if( NULL == oDocument.m_oFootnoteSep )
		{
			oDocument.m_oFootnoteSep		= TextItemContainerPtr( new TextItemContainer() );
			RtfParagraphPtr		oNewPar		= RtfParagraphPtr( new RtfParagraph() );
			RtfCharSpecialPtr	oNewChar	= RtfCharSpecialPtr( new RtfCharSpecial() );
			oNewChar->m_eType				= RtfCharSpecial::rsc_chftnsep;
			
			oNewPar->AddItem( oNewChar );
			oDocument.m_oFootnoteSep->AddItem( oNewPar );
			oDocument.m_oProperty.m_aSpecialFootnotes.push_back( 0 );
		}
		if( NULL == oDocument.m_oEndnoteCon )
		{
			oDocument.m_oEndnoteCon			= TextItemContainerPtr( new TextItemContainer() );
			RtfParagraphPtr		oNewPar		= RtfParagraphPtr( new RtfParagraph() );
			RtfCharSpecialPtr	oNewChar	= RtfCharSpecialPtr( new RtfCharSpecial() );
			oNewChar->m_eType				= RtfCharSpecial::rsc_chftnsepc;
			
			oNewPar->AddItem( oNewChar );
			oDocument.m_oEndnoteCon->AddItem( oNewPar );
			oDocument.m_oProperty.m_aSpecialEndnotes.push_back( 1 );
		}
		if( NULL == oDocument.m_oEndnoteSep )
		{
			oDocument.m_oEndnoteSep			= TextItemContainerPtr( new TextItemContainer() );
			RtfParagraphPtr		oNewPar		= RtfParagraphPtr( new RtfParagraph() );
			RtfCharSpecialPtr	oNewChar	= RtfCharSpecialPtr( new RtfCharSpecial() );
			oNewChar->m_eType				= RtfCharSpecial::rsc_chftnsep;
			
			oNewPar->AddItem( oNewChar );
			oDocument.m_oEndnoteSep->AddItem( oNewPar );
			oDocument.m_oProperty.m_aSpecialEndnotes.push_back( 0 );
		}
	}
private: 
	void SectDef( RtfDocument& oDocument, RtfReader& oReader )
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
};
