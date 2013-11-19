#pragma once
#include "RtfDocument.h"
#include "XmlUtils.h"
#include "RtfProperty.h"


	class OOXLatentStyleReader
	{
	public: OOXLatentStyleReader()
			{
			}
	public: bool Parse( ReaderParameter oParam )
			{
				XmlUtils::CXmlReader oXmlReader;
				if( TRUE ==  oXmlReader.OpenFromXmlNode(oParam.oNode) )
				{
					CString nCount = oXmlReader.ReadNodeAttribute( _T("w:count") );
					if( _T("") != nCount )
						oParam.oDocument->m_oLatentStyleTable.m_nCount = Strings::ToInteger( nCount );
					CString nLocked = oXmlReader.ReadNodeAttribute( _T("w:defLockedState") );
					if( _T("") != nLocked )
						oParam.oDocument->m_oLatentStyleTable.m_nLocked= Strings::ToInteger( nLocked );
					CString nSemiHid = oXmlReader.ReadNodeAttribute( _T("w:defSemiHidden") );
					if( _T("") != nSemiHid )
						oParam.oDocument->m_oLatentStyleTable.m_nSemiHidden= Strings::ToInteger( nSemiHid );
					CString nUNhidWU = oXmlReader.ReadNodeAttribute( _T("w:defUnhideWhenUsed") );
					if( _T("") != nUNhidWU )
						oParam.oDocument->m_oLatentStyleTable.m_nUnHiddenWhenUse= Strings::ToInteger( nUNhidWU );
					CString nQFormat = oXmlReader.ReadNodeAttribute( _T("w:defQFormat") );
					if( _T("") != nQFormat )
						oParam.oDocument->m_oLatentStyleTable.m_nQFormat= Strings::ToInteger( nQFormat );
					CString nUIPriority = oXmlReader.ReadNodeAttribute( _T("w:defUIPriority") );
					if( _T("") != nUIPriority )
						oParam.oDocument->m_oLatentStyleTable.m_nPriority= Strings::ToInteger( nUIPriority );

					oXmlReader.ReadNodeList( _T("w:lsdException") );
					for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
					{
						RtfStyleException oNewException;
						oNewException.m_sName = oXmlReader.ReadNodeAttribute( _T("w:name"), _T("") );
						CString nPriority = oXmlReader.ReadNodeAttribute( _T("w:uiPriority"));
						if( _T("") != nPriority )
							oNewException.m_nPriority = Strings::ToInteger( nPriority );
						CString nHiddenWhenUse = oXmlReader.ReadNodeAttribute( _T("w:unhideWhenUsed") );
						if( _T("") != nHiddenWhenUse )
							oNewException.m_nHiddenWhenUse = Strings::ToInteger( nHiddenWhenUse );
						CString nQFormat = oXmlReader.ReadNodeAttribute( _T("w:qFormat") );
						if( _T("") != nQFormat )
							oNewException.m_nQFormat = Strings::ToInteger( nQFormat );
						CString nLocked = oXmlReader.ReadNodeAttribute( _T("w:locked") );
						if( _T("") != nLocked )
							oNewException.m_nLocked = Strings::ToInteger( nLocked );
						CString nSemiHidden = oXmlReader.ReadNodeAttribute( _T("w:semiHidden") );
						if( _T("") != nSemiHidden )
							oNewException.m_nSemiHidden = Strings::ToInteger( nSemiHidden );

						if( true == oNewException.IsValid() )
							oParam.oDocument->m_oLatentStyleTable.AddItem( oNewException );
					}

				}
				return false;
			}

	};
