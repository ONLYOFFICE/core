#pragma once

#include <vector>
#include <map>

#include "DocFile.h"

#include "DocxToDocUtils.h"
#include "Logic\RunProperty.h"
#include "Logic\TableProperty.h"
#include "..\Common\SPRMCodes.h"

typedef std::vector<AVSDocFileFormat::Prl>	PrlList;

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
		byte bold = ( ( *oXmlRunProperties.Bold ) ? ( 1 ) : ( 0 ) );

		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCFBold, &bold ) );
		}

		if ( oXmlRunProperties.Italic.is_init() )
		{
		byte italic = ( ( *oXmlRunProperties.Italic ) ? ( 1 ) : ( 0 ) );

		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCFItalic, &italic ) );
		}

		if ( oXmlRunProperties.Under.is_init() && *oXmlRunProperties.Under && oXmlRunProperties.UnderType.is_init() )
		{
		byte under = this->kulMap[*oXmlRunProperties.UnderType];

		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCKul, &under ) );
		}

		if ( oXmlRunProperties.Strike.is_init() )
		{
		byte strike = ( ( *oXmlRunProperties.Strike ) ? ( 1 ) : ( 0 ) );

		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCFStrike, &strike ) );
		}

		if ( oXmlRunProperties.DStrike.is_init() )
		{
		byte dStrike = ( ( *oXmlRunProperties.DStrike ) ? ( 1 ) : ( 0 ) );

		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCFDStrike, &dStrike ) );
		}

		if ( oXmlRunProperties.SmallCaps.is_init() )
		{
		byte smallCaps = ( ( *oXmlRunProperties.SmallCaps ) ? ( 1 ) : ( 0 ) );

		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCFSmallCaps, &smallCaps ) );
		}

		if ( oXmlRunProperties.Caps.is_init() )
		{
		byte caps = ( ( *oXmlRunProperties.Caps ) ? ( 1 ) : ( 0 ) );

		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCFCaps, &caps ) );
		}

		if ( oXmlRunProperties.Emboss.is_init() )
		{
		byte emboss = ( ( *oXmlRunProperties.Emboss ) ? ( 1 ) : ( 0 ) );

		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCFEmboss, &emboss ) );
		}

		if ( oXmlRunProperties.Imprint.is_init() )
		{
		byte imprint = ( ( *oXmlRunProperties.Imprint ) ? ( 1 ) : ( 0 ) );

		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCFImprint, &imprint ) );
		}

		if ( oXmlRunProperties.Outline.is_init() )
		{
		byte outline = ( ( *oXmlRunProperties.Outline ) ? ( 1 ) : ( 0 ) );

		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCFOutline, &outline ) );
		}

		if ( oXmlRunProperties.Shadow.is_init() )
		{
		byte shadow = ( ( *oXmlRunProperties.Shadow ) ? ( 1 ) : ( 0 ) );

		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCFShadow, &shadow ) );
		}

		if ( oXmlRunProperties.Vanish.is_init() )
		{
		byte vanish = ( ( *oXmlRunProperties.Vanish ) ? ( 1 ) : ( 0 ) );

		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCFVanish, &vanish ) );
		}

		if ( oXmlRunProperties.FontSize.is_init() )
		{
		unsigned short fontSize = (unsigned short)oXmlRunProperties.FontSize;
		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCHps, (byte*)&fontSize ) );
		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCHpsBi, (byte*)&fontSize ) );
		}

		if (oXmlRunProperties.FontColor.is_init())
		{
		int colorIntValue	=	AVSDocFileFormat::COLORREF::cvAuto;
		if (false == oXmlRunProperties.FontColor->isAuto())
		colorIntValue	=	HexString2Int( oXmlRunProperties.FontColor->ToString() );

		AVSDocFileFormat::COLORREF color (colorIntValue);

		docRunProperties.push_back (AVSDocFileFormat::Prl((short)DocFileFormat::sprmCCv, color));
		}

		if ( oXmlRunProperties.Highlight.is_init() )
		{
		byte ico = DOCXDOCUTILS::ColorToIco (*oXmlRunProperties.Highlight);
		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCHighlight, &ico ) );
		}

		if ( oXmlRunProperties.Shading.is_init() && oXmlRunProperties.Shading->Fill.is_init() )
		{
		int colorIntValue	=	0;
		int fillAuto		=	0x00;

		if (oXmlRunProperties.Shading->Fill == "auto")
		{
		fillAuto = AVSDocFileFormat::COLORREF::cvAuto;
		}
		else
		{
		colorIntValue = HexString2Int( *oXmlRunProperties.Shading->Fill );
		}

		AVSDocFileFormat::SHDOperand shdOperand ( 
		AVSDocFileFormat::Shd( AVSDocFileFormat::COLORREF( (int)( 0 | fillAuto ) ), 
		AVSDocFileFormat::COLORREF( colorIntValue | fillAuto ),
		0 ) );

		docRunProperties.push_back (AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCShd, shdOperand ));
		}

		if ( oXmlRunProperties.rFonts.is_init() )
		{
		short fontIndex = 0;

		if ( oXmlRunProperties.rFonts->Ascii.is_init() && !oXmlRunProperties.rFonts->AsciiTheme.is_init() )
		{
		fontIndex = this->fontTableMap[oXmlRunProperties.rFonts->Ascii];
		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCRgFtc0, (byte*)&fontIndex ) );
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

		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCRgFtc0, (byte*)&fontIndex ) );
		}
		}

		if ( oXmlRunProperties.rFonts->Cs.is_init() && !oXmlRunProperties.rFonts->Cstheme.is_init() )
		{
		fontIndex = fontTableMap[oXmlRunProperties.rFonts->Cs];
		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCRgFtc1, (byte*)&fontIndex ) );
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

		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCRgFtc1, (byte*)&fontIndex ) );
		}
		}

		if ( oXmlRunProperties.rFonts->HAnsi.is_init() && !oXmlRunProperties.rFonts->HAnsiTheme.is_init() )
		{
		fontIndex = this->fontTableMap[oXmlRunProperties.rFonts->HAnsi];
		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCRgFtc2, (byte*)&fontIndex ) );
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

		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCRgFtc2, (byte*)&fontIndex ) );
		}
		}
		}

		if ( oXmlRunProperties.Lang.is_init() )
		{
		if ( oXmlRunProperties.Lang->Value.is_init() )
		{
		AVSDocFileFormat::LID lid = lidMap[oXmlRunProperties.Lang->Value];
		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCRgLid0_80, (byte*)lid ) );
		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCRgLid0, (byte*)lid ) );
		}

		if ( oXmlRunProperties.Lang->EastAsia.is_init() )
		{
		AVSDocFileFormat::LID lid = lidMap[oXmlRunProperties.Lang->EastAsia];
		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCRgLid1_80, (byte*)lid ) );
		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCRgLid1, (byte*)lid ) );
		}

		if ( oXmlRunProperties.Lang->Bidi.is_init() )
		{
		AVSDocFileFormat::LID lid = lidMap[oXmlRunProperties.Lang->Bidi];
		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCLidBi, (byte*)lid ) );
		}
		}

		if ( oXmlRunProperties.Spacing.is_init() )
		{
		short CDxaSpace = AVSDocFileFormat::XAS( *oXmlRunProperties.Spacing );

		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCDxaSpace, (byte*)&CDxaSpace ) );
		}

		if ( oXmlRunProperties.Index.is_init() )
		{
		string index = oXmlRunProperties.Index->ToString();
		byte CIss = (byte)AVSDocFileFormat::Constants::superSubScriptNormalText;

		if ( index == string( "superscript" ) )
		{
		CIss = (byte)AVSDocFileFormat::Constants::superSubScriptSuperscript;  
		}
		else if ( index == string( "subscript" ) )
		{
		CIss = (byte)AVSDocFileFormat::Constants::superSubScriptSubscript;  
		}

		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCIss, (byte*)&CIss ) );
		}

		if ( oXmlRunProperties.Kern.is_init() )
		{
		int kern = *oXmlRunProperties.Kern;

		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCHpsKern, (byte*)&kern ) );
		}

		if ( oXmlRunProperties.Position.is_init() )
		{
		int position = *oXmlRunProperties.Position;

		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCHpsPos, (byte*)&position ) );
		}

		if ( oXmlRunProperties.Scale.is_init() )
		{
		unsigned short scale = *oXmlRunProperties.Scale;

		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCCharScale, (byte*)&scale ) );
		}

		if ( oXmlRunProperties.Border.is_init() )
		{
		unsigned int brc80 =  (unsigned int)AVSDocFileFormat::Brc80(
		oXmlRunProperties.Border->Bdr->Sz.get_value_or_default(), 
		m_brcTypeMap[*oXmlRunProperties.Border->Bdr->Value], 
		DOCXDOCUTILS::ColorToIco (oXmlRunProperties.Border->Bdr->Color.get_value_or_default()), 
		oXmlRunProperties.Border->Bdr->Space.get_value_or_default(), 
		false, 
		false );

		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCBrc80, (byte*)(&brc80) ) );

		AVSDocFileFormat::BrcOperand brcOperand( AVSDocFileFormat::Brc(
		AVSDocFileFormat::COLORREF( HexString2Int( oXmlRunProperties.Border->Bdr->Color.get_value_or_default().ToString() ) ),
		oXmlRunProperties.Border->Bdr->Sz.get_value_or_default(), 
		m_brcTypeMap[*oXmlRunProperties.Border->Bdr->Value],
		oXmlRunProperties.Border->Bdr->Space.get_value_or_default(), 
		false, 
		false ) );

		docRunProperties.push_back( AVSDocFileFormat::Prl( (short)DocFileFormat::sprmCBrc, (byte*)brcOperand ) );
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
