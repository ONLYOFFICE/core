#pragma once

#include <map>
#include <string>
#include <algorithm>
#include <math.h>

#include "Prl.h"
#include "Constants.h"
#include "CSSAOperand.h"
#include "COLORREF.h"
#include "SHDOperand.h"
#include "..\Common\SPRMCodes.h"

#include "Brc.h"
#include "Brc80MayBeNil.h"

#include "TableBrc80Operand.h"
#include "TableBrcOperand.h"
#include "ItcFirstLim.h"

#include "Logic\TCMar.h"
#include "Styles.h"
#include "Logic\Shading.h"
#include "Logic\Color.h"
#include "Logic\ColorsTable.h"
#include "Logic\Table.h"
#include "Logic\TableBorders.h"
#include "Logic\TableStyleProperties.h"
#include "Logic\TableCellBorders.h"

typedef vector<ASCDocFileFormat::Shd> ShdList;

namespace DOCXDOCUTILS
{
	inline int ShadingPattern (const std::string& type)
	{
		int ipat = 0;

		if(std::string("clear") == type)
			ipat = 0;
		else if(std::string("nil") == type)
			ipat = 0;
		else if(std::string("solid") == type)
			ipat = 1;
		else if(std::string("horzStripe") == type)
			ipat = 14;
		else if(std::string("vertStripe") == type)
			ipat = 15;
		else if(std::string("diagStripe") == type)
			ipat = 16;
		else if(std::string("reverseDiagStripe") == type)
			ipat = 17;
		else if(std::string("horzCross") == type)
			ipat = 18;
		else if(std::string("diagCross") == type)
			ipat = 19;
		else if(std::string("thinHorzStripe") == type)
			ipat = 20;
		else if(std::string("thinVertStripe") == type)
			ipat = 21;
		else if(std::string("thinDiagStripe") == type)
			ipat = 22;
		else if(std::string("thinReverseDiagStripe") == type)
			ipat = 23;
		else if(std::string("thinHorzCross") == type)
			ipat = 24;
		else if(std::string("thinDiagCross") == type)
			ipat = 25;
		else if(std::string("pct5") == type)
			ipat = 2;
		else if(std::string("pct10") == type)
			ipat = 3;
		else if(std::string("pct12") == type)
			ipat = 37;
		else if(std::string("pct15") == type)
			ipat = 38;
		else if(std::string("pct20") == type)
			ipat = 4;
		else if(std::string("pct25") == type)
			ipat = 5;
		else if(std::string("pct30") == type)
			ipat = 6;
		else if(std::string("pct35") == type)
			ipat = 43;
		else if(std::string("pct37") == type)
			ipat = 44;
		else if(std::string("pct40") == type)
			ipat = 7;
		else if(std::string("pct45") == type)
			ipat = 46;
		else if(std::string("pct50") == type)
			ipat = 8;
		else if(std::string("pct55") == type)
			ipat = 49;
		else if(std::string("pct60") == type)
			ipat = 9;
		else if(std::string("pct62") == type)
			ipat = 51;
		else if(std::string("pct65") == type)
			ipat = 52;
		else if(std::string("pct70") == type)
			ipat = 10;
		else if(std::string("pct75") == type)
			ipat = 11;
		else if(std::string("pct80") == type)
			ipat = 12;
		else if(std::string("pct85") == type)
			ipat = 57;
		else if(std::string("pct87") == type)
			ipat = 58;
		else if(std::string("pct90") == type)
			ipat = 13;
		else if(std::string("pct95") == type)
			ipat = 60;

		return ipat;
	}
	inline ASCDocFileFormat::Constants::Fts TableWidthUnitsFromString (const std::string& type)
	{
		if (type == std::string("nil"))
			return ASCDocFileFormat::Constants::ftsNil;
		else if (type == std::string("auto"))
			return ASCDocFileFormat::Constants::ftsAuto;
		else if (type == std::string("pct"))
			return ASCDocFileFormat::Constants::ftsPercent;
		else if (type == std::string("dxa"))
			return ASCDocFileFormat::Constants::ftsDxa;

		return ASCDocFileFormat::Constants::ftsNil;	
	}

	inline ASCDocFileFormat::Prl GetOpTop  (const OOX::Logic::TCMar& oTCMar, int nCells)
	{
		ASCDocFileFormat::CSSAOperand opCSSA (ASCDocFileFormat::CSSA(ASCDocFileFormat::ItcFirstLim(0, nCells), 
			ASCDocFileFormat::Constants::fbrcTop,
			TableWidthUnitsFromString (oTCMar.Top->Margin->Type),
			oTCMar.Top->Margin->W));

		return ASCDocFileFormat::Prl( (short)DocFileFormat::sprmTCellPadding, (byte*)opCSSA );
	}

	inline ASCDocFileFormat::Prl GetOpLeft (const OOX::Logic::TCMar& oTCMar, int nCells)
	{
		ASCDocFileFormat::CSSAOperand opCSSA(ASCDocFileFormat::CSSA( ASCDocFileFormat::ItcFirstLim(0, nCells), 
			ASCDocFileFormat::Constants::fbrcLeft,
			TableWidthUnitsFromString (oTCMar.Left->Margin->Type),
			oTCMar.Left->Margin->W));

		return ASCDocFileFormat::Prl( (short)DocFileFormat::sprmTCellPadding, (byte*)opCSSA );
	}

	inline ASCDocFileFormat::Prl GetOpRight (const OOX::Logic::TCMar& oTCMar, int nCells)
	{
		ASCDocFileFormat::CSSAOperand opCSSA( ASCDocFileFormat::CSSA( ASCDocFileFormat::ItcFirstLim(0, nCells), 
			ASCDocFileFormat::Constants::fbrcRight,
			DOCXDOCUTILS::TableWidthUnitsFromString (oTCMar.Right->Margin->Type),
			oTCMar.Right->Margin->W ) );

		return ASCDocFileFormat::Prl( (short)DocFileFormat::sprmTCellPadding, (byte*)opCSSA );
	}

	inline ASCDocFileFormat::Prl GetOpBottom (const OOX::Logic::TCMar& oTCMar, int nCells)
	{
		ASCDocFileFormat::CSSAOperand opCSSA( ASCDocFileFormat::CSSA( ASCDocFileFormat::ItcFirstLim(0, nCells), 
			ASCDocFileFormat::Constants::fbrcBottom,
			DOCXDOCUTILS::TableWidthUnitsFromString (oTCMar.Bottom->Margin->Type),
			oTCMar.Bottom->Margin->W ) );

		return ASCDocFileFormat::Prl( (short)DocFileFormat::sprmTCellPadding, (byte*)opCSSA );
	}

	inline ASCDocFileFormat::Shd GetBackGroundShading (const nullable_property<OOX::Logic::Shading>& oShading)
	{
		// <w:shd w:val="clear" w:color="auto" w:fill="4F81BD" w:themeFill="accent1"/>

		int nFillValue	=	0;
		int nFillAuto	=	0;
		int nColorValue	=	0;
		int nColorAuto	=	0;
		int nIpat		=	0;		

		if (oShading.is_init())
		{
			if (oShading->fill.is_init())
			{
				if (std::string("auto") == (*oShading->fill))
					nFillAuto	=	ASCDocFileFormat::COLORREF::cvAuto;
				else
					nFillValue	=	HexString2Int(*oShading->fill);
			}

			if (oShading->color.is_init())
			{
				if (std::string("auto") == (*oShading->color))
					nColorAuto	=	ASCDocFileFormat::COLORREF::cvAuto;
				else
					nColorValue	=	HexString2Int(*oShading->color );
			}

			if (oShading->value.is_init())
				nIpat			=	ShadingPattern (*oShading->value);

			return ASCDocFileFormat::Shd(ASCDocFileFormat::COLORREF((int)(nColorValue | nColorAuto)), ASCDocFileFormat::COLORREF(nFillValue | nFillAuto), nIpat);		
		}

		return ASCDocFileFormat::Shd();
	}
}

namespace DOCXDOCUTILS	//	Help Borders and Fill 
{
	class CTblBorders
	{
	public:

		enum Border
		{
			TOP_BORDER		=	0,
			BOTTOM_BORDER	=	1,
			LEFT_BORDER		=	2,
			RIGHT_BORDER	=	3,
			INSIDEH_BORDER	=	4,
			INSIDEV_BORDER	=	5
		};

	public:

		CTblBorders ()
		{

		}

		CTblBorders (OOX::Styles::Style oStyle) : m_oStyle (oStyle)
		{

		}

		CTblBorders (const OOX::Logic::Table& oXmlTable, OOX::Styles::Style oStyle, BOOL bCacheTableStyles = TRUE) : m_oStyle (oStyle)
		{
			 GetBrcMap ();

			// параметры могут быть не правильно выставлены, поэтому стоит прочекать стили на существование

			m_bHaveBeginRow			=	oXmlTable.tblPr->firstRow;
			m_bHaveEndRow			=	oXmlTable.tblPr->lastRow;
			m_bHaveBeginCol			=	oXmlTable.tblPr->firstColumn;
			m_bHaveEndCol			=	oXmlTable.tblPr->lastColumn;

			m_bNoHBand				=	oXmlTable.tblPr->noHBand;
			m_bNoVBand				=	oXmlTable.tblPr->noVBand;
			
			m_bStyleFirstRow		=	FALSE;
			m_bStyleBand1Horz		=	FALSE;
			m_bStyleBand1Vert		=	FALSE;
			m_bStyleFirstCol		=	FALSE;
			m_bStyleLastRow			=	FALSE;
			m_bStyleLastCol			=	FALSE;

			m_bExistStyleFirstRow	=	FALSE;
			m_bExistStyleBand1Horz	=	FALSE;
			m_bExistStyleBand1Vert	=	FALSE;
			m_bExistStyleFirstCol	=	FALSE;
			m_bExistStyleLastRow	=	FALSE;
			m_bExistStyleLastCol	=	FALSE;

			if (bCacheTableStyles)
			{
				m_nRowBand			=	1;
				if(m_oStyle.tblPr.is_init())
					m_nRowBand		=	m_oStyle.tblPr->tblStyleRowBandSize;

				int nBandCol		=	1;
				if(m_oStyle.tblPr.is_init())
					m_nBandCol		=	m_oStyle.tblPr->tblStyleColBandSize;

				m_bStyleBorders		=	FALSE;

				if (m_oStyle.tblPr.is_init())
				{
					if (m_oStyle.tblPr->tblBorders.is_init())
					{
						m_oStyleBorders	=	m_oStyle.tblPr->tblBorders;
						m_bStyleBorders	=	TRUE;
					}
				}

				if (m_oStyle.tableStyles->exist(std::string("firstRow")))
				{
					m_bExistStyleFirstRow	=	TRUE;

					const OOX::Logic::TableStyleProperties& oInternalStyle = m_oStyle.tableStyles->operator [](std::string("firstRow"));
					if (oInternalStyle.tcPr.is_init() && oInternalStyle.tcPr->tblBorders.is_init())
					{
						m_bStyleFirstRow	=	TRUE;
						m_oBordersFirstRow	=	oInternalStyle.tcPr->tblBorders;
					}
				}

				if (m_oStyle.tableStyles->exist(std::string("lastRow")))
				{
					m_bExistStyleLastRow	=	TRUE;

					const OOX::Logic::TableStyleProperties& oInternalStyle = m_oStyle.tableStyles->operator [](std::string("lastRow"));
					if (oInternalStyle.tcPr.is_init() && oInternalStyle.tcPr->tblBorders.is_init())
					{
						m_bStyleLastRow		=	TRUE;
						m_oBordersLastRow	=	oInternalStyle.tcPr->tblBorders;
					}
				}

				if (m_oStyle.tableStyles->exist(std::string("band1Horz")))
				{
					m_bExistStyleBand1Horz	=	TRUE;

					const OOX::Logic::TableStyleProperties& oInternalStyle = m_oStyle.tableStyles->operator [](std::string("band1Horz"));
					if (oInternalStyle.tcPr.is_init() && oInternalStyle.tcPr->tblBorders.is_init())
					{
						m_bStyleBand1Horz	=	TRUE;
						m_oBordersBand1Horz	=	oInternalStyle.tcPr->tblBorders;
					}
				}

				if (m_oStyle.tableStyles->exist(std::string("band1Vert")))
				{
					m_bExistStyleBand1Vert	=	TRUE;

					const OOX::Logic::TableStyleProperties& oInternalStyle = m_oStyle.tableStyles->operator [](std::string("band1Vert"));
					if (oInternalStyle.tcPr.is_init() && oInternalStyle.tcPr->tblBorders.is_init())
					{
						m_bStyleBand1Vert	=	TRUE;
						m_oBordersBand1Vert	=	oInternalStyle.tcPr->tblBorders;
					}
				}

				if (m_oStyle.tableStyles->exist(std::string("band2Horz")))
				{
					m_bExistStyleBand2Horz	=	TRUE;

					const OOX::Logic::TableStyleProperties& oInternalStyle = m_oStyle.tableStyles->operator [](std::string("band2Horz"));
					if (oInternalStyle.tcPr.is_init() && oInternalStyle.tcPr->tblBorders.is_init())
					{
						m_bStyleBand2Horz	=	TRUE;
						m_oBordersBand2Horz	=	oInternalStyle.tcPr->tblBorders;
					}
				}

				if (m_oStyle.tableStyles->exist(std::string("band2Vert")))
				{
					m_bExistStyleBand2Vert	=	TRUE;

					const OOX::Logic::TableStyleProperties& oInternalStyle = m_oStyle.tableStyles->operator [](std::string("band2Vert"));
					if (oInternalStyle.tcPr.is_init() && oInternalStyle.tcPr->tblBorders.is_init())
					{
						m_bStyleBand2Vert	=	TRUE;
						m_oBordersBand2Vert	=	oInternalStyle.tcPr->tblBorders;
					}
				}

				if (m_oStyle.tableStyles->exist(std::string("firstCol")))
				{
					m_bExistStyleFirstCol	=	TRUE;

					const OOX::Logic::TableStyleProperties& oInternalStyle = m_oStyle.tableStyles->operator [](std::string("firstCol"));
					if (oInternalStyle.tcPr.is_init() && oInternalStyle.tcPr->tblBorders.is_init())
					{
						m_bStyleFirstCol	=	TRUE;
						m_oBordersFirstCol	=	oInternalStyle.tcPr->tblBorders;
					}
				}

				if (m_oStyle.tableStyles->exist(std::string("lastCol")))
				{
					m_bExistStyleFirstCol	=	TRUE;

					const OOX::Logic::TableStyleProperties& oInternalStyle = m_oStyle.tableStyles->operator [](std::string("lastCol"));
					if (oInternalStyle.tcPr.is_init() && oInternalStyle.tcPr->tblBorders.is_init())
					{
						m_bStyleLastCol		=	TRUE;
						m_oBordersLastCol	=	oInternalStyle.tcPr->tblBorders;
					}
				}
			}
		}

		//
		inline BOOL Append (int nX, int nY, int nCX, int nCY, const nullable_property<OOX::Logic::TableBorders>& oTableBorders, const nullable_property<OOX::Logic::TableCellBorders>& oInternalBorders)			
		{
			m_nIndCellX				=	nX;
			m_nIndCellY				=	nY;
			
			m_nCellsX				=	nCX - 1;
			m_nCellsY				=	nCY - 1;

			/*

			m_oBrc80MayBeNilTop		=	ASCDocFileFormat::Brc80MayBeNil(2, 1, 1, 0, false, false);
			m_oBrc80MayBeNilBottom	=	ASCDocFileFormat::Brc80MayBeNil(2, 1, 1, 0, false, false);
			m_oBrc80MayBeNilLeft	=	ASCDocFileFormat::Brc80MayBeNil(2, 1, 1, 0, false, false);
			m_oBrc80MayBeNilRight	=	ASCDocFileFormat::Brc80MayBeNil(2, 1, 1, 0, false, false);

			m_oBrcTop				=	ASCDocFileFormat::Brc(ASCDocFileFormat::COLORREF(HexString2Int(std::string ("000000"))), 1, 0, 0, false, false);
			m_oBrc80Top				=	ASCDocFileFormat::Brc80MayBeNil(0, 0, 0, 0, false, false);

			m_oBrcBottom			=	ASCDocFileFormat::Brc(ASCDocFileFormat::COLORREF(HexString2Int(std::string ("000000"))), 1, 0, 0, false, false);
			m_oBrc80Bottom			=	ASCDocFileFormat::Brc80MayBeNil(0, 0, 0, 0, false, false);

			m_oBrcLeft				=	ASCDocFileFormat::Brc(ASCDocFileFormat::COLORREF(HexString2Int(std::string ("000000"))), 1, 0, 0, false, false);
			m_oBrc80Left			=	ASCDocFileFormat::Brc80MayBeNil(0, 0, 0, 0, false, false);

			m_oBrcRight				=	ASCDocFileFormat::Brc(ASCDocFileFormat::COLORREF(HexString2Int(std::string ("000000"))), 1, 0, 0, false, false);
			m_oBrc80Right			=	ASCDocFileFormat::Brc80MayBeNil(0, 0, 0, 0, false, false);

			// for test

			m_oBrcTop				=	ASCDocFileFormat::Brc(ASCDocFileFormat::COLORREF(0xFF0000), 50, 1, 0, false, false);
			m_oBrc80Top				=	ASCDocFileFormat::Brc80MayBeNil(50, 1, 0, 0, false, false);
			m_oBrcBottom			=	ASCDocFileFormat::Brc(ASCDocFileFormat::COLORREF(0xFF), 50, 1, 0, false, false);
			m_oBrc80Bottom			=	ASCDocFileFormat::Brc80MayBeNil(50, 1, 0, 0, false, false);
			m_oBrcLeft				=	ASCDocFileFormat::Brc(ASCDocFileFormat::COLORREF(0xFFFF00), 50, 1, 0, false, false);
			m_oBrc80Left			=	ASCDocFileFormat::Brc80MayBeNil(50, 1, 0, 0, false, false);
			m_oBrcRight				=	ASCDocFileFormat::Brc(ASCDocFileFormat::COLORREF(0xF0F0FF), 50, 1, 0, false, false);
			m_oBrc80Right			=	ASCDocFileFormat::Brc80MayBeNil(50, 1, 0, 0, false, false);

			*/

			SetNilBrc (TOP_BORDER);
			SetNilBrc (BOTTOM_BORDER);
			SetNilBrc (LEFT_BORDER);
			SetNilBrc (RIGHT_BORDER);

			ReadTopNillBorder	 (oTableBorders);
			ReadBottomNillBorder (oTableBorders);
			ReadLeftNillBorder	 (oTableBorders);
			ReadRightNillBorder	 (oTableBorders);

			AppendCellBorder (TOP_BORDER,	 oTableBorders, oInternalBorders);
			AppendCellBorder (BOTTOM_BORDER, oTableBorders, oInternalBorders);
			AppendCellBorder (LEFT_BORDER,	 oTableBorders, oInternalBorders);
			AppendCellBorder (RIGHT_BORDER,  oTableBorders, oInternalBorders);

			return TRUE;
		}

		inline BOOL ReadTopNillBorder (const nullable_property<OOX::Logic::TableBorders>& oBorders)
		{
			if (oBorders.is_init())
			{				
				if (oBorders->top.is_init())
				{
					m_oBrc80MayBeNilTop	= ASCDocFileFormat::Brc80MayBeNil(oBorders->top.get_value_or_default().Bdr->Sz.get_value_or_default(), 
						m_oBrcTypeMap[oBorders->top.get_value_or_default().Bdr->Value], 
						ColorToIco (oBorders->top.get_value_or_default().Bdr->Color.get_value_or_default()), 
						oBorders->top.get_value_or_default().Bdr->Space.get_value_or_default(), 
						false, 
						false);

					return TRUE;
				}
			}

			return FALSE;
		}

		inline BOOL ReadBottomNillBorder (const nullable_property<OOX::Logic::TableBorders>& oBorders)
		{
			if (oBorders.is_init())
			{				
				if (oBorders->bottom.is_init())
				{
					m_oBrc80MayBeNilBottom	= ASCDocFileFormat::Brc80MayBeNil(oBorders->bottom.get_value_or_default().Bdr->Sz.get_value_or_default(), 
						m_oBrcTypeMap[oBorders->bottom.get_value_or_default().Bdr->Value], 
						ColorToIco (oBorders->bottom.get_value_or_default().Bdr->Color.get_value_or_default()), 
						oBorders->bottom.get_value_or_default().Bdr->Space.get_value_or_default(), 
						false, 
						false);

					return TRUE;
				}
			}

			return FALSE;
		}

		inline BOOL ReadLeftNillBorder (const nullable_property<OOX::Logic::TableBorders>& oBorders)
		{
			if (oBorders.is_init())
			{				
				if (oBorders->left.is_init())
				{
					m_oBrc80MayBeNilLeft = ASCDocFileFormat::Brc80MayBeNil(oBorders->left.get_value_or_default().Bdr->Sz.get_value_or_default(), 
						m_oBrcTypeMap[oBorders->left.get_value_or_default().Bdr->Value], 
						ColorToIco (oBorders->left.get_value_or_default().Bdr->Color.get_value_or_default()), 
						oBorders->left.get_value_or_default().Bdr->Space.get_value_or_default(), 
						false, 
						false);

					return TRUE;
				}
			}

			return FALSE;
		}

		inline BOOL ReadRightNillBorder (const nullable_property<OOX::Logic::TableBorders>& oBorders)
		{
			if (oBorders.is_init())
			{				
				if (oBorders->right.is_init())
				{
					m_oBrc80MayBeNilRight = ASCDocFileFormat::Brc80MayBeNil(oBorders->right.get_value_or_default().Bdr->Sz.get_value_or_default(), 
						m_oBrcTypeMap[oBorders->right.get_value_or_default().Bdr->Value], 
						ColorToIco (oBorders->right.get_value_or_default().Bdr->Color.get_value_or_default()), 
						oBorders->right.get_value_or_default().Bdr->Space.get_value_or_default(), 
						false, 
						false);

					return TRUE;
				}
			}

			return FALSE;
		}

		//
		inline const vector<ASCDocFileFormat::TableBrcOperand>& GetSpecificationBorders () const 
		{
			return m_oBrcs;
		}

		inline const vector<ASCDocFileFormat::TableBrc80Operand>& GetDescriptBorders () const 
		{
			return m_oBrc80s;
		}

		inline const ASCDocFileFormat::Brc80MayBeNil& GetTopNillBorder () const
		{
			return m_oBrc80MayBeNilTop;
		}

		inline const ASCDocFileFormat::Brc80MayBeNil& GetBottomNillBorder () const
		{
			return m_oBrc80MayBeNilBottom;
		}

		inline const ASCDocFileFormat::Brc80MayBeNil& GetLeftNillBorder () const
		{
			return m_oBrc80MayBeNilLeft;
		}

		inline const ASCDocFileFormat::Brc80MayBeNil& GetRightNillBorder () const
		{
			return m_oBrc80MayBeNilRight;
		}

		//
		inline void Clean ()
		{
			m_oBrcs.clear ();
			m_oBrc80s.clear ();
		}


		//
		inline map<std::string, byte>& GetBrcMap ()
		{
			if (0 == m_oBrcTypeMap.size())
			{
				m_oBrcTypeMap.insert( pair<string, byte>( string( "none" ), 0x00 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "single" ), 0x01 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "thick" ), 0x02 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "double" ), 0x03 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "hairline" ), 0x05 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "dotted" ), 0x06 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "dashed" ), 0x07 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "dotDash" ), 0x08 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "dotDotDash" ), 0x09 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "triple" ), 0x0A ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "thinThickSmallGap" ), 0x0B ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "thickThinSmallGap" ), 0x0C ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "thinThickThinSmallGap" ), 0x0D ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "thinThickMediumGap" ), 0x0E ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "thickThinMediumGap" ), 0x0F ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "thinThickThinMediumGap" ), 0x10 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "thinThickLargeGap" ), 0x11 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "thickThinLargeGap" ), 0x12 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "thinThickThinLargeGap" ), 0x13 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "wave" ), 0x14 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "doubleWave" ), 0x15 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "dashSmallGap" ), 0x16 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "dashDotStroked" ), 0x17 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "threeDEmboss" ), 0x18 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "threeDEngrave" ), 0x19 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "outset" ), 0x1A ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "inset" ), 0x1B ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "apples" ), 0x40 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "archedScallops" ), 0x41 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "babyPacifier" ), 0x42 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "babyRattle" ), 0x43 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "balloons3Colors" ), 0x44 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "balloonsHotAir" ), 0x45 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "basicBlackDashes" ), 0x46 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "basicBlackDots" ), 0x47 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "basicBlackSquares" ), 0x48 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "basicThinLines" ), 0x49 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "basicWhiteDashes" ), 0x4A ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "basicWhiteDots" ), 0x4B ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "basicWhiteSquares" ), 0x4C ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "basicWideInline" ), 0x4D ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "basicWideMidline" ), 0x4E ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "basicWideOutline" ), 0x4F ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "bats" ), 0x50 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "birds" ), 0x51 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "birdsFlight" ), 0x52 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "cabins" ), 0x53 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "cakeSlice" ), 0x54 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "candyCorn" ), 0x55 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "celticKnotwork" ), 0x56 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "certificateBanner" ), 0x57 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "chainLink" ), 0x58 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "champagneBottle" ), 0x59 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "checkedBarBlack" ), 0x5A ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "checkedBarColor" ), 0x5B ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "checkered" ), 0x5C ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "christmasTree" ), 0x5D ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "circlesLines" ), 0x5E ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "circlesRectangles" ), 0x5F ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "classicalWave" ), 0x60 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "clocks" ), 0x61 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "compass" ), 0x62 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "confetti" ), 0x63 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "confettiGrays" ), 0x64 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "confettiOutline" ), 0x65 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "confettiStreamers" ), 0x66 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "confettiWhite" ), 0x67 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "cornerTriangles" ), 0x68 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "couponCutoutDashes" ), 0x69 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "couponCutoutDots" ), 0x6A ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "crazyMaze" ), 0x6B ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "creaturesButterfly" ), 0x6C ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "creaturesFish" ), 0x6D ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "creaturesInsects" ), 0x6E ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "creaturesLadyBug" ), 0x6F ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "crossStitch" ), 0x70 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "cup" ), 0x71 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "decoArch" ), 0x72 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "decoArchColor" ), 0x73 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "decoBlocks" ), 0x74 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "diamondsGray" ), 0x75 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "doubleD" ), 0x76 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "doubleDiamonds" ), 0x77 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "earth1" ), 0x78 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "earth2" ), 0x79 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "eclipsingSquares1" ), 0x7A ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "eclipsingSquares2" ), 0x7B ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "eggsBlack" ), 0x7C ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "fans" ), 0x7D ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "film" ), 0x7E ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "firecrackers" ), 0x7F ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "flowersBlockPrint" ), 0x80 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "flowersDaisies" ), 0x81 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "flowersModern1" ), 0x82 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "flowersModern2" ), 0x83 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "flowersPansy" ), 0x84 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "flowersRedRose" ), 0x85 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "flowersRoses" ), 0x86 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "flowersTeacup" ), 0x87 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "flowersTiny" ), 0x88 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "gems" ), 0x89 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "gingerbreadMan" ), 0x8A ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "gradient" ), 0x8B ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "handmade1" ), 0x8C ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "handmade2" ), 0x8D ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "heartBalloon" ), 0x8E ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "heartGray" ), 0x8F ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "hearts" ), 0x90 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "heebieJeebies" ), 0x91 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "holly" ), 0x92 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "houseFunky" ), 0x93 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "hypnotic" ), 0x94 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "iceCreamCones" ), 0x95 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "lightBulb" ), 0x96 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "lightning1" ), 0x97 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "lightning2" ), 0x98 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "mapPins" ), 0x99 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "mapleLeaf" ), 0x9A ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "mapleMuffins" ), 0x9B ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "marquee" ), 0x9C ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "marqueeToothed" ), 0x9D ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "moons" ), 0x9E ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "mosaic" ), 0x9F ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "musicNotes" ), 0xA0 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "northwest" ), 0xA1 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "ovals" ), 0xA2 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "packages" ), 0xA3 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "palmsBlack" ), 0xA4 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "palmsColor" ), 0xA5 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "paperClips" ), 0xA6 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "papyrus" ), 0xA7 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "partyFavor" ), 0xA8 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "partyGlass" ), 0xA9 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "pencils" ), 0xAA ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "people" ), 0xAB ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "peopleWaving" ), 0xAC ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "peopleHats" ), 0xAD ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "poinsettias" ), 0xAE ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "postageStamp" ), 0xAF ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "pumpkin1" ), 0xB0 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "pushPinNote2" ), 0xB1 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "pushPinNote1" ), 0xB2 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "pyramids" ), 0xB3 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "pyramidsAbove" ), 0xB4 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "quadrants" ), 0xB5 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "rings" ), 0xB6 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "safari" ), 0xB7 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "sawtooth" ), 0xB8 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "sawtoothGray" ), 0xB9 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "scaredCat" ), 0xBA ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "seattle" ), 0xBB ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "shadowedSquares" ), 0xBC ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "sharksTeeth" ), 0xBD ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "shorebirdTracks" ), 0xBE ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "skyrocket" ), 0xBF ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "snowflakeFancy" ), 0xC0 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "snowflakes" ), 0xC1 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "sombrero" ), 0xC2 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "southwest" ), 0xC3 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "stars" ), 0xC4 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "starsTop" ), 0xC5 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "stars3d" ), 0xC6 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "starsBlack" ), 0xC7 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "starsShadowed" ), 0xC8 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "sun" ), 0xC9 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "swirligig" ), 0xCA ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "tornPaper" ), 0xCB ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "tornPaperBlack" ), 0xCC ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "trees" ), 0xCD ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "triangleParty" ), 0xCE ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "triangles" ), 0xCF ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "tribal1" ), 0xD0 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "tribal2" ), 0xD1 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "tribal3" ), 0xD2 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "tribal4" ), 0xD3 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "tribal5" ), 0xD4 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "tribal6" ), 0xD5 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "twistedLines1" ), 0xD6 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "twistedLines2" ), 0xD7 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "vine" ), 0xD8 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "waveline" ), 0xD9 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "weavingAngles" ), 0xDA ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "weavingBraid" ), 0xDB ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "weavingRibbon" ), 0xDC ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "weavingStrips" ), 0xDD ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "whiteFlowers" ), 0xDE ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "woodwork" ), 0xDF ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "xIllusions" ), 0xE0 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "zanyTriangles" ), 0xE1 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "zigZag" ), 0xE2 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "zigZagStitch" ), 0xE3 ) );
				m_oBrcTypeMap.insert( pair<string, byte>( string( "none" ), 0xFF ) );
			}

			return m_oBrcTypeMap;
		}

	private:

		inline BOOL AppendCellBorder (int nBorder, const nullable_property<OOX::Logic::TableBorders>& oTableBorders, const nullable_property<OOX::Logic::TableCellBorders>& oBorders)
		{		
			if (m_nIndCellX==0 && m_nIndCellY == 5)
			{
				int c = 0;
			}

			if (ApplyBorderFromInlineStyle(nBorder, oBorders))						//	€чейка имеет свои собственные настройки дл€ границ (inline-стиль)
				return TRUE;

			if (AppleBorderFromIndeTableStyle (nBorder, oTableBorders))				//	таблица имеет свои собственные настройки дл€ границ (inline-стиль)
				return TRUE;

			BOOL bAppend	=	ApplyBorderFromStylePropery (nBorder);				//	если у €чейки есть настройки по умолчанию
			
			if (0 == m_bNoVBand)													//	контент (вертикальный набор)
			{
				if (m_nIndCellX > 0 || (0 == m_bHaveBeginCol))
				{
					BOOL bValidInd	=	((m_nIndCellX - 1) / m_nBandCol) % 2 - 1;	//	смещаем, если доступен стиль у первого столбца
					if (0 == m_bHaveBeginCol)
						bValidInd	=	((m_nIndCellX - 1) / m_nBandCol) % 2;

					if (bValidInd)
					{
						if (m_oBordersBand1Vert.ValidBorder(nBorder) || m_oBordersBand1Horz.ValidBorder(INSIDEV_BORDER))
						{
							SetupBrc (m_oBordersBand1Vert, nBorder, 1);
							bAppend	=	TRUE;
						}
					}
					else
					{
						if (m_oBordersBand2Vert.ValidBorder(nBorder) || m_oBordersBand1Horz.ValidBorder(INSIDEV_BORDER))
						{
							SetupBrc (m_oBordersBand2Vert, nBorder, 1);
							bAppend	=	TRUE;
						}
					}
				}
			}

			// контент (горизонтальный набор)

			if (0 == m_bNoHBand)
			{
				if (m_nIndCellY > 0 || (0 == m_bHaveBeginRow))
				{
					BOOL bValidInd	=	((m_nIndCellY - 1) / m_nRowBand) % 2 - 1;	//	смещаем, если доступен стиль у первой строка
					if (0 == m_bHaveBeginRow)
						bValidInd	=	((m_nIndCellY - 1) / m_nRowBand) % 2;

					if (bValidInd)
					{
						if (m_oBordersBand1Horz.ValidBorder(nBorder) || m_oBordersBand1Horz.ValidBorder(INSIDEH_BORDER))
						{
							SetupBrc (m_oBordersBand1Horz, nBorder, 0);
							bAppend	=	TRUE;
						}							
					}
					else
					{
						if (m_oBordersBand2Horz.ValidBorder(nBorder) || m_oBordersBand2Horz.ValidBorder(INSIDEH_BORDER))
						{
							SetupBrc (m_oBordersBand2Horz, nBorder, 0);
							bAppend	=	TRUE;
						}
					}
				}
			}

			// первый столбец

			if (m_bHaveBeginCol)
			{
				if (0 == m_nIndCellX)
				{
					if (m_oBordersFirstCol.ValidBorder(nBorder))
					{
						SetupBrc (m_oBordersFirstCol, nBorder, 1, FALSE);
						bAppend	=	TRUE;
					}			
				}
			}

			// последний столбец

			if (m_bHaveEndCol)
			{
				if (m_nIndCellX == m_nCellsX)
				{
					if (m_oBordersLastCol.ValidBorder(nBorder))
					{
						SetupBrc (m_oBordersLastCol, nBorder, 1, FALSE);
						bAppend	=	TRUE;
					}
				}
			}

			// перва€ строка

			if (m_bHaveBeginRow)
			{
				if (0 == m_nIndCellY)
				{
					if (m_oBordersFirstRow.ValidBorder(nBorder))
					{
						SetupBrc (m_oBordersFirstRow, nBorder, 0, FALSE);
						bAppend	=	TRUE;
					}
				}	
			}

			// последн€€ строка

			if (m_bHaveEndRow)
			{
				if (m_nIndCellY == m_nCellsY)
				{
					if (m_oBordersLastRow.ValidBorder(nBorder))
					{
						SetupBrc (m_oBordersLastRow, nBorder, 0, FALSE);
						bAppend	=	TRUE;
					}
				}
			}

			AppendOpList (nBorder);

			return FALSE;
		}

		inline BOOL ApplyBorderFromInlineStyle (int nBorder, const nullable_property<OOX::Logic::TableCellBorders>& oBorders)
		{
			if (oBorders.is_init())							//	у €чейки свой стиль
			{
				OOX::Logic::Border oBorder;
				if (oBorders->GetBorder(nBorder, oBorder))
				{
					CompileSide (nBorder, oBorder);
					AppendOpList (nBorder);

					return TRUE;
				}				
			}

			return FALSE;
		}

		inline BOOL ApplyBorderFromStylePropery (int nBorder)
		{
			BOOL bAppend = FALSE;

			if (m_bStyleBorders)
			{
				// внешние границы

				if ((nBorder == TOP_BORDER) && (0 == m_nIndCellY))
				{
					OOX::Logic::Border oBorder;
					if (m_oStyleBorders.GetBorder (TOP_BORDER,oBorder))
					{
						CompileSide (TOP_BORDER, oBorder);
						bAppend	=	TRUE;
					}	
				}

				// внешние границы

				if ((nBorder == BOTTOM_BORDER) && (m_nIndCellY == m_nCellsY))
				{
					OOX::Logic::Border oBorder;
					if (m_oStyleBorders.GetBorder (BOTTOM_BORDER,oBorder))
					{
						CompileSide (BOTTOM_BORDER, oBorder);
						bAppend	=	TRUE;
					}					
				}

				// внешние границы

				if ((nBorder == LEFT_BORDER) && (0 == m_nIndCellX))
				{
					OOX::Logic::Border oBorder;
					if (m_oStyleBorders.GetBorder (LEFT_BORDER,oBorder))
					{
						CompileSide (LEFT_BORDER, oBorder);
						bAppend	=	TRUE;
					}					
				}

				// внешние границы

				if ((nBorder == RIGHT_BORDER) && (m_nIndCellX == m_nCellsX))
				{
					OOX::Logic::Border oBorder;
					if (m_oStyleBorders.GetBorder (RIGHT_BORDER,oBorder))
					{
						CompileSide (RIGHT_BORDER, oBorder);
						bAppend	=	TRUE;
					}					
				}

				// внутренние горизонтальные границы 

				if (((nBorder == TOP_BORDER) && (0 < m_nIndCellY)) || ((nBorder == BOTTOM_BORDER) && (m_nIndCellY != m_nCellsY)))
				{
					OOX::Logic::Border oBorder;
					if (m_oStyleBorders.GetBorder (INSIDEH_BORDER,oBorder))
					{
						CompileSide (nBorder, oBorder);
						bAppend	=	TRUE;
					}					
				}

				// внутренние вертикальные границы 

				if (((nBorder == LEFT_BORDER) && (0 < m_nIndCellX)) || ((nBorder == RIGHT_BORDER) && (m_nIndCellX != m_nCellsX)))
				{
					OOX::Logic::Border oBorder;
					if (m_oStyleBorders.GetBorder (INSIDEV_BORDER,oBorder))
					{
						CompileSide (nBorder, oBorder);
						bAppend	=	TRUE;
					}					
				}
			}

			return bAppend;
		}

		inline BOOL AppleBorderFromIndeTableStyle (int nBorder, const nullable_property<OOX::Logic::TableBorders>& oTableBorders)
		{
			if (oTableBorders.is_init())				
			{
				OOX::Logic::Border oBorder;
				if (oTableBorders->GetBorder(nBorder, oBorder))
				{
					CompileSide (nBorder, oBorder);
					AppendOpList (nBorder);

					return TRUE;
				}				
			}

			return FALSE;
		}

		inline BOOL IsInsideBorder (int nBorder, BOOL bEven = FALSE)
		{
			if (TOP_BORDER == nBorder)
			{
				if (1 == m_nRowBand)	// inside объектов нету
					return FALSE;

				BOOL bIsOdd			=	(m_nIndCellY / m_nRowBand) % 2;
				if (m_bHaveBeginRow)
					bIsOdd			=	((m_nIndCellY-1) / m_nRowBand) % 2;

				if (bIsOdd)
				{
					int nModulo		=	m_nIndCellY % m_nRowBand;
					if (m_bHaveBeginRow)
						nModulo		=	(m_nIndCellY-1) % m_nRowBand;

					if (0 != nModulo)	//	верх в наборе
						return TRUE;
				}					
				else
				{
					int nModulo		=	m_nIndCellY % m_nRowBand;
					if (m_bHaveBeginRow)
						nModulo		=	(m_nIndCellY-1) % m_nRowBand;

					if (0 != nModulo)	//	верх в наборе
						return TRUE;
				}

				return FALSE;
			}

			if (BOTTOM_BORDER == nBorder)
			{
				if (1 == m_nRowBand)	// inside объектов нету
					return FALSE;

				BOOL bIsOdd			=	(m_nIndCellY / m_nRowBand) % 2;
				if (m_bHaveBeginRow)
					bIsOdd			=	((m_nIndCellY-1) / m_nRowBand) % 2;

				if (bIsOdd)
				{
					int nModulo		=	m_nIndCellY % m_nRowBand;
					if (m_bHaveBeginRow)
						nModulo		=	(m_nIndCellY-1) % m_nRowBand;

					//if (2 == m_nIndCellX)
					//	ATLTRACE (L"(%d,%d) : %d, Even : %d\n", m_nIndCellX, m_nIndCellY, nModulo, bIsEven);

					if (nModulo < m_nRowBand)	//	низ в наборе
						return TRUE;
				}					
				else
				{
					int nModulo		=	m_nIndCellY % m_nRowBand;
					if (m_bHaveBeginRow)
						nModulo		=	(m_nIndCellY-1) % m_nRowBand;

					//if (2 == m_nIndCellX)
					//	ATLTRACE (L"(%d,%d) : %d, Even : %d\n", m_nIndCellX, m_nIndCellY, nModulo, bIsEven);

					if (nModulo < m_nRowBand)	//	низ в наборе
						return TRUE;
				}

				return FALSE;
			}

			if (LEFT_BORDER == nBorder)
			{
				if (1 == m_nBandCol)
					return FALSE;

				int nModulo		=	m_nIndCellX % m_nBandCol;
				if (m_bHaveBeginCol)
					nModulo		=	(m_nIndCellX - 1) % m_nBandCol;

				if (0 != nModulo)	
					return TRUE;

				return FALSE;
			}

			if (RIGHT_BORDER == nBorder)
			{
				if (1 == m_nBandCol)
					return FALSE;

				int nModulo		=	m_nIndCellY % m_nBandCol;
				if (m_bHaveBeginRow)
					nModulo		=	(m_nIndCellY - 1) % m_nBandCol;

				if (nModulo < m_nRowBand)
					return TRUE;

				return FALSE;
			}

			return FALSE;
		}

		//
		inline ASCDocFileFormat::Brc80MayBeNil CreateNilBorder (const OOX::Logic::Border& oBorder)
		{
			return ASCDocFileFormat::Brc80MayBeNil(oBorder.Bdr->Sz.get_value_or_default(), 
				m_oBrcTypeMap[oBorder.Bdr->Value], ColorToIco (oBorder.Bdr->Color.get_value_or_default()), oBorder.Bdr->Space.get_value_or_default(), false, false);
		}

		inline ASCDocFileFormat::Brc CreateBorder (const OOX::Logic::Border& oBorder)
		{
			std::string strColor	=	oBorder.Bdr->Color.get_value_or_default().ToString();
			if (std::string("auto") == strColor)
				strColor			=	std::string ("000000");

			return ASCDocFileFormat::Brc(ASCDocFileFormat::COLORREF(HexString2Int(strColor)),
				oBorder.Bdr->Sz.get_value_or_default(), m_oBrcTypeMap[oBorder.Bdr->Value], oBorder.Bdr->Space.get_value_or_default(), false, false);
		}

		inline void CompileSide (int nSide, const OOX::Logic::Border& oBorder)
		{
			if (TOP_BORDER == nSide)
			{
				m_oBrc80Top				=	CreateNilBorder (oBorder);
				m_oBrcTop				=	CreateBorder (oBorder);
				m_oBrc80MayBeNilTop		=	m_oBrc80Top;

				return;
			}

			if (BOTTOM_BORDER == nSide)
			{
				m_oBrc80Bottom			=	CreateNilBorder (oBorder);
				m_oBrcBottom			=	CreateBorder (oBorder);
				m_oBrc80MayBeNilBottom	=	m_oBrc80Top;

				return;
			}

			if (LEFT_BORDER == nSide)
			{
				m_oBrc80Left			=	CreateNilBorder (oBorder);
				m_oBrcLeft				=	CreateBorder (oBorder);
				m_oBrc80MayBeNilLeft	=	m_oBrc80Left;

				return;
			}

			if (RIGHT_BORDER == nSide)
			{
				m_oBrc80Right			=	CreateNilBorder (oBorder);
				m_oBrcRight				=	CreateBorder (oBorder);
				m_oBrc80MayBeNilRight	=	m_oBrc80Right;
			}
		}

		inline void SetNilBrc (int nBorder)
		{
			if (TOP_BORDER == nBorder)
			{
				m_oBrc80MayBeNilTop			=	ASCDocFileFormat::Brc80MayBeNil(0xFFFFFFFF);							
				m_oBrc80Top					=	ASCDocFileFormat::Brc80MayBeNil(0xFFFFFFFF);
				m_oBrcTop					=	ASCDocFileFormat::Brc(ASCDocFileFormat::COLORREF(0xFF000000), 0, 0, 0, false, false);			
			}

			if (BOTTOM_BORDER == nBorder)
			{
				m_oBrc80MayBeNilBottom		=	ASCDocFileFormat::Brc80MayBeNil(0xFFFFFFFF);							
				m_oBrc80Bottom				=	ASCDocFileFormat::Brc80MayBeNil(0xFFFFFFFF);
				m_oBrcBottom				=	ASCDocFileFormat::Brc(ASCDocFileFormat::COLORREF(0xFF000000), 0, 0, 0, false, false);
			}

			if (LEFT_BORDER == nBorder)
			{
				m_oBrc80MayBeNilLeft		=	ASCDocFileFormat::Brc80MayBeNil(0xFFFFFFFF);							
				m_oBrc80Left				=	ASCDocFileFormat::Brc80MayBeNil(0xFFFFFFFF);
				m_oBrcLeft					=	ASCDocFileFormat::Brc(ASCDocFileFormat::COLORREF(0xFF000000), 0, 0, 0, false, false);
			}

			if (RIGHT_BORDER == nBorder)
			{
				m_oBrc80MayBeNilRight		=	ASCDocFileFormat::Brc80MayBeNil(0xFFFFFFFF);							
				m_oBrc80Right				=	ASCDocFileFormat::Brc80MayBeNil(0xFFFFFFFF);
				m_oBrcRight					=	ASCDocFileFormat::Brc(ASCDocFileFormat::COLORREF(0xFF000000), 0, 0, 0, false, false);
			}
		}

		inline void SetupBrc (const OOX::Logic::TableCellBorders& oBorder, int nBorder, BOOL bBand, BOOL bInside = TRUE)
		{
			if (TOP_BORDER == nBorder)
			{
				BOOL bValidInside = TRUE;

				OOX::Logic::Border oInsideH;
				if ((TRUE == bInside) && (m_nIndCellY > 0) && (oBorder.GetBorder(INSIDEH_BORDER, oInsideH)))
				{
					if (std::string("nil") == oInsideH.Bdr->Value)		//	запрет внутри таблицы на рисование границ
					{
						if (IsInsideBorder(nBorder))
							SetNilBrc (nBorder);
					}
					else
					{
						CompileSide (nBorder, oInsideH);
					}
				}
				else
				{
					if (oBorder.top.is_init())
					{
						if (std::string("nil") != oBorder.top->Bdr->Value && ((m_nIndCellY == 0) || (0==bBand)))
						{
							CompileSide (nBorder, oBorder.top);
						}
						else
						{
							SetNilBrc (TOP_BORDER);
						}
					}
					else
					{
						// SetNilBrc (TOP_BORDER);
					}
				}
			}

			if (BOTTOM_BORDER == nBorder)
			{
				if (m_nIndCellX == 1 && m_nIndCellY == 1)
				{
					int ccc = 0;
				}

				BOOL bValidInside = TRUE;

				OOX::Logic::Border oInsideH;
				if ((TRUE == bInside) && (m_nIndCellY != m_nCellsY) && (oBorder.GetBorder(INSIDEH_BORDER, oInsideH)))
				{
					if (std::string("nil") == oInsideH.Bdr->Value)		//	запрет внутри таблицы на рисование границ
					{
						if (IsInsideBorder(nBorder))
							SetNilBrc (nBorder);
					}
					else
					{
						CompileSide (nBorder, oInsideH);
					}
				}
				else
				{
					if (oBorder.bottom.is_init())
					{
						if (std::string("nil") != oBorder.bottom->Bdr->Value && ((m_nIndCellY == m_nCellsY) || (0==bBand)))
						{
							CompileSide (nBorder, oBorder.bottom);
						}
						else
						{
							SetNilBrc (BOTTOM_BORDER);
						}
					}
					else
					{
						// SetNilBrc (BOTTOM_BORDER);
					}
				}
			}

			if (LEFT_BORDER == nBorder)
			{
				OOX::Logic::Border oInsideV;
				if ((TRUE == bInside) && (m_nIndCellX > 0) && oBorder.GetBorder(INSIDEV_BORDER, oInsideV))	// более приоритетен над стил€м которые задают границу
				{
					if (std::string("nil") == oInsideV.Bdr->Value)		//	запрет внутри таблицы на рисование границ
					{
						if (IsInsideBorder(nBorder))
							SetNilBrc (nBorder);
					}
					else
					{
						CompileSide (nBorder, oInsideV);
					}
				}
				else
				{
					if (oBorder.left.is_init())
					{
						if (std::string("nil") != oBorder.left->Bdr->Value && ((m_nIndCellX == 0) || (1==bBand)))
						{
							CompileSide (nBorder, oBorder.left);
						}
						else
						{
							SetNilBrc (LEFT_BORDER);
						}
					}
					else
					{
						// SetNilBrc (LEFT_BORDER);
					}
				}
			}

			if (RIGHT_BORDER == nBorder)
			{
				OOX::Logic::Border oInsideV;
				if ((TRUE == bInside) && (m_nIndCellX != m_nCellsX) && oBorder.GetBorder(INSIDEV_BORDER, oInsideV))
				{
					if (std::string("nil") == oInsideV.Bdr->Value)		//	запрет внутри таблицы на рисование границ
					{
						if (IsInsideBorder(nBorder))
							SetNilBrc (nBorder);
					}
					else
					{
						CompileSide (nBorder, oInsideV);
					}
				}
				else
				{
					if (oBorder.right.is_init())
					{
						if ((std::string("nil") != oBorder.right->Bdr->Value) && ((m_nIndCellX == m_nCellsX) || (1==bBand)))
						{
							CompileSide (nBorder, oBorder.right);
						}
						else
						{
							SetNilBrc (RIGHT_BORDER);
						}
					}
					else
					{
						// SetNilBrc (RIGHT_BORDER);
					}
				}
			}
		}

		inline void AppendOpList (int nBorder)
		{
			if (TOP_BORDER == nBorder)
			{
				ASCDocFileFormat::TableBrc80Operand brc80Operand(ASCDocFileFormat::ItcFirstLim(m_nIndCellX, (m_nIndCellX + 1)), ASCDocFileFormat::Constants::bordersToApplyTopBorder, m_oBrc80Top);
				m_oBrc80s.push_back(brc80Operand);

				ASCDocFileFormat::TableBrcOperand brcOperand(ASCDocFileFormat::ItcFirstLim(m_nIndCellX, (m_nIndCellX + 1)), ASCDocFileFormat::Constants::bordersToApplyTopBorder, m_oBrcTop);
				m_oBrcs.push_back(brcOperand);
			}

			if (BOTTOM_BORDER == nBorder)
			{
				ASCDocFileFormat::TableBrc80Operand brc80Operand( ASCDocFileFormat::ItcFirstLim(m_nIndCellX, (m_nIndCellX + 1)), ASCDocFileFormat::Constants::bordersToApplyBottomBorder, m_oBrc80Bottom);
				m_oBrc80s.push_back(brc80Operand);

				ASCDocFileFormat::TableBrcOperand brcOperand( ASCDocFileFormat::ItcFirstLim(m_nIndCellX, (m_nIndCellX + 1)), ASCDocFileFormat::Constants::bordersToApplyBottomBorder, m_oBrcBottom);
				m_oBrcs.push_back(brcOperand);
			}

			if (LEFT_BORDER == nBorder)
			{
				ASCDocFileFormat::TableBrc80Operand brc80Operand( ASCDocFileFormat::ItcFirstLim(m_nIndCellX, (m_nIndCellX + 1)), ASCDocFileFormat::Constants::bordersToApplyLogicalLeftBorder, m_oBrc80Left);
				m_oBrc80s.push_back(brc80Operand);

				ASCDocFileFormat::TableBrcOperand brcOperand( ASCDocFileFormat::ItcFirstLim(m_nIndCellX, (m_nIndCellX + 1)), ASCDocFileFormat::Constants::bordersToApplyLogicalLeftBorder, m_oBrcLeft);
				m_oBrcs.push_back(brcOperand);
			}

			if (RIGHT_BORDER == nBorder)
			{
				ASCDocFileFormat::TableBrc80Operand brc80Operand(ASCDocFileFormat::ItcFirstLim(m_nIndCellX, (m_nIndCellX + 1)), ASCDocFileFormat::Constants::bordersToApplyLogicalRightBorder, m_oBrc80Right);
				m_oBrc80s.push_back(brc80Operand);

				ASCDocFileFormat::TableBrcOperand brcOperand(ASCDocFileFormat::ItcFirstLim(m_nIndCellX, (m_nIndCellX + 1)), ASCDocFileFormat::Constants::bordersToApplyLogicalRightBorder, m_oBrcRight);
				m_oBrcs.push_back(brcOperand);
			}
		}

		inline unsigned char ColorToIco	(const OOX::Logic::Color& oColor)
		{
			OOX::Logic::ColorsTable colorsTable;

			std::string colorName = colorsTable.fromColor(oColor);

			if (0 == m_oColorsMap.size())
			{
				m_oColorsMap.insert( pair<string, byte>( "auto", 0x00 ) );
				m_oColorsMap.insert( pair<string, byte>( "black", 0x01 ) );
				m_oColorsMap.insert( pair<string, byte>( "blue", 0x02 ) );
				m_oColorsMap.insert( pair<string, byte>( "cyan", 0x03 ) );
				m_oColorsMap.insert( pair<string, byte>( "green", 0x04 ) );
				m_oColorsMap.insert( pair<string, byte>( "magenta", 0x05 ) );
				m_oColorsMap.insert( pair<string, byte>( "red", 0x06 ) );
				m_oColorsMap.insert( pair<string, byte>( "yellow", 0x07 ) );
				m_oColorsMap.insert( pair<string, byte>( "white", 0x08 ) );
				m_oColorsMap.insert( pair<string, byte>( "darkBlue", 0x09 ) );
				m_oColorsMap.insert( pair<string, byte>( "darkCyan", 0x0A ) );
				m_oColorsMap.insert( pair<string, byte>( "darkGreen", 0x0B ) );
				m_oColorsMap.insert( pair<string, byte>( "darkMagenta", 0x0C ) );
				m_oColorsMap.insert( pair<string, byte>( "darkRed", 0x0D ) );
				m_oColorsMap.insert( pair<string, byte>( "darkYellow", 0x0E ) );
				m_oColorsMap.insert( pair<string, byte>( "darkGray", 0x0F ) );
				m_oColorsMap.insert( pair<string, byte>( "lightGray", 0x10 ) );
			}

			return m_oColorsMap[colorName];
		}


	private:

		static map<std::string, byte>				m_oBrcTypeMap;
		static map<std::string, byte>				m_oColorsMap;

		OOX::Styles::Style							m_oStyle;

		BOOL										m_bHaveBeginRow;
		BOOL										m_bHaveBeginCol;
		BOOL										m_bHaveEndRow;
		BOOL										m_bHaveEndCol;

		BOOL										m_bNoHBand;
		BOOL										m_bNoVBand;

		int											m_nIndCellX;
		int											m_nIndCellY;

		int											m_nCellsX;
		int											m_nCellsY;

		int											m_nRowBand;
		int											m_nBandCol;

		vector<ASCDocFileFormat::TableBrcOperand>	m_oBrcs;
		vector<ASCDocFileFormat::TableBrc80Operand>	m_oBrc80s;

		ASCDocFileFormat::Brc						m_oBrcTop;
		ASCDocFileFormat::Brc80MayBeNil				m_oBrc80Top;	
		ASCDocFileFormat::Brc						m_oBrcBottom;
		ASCDocFileFormat::Brc80MayBeNil				m_oBrc80Bottom;	
		ASCDocFileFormat::Brc						m_oBrcLeft;
		ASCDocFileFormat::Brc80MayBeNil				m_oBrc80Left;	
		ASCDocFileFormat::Brc						m_oBrcRight;
		ASCDocFileFormat::Brc80MayBeNil				m_oBrc80Right;	

		ASCDocFileFormat::Brc80MayBeNil				m_oBrc80MayBeNilTop;
		ASCDocFileFormat::Brc80MayBeNil				m_oBrc80MayBeNilBottom;
		ASCDocFileFormat::Brc80MayBeNil				m_oBrc80MayBeNilLeft;
		ASCDocFileFormat::Brc80MayBeNil				m_oBrc80MayBeNilRight;

		BOOL										m_bStyleBorders;
		OOX::Logic::TableBorders					m_oStyleBorders;

		BOOL										m_bExistStyleFirstRow;
		BOOL										m_bStyleFirstRow;
		OOX::Logic::TableCellBorders				m_oBordersFirstRow;

		BOOL										m_bExistStyleLastRow;
		BOOL										m_bStyleLastRow;
		OOX::Logic::TableCellBorders				m_oBordersLastRow;

		BOOL										m_bExistStyleBand1Horz;
		BOOL										m_bStyleBand1Horz;
		OOX::Logic::TableCellBorders				m_oBordersBand1Horz;

		BOOL										m_bExistStyleBand1Vert;
		BOOL										m_bStyleBand1Vert;
		OOX::Logic::TableCellBorders				m_oBordersBand1Vert;

		BOOL										m_bExistStyleBand2Horz;
		BOOL										m_bStyleBand2Horz;
		OOX::Logic::TableCellBorders				m_oBordersBand2Horz;

		BOOL										m_bExistStyleBand2Vert;
		BOOL										m_bStyleBand2Vert;
		OOX::Logic::TableCellBorders				m_oBordersBand2Vert;

		BOOL										m_bExistStyleFirstCol;
		BOOL										m_bStyleFirstCol;
		OOX::Logic::TableCellBorders				m_oBordersFirstCol;

		BOOL										m_bExistStyleLastCol;
		BOOL										m_bStyleLastCol;
		OOX::Logic::TableCellBorders				m_oBordersLastCol;
	};

	class CTblFill
	{
	public:

		CTblFill (const OOX::Logic::Table& oXmlTable, OOX::Styles::Style oStyle) : m_oStyle (oStyle)
		{
			m_bHaveBeginRow		=	oXmlTable.tblPr->firstRow;
			m_bHaveEndRow		=	oXmlTable.tblPr->lastRow;
			m_bHaveBeginCol		=	oXmlTable.tblPr->firstColumn;
			m_bHaveEndCol		=	oXmlTable.tblPr->lastColumn;

			m_bNoHBand			=	oXmlTable.tblPr->noHBand;
			m_bNoVBand			=	oXmlTable.tblPr->noVBand;

			if (m_bHaveBeginRow)
			{
				if (false == m_oStyle.tableStyles->exist(std::string("firstRow")))
					m_bHaveBeginRow		=	0;
			}

			if (m_bHaveEndRow)
			{
				if (false == m_oStyle.tableStyles->exist(std::string("lastRow")))
					m_bHaveEndRow		=	0;
			}

			if (m_bHaveBeginCol)
			{
				if (false == m_oStyle.tableStyles->exist(std::string("firstCol")))
					m_bHaveBeginCol		=	0;
			}

			if (m_bHaveEndCol)
			{
				if (false == m_oStyle.tableStyles->exist(std::string("lastCol")))
					m_bHaveEndCol		=	0;
			}

			// TODO : кэширование стилей
		}

		BOOL AddCell (int nX, int nY, int nCX, int nCY, const nullable_property<OOX::Logic::Shading>& oShading)
		{	
			/*

			ѕриоритет заливки €чейки. 

			-	ѕараметры у самой €чейки.
			-	ƒополнительные стили у стил€ таблицы по строкам и столбцам (стиль у строки, затрет стиль столбца).
			-	Ќастройки по умолчанию у стил€ к таблице.

			*/

			ASCDocFileFormat::Shd	oShd;

			int nRowBand	=	1;
			if(m_oStyle.tblPr.is_init())
				nRowBand	=	__max(m_oStyle.tblPr->tblStyleRowBandSize,1);

			int nBandCol	=	1;
			if(m_oStyle.tblPr.is_init())
				nBandCol	=	__max(m_oStyle.tblPr->tblStyleColBandSize,1);

			BOOL bAppend	=	FALSE;

			if (oShading.is_init())
			{
				oShd		=	GetBackGroundShading (oShading);
				bAppend		=	TRUE;
			}
			else
			{
				if (m_oStyle.tcPr.is_init())	//	если у €чейки есть настройки по умолчанию
				{				
					if (m_oStyle.tcPr->Shading.is_init())
						oShd	=	GetBackGroundShading (m_oStyle.tcPr->Shading);

					bAppend		=	TRUE;
				}

				// контент (вертикальный набор)

				if (0 == m_bNoVBand)
				{
					if (nX > 0 || (0 == m_bHaveBeginCol))
					{
						BOOL bEven	=	(nX / nBandCol) % 2;
						if (m_bHaveBeginRow)
							bEven	=	((nX-1) / nBandCol) % 2;	//	смещаем, если доступен столбец заголовка

						if (0==bEven)
						{
							if (UpdateShading(std::string("band1Vert"), oShd))
								bAppend	=	TRUE;
						}
						else
						{
							if (UpdateShading(std::string("band2Vert"), oShd))
								bAppend	=	TRUE;
						}
					}
				}

				// контент (горизонтальный набор)

				if (0 == m_bNoHBand)
				{
					if (nY > 0 || (0 == m_bHaveBeginRow))
					{
						BOOL bEven	=	(nY / nRowBand) % 2;
						if (m_bHaveBeginRow)
							bEven	=	((nY-1) / nRowBand) % 2;	//	смещаем, если доступна строка заголовка

						if (0==bEven)
						{
							if (UpdateShading(std::string("band1Horz"), oShd))
								bAppend	=	TRUE;
						}
						else
						{
							if (UpdateShading(std::string("band2Horz"), oShd))
								bAppend	=	TRUE;
						}
					}
				}

				// первый столбец

				if (m_bHaveBeginCol)
				{
					if (0 == nX)
					{
						if (UpdateShading(std::string("firstCol"), oShd))
							bAppend	=	TRUE;
					}
				}

				// последний столбец

				if (m_bHaveEndCol)
				{
					if (nX == nCX)
					{
						if (UpdateShading(std::string("lastCol"), oShd))
							bAppend	=	TRUE;
					}
				}

				// перва€ строка

				if (m_bHaveBeginRow)
				{
					if (0 == nY)
					{
						if (UpdateShading(std::string("firstRow"), oShd))
							bAppend	=	TRUE;
					}	
				}

				// последн€€ строка

				if (m_bHaveEndRow)
				{
					if (nY == nCY)
					{
						if (UpdateShading(std::string("lastRow"), oShd))
							bAppend	=	TRUE;
					}
				}

				// дополнительно можем форматировать угловые элементы

				if (UpdateOneCellFormat(nX, nY, nCX, nCY, oShd))					
					bAppend	=	TRUE;					
			}

			if (bAppend)
			{
				if (nX < 22)
					m_arShd22.push_back(oShd);
				else if (nX < 44)
					m_arShd44.push_back(oShd);
				else if (nX < 63)
					m_arShd63.push_back(oShd);   
			}
			else
			{
				if (nX < 22)
					m_arShd22.push_back(ASCDocFileFormat::Shd());
				else if (nX < 44)
					m_arShd44.push_back(ASCDocFileFormat::Shd());
				else if (nX < 63)
					m_arShd63.push_back(ASCDocFileFormat::Shd());   
			}


			return TRUE;
		}

		inline BOOL UpdateShading (const std::string& strStyle, ASCDocFileFormat::Shd& oShd)
		{
			if (m_oStyle.tableStyles->exist(strStyle))
			{
				const OOX::Logic::TableStyleProperties& oInternal = m_oStyle.tableStyles->operator [](strStyle);
				if (oInternal.tcPr.is_init() && oInternal.tcPr->Shading.is_init())
				{
					oShd = GetBackGroundShading (oInternal.tcPr->Shading);

					return TRUE;
				}
			}

			return FALSE;
		}


		//
		inline const ShdList& GetShd22 () 
		{
			return m_arShd22;
		}

		inline const ShdList& GetShd44 ()
		{
			return m_arShd44;
		}

		inline const ShdList& GetShd63 ()
		{
			return m_arShd63;
		}

	private:

		inline BOOL UpdateOneCellFormat (int nX, int nY, int nCX, int nCY, ASCDocFileFormat::Shd& oShd)
		{
			if ((0 == nX) && (0 == nY))
			{
				if (UpdateShading(std::string("nwCell"), oShd))
					return TRUE;
			}
		
			if ((nCX == nX) && (0 == nY))
			{
				if (UpdateShading(std::string("neCell"), oShd))
					return TRUE;
			}
		
			if ((0 == nX) && (nCY == nY))
			{
				if (UpdateShading(std::string("swCell"), oShd))
					return TRUE;
			}
		
			if ((nCX == nX) && (nCY == nY))
			{
				if (UpdateShading(std::string("seCell"), oShd))
					return TRUE;
			}

			return FALSE;
		}

	private:

		OOX::Styles::Style	m_oStyle;

		BOOL	m_bHaveBeginRow;
		BOOL	m_bHaveBeginCol;
		BOOL	m_bHaveEndRow;
		BOOL	m_bHaveEndCol;

		BOOL	m_bNoHBand;
		BOOL	m_bNoVBand;

		int		m_nIndCellX;
		int		m_nIndCellY;

		int		m_nCellsX;
		int		m_nCellsY;

		ShdList	m_arShd22;
		ShdList m_arShd44;
		ShdList m_arShd63;
	};
}