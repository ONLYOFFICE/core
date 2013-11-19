#include "oMathPara.h"

// TO DO: Нехватающие классы:
//        <w:del>
//        <w:ins>
//        <w:moveFrom>
//        <w:moveTo>

namespace OOX
{	
	namespace Logic
	{
		void    CMathArgNodes::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			sNodeName = oReader.GetName();
			if ( oReader.IsEmptyNode() && (sNodeName != _T("m:e")))
				return;
			
			if(sNodeName == _T("m:deg"))
					eType = et_m_deg;
				else if (sNodeName == _T("m:den"))
					eType = et_m_den;
				else if (sNodeName == _T("m:e"))
					eType = et_m_e;
				else if (sNodeName == _T("m:fName"))
					eType = et_m_fName;
				else if (sNodeName == _T("m:lim"))
					eType = et_m_lim;
				else if (sNodeName == _T("m:num"))
					eType = et_m_num;
				else if (sNodeName == _T("m:oMath"))
					eType = et_m_oMath;
				else if (sNodeName == _T("m:sub"))
					eType = et_m_sub;
				else if (sNodeName == _T("m:sup"))
					eType = et_m_sup;
				else
					eType = et_Unknown;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				CWCharWrapper sName = oReader.GetName();
				WritingElement *pItem = NULL;

				if ( _T("m:acc") == sName )
					pItem = new OOX::Logic::CAcc( oReader );
				else if ( _T("m:argPr") == sName )
					pItem = new OOX::Logic::CArgPr( oReader );
				else if ( _T("m:bar") == sName )
					pItem = new OOX::Logic::CBar( oReader );
				else if ( _T("w:bookmarkEnd") == sName )
					pItem = new OOX::Logic::CBookmarkEnd( oReader );
				else if ( _T("w:bookmarkStart") == sName )
					pItem = new OOX::Logic::CBookmarkStart( oReader );
				else if ( _T("m:borderBox") == sName )
					pItem = new OOX::Logic::CBorderBox( oReader );
				else if ( _T("m:box") == sName )
					pItem = new OOX::Logic::CBox( oReader );
				else if ( _T("w:commentRangeEnd") == sName )
					pItem = new OOX::Logic::CCommentRangeEnd( oReader );
				else if ( _T("w:commentRangeStart") == sName )
					pItem = new OOX::Logic::CCommentRangeStart( oReader );
				else if ( _T("m:ctrlPr") == sName )
					pItem = new OOX::Logic::CCtrlPr( oReader );
				else if ( _T("w:customXmlDelRangeEnd") == sName ) 
					pItem = new OOX::Logic::CCustomXmlDelRangeEnd( oReader );
				else if ( _T("w:customXmlDelRangeStart") == sName )
					pItem = new OOX::Logic::CCustomXmlDelRangeStart( oReader );
				else if ( _T("w:customXmlInsRangeEnd") == sName )
					pItem = new OOX::Logic::CCustomXmlInsRangeEnd( oReader );
				else if ( _T("w:customXmlInsRangeStart") == sName )
					pItem = new OOX::Logic::CCustomXmlInsRangeStart( oReader );
				else if ( _T("w:customXmlMoveFromRangeEnd") == sName )
					pItem = new OOX::Logic::CCustomXmlMoveFromRangeEnd( oReader );
				else if ( _T("w:customXmlMoveFromRangeStart") == sName )
					pItem = new OOX::Logic::CCustomXmlMoveFromRangeStart( oReader );
				else if ( _T("w:customXmlMoveToRangeEnd") == sName )
					pItem = new OOX::Logic::CCustomXmlMoveToRangeEnd( oReader );
				else if ( _T("w:customXmlMoveToRangeStart") == sName )
					pItem = new OOX::Logic::CCustomXmlMoveToRangeStart( oReader );
				else if ( _T("m:d") == sName )
					pItem = new OOX::Logic::CDelimiter( oReader );
				//else if ( _T("w:del") == sName )
				//	pItem = new CDel( oReader );
				else if ( _T("m:eqArr") == sName )
					pItem = new OOX::Logic::CEqArr( oReader );
				else if ( _T("m:f") == sName )
					pItem = new OOX::Logic::CFraction( oReader );
				else if ( _T("m:func") == sName )
					pItem = new OOX::Logic::CFunc( oReader );
				else if ( _T("m:groupChr") == sName )
					pItem = new CGroupChr( oReader );
				//else if ( _T("w:ins") == sName )
				//	pItem = new CIns( oReader );
				else if ( _T("m:limLow") == sName )
					pItem = new OOX::Logic::CLimLow( oReader );
				else if ( _T("m:limUpp") == sName )
					pItem = new OOX::Logic::CLimUpp( oReader );
				else if ( _T("m:m") == sName )
					pItem = new OOX::Logic::CMatrix( oReader );
				//else if ( _T("w:moveFrom") == sName )
				//	pItem = new OOX::Logic::CMoveFrom( oReader );
				else if ( _T("w:moveFromRangeEnd") == sName )
					pItem = new OOX::Logic::CMoveFromRangeEnd( oReader );
				else if ( _T("w:moveFromRangeStart") == sName )
					pItem = new OOX::Logic::CMoveFromRangeStart( oReader );
				//else if ( _T("w:moveTo") == sName )
				//	pItem = new CMoveTo( oReader );
				else if ( _T("w:moveToRangeEnd") == sName )
					pItem = new OOX::Logic::CMoveToRangeEnd( oReader );
				else if ( _T("w:moveToRangeStart") == sName )
					pItem = new OOX::Logic::CMoveToRangeStart( oReader );
				else if ( _T("m:nary") == sName )
					pItem = new OOX::Logic::CNary( oReader );
				else if ( _T("m:oMath") == sName )
					pItem = new OOX::Logic::COMath( oReader );
				else if ( _T("m:oMathPara") == sName )
					pItem = new OOX::Logic::COMathPara( oReader );
				else if ( _T("w:permEnd") == sName )
					pItem = new OOX::Logic::CPermEnd( oReader );
				else if ( _T("w:permStart") == sName )
					pItem = new OOX::Logic::CPermStart( oReader );
				else if ( _T("m:phant") == sName )
					pItem = new OOX::Logic::CPhant( oReader );
				else if ( _T("w:proofErr") == sName )
					pItem = new OOX::Logic::CProofErr( oReader );
				else if ( _T("m:r") == sName )
					pItem = new OOX::Logic::CMRun( oReader );
				else if ( _T("m:rad") == sName )
					pItem = new OOX::Logic::CRad( oReader );
				else if ( _T("m:sPre") == sName )
					pItem = new OOX::Logic::CSPre( oReader );
				else if ( _T("m:sSub") == sName )
					pItem = new OOX::Logic::CSSub( oReader );
				else if ( _T("m:sSubSup") == sName )
					pItem = new OOX::Logic::CSSubSup( oReader );
				else if ( _T("m:sSup") == sName )
					pItem = new OOX::Logic::CSSup( oReader );

				if ( pItem )
					m_arrItems.Add( pItem );
			}
		}
	}//namespace Logic
}//namespace OOX