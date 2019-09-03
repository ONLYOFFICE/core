/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
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

#include "../../XlsxFormat/Drawing/CellAnchor.h"

namespace OOX
{
// TO DO: Нехватающие классы:
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
				std::wstring sName = oReader.GetName();
				WritingElement *pItem = NULL;

				if ( _T("w:altChunk") == sName )
					pItem = new CAltChunk( oReader );
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
				else if ( _T("w:ins") == sName )
					pItem = new CIns( oReader );
				else if ( _T("w:moveFrom") == sName )
					pItem = new CMoveFrom( oReader );
				else if ( _T("w:moveFromRangeEnd") == sName )
					pItem = new CMoveFromRangeEnd( oReader );
				else if ( _T("w:moveFromRangeStart") == sName )
					pItem = new CMoveFromRangeStart( oReader );
				else if ( _T("w:moveTo") == sName )
					pItem = new CMoveTo( oReader );
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
		std::wstring CTxbxContent::toXML() const
		{
			std::wstring sResult = _T("<w:txbxContent>");

            for ( size_t i = 0; i < m_arrItems.size(); ++i)
            {
                if ( m_arrItems[i] )
                {
                    sResult += m_arrItems[i]->toXML();
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
				std::wstring sName = oReader.GetName();
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
                    {
						pItem = new OOX::Vml::CClientData( oReader );
                        OOX::Vml::CClientData * client_data = dynamic_cast<OOX::Vml::CClientData*>(pItem);
                        if ((client_data) && (client_data->m_oObjectType.IsInit()))
                               if (client_data->m_oObjectType->GetValue() == SimpleTypes::Vml::vmlclientdataobjecttypeNote)
                                    m_bComment = true;
                    }

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

			// Читаем атрибуты
			if ( oReader.GetAttributesCount() <= 0 )
				return;
			
			if ( !oReader.MoveToFirstAttribute() )
				return;
			
			std::wstring wsName = oReader.GetName();
			while( !wsName.empty() )
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
		std::wstring CVmlCommonElements::WriteElements() const
		{
			std::wstring sResult;

            for ( size_t i = 0; i < m_arrItems.size(); ++i)
            {
                if ( m_arrItems[i] )
                {
                    sResult += m_arrItems[i]->toXML();
                }
            }

			return sResult;
		};
		std::wstring CVmlCommonElements::WriteAttributes() const
		{
			std::wstring sResult;

			ComplexTypes_WriteAttribute3( _T("id=\""),          m_sId );
			ComplexTypes_WriteAttribute ( _T("style=\""),       m_oStyle );
			ComplexTypes_WriteAttribute3( _T("href=\""),        m_sHref );
			ComplexTypes_WriteAttribute3( _T("target=\""),      m_sTarget );
			ComplexTypes_WriteAttribute3( _T("class=\""),       m_sClass );
			ComplexTypes_WriteAttribute3( _T("title=\""),       m_sTitle );
			ComplexTypes_WriteAttribute3( _T("alt=\""),         m_sAlt );

			ComplexTypes_WriteAttribute ( _T("coordsize=\""),   m_oCoordSize );
			ComplexTypes_WriteAttribute ( _T("coordorigin=\""), m_oCoordOrigin );
			ComplexTypes_WriteAttribute ( _T("wrapcoords=\""),  m_oWrapCoords );

			if ( SimpleTypes::booleanTrue != m_oPrint.GetValue() )
				sResult += _T("print=\"false\" ");

			ComplexTypes_WriteAttribute3( _T("o:spid=\""),      m_sSpId );

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
			ComplexTypes_WriteAttribute3( _T("o:dgmnodekind=\""),   m_oDgmNodeKind );

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

			if ( m_oSpt.IsInit() )
				sResult += _T("o:spt=\"") + m_oSpt->ToString() + _T("\" ");

			if (m_oConnectorType.IsInit())
			{
				sResult += _T("o:connectortype=\"") + m_oConnectorType->ToString() + _T("\" ");
			}

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
		std::wstring CVmlCommonElements::toXML() const
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
				std::wstring sName = oReader.GetName();
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
		void CGroup::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Выставляем значения по умолчанию
			m_oHrPct.SetValue( 0 );

			// Читаем атрибуты
			if ( oReader.GetAttributesCount() <= 0 )
				return;
			
			if ( !oReader.MoveToFirstAttribute() )
				return;
			
			std::wstring wsName = oReader.GetName();
			while( !wsName.empty() )
			{
				wchar_t wsChar = wsName[0];
				switch ( wsChar )
				{
				case 'a':
					if      ( _T("alt") == wsName ) m_sAlt = oReader.GetText();
					break;

				case 'c':
					if      ( _T("class")       == wsName ) m_sClass       = oReader.GetText();
					else if ( _T("coordorigin") == wsName ) m_oCoordOrigin = oReader.GetText();
					else if ( _T("coordsize")   == wsName ) m_oCoordSize   = oReader.GetText();
					break;

				case 'e':
					if      ( _T("editas") == wsName ) m_oEditAs = oReader.GetText();
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
					break;

				case 'o':
					{
						wchar_t wsChar2 = wsName[2]; // o:_
						switch ( wsChar2 )
						{
						case 'a':
							if      ( _T("o:allowincell")  == wsName ) m_oAllowInCell  = oReader.GetText();
							else if ( _T("o:allowoverlap") == wsName ) m_oAllowOverlap = oReader.GetText();
							break;
						case 'b':
							if      ( _T("o:borderbottomcolor") == wsName ) m_oBorderBottomColor = oReader.GetText();
							else if ( _T("o:borderleftcolor")   == wsName ) m_oBorderLeftColor   = oReader.GetText();
							else if ( _T("o:borderrightcolor")  == wsName ) m_oBorderRightColor  = oReader.GetText();
							else if ( _T("o:bordertopcolor")    == wsName ) m_oBorderTopColor    = oReader.GetText();
							else if ( _T("o:bullet")            == wsName ) m_oBullet            = oReader.GetText();
							else if ( _T("o:button")            == wsName ) m_oButton            = oReader.GetText();
							break;
						case 'd':
							if      ( _T("o:doubleclicknotify") == wsName ) m_oDoubleClickNotify = oReader.GetText();
							else if ( _T("o:dgmlayout")         == wsName ) m_oDgmLayout         = oReader.GetText();
							else if ( _T("o:dgmlayoutmru")      == wsName ) m_oDgmLayoutMru      = oReader.GetText();
							else if ( _T("o:dgmnodekind")       == wsName ) m_oDgmNodeKind       = oReader.GetText();
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
							if      ( _T("o:oned")    == wsName ) m_oOned       = oReader.GetText();
							break;
						case 'r':
							if      ( _T("o:regroupid") == wsName ) m_oRegroupId = oReader.GetText();
							break;
						case 's':
							if      ( _T("o:spid") == wsName ) m_sSpId = oReader.GetText();
							break;
						case 't':
							if      ( _T("o:tableproperties")  == wsName ) m_oTableProperties = oReader.GetText();
							else if ( _T("o:tablelimits")      == wsName ) m_oTableLimits     = oReader.GetText();
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
					if      ( _T("style") == wsName ) m_oStyle = oReader.GetText();
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


		std::wstring CGroup::toXML() const
		{
			std::wstring sResult = _T("<v:group ");

			ComplexTypes_WriteAttribute3( _T("id=\""),          m_sId );
			ComplexTypes_WriteAttribute ( _T("style=\""),       m_oStyle );
			ComplexTypes_WriteAttribute3( _T("href=\""),        m_sHref );
			ComplexTypes_WriteAttribute3( _T("target=\""),      m_sTarget );
			ComplexTypes_WriteAttribute3( _T("class=\""),       m_sClass );
			ComplexTypes_WriteAttribute3( _T("title=\""),       m_sTitle );
			ComplexTypes_WriteAttribute3( _T("alt=\""),         m_sAlt );

			ComplexTypes_WriteAttribute ( _T("coordsize=\""),   m_oCoordSize );
			ComplexTypes_WriteAttribute ( _T("coordorigin=\""), m_oCoordOrigin );
			ComplexTypes_WriteAttribute ( _T("wrapcoords=\""),  m_oWrapCoords );

			if ( SimpleTypes::booleanTrue != m_oPrint.GetValue() )
				sResult += _T("print=\"false\" ");

			ComplexTypes_WriteAttribute3( _T("o:spid=\""),      m_sSpId );

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
			ComplexTypes_WriteAttribute3( _T("o:dgmnodekind=\""),   m_oDgmNodeKind );

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

            for ( size_t i = 0; i < m_arrItems.size(); ++i)
            {
                if ( m_arrItems[i] )
                {
                    sResult += m_arrItems[i]->toXML();
                }
            }

			sResult += _T("</v:group>");

			return sResult;
		}
		//--------------------------------------------------------------------------------
		// CPolyLine 14.1.2.15 (Part4)
		//--------------------------------------------------------------------------------	
		std::wstring CPolyLine::toXML() const
		{
			std::wstring sResult = _T("<v:polyline ");

			sResult += CVmlCommonElements::WriteAttributes();

			sResult += _T("points=\"") + m_oPoints.ToString() + _T("\">");

			sResult += CVmlCommonElements::WriteElements();

			sResult += _T("</v:polyline>");

			return sResult;
		}
		//--------------------------------------------------------------------------------
		// CShape 14.1.2.19 (Part4)
		//--------------------------------------------------------------------------------	
		std::wstring CShape::toXML() const
		{
			std::wstring sResult = L"<v:shape ";

			sResult += CVmlCommonElements::WriteAttributes();

			if ( m_sType.IsInit() )
			{
				sResult += L"type=\"" + *m_sType + L"\" ";
			}

            if ( m_sAdj.IsInit() )
			{
				sResult += L"adj=\"" + *m_sAdj + L"\" ";
			}

			if ( m_oPath.IsInit() )
				sResult += L"path=\"" + m_oPath->ToString() + L"\" ";

			// TO DO: сделать запись o:gfxdata

			if ( m_sEquationXML.IsInit() )
			{
				sResult += L"equationxml=\"" + *m_sEquationXML + L"\" ";
			}

			sResult += L">";

			sResult += CVmlCommonElements::WriteElements();

			sResult += L"</v:shape>";

			return sResult;
		}
		//--------------------------------------------------------------------------------
		// CShapeType 14.1.2.20 (Part4)
		//--------------------------------------------------------------------------------	
		std::wstring CShapeType::toXML() const
		{
			std::wstring sResult = _T("<v:shapetype ");

			sResult += CVmlCommonElements::WriteAttributes();

            if ( m_sAdj.IsInit() )
			{
				sResult += L"adj=\"" + *m_sAdj + L"\" ";
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
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				std::wstring sContent = oReader.GetText2();

				if		( L"MoveWithCells"	== sName )	m_oMoveWithCells = sContent.empty() ? L"t" : sContent;
				else if ( L"SizeWithCells"	== sName )	m_oSizeWithCells = sContent.empty() ? L"t" : sContent;
				else if ( L"Anchor"			== sName )	m_oAnchor = sContent;
				else if ( L"Row"			== sName )	m_oRow = sContent;
				else if ( L"Column"			== sName )	m_oColumn = sContent;
				else if ( L"DefaultSize"	== sName )	m_oDefaultSize = sContent.empty() ? L"t" : sContent;
				else if ( L"AutoLine"		== sName )	m_oAutoLine = sContent.empty() ? L"t" : sContent;
				else if ( L"AutoFill"		== sName )	m_oAutoFill = sContent.empty() ? L"t" : sContent;
				else if ( L"AutoPict"		== sName )	m_oAutoPict = sContent.empty() ? L"t" : sContent;
				else if ( L"AutoScale"		== sName )	m_oAutoScale = sContent.empty() ? L"t" : sContent;
				else if ( L"FmlaLink"		== sName )	m_oFmlaLink = sContent;
				else if ( L"FmlaRange"		== sName )	m_oFmlaRange = sContent;
				else if ( L"FmlaMacro"		== sName )	m_oFmlaMacro = sContent;
				else if ( L"FmlaTxbx"		== sName )	m_oFmlaTxbx = sContent;
				else if ( L"FmlaGroup"		== sName )	m_oFmlaGroup = sContent;
				else if ( L"CF"				== sName )	m_oCf = sContent;
				else if ( L"Min"			== sName )	m_oMin = sContent;
				else if ( L"Max"			== sName )	m_oMax = sContent;
				else if ( L"Val"			== sName )	m_oVal = sContent;
				else if ( L"Inc"			== sName )	m_oInc = sContent;
				else if ( L"Sel"			== sName )	m_oSel = sContent.empty() ? L"t" : sContent;
				else if ( L"WidthMin"		== sName )	m_oWidthMin = sContent;
				else if ( L"Dx"				== sName )	m_oDx = sContent;
				else if ( L"Page"			== sName )	m_oPage = sContent;
				else if ( L"DropLines"		== sName )	m_oDropLines = sContent;
				else if ( L"NoThreeD2"		== sName )	m_oNoThreeD2 = sContent.empty() ? L"t" : sContent;
				else if ( L"NoThreeD"		== sName )	m_oNoThreeD = sContent.empty() ? L"t" : sContent;
				else if ( L"DropStyle"		== sName )	m_oDropStyle = sContent;
				else if ( L"FirstButton"	== sName )	m_oFirstButton = sContent.empty() ? L"t" : sContent;
				else if ( L"VScroll"		== sName )	m_oVScroll = sContent.empty() ? L"t" : sContent;
				else if ( L"Horiz"			== sName )	m_oHoriz = sContent.empty() ? L"t" : sContent;
				else if ( L"TextHAlign"		== sName )	m_oTextHAlign = sContent;
				else if ( L"TextVAlign"		== sName )	m_oTextVAlign = sContent;
				else if ( L"Colored"		== sName )	m_oColored = sContent.empty() ? L"t" : sContent;
				else if ( L"MultiLine"		== sName )	m_oMultiLine = sContent.empty() ? L"t" : sContent;
				else if ( L"LockText"		== sName )	m_oLockText = sContent.empty() ? L"t" : sContent;
				else if ( L"JustLastX"		== sName )	m_oJustLastX = sContent.empty() ? L"t" : sContent;
				else if ( L"SecretEdit"		== sName )	m_oSecretEdit = sContent.empty() ? L"t" : sContent;
				else if ( L"SelType"		== sName )	m_oSelType = sContent;
				else if ( L"Visible"		== sName )	m_oVisible = sContent.empty() ? L"t" : sContent;
			}
		}
		std::wstring CClientData::toXML() const
		{
			std::wstring sResult = L"<x:ClientData";
			if(m_oObjectType.IsInit())
			{
				sResult += L" ObjectType=\"" + m_oObjectType->ToString() + L"\"";
			}
			sResult += L">";
			if(m_oMoveWithCells.IsInit() && *m_oMoveWithCells)
				sResult += L"<x:MoveWithCells/>";
			if(m_oSizeWithCells.IsInit() && *m_oSizeWithCells)
				sResult += L"<x:SizeWithCells/>";
			if(m_oAnchor.IsInit())
			{
				sResult += L"<x:Anchor>" + m_oAnchor.get() + L"</x:Anchor>";
			}
			if(m_oDefaultSize.IsInit())
			{
				sResult += std::wstring(L"<x:DefaultSize>") + (*m_oDefaultSize ? L"True" : L"False") + L"</x:DefaultSize>";
			}
			if(m_oAutoLine.IsInit())
			{
				sResult += std::wstring(L"<x:AutoLine>") + (*m_oAutoLine ? L"True" : L"False") + L"</x:AutoLine>";
			}
			if(m_oAutoScale.IsInit())
			{
				sResult += std::wstring(L"<x:AutoScale>") + (*m_oAutoScale ? L"True" : L"False") + L"</x:AutoScale>";
			}	
			if(m_oAutoPict.IsInit())
			{
				sResult += std::wstring(L"<x:AutoPict>") + (*m_oAutoPict ? L"True" : L"False") + L"</x:AutoPict>";
			}	
			if(m_oAutoFill.IsInit())
			{
				sResult += std::wstring(L"<x:AutoFill>") + (*m_oAutoFill ? L"True" : L"False") + L"</x:AutoFill>";
			}	
			if(m_oFirstButton.IsInit())
			{
				sResult += std::wstring(L"<x:FirstButton>") + (*m_oFirstButton ? L"True" : L"False") + L"</x:FirstButton>";
			}
			if(m_oVScroll.IsInit())
			{
				sResult += std::wstring(L"<x:VScroll>") + (*m_oVScroll ? L"True" : L"False") + L"</x:VScroll>";
			}	
			if(m_oHoriz.IsInit())
			{
				sResult += std::wstring(L"<x:Horiz>") + (*m_oHoriz ? L"True" : L"False") + L"</x:Horiz>";
			}
			if(m_oColored.IsInit())
			{
				sResult += std::wstring(L"<x:Colored>") + (*m_oColored ? L"True" : L"False") + L"</x:Colored>";
			}			
			if(m_oJustLastX.IsInit())
			{
				sResult += std::wstring(L"<x:JustLastX>") + (*m_oJustLastX ? L"True" : L"False") + L"</x:JustLastX>";
			}
			if(m_oMultiLine.IsInit())
			{
				sResult += std::wstring(L"<x:MultiLine>") + (*m_oMultiLine ? L"True" : L"False") + L"</x:MultiLine>";
			}
			if(m_oTextHAlign.IsInit())
			{
				sResult += std::wstring(L"<x:TextHAlign>") + m_oTextHAlign->ToString() + L"</x:TextHAlign>";
			}
			if(m_oTextVAlign.IsInit())
			{
				sResult += std::wstring(L"<x:TextVAlign>") + m_oTextVAlign->ToString() + L"</x:TextVAlign>";
			}
			if(m_oLockText.IsInit())
			{
				sResult += std::wstring(L"<x:LockText>") + (*m_oLockText ? L"True" : L"False") + L"</x:LockText>";
			}			
			if(m_oRow.IsInit())
			{
                sResult += L"<x:Row>" + std::to_wstring(m_oRow->GetValue()) + L"</x:Row>";
			}
			if(m_oColumn.IsInit())
			{
                sResult += L"<x:Column>" + std::to_wstring(m_oColumn->GetValue()) + L"</x:Column>";
			}
			if(m_oVal.IsInit())
			{
                sResult += L"<x:Val>" + std::to_wstring(*m_oVal) + L"</x:Val>";
			}
			if(m_oCf.IsInit())
			{
                sResult += L"<x:CF>" + (*m_oCf) + L"</x:CF>";
			}
			if(m_oMultiSel.IsInit())
			{
                sResult += L"<x:MultiSel>" + (*m_oMultiSel) + L"</x:MultiSel>";
			}			
			if (m_oChecked.IsInit())
			{
				sResult += _T("<x:Checked>") + m_oChecked->ToString() + _T("</x:Checked>");
			}
			if (m_oFmlaLink.IsInit())
			{
				sResult += _T("<x:FmlaLink>") + *m_oFmlaLink + _T("</x:FmlaLink>");
			}
			if (m_oFmlaRange.IsInit())
			{
				sResult += _T("<x:FmlaRange>") + *m_oFmlaRange + _T("</x:FmlaRange>");
			}
			if (m_oFmlaMacro.IsInit())
			{
				sResult += _T("<x:FmlaMacro>") + *m_oFmlaMacro + _T("</x:FmlaMacro>");
			}
			if (m_oFmlaTxbx.IsInit())
			{
				sResult += _T("<x:FmlaTxbx>") + *m_oFmlaTxbx + _T("</x:FmlaTxbx>");
			}
			if (m_oFmlaGroup.IsInit())
			{
				sResult += _T("<x:FmlaGroup>") + *m_oFmlaGroup + _T("</x:FmlaGroup>");
			}
			if (m_oMin.IsInit())
			{
                sResult += L"<x:Min>" + std::to_wstring(m_oMin->GetValue()) + L"</x:Min>";
			}
			if (m_oMax.IsInit())
			{
                sResult += L"<x:Max>" + std::to_wstring(m_oMax->GetValue()) + L"</x:Max>";
			}
			if (m_oInc.IsInit())
			{
                sResult += L"<x:Inc>" + std::to_wstring(m_oInc->GetValue()) + L"</x:Inc>";
			}
			if (m_oSel.IsInit())
			{
                sResult += L"<x:Sel>" + std::to_wstring(m_oSel->GetValue()) + L"</x:Sel>";
			}
			if (m_oSelType.IsInit())
			{
                sResult += L"<x:SelType>" + m_oSelType->ToString() + L"</x:SelType>";
			}			
			if (m_oDx.IsInit())
			{
                sResult += L"<x:Dx>" + std::to_wstring(m_oDx->GetValue()) + L"</x:Dx>";
			}
			if (m_oDropStyle.IsInit())
			{
				sResult += _T("<x:DropStyle>") + m_oDropStyle->ToString() + _T("</x:DropStyle>");
			}
			if (m_oDropLines.IsInit())
			{
                sResult += L"<x:DropLines>" + std::to_wstring(m_oDropLines->GetValue()) + L"</x:DropLines>";
			}
			if (m_oPage.IsInit())
			{
                sResult += L"<x:Page>" + std::to_wstring(m_oPage->GetValue()) + L"</x:Page>";
			}
			if (m_oWidthMin.IsInit())
			{
                sResult += L"<x:WidthMin>" + std::to_wstring(m_oWidthMin->GetValue()) + L"</x:WidthMin>";
			}
			if (m_oNoThreeD.IsInit())
			{
				sResult += std::wstring(L"<x:NoThreeD>") + (*m_oNoThreeD ? L"True" : L"False") + L"</x:NoThreeD>";
			}					
			if (m_oNoThreeD2.IsInit())
			{
				sResult += std::wstring(L"<x:NoThreeD2>") + (*m_oNoThreeD2 ? L"True" : L"False") + L"</x:NoThreeD2>";
			}
			if (m_oSecretEdit.IsInit())
			{
				sResult += std::wstring(L"<x:SecretEdit>") + (*m_oSecretEdit ? L"True" : L"False") + L"</x:SecretEdit>";
			}			
			sResult += L"</x:ClientData>";
			return sResult;
		}
		void CClientData::toCellAnchor(OOX::Spreadsheet::CCellAnchor *pCellAnchor)
		{
			if (!m_oAnchor.IsInit()) return;
			if (!pCellAnchor) return;

			std::vector<std::wstring> sAnchors;
            boost::algorithm::split(sAnchors, m_oAnchor.get(), boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);
			
			if (sAnchors.size() < 8) return;

			if (pCellAnchor == NULL)
			{
				SimpleTypes::Spreadsheet::CCellAnchorType<> eAnchorType;
				eAnchorType.SetValue(SimpleTypes::Spreadsheet::cellanchorTwoCell);
				
				pCellAnchor = new OOX::Spreadsheet::CCellAnchor(eAnchorType);
			}
            
			pCellAnchor->m_oFrom.Init();
			pCellAnchor->m_oFrom->m_oCol.Init();
			pCellAnchor->m_oFrom->m_oCol->SetValue(XmlUtils::GetInteger(sAnchors[0]));
			pCellAnchor->m_oFrom->m_oColOff.Init();
			pCellAnchor->m_oFrom->m_oColOff->FromPx(XmlUtils::GetInteger(sAnchors[1]));
			pCellAnchor->m_oFrom->m_oRow.Init();
			pCellAnchor->m_oFrom->m_oRow->SetValue(XmlUtils::GetInteger(sAnchors[2]));
			pCellAnchor->m_oFrom->m_oRowOff.Init();
			pCellAnchor->m_oFrom->m_oRowOff->FromPx(XmlUtils::GetInteger(sAnchors[3]));

			pCellAnchor->m_oTo.Init();
			pCellAnchor->m_oTo->m_oCol.Init();
			pCellAnchor->m_oTo->m_oCol->SetValue(XmlUtils::GetInteger(sAnchors[4]));
			pCellAnchor->m_oTo->m_oColOff.Init();
			pCellAnchor->m_oTo->m_oColOff->FromPx(XmlUtils::GetInteger(sAnchors[5]));
			pCellAnchor->m_oTo->m_oRow.Init();
			pCellAnchor->m_oTo->m_oRow->SetValue(XmlUtils::GetInteger(sAnchors[6]));
			pCellAnchor->m_oTo->m_oRowOff.Init();
			pCellAnchor->m_oTo->m_oRowOff->FromPx(XmlUtils::GetInteger(sAnchors[7]));

		}
		void CClientData::toFormControlPr(OOX::Spreadsheet::CFormControlPr* pFormControlPr)
		{
			if (!pFormControlPr) return;

			if (!pFormControlPr->m_oDropLines.IsInit())		pFormControlPr->m_oDropLines = m_oDropLines;
			if (!pFormControlPr->m_oChecked.IsInit())		pFormControlPr->m_oChecked = m_oChecked;
			if (!pFormControlPr->m_oDropStyle.IsInit())		pFormControlPr->m_oDropStyle = m_oDropStyle;
			if (!pFormControlPr->m_oDx.IsInit())			pFormControlPr->m_oDx = m_oDx;
			if (!pFormControlPr->m_oInc.IsInit())			pFormControlPr->m_oInc = m_oInc;
			if (!pFormControlPr->m_oMin.IsInit())			pFormControlPr->m_oMin = m_oMin;
			if (!pFormControlPr->m_oMax.IsInit())			pFormControlPr->m_oMax = m_oMax;
			if (!pFormControlPr->m_oPage.IsInit())			pFormControlPr->m_oPage = m_oPage;
			if (!pFormControlPr->m_oSel.IsInit())			pFormControlPr->m_oSel = m_oSel;
			if (!pFormControlPr->m_oSelType.IsInit())		pFormControlPr->m_oSelType = m_oSelType;
			if (!pFormControlPr->m_oTextHAlign.IsInit())	pFormControlPr->m_oTextHAlign = m_oTextHAlign;
			if (!pFormControlPr->m_oTextVAlign.IsInit())	pFormControlPr->m_oTextVAlign = m_oTextVAlign;
			if (!pFormControlPr->m_oVal.IsInit())			pFormControlPr->m_oVal = m_oVal;
			if (!pFormControlPr->m_oWidthMin.IsInit())		pFormControlPr->m_oWidthMin = m_oWidthMin;
//			if (!pFormControlPr->m_oEditVal.IsInit())		pFormControlPr->m_oEditVal = m_oEditVal;
			if (!pFormControlPr->m_oFmlaGroup.IsInit())		pFormControlPr->m_oFmlaGroup = m_oFmlaGroup;
			if (!pFormControlPr->m_oFmlaLink.IsInit())		pFormControlPr->m_oFmlaLink = m_oFmlaLink;
			if (!pFormControlPr->m_oFmlaRange.IsInit())		pFormControlPr->m_oFmlaRange = m_oFmlaRange;
			if (!pFormControlPr->m_oFmlaTxbx.IsInit())		pFormControlPr->m_oFmlaTxbx	= m_oFmlaTxbx;
			if (!pFormControlPr->m_oColored.IsInit())		pFormControlPr->m_oColored = m_oColored;
			if (!pFormControlPr->m_oFirstButton.IsInit())	pFormControlPr->m_oFirstButton = m_oFirstButton;
			if (!pFormControlPr->m_oHoriz.IsInit())			pFormControlPr->m_oHoriz = m_oHoriz;
			if (!pFormControlPr->m_oJustLastX.IsInit())		pFormControlPr->m_oJustLastX = m_oJustLastX;
			if (!pFormControlPr->m_oLockText.IsInit())		pFormControlPr->m_oLockText = m_oLockText;
			if (!pFormControlPr->m_oMultiSel.IsInit())		pFormControlPr->m_oMultiSel = m_oMultiSel;
			if (!pFormControlPr->m_oNoThreeD.IsInit())		pFormControlPr->m_oNoThreeD = m_oNoThreeD;
			if (!pFormControlPr->m_oNoThreeD2.IsInit())		pFormControlPr->m_oNoThreeD2 = m_oNoThreeD2;
			if (!pFormControlPr->m_oMultiLine.IsInit())		pFormControlPr->m_oMultiLine = m_oMultiLine;
			if (!pFormControlPr->m_oVerticalBar.IsInit())	pFormControlPr->m_oVerticalBar = m_oVScroll;
			if (!pFormControlPr->m_oPasswordEdit.IsInit())	pFormControlPr->m_oPasswordEdit = m_oSecretEdit;
		}		
		void CTextbox::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while ( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( L"txbxContent" == sName ) //namespaces w & wne
					m_oTxtbxContent = oReader;
				else if (L"div" == sName)
				{
					m_oText = oReader.GetText3();
					//m_arDivs.push_back();
				}
			}
		}
		std::wstring CTextbox::toXML() const
		{
			std::wstring sResult = L"<v:textbox ";

			ComplexTypes_WriteAttribute3(L"id=\"",    m_oId );
			ComplexTypes_WriteAttribute (L"style=\"", m_oStyle );

			if (m_oInset.IsInit())
			{
				sResult += L"inset=\"" + m_oInset->ToString() + L"\" ";
			}

			if ( SimpleTypes::booleanFalse != m_oSingleClick.GetValue() )
				sResult += L"o:singleclick=\"true\" ";

			if ( SimpleTypes::insetmodeCustom != m_oInsetMode.GetValue() )
				sResult += L"o:insetmode=\"" + m_oInsetMode.ToString() + L"\" ";

			sResult += L">";

			if ( m_oTxtbxContent.IsInit() )
				sResult += m_oTxtbxContent->toXML();

			if (m_oText.IsInit())
			{
				sResult += L"<div>" + *m_oText + L"</div>";
			}

			sResult += L"</v:textbox>";

			return sResult;
		}
		void CTextbox::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Выставляем значения по умолчанию
			m_oInset.Init();
			m_oInset->Set(7.2, 3.6, 7.2, 3.6 );
			
			// Читаем атрибуты
			if ( oReader.GetAttributesCount() <= 0 )
				return;
			
			if ( !oReader.MoveToFirstAttribute() )
				return;
			
			std::wstring wsName = oReader.GetName();
			while( !wsName.empty() )
			{
				wchar_t wsChar = wsName[0];
				switch ( wsChar )
				{
				case 'i':
					if      ( _T("id")            == wsName ) m_oId          = oReader.GetText();
					else if ( _T("inset")         == wsName ) m_oInset       = oReader.GetText();
					else if ( _T("insetmode")     == wsName ) m_oInsetMode   = oReader.GetText();
					break;
				case 'o':
					if      ( _T("o:singleclick") == wsName ) m_oSingleClick = oReader.GetText();
					break;
				case 's':
					if      ( _T("style")         == wsName ) m_oStyle       = oReader.GetText();
					break;
				}

				if ( !oReader.MoveToNextAttribute() )
					break;

				wsName = oReader.GetName();
			}
			oReader.MoveToElement();
		}
		void CFill::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Выставляем значения по умолчанию
			//m_oFocus.SetValue( 0 );
			//m_oFocusPosition.SetValue( 0.0, 0.0 );
			//m_oFocusSize.SetValue( 0.0, 0.0 );

			std::wstring sColors;
			// Читаем атрибуты
			if ( oReader.GetAttributesCount() <= 0 )
				return;
			
			if ( !oReader.MoveToFirstAttribute() )
				return;
			
			std::wstring wsName = oReader.GetName();
			while( !wsName.empty() )
			{
				wchar_t wsChar = wsName[0];
				switch ( wsChar )
				{
				case 'a':
					if      ( _T("aspect")     == wsName ) m_oAspect     = oReader.GetText();
					else if ( _T("angle")      == wsName ) m_oAngle      = oReader.GetText();
					else if ( _T("alignshape") == wsName ) m_oAlignShape = oReader.GetText();
					break;

				case 'c':
					if      ( _T("color")  == wsName ) m_oColor   = oReader.GetText();
					else if ( _T("color2") == wsName ) m_oColor2  = oReader.GetText();
					else if ( _T("colors") == wsName ) sColors    = oReader.GetText();
					break;

				case 'i':
					if      ( _T("id")   == wsName ) m_sId  = oReader.GetText();
					break;

				case 'm':
					if      ( _T("method")   == wsName ) m_oMethod = oReader.GetText();
					break;

				case 'f':
					if      ( _T("focus")         == wsName ) m_oFocus         = oReader.GetText();
					else if ( _T("focussize")     == wsName ) m_oFocusSize     = oReader.GetText();
					else if ( _T("focusposition") == wsName ) m_oFocusPosition = oReader.GetText();
					break;

				case 'o':
					if      ( _T("on")                 == wsName ) m_oOn               = oReader.GetText();
					else if ( _T("opacity")            == wsName ) m_oOpacity          = oReader.GetText();
					else if ( _T("o:href")             == wsName ) m_sHref             = oReader.GetText();
					else if ( _T("o:althref")          == wsName ) m_sAltHref          = oReader.GetText();
					else if ( _T("origin")             == wsName ) m_oOrigin           = oReader.GetText();
					else if ( _T("o:detectmouseclick") == wsName ) m_oDetectMouseClick = oReader.GetText();
					else if ( _T("o:title")            == wsName ) m_sTitle            = oReader.GetText();
					else if ( _T("o:opacity2")         == wsName ) m_oOpacity2         = oReader.GetText();
					else if ( _T("o:relid")            == wsName ) m_oRelId            = oReader.GetText();
					break;

				case 'p':
					if      ( _T("position") == wsName ) m_oPosition = oReader.GetText();
					break;

				case 'r':
					if      ( _T("recolor") == wsName ) m_oRecolor = oReader.GetText();
					else if ( _T("rotate")  == wsName ) m_oRotate  = oReader.GetText();
					else if ( _T("r:id")    == wsName ) m_rId      = oReader.GetText();
					break;

				case 's':
					if      ( _T("src")   == wsName ) m_sSrc    = oReader.GetText();
					else if ( _T("size")  == wsName ) m_oSize   = oReader.GetText();
					break;

				case 't':
					if      ( _T("type") == wsName ) m_oType = oReader.GetText();
					break;
				}

				if ( !oReader.MoveToNextAttribute() )
					break;

				wsName = oReader.GetName();
			}
			oReader.MoveToElement();

			// TO DO: сделать парсер цветов CFill::m_arrColors
		}



		std::wstring CFill::toXML() const
		{
			std::wstring sResult = _T("<v:fill ");
							
			ComplexTypes_WriteAttribute3( _T("id=\""), m_sId );
			sResult += _T("type=\"") + m_oType.ToString() + _T("\" ");
			
			if ( (m_oOn.IsInit()) && ( SimpleTypes::booleanTrue != m_oOn->GetValue() ))
				sResult += _T("on=\"false\" ");

			if (  m_oOpacity.IsInit() )
				sResult += _T("opacity=\"") + m_oOpacity->ToString() + _T("\" ");

			ComplexTypes_WriteAttribute (_T("color=\""), m_oColor);
			ComplexTypes_WriteAttribute (_T("color2=\""),	  m_oColor2);
			ComplexTypes_WriteAttribute3( _T("src=\""),       m_sSrc );
			ComplexTypes_WriteAttribute3( _T("o:href=\""),    m_sHref );
			ComplexTypes_WriteAttribute3( _T("o:althref=\""), m_sAltHref );
			ComplexTypes_WriteAttribute ( _T("size=\""),      m_oSize );
			ComplexTypes_WriteAttribute ( _T("origin=\""),    m_oOrigin );
			ComplexTypes_WriteAttribute ( _T("position=\""),  m_oPosition );

			if ((m_oAspect.IsInit()) && (SimpleTypes::imageaspectIgnore != m_oAspect->GetValue() ))
				sResult += _T("aspect=\"") + m_oAspect->ToString() + _T("\" ");

			// TO DO: Сделать запись m_arrColors

			ComplexTypes_WriteAttribute ( _T("angle=\""),         m_oAngle );

			if ( (m_oAlignShape.IsInit()) && (SimpleTypes::booleanTrue != m_oAlignShape->GetValue() ))
				sResult += _T("alignshape=\"false\" ");

			if ( (m_oFocus.IsInit()) && (0 != m_oFocus->GetValue() ))
				sResult += _T("focus=\"") + m_oFocus->ToString() + _T("\" ");

			if ( (m_oFocusPosition.IsInit()) && (0 != m_oFocusPosition->GetX() || 0 != m_oFocusPosition->GetY() ))
				sResult += _T("focusposition=\"") + m_oFocusPosition->ToString() + _T("\" ");

			if ( ( m_oFocusSize.IsInit() ) && (0 != m_oFocusSize->GetX() || 0 != m_oFocusSize->GetY() ))
				sResult += _T("focussize=\"") + m_oFocusSize->ToString() + _T("\" ");

			if ( (m_oMethod.IsInit() ) && (SimpleTypes::fillmethodSigma != m_oMethod->GetValue() ))
				sResult += _T("method=\"") + m_oMethod->ToString() + _T("\" ");

			ComplexTypes_WriteAttribute ( _T("o:detectmouseclick=\""), m_oDetectMouseClick );
			ComplexTypes_WriteAttribute3( _T("o:title=\""),            m_sTitle );

			if ( m_oOpacity2.IsInit() )
				sResult += _T("o:opacity2=\"") + m_oOpacity2->ToString() + _T("\" ");

			if ( (m_oRecolor.IsInit()) && (SimpleTypes::booleanFalse != m_oRecolor->GetValue() ))
				sResult += _T("recolor=\"true\" ");

			if (( m_oRotate.IsInit()) && (SimpleTypes::booleanFalse != m_oRotate->GetValue() ))
				sResult += _T("rotate=\"true\" ");

			ComplexTypes_WriteAttribute ( _T("r:id=\""),    m_rId );
			ComplexTypes_WriteAttribute ( _T("o:relid=\""), m_oRelId );

			sResult += _T(">");

			if ( m_oFill.IsInit() )
				sResult += m_oFill->toXML();

			sResult += _T("</v:fill>");

			return sResult;
		}
		void CStroke::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.GetAttributesCount() <= 0 )
				return;
			
			if ( !oReader.MoveToFirstAttribute() )
				return;
			
			std::wstring wsName = oReader.GetName();
			while( !wsName.empty() )
			{
				wchar_t wsChar = wsName[0];
				switch ( wsChar )
				{
				case 'c':
					if      ( _T("color")     == wsName ) m_oColor     = oReader.GetText();
					else if ( _T("color2")    == wsName ) m_oColor2    = oReader.GetText();
					break;
				case 'd':
					if      ( _T("dashstyle") == wsName ) m_oDahsStyle = oReader.GetText();
					break;
				case 'e':
					if      ( _T("endarrow")       == wsName ) m_oEndArrow       = oReader.GetText();
					else if ( _T("endarrowlength") == wsName ) m_oEndArrowLength = oReader.GetText();
					else if ( _T("endarrowwidth")  == wsName ) m_oEndArrowWidth  = oReader.GetText();
					else if ( _T("endcap")         == wsName ) m_oEndCap         = oReader.GetText();
					break;
				case 'f':
					if      ( _T("filltype") == wsName ) m_oFillType = oReader.GetText();
					break;
				case 'i':
					if      ( _T("id")              == wsName ) m_oId              = oReader.GetText();
					else if ( _T("imagealignshape") == wsName ) m_oImageAlignShape = oReader.GetText();
					else if ( _T("imageaspect")     == wsName ) m_oImageAspect     = oReader.GetText();
					else if ( _T("imagesize")       == wsName ) m_oImageSize       = oReader.GetText();
					else if ( _T("insetpen")        == wsName ) m_oInsetPen        = oReader.GetText();
					break;
				case 'j':
					if      ( _T("joinstyle") == wsName ) m_oJoinStyle = oReader.GetText();
					break;
				case 'l':
					if      ( _T("linestyle") == wsName ) m_oLineStyle = oReader.GetText();
					break;
				case 'm':
					if      ( _T("miterlimit") == wsName ) m_oMiterLimit = oReader.GetText();
					break;
				case 'o':
					if      ( _T("o:althref")   == wsName ) m_sAltHref   = oReader.GetText();
					else if ( _T("o:forcedash") == wsName ) m_oForceDash = oReader.GetText();
					else if ( _T("o:href")      == wsName ) m_sHref      = oReader.GetText();
					else if ( _T("on")          == wsName ) m_oOn        = oReader.GetText();
					else if ( _T("opacity")     == wsName ) m_oOpacity   = oReader.GetText();
					else if ( _T("o:relid")     == wsName ) m_oRelId     = oReader.GetText();
					else if ( _T("o:title")     == wsName ) m_sTitle     = oReader.GetText();
					break;
				case 'r':
					if      ( _T("r:id") == wsName ) m_rId = oReader.GetText();
					break;
				case 's':
					if      ( _T("src")              == wsName ) m_sSrc              = oReader.GetText();
					else if ( _T("startarrow")       == wsName ) m_oStartArrow       = oReader.GetText();
					else if ( _T("startarrowlength") == wsName ) m_oStartArrowLength = oReader.GetText();
					else if ( _T("startarrowwidth")  == wsName ) m_oStartArrowWidth  = oReader.GetText();
					break;
				case 'w':
					if      ( _T("weight") == wsName ) m_oWeight = oReader.GetText();
					break;
				}

				if ( !oReader.MoveToNextAttribute() )
					break;

				wsName = oReader.GetName();
			}
			oReader.MoveToElement();

			// TO DO: Сделать парсер формул ( или использовать уже сделанный парсер в OfficeDrawing\Shapes)
		}

		std::wstring CStroke::toXML() const
		{
			std::wstring sResult = _T("<v:stroke ");

			ComplexTypes_WriteAttribute3( _T("id=\""), m_oId );
			if ( SimpleTypes::booleanTrue != m_oOn.GetValue() )
				sResult += _T("on=\"false\" ");

			if ( 1 != m_oWeight.GetValue() )
				sResult += _T("weight=\"") + m_oWeight.ToString() + _T("\" ");

			//if ( SimpleTypes::colortypeBlack != m_oColor.GetValue() )
			//	sResult += _T("color=\"") + m_oColor.ToString() + _T("\" ");
			ComplexTypes_WriteAttribute ( _T("color=\""), m_oColor );

			if (m_oOpacity.IsInit() )
				sResult += _T("opacity=\"") + m_oOpacity->ToString() + _T("\" ");

			//if ( SimpleTypes::strokelinestyleSingle != m_oLineStyle.GetValue() )
			//	sResult += _T("linestyle=\"") + m_oLineStyle.ToString() + _T("\" ");
			ComplexTypes_WriteAttribute ( _T("linestyle=\""), m_oLineStyle );
			
			if ( 8 != m_oMiterLimit.GetValue() )
				sResult += _T("miterlimit=\"") + m_oMiterLimit.ToString() + _T("\" ");

			if ( SimpleTypes::strokejoinstyleRound != m_oJoinStyle.GetValue() )
				sResult += _T("joinstyle=\"") + m_oJoinStyle.ToString() + _T("\" ");

			if ( SimpleTypes::strokeendcapFlat != m_oEndCap.GetValue() )
				sResult += _T("endcap=\"") + m_oEndCap.ToString() + _T("\" ");

			if ( SimpleTypes::Vml::vmldashstyleSolid != m_oDahsStyle.GetValue() )
				sResult += _T("dashstyle=\"") + m_oDahsStyle.ToString() + _T("\" ");

			if ( SimpleTypes::filltypeSolid != m_oFillType.GetValue() )
				sResult += _T("filltype=\"") + m_oFillType.ToString() + _T("\" ");

			ComplexTypes_WriteAttribute3( _T("src=\""), m_sSrc );

			if ( SimpleTypes::imageaspectIgnore != m_oImageAspect.GetValue() )
				sResult += _T("imageaspect=\"") + m_oImageAspect.ToString() + _T("\" ");

			ComplexTypes_WriteAttribute ( _T("imagesize=\""), m_oImageSize );

			if ( SimpleTypes::booleanTrue != m_oOn.GetValue() )
				sResult += _T("imagealignshape=\"false\" ");

			ComplexTypes_WriteAttribute ( _T("color2=\""), m_oColor2 );

			if ( SimpleTypes::strokearrowtypeNone != m_oStartArrow.GetValue() )
				sResult += _T("startarrow=\"") + m_oStartArrow.ToString() + _T("\" ");

			if ( SimpleTypes::strokearrowwidthMedium != m_oStartArrowWidth.GetValue() )
				sResult += _T("startarrowwidth=\"") + m_oStartArrowWidth.ToString() + _T("\" ");

			if ( SimpleTypes::strokearrowlengthMedium != m_oStartArrowLength.GetValue() )
				sResult += _T("startarrowlength=\"") + m_oStartArrowLength.ToString() + _T("\" ");

			if ( SimpleTypes::strokearrowtypeNone != m_oEndArrow.GetValue() )
				sResult += _T("endarrow=\"") + m_oEndArrow.ToString() + _T("\" ");

			if ( SimpleTypes::strokearrowwidthMedium != m_oEndArrowWidth.GetValue() )
				sResult += _T("endarrowwidth=\"") + m_oEndArrowWidth.ToString() + _T("\" ");

			if ( SimpleTypes::strokearrowlengthMedium != m_oEndArrowLength.GetValue() )
				sResult += _T("endarrowlength=\"") + m_oEndArrowLength.ToString() + _T("\" ");

			ComplexTypes_WriteAttribute3( _T("o:href=\""),    m_sHref );
			ComplexTypes_WriteAttribute3( _T("o:althref=\""), m_sAltHref );
			ComplexTypes_WriteAttribute3( _T("o:title=\""),   m_sTitle );

			if ( SimpleTypes::booleanFalse != m_oForceDash.GetValue() )
				sResult += _T("o:forcedash=\"true\" ");

			ComplexTypes_WriteAttribute ( _T("r:id=\""),     m_rId );
			ComplexTypes_WriteAttribute ( _T("insetpen=\""), m_oInsetPen );
			ComplexTypes_WriteAttribute ( _T("o:relid=\""),  m_oRelId );

			sResult += _T(">");

			if ( m_oLeft.IsInit() )
				sResult += m_oLeft->toXML();

			if ( m_oTop.IsInit() )
				sResult += m_oTop->toXML();

			if ( m_oRight.IsInit() )
				sResult += m_oRight->toXML();

			if ( m_oBottom.IsInit() )
				sResult += m_oBottom->toXML();

			if ( m_oColumn.IsInit() )
				sResult += m_oColumn->toXML();

			sResult += _T("</v:stroke>");

			return sResult;
		}
		void CPath::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Выставляем значения по умолчанию
			m_oLimo.SetValue( 0, 0 );
			// Читаем атрибуты
			if ( oReader.GetAttributesCount() <= 0 )
				return;
			
			if ( !oReader.MoveToFirstAttribute() )
				return;
			
			std::wstring wsName = oReader.GetName();
			while( !wsName.empty() )
			{
				wchar_t wsChar = wsName[0];
				switch ( wsChar )
				{
				case 'a':
					if      ( _T("arrowok")    == wsName ) m_oArrowOk    = oReader.GetText();
					break;

				case 'f':
					if      ( _T("fillok")     == wsName ) m_oFillOk     = oReader.GetText();
					break;

				case 'g':
					if      ( _T("gradientshapeok") == wsName ) m_oGradientShapeOk = oReader.GetText();
					break;

				case 'i':
					if      ( _T("id")         == wsName ) m_oId         = oReader.GetText();
					else if ( _T("insetpenok") == wsName ) m_oInsetPenOk = oReader.GetText();
					break;

				case 'l':
					if      ( _T("limo")       == wsName ) m_oLimo         = oReader.GetText();
					break;

				case 'o':
					{
						wchar_t wsChar2 = wsName[2]; // o:_
						switch ( wsChar2 )
						{
						case 'c':
							if      ( _T("o:connectangles") == wsName ) m_oConnectAngles = oReader.GetText();
							else if ( _T("o:connectlocs")   == wsName ) m_oConnectLocs   = oReader.GetText();
							else if ( _T("o:connecttype")   == wsName ) m_oConnectType   = oReader.GetText();
							break;
						case 'e':
							if      ( _T("o:extrusionok")   == wsName ) m_oExtrusionOk   = oReader.GetText();
							break;
						}

					break;
					}

				case 's':
					if      ( _T("shadowok")    == wsName ) m_oShadowOk    = oReader.GetText();
					else if ( _T("strokeok")    == wsName ) m_oStrokeOk    = oReader.GetText();
					break;

				case 't':
					if      ( _T("textboxrect") == wsName ) m_oTextBoxRect = oReader.GetText();
					else if ( _T("textpathok")  == wsName ) m_oTextPathOk  = oReader.GetText();
					break;

				case 'v':
					if      ( _T("v")           == wsName ) m_oV           = oReader.GetText();
					break;
				}

				if ( !oReader.MoveToNextAttribute() )
					break;

				wsName = oReader.GetName();
			}
			oReader.MoveToElement();

			// TO DO: Сделать парсер параметров connectangles, connectlocs
		}

		std::wstring CPath::toXML() const
		{
			std::wstring sResult = _T("<v:path ");

			ComplexTypes_WriteAttribute3( _T("id=\""), m_oId );
			ComplexTypes_WriteAttribute ( _T("v=\""),  m_oV );

			if ( 0 != m_oLimo.GetX() || 0 != m_oLimo.GetY() )
				sResult += _T("limo=\"") + m_oLimo.ToString() + _T("\" ");

			ComplexTypes_WriteAttribute ( _T("textboxrect=\""), m_oTextBoxRect );

			if ( SimpleTypes::booleanTrue != m_oFillOk.GetValue() )
				sResult += _T("fillok=\"false\" ");

			if ( SimpleTypes::booleanTrue != m_oStrokeOk.GetValue() )
				sResult += _T("strokeok=\"false\" ");

			if ( SimpleTypes::booleanTrue != m_oShadowOk.GetValue() )
				sResult += _T("shadowok=\"false\" ");

			if ( SimpleTypes::booleanFalse != m_oArrowOk.GetValue() )
				sResult += _T("arrowok=\"true\" ");

			if ( SimpleTypes::booleanFalse != m_oGradientShapeOk.GetValue() )
				sResult += _T("gradientshapeok=\"true\" ");

			if ( SimpleTypes::booleanFalse != m_oTextPathOk.GetValue() )
				sResult += _T("textpathok=\"true\" ");

			if ( SimpleTypes::booleanFalse != m_oInsetPenOk.GetValue() )
				sResult += _T("insetpenok=\"true\" ");

			if ( SimpleTypes::connecttypeNone != m_oConnectType.GetValue() )
				sResult += _T("o:connecttype=\"") + m_oConnectType.ToString() + _T("\" ");

			ComplexTypes_WriteAttribute3( _T("o:connectlocs=\""),   m_oConnectLocs );
			ComplexTypes_WriteAttribute3( _T("o:connectangles=\""), m_oConnectAngles );

			if ( SimpleTypes::booleanTrue != m_oExtrusionOk.GetValue() )
				sResult += _T("extrusionok=\"false\" ");

			sResult += _T("/>");

			return sResult;
		}
		void CImageData::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Выставляем значения по умолчанию
			m_oBlackLevel.SetValue( 0.0 );
			m_oGain.SetValue( 1.0 );
			m_oGamma.SetValue( 1.0 );

			// Читаем атрибуты
			if ( oReader.GetAttributesCount() <= 0 )
				return;
			
			if ( !oReader.MoveToFirstAttribute() )
				return;
			
			std::wstring wsName = oReader.GetName();
			while( !wsName.empty() )
			{
				wchar_t wsChar = wsName[0];
				switch ( wsChar )
				{
				case 'a':
					if      ( _T("althref")    == wsName ) m_sAltHref    = oReader.GetText();
					break;

				case 'b':
					if      ( _T("bilevel")    == wsName ) m_oBiLevel    = oReader.GetText();
					else if ( _T("blacklevel") == wsName ) m_oBlackLevel = oReader.GetText();
					break;

				case 'c':
					if      ( _T("cropleft")   == wsName ) m_oCropLeft   = oReader.GetText();
					else if ( _T("croptop")    == wsName ) m_oCropTop    = oReader.GetText();
					else if ( _T("cropright")  == wsName ) m_oCropRight  = oReader.GetText();
					else if ( _T("cropbottom") == wsName ) m_oCropBottom = oReader.GetText();
					break;

				case 'e':
					if      ( _T("embosscolor")== wsName ) m_oEmbossColor= oReader.GetText();
					break;

				case 'g':
					if      ( _T("gain")       == wsName ) m_oGain       = oReader.GetText();
					if      ( _T("gamma")      == wsName ) m_oGamma      = oReader.GetText();
					if      ( _T("grayscale")  == wsName ) m_oGrayscale  = oReader.GetText();
					break;

				case 'i':
					if      ( _T("id")         == wsName ) m_oId         = oReader.GetText();
					break;

				case 'o':
					{
						wchar_t wsChar2 = wsName[2]; // o:_
						switch ( wsChar2 )
						{
						case 'd':
							if      ( _T("o:detectmouseclick") == wsName ) m_oDetectMouseClick = oReader.GetText();
							break;
						case 'h':
							if      ( _T("o:href")   == wsName ) m_oHref    = oReader.GetText();
							break;
						case 'm':
							if      ( _T("o:movie")  == wsName ) m_oMovie   = oReader.GetText();
							break;
						case 'o':
							if      ( _T("o:oleid")  == wsName ) m_oOleId   = oReader.GetText();
							break;
						case 'r':
							if      ( _T("o:relid")  == wsName ) m_oRelId   = oReader.GetText();
							break;
						case 't':
							if      ( _T("title")    == wsName ) m_sTitle   = oReader.GetText();
							break;
						}

					break;
					}

				case 'r':
					if      ( _T("r:href")        == wsName ) m_rHref          = oReader.GetText();
					else if ( _T("r:id")          == wsName ) m_rId            = oReader.GetText();
					else if ( _T("r:pict")        == wsName ) m_rPict          = oReader.GetText();
					else if ( _T("recolortarget") == wsName ) m_oRecolorTarget = oReader.GetText();
					break;

				case 's':
					if      ( _T("src")           == wsName ) m_sSrc           = oReader.GetText();
					break;
				}

				if ( !oReader.MoveToNextAttribute() )
					break;

				wsName = oReader.GetName();
			}
			oReader.MoveToElement();
		}



		std::wstring CImageData::toXML() const
		{
			std::wstring sResult = L"<v:imagedata ";

			ComplexTypes_WriteAttribute3(L"id=\"", m_oId );

			if ( _T("") !=  m_sSrc )
				sResult += L"src=\"" + m_sSrc + L"\" ";

			ComplexTypes_WriteAttribute(L"cropleft=\"",		m_oCropLeft);
			ComplexTypes_WriteAttribute(L"croptop=\"",		m_oCropTop);
			ComplexTypes_WriteAttribute(L"cropright=\"",	m_oCropRight);
			ComplexTypes_WriteAttribute(L"cropbottom=\"",	m_oCropBottom);

			if ( 1 != m_oGain.GetValue() )
				sResult += _T("gain=\"") + m_oGain.ToString() + _T("\" ");

			if ( 0 != m_oBlackLevel.GetValue() )
				sResult += _T("blacklevel=\"") + m_oBlackLevel.ToString() + _T("\" ");

			if ( 1 != m_oGamma.GetValue() )
				sResult += _T("gamma=\"") + m_oGamma.ToString() + _T("\" ");

			if ( SimpleTypes::booleanFalse != m_oGrayscale.GetValue() )
				sResult += _T("grayscale=\"true\" ");

			if ( SimpleTypes::booleanFalse != m_oBiLevel.GetValue() )
				sResult += _T("bilevel=\"true\" ");

			ComplexTypes_WriteAttribute ( _T("chromakey=\""),    m_oChromaKey );
			ComplexTypes_WriteAttribute ( _T("embosscolor=\""),  m_oEmbossColor );

			ComplexTypes_WriteAttribute3( _T("o:href=\""),             m_oHref );
			ComplexTypes_WriteAttribute3( _T("o:althref=\""),          m_sAltHref );
			ComplexTypes_WriteAttribute3( _T("o:title=\""),            m_sTitle );
			ComplexTypes_WriteAttribute3( _T("o:oleid=\""),            m_oOleId );
			ComplexTypes_WriteAttribute ( _T("o:detectmouseclick=\""), m_oDetectMouseClick );
			ComplexTypes_WriteAttribute ( _T("o:movie=\""),            m_oMovie );
			ComplexTypes_WriteAttribute ( _T("o:relid=\""),            m_oRelId );

			ComplexTypes_WriteAttribute ( _T("r:id=\""),               m_rId );
			ComplexTypes_WriteAttribute ( _T("r:pict=\""),             m_rPict );
			ComplexTypes_WriteAttribute ( _T("r:href=\""),             m_rHref );

			sResult += _T("/>");

			return sResult;
		}
		void CImage::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Выставляем значения по умолчанию
			m_oBlackLevel.SetValue( 0.0 );
			m_oCropLeft.SetValue( 0.0 );
			m_oCropRight.SetValue( 0.0 );
			m_oCropBottom.SetValue( 0.0 );
			m_oCropTop.SetValue( 0.0 );
			m_oGain.SetValue( 1.0 );
			m_oGamma.SetValue( 1.0 );

			// Читаем атрибуты
			if ( oReader.GetAttributesCount() <= 0 )
				return;
			
			if ( !oReader.MoveToFirstAttribute() )
				return;
			
			std::wstring wsName = oReader.GetName();
			while( !wsName.empty() )
			{
				wchar_t wsChar = wsName[0];
				switch ( wsChar )
				{
				case 'b':
					if      ( _T("bilevel")    == wsName ) m_oBiLevel    = oReader.GetText();
					else if ( _T("blacklevel") == wsName ) m_oBlackLevel = oReader.GetText();
					break;

				case 'c':
					if      ( _T("chromakey")  == wsName ) m_oChromaKey  = oReader.GetText();
					else if ( _T("cropleft")   == wsName ) m_oCropLeft   = oReader.GetText();
					else if ( _T("croptop")    == wsName ) m_oCropTop    = oReader.GetText();
					else if ( _T("cropright")  == wsName ) m_oCropRight  = oReader.GetText();
					else if ( _T("cropbottom") == wsName ) m_oCropBottom = oReader.GetText();
					break;

				case 'g':
					if      ( _T("gain")       == wsName ) m_oGain       = oReader.GetText();
					if      ( _T("gamma")      == wsName ) m_oGamma      = oReader.GetText();
					if      ( _T("grayscale")  == wsName ) m_oGrayscale  = oReader.GetText();
					break;

				case 's':
					if      ( _T("src")        == wsName ) m_sSrc       = oReader.GetText();
					break;
				}

				if ( !oReader.MoveToNextAttribute() )
					break;

				wsName = oReader.GetName();
			}
			oReader.MoveToElement();
		}

		std::wstring CImage::toXML() const
		{
			std::wstring sResult = _T("<v:image ");

			sResult += CVmlCommonElements::WriteAttributes();

			if ( _T("") !=  m_sSrc )
				sResult += _T("src=\"") + m_sSrc + _T("\" ");

			if ( 0 != m_oCropLeft.GetValue() )
				sResult += _T("cropleft=\"") + m_oCropLeft.ToString() + _T("\" ");

			if ( 0 != m_oCropTop.GetValue() )
				sResult += _T("croptop=\"") + m_oCropTop.ToString() + _T("\" ");

			if ( 0 != m_oCropRight.GetValue() )
				sResult += _T("cropright=\"") + m_oCropRight.ToString() + _T("\" ");

			if ( 0 != m_oCropBottom.GetValue() )
				sResult += _T("cropbottom=\"") + m_oCropBottom.ToString() + _T("\" ");

			if ( 1 != m_oGain.GetValue() )
				sResult += _T("gain=\"") + m_oGain.ToString() + _T("\" ");

			if ( 0 != m_oBlackLevel.GetValue() )
				sResult += _T("blacklevel=\"") + m_oBlackLevel.ToString() + _T("\" ");

			if ( 1 != m_oGamma.GetValue() )
				sResult += _T("gamma=\"") + m_oGamma.ToString() + _T("\" ");

			if ( SimpleTypes::booleanFalse != m_oGrayscale.GetValue() )
				sResult += _T("grayscale=\"true\" ");

			if ( SimpleTypes::booleanFalse != m_oBiLevel.GetValue() )
				sResult += _T("bilevel=\"true\" ");

			sResult += _T(">");

			sResult += CVmlCommonElements::WriteElements();

			sResult += _T("</v:image>");

			return sResult;
		}
		void CH::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Выставляем значения по умолчанию
			m_oMap.SetValue( 0, 1000 );
			m_oPosition.SetConstantX( 0 );
			m_oPosition.SetConstantY( 0 );
			m_oRadiusRange.SetValue( 0, 0 );
			m_oXRange.SetValue( 0, 0 );
			m_oYRange.SetValue( 0, 0 );

			// Читаем атрибуты
			if ( oReader.GetAttributesCount() <= 0 )
				return;
			
			if ( !oReader.MoveToFirstAttribute() )
				return;
			
			std::wstring wsName = oReader.GetName();
			while( !wsName.empty() )
			{
				wchar_t wsChar = wsName[0];
				switch ( wsChar )
				{
				case 'i':
					if      ( _T("invx") == wsName ) m_oInvX = oReader.GetText();
					else if ( _T("invy") == wsName ) m_oInvY = oReader.GetText();
					break;

				case 'm':
					if      ( _T("map") == wsName ) m_oMap = oReader.GetText();
					break;

				case 'p':
					if      ( _T("position") == wsName ) m_oPosition = oReader.GetText();
					else if ( _T("polar")    == wsName ) m_oPolar    = oReader.GetText();
					break;

				case 'r':
					if      ( _T("radiusrange") == wsName ) m_oRadiusRange = oReader.GetText();
					break;

				case 's':
					if      ( _T("switch") == wsName ) m_oSwitch = oReader.GetText();
					break;

				case 'x':
					if      ( _T("xrange") == wsName ) m_oXRange = oReader.GetText();
					break;
				case 'y':
					if      ( _T("yrange") == wsName ) m_oYRange = oReader.GetText();
					break;
				}

				if ( !oReader.MoveToNextAttribute() )
					break;

				wsName = oReader.GetName();
			}
			oReader.MoveToElement();
		}

		std::wstring CH::toXML() const
		{
			std::wstring sResult = _T("<v:h ");

			if ( SimpleTypes::Vml::vmlvector2dposConstant != m_oPosition.GetTypeX() || SimpleTypes::Vml::vmlvector2dposConstant != m_oPosition.GetTypeY() || 0 != m_oPosition.GetX() || 0 != m_oPosition.GetY() )
				sResult += _T("position=\"") + m_oPosition.ToString() + _T("\" ");
							
			ComplexTypes_WriteAttribute ( _T("polar=\""), m_oPolar );

			if ( 0 != m_oMap.GetX() || 1000 != m_oMap.GetY() )
				sResult += _T("map=\"") + m_oMap.ToString() + _T("\" ");

			if ( SimpleTypes::booleanFalse != m_oInvX.GetValue() )
				sResult += _T("invx=\"true\" ");

			if ( SimpleTypes::booleanFalse != m_oInvY.GetValue() )
				sResult += _T("invy=\"true\" ");

			if ( SimpleTypes::booleanFalse != m_oSwitch.GetValue() )
				sResult += _T("switch=\"true\" ");

			if ( 0 != m_oXRange.GetX() || 0 != m_oXRange.GetY() )
				sResult += _T("xrange=\"") + m_oXRange.ToString() + _T("\" ");

			if ( 0 != m_oYRange.GetX() || 0 != m_oYRange.GetY() )
				sResult += _T("yrange=\"") + m_oYRange.ToString() + _T("\" ");

			if ( 0 != m_oRadiusRange.GetX() || 0 != m_oRadiusRange.GetY() )
				sResult += _T("radiusrange=\"") + m_oRadiusRange.ToString() + _T("\" ");

			sResult += _T("/>");

			return sResult;
		}



	} // Vml
	namespace VmlOffice
	{
		void CShapeDefaults::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while ( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( _T("v:fill") == sName )
					m_oVmlFill = oReader;
				else if ( _T("v:stroke") == sName )
					m_oVmlStroke = oReader;
				else if ( _T("v:textbox") == sName )
					m_oVmlTextbox = oReader;
				else if ( _T("v:shadow") == sName )
					m_oVmlShadow = oReader;
				else if ( _T("o:skew") == sName )
					m_oSkew = oReader;
				else if ( _T("o:extrusion") == sName )
					m_oExtrusion = oReader;
				else if ( _T("o:callout") == sName )
					m_oCallout = oReader;
				else if ( _T("o:lock") == sName )
					m_oLock = oReader;
				else if ( _T("o:colormru") == sName )
					m_oColorMru = oReader;
				else if ( _T("o:colormenu") == sName )
					m_oColorMenu = oReader;
			}
		}
		void CShapeDefaults::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{				
			// Читаем атрибуты
			if ( oReader.GetAttributesCount() <= 0 )
				return;
			
			if ( !oReader.MoveToFirstAttribute() )
				return;
			
			std::wstring wsName = oReader.GetName();
			while( !wsName.empty() )
			{
				wchar_t wsChar = wsName[0];
				switch ( wsChar )
				{
				case 'f':
					if      ( _T("fill")          == wsName ) m_oFill        = oReader.GetText();
					else if ( _T("fillcolor")     == wsName ) m_oFillColor   = oReader.GetText();
					break;
				case 'o':
					if      ( _T("o:allowincell") == wsName ) m_oAllowInCell = oReader.GetText();
					break;
				case 's':
					if      ( _T("spidmax")       == wsName ) m_oSpIdMax     = oReader.GetText();
					else if ( _T("style")         == wsName ) m_oStyle       = oReader.GetText();
					else if ( _T("stroke")        == wsName ) m_oStroke      = oReader.GetText();
					else if ( _T("strokecolor")   == wsName ) m_oStrokeColor = oReader.GetText();
					break;
				case 'v':
					if      ( _T("v:ext")         == wsName ) m_oExt         = oReader.GetText();
					break;
				}

				if ( !oReader.MoveToNextAttribute() )
					break;

				wsName = oReader.GetName();
			}
			oReader.MoveToElement();
		}



		std::wstring CShapeDefaults::toXML() const
		{
			std::wstring sResult = _T("<o:shapedefaults ");

			ComplexTypes_WriteAttribute ( _T("v:ext=\""), m_oExt );

			if ( 0 != m_oSpIdMax.GetValue() )
				sResult += _T("spidmax=\"") + m_oSpIdMax.ToString() + _T("\" ");

			ComplexTypes_WriteAttribute ( _T("style=\""), m_oStyle );

			if ( SimpleTypes::booleanTrue != m_oFill.GetValue() )
				sResult += _T("fill=\"false\" ");

			ComplexTypes_WriteAttribute ( _T("fillcolor=\""), m_oFillColor );

			if ( SimpleTypes::booleanTrue != m_oStroke.GetValue() )
				sResult += _T("stroke=\"false\" ");

			if ( SimpleTypes::colortypeBlack != m_oStrokeColor.GetValue() )
				sResult += _T("strokecolor=\"") + m_oStrokeColor.ToString() + _T("\" ");

			if ( SimpleTypes::booleanFalse != m_oAllowInCell.GetValue() )
				sResult += _T("o:allowincell=\"true\" ");

			sResult += _T(">");

			if ( m_oVmlFill.IsInit() )
				sResult += m_oVmlFill->toXML();

			if ( m_oVmlStroke.IsInit() )
				sResult += m_oVmlStroke->toXML();

			if ( m_oVmlTextbox.IsInit() )
				sResult += m_oVmlTextbox->toXML();

			if ( m_oVmlShadow.IsInit() )
				sResult += m_oVmlShadow->toXML();

			if ( m_oSkew.IsInit() )
				sResult += m_oSkew->toXML();

			if ( m_oExtrusion.IsInit() )
				sResult += m_oExtrusion->toXML();

			if ( m_oCallout.IsInit() )
				sResult += m_oCallout->toXML();

			if ( m_oLock.IsInit() )
				sResult += m_oLock->toXML();

			if ( m_oColorMru.IsInit() )
				sResult += m_oColorMru->toXML();

			if ( m_oColorMenu.IsInit() )
				sResult += m_oColorMenu->toXML();

			sResult += _T("</o:shapedefaults>");

			return sResult;
		}
	}// VmlOffice
} // OOX
