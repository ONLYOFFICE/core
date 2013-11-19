#pragma once
#include "XmlUtils.h"
#include "RtfDocument.h"
#include "RtfProperty.h"
#include "OOXReaderBasic.h"

class OOXBorderReader
{
public: OOXBorderReader()
		{
		}
public: bool Parse( ReaderParameter oParam, RtfBorder& oOutputBorder )
		{	
			XmlUtils::CXmlReader oXmlReader;
			if( TRUE == oXmlReader.OpenFromXmlNode( oParam.oNode ) )
			{
				CString sXml = oXmlReader.ReadNodeXml();
				RtfColor oBorderColor;
				CString sColor;
				sColor = oXmlReader.ReadNodeAttribute( _T("w:color") );
				CharLower( sColor.GetBuffer() );
				sColor.ReleaseBuffer();
				
				if( _T("") != sColor && _T("auto") != sColor && sColor.GetLength() == 6 )
				{
					RtfColor oColor;
					oColor.SetHEXString( sColor );
					oOutputBorder.m_nColor = oParam.oDocument->m_oColorTable.AddItem( oColor );
				}
				int nSpacePoint = PROP_DEF;
				COMMAND_OOX_INT_ATTRIBUTE_2( _T("w:space"), nSpacePoint, oXmlReader );
				if( PROP_DEF != nSpacePoint )
					oOutputBorder.m_nSpace = RtfUtility::pt2Twip( nSpacePoint );
				int nSize = PROP_DEF;
				COMMAND_OOX_INT_ATTRIBUTE_2( _T("w:sz"), nSize, oXmlReader )
				if( PROP_DEF != nSize )//w:sz  1/8 twips (equivalent to 1/576th of an inch)
					oOutputBorder.m_nWidth = 5 * nSize / 2;
				CString sType = oXmlReader.ReadNodeAttribute( _T("w:val") );
				if( _T("") != sType )
				{
					if( _T("single") == sType )
						oOutputBorder.m_eType = RtfBorder::bt_brdrs;
					else if( _T("thick") == sType )
						oOutputBorder.m_eType = RtfBorder::bt_brdrth;
					else if( _T("thin") == sType )
						oOutputBorder.m_eType = RtfBorder::bt_brdrsh;
					else if( _T("double") == sType )
						oOutputBorder.m_eType = RtfBorder::bt_brdrdb;
					else if( _T("dotted") == sType )
						oOutputBorder.m_eType = RtfBorder::bt_brdrdot;
					else if( _T("dashed") == sType )
						oOutputBorder.m_eType = RtfBorder::bt_brdrdash;
					else if( _T("hair") == sType )
						oOutputBorder.m_eType = RtfBorder::bt_brdrhair;
					else if( _T("dashSmallGap") == sType )
						oOutputBorder.m_eType = RtfBorder::bt_brdrdashsm;
					else if( _T("dotDash") == sType )
						oOutputBorder.m_eType = RtfBorder::bt_brdrdashd;
					else if( _T("dotDotDash") == sType )
						oOutputBorder.m_eType = RtfBorder::bt_brdrdashdd;
					else if( _T("inset") == sType )
						oOutputBorder.m_eType = RtfBorder::bt_brdrinset;
					else if( _T("none") == sType )
						oOutputBorder.m_eType = RtfBorder::bt_brdrnone;
					else if( _T("nil") == sType )
						oOutputBorder.m_eType = RtfBorder::bt_brdrnone;
					else if( _T("outset") == sType )
						oOutputBorder.m_eType = RtfBorder::bt_brdroutset;
					else if( _T("triple") == sType )
						oOutputBorder.m_eType = RtfBorder::bt_brdrtriple;
					else if( _T("thinThickSmallGap") == sType )
						oOutputBorder.m_eType = RtfBorder::bt_brdrtnthsg;
					else if( _T("thickThinSmallGap") == sType )
						oOutputBorder.m_eType = RtfBorder::bt_brdrthtnsg;
					else if( _T("thinThickThinSmallGap") == sType )
						oOutputBorder.m_eType = RtfBorder::bt_brdrtnthtnsg;
					else if( _T("thinThickThinMediumGap") == sType )
						oOutputBorder.m_eType = RtfBorder::bt_brdrtnthtnmg;
					else if( _T("thinThickMediumGap") == sType )
						oOutputBorder.m_eType = RtfBorder::bt_brdrtnthmg;
					else if( _T("thickThinMediumGap") == sType )
						oOutputBorder.m_eType = RtfBorder::bt_brdrthtnmg;
					else if( _T("thinThickLargeGap") == sType )
						oOutputBorder.m_eType = RtfBorder::bt_brdrtnthlg;
					else if( _T("thickThinLargeGap") == sType )
						oOutputBorder.m_eType = RtfBorder::bt_brdrthtnlg;
					else if( _T("thinThickThinLargeGap") == sType )
						oOutputBorder.m_eType = RtfBorder::bt_brdrtnthtnlg;
					else if( _T("wave") == sType )
						oOutputBorder.m_eType = RtfBorder::bt_brdrwavy;
					else if( _T("doubleWave") == sType )
						oOutputBorder.m_eType = RtfBorder::bt_brdrwavydb;
					else if( _T("dashDotStroked") == sType )
						oOutputBorder.m_eType = RtfBorder::bt_brdrdashdotstr;
					else if( _T("threeDEmboss") == sType )
						oOutputBorder.m_eType = RtfBorder::bt_brdremboss;
					else if( _T("threeDEngrave") == sType )
						oOutputBorder.m_eType = RtfBorder::bt_brdrengrave;
				}
			}
			return true;
		}
};
