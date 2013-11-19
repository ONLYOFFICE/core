#pragma once
#include "OOXReaderBasic.h"
#include "RtfProperty.h"

class OOXpPrFrameReader
{
public: bool Parse( ReaderParameter oParam ,RtfFrame& oOutputProperty)
		{
			XmlUtils::CXmlReader oXmlReader;
			oXmlReader.OpenFromXmlNode(oParam.oNode);

			COMMAND_OOX_INT_ATTRIBUTE_2( _T("w:w"), oOutputProperty.m_nWidth, oXmlReader )
			COMMAND_OOX_INT_ATTRIBUTE_2( _T("w:h"), oOutputProperty.m_nHeight, oXmlReader )

			CString sHRef = oXmlReader.ReadNodeAttribute( _T("w:hAnchor")  );
			if( _T("margin") == sHRef )
				oOutputProperty.m_eHRef = RtfFrame::hr_phmrg;
			else if( _T("page") == sHRef )
				oOutputProperty.m_eHRef = RtfFrame::hr_phpg;
			else if( _T("text") == sHRef )
				oOutputProperty.m_eHRef = RtfFrame::hr_phcol;
			COMMAND_OOX_INT_ATTRIBUTE_2( _T("w:x"), oOutputProperty.m_nHPos, oXmlReader )
			CString sHPos = oXmlReader.ReadNodeAttribute( _T("w:xAlign") );
			if( _T("center") == sHPos )
				oOutputProperty.m_eHPos = RtfFrame::hp_posxc;
			else if( _T("inside") == sHPos )
				oOutputProperty.m_eHPos = RtfFrame::hp_posxi;
			else if( _T("outside") == sHPos )
				oOutputProperty.m_eHPos = RtfFrame::hp_posxo;
			else if( _T("left") == sHPos )
				oOutputProperty.m_eHPos = RtfFrame::hp_posxl;
			else if( _T("right") == sHPos )
				oOutputProperty.m_eHPos = RtfFrame::hp_posxr;

			CString sVRef = oXmlReader.ReadNodeAttribute( _T("w:vAnchor") );
			if( _T("margin") == sVRef )
				oOutputProperty.m_eVRef = RtfFrame::vr_pvmrg;
			else if( _T("page") == sVRef )
				oOutputProperty.m_eVRef = RtfFrame::vr_pvpg;
			else if( _T("text") == sVRef )
				oOutputProperty.m_eVRef = RtfFrame::vr_pvpara;
			COMMAND_OOX_INT_ATTRIBUTE_2( _T("w:y"), oOutputProperty.m_nVPos, oXmlReader )
			CString sVPos = oXmlReader.ReadNodeAttribute( _T("w:yAlign") );
			if( _T("top") == sVPos )
				oOutputProperty.m_eVPos = RtfFrame::vp_posyt;
			else if( _T("inline") == sVPos )
				oOutputProperty.m_eVPos = RtfFrame::vp_posyil;
			else if( _T("bottom") == sVPos )
				oOutputProperty.m_eVPos = RtfFrame::vp_posyb;
			else if( _T("center") == sVPos )
				oOutputProperty.m_eVPos = RtfFrame::vp_posyc;
			else if( _T("inside") == sVPos )
				oOutputProperty.m_eVPos = RtfFrame::vp_posyin;
			else if( _T("outside") == sVPos )
				oOutputProperty.m_eVPos = RtfFrame::vp_posyout;

			COMMAND_OOX_BOOL_ATTRIBUTE_2( _T("w:anchorLock"), oOutputProperty.m_bLockAnchor, oXmlReader )
			CString sWrap = oXmlReader.ReadNodeAttribute( _T("w:wrap") );
			if( _T("auto") == sWrap )
				oOutputProperty.m_eWrap = RtfFrame::tw_wrapdefault;
			else if( _T("around") == sWrap )
				oOutputProperty.m_eWrap = RtfFrame::tw_wraparound;
			else if( _T("tight") == sWrap )
				oOutputProperty.m_eWrap = RtfFrame::tw_wraptight;
			else if( _T("through") == sWrap )
				oOutputProperty.m_eWrap = RtfFrame::tw_wrapthrough;

			CString sDropCap = oXmlReader.ReadNodeAttribute( _T("w:dropCap") );
			if( _T("drop") == sDropCap )
				oOutputProperty.m_DropcapType = 1;
			else if( _T("margin") == sDropCap )
				oOutputProperty.m_DropcapType = 2;

			COMMAND_OOX_BOOL_ATTRIBUTE_2( _T("w:lines"), oOutputProperty.m_DropcapLines, oXmlReader )

			CString sHSpace = oXmlReader.ReadNodeAttribute( _T("w:hSpace") );
			if( _T("") != sHSpace )
			{
				oOutputProperty.m_nHorSpace = Strings::ToInteger( sHSpace );
				if( PROP_DEF == oOutputProperty.m_nAllSpace || oOutputProperty.m_nAllSpace < oOutputProperty.m_nHorSpace )
				{
					oOutputProperty.m_nAllSpace = oOutputProperty.m_nHorSpace;
				}
			}
			CString sVSpace = oXmlReader.ReadNodeAttribute( _T("w:vSpace") );
			if( _T("") != sVSpace )
			{
				oOutputProperty.m_nVerSpace = Strings::ToInteger( sVSpace );
				if( PROP_DEF == oOutputProperty.m_nAllSpace || oOutputProperty.m_nAllSpace < oOutputProperty.m_nVerSpace )
				{
					oOutputProperty.m_nAllSpace = oOutputProperty.m_nVerSpace;
				}
			}

			return true;
		}
};
