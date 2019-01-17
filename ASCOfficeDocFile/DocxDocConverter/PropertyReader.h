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

#include <vector>
#include <map>

#include "DocFile.h"

#include "DocxToDocUtils.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Logic/RunProperty.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Logic/TableProperty.h"
#include "../Common/SPRMCodes.h"

typedef std::vector<ASCDocFileFormat::Prl>	PrlList;

class CXmlPropertyReader
{
public:

	CXmlPropertyReader ()
	{

	}

	~CXmlPropertyReader ()
	{

	}

	/*
	inline PrlList GetRunProperties				(const OOX::Logic::RunProperty& oXmlRunProperties) 
	{
		PrlList docRunProperties;

		if ( oXmlRunProperties.Bold.is_init() )
		{
		unsigned char bold = ( ( *oXmlRunProperties.Bold ) ? ( 1 ) : ( 0 ) );

		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCFBold, &bold ) );
		}

		if ( oXmlRunProperties.Italic.is_init() )
		{
		unsigned char italic = ( ( *oXmlRunProperties.Italic ) ? ( 1 ) : ( 0 ) );

		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCFItalic, &italic ) );
		}

		if ( oXmlRunProperties.Under.is_init() && *oXmlRunProperties.Under && oXmlRunProperties.UnderType.is_init() )
		{
		unsigned char under = this->kulMap[*oXmlRunProperties.UnderType];

		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCKul, &under ) );
		}

		if ( oXmlRunProperties.Strike.is_init() )
		{
		unsigned char strike = ( ( *oXmlRunProperties.Strike ) ? ( 1 ) : ( 0 ) );

		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCFStrike, &strike ) );
		}

		if ( oXmlRunProperties.DStrike.is_init() )
		{
		unsigned char dStrike = ( ( *oXmlRunProperties.DStrike ) ? ( 1 ) : ( 0 ) );

		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCFDStrike, &dStrike ) );
		}

		if ( oXmlRunProperties.SmallCaps.is_init() )
		{
		unsigned char smallCaps = ( ( *oXmlRunProperties.SmallCaps ) ? ( 1 ) : ( 0 ) );

		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCFSmallCaps, &smallCaps ) );
		}

		if ( oXmlRunProperties.Caps.is_init() )
		{
		unsigned char caps = ( ( *oXmlRunProperties.Caps ) ? ( 1 ) : ( 0 ) );

		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCFCaps, &caps ) );
		}

		if ( oXmlRunProperties.Emboss.is_init() )
		{
		unsigned char emboss = ( ( *oXmlRunProperties.Emboss ) ? ( 1 ) : ( 0 ) );

		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCFEmboss, &emboss ) );
		}

		if ( oXmlRunProperties.Imprint.is_init() )
		{
		unsigned char imprint = ( ( *oXmlRunProperties.Imprint ) ? ( 1 ) : ( 0 ) );

		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCFImprint, &imprint ) );
		}

		if ( oXmlRunProperties.Outline.is_init() )
		{
		unsigned char outline = ( ( *oXmlRunProperties.Outline ) ? ( 1 ) : ( 0 ) );

		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCFOutline, &outline ) );
		}

		if ( oXmlRunProperties.Shadow.is_init() )
		{
		unsigned char shadow = ( ( *oXmlRunProperties.Shadow ) ? ( 1 ) : ( 0 ) );

		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCFShadow, &shadow ) );
		}

		if ( oXmlRunProperties.Vanish.is_init() )
		{
		unsigned char vanish = ( ( *oXmlRunProperties.Vanish ) ? ( 1 ) : ( 0 ) );

		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCFVanish, &vanish ) );
		}

		if ( oXmlRunProperties.FontSize.is_init() )
		{
		unsigned short fontSize = (unsigned short)oXmlRunProperties.FontSize;
		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCHps, (unsigned char*)&fontSize ) );
		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCHpsBi, (unsigned char*)&fontSize ) );
		}

		if (oXmlRunProperties.FontColor.is_init())
		{
		int colorIntValue	=	ASCDocFileFormat::COLORREF::cvAuto;
		if (false == oXmlRunProperties.FontColor->isAuto())
		colorIntValue	=	HexString2Int( oXmlRunProperties.FontColor->ToString() );

		ASCDocFileFormat::COLORREF color (colorIntValue);

		docRunProperties.push_back (ASCDocFileFormat::Prl((short)DocFileFormat::sprmCCv, color));
		}

		if ( oXmlRunProperties.Highlight.is_init() )
		{
		unsigned char ico = DOCXDOCUTILS::ColorToIco (*oXmlRunProperties.Highlight);
		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCHighlight, &ico ) );
		}

		if ( oXmlRunProperties.Shading.is_init() && oXmlRunProperties.Shading->Fill.is_init() )
		{
		int colorIntValue	=	0;
		int fillAuto		=	0x00;

		if (oXmlRunProperties.Shading->Fill == "auto")
		{
		fillAuto = ASCDocFileFormat::COLORREF::cvAuto;
		}
		else
		{
		colorIntValue = HexString2Int( *oXmlRunProperties.Shading->Fill );
		}

		ASCDocFileFormat::SHDOperand shdOperand ( 
		ASCDocFileFormat::Shd( ASCDocFileFormat::COLORREF( (int)( 0 | fillAuto ) ), 
		ASCDocFileFormat::COLORREF( colorIntValue | fillAuto ),
		0 ) );

		docRunProperties.push_back (ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCShd, shdOperand ));
		}

		if ( oXmlRunProperties.rFonts.is_init() )
		{
		short fontIndex = 0;

		if ( oXmlRunProperties.rFonts->Ascii.is_init() && !oXmlRunProperties.rFonts->AsciiTheme.is_init() )
		{
		fontIndex = this->fontTableMap[oXmlRunProperties.rFonts->Ascii];
		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCRgFtc0, (unsigned char*)&fontIndex ) );
		}
		else if ( oXmlRunProperties.rFonts->AsciiTheme.is_init() )
		{
		string fontNameByThemeName = this->GetFontNameByThemeName( oXmlRunProperties.rFonts->AsciiTheme );

		if ( !fontNameByThemeName.empty() )
		{
		map<string, short>::const_iterator findResult = this->fontTableMap.find( fontNameByThemeName );  

		if ( findResult != this->fontTableMap.end() )
		{
		fontIndex = findResult->second;
		}

		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCRgFtc0, (unsigned char*)&fontIndex ) );
		}
		}

		if ( oXmlRunProperties.rFonts->Cs.is_init() && !oXmlRunProperties.rFonts->Cstheme.is_init() )
		{
		fontIndex = fontTableMap[oXmlRunProperties.rFonts->Cs];
		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCRgFtc1, (unsigned char*)&fontIndex ) );
		}
		else if ( oXmlRunProperties.rFonts->Cstheme.is_init() )
		{
		string fontNameByThemeName = this->GetFontNameByThemeName( oXmlRunProperties.rFonts->Cstheme );

		if ( !fontNameByThemeName.empty() )
		{
		map<string, short>::const_iterator findResult = fontTableMap.find( fontNameByThemeName );  

		if ( findResult != this->fontTableMap.end() )
		{
		fontIndex = findResult->second;
		}

		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCRgFtc1, (unsigned char*)&fontIndex ) );
		}
		}

		if ( oXmlRunProperties.rFonts->HAnsi.is_init() && !oXmlRunProperties.rFonts->HAnsiTheme.is_init() )
		{
		fontIndex = this->fontTableMap[oXmlRunProperties.rFonts->HAnsi];
		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCRgFtc2, (unsigned char*)&fontIndex ) );
		}
		else if ( oXmlRunProperties.rFonts->HAnsiTheme.is_init() )
		{
		string fontNameByThemeName = this->GetFontNameByThemeName( oXmlRunProperties.rFonts->HAnsiTheme );

		if ( !fontNameByThemeName.empty() )
		{
		map<string, short>::const_iterator findResult = fontTableMap.find( fontNameByThemeName );  

		if (findResult != fontTableMap.end())
		{
		fontIndex = findResult->second;
		}

		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCRgFtc2, (unsigned char*)&fontIndex ) );
		}
		}
		}

		if ( oXmlRunProperties.Lang.is_init() )
		{
		if ( oXmlRunProperties.Lang->Value.is_init() )
		{
		ASCDocFileFormat::LID lid = lidMap[oXmlRunProperties.Lang->Value];
		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCRgLid0_80, (unsigned char*)lid ) );
		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCRgLid0, (unsigned char*)lid ) );
		}

		if ( oXmlRunProperties.Lang->EastAsia.is_init() )
		{
		ASCDocFileFormat::LID lid = lidMap[oXmlRunProperties.Lang->EastAsia];
		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCRgLid1_80, (unsigned char*)lid ) );
		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCRgLid1, (unsigned char*)lid ) );
		}

		if ( oXmlRunProperties.Lang->Bidi.is_init() )
		{
		ASCDocFileFormat::LID lid = lidMap[oXmlRunProperties.Lang->Bidi];
		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCLidBi, (unsigned char*)lid ) );
		}
		}

		if ( oXmlRunProperties.Spacing.is_init() )
		{
		short CDxaSpace = ASCDocFileFormat::XAS( *oXmlRunProperties.Spacing );

		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCDxaSpace, (unsigned char*)&CDxaSpace ) );
		}

		if ( oXmlRunProperties.Index.is_init() )
		{
		string index = oXmlRunProperties.Index->ToString();
		unsigned char CIss = (unsigned char)ASCDocFileFormat::Constants::superSubScriptNormalText;

		if ( index == string( "superscript" ) )
		{
		CIss = (unsigned char)ASCDocFileFormat::Constants::superSubScriptSuperscript;  
		}
		else if ( index == string( "subscript" ) )
		{
		CIss = (unsigned char)ASCDocFileFormat::Constants::superSubScriptSubscript;  
		}

		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCIss, (unsigned char*)&CIss ) );
		}

		if ( oXmlRunProperties.Kern.is_init() )
		{
		int kern = *oXmlRunProperties.Kern;

		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCHpsKern, (unsigned char*)&kern ) );
		}

		if ( oXmlRunProperties.Position.is_init() )
		{
		int position = *oXmlRunProperties.Position;

		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCHpsPos, (unsigned char*)&position ) );
		}

		if ( oXmlRunProperties.Scale.is_init() )
		{
		unsigned short scale = *oXmlRunProperties.Scale;

		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCCharScale, (unsigned char*)&scale ) );
		}

		if ( oXmlRunProperties.Border.is_init() )
		{
		unsigned int brc80 =  (unsigned int)ASCDocFileFormat::Brc80(
		oXmlRunProperties.Border->Bdr->Sz.get_value_or_default(), 
		m_brcTypeMap[*oXmlRunProperties.Border->Bdr->Value], 
		DOCXDOCUTILS::ColorToIco (oXmlRunProperties.Border->Bdr->Color.get_value_or_default()), 
		oXmlRunProperties.Border->Bdr->Space.get_value_or_default(), 
		false, 
		false );

		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCBrc80, (unsigned char*)(&brc80) ) );

		ASCDocFileFormat::BrcOperand brcOperand( ASCDocFileFormat::Brc(
		ASCDocFileFormat::COLORREF( HexString2Int( oXmlRunProperties.Border->Bdr->Color.get_value_or_default().ToString() ) ),
		oXmlRunProperties.Border->Bdr->Sz.get_value_or_default(), 
		m_brcTypeMap[*oXmlRunProperties.Border->Bdr->Value],
		oXmlRunProperties.Border->Bdr->Space.get_value_or_default(), 
		false, 
		false ) );

		docRunProperties.push_back( ASCDocFileFormat::Prl( (short)DocFileFormat::sprmCBrc, (unsigned char*)brcOperand ) );
		}		

		return docRunProperties;
	}

	*/

	//
	inline PrlList GetTableStyleProperties		(const PrlList& arSource)
	{
		PrlList arFinal;

		for (size_t i = 0; i < arSource.size(); ++i)
		{
			unsigned short nCode	=	arSource[i].GetSprmCode();

			if (DocFileFormat::sprmTDxaLeft				== nCode ||
				DocFileFormat::sprmTDefTable 			== nCode ||
				DocFileFormat::sprmTDefTableShd80 		== nCode ||	
				DocFileFormat::sprmTDefTableShd3rd 		== nCode ||
				DocFileFormat::sprmTDefTableShd 		== nCode ||
				DocFileFormat::sprmTDefTableShd2nd 		== nCode ||
				DocFileFormat::sprmTWidthAfter 			== nCode ||
				DocFileFormat::sprmTFKeepFollow 		== nCode ||
				DocFileFormat::sprmTBrcTopCv 			== nCode ||
				DocFileFormat::sprmTBrcLeftCv 			== nCode ||
				DocFileFormat::sprmTBrcBottomCv 		== nCode ||
				DocFileFormat::sprmTBrcRightCv 			== nCode ||
				DocFileFormat::sprmTSetBrc80 			== nCode ||
				DocFileFormat::sprmTInsert				== nCode ||
				DocFileFormat::sprmTDelete				== nCode ||
				DocFileFormat::sprmTDxaCol 				== nCode ||
				DocFileFormat::sprmTMerge 				== nCode ||
				DocFileFormat::sprmTSplit 				== nCode ||
				DocFileFormat::sprmTTextFlow 			== nCode ||
				DocFileFormat::sprmTVertMerge 			== nCode ||
				DocFileFormat::sprmTVertAlign 			== nCode ||
				DocFileFormat::sprmTSetBrc 				== nCode ||
				DocFileFormat::sprmTCellPadding 		== nCode ||
				DocFileFormat::sprmTCellWidth 			== nCode ||
				DocFileFormat::sprmTFitText 			== nCode ||
				DocFileFormat::sprmTFCellNoWrap 		== nCode ||
				0xD642									== nCode ||		//	sprmTCellFHideMark
				DocFileFormat::sprmTSetShdTable 		== nCode ||
				DocFileFormat::sprmTCellBrcType 		== nCode ||
				DocFileFormat::sprmTFBiDi90  			== nCode ||
				DocFileFormat::sprmTFNoAllowOverlap		== nCode ||
				DocFileFormat::sprmTIpgp  				== nCode ||
				DocFileFormat::sprmTDefTableShdRaw  	== nCode ||
				DocFileFormat::sprmTDefTableShdRaw2nd  	== nCode ||
				DocFileFormat::sprmTDefTableShdRaw3rd 	== nCode ||

				DocFileFormat::sprmTCnf					== nCode )
			{
				continue;
			}

			arFinal.push_back (arSource[i]);
		}

		return arFinal;
	}	

	inline PrlList GetRunStyleProperties		(const PrlList& arSource)
	{
		PrlList arFinal;

		for (size_t i = 0; i < arSource.size(); ++i)
		{
			unsigned short nCode	=	arSource[i].GetSprmCode();

			if (DocFileFormat::sprmCFSpecVanish 		== nCode ||
				DocFileFormat::sprmCIstd 				== nCode ||
				DocFileFormat::sprmCIstdPermute 		== nCode ||
				DocFileFormat::sprmCPlain 				== nCode ||
				DocFileFormat::sprmCMajority 			== nCode ||
				DocFileFormat::sprmCDispFldRMark		== nCode ||
				DocFileFormat::sprmCIdslRMarkDel		== nCode ||
				DocFileFormat::sprmCLbcCRJ 				== nCode ||
				DocFileFormat::sprmCPbiIBullet 			== nCode ||
				DocFileFormat::sprmCPbiGrf				== nCode ||

				DocFileFormat::sprmCCnf					== nCode )
			{
				continue;
			}

			arFinal.push_back (arSource[i]);
		}

		return arFinal;
	}	

	inline PrlList GetParagraphStyleProperties	(const PrlList& arSource)
	{
		PrlList arFinal;

		for (size_t i = 0; i < arSource.size(); ++i)
		{
			unsigned short nCode	=	arSource[i].GetSprmCode();

			if (DocFileFormat::sprmPIstd 			== nCode ||
				DocFileFormat::sprmPIstdPermute 	== nCode ||
				DocFileFormat::sprmPIncLvl 			== nCode ||
				DocFileFormat::sprmPNest80 			== nCode ||
				DocFileFormat::sprmPChgTabs 		== nCode ||
				DocFileFormat::sprmPDcs 			== nCode ||
				DocFileFormat::sprmPHugePapx 		== nCode ||
				DocFileFormat::sprmPFInnerTtp 		== nCode ||
				0x245A								== nCode ||		//	sprmPFOpenTch 
				DocFileFormat::sprmPNest 			== nCode ||
				DocFileFormat::sprmPFNoAllowOverlap == nCode ||
				DocFileFormat::sprmPIstdListPermute == nCode ||
				DocFileFormat::sprmPTableProps		== nCode ||
				DocFileFormat::sprmPTIstdInfo		== nCode ||

				0xC666								== nCode )		//	sprmPCnf
			{
				continue;
			}

			arFinal.push_back (arSource[i]);
		}

		return arFinal;
	}	


private:

};
