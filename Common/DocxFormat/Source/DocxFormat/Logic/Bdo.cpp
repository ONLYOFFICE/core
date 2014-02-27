#include "Bdo.h"
#include "Paragraph.h"
#include "Annotations.h"
#include "Run.h"
#include "FldSimple.h"
#include "Sdt.h"
#include "Hyperlink.h"
#include "SmartTag.h"
#include "Dir.h"
#include "../Math/oMathPara.h"
#include "../Math/oMath.h"

// TO DO: Нехватающие классы:
//        <w:customXml>
//        <w:moveFrom>
//        <w:moveTo>
//        <w:subDoc>


namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// CBdo 17.3.1.22 (Part 1)
		//--------------------------------------------------------------------------------	

		void    CBdo::fromXML(XmlUtils::CXmlNode& oNode)
		{
			oNode.ReadAttributeBase( _T("w:val"), m_oVal );

			XmlUtils::CXmlNodes oChilds;
			if ( oNode.GetNodes( _T("*"), oChilds ) )
			{
				XmlUtils::CXmlNode oItem;
				for ( int nIndex = 0; nIndex < oChilds.GetCount(); nIndex++ )
				{
					if ( oChilds.GetAt( nIndex, oItem ) )
					{
						CString sName = oItem.GetName();
						WritingElement *pItem = NULL;

						if ( _T("w:bdo") == sName )
							pItem = new CBdo( oItem );
						else if ( _T("w:bookmarkEnd") == sName )
							pItem = new CBookmarkEnd( oItem );
						else if ( _T("w:bookmarkStart") == sName )
							pItem = new CBookmarkStart( oItem );
						else if ( _T("w:commentRangeEnd") == sName )
							pItem = new CCommentRangeEnd( oItem );
						else if ( _T("w:commentRangeStart") == sName )
							pItem = new CCommentRangeStart( oItem );
						//else if ( _T("w:customXml") == sName )
						//	pItem = new CCustomXml( oItem );
						else if ( _T("w:customXmlDelRangeEnd") == sName )
							pItem = new CCustomXmlDelRangeEnd( oItem );
						else if ( _T("w:customXmlDelRangeStart") == sName )
							pItem = new CCustomXmlDelRangeStart( oItem );
						else if ( _T("w:customXmlInsRangeEnd") == sName )
							pItem = new CCustomXmlInsRangeEnd( oItem );
						else if ( _T("w:customXmlInsRangeStart") == sName )
							pItem = new CCustomXmlInsRangeStart( oItem );
						else if ( _T("w:customXmlMoveFromRangeEnd") == sName ) 
							pItem = new CCustomXmlMoveFromRangeEnd( oItem );
						else if ( _T("w:customXmlMoveFromRangeStart") == sName )
							pItem = new CCustomXmlMoveFromRangeStart( oItem );
						else if ( _T("w:customXmlMoveToRangeEnd") == sName ) 
							pItem = new CCustomXmlMoveToRangeEnd( oItem );
						else if ( _T("w:customXmlMoveToRangeStart") == sName )
							pItem = new CCustomXmlMoveToRangeStart( oItem );
						//else if ( _T("w:del") == sName )
						//	pItem = new CDel( oItem );
						else if ( _T("w:dir") == sName )
							pItem = new CDir( oItem );
						else if ( _T("w:fldSimple") == sName )
							pItem = new CFldSimple( oItem );
						else if ( _T("w:hyperlink") == sName )
							pItem = new CHyperlink( oItem );
						//else if ( _T("w:ins") == sName )
						//	pItem = new CIns( oItem );
						//else if ( _T("w:moveFrom") == sName )
						//	pItem = new CMoveFrom( oItem );
						else if ( _T("w:moveFromRangeEnd") == sName )
							pItem = new CMoveToRangeEnd( oItem );
						else if ( _T("w:moveFromRangeStart") == sName )
							pItem = new CMoveToRangeStart( oItem );
						//else if ( _T("w:moveTo") == sName )
						//	pItem = new CMoveTo( oItem );
						else if ( _T("w:moveToRangeEnd") == sName )
							pItem = new CMoveToRangeEnd( oItem );
						else if ( _T("w:moveToRangeStart") == sName )
							pItem = new CMoveToRangeStart( oItem );
						else if ( _T("m:oMath") == sName )
							pItem = new COMath( oItem );
						else if ( _T("m:oMathPara") == sName )
							pItem = new COMathPara( oItem );
						else if ( _T("w:permEnd") == sName )
							pItem = new CPermEnd( oItem );
						else if ( _T("w:permStart") == sName )
							pItem = new CPermStart( oItem );
						else if ( _T("w:proofErr") == sName )
							pItem = new CProofErr( oItem );
						else if ( _T("w:r") == sName )
							pItem = new CRun( oItem );
						else if ( _T("w:sdt") == sName )
							pItem = new CSdt( oItem );
						else if ( _T("w:smartTag") == sName )
							pItem = new CSmartTag( oItem );
						//else if ( _T("w:subDoc") == sName )
						//	pItem = new CSubDoc( oItem );

						if ( pItem )
							m_arrItems.Add( pItem );
					}
				}
			}
		}


		void    CBdo::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				CWCharWrapper sName = oReader.GetName();
				WritingElement *pItem = NULL;

				if ( _T("w:bdo") == sName )
					pItem = new CBdo( oReader );
				else if ( _T("w:bookmarkEnd") == sName )
					pItem = new CBookmarkEnd( oReader );
				else if ( _T("w:bookmarkStart") == sName )
					pItem = new CBookmarkStart( oReader );
				else if ( _T("w:commentRangeEnd") == sName )
					pItem = new CCommentRangeEnd( oReader );
				else if ( _T("w:commentRangeStart") == sName )
					pItem = new CCommentRangeStart( oReader );
				//else if ( _T("w:customXml") == sName )
				//	pItem = new CCustomXml( oReader );
				else if ( _T("w:customXmlDelRangeEnd") == sName )
					pItem = new CCustomXmlDelRangeEnd( oReader );
				else if ( _T("w:customXmlDelRangeStart") == sName )
					pItem = new CCustomXmlDelRangeStart( oReader );
				else if ( _T("w:customXmlInsRangeEnd") == sName )
					pItem = new CCustomXmlInsRangeEnd( oReader );
				else if ( _T("w:customXmlInsRangeStart") == sName )
					pItem = new CCustomXmlInsRangeStart( oReader );
				else if ( _T("w:customXmlMoveFromRangeEnd") == sName ) 
					pItem = new CCustomXmlMoveFromRangeEnd( oReader );
				else if ( _T("w:customXmlMoveFromRangeStart") == sName )
					pItem = new CCustomXmlMoveFromRangeStart( oReader );
				else if ( _T("w:customXmlMoveToRangeEnd") == sName ) 
					pItem = new CCustomXmlMoveToRangeEnd( oReader );
				else if ( _T("w:customXmlMoveToRangeStart") == sName )
					pItem = new CCustomXmlMoveToRangeStart( oReader );
				else if ( _T("w:del") == sName )
					pItem = new CDel( oReader );
				else if ( _T("w:dir") == sName )
					pItem = new CDir( oReader );
				else if ( _T("w:fldSimple") == sName )
					pItem = new CFldSimple( oReader );
				else if ( _T("w:hyperlink") == sName )
					pItem = new CHyperlink( oReader );
				else if ( _T("w:ins") == sName )
					pItem = new CIns( oReader );
				//else if ( _T("w:moveFrom") == sName )
				//	pItem = new CMoveFrom( oReader );
				else if ( _T("w:moveFromRangeEnd") == sName )
					pItem = new CMoveToRangeEnd( oReader );
				else if ( _T("w:moveFromRangeStart") == sName )
					pItem = new CMoveToRangeStart( oReader );
				//else if ( _T("w:moveTo") == sName )
				//	pItem = new CMoveTo( oReader );
				else if ( _T("w:moveToRangeEnd") == sName )
					pItem = new CMoveToRangeEnd( oReader );
				else if ( _T("w:moveToRangeStart") == sName )
					pItem = new CMoveToRangeStart( oReader );
				else if ( _T("m:oMath") == sName )
					pItem = new COMath( oReader );
				else if ( _T("m:oMathPara") == sName )
					pItem = new COMathPara( oReader );
				else if ( _T("w:permEnd") == sName )
					pItem = new CPermEnd( oReader );
				else if ( _T("w:permStart") == sName )
					pItem = new CPermStart( oReader );
				else if ( _T("w:proofErr") == sName )
					pItem = new CProofErr( oReader );
				else if ( _T("w:r") == sName )
					pItem = new CRun( oReader );
				else if ( _T("w:sdt") == sName )
					pItem = new CSdt( oReader );
				else if ( _T("w:smartTag") == sName )
					pItem = new CSmartTag( oReader );
				//else if ( _T("w:subDoc") == sName )
				//	pItem = new CSubDoc( oReader );

				if ( pItem )
					m_arrItems.Add( pItem );
			}
		}


		CString CBdo::toXML() const
		{
				CString sResult = _T("<w:bdo ");

				sResult += _T("w:val=\"");
				sResult += m_oVal.ToString();
				sResult += _T("\">");

				for ( int nIndex = 0; nIndex < m_arrItems.GetSize(); nIndex++ )
				{
					if ( m_arrItems[nIndex] )
					{
						sResult += m_arrItems[nIndex]->toXML();
					}
				}

				sResult += _T("</w:bdo>");

				return sResult;
		}

	} // namespace Logic
} // namespace OOX