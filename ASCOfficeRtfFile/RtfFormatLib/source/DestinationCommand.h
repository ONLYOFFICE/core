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
#pragma once 
#include <boost/algorithm/string.hpp>

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

#include "../../../ASCOfficePPTXFile/Editor/Drawing/Shapes/BaseShape/PPTShape/Enums.h"

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
			RtfCharSpecialPtr pNewChar ( new RtfCharSpecial() );\
			pNewChar->m_eType = parameter;\
			pNewChar->m_oProperty = oReader.m_oState->m_oCharProp;\
			target->AddItem( pNewChar );\
		}
//Command не имеет состояний
#include "math.h"

class RtfBorderCommand
{
public:
    static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter,RtfBorder& oOutput);
};
class RtfShadingCommand
{
public:
    static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,  std::string sCommand, bool hasParameter, int parameter, RtfShading& oOutput );
};
class RtfShadingCellCommand
{
public: 
    static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter, RtfShading& oOutput);
};

class RtfShadingCharCommand
{
public: 
    static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter, RtfShading& oOutput );
};

class RtfShadingRowCommand
{
public: 
    static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter, RtfShading& oOutput);
};

class RtfFrameCommand
{
public:
    static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter, RtfFrame& oOutput )
	{
		return true;
	}
};
//class RtfOldList;
class TextReader : public RtfAbstractReader
{
private: 
    std::wstring&	m_sName;
	bool			m_bErease;
public:
	TextReader( std::wstring& sName, bool bErease = true ) : m_sName(sName), m_bErease(bErease)
	{
	}
    void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText)
	{
		if( true == m_bErease )
            XmlUtils::replace_all(sText, L";", L"");
		m_sName += sText;
	}
};
class RtfCharPropsCommand
{
public:
    static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,  std::string sCommand, bool hasParameter, int parameter, RtfCharProperty * props, bool bLookOnBorder = true);
	//bool bLookOnBorder; Надо ли читать свойства border( актуально для ParagraphReader )
};
class RtfParagraphPropsCommand
{
public:
    static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,  std::string sCommand, bool hasParameter, int parameter, RtfParagraphProperty * props);
};
class RtfTableCellPropsCommand
{
public:
    static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,  std::string sCommand, bool hasParameter, int parameter, RtfCellProperty * props);
};
class RtfTableRowPropsCommand
{
public:
    static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,  std::string sCommand, bool hasParameter, int parameter, RtfRowProperty * props);
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
    bool ExecuteCommand( RtfDocument& oDocument, RtfReader& oReader, std::string sKey, bool bHasPar, int nPar );
	void ExitReader( RtfDocument& oDocument, RtfReader& oReader )
	{
		m_oTarget.m_oLevelText->m_oProperty.m_oCharProperty = oReader.m_oState->m_oCharProp;
	}
};
//class RtfColorSchemeReader: public RtfAbstractReader
//{
//private: std::wstring sSchemeXml;
//public: void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText)
//		{
//			sSchemeXml += sText;
//		}
//		void ExitReader(RtfDocument& oDocument, RtfReader& oReader)
//		{
//			std::wstring sXml = RtfUtility::EncodeHex(sSchemeXml);
//			XmlUtils::CXmlLiteReader oXmlReader;
//			oXmlReader.OpenFromXmlString(sXml);
//			oXmlReader.ReadRootNode(L"a:clrMap");
//			std::wstring sValue;
//			sValue = oXmlReader.ReadNodeAttribute(L"bg1",L"");
//			if( L"" != sValue )
//				oDocument.m_oColorSchemeMapping.bg1 = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(L"tx1",L"");
//			if( L"" != sValue )
//				oDocument.m_oColorSchemeMapping.t1 = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(L"bg2",L"");
//			if( L"" != sValue )
//				oDocument.m_oColorSchemeMapping.bg2 = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(L"tx2",L"");
//			if( L"" != sValue )
//				oDocument.m_oColorSchemeMapping.t2 = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(L"accent1",L"");
//			if( L"" != sValue )
//				oDocument.m_oColorSchemeMapping.accent1 = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(L"accent2",L"");
//			if( L"" != sValue )
//				oDocument.m_oColorSchemeMapping.accent3 = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(L"accent3", L"");
//			if( L"" != sValue )
//				oDocument.m_oColorSchemeMapping.accent4 = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(L"accent4", L"");
//			if( L"" != sValue )
//				oDocument.m_oColorSchemeMapping.accent5 = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(L"accent5", L"");
//			if( L"" != sValue )
//				oDocument.m_oColorSchemeMapping.accent6 = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(L"accent6",L"");
//			if( L"" != sValue )
//				oDocument.m_oColorSchemeMapping.hyperlink = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(L"hlink", L"");
//			if( L"" != sValue )
//				oDocument.m_oColorSchemeMapping.hyperlink = sValue;
//			sValue = oXmlReader.ReadNodeAttribute(L"folHlink", L"");
//			if( L"" != sValue )
//				oDocument.m_oColorSchemeMapping.followedHyperlink = sValue;
//		}
//};
//class RtfThemeDataReader: public RtfAbstractReader
//{
//public: void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText )
//		{
//			oDocument.m_sThemeData += sText;
//		}
//};
class RtfFontTableReader: public RtfAbstractReader
{
private: 
    enum _InternalState{ is_normal, is_panose, is_altname };

    RtfFont			m_oFont;
    _InternalState	m_eInternalState;
public: 
	RtfFontTableReader()
	{
        m_bUseGlobalCodepage    = true;

		m_eInternalState = is_normal;
		m_oFont.SetDefaultOOX();
	}

    bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
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
    void ExecuteTextInternal2( RtfDocument& oDocument, RtfReader& oReader, std::string & sKey, int& nSkipChars)
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

    std::wstring RemoveLastUnchar(std::wstring str)
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
    void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText)
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
        bool ExecuteCommand( RtfDocument& oDocument, RtfReader& oReader, std::string sKey, bool bHasPar, int nPar )
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
        void ExecuteText( RtfDocument& oDocument, RtfReader& oReader, std::wstring oText )
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
};
class RtfDefCharPropReader: public RtfAbstractReader
{
public: 
	RtfCharProperty& m_oCharProp;
	RtfDefCharPropReader( RtfCharProperty& oOutput ):m_oCharProp(oOutput)
	{
	}
    bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
	{
        if( "defchp" == sCommand )
			return true;
		else 
			return RtfCharPropsCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, &oReader.m_oState->m_oCharProp );
	}
	void ExitReader( RtfDocument& oDocument, RtfReader& oReader )
	{
		m_oCharProp = oReader.m_oState->m_oCharProp;
	}
};

class RtfRevisionTableReader : public RtfAbstractReader
{
public: 
	RtfRevisionTableReader() {}

    bool ExecuteCommand( RtfDocument& oDocument, RtfReader& oReader, std::string sKey, bool bHasPar, int nPar )
	 {
        if( "revtbl" == sKey )
			return true;

		return true;
	 }

    void ExecuteText( RtfDocument& oDocument, RtfReader& oReader, std::wstring sText )
	{
        XmlUtils::replace_all(sText, L";", L"");
		oDocument.m_oRevisionTable.AddItem( sText );
	}


};

class RtfInfoReader: public RtfAbstractReader
{
private: 
	enum _InternalState{ is_normal, is_title, is_subject, is_author, is_manager, is_company, is_operator, is_category, is_keywords,
						is_comment, is_doccomm, is_hlinkbase, is_creatim, is_revtim, is_printim, is_buptim } ;
	_InternalState m_eInternalState;
public: 

	RtfInfoReader()
	{
		m_eInternalState = is_normal;
	}

    bool ExecuteCommand( RtfDocument& oDocument, RtfReader& oReader,  std::string sCommand, bool hasParameter, int parameter)
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
	void ExecuteText( RtfDocument& oDocument, RtfReader& oReader, std::wstring sText )
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
};
class RtfPictureReader :  public RtfAbstractReader
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
        std::wstring ToString()
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
            std::wstring ByteToString( BYTE* pbData, int nSize, bool bLittleEnd = true )
			 {
                std::wstring sResult;
				if( true == bLittleEnd )
				{
					for( int i = 0; i < nSize; i++ )
					{
						BYTE byteVal = pbData[i];
                        sResult += XmlUtils::IntToString(byteVal, L"%02X");
					}
				}
				else
				{
					for( int i = nSize - 1 ; i >= 0; i-- )
					{
						BYTE byteVal = pbData[i];
                        sResult += XmlUtils::IntToString(byteVal, L"%02X");
					}
				}
				return sResult;
			 }
	};

private: 
	RtfShape&		m_oShape;
    std::wstring	m_sFile;
    std::wstring	m_sData;
	bool			m_bBin;
    BYTE*			m_pbBin;
	size_t			m_nBinLength;

public: 
	RtfPictureReader( RtfReader& oReader, RtfShape& oShape ) : m_oShape(oShape)
	{
		m_bBin = false;
		m_pbBin = NULL;
		m_nBinLength = 0;
	}
	~RtfPictureReader()
	{
		RELEASEARRAYOBJECTS(m_pbBin);
	}
    bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);
    void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText)
	{
		m_sData += sText;
	}
	void ExitReader( RtfDocument& oDocument, RtfReader& oReader )
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
};

class RtfOleReader:  public RtfAbstractReader
{
private: 
	RtfOle& m_oOle;
public: 
	RtfOleReader(RtfOle& oOle) : m_oOle(oOle)
	{
	}
    bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);
};

class RtfOleBinReader : public RtfAbstractReader
{
private: 
	std::vector<std::string>/*&*/ m_arData;
public: 
	RtfOleBinReader()
	{
	}
    bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);
   
	void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText);
	void GetData( BYTE** ppData, long& nSize);
};

class RtfTrackerChangesReader:  public RtfAbstractReader
{
private: 
	enum _InternalState{ is_normal,is_charBorder, is_borderTop, is_borderLeft, is_borderBottom, is_borderRight, is_borderBox, is_borderBar,
			is_borderCellLeft,is_borderCellTop,is_borderCellRight,is_borderCellBottom,is_borderCellLR,is_borderCellRL,
			is_borderRowLeft,is_borderRowTop,is_borderRowRight,is_borderRowBottom,is_borderRowVer,is_borderRowHor} ;
//только для определения бордеров
	
	_InternalState				m_eInternalState;

	RtfCharPropertyPtr			m_pCharProps;
	RtfParagraphPropertyPtr		m_pParagraphProps;
	RtfRowPropertyPtr			m_pTableRowProps;
	//RtfCellPropertyPtr		m_pTableCellProps;
	RtfSectionPropertyPtr		m_pSectionProps;

public: 
	RtfTrackerChangesReader(RtfCharPropertyPtr			& prop) : m_pCharProps		(prop), m_eInternalState(is_normal) {}
	RtfTrackerChangesReader(RtfParagraphPropertyPtr		& prop) : m_pParagraphProps	(prop), m_eInternalState(is_normal) {}
	RtfTrackerChangesReader(RtfRowPropertyPtr			& prop) : m_pTableRowProps	(prop), m_eInternalState(is_normal) {}
	//RtfTrackerChangesReader(RtfCellPropertyPtr		& prop) : m_pTableCellProps	(prop), m_eInternalState(is_normal) {}
	RtfTrackerChangesReader(RtfSectionPropertyPtr		& prop) : m_pSectionProps	(prop), m_eInternalState(is_normal) {}

    bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,  std::string sCommand, bool hasParameter, int parameter);
};
class RtfShapeReader : public RtfAbstractReader
{
public:
	class ShapePropertyReader : public RtfAbstractReader
	{
	public:
		class ShapePropertyValueReader : public RtfAbstractReader
		{
            std::wstring& m_sPropName;
			RtfShape& m_oShape;
		public: 
            std::wstring m_sPropValue;
            ShapePropertyValueReader(std::wstring& sPropName, RtfShape& oShape):m_sPropName(sPropName),m_oShape(oShape)
			{
			}
            bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,  std::string sCommand, bool hasParameter, int parameter)
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
			void PopState( RtfDocument& oDocument, RtfReader& oReader );
            void ExecuteText( RtfDocument& oDocument, RtfReader& oReader, std::wstring oText)
			{
				m_sPropValue += oText;
			}
		};
		private:
            std::wstring sPropName;
			RtfShape& m_oShape;
		public: 
			ShapePropertyReader(RtfShape& oShape):m_oShape(oShape)
			{
			}
            bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,  std::string sCommand, bool hasParameter, int parameter)
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
	};
private: 
	RtfShape& m_oShape;
public: 
	RtfShapeReader( RtfShape& oShape ):m_oShape(oShape)
	{
	}
    bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);
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

class RtfShapeGroupReader : public RtfShapeReader
{
public: 
	RtfShape&	m_oShapeGroup;
	bool		m_bHeader; //чтобы отличать заголовок от вложенных групп

	RtfShapeGroupReader( RtfShape& oShape ) : RtfShapeReader(oShape), m_oShapeGroup(oShape)
	{
		m_bHeader = true;
	}
    bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
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
};
class RtfBackgroundReader : public RtfShapeReader
{
public: 
	RtfShape&		m_oShape;

	RtfBackgroundReader( RtfShape& oShape ) : RtfShapeReader(oShape), m_oShape(oShape)
	{
	}
    bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
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
};
class RtfOldShapeReader : public RtfAbstractReader
{
private: 
	RtfShape& m_oShape;
public: 
	RtfOldShapeReader( RtfShape& oShape ):m_oShape(oShape)
	{
	}
    bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,  std::string sCommand, bool hasParameter, int parameter);
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

class RtfShppictReader : public RtfAbstractReader
{
private: 
	RtfShape& m_oShape;
public: 
	RtfShppictReader( RtfShape& oShape ):m_oShape(oShape)
	{
	}
    bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
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
};

class RtfAllPictReader : public RtfAbstractReader
{
private: 
	RtfShape& m_oShape;
public: 
	RtfAllPictReader( RtfShape& oShape ) : m_oShape(oShape)
	{
	}
    bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
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
};

class RtfMathReader: public RtfAbstractReader
{
private: 
	RtfCharProperty m_oCharProp;
public: 
	RtfMath&								m_oMath;
	RtfParagraphProperty::ParagraphAlign	m_eParAlign;

	RtfMathReader(RtfMath& oMath): m_oMath(oMath)
	{
		m_eParAlign = RtfParagraphProperty::pa_none;
		m_oCharProp.SetDefaultRtf();
	}

    bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
	{
        if( "mmathPict" == sCommand  )
			;
        else if( "shppict" == sCommand )
		{
			m_oMath.m_oPicture = RtfShapePtr( new RtfShape() );
			RtfShppictReader oShppictReader( *m_oMath.m_oPicture );
			StartSubReader( oShppictReader, oDocument, oReader );
		}
        else if( "nonshppict" == sCommand )
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
					RtfMathPtr oNewMath ( new RtfMath() );
					oNewMath->SetRtfName( sCommand );
					
					oNewMath->m_bIsVal	= isValMath;
					oNewMath->m_bIsBool	= isBoolMath;

					RtfMathReader oSubMathReader( *oNewMath );
					bool resParseSub = StartSubReader( oSubMathReader, oDocument, oReader );

                    if (resParseSub && "mctrlPr" == sCommand)
					{
						RtfCharPropertyPtr oNewCharProp ( new RtfCharProperty() );
						oNewCharProp->Merge(oSubMathReader.m_oCharProp);
						oNewMath->AddItem( oNewCharProp );
					}

					if( oNewMath->IsValid() == true )
					{
						if ((oNewMath->m_bIsVal || oNewMath->m_bIsBool) && hasParameter)
						{
							RtfCharPtr oChar = RtfCharPtr(new RtfChar);
                            std::wstring s = ExecuteMathProp(oDocument, sCommand, parameter);
							oChar->setText( s);
							oNewMath->m_oVal.AddItem( oChar );
						}

						m_oMath.AddItem( oNewMath );
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
    void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText)
	{
		RtfCharPtr pNewChar ( new RtfChar() );
		pNewChar->m_oProperty.Merge(m_oCharProp);

		pNewChar->setText( sText ); 
		m_oMath.AddItem( pNewChar );
	}
    std::wstring ExecuteMathProp(RtfDocument& oDocument, std::string sCommand, int parameter)
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
	void ExitReader2( RtfDocument& oDocument, RtfReader& oReader )
	{
		if( RtfParagraphProperty::pa_none != m_eParAlign )
			oReader.m_oState->m_oParagraphProp.m_eAlign = m_eParAlign;
	}
};
class RtfFieldReader: public RtfAbstractReader
{
private: 
	enum _InternalState{ is_normal, is_insert, is_datafield, is_formfield, is_result } InternalState;

	_InternalState	m_eInternalState;
	RtfField&		m_oField;

public: 
	RtfFieldReader( RtfField& oField ):m_oField(oField)
	{
		m_eInternalState = is_normal;
	}
    bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter);
	void ExitReader( RtfDocument& oDocument, RtfReader& oReader )
	{
		//если результат пустой пытаемся его сгенерировать
		if( true == IsEmptyText( oDocument ) )
			TryToPepairResult( oDocument, oReader );
	}
	bool IsEmptyText( RtfDocument& oDocument )
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
private: 
	void TryToPepairResult( RtfDocument& oDocument, RtfReader& oReader )
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
};
class RtfAnnotElemReader: public RtfAbstractReader
{
public: 
	RtfAnnotElem& m_oAnnot;

	RtfAnnotElemReader( RtfAnnotElem& oAnnot ) : m_oAnnot(oAnnot)
	{
	}
    bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
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
    void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText)
	{
		m_oAnnot.m_sValue += sText ;
	}
};


class RtfBookmarkStartReader: public RtfAbstractReader
{
public: 
	RtfBookmarkStart& m_oBookmarkStart;

	RtfBookmarkStartReader( RtfBookmarkStart& oBookmark ):m_oBookmarkStart(oBookmark)
	{
	}
    bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
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
    void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText)
	{
		m_oBookmarkStart.m_sName += sText ;
	}
};

class RtfBookmarkEndReader: public RtfAbstractReader
{
public: 
	RtfBookmarkEnd& m_oBookmarkEnd;

	RtfBookmarkEndReader( RtfBookmarkEnd& oBookmark ):m_oBookmarkEnd(oBookmark)
	{
	}
    void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText)
	{
		m_oBookmarkEnd.m_sName += sText;
	}
};

class FootnoteReader;
//Destination имеет состояния
class RtfParagraphPropDestination // todo - последний параграф не обрабатывается
{
public:
	enum _InternalState{ is_normal,is_charBorder, is_borderTop, is_borderLeft, is_borderBottom, is_borderRight, is_borderBox, is_borderBar,
		is_borderCellLeft,is_borderCellTop,is_borderCellRight,is_borderCellBottom,is_borderCellLR,is_borderCellRL,
		is_borderRowLeft,is_borderRowTop,is_borderRowRight,is_borderRowBottom,is_borderRowVer,is_borderRowHor} ;
//только для определения бордера
	
private: 
	_InternalState	m_eInternalState;
	RtfParagraphPtr m_oCurParagraph;

//реальные параграфы и таблицы
	std::vector< ITextItemPtr >		aCellRenderables;
	std::vector< int >				aItaps;		//вложенность параграфов
	std::vector< RtfTableCellPtr >	aCells;
	std::vector< int >				aCellItaps; //вложенность cell
	std::vector< RtfTableRowPtr >	aRows;
	std::vector< int >				aRowItaps; //вложенность row
	RtfRowProperty oCurRowProperty;

	RtfReader* m_oReader;
public: 
	bool					m_bPar;				// если последняя команда была par, то не надо добавлять параграф
	TextItemContainerPtr	m_oTextItems;		//для разбивки на TextItem
	RtfTab					m_oCurTab;
	int						nTargetItap;		//уровень который считается не таблицей ( для того чтобы читать параграфы в таблицах )
	int						nCurItap;

	RtfParagraphPropDestination( )
	{
		nTargetItap			= PROP_DEF;
		m_bPar				= false;
		nCurItap			= 0;				//main document
		m_eInternalState	= is_normal;
		m_oCurParagraph		= RtfParagraphPtr		(new RtfParagraph());
		m_oTextItems		= TextItemContainerPtr	( new TextItemContainer() );
	}
    bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,RtfAbstractReader& oAbstrReader, std::string sCommand, bool hasParameter, int parameter);
    void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText)
	{
		m_bPar = false;
		RtfCharPtr pNewChar ( new RtfChar() );
		pNewChar->m_oProperty = oReader.m_oState->m_oCharProp;
		pNewChar->setText( sText );
		m_oCurParagraph->AddItem( pNewChar );
	}
	void AddItem( RtfParagraphPtr oItem, RtfReader& oReader, bool bEndCell, bool bEndRow );
	
	void Finalize( RtfReader& oReader);

	void EndRows(RtfReader& oReader);
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
            std::wstring sText = reader.ExecuteTextInternal( oDocument, oReader, sBullet, false, 0, nSkip );
			ExecuteText( oDocument, oReader, sText );
		 }
	 }
};
class RtfFootnoteReader: public RtfAbstractReader
{
private: 
	RtfParagraphPropDestination m_oParPropDest;
public: 
	RtfFootnote&	m_oRtfFootnote;

	RtfFootnoteReader( RtfFootnote& oRtfFootnote ):m_oRtfFootnote(oRtfFootnote)
	{
	}
    bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
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
    void ExecuteText( RtfDocument& oDocument, RtfReader& oReader, std::wstring sText )
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

class RtfAnnotationReader: public RtfAbstractReader
{
private: 
	RtfParagraphPropDestination m_oParPropDest;
public: 
	RtfAnnotation&	m_oRtfAnnotation;

	RtfAnnotationReader( RtfAnnotation& oRtfAnnotation ) : m_oRtfAnnotation(oRtfAnnotation)
	{
	}
    bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
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
    void ExecuteText( RtfDocument& oDocument, RtfReader& oReader, std::wstring sText )
	{
		m_oParPropDest.ExecuteText( oDocument, oReader, sText );
	}
	void ExitReader( RtfDocument& oDocument, RtfReader& oReader )
	{
		m_oParPropDest.Finalize( oReader );

		m_oRtfAnnotation.m_oContent = m_oParPropDest.m_oTextItems;
	}
};
class RtfDefParPropReader: public RtfAbstractReader
{
private: 
	RtfParagraphPropDestination m_oParPropDest;
public: 
	RtfDefParPropReader( )
	{
	}

    bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
	{
        if( "defpap" == sCommand )
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
//	bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
//	{
//		if( "lsdlockedexcept" == sCommand )
//			return true;
//		else if( "lsdpriority" == sCommand )
//		{
//			if( true == hasParameter )
//				m_oCurException.m_nPriority = parameter;
//		}
//		else if( "lsdunhideused" == sCommand )
//		{
//			if( true == hasParameter )
//				m_oCurException.m_nHiddenWhenUse = parameter;
//		}
//		else if( "lsdqformat" == sCommand )
//		{
//			if( true == hasParameter )
//				m_oCurException.m_nQFormat = parameter;
//		}
//		else if( "lsdlocked" == sCommand )
//		{
//			if( true == hasParameter )
//				m_oCurException.m_nLocked = parameter;
//		}
//		else if( "lsdsemihidden" == sCommand )
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
//	void ExecuteText( RtfDocument& oDocument, RtfReader& oReader, std::wstring oText, std::string oTextOriginal )
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
//public: bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
//		{
//			if( "latentstyles" == sCommand )
//				return true;
//			else if( "lsdstimax" == sCommand )
//			{
//				if( true == hasParameter )
//					oDocument.m_oLatentStyleTable.m_nCount = parameter;
//			}
//			else if( "lsdlockeddef" == sCommand )
//			{
//				if( true == hasParameter )
//					oDocument.m_oLatentStyleTable.m_nLocked = parameter;
//			}
//			else if( "lsdsemihiddendef" == sCommand )
//			{
//				if( true == hasParameter )
//					oDocument.m_oLatentStyleTable.m_nSemiHidden = parameter;
//			}
//			else if( "lsdunhideuseddef" == sCommand )
//			{
//				if( true == hasParameter )
//					oDocument.m_oLatentStyleTable.m_nUnHiddenWhenUse = parameter;
//			}
//			else if( "lsdqformatdef" == sCommand )
//			{
//				if( true == hasParameter )
//					oDocument.m_oLatentStyleTable.m_nQFormat = parameter;
//			}
//			else if( "lsdprioritydef" == sCommand )
//			{
//				if( true == hasParameter )
//					oDocument.m_oLatentStyleTable.m_nPriority = parameter;
//			}
//			else if( "lsdlockedexcept" == sCommand )
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

class RtfStyleTableReader: public RtfAbstractReader
{
	class RtfStyleReader: public RtfAbstractReader
	{
	//только для определения бордеров
	private: 
		enum _InternalState{ is_normal, is_tsbrdrt, is_tsbrdrb, is_tsbrdrl, is_tsbrdrr, is_tsbrdrh, is_tsbrdrv } InternalState;
		
		_InternalState				m_eInternalState;
		RtfParagraphPropDestination	m_oParDest;
		RtfStylePtr					m_oCurStyle;

//		RtfTableStyleProperty		m_oTableStyleProperty;
	public: 
		RtfStyleReader()
		{
			m_eInternalState = is_normal;
			
			m_oCurStyle = RtfParagraphStylePtr ( new RtfParagraphStyle() );
			m_oCurStyle->m_nID = 0;
		}
            bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
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

            void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText)
			{
                while (true)
                {
                    size_t pos = sText.find(';');
                    if (std::wstring::npos == pos) break;

                    sText.erase(pos, 1);
                }

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
    bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
	{
        if( "stylesheet" == sCommand )
			return true;
		else
		{
			RtfStyleReader oStyleReader;
			return StartSubReader( oStyleReader, oDocument, oReader );
		}
	}
};

class RtfListTableReader: public RtfAbstractReader
{
public: 
	class ListReader: public RtfAbstractReader
	{
	public:
		class ListLevelReader: public RtfAbstractReader
		{
			private: 
				RtfListLevelProperty & m_oListLevelProp;
			public: 
				ListLevelReader(RtfListLevelProperty & oListLevelProp):m_oListLevelProp(oListLevelProp)
				{
				}
                bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
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
						return RtfCharPropsCommand::ExecuteCommand( oDocument, oReader, sCommand, hasParameter, parameter, &oReader.m_oState->m_oCharProp  );
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
//--------------------------------
		ListReader(RtfListProperty& oListProp):m_oListProp(oListProp)
		{
		}
        bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
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
	private: 
		RtfListProperty& m_oListProp;

	};
	class ListPictureReader: public RtfAbstractReader
	{
	public: 
        bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
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
	};
	RtfListTableReader()
	{

	}
    bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
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
};
class RtfListOverrideTableReader: public RtfAbstractReader
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
			bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
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
		};
		private: 
			RtfListOverrideProperty& m_oProperty;
		 public:
			ListOverrideReader(RtfListOverrideProperty& oProperty):m_oProperty(oProperty)
			{
			}
            bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
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
		 };
public: 
	RtfListOverrideTableReader()
	{
	}
    bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
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
};

class RtfParagraphReader : public RtfAbstractReader
{
private: 
    std::string m_sHeader;
public: 
	RtfParagraphPropDestination m_oParPropDest;

    RtfParagraphReader( std::string sHeader, RtfReader& oReader ): m_sHeader(sHeader)
	{
		if( PROP_DEF != oReader.m_oState->m_oParagraphProp.m_nItap )
			m_oParPropDest.nTargetItap = oReader.m_oState->m_oParagraphProp.m_nItap;
	}
    bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter)
	{
		if( m_sHeader == sCommand )
			return true;
		else
			return m_oParPropDest.ExecuteCommand( oDocument, oReader, (*this), sCommand, hasParameter, parameter );
	}
    void ExecuteText( RtfDocument& oDocument, RtfReader& oReader, std::wstring sText )
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
    static bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader,  std::string sCommand, bool hasParameter, int parameter);
};
class RtfSectionCommand
{
private: 
	enum _InternalState{ is_none, is_border_left, is_border_top, is_border_right, is_border_bottom }InternalState;
	
	_InternalState m_eInternalState;
	int nCurCollumnNumber;
public: 
	RtfSectionCommand()
	{
		m_eInternalState	= is_none;
		nCurCollumnNumber	= PROP_DEF;
	}
    bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader, RtfAbstractReader& oAbstrReader, std::string sCommand, bool hasParameter, int parameter);
};
class RtfNormalReader : public RtfAbstractReader
{
public: 
	RtfParagraphPropDestination	oParagraphReaderDestination;
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
    bool ExecuteCommand( RtfDocument& oDocument, RtfReader& oReader, std::string sCommand, bool hasParameter, int parameter );
    void ExecuteText(RtfDocument& oDocument, RtfReader& oReader, std::wstring sText)
	{
		oParagraphReaderDestination.ExecuteText( oDocument, oReader, sText );
	}
	void ExitReader(RtfDocument& oDocument, RtfReader& oReader)
	{
		oParagraphReaderDestination.m_bPar = true;
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


class RtfFieldInstReader : public RtfAbstractReader, public RtfParagraphPropDestination
{

public: 
	RtfFieldInst& m_oFieldInst;

	RtfFieldInstReader( RtfFieldInst& oFieldInst ) :  m_oFieldInst(oFieldInst) {}
    bool ExecuteCommand(RtfDocument& oDocument, RtfReader& oReader , std::string sCommand, bool hasParameter, int parameter);

    void ExecuteText( RtfDocument& oDocument, RtfReader& oReader, std::wstring sText )
	{
		RtfParagraphPropDestination::ExecuteText( oDocument, oReader, sText );
	}
	void ExitReader( RtfDocument& oDocument, RtfReader& oReader )
	{
		RtfParagraphPropDestination::Finalize( oReader );

		m_oFieldInst.m_pTextItems	= m_oTextItems;
	}

};
