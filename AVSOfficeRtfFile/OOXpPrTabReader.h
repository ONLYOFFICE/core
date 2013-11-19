#pragma once
#include "OOXReaderBasic.h"
#include "RtfProperty.h"

class OOXpPrTabReader
{
public: bool Parse( ReaderParameter oParam ,RtfTabs& oOutputProperty)
		{
			oOutputProperty.SetDefault();

			XmlUtils::CXmlReader oXmlReader;
			if( TRUE == oXmlReader.OpenFromXmlNode(oParam.oNode) )
				if( TRUE == oXmlReader.ReadNodeList( _T("*") ) )
				{
					for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
					{
						RtfTab oCurTab;

						CString sLeader = oXmlReader.ReadNodeAttribute( i, _T("w:leader"), _T("") );
						if( _T("dot") == sLeader )
							oCurTab.m_eLeader = RtfTab::tl_dot;
						else if( _T("middleDot") == sLeader )
							oCurTab.m_eLeader = RtfTab::tl_mdot;
						else if( _T("hyphen") == sLeader )
							oCurTab.m_eLeader = RtfTab::tl_hyph;
						else if( _T("underscore") == sLeader )
							oCurTab.m_eLeader = RtfTab::tl_ul;

						CString sKind = oXmlReader.ReadNodeAttribute( i, _T("w:val"), _T("") );
						if( _T("left") == sKind )
							oCurTab.m_eKind = RtfTab::tk_tql;
						else if( _T("right") == sKind )
							oCurTab.m_eKind = RtfTab::tk_tqr;
						else if( _T("start") == sKind )
							oCurTab.m_eKind = RtfTab::tk_tql;
						else if( _T("end") == sKind )
							oCurTab.m_eKind = RtfTab::tk_tqr;
						else if( _T("center") == sKind )
							oCurTab.m_eKind = RtfTab::tk_tqc;
						else if( _T("decimal") == sKind )
							oCurTab.m_eKind = RtfTab::tk_tqdec;
						else if( _T("bar") == sKind )
							oCurTab.m_eKind = RtfTab::tk_tqbar;

						COMMAND_OOX_INT_ATTRIBUTE( _T("w:pos"), oCurTab.m_nTab, oXmlReader, i );
						
						oOutputProperty.m_aTabs.Add( oCurTab );
					}
				}
			return true;
		}
};
