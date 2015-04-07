#include "Vml.h"
#include "VmlOfficeDrawing.h"

#include "Paragraph.h"
#include "Annotations.h"
#include "Run.h"
#include "RunProperty.h"
#include "ParagraphProperty.h"
#include "Sdt.h"
#include "Hyperlink.h"
#include "Table.h"
#include "../Math/oMathPara.h"
#include "../Math/OMath.h"

namespace OOX
{
// TO DO: Нехватающие классы:
	//        <w:altChunk>
	//        <w:customXml>
	//        <w:moveFrom>
	//        <w:moveTo>
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// CTxbxContent 9.8.1.1 (Part 4)
		//--------------------------------------------------------------------------------	
		void CTxbxContent::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CTxbxContent::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CTxbxContent::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ClearItems();
			
			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				CWCharWrapper sName = oReader.GetName();
				WritingElement *pItem = NULL;

				/*				if ( _T("w:altChunk") == sName )
				pItem = new CAltChunk( oReader );
				else */if ( _T("w:bookmarkEnd") == sName )
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
				else if ( _T("w:p") == sName )
					pItem = new CParagraph( oReader );
				else if ( _T("w:permEnd") == sName )
					pItem = new CPermEnd( oReader );
				else if ( _T("w:permStart") == sName )
					pItem = new CPermStart( oReader );
				else if ( _T("w:proofErr") == sName )
					pItem = new CProofErr( oReader );
				else if ( _T("w:sdt") == sName )
					pItem = new CSdt( oReader );
				else if ( _T("w:tbl") == sName )
					pItem = new CTbl( oReader );

				if ( pItem )
					m_arrItems.push_back( pItem );
			}
		}
		CString CTxbxContent::toXML() const
		{
			CString sResult = _T("<w:txbxContent>");

			for ( unsigned int nIndex = 0; nIndex < m_arrItems.size(); nIndex++ )
			{
				if ( m_arrItems[nIndex] )
				{
					sResult += m_arrItems[nIndex]->toXML();
				}
			}

			sResult += _T("</w:txbxContent>");

			return sResult;
		}

	} // namespace Logic
} // namespace OOX
namespace OOX
{
	namespace Vml
	{
		void CVmlCommonElements::ReadElements(XmlUtils::CXmlLiteReader& oReader) 
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while ( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				CWCharWrapper sName = oReader.GetName();
				wchar_t wsChar1 = sName[0];
				wchar_t wsChar3 = sName[2];

				WritingElement *pItem = NULL;
				switch ( wsChar1 )
				{
				case 'o':

					switch ( wsChar3 )
					{
					case 'c':

						if ( _T("o:callout") == sName )
							pItem = new OOX::VmlOffice::CCallout( oReader );
						else if ( _T("o:clippath") == sName )
							pItem = new OOX::VmlOffice::CClipPath( oReader );

						break;

					case 'e':

						if ( _T("o:extrusion") == sName )
							pItem = new OOX::VmlOffice::CExtrusion( oReader );

						break;

					case 'l':

						if ( _T("o:lock") == sName )
							pItem = new OOX::VmlOffice::CLock( oReader );

						break;

					case 's':

						if ( _T("o:signatureline") == sName )
							pItem = new OOX::VmlOffice::CSignatureLine( oReader );
						else if ( _T("o:skew") == sName )
							pItem = new OOX::VmlOffice::CSkew( oReader );

						break;
					}

					break;

				case 'p':
					//if ( _T("ppt:textdata") == sName )
					//	pItem = new OOX::Vml::CTextData( oReader );

					break;

				case 'v':

					switch ( wsChar3 )
					{
					case 'f':
						if ( _T("v:fill") == sName )
							pItem = new OOX::Vml::CFill( oReader );
						else if ( _T("v:formulas") == sName )
							pItem = new OOX::Vml::CFormulas( oReader );

						break;

					case 'h':

						if ( _T("v:handles") == sName )
							pItem = new OOX::Vml::CHandles( oReader );

						break;

					case 'i':

						if ( _T("v:imagedata") == sName )
							pItem = new OOX::Vml::CImageData( oReader );

						break;

					case 'p':

						if ( _T("v:path") == sName )
							pItem = new OOX::Vml::CPath( oReader );

						break;

					case 's':

						if ( _T("v:shadow") == sName )
							pItem = new OOX::Vml::CShadow( oReader );
						else if ( _T("v:stroke") == sName )
							pItem = new OOX::Vml::CStroke( oReader );

						break;

					case 't':

						if ( _T("v:textbox") == sName )
							pItem = new OOX::Vml::CTextbox( oReader );
						else if ( _T("v:textpath") == sName )
							pItem = new OOX::Vml::CTextPath( oReader );

						break;
					}

					break;


				case 'w':

					if ( _T("wd:anchorLock") == sName )
						pItem = new OOX::VmlWord::CAnchorLock( oReader );
					else if ( _T("wd:borderbottom") == sName )
						pItem = new OOX::VmlWord::CBorder( oReader );
					else if ( _T("wd:borderleft") == sName )
						pItem = new OOX::VmlWord::CBorder( oReader );
					else if ( _T("wd:borderright") == sName )
						pItem = new OOX::VmlWord::CBorder( oReader );
					else if ( _T("wd:bordertop") == sName )
						pItem = new OOX::VmlWord::CBorder( oReader );
					else if ( _T("wd:wrap") == sName )
						pItem = new OOX::VmlWord::CWrap( oReader );
					else if ( _T("w10:wrap") == sName )
						pItem = new OOX::VmlWord::CWrap( oReader );

					break;

				case 'x':

					if ( _T("x:ClientData") == sName )
						pItem = new OOX::Vml::CClientData( oReader );

					break;

				}

				if ( NULL != pItem )
					m_arrItems.push_back( pItem );
			}
		}

		void CVmlCommonElements::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Выставляем значения по умолчанию
			m_oHrPct.SetValue( 0 );
			m_oSpt.SetValue( 0 );

			// Читаем атрибуты
			if ( oReader.GetAttributesCount() <= 0 )
				return;
			
			if ( !oReader.MoveToFirstAttribute() )
				return;
			
			CWCharWrapper wsName = oReader.GetName();
			while( !wsName.IsNull() )
			{
				wchar_t wsChar = wsName[0];
				switch ( wsChar )
				{
				case 'a':
					if      ( _T("alt") == wsName ) m_sAlt = oReader.GetText();
					break;

				case 'c':
					if      ( _T("chromakey")   == wsName ) m_oChromaKey   = oReader.GetText();
					else if ( _T("class")       == wsName ) m_sClass       = oReader.GetText();
					else if ( _T("coordorigin") == wsName ) m_oCoordOrigin = oReader.GetText();
					else if ( _T("coordsize")   == wsName ) m_oCoordSize   = oReader.GetText();
					break;

				case 'f':
					if      ( _T("fillcolor") == wsName ) m_oFillColor = oReader.GetText();
					else if ( _T("filled")    == wsName ) m_oFilled    = oReader.GetText();
					break;

				case 'h':
					if      ( _T("href") == wsName ) m_sHref = oReader.GetText();
					break;

				case 'i':
					if      ( _T("id")       == wsName ) m_sId       = oReader.GetText();
					else if ( _T("insetpen") == wsName ) m_oInsetPen = oReader.GetText();
					break;

				case 'o':
					{
						wchar_t wsChar2 = wsName[2]; // o:_
						switch ( wsChar2 )
						{
						case 'a':
							if      ( _T("o:allowincell")  == wsName ) m_oAllowInCell  = oReader.GetText();
							else if ( _T("o:allowoverlap") == wsName ) m_oAllowOverlap = oReader.GetText();
							else if ( _T("opacity")        == wsName ) m_oOpacity      = oReader.GetText();
							break;
						case 'b':
							if      ( _T("o:borderbottomcolor") == wsName ) m_oBorderBottomColor = oReader.GetText();
							else if ( _T("o:borderleftcolor")   == wsName ) m_oBorderLeftColor   = oReader.GetText();
							else if ( _T("o:borderrightcolor")  == wsName ) m_oBorderRightColor  = oReader.GetText();
							else if ( _T("o:bordertopcolor")    == wsName ) m_oBorderTopColor    = oReader.GetText();
							else if ( _T("o:bullet")            == wsName ) m_oBullet            = oReader.GetText();
							else if ( _T("o:button")            == wsName ) m_oButton            = oReader.GetText();
							else if ( _T("o:bwmode")            == wsName ) m_oBwMode            = oReader.GetText();
							else if ( _T("o:bwnormal")          == wsName ) m_oBwNormal           = oReader.GetText();
							else if ( _T("o:bwpure")            == wsName ) m_oBwPure            = oReader.GetText();
							break;
						case 'c':  
							if      ( _T("o:clip")          == wsName ) m_oClip          = oReader.GetText();
							else if ( _T("o:cliptowrap")    == wsName ) m_oClipToWrap    = oReader.GetText();
							else if ( _T("o:connectortype") == wsName ) m_oConnectorType = oReader.GetText();
							break;
						case 'd':
							if      ( _T("o:doubleclicknotify") == wsName ) m_oDoubleClickNotify = oReader.GetText();
							else if ( _T("o:dgmlayout")         == wsName ) m_oDgmLayout         = oReader.GetText();
							else if ( _T("o:dgmlayoutmru")      == wsName ) m_oDgmLayoutMru      = oReader.GetText();
							else if ( _T("o:dgmnodekind")       == wsName ) m_oDgmNodeKind       = oReader.GetText();
							break;
						case 'f':
							if      ( _T("o:forcedash") == wsName ) m_oForceDash = oReader.GetText();
							break;
						case 'h':  
							if      ( _T("o:hr")        == wsName ) m_oHr        = oReader.GetText();
							else if ( _T("o:hralign")   == wsName ) m_oHrAlign   = oReader.GetText();
							else if ( _T("o:hrnoshade") == wsName ) m_oHrNoShade = oReader.GetText();
							else if ( _T("o:hrpct")     == wsName ) m_oHrPct     = oReader.GetText();
							else if ( _T("o:hrstd")     == wsName ) m_oHrStd     = oReader.GetText();
							break;
						case 'i':
							if      ( _T("o:insetmode") == wsName ) m_oInsetMode = oReader.GetText();
							break;
						case 'o':  
							if      ( _T("o:ole")     == wsName ) m_oOle        = oReader.GetText();
							else if ( _T("o:oleicon") == wsName ) m_oOleIcon    = oReader.GetText();
							else if ( _T("o:oned")    == wsName ) m_oOned       = oReader.GetText();
							break;
						case 'p':
							if      ( _T("o:preferrelative") == wsName ) m_oPreferRelative = oReader.GetText();
							break;
						case 'r':
							if      ( _T("o:regroupid") == wsName ) m_oRegroupId = oReader.GetText();
							break;
						case 's':
							if      ( _T("o:spid") == wsName ) m_sSpId = oReader.GetText();
							else if ( _T("o:spt")  == wsName ) m_oSpt  = oReader.GetText();
							break;
						case 'u':
							if      ( _T("o:userdrawn")  == wsName ) m_oUserDrawn  = oReader.GetText();
							else if ( _T("o:userhidden") == wsName ) m_oUserHidden = oReader.GetText();
							break;
						}

					break;
					}

				case 'p':
					if      ( _T("print") == wsName ) m_oPrint = oReader.GetText();
					break;
				case 's':
					if      ( _T("strokecolor")  == wsName ) m_oStrokeColor  = oReader.GetText();
					else if ( _T("stroked")      == wsName ) m_oStroked      = oReader.GetText();
					else if ( _T("strokeweight") == wsName ) m_oStrokeWeight = oReader.GetText();
					else if ( _T("style")        == wsName ) m_oStyle        = oReader.GetText();
					break;
				case 't':
					if      ( _T("target") == wsName ) m_sTarget = oReader.GetText();
					else if ( _T("title")  == wsName ) m_sTitle  = oReader.GetText();
					break;
				case 'w':
					if      ( _T("wrapcoords") == wsName ) m_oWrapCoords = oReader.GetText();
					break;
				}

				if ( !oReader.MoveToNextAttribute() )
					break;

				wsName = oReader.GetName();
			}
			oReader.MoveToElement();
		}
		CString CVmlCommonElements::WriteElements() const
		{
			CString sResult;

			for ( unsigned int nIndex = 0; nIndex < m_arrItems.size(); nIndex++ )
			{
				if ( m_arrItems[nIndex] )
					sResult += m_arrItems[nIndex]->toXML();
			}

			return sResult;
		};
		CString CVmlCommonElements::WriteAttributes() const
		{
			CString sResult;

			ComplexTypes_WriteAttribute2( _T("id=\""),          m_sId );
			ComplexTypes_WriteAttribute ( _T("style=\""),       m_oStyle );
			ComplexTypes_WriteAttribute2( _T("href=\""),        m_sHref );
			ComplexTypes_WriteAttribute2( _T("target=\""),      m_sTarget );
			ComplexTypes_WriteAttribute2( _T("class=\""),       m_sClass );
			ComplexTypes_WriteAttribute2( _T("title=\""),       m_sTitle );
			ComplexTypes_WriteAttribute2( _T("alt=\""),         m_sAlt );

			ComplexTypes_WriteAttribute ( _T("coordsize=\""),   m_oCoordSize );
			ComplexTypes_WriteAttribute ( _T("coordorigin=\""), m_oCoordOrigin );
			ComplexTypes_WriteAttribute ( _T("wrapcoords=\""),  m_oWrapCoords );

			if ( SimpleTypes::booleanTrue != m_oPrint.GetValue() )
				sResult += _T("print=\"false\" ");

			ComplexTypes_WriteAttribute2( _T("o:spid=\""),      m_sSpId );

			if ( SimpleTypes::booleanFalse != m_oOned.GetValue() )
				sResult += _T("o:oned=\"true\" ");
			
			ComplexTypes_WriteAttribute ( _T("o:regroupid=\""), m_oRegroupId );

			if ( SimpleTypes::booleanFalse != m_oDoubleClickNotify.GetValue() )
				sResult += _T("o:doubleclicknotify=\"true\" ");

			if ( SimpleTypes::booleanFalse != m_oButton.GetValue() )
				sResult += _T("o:button=\"true\" ");

			if ( SimpleTypes::booleanFalse != m_oUserHidden.GetValue() )
				sResult += _T("o:userhidden=\"true\" ");

			if ( SimpleTypes::booleanFalse != m_oBullet.GetValue() )
				sResult += _T("o:bullet=\"true\" ");

			if ( SimpleTypes::booleanFalse != m_oHr.GetValue() )
				sResult += _T("o:hr=\"true\" ");

			if ( SimpleTypes::booleanFalse != m_oHrStd.GetValue() )
				sResult += _T("o:hrstd=\"true\" ");

			if ( SimpleTypes::booleanFalse != m_oHrNoShade.GetValue() )
				sResult += _T("o:hrnoshade=\"true\" ");

			if ( 0 != m_oHrPct.GetValue() )
				sResult += _T("o:hrpct=\"") + m_oHrPct.ToString() + _T("\" ");

			if ( SimpleTypes::hralignLeft != m_oHrAlign.GetValue() )
				sResult += _T("o:hralign=\"") + m_oHrAlign.ToString() + _T("\" ");

			if ( SimpleTypes::booleanFalse != m_oAllowInCell.GetValue() )
				sResult += _T("o:allowincell=\"true\" ");

			if ( SimpleTypes::booleanFalse != m_oAllowOverlap.GetValue() )
				sResult += _T("o:allowoverlap=\"true\" ");

			if ( SimpleTypes::booleanFalse != m_oUserDrawn.GetValue() )
				sResult += _T("o:userdrawn=\"true\" ");

			ComplexTypes_WriteAttribute ( _T("o:bordertopcolor=\""),    m_oBorderTopColor );
			ComplexTypes_WriteAttribute ( _T("o:borderleftcolor=\""),   m_oBorderLeftColor );
			ComplexTypes_WriteAttribute ( _T("o:borderbottomcolor=\""), m_oBorderBottomColor );
			ComplexTypes_WriteAttribute ( _T("o:borderrightcolor=\""),  m_oBorderRightColor );

			ComplexTypes_WriteAttribute ( _T("o:dgmlayout=\""),     m_oDgmLayout );
			ComplexTypes_WriteAttribute ( _T("o:dgmlayoutmru=\""),  m_oDgmLayoutMru );
			ComplexTypes_WriteAttribute2( _T("o:dgmnodekind=\""),   m_oDgmNodeKind );

			if ( SimpleTypes::insetmodeCustom != m_oInsetMode.GetValue() )
				sResult += _T("o:insetmode=\"") + m_oInsetMode.ToString() + _T("\" ");

			ComplexTypes_WriteAttribute ( _T("chromakey=\""),  m_oChromaKey );

			ComplexTypes_WriteAttribute ( _T("filled=\""),		m_oFilled );

			ComplexTypes_WriteAttribute ( _T("fillcolor=\""),		m_oFillColor );
			ComplexTypes_WriteAttribute ( _T("opacity=\""),			m_oOpacity );

			ComplexTypes_WriteAttribute ( _T("stroked=\""), m_oStroked );

			ComplexTypes_WriteAttribute ( _T("strokecolor=\""), m_oStrokeColor);

			if ( m_oStrokeWeight.IsInit() )
				sResult += _T("strokeweight=\"") + m_oStrokeWeight->ToString() + _T("\" ");

			ComplexTypes_WriteAttribute ( _T("insetpen=\""),  m_oInsetPen );

			if ( 0 != m_oSpt.GetValue() )
				sResult += _T("o:spt=\"") + m_oSpt.ToString() + _T("\" ");

			if ( SimpleTypes::connectortypeStraight != m_oConnectorType.GetValue() )
				sResult += _T("o:connectortype=\"") + m_oConnectorType.ToString() + _T("\" ");

			ComplexTypes_WriteAttribute ( _T("o:bwmode=\""),   m_oBwMode );
			ComplexTypes_WriteAttribute ( _T("o:bwpure=\""),   m_oBwPure );
			ComplexTypes_WriteAttribute ( _T("o:bwnormal=\""), m_oBwNormal );

			if ( SimpleTypes::booleanFalse != m_oForceDash.GetValue() )
				sResult += _T("o:forcedash=\"true\" ");

			if ( SimpleTypes::booleanFalse != m_oOleIcon.GetValue() )
				sResult += _T("o:oleicon=\"true\" ");

			if ( SimpleTypes::booleanFalse != m_oOle.GetValue() )
				sResult += _T("o:ole=\"true\" ");

			if ( SimpleTypes::booleanFalse != m_oPreferRelative.GetValue() )
				sResult += _T("o:preferrelative=\"true\" ");

			if ( SimpleTypes::booleanFalse != m_oClipToWrap.GetValue() )
				sResult += _T("o:cliptowrap=\"true\" ");

			ComplexTypes_WriteAttribute ( _T("o:clip=\""), m_oClip );

			return sResult;
		}
		CString CVmlCommonElements::toXML() const
		{
			return _T("");
		}
		//--------------------------------------------------------------------------------
		// CGroup 14.1.2.6 (Part4)
		//--------------------------------------------------------------------------------	
		void CGroup::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CGroup::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CGroup::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while ( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				CWCharWrapper sName = oReader.GetName();
				wchar_t wsChar1 = sName[0];
				wchar_t wsChar3 = sName[2];

				WritingElement *pItem = NULL;
				switch ( wsChar1 )
				{
				case 'o':

					switch ( wsChar3 )
					{
					case 'c':

						if ( _T("o:callout") == sName )
							pItem = new OOX::VmlOffice::CCallout( oReader );
						else if ( _T("o:clippath") == sName )
							pItem = new OOX::VmlOffice::CClipPath( oReader );

						break;

					case 'd':

						if ( _T("o:diagram") == sName )
							pItem = new OOX::VmlOffice::CDiagram( oReader );

						break;


					case 'e':

						if ( _T("o:extrusion") == sName )
							pItem = new OOX::VmlOffice::CExtrusion( oReader );

						break;

					case 'l':

						if ( _T("o:lock") == sName )
							pItem = new OOX::VmlOffice::CLock( oReader );

						break;

					case 's':

						if ( _T("o:signatureline") == sName )
							pItem = new OOX::VmlOffice::CSignatureLine( oReader );
						else if ( _T("o:skew") == sName )
							pItem = new OOX::VmlOffice::CSkew( oReader );

						break;
					}

					break;

				case 'p':
					//if ( _T("ppt:textdata") == sName )
					//	pItem = new OOX::Vml::CTextData( oReader );

					break;

				case 'v':

					switch ( wsChar3 )
					{
					case 'a':
						if ( _T("v:arc") == sName )
							pItem = new OOX::Vml::CArc(oReader);
						break;

					case 'c':
						if ( _T("v:curve") == sName )
							pItem = new OOX::Vml::CCurve(oReader);
						break;

					case 'f':
						if ( _T("v:fill") == sName )
							pItem = new OOX::Vml::CFill( oReader );
						else if ( _T("v:formulas") == sName )
							pItem = new OOX::Vml::CFormulas( oReader );

						break;

					case 'g':
						if ( _T("v:group") == sName )
							pItem = new OOX::Vml::CGroup(oReader);

						break;

					case 'h':

						if ( _T("v:handles") == sName )
							pItem = new OOX::Vml::CHandles( oReader );

						break;

					case 'i':

						if ( _T("v:imagedata") == sName )
							pItem = new OOX::Vml::CImageData( oReader );
						else if ( _T("v:image") == sName )
							pItem = new OOX::Vml::CImage( oReader );

						break;

					case 'l':

						if ( _T("v:line") == sName )
							pItem = new OOX::Vml::CLine (oReader);

						break;

					case 'o':

						if ( _T("v:oval") == sName )
							pItem = new OOX::Vml::COval (oReader);

						break;

					case 'p':

						if ( _T("v:path") == sName )
							pItem = new OOX::Vml::CPath( oReader );
						else if ( _T("v:polyline") == sName )
							pItem = new OOX::Vml::CPolyLine(oReader);

						break;

					case 'r':

						if ( _T("v:rect") == sName )
							pItem = new OOX::Vml::CRect (oReader);
						else if ( _T("v:roundrect") == sName )
							pItem = new OOX::Vml::CRoundRect( oReader);

						break;

					case 's':

						if ( _T("v:shadow") == sName )
							pItem = new OOX::Vml::CShadow( oReader );
						else if ( _T("v:shape") == sName )
							pItem = new OOX::Vml::CShape( oReader);
						else if ( _T("v:shapetype") == sName )
							pItem = new OOX::Vml::CShapeType( oReader);
						else if ( _T("v:stroke") == sName )
							pItem = new OOX::Vml::CStroke( oReader );

						break;

					case 't':

						if ( _T("v:textbox") == sName )
							pItem = new OOX::Vml::CTextbox( oReader );
						else if ( _T("v:textpath") == sName )
							pItem = new OOX::Vml::CTextPath( oReader );

						break;
					}

					break;


				case 'w':

					if ( _T("wd:anchorLock") == sName )
						pItem = new OOX::VmlWord::CAnchorLock( oReader );
					else if ( _T("wd:borderbottom") == sName )
						pItem = new OOX::VmlWord::CBorder( oReader );
					else if ( _T("wd:borderleft") == sName )
						pItem = new OOX::VmlWord::CBorder( oReader );
					else if ( _T("wd:borderright") == sName )
						pItem = new OOX::VmlWord::CBorder( oReader );
					else if ( _T("wd:bordertop") == sName )
						pItem = new OOX::VmlWord::CBorder( oReader );
					else if ( _T("wd:wrap") == sName )
						pItem = new OOX::VmlWord::CWrap( oReader );

					break;

				case 'x':

					if ( _T("x:ClientData") == sName )
						pItem = new OOX::Vml::CClientData( oReader );

					break;

				}

				if ( NULL != pItem )
					m_arrItems.push_back( pItem );
			}
		}
		CString CGroup::toXML() const
		{
			CString sResult = _T("<v:group ");

			ComplexTypes_WriteAttribute2( _T("id=\""),          m_sId );
			ComplexTypes_WriteAttribute ( _T("style=\""),       m_oStyle );
			ComplexTypes_WriteAttribute2( _T("href=\""),        m_sHref );
			ComplexTypes_WriteAttribute2( _T("target=\""),      m_sTarget );
			ComplexTypes_WriteAttribute2( _T("class=\""),       m_sClass );
			ComplexTypes_WriteAttribute2( _T("title=\""),       m_sTitle );
			ComplexTypes_WriteAttribute2( _T("alt=\""),         m_sAlt );

			ComplexTypes_WriteAttribute ( _T("coordsize=\""),   m_oCoordSize );
			ComplexTypes_WriteAttribute ( _T("coordorigin=\""), m_oCoordOrigin );
			ComplexTypes_WriteAttribute ( _T("wrapcoords=\""),  m_oWrapCoords );

			if ( SimpleTypes::booleanTrue != m_oPrint.GetValue() )
				sResult += _T("print=\"false\" ");

			ComplexTypes_WriteAttribute2( _T("o:spid=\""),      m_sSpId );

			if ( SimpleTypes::booleanFalse != m_oOned.GetValue() )
				sResult += _T("o:oned=\"true\" ");

			ComplexTypes_WriteAttribute ( _T("o:regroupid=\""), m_oRegroupId );

			if ( SimpleTypes::booleanFalse != m_oDoubleClickNotify.GetValue() )
				sResult += _T("o:doubleclicknotify=\"true\" ");

			if ( SimpleTypes::booleanFalse != m_oButton.GetValue() )
				sResult += _T("o:button=\"true\" ");

			if ( SimpleTypes::booleanFalse != m_oUserHidden.GetValue() )
				sResult += _T("o:userhidden=\"true\" ");

			if ( SimpleTypes::booleanFalse != m_oBullet.GetValue() )
				sResult += _T("o:bullet=\"true\" ");

			if ( SimpleTypes::booleanFalse != m_oHr.GetValue() )
				sResult += _T("o:hr=\"true\" ");

			if ( SimpleTypes::booleanFalse != m_oHrStd.GetValue() )
				sResult += _T("o:hrstd=\"true\" ");

			if ( SimpleTypes::booleanFalse != m_oHrNoShade.GetValue() )
				sResult += _T("o:hrnoshade=\"true\" ");

			if ( 0 != m_oHrPct.GetValue() )
				sResult += _T("o:hrpct=\"") + m_oHrPct.ToString() + _T("\" ");

			if ( SimpleTypes::hralignLeft != m_oHrAlign.GetValue() )
				sResult += _T("o:hralign=\"") + m_oHrAlign.ToString() + _T("\" ");

			if ( SimpleTypes::booleanFalse != m_oAllowInCell.GetValue() )
				sResult += _T("o:allowincell=\"true\" ");

			if ( SimpleTypes::booleanFalse != m_oAllowOverlap.GetValue() )
				sResult += _T("o:allowoverlap=\"true\" ");

			if ( SimpleTypes::booleanFalse != m_oUserDrawn.GetValue() )
				sResult += _T("o:userdrawn=\"true\" ");

			ComplexTypes_WriteAttribute ( _T("o:bordertopcolor=\""),    m_oBorderTopColor );
			ComplexTypes_WriteAttribute ( _T("o:borderleftcolor=\""),   m_oBorderLeftColor );
			ComplexTypes_WriteAttribute ( _T("o:borderbottomcolor=\""), m_oBorderBottomColor );
			ComplexTypes_WriteAttribute ( _T("o:borderrightcolor=\""),  m_oBorderRightColor );

			ComplexTypes_WriteAttribute ( _T("o:dgmlayout=\""),     m_oDgmLayout );
			ComplexTypes_WriteAttribute ( _T("o:dgmlayoutmru=\""),  m_oDgmLayoutMru );
			ComplexTypes_WriteAttribute2( _T("o:dgmnodekind=\""),   m_oDgmNodeKind );

			if ( SimpleTypes::insetmodeCustom != m_oInsetMode.GetValue() )
				sResult += _T("o:insetmode=\"") + m_oInsetMode.ToString() + _T("\" ");

			if ( SimpleTypes::booleanTrue != m_oFilled.GetValue() )
				sResult += _T("filled=\"false\" ");

			ComplexTypes_WriteAttribute ( _T("fillcolor=\""),		m_oFillColor );
			ComplexTypes_WriteAttribute ( _T("editas=\""),			m_oEditAs );

			if ( 0 != m_oTableProperties.GetValue() )
				sResult += _T("o:tableproperties=\"") + m_oTableProperties.ToString() + _T("\" ");

			ComplexTypes_WriteAttribute ( _T("o:tablelimits=\""),  m_oTableLimits );

			sResult += _T(">");

			for (unsigned int nIndex = 0; nIndex < m_arrItems.size(); nIndex++ )
			{
				if ( m_arrItems[nIndex] )
					sResult += m_arrItems[nIndex]->toXML();
			}

			sResult += _T("</v:group>");

			return sResult;
		}
		//--------------------------------------------------------------------------------
		// CPolyLine 14.1.2.15 (Part4)
		//--------------------------------------------------------------------------------	
		CString CPolyLine::toXML() const
		{
			CString sResult = _T("<v:polyline ");

			sResult += CVmlCommonElements::WriteAttributes();

			sResult += _T("points=\"") + m_oPoints.ToString() + _T("\">");

			sResult += CVmlCommonElements::WriteElements();

			sResult += _T("</v:polyline>");

			return sResult;
		}
		//--------------------------------------------------------------------------------
		// CShape 14.1.2.19 (Part4)
		//--------------------------------------------------------------------------------	
		CString CShape::toXML() const
		{
			CString sResult = _T("<v:shape ");

			sResult += CVmlCommonElements::WriteAttributes();

			if ( m_sType.IsInit() )
			{
				sResult += _T("type=\"");
				sResult += m_sType->GetString();
				sResult += _T("\" ");
			}

			if ( m_oAdj.IsInit() )
			{
				sResult += _T("adj=\"");
				sResult += m_oAdj->GetString();
				sResult += _T("\" ");
			}

			if ( m_oPath.IsInit() )
				sResult += _T("path=\"") + m_oPath->ToString() + _T("\" ");

			// TO DO: сделать запись o:gfxdata

			if ( m_sEquationXML.IsInit() )
			{
				sResult += _T("equationxml=\"");
				sResult += m_sEquationXML->GetString();
				sResult += _T("\" ");
			}

			sResult += _T(">");

			sResult += CVmlCommonElements::WriteElements();

			sResult += _T("</v:shape>");

			return sResult;
		}
		//--------------------------------------------------------------------------------
		// CShapeType 14.1.2.20 (Part4)
		//--------------------------------------------------------------------------------	
		CString CShapeType::toXML() const
		{
			CString sResult = _T("<v:shapetype ");

			sResult += CVmlCommonElements::WriteAttributes();

			if ( m_oAdj.IsInit() )
			{
				sResult += _T("adj=\"");
				sResult += m_oAdj->GetString();
				sResult += _T("\" ");
			}

			if ( m_oPath.IsInit() )
				sResult += _T("path=\"") + m_oPath->ToString() + _T("\" ");

			if ( SimpleTypes::booleanFalse != m_oMaster.GetValue() )
				sResult += _T("o:master=\"true\" ");

			sResult += _T(">");

			sResult += CVmlCommonElements::WriteElements();

			sResult += _T("</v:shapetype>");

			return sResult;
		}

		void CClientData::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CShapeType::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CClientData::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while ( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				CWCharWrapper sName = oReader.GetName();

				if ( _T("x:MoveWithCells") == sName )
					m_oMoveWithCells = oReader.GetText2().GetString();
				else if ( _T("x:SizeWithCells") == sName )
					m_oSizeWithCells = oReader.GetText2().GetString();
				else if ( _T("x:Anchor") == sName )
					m_oAnchor = oReader.GetText2().GetString();
				else if ( _T("x:Row") == sName )
					m_oRow = oReader.GetText2().GetString();
				else if ( _T("x:Column") == sName )
					m_oColumn = oReader.GetText2().GetString();
			}
		}
		CString CClientData::toXML() const
		{
			CString sResult = _T("<x:ClientData ");
			if(m_oObjectType.IsInit())
				sResult += m_oObjectType->ToString();
			sResult += _T(">");
			if(m_oMoveWithCells.IsInit() && true == m_oMoveWithCells->ToBool())
				sResult += _T("<x:MoveWithCells/>");
			if(m_oSizeWithCells.IsInit() && true == m_oSizeWithCells->ToBool())
				sResult += _T("<x:SizeWithCells/>");
			if(m_oAnchor.IsInit())
			{
				CString sAnchor;sAnchor.Format(_T("<x:Anchor>%ls</x:Anchor>"), m_oAnchor.get());
				sResult += sAnchor;
			}
			if(m_oRow.IsInit())
			{
                CString sRow;sRow.Format(_T("<x:Row>%d</x:Row>"), m_oRow.get().GetValue());
				sResult += sRow;
			}
			if(m_oColumn.IsInit())
			{
                CString sColumn;sColumn.Format(_T("<x:Column>%d</x:Column>"), m_oColumn.get().GetValue());
				sResult += sColumn;
			}
			sResult = _T("</x:ClientData>");
			return sResult;
		}
	} // Vml
} // OOX
