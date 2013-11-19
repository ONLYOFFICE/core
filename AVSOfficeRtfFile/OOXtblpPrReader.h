#pragma once
#include "OOXReader.h"
#include "RtfDocument.h"

	class OOXtblpPrReader
	{
	public: OOXtblpPrReader()
			{
			}
	public: bool Parse( ReaderParameter oParam , RtfTableProperty& oOutputProperty)
			{
				XmlUtils::CXmlReader oXmlReader;
				if( TRUE ==  oXmlReader.OpenFromXmlNode(oParam.oNode) )
				{
					CString sLeftWrap = oXmlReader.ReadNodeAttribute( _T("w:leftFromText") );
					CString sRightWrap = oXmlReader.ReadNodeAttribute( _T("w:rightFromText") );
					CString sTopWrap = oXmlReader.ReadNodeAttribute( _T("w:topFromText") );
					CString sBottomWrap = oXmlReader.ReadNodeAttribute( _T("w:bottomFromText") );

					CString sHorAhchor = oXmlReader.ReadNodeAttribute( _T("w:horzAnchor") );
					CString sX = oXmlReader.ReadNodeAttribute( _T("w:tblpX") );
					CString sXSpec = oXmlReader.ReadNodeAttribute( _T("w:tblpXSpec") );

					CString sVerAhchor = oXmlReader.ReadNodeAttribute( _T("w:vertAnchor"));
					CString sY = oXmlReader.ReadNodeAttribute( _T("w:tblpY") );
					CString sYSpec = oXmlReader.ReadNodeAttribute( _T("w:tblpYSpec"));

					if( _T("") != sLeftWrap )
						oOutputProperty.m_nWrapLeft = Strings::ToInteger( sLeftWrap );
					if( _T("") != sRightWrap )
						oOutputProperty.m_nWrapRight = Strings::ToInteger( sRightWrap );
					if( _T("") != sTopWrap )
						oOutputProperty.m_nWrapTop = Strings::ToInteger( sTopWrap );
					if( _T("") != sBottomWrap )
						oOutputProperty.m_nWrapBottom = Strings::ToInteger( sBottomWrap );
					if( _T("") != sX )
						oOutputProperty.m_nHPos = Strings::ToInteger( sX );
					if( _T("") != sY )
						oOutputProperty.m_nVPos = Strings::ToInteger( sY );

					if( _T("margin") == sHorAhchor )
						oOutputProperty.m_eHRef = RtfTableProperty::hr_phmrg;
					else if( _T("page") == sHorAhchor )
						oOutputProperty.m_eHRef = RtfTableProperty::hr_phpg;
					else if( _T("text") == sHorAhchor )
						oOutputProperty.m_eHRef = RtfTableProperty::hr_phcol;

					if( _T("center") == sXSpec )
						oOutputProperty.m_eHPos = RtfTableProperty::hp_posxc;
					else if( _T("inside") == sXSpec )
						oOutputProperty.m_eHPos = RtfTableProperty::hp_posxi;
					else if( _T("outside") == sXSpec )
						oOutputProperty.m_eHPos = RtfTableProperty::hp_posxo;
					else if( _T("left") == sXSpec )
						oOutputProperty.m_eHPos = RtfTableProperty::hp_posxl;
					else if( _T("right") == sXSpec )
						oOutputProperty.m_eHPos = RtfTableProperty::hp_posxr;

					if( _T("margin") == sVerAhchor )
						oOutputProperty.m_eVRef = RtfTableProperty::vr_pvmrg;
					else if( _T("page") == sVerAhchor )
						oOutputProperty.m_eVRef = RtfTableProperty::vr_pvpg;
					else if( _T("text") == sVerAhchor )
						oOutputProperty.m_eVRef = RtfTableProperty::vr_pvpara;

					if( _T("center") == sYSpec )
						oOutputProperty.m_eVPos = RtfTableProperty::vp_posyc;
					else if( _T("inside") == sYSpec )
						oOutputProperty.m_eVPos = RtfTableProperty::vp_posyin;
					else if( _T("outside") == sYSpec )
						oOutputProperty.m_eVPos = RtfTableProperty::vp_posyout;
					else if( _T("top") == sYSpec )
						oOutputProperty.m_eVPos = RtfTableProperty::vp_posyt;
					else if( _T("bottom") == sYSpec )
						oOutputProperty.m_eVPos = RtfTableProperty::vp_posyb;
					}
				return false;
			}
	};
