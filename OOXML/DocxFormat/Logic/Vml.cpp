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
#include "../DocxFlat.h"
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
	namespace Vml
	{
		CVmlCommonElements::CVmlCommonElements(OOX::Document *pMain) : WritingElementWithChilds<>(pMain)
		{
		};
		CVmlCommonElements::~CVmlCommonElements(){}
		void CVmlCommonElements::fromXML(XmlUtils::CXmlNode& oNode){}
		void CVmlCommonElements::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes	(oReader);
			ReadElements	(oReader);
		}
		void CVmlCommonElements::CreateElement(XmlUtils::CXmlLiteReader& oReader)
		{
			OOX::Document* document = WritingElementWithChilds<>::m_pMainDocument;

			std::wstring sName = oReader.GetName();
			wchar_t wsChar1 = sName[0];
			wchar_t wsChar3 = sName[2];

			WritingElement *pItem = NULL;

			OOX::Vml::CClientData *client_data = NULL; // for set marker comment
			OOX::Vml::CImageData *image_data = NULL; // for set marker image
			switch (wsChar1)
			{
			case 'o':
				switch (wsChar3)
				{
				case 'c':

					if (L"o:callout" == sName)
						pItem = new OOX::VmlOffice::CCallout(document);
					else if (L"o:clippath" == sName)
						pItem = new OOX::VmlOffice::CClipPath(document);

					break;
				case 'e':

					if (L"o:extrusion" == sName)
						pItem = new OOX::VmlOffice::CExtrusion(document);

					break;
				case 'l':

					if (L"o:lock" == sName)
						pItem = new OOX::VmlOffice::CLock(document);

					break;
				case 's':

					if (L"o:signatureline" == sName)
						pItem = new OOX::VmlOffice::CSignatureLine(document);
					else if (L"o:skew" == sName)
						pItem = new OOX::VmlOffice::CSkew(document);

					break;
				}
				break;
			case 'p':
				//if (L"ppt:textdata" == sName )
				//	pItem = new OOX::Vml::CTextData( oReader );

				break;
			case 'v':

				switch (wsChar3)
				{
				case 'f':
					if (L"v:fill" == sName)
						pItem = new OOX::Vml::CFill(document);
					else if (L"v:formulas" == sName)
						pItem = new OOX::Vml::CFormulas(document);

					break;
				case 'h':

					if (L"v:handles" == sName)
						pItem = new OOX::Vml::CHandles(document);

					break;
				case 'i':

					if (L"v:imagedata" == sName)
						pItem = image_data = new OOX::Vml::CImageData(document);
					break;
				case 'p':

					if (L"v:path" == sName)
						pItem = new OOX::Vml::CPath(document);

					break;
				case 's':

					if (L"v:shadow" == sName)
						pItem = new OOX::Vml::CShadow(document);
					else if (L"v:stroke" == sName)
						pItem = new OOX::Vml::CStroke(document);

					break;
				case 't':

					if (L"v:textbox" == sName)
						pItem = new OOX::Vml::CTextbox(document);
					else if (L"v:textpath" == sName)
						pItem = new OOX::Vml::CTextPath(document);

					break;
				}
				break;
			case 'w':

				if (L"wd:anchorLock" == sName)
					pItem = new OOX::VmlWord::CAnchorLock(document);
				else if (L"wd:borderbottom" == sName)
					pItem = new OOX::VmlWord::CBorder(document);
				else if (L"wd:borderleft" == sName)
					pItem = new OOX::VmlWord::CBorder(document);
				else if (L"wd:borderright" == sName)
					pItem = new OOX::VmlWord::CBorder(document);
				else if (L"wd:bordertop" == sName)
					pItem = new OOX::VmlWord::CBorder(document);
				else if (L"wd:wrap" == sName)
					pItem = new OOX::VmlWord::CWrap(document);
				else if (L"w10:wrap" == sName)
					pItem = new OOX::VmlWord::CWrap(document);

				break;
			case 'x':

				if (L"x:ClientData" == sName)
				{
					pItem = client_data = new OOX::Vml::CClientData(document);
				}
				break;
			}

			if (NULL != pItem)
			{
				m_arrItems.push_back(pItem);
				pItem->fromXML(oReader);
			}

			if ((image_data) && (image_data->m_rId.IsInit()))
			{
				m_bImage = true;
			}
			if ((client_data) && (client_data->m_oObjectType.IsInit())
				&& (client_data->m_oObjectType->GetValue() == SimpleTypes::Vml::vmlclientdataobjecttypeNote))
			{
				m_bComment = true;
			}
		}
		void CVmlCommonElements::ReadElements(XmlUtils::CXmlLiteReader& oReader) 
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				CreateElement(oReader);
			}
		}
		void CVmlCommonElements::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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
				case 'a':
					if      (L"alt" == wsName ) m_sAlt = oReader.GetText();
					break;

				case 'c':
					if      (L"chromakey"   == wsName ) m_oChromaKey   = oReader.GetText();
					else if (L"class"       == wsName ) m_sClass       = oReader.GetText();
					else if (L"coordorigin" == wsName ) m_oCoordOrigin = oReader.GetText();
					else if (L"coordsize"   == wsName ) m_oCoordSize   = oReader.GetText();
					break;

				case 'f':
					if      (L"fillcolor" == wsName ) m_oFillColor = oReader.GetText();
					else if (L"filled"    == wsName ) m_oFilled    = oReader.GetText();
					break;

				case 'h':
					if      (L"href" == wsName ) m_sHref = oReader.GetText();
					break;

				case 'i':
					if      (L"id"       == wsName ) m_sId       = oReader.GetText();
					else if (L"insetpen" == wsName ) m_oInsetPen = oReader.GetText();
					break;

				case 'o':
					{
						wchar_t wsChar2 = wsName[2]; // o:_
						switch ( wsChar2 )
						{
						case 'a':
							if      (L"o:allowincell"  == wsName ) m_oAllowInCell  = oReader.GetText();
							else if (L"o:allowoverlap" == wsName ) m_oAllowOverlap = oReader.GetText();
							else if (L"opacity"        == wsName ) m_oOpacity      = oReader.GetText();
							break;
						case 'b':
							if      (L"o:borderbottomcolor" == wsName ) m_oBorderBottomColor = oReader.GetText();
							else if (L"o:borderleftcolor"   == wsName ) m_oBorderLeftColor   = oReader.GetText();
							else if (L"o:borderrightcolor"  == wsName ) m_oBorderRightColor  = oReader.GetText();
							else if (L"o:bordertopcolor"    == wsName ) m_oBorderTopColor    = oReader.GetText();
							else if (L"o:bullet"            == wsName ) m_oBullet            = oReader.GetText();
							else if (L"o:button"            == wsName ) m_oButton            = oReader.GetText();
							else if (L"o:bwmode"            == wsName ) m_oBwMode            = oReader.GetText();
							else if (L"o:bwnormal"          == wsName ) m_oBwNormal           = oReader.GetText();
							else if (L"o:bwpure"            == wsName ) m_oBwPure            = oReader.GetText();
							break;
						case 'c':  
							if      (L"o:clip"          == wsName ) m_oClip          = oReader.GetText();
							else if (L"o:cliptowrap"    == wsName ) m_oClipToWrap    = oReader.GetText();
							else if (L"o:connectortype" == wsName ) m_oConnectorType = oReader.GetText();
							break;
						case 'd':
							if      (L"o:doubleclicknotify" == wsName ) m_oDoubleClickNotify = oReader.GetText();
							else if (L"o:dgmlayout"         == wsName ) m_oDgmLayout         = oReader.GetText();
							else if (L"o:dgmlayoutmru"      == wsName ) m_oDgmLayoutMru      = oReader.GetText();
							else if (L"o:dgmnodekind"       == wsName ) m_oDgmNodeKind       = oReader.GetText();
							break;
						case 'f':
							if      (L"o:forcedash" == wsName ) m_oForceDash = oReader.GetText();
							break;
						case 'h':  
							if      (L"o:hr"        == wsName ) m_oHr        = oReader.GetText();
							else if (L"o:hralign"   == wsName ) m_oHrAlign   = oReader.GetText();
							else if (L"o:hrnoshade" == wsName ) m_oHrNoShade = oReader.GetText();
							else if (L"o:hrpct"     == wsName ) m_oHrPct     = oReader.GetText();
							else if (L"o:hrstd"     == wsName ) m_oHrStd     = oReader.GetText();
							break;
						case 'i':
							if      (L"o:insetmode" == wsName ) m_oInsetMode = oReader.GetText();
							break;
						case 'o':  
							if      (L"o:ole"     == wsName ) m_oOle        = oReader.GetText();
							else if (L"o:oleicon" == wsName ) m_oOleIcon    = oReader.GetText();
							else if (L"o:oned"    == wsName ) m_oOned       = oReader.GetText();
							break;
						case 'p':
							if      (L"o:preferrelative" == wsName ) m_oPreferRelative = oReader.GetText();
							break;
						case 'r':
							if      (L"o:regroupid" == wsName ) m_oRegroupId = oReader.GetText();
							break;
						case 's':
							if      (L"o:spid" == wsName ) m_sSpId = oReader.GetText();
							else if (L"o:spt"  == wsName ) m_oSpt  = oReader.GetText();
							break;
						case 'u':
							if      (L"o:userdrawn"  == wsName ) m_oUserDrawn  = oReader.GetText();
							else if (L"o:userhidden" == wsName ) m_oUserHidden = oReader.GetText();
							break;
						}

					break;
					}

				case 'p':
					if      (L"print" == wsName ) m_oPrint = oReader.GetText();
					break;
				case 's':
					if      (L"strokecolor"  == wsName ) m_oStrokeColor  = oReader.GetText();
					else if (L"stroked"      == wsName ) m_oStroked      = oReader.GetText();
					else if (L"strokeweight" == wsName ) m_oStrokeWeight = oReader.GetText();
					else if (L"style"        == wsName ) m_oStyle        = oReader.GetText();
					break;
				case 't':
					if      (L"target" == wsName ) m_sTarget = oReader.GetText();
					else if (L"title"  == wsName ) m_sTitle  = oReader.GetText();
					break;
				case 'w':
					if      (L"wrapcoords" == wsName ) m_oWrapCoords = oReader.GetText();
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
		void CVmlCommonElements::mergeFrom(CVmlCommonElements* parent)
		{
			if (!parent) return;

			if (false == m_oStyle.IsInit()) m_oStyle = parent->m_oStyle;
			else
			{
				m_oStyle->mergeFrom(parent->m_oStyle.GetPointer());
			}
			if (false == m_sHref.IsInit())			m_sHref = parent->m_sHref;
			if (false == m_sTarget.IsInit())		m_sTarget = parent->m_sTarget;
			if (false == m_sClass.IsInit())			m_sClass = parent->m_sClass;
			if (false == m_sTitle.IsInit())			m_sTitle = parent->m_sTitle;
			if (false == m_sAlt.IsInit())			m_sAlt = parent->m_sAlt;
			if (false == m_oCoordSize.IsInit())		m_oCoordSize = parent->m_oCoordSize;
			if (false == m_oCoordOrigin.IsInit())	m_oCoordOrigin = parent->m_oCoordOrigin;
			if (false == m_oWrapCoords.IsInit())	m_oWrapCoords = parent->m_oWrapCoords;
			if (false == m_oPrint.IsInit())			m_oPrint = parent->m_oPrint;

			if (false == m_sSpId.IsInit())			m_sSpId = parent->m_sSpId;
			if (false == m_oOned.IsInit())			m_oOned = parent->m_oOned;
			if (false == m_oRegroupId.IsInit())		m_oRegroupId = parent->m_oRegroupId;
			if (false == m_oDoubleClickNotify.IsInit())  m_oDoubleClickNotify = parent->m_oDoubleClickNotify;
			if (false == m_oButton.IsInit())		m_oButton = parent->m_oButton;
			if (false == m_oUserHidden.IsInit())	m_oUserHidden = parent->m_oUserHidden;
			if (false == m_oBullet.IsInit())		m_oBullet = parent->m_oBullet;
			if (false == m_oHr.IsInit())			m_oHr = parent->m_oHr;
			if (false == m_oHrStd.IsInit())			m_oHrStd = parent->m_oHrStd;
			if (false == m_oHrNoShade.IsInit())		m_oHrNoShade = parent->m_oHrNoShade;
			if (false == m_oHrPct.IsInit())			m_oHrPct = parent->m_oHrPct;
			if (false == m_oHrAlign.IsInit())		m_oHrAlign = parent->m_oHrAlign;
			if (false == m_oAllowInCell.IsInit())	m_oAllowInCell = parent->m_oAllowInCell;
			if (false == m_oAllowOverlap.IsInit())  m_oAllowOverlap = parent->m_oAllowOverlap;
			if (false == m_oUserDrawn.IsInit())		m_oUserDrawn = parent->m_oUserDrawn;
			if (false == m_oBorderTopColor.IsInit())  m_oBorderTopColor = parent->m_oBorderTopColor;
			if (false == m_oBorderLeftColor.IsInit())  m_oBorderLeftColor = parent->m_oBorderLeftColor;
			if (false == m_oBorderBottomColor.IsInit())  m_oBorderBottomColor = parent->m_oBorderBottomColor;
			if (false == m_oBorderRightColor.IsInit())  m_oBorderRightColor = parent->m_oBorderRightColor;
			if (false == m_oDgmLayout.IsInit())		m_oDgmLayout = parent->m_oDgmLayout;
			if (false == m_oDgmNodeKind.IsInit())	m_oDgmNodeKind = parent->m_oDgmNodeKind;
			if (false == m_oDgmLayoutMru.IsInit())  m_oDgmLayoutMru = parent->m_oDgmLayoutMru;
			if (false == m_oInsetMode.IsInit())		m_oInsetMode = parent->m_oInsetMode;

			if (false == m_oChromaKey.IsInit())		m_oChromaKey = parent->m_oChromaKey;
			if (false == m_oFilled.IsInit())		m_oFilled = parent->m_oFilled;
			if (false == m_oFillColor.IsInit())		m_oFillColor = parent->m_oFillColor;
			if (false == m_oOpacity.IsInit())		m_oOpacity = parent->m_oOpacity;
			if (false == m_oStroked.IsInit())		m_oStroked = parent->m_oStroked;
			if (false == m_oStrokeColor.IsInit())	m_oStrokeColor = parent->m_oStrokeColor;
			if (false == m_oStrokeWeight.IsInit())  m_oStrokeWeight = parent->m_oStrokeWeight;
			if (false == m_oInsetPen.IsInit())		m_oInsetPen = parent->m_oInsetPen;

			if (false == m_oSpt.IsInit())			m_oSpt = parent->m_oSpt;
			if (false == m_oConnectorType.IsInit())	m_oConnectorType = parent->m_oConnectorType;
			if (false == m_oBwMode.IsInit())		m_oBwMode = parent->m_oBwMode;
			if (false == m_oBwPure.IsInit())		m_oBwPure = parent->m_oBwPure;
			if (false == m_oBwNormal.IsInit())		m_oBwNormal = parent->m_oBwNormal;
			if (false == m_oForceDash.IsInit())		m_oForceDash = parent->m_oForceDash;
			if (false == m_oOleIcon.IsInit())		m_oOleIcon = parent->m_oOleIcon;
			if (false == m_oOle.IsInit())			m_oOle = parent->m_oOle;
			if (false == m_oPreferRelative.IsInit())  m_oPreferRelative = parent->m_oPreferRelative;
			if (false == m_oClipToWrap.IsInit())	m_oClipToWrap = parent->m_oClipToWrap;
			if (false == m_oClip.IsInit())			m_oClip = parent->m_oClip;
		}
		std::wstring CVmlCommonElements::WriteAttributes() const
		{
			std::wstring sResult;

			ComplexTypes_WriteAttribute3(L"id=\"",          m_sId );
			ComplexTypes_WriteAttribute (L"style=\"",       m_oStyle );
			ComplexTypes_WriteAttribute3(L"href=\"",        m_sHref );
			ComplexTypes_WriteAttribute3(L"target=\"",      m_sTarget );
			ComplexTypes_WriteAttribute3(L"class=\"",       m_sClass );
			ComplexTypes_WriteAttribute3(L"title=\"",       m_sTitle );
			ComplexTypes_WriteAttribute3(L"alt=\"",         m_sAlt );
			ComplexTypes_WriteAttribute (L"coordsize=\"",   m_oCoordSize );
			ComplexTypes_WriteAttribute (L"coordorigin=\"", m_oCoordOrigin );
			ComplexTypes_WriteAttribute (L"wrapcoords=\"",  m_oWrapCoords );

			if (!m_oPrint.get_value_or(true))
				sResult += L"print=\"f\" ";

			ComplexTypes_WriteAttribute3(L"o:spid=\"", m_sSpId );

			if (m_oOned.get_value_or(false))
				sResult += L"o:oned=\"t\" ";
			
			if (m_oRegroupId.IsInit())
				sResult += L"o:regroupid=\"" + std::to_wstring(*m_oRegroupId) + L"\" ";

			if (m_oDoubleClickNotify.get_value_or(false))
				sResult += L"o:doubleclicknotify=\"t\" ";

			if (m_oButton.get_value_or(false))
				sResult += L"o:button=\"t\" ";

			if (m_oUserHidden.get_value_or(false))
				sResult += L"o:userhidden=\"t\" ";

			if (m_oBullet.get_value_or(false))
				sResult += L"o:bullet=\"t\" ";

			if (m_oHr.get_value_or(false))
				sResult += L"o:hr=\"t\" ";

			if (m_oHrStd.get_value_or(false))
				sResult += L"o:hrstd=\"t\" ";

			if (m_oHrNoShade.get_value_or(false))
				sResult += L"o:hrnoshade=\"t\" ";

			if (m_oHrPct.IsInit())
				sResult += L"o:hrpct=\"" + std::to_wstring(*m_oHrPct) + L"\" ";

			ComplexTypes_WriteAttribute(L"o:hralign=\"", m_oHrAlign);

			if (m_oAllowInCell.get_value_or(false))
				sResult += L"o:allowincell=\"t\" ";

			if (m_oAllowOverlap.IsInit())
				sResult += L"o:allowoverlap=\"" + std::wstring(*m_oAllowOverlap ? L"t" : L"f") + L"\" ";

			if (m_oUserDrawn.get_value_or(false))
				sResult += L"o:userdrawn=\"t\" ";

			ComplexTypes_WriteAttribute (L"o:bordertopcolor=\"",    m_oBorderTopColor );
			ComplexTypes_WriteAttribute (L"o:borderleftcolor=\"",   m_oBorderLeftColor );
			ComplexTypes_WriteAttribute (L"o:borderbottomcolor=\"", m_oBorderBottomColor );
			ComplexTypes_WriteAttribute (L"o:borderrightcolor=\"",  m_oBorderRightColor );
			ComplexTypes_WriteAttribute (L"o:dgmlayout=\"",     m_oDgmLayout );
			ComplexTypes_WriteAttribute (L"o:dgmlayoutmru=\"",  m_oDgmLayoutMru );
			ComplexTypes_WriteAttribute3(L"o:dgmnodekind=\"",   m_oDgmNodeKind );
			ComplexTypes_WriteAttribute (L"o:insetmode=\"" , m_oInsetMode);
			ComplexTypes_WriteAttribute (L"chromakey=\"",  m_oChromaKey );
			ComplexTypes_WriteAttribute (L"filled=\"",		m_oFilled );
			ComplexTypes_WriteAttribute (L"fillcolor=\"",	m_oFillColor );
			ComplexTypes_WriteAttribute (L"opacity=\"",	m_oOpacity );
			ComplexTypes_WriteAttribute (L"stroked=\"", m_oStroked );
			ComplexTypes_WriteAttribute (L"strokecolor=\"", m_oStrokeColor);
			ComplexTypes_WriteAttribute (L"strokeweight=\"", m_oStrokeWeight);
			ComplexTypes_WriteAttribute (L"insetpen=\"",  m_oInsetPen );
			ComplexTypes_WriteAttribute (L"o:spt=\"", m_oSpt);
			ComplexTypes_WriteAttribute (L"o:connectortype=\"", m_oConnectorType);
			ComplexTypes_WriteAttribute (L"o:bwmode=\"",   m_oBwMode );
			ComplexTypes_WriteAttribute (L"o:bwpure=\"",   m_oBwPure );
			ComplexTypes_WriteAttribute (L"o:bwnormal=\"", m_oBwNormal );

			if (m_oForceDash.get_value_or(false))
				sResult += L"o:forcedash=\"t\" ";

			if (m_oOleIcon.get_value_or(false))
				sResult += L"o:oleicon=\"t\" ";

			if (m_oOle.get_value_or(false))
				sResult += L"o:ole=\"t\" ";

			if (m_oPreferRelative.get_value_or(false))
				sResult += L"o:preferrelative=\"t\" ";

			if (m_oClipToWrap.get_value_or(false))
				sResult += L"o:cliptowrap=\"t\" ";

			if (m_oClipToWrap.IsInit())
				sResult += L"o:clip=\"" + std::wstring(*m_oClip ? L"t" : L"f") + L"\" ";

			return sResult;
		}
		std::wstring CVmlCommonElements::toXML() const
		{
			return L"";
		}
		EElementType CVmlCommonElements::getType() const
		{
			return et_v_shape_elements;
		}

		//--------------------------------------------------------------------------------
		// CArc 14.1.2.1 (Part4)
		//--------------------------------------------------------------------------------
		CArc::CArc(OOX::Document *pMain) : CVmlCommonElements(pMain)
		{
		}
		CArc::~CArc(){}
		void CArc::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CArc::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CArc::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			CVmlCommonElements::ReadAttributes( oReader );
			CVmlCommonElements::ReadElements( oReader );
		}
		std::wstring CArc::toXML() const
		{
			std::wstring sResult = _T("<v:arc ");

			sResult += CVmlCommonElements::WriteAttributes();

			sResult += _T("startangle=\"") + m_oStartAngle.ToString() + _T("\" ");
			sResult += _T("endangle=\"")   + m_oEndAngle.ToString()   + _T("\">");

			sResult += CVmlCommonElements::WriteElements();

			sResult += _T("</v:arc>");

			return sResult;
		}
		EElementType CArc::getType() const
		{
			return OOX::et_v_arc;
		}
		void CArc::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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
					case 'e':
						if      ( _T("endangle") == wsName || _T("endAngle") == wsName ) m_oEndAngle   = oReader.GetText();
						break;

					case 's':
						if      ( _T("startangle") == wsName || _T("startAngle") == wsName ) m_oStartAngle = oReader.GetText();
						break;
					}

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}
				oReader.MoveToElement();
			}

		//--------------------------------------------------------------------------------
		// CCurve 14.1.2.3 (Part4)
		//--------------------------------------------------------------------------------
		CCurve::CCurve(OOX::Document *pMain) : CVmlCommonElements(pMain)
		{
		}
		CCurve::~CCurve(){}
		void CCurve::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CCurve::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CCurve::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );
			CVmlCommonElements::ReadAttributes( oReader );
			CVmlCommonElements::ReadElements( oReader );
		}
		std::wstring CCurve::toXML() const
		{
			std::wstring sResult = _T("<v:curve ");

			sResult += CVmlCommonElements::WriteAttributes();

			sResult += _T("from=\"")     + m_oFrom.ToString()     + _T("\" ");
			sResult += _T("control1=\"") + m_oControl1.ToString() + _T("\" ");
			sResult += _T("control2=\"") + m_oControl2.ToString() + _T("\" ");
			sResult += _T("to=\"")       + m_oTo.ToString()       + _T("\">");

			sResult += CVmlCommonElements::WriteElements();

			sResult += _T("</v:curve>");

			return sResult;
		}
		EElementType CCurve::getType() const
		{
			return OOX::et_v_curve;
		}
		void CCurve::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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
					case 'c':
						if      ( _T("control1") == wsName ) m_oControl1 = oReader.GetText();
						else if ( _T("control2") == wsName ) m_oControl2 = oReader.GetText();
						break;

					case 'f':
						if      ( _T("from")     == wsName ) m_oFrom     = oReader.GetText();
						break;

					case 't':
						if      ( _T("to")       == wsName ) m_oTo       = oReader.GetText();
						break;
					}

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}
				oReader.MoveToElement();
			}

		//--------------------------------------------------------------------------------
		// CF 14.1.2.4 (Part4)
		//--------------------------------------------------------------------------------
		CF::CF()
		{
		}
		CF::~CF()
		{
		}
		void CF::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CF::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CF::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CF::toXML() const
		{
			std::wstring sResult = _T("<v:f eqn=\"") + m_sEqn + _T("\"/>");
			return sResult;
		}
		EElementType CF::getType() const
		{
			return OOX::et_v_f;
		}
		void CF::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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
					case 'e':
						if      ( _T("eqn") == wsName ) m_sEqn = oReader.GetText();
						break;
					}

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}
				oReader.MoveToElement();

				// TO DO: Сделать парсер формул ( или использовать уже сделанный парсер в OfficeDrawing\Shapes)
			}

		//--------------------------------------------------------------------------------
		// CGroup 14.1.2.6 (Part4)
		//--------------------------------------------------------------------------------	
		CGroup::CGroup(OOX::Document *pMain) : CVmlCommonElements(pMain)
		{
		}
		CGroup::~CGroup(){}
		EElementType CGroup::getType() const
		{
			return OOX::et_v_group;
		}
		void CGroup::fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		void CGroup::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);
			CVmlCommonElements::ReadAttributes(oReader);

			if ( oReader.IsEmptyNode() )
				return;

			OOX::Document* document = WritingElementWithChilds<>::m_pMainDocument;

			int nCurDepth = oReader.GetDepth();
			while ( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				WritingElement *pItem = NULL;
				std::wstring sName = oReader.GetName();

				if (L"v:arc" == sName)
					pItem = new OOX::Vml::CArc(document);
				else if (L"v:curve" == sName)
					pItem = new OOX::Vml::CCurve(document);
				else if (L"v:line" == sName)
					pItem = new OOX::Vml::CLine(document);
				else if (L"v:image" == sName)
					pItem = new OOX::Vml::CImage(document);
				else if (L"v:group" == sName)
					pItem = new OOX::Vml::CGroup(document);
				else if (L"v:oval" == sName)
					pItem = new OOX::Vml::COval(document);
				else if (L"v:shape" == sName)
					pItem = new OOX::Vml::CShape(document);
				else if (L"v:polyline" == sName)
					pItem = new OOX::Vml::CPolyLine(document);
				else if (L"v:rect" == sName)
					pItem = new OOX::Vml::CRect(document);
				else if (L"v:roundrect" == sName)
					pItem = new OOX::Vml::CRoundRect(document);
				else if (L"v:shapetype" == sName)
				{
					m_arrShapeTypes.push_back(new OOX::Vml::CShapeType(document));
					m_arrShapeTypes.back()->fromXML(oReader);
				}
				else
					CreateElement(oReader);

				if ( NULL != pItem )
				{
					m_arrElements.push_back( pItem );
					pItem->fromXML(oReader);
				}
			}
		}
		void CGroup::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"o:tableproperties", m_oTableProperties)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"o:tablelimits", m_oTableLimits)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"w:editas", m_oEditAs)
			WritingElement_ReadAttributes_End(oReader)
		}
		std::wstring CGroup::toXML() const
		{
			std::wstring sResult = L"<v:group ";

			sResult += CVmlCommonElements::WriteAttributes();
			ComplexTypes_WriteAttribute (L"editas=\"", m_oEditAs );
			if ( 0 != m_oTableProperties.GetValue() )
				sResult += L"o:tableproperties=\"" + m_oTableProperties.ToString() + L"\" ";
			ComplexTypes_WriteAttribute (L"o:tablelimits=\"",  m_oTableLimits );

			sResult += L">";

			sResult += CVmlCommonElements::WriteElements();

			sResult += L"</v:group>";

			return sResult;
		}

		//--------------------------------------------------------------------------------
		// CPolyLine 14.1.2.15 (Part4)
		//--------------------------------------------------------------------------------
		CPolyLine::CPolyLine(OOX::Document *pMain) : CVmlCommonElements(pMain)
		{
		}
		CPolyLine::~CPolyLine(){}
		void CPolyLine::fromXML(XmlUtils::CXmlNode& oNode){}
		void CPolyLine::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );
			CVmlCommonElements::ReadAttributes( oReader );
			CVmlCommonElements::ReadElements( oReader );
		}
		EElementType CPolyLine::getType() const
		{
			return OOX::et_v_polyline;
		}
		void CPolyLine::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Выставляем значения по умолчанию
				m_oPoints.SetDelimiter( ' ' );

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
					case 'p':
						if      ( _T("points") == wsName ) m_oPoints = oReader.GetText();
						break;
					}

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}
				oReader.MoveToElement();
			}
		std::wstring CPolyLine::toXML() const
		{
			std::wstring sResult = L"<v:polyline ";

			sResult += CVmlCommonElements::WriteAttributes();

			sResult += L"points=\"" + m_oPoints.ToString() + L"\">";

			sResult += CVmlCommonElements::WriteElements();

			sResult += L"</v:polyline>";

			return sResult;
		}

		//--------------------------------------------------------------------------------
		// CRect 14.1.2.16 (Part4)
		//--------------------------------------------------------------------------------
		CRect::CRect(OOX::Document *pMain) : CVmlCommonElements(pMain)
		{
		}
		CRect::~CRect(){}
		void CRect::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CRect::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CRect::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			CVmlCommonElements::ReadAttributes( oReader );
			CVmlCommonElements::ReadElements( oReader );
		}
		std::wstring CRect::toXML() const
		{
			std::wstring sResult = _T("<v:rect ");

			sResult += CVmlCommonElements::WriteAttributes();

			sResult += _T(">");

			sResult += CVmlCommonElements::WriteElements();

			sResult += _T("</v:rect>");

			return sResult;
		}
		EElementType CRect::getType() const
			{
				return OOX::et_v_rect;
			}

		//--------------------------------------------------------------------------------
		// CRoundRect 14.1.2.17 (Part4)
		//--------------------------------------------------------------------------------
		CRoundRect::CRoundRect(OOX::Document *pMain) : CVmlCommonElements(pMain)
		{
		}
		CRoundRect::~CRoundRect(){}
		void CRoundRect::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CRoundRect::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CRoundRect::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );
			CVmlCommonElements::ReadAttributes( oReader );
			CVmlCommonElements::ReadElements( oReader );
		}
		std::wstring CRoundRect::toXML() const
		{
			std::wstring sResult = _T("<v:roundrect ");

			sResult += CVmlCommonElements::WriteAttributes();

			sResult += _T("arcsize=\"") + m_oArcSize.ToString() + _T("\" ");

			sResult += CVmlCommonElements::WriteElements();

			sResult += _T("</v:roundrect>");

			return sResult;
		}
		EElementType CRoundRect::getType() const
		{
			return OOX::et_v_roundrect;
		}
		void CRoundRect::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Выставляем значения по умолчанию
				m_oArcSize.SetPercentage( 20 );

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
						if      ( _T("arcsize") == wsName ) m_oArcSize = oReader.GetText();
						break;
					}

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}
				oReader.MoveToElement();
			}

		//--------------------------------------------------------------------------------
		// CShadow 14.1.2.18 (Part4)
		//--------------------------------------------------------------------------------
		CShadow::CShadow(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CShadow::~CShadow()
		{
		}
		void CShadow::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase(oNode, L"on", m_oOn);
			XmlMacroReadAttributeBase(oNode, L"id", m_oId);
			XmlMacroReadAttributeBase(oNode, L"type", m_oType);
			XmlMacroReadAttributeBase(oNode, L"color", m_oColor);
			XmlMacroReadAttributeBase(oNode, L"opacity", m_oOpacity);
			XmlMacroReadAttributeBase(oNode, L"offset", m_oOffset);
			XmlMacroReadAttributeBase(oNode, L"origin", m_oOrigin);
			XmlMacroReadAttributeBase(oNode, L"obscured", m_oObscured);
			XmlMacroReadAttributeBase(oNode, L"color2", m_oColor2);
			XmlMacroReadAttributeBase(oNode, L"offset2", m_oOffset2);
			XmlMacroReadAttributeBase(oNode, L"matrix", m_oMatrix);
		}
		void CShadow::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CShadow::toXML() const
		{
			std::wstring sResult = _T("<v:shadow ");

			ComplexTypes_WriteAttribute3( L"id=\"", m_oId );

			if ( SimpleTypes::booleanTrue != m_oOn.GetValue() )
				sResult += L"on=\"false\" ";

			if ( SimpleTypes::shadowtypeSingle != m_oType.GetValue() )
				sResult += _T("type=\"") + m_oType.ToString() + _T("\" ");

			if ( SimpleTypes::booleanFalse != m_oObscured.GetValue() )
				sResult += _T("obscured=\"true\" ");

			if ( 128 != m_oColor.Get_R() || 128 != m_oColor.Get_G() || 128 != m_oColor.Get_B() )
				sResult += _T("color=\"") + m_oColor.ToString() + _T("\" ");

			if (m_oOpacity.IsInit())
				sResult += _T("opacity=\"") + m_oOpacity->ToString() + _T("\" ");

			sResult += _T("offset=\"") + m_oOffset.ToString() + _T("\" ");

			if ( 203 != m_oColor2.Get_R() || 203 != m_oColor2.Get_G() || 203 != m_oColor2.Get_B() )
				sResult += _T("color2=\"") + m_oColor2.ToString() + _T("\" ");

			sResult += _T("offset2=\"") + m_oOffset2.ToString() + _T("\" ");

			if ( 0 != m_oOrigin.GetX() || 0 != m_oOrigin.GetY() )
				sResult += _T("origin=\"") + m_oOrigin.ToString() + _T("\" ");

			ComplexTypes_WriteAttribute ( _T("matrix=\""), m_oMatrix );

			sResult += _T("/>");

			return sResult;
		}
		EElementType CShadow::getType() const
		{
			return OOX::et_v_shadow;
		}
		void CShadow::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Выставляем значения по умолчанию
				m_oColor.SetRGB( 128, 128, 128 );
				m_oColor2.SetRGB( 203, 203, 203 );
				m_oOffset.SetValue_Points( 2, 2 );
				m_oOffset2.SetValue_Points( -2, -2 );
				m_oOrigin.SetValue( 0, 0 );

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
						if      ( _T("color")      == wsName ) m_oColor      = oReader.GetText();
						else if ( _T("color2")     == wsName ) m_oColor2     = oReader.GetText();
						break;

					case 'i':
						if      ( _T("id")         == wsName ) m_oId         = oReader.GetText();
						break;

					case 'm':
						if      ( _T("matrix")     == wsName ) m_oMatrix     = oReader.GetText();
						break;

					case 'o':
						if      ( _T("obscured")   == wsName ) m_oObscured   = oReader.GetText();
						else if ( _T("offset")     == wsName ) m_oOffset     = oReader.GetText();
						else if ( _T("offset2")    == wsName ) m_oOffset2    = oReader.GetText();
						else if ( _T("on")         == wsName ) m_oOn         = oReader.GetText();
						else if ( _T("opacity")    == wsName ) m_oOpacity    = oReader.GetText();
						else if ( _T("origin")     == wsName ) m_oOrigin     = oReader.GetText();
						break;

					case 't':
						if      ( _T("type")       == wsName ) m_oType       = oReader.GetText();
						break;
					}

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}
				oReader.MoveToElement();
			}

		//--------------------------------------------------------------------------------
		// CShape 14.1.2.19 (Part4)
		//--------------------------------------------------------------------------------	
		CShape::CShape(OOX::Document *pMain) : CVmlCommonElements(pMain)
		{
		}
		CShape::~CShape(){}
		void CShape::fromXML(XmlUtils::CXmlNode& oNode){}
		void CShape::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );
			CVmlCommonElements::ReadAttributes( oReader );
			CVmlCommonElements::ReadElements( oReader );
		}
		EElementType CShape::getType() const
		{
			return OOX::et_v_shape;
		}
		void CShape::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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
				case 'a':
					if      ( _T("adj")         == wsName ) m_sAdj         = oReader.GetText();
					break;
				case 'e':
					if      ( _T("equationxml") == wsName ) m_sEquationXML = oReader.GetText();
					break;
				case 'o':
					if      ( _T("o:gfxdata")   == wsName ) m_sGfxData       = oReader.GetText();
					break;
				case 'p':
					if      ( _T("path")        == wsName ) m_oPath        = oReader.GetText();
					break;
				case 't':
					if      ( _T("type")        == wsName ) m_sType        = oReader.GetText();
					break;
				}

				if ( !oReader.MoveToNextAttribute() )
					break;

				wsName = oReader.GetName();
			}
			oReader.MoveToElement();

			// TO DO: Сделать парсер Adj
		}
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

			if ( m_sEquationXML.IsInit() )
			{
				sResult += L"equationxml=\"" + *m_sEquationXML + L"\" ";
			}

			sResult += L">";

			sResult += CVmlCommonElements::WriteElements();

			sResult += L"</v:shape>";

			return sResult;
		}
		void CShape::mergeFrom(CShapeType* shape_type)
		{
			if (!shape_type) return;

			if (false == m_oPath.IsInit()) m_oPath = shape_type->m_oPath;
			if (false == m_sAdj.IsInit()) m_sAdj = shape_type->m_sAdj;

			CVmlCommonElements::mergeFrom(shape_type);
		}

		//--------------------------------------------------------------------------------
		// CShapeType 14.1.2.20 (Part4)
		//--------------------------------------------------------------------------------	
		CShapeType::CShapeType(OOX::Document *pMain) : CVmlCommonElements(pMain)
		{
		}
		CShapeType::~CShapeType() {}
		void CShapeType::fromXML(XmlUtils::CXmlNode& oNode) {}
		void CShapeType::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);
			CVmlCommonElements::ReadAttributes(oReader);
			CVmlCommonElements::ReadElements(oReader);
		}
		EElementType CShapeType::getType() const
		{
			return OOX::et_v_shapetype;
		}
		void CShapeType::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.GetAttributesCount() <= 0)
				return;
			if (!oReader.MoveToFirstAttribute())
				return;

			std::wstring wsName = oReader.GetName();
			while (!wsName.empty())
			{
				wchar_t wsChar = wsName[0];
				switch (wsChar)
				{
				case 'o':
					if (_T("o:master") == wsName) m_oMaster = oReader.GetText();
					break;
				case 'p':
					if (_T("path") == wsName) m_oPath = oReader.GetText();
					break;
				case 'a':
					if (_T("adj") == wsName) m_sAdj = oReader.GetText();
					break;
				case 't':
					if (_T("type") == wsName) m_sType = oReader.GetText(); // для некоторых багнутых файлов
					break;
				}

				if (!oReader.MoveToNextAttribute())
					break;

				wsName = oReader.GetName();
			}
			oReader.MoveToElement();

			// TO DO: Сделать парсер Adj
		}
		std::wstring CShapeType::toXML() const
		{
			std::wstring sResult = L"<v:shapetype ";

			sResult += CVmlCommonElements::WriteAttributes();

            if ( m_sAdj.IsInit() )
			{
				sResult += L"adj=\"" + *m_sAdj + L"\" ";
			}

			if ( m_oPath.IsInit() )
				sResult += L"path=\"" + m_oPath->ToString() + L"\" ";

			if ((m_oMaster.IsInit()) && (*m_oMaster))
				sResult += L"o:master=\"t\" ";

			sResult += L">";

			sResult += CVmlCommonElements::WriteElements();

			sResult += L"</v:shapetype>";

			return sResult;
		}

		CClientData::CClientData(OOX::Document *pMain) : WritingElement(pMain) {}
		CClientData::~CClientData() {}
		EElementType CClientData::getType() const
		{
			return OOX::et_v_ClientData;
		}
		void CClientData::getAnchorArray(std::vector<int>& aAnchor) const
		{
			aAnchor.clear();
			if(m_oAnchor.IsInit())
			{
				std::vector<std::wstring> arSplit;
				boost::algorithm::split(arSplit, m_oAnchor.get(), boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);
				for (size_t i = 0 ; i < arSplit.size(); i++)
				{
					aAnchor.push_back(XmlUtils::GetInteger(arSplit[i]));
				}
			}
		}
		void CClientData::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if ( oReader, _T("ObjectType"), m_oObjectType )
			WritingElement_ReadAttributes_End( oReader )
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

				if		(L"MoveWithCells"	== sName )	m_oMoveWithCells = sContent.empty() ? L"t" : sContent;
				else if (L"SizeWithCells"	== sName )	m_oSizeWithCells = sContent.empty() ? L"t" : sContent;
				else if (L"Anchor"			== sName )	m_oAnchor = sContent;
				else if (L"Row"			== sName )	m_oRow = sContent;
				else if (L"Column"			== sName )	m_oColumn = sContent;
				else if (L"DefaultSize"	== sName )	m_oDefaultSize = sContent.empty() ? L"t" : sContent;
				else if (L"AutoLine"		== sName )	m_oAutoLine = sContent.empty() ? L"t" : sContent;
				else if (L"AutoFill"		== sName )	m_oAutoFill = sContent.empty() ? L"t" : sContent;
				else if (L"AutoPict"		== sName )	m_oAutoPict = sContent.empty() ? L"t" : sContent;
				else if (L"AutoScale"		== sName )	m_oAutoScale = sContent.empty() ? L"t" : sContent;
				else if (L"FmlaLink"		== sName )	m_oFmlaLink = sContent;
				else if (L"FmlaRange"		== sName )	m_oFmlaRange = sContent;
				else if (L"FmlaMacro"		== sName )	m_oFmlaMacro = sContent;
				else if (L"FmlaTxbx"		== sName )	m_oFmlaTxbx = sContent;
				else if (L"FmlaGroup"		== sName )	m_oFmlaGroup = sContent;
				else if (L"CF"				== sName )	m_oCf = sContent;
				else if (L"Min"			== sName )	m_oMin = sContent;
				else if (L"Max"			== sName )	m_oMax = sContent;
				else if (L"Val"			== sName )	m_oVal = sContent;
				else if (L"Inc"			== sName )	m_oInc = sContent;
				else if (L"Sel"			== sName )	m_oSel = sContent.empty() ? L"t" : sContent;
				else if (L"WidthMin"		== sName )	m_oWidthMin = sContent;
				else if (L"Dx"				== sName )	m_oDx = sContent;
				else if (L"Page"			== sName )	m_oPage = sContent;
				else if (L"DropLines"		== sName )	m_oDropLines = sContent;
				else if (L"NoThreeD2"		== sName )	m_oNoThreeD2 = sContent.empty() ? L"t" : sContent;
				else if (L"NoThreeD"		== sName )	m_oNoThreeD = sContent.empty() ? L"t" : sContent;
				else if (L"DropStyle"		== sName )	m_oDropStyle = sContent;
				else if (L"FirstButton"	== sName )	m_oFirstButton = sContent.empty() ? L"t" : sContent;
				else if (L"VScroll"		== sName )	m_oVScroll = sContent.empty() ? L"t" : sContent;
				else if (L"Horiz"			== sName )	m_oHoriz = sContent.empty() ? L"t" : sContent;
				else if (L"TextHAlign"		== sName )	m_oTextHAlign = sContent;
				else if (L"TextVAlign"		== sName )	m_oTextVAlign = sContent;
				else if (L"Colored"		== sName )	m_oColored = sContent.empty() ? L"t" : sContent;
				else if (L"MultiLine"		== sName )	m_oMultiLine = sContent.empty() ? L"t" : sContent;
				else if (L"LockText"		== sName )	m_oLockText = sContent.empty() ? L"t" : sContent;
				else if (L"JustLastX"		== sName )	m_oJustLastX = sContent.empty() ? L"t" : sContent;
				else if (L"SecretEdit"		== sName )	m_oSecretEdit = sContent.empty() ? L"t" : sContent;
				else if (L"SelType"		== sName )	m_oSelType = sContent;
				else if (L"Visible"		== sName )	m_oVisible = sContent.empty() ? L"t" : sContent;
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
				sResult += std::wstring(L"<x:TextHAlign>") + m_oTextHAlign->ToVmlString() + L"</x:TextHAlign>";
			}
			if(m_oTextVAlign.IsInit())
			{
				sResult += std::wstring(L"<x:TextVAlign>") + m_oTextVAlign->ToVmlString() + L"</x:TextVAlign>";
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
				sResult += L"<x:Checked>" + m_oChecked->ToString() + L"</x:Checked>";
			}
			if (m_oFmlaLink.IsInit())
			{
				sResult += L"<x:FmlaLink>" + *m_oFmlaLink + L"</x:FmlaLink>";
			}
			if (m_oFmlaRange.IsInit())
			{
				sResult += L"<x:FmlaRange>" + *m_oFmlaRange + L"</x:FmlaRange>";
			}
			if (m_oFmlaMacro.IsInit())
			{
				sResult += L"<x:FmlaMacro>" + *m_oFmlaMacro + L"</x:FmlaMacro>";
			}
			if (m_oFmlaTxbx.IsInit())
			{
				sResult += L"<x:FmlaTxbx>" + *m_oFmlaTxbx + L"</x:FmlaTxbx>";
			}
			if (m_oFmlaGroup.IsInit())
			{
				sResult += L"<x:FmlaGroup>" + *m_oFmlaGroup + L"</x:FmlaGroup>";
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
                sResult += L"<x:SelType>" + m_oSelType->ToVmlString() + L"</x:SelType>";
			}			
			if (m_oDx.IsInit())
			{
                sResult += L"<x:Dx>" + std::to_wstring(m_oDx->GetValue()) + L"</x:Dx>";
			}
			if (m_oDropStyle.IsInit())
			{
				sResult += L"<x:DropStyle>" + m_oDropStyle->ToVmlString() + L"</x:DropStyle>";
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
		bool CClientData::toCellAnchor(OOX::Spreadsheet::CCellAnchor *pCellAnchor)
		{
			if (!m_oAnchor.IsInit()) return false;
			if (!pCellAnchor) return false;

			std::vector<std::wstring> sAnchors;
			boost::algorithm::split(sAnchors, *m_oAnchor, boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);

			if (sAnchors.size() < 8) return false;

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

			return true;
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

		//--------------------------------------------------------------------------------
		// CTextbox 14.1.2.22 (Part4)
		//--------------------------------------------------------------------------------
		CTextbox::CTextbox(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CTextbox::~CTextbox()
		{
		}
		void CTextbox::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CTextbox::fromXML(XmlUtils::CXmlNode& oNode)
		}
		EElementType CTextbox::getType() const
		{
			return OOX::et_v_textbox;
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

				if (L"txbxContent" == sName ) //namespaces w & wne
				{
					m_oTxtbxContent = new OOX::Logic::CTxbxContent(m_pMainDocument);
					m_oTxtbxContent->fromXML(oReader);
				}
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
					if      (L"id"            == wsName ) m_oId          = oReader.GetText();
					else if (L"inset"         == wsName ) m_oInset       = oReader.GetText();
					else if (L"insetmode"     == wsName ) m_oInsetMode   = oReader.GetText();
					break;
				case 'o':
					if      (L"o:singleclick" == wsName ) m_oSingleClick = oReader.GetText();
					break;
				case 's':
					if      (L"style"         == wsName ) m_oStyle       = oReader.GetText();
					break;
				}

				if ( !oReader.MoveToNextAttribute() )
					break;

				wsName = oReader.GetName();
			}
			oReader.MoveToElement();
		}

		//--------------------------------------------------------------------------------
		// CTextPath 14.1.2.23 (Part4)
		//--------------------------------------------------------------------------------
		CTextPath::CTextPath(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CTextPath::~CTextPath()
		{
		}
		void CTextPath::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CTextPath::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CTextPath::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			m_sStringOriginal = oReader.GetText2();

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CTextPath::toXML() const
		{
			std::wstring sResult = _T("<v:textpath ");

			ComplexTypes_WriteAttribute3( L"id=\"",    m_oId );
			ComplexTypes_WriteAttribute ( L"style=\"", m_oStyle );

			if ( (m_oOn.IsInit()) && (SimpleTypes::booleanFalse != m_oOn->GetValue()) )
				sResult += _T("on=\"true\" ");

			if ( (m_oFitShape.IsInit()) && ( SimpleTypes::booleanFalse != m_oFitShape->GetValue()) )
				sResult += _T("fitshape=\"true\" ");

			if ( (m_oFitPath.IsInit()) && ( SimpleTypes::booleanFalse != m_oFitPath->GetValue()) )
				sResult += _T("fitpath=\"true\" ");

			if ( (m_oTrim.IsInit()) && ( SimpleTypes::booleanFalse != m_oTrim->GetValue()) )
				sResult += _T("trim=\"true\" ");

			if ( (m_oXScale.IsInit()) && ( SimpleTypes::booleanFalse != m_oXScale->GetValue()) )
				sResult += _T("xscale=\"true\" ");

			ComplexTypes_WriteAttribute3( _T("string=\""), m_sString );

			sResult += _T("/>");
			return sResult;
		}
		EElementType CTextPath::getType() const
		{
			return OOX::et_v_textpath;
		}
		void CTextPath::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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
					case 'f':
						if      ( _T("fitpath")  == wsName ) m_oFitPath  = oReader.GetText();
						else if ( _T("fitshape") == wsName ) m_oFitShape = oReader.GetText();
						break;
					case 'i':
						if      ( _T("id")       == wsName ) m_oId       = oReader.GetText();
						break;
					case 'o':
						if      ( _T("on")       == wsName ) m_oOn       = oReader.GetText();
						break;
					case 's':
						if      ( _T("string")   == wsName ) m_sString   = oReader.GetText();
						else if ( _T("style")    == wsName ) m_oStyle    = oReader.GetText();
						break;
					case 't':
						if      ( _T("trim")     == wsName ) m_oTrim     = oReader.GetText();
						break;
					case 'x':
						if      ( _T("xscale")   == wsName ) m_oXScale   = oReader.GetText();
						break;
					}

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}
				oReader.MoveToElement();
			}

		//--------------------------------------------------------------------------------
		// CFill 14.1.2.5 (Part4)
		//--------------------------------------------------------------------------------
		CFill::CFill(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CFill::~CFill()
		{
		}
		void CFill::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CFill::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CFill::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while ( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( _T("o:fill") == sName )
					m_oFill = oReader;
			}
		}
		void CFill::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
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
					if      (L"aspect"     == wsName ) m_oAspect     = oReader.GetText();
					else if (L"angle"      == wsName ) m_oAngle      = oReader.GetText();
					else if (L"alignshape" == wsName ) m_oAlignShape = oReader.GetText();
					break;

				case 'c':
					if      (L"color"  == wsName ) m_oColor   = oReader.GetText();
					else if (L"color2" == wsName ) m_oColor2  = oReader.GetText();
					else if (L"colors" == wsName ) sColors    = oReader.GetText();
					break;

				case 'i':
					if      (L"id"   == wsName ) m_sId  = oReader.GetText();
					break;

				case 'm':
					if      (L"method"   == wsName ) m_oMethod = oReader.GetText();
					break;

				case 'f':
					if      (L"focus"         == wsName ) m_oFocus         = oReader.GetText();
					else if (L"focussize"     == wsName ) m_oFocusSize     = oReader.GetText();
					else if (L"focusposition" == wsName ) m_oFocusPosition = oReader.GetText();
					break;

				case 'o':
					if      (L"on"                 == wsName ) m_oOn               = oReader.GetText();
					else if (L"opacity"            == wsName ) m_oOpacity          = oReader.GetText();
					else if (L"o:href"             == wsName ) m_sHref             = oReader.GetText();
					else if (L"o:althref"          == wsName ) m_sAltHref          = oReader.GetText();
					else if (L"origin"             == wsName ) m_oOrigin           = oReader.GetText();
					else if (L"o:detectmouseclick" == wsName ) m_oDetectMouseClick = oReader.GetText();
					else if (L"o:title"            == wsName ) m_sTitle            = oReader.GetText();
					else if (L"o:opacity2"         == wsName ) m_oOpacity2         = oReader.GetText();
					else if (L"o:relid"            == wsName ) m_oRelId            = oReader.GetText();
					break;

				case 'p':
					if      (L"position" == wsName ) m_oPosition = oReader.GetText();
					break;

				case 'r':
					if      (L"recolor" == wsName ) m_oRecolor = oReader.GetText();
					else if (L"rotate"  == wsName ) m_oRotate  = oReader.GetText();
					else if (L"r:id"    == wsName ) m_rId      = oReader.GetText();
					else if (L"relationships:id" == wsName ) m_rId = oReader.GetText();
					break;

				case 's':
					if      (L"src"   == wsName ) m_sSrc    = oReader.GetText();
					else if (L"size"  == wsName ) m_oSize   = oReader.GetText();
					break;

				case 't':
					if      (L"type" == wsName ) m_oType = oReader.GetText();
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
			std::wstring sResult = L"<v:fill ";
							
			ComplexTypes_WriteAttribute3(L"id=\"", m_sId );
			sResult += L"type=\"" + m_oType.ToString() + L"\" ";
			
			if (m_oOn.IsInit() && ( SimpleTypes::booleanTrue != m_oOn->GetValue() ))
				sResult += L"on=\"false\" ";

			if (  m_oOpacity.IsInit() )
				sResult += L"opacity=\"" + m_oOpacity->ToString() + L"\" ";

			ComplexTypes_WriteAttribute (L"color=\"", m_oColor);
			ComplexTypes_WriteAttribute (L"color2=\"",	  m_oColor2);
			ComplexTypes_WriteAttribute3(L"src=\"",       m_sSrc );
			ComplexTypes_WriteAttribute3(L"o:href=\"",    m_sHref );
			ComplexTypes_WriteAttribute3(L"o:althref=\"", m_sAltHref );
			ComplexTypes_WriteAttribute (L"size=\"",      m_oSize );
			ComplexTypes_WriteAttribute (L"origin=\"",    m_oOrigin );
			ComplexTypes_WriteAttribute (L"position=\"",  m_oPosition );

			if ((m_oAspect.IsInit()) && (SimpleTypes::imageaspectIgnore != m_oAspect->GetValue() ))
				sResult += L"aspect=\"" + m_oAspect->ToString() + L"\" ";

			// TO DO: Сделать запись m_arrColors

			ComplexTypes_WriteAttribute (L"angle=\"", m_oAngle );

			if (m_oAlignShape.IsInit() && (SimpleTypes::booleanTrue != m_oAlignShape->GetValue() ))
				sResult += L"alignshape=\"false\" ";

			if (m_oFocus.IsInit() && (0 != m_oFocus->GetValue() ))
				sResult += L"focus=\"" + m_oFocus->ToString() + L"\" ";

			if ( m_oFocusPosition.IsInit())
				sResult += L"focusposition=\"" + m_oFocusPosition->ToString() + L"\" ";

			if ( m_oFocusSize.IsInit() && (0 != m_oFocusSize->GetX() || 0 != m_oFocusSize->GetY() ))
				sResult += L"focussize=\"" + m_oFocusSize->ToString() + L"\" ";

			if (m_oMethod.IsInit() && (SimpleTypes::fillmethodSigma != m_oMethod->GetValue() ))
				sResult += L"method=\"" + m_oMethod->ToString() + L"\" ";

			ComplexTypes_WriteAttribute (L"o:detectmouseclick=\"", m_oDetectMouseClick );
			ComplexTypes_WriteAttribute3(L"o:title=\"",            m_sTitle );

			if ( m_oOpacity2.IsInit() )
				sResult += L"o:opacity2=\"" + m_oOpacity2->ToString() + L"\" ";

			if (m_oRecolor.IsInit() && m_oRecolor->GetBool())
				sResult += L"recolor=\"true\" ";

			if (( m_oRotate.IsInit()) && m_oRotate->GetBool())
				sResult += L"rotate=\"true\" ";

			ComplexTypes_WriteAttribute (L"r:id=\"",    m_rId );
			ComplexTypes_WriteAttribute (L"o:relid=\"", m_oRelId );

			sResult += L">";

			if ( m_oFill.IsInit() )
				sResult += m_oFill->toXML();

			sResult += L"</v:fill>";

			return sResult;
		}
		EElementType CFill::getType() const
		{
			return OOX::et_v_fill;
		}

		//--------------------------------------------------------------------------------
		// CBackground 14.1.2.2 (Part4)
		//--------------------------------------------------------------------------------
		CBackground::CBackground(OOX::Document *pMain) : CVmlCommonElements(pMain)
		{
		}
		CBackground::~CBackground()
		{
		}
		void CBackground::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CBackground::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CBackground::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			CVmlCommonElements::ReadAttributes( oReader );
			CVmlCommonElements::ReadElements( oReader );

		}
		std::wstring CBackground::toXML() const
		{
			std::wstring sResult = _T("<v:background ");

			sResult += CVmlCommonElements::WriteAttributes();

			ComplexTypes_WriteAttribute ( _T("o:targetscreensize=\""),	m_oTargetScreenSize );

			sResult += _T(">");

			sResult += CVmlCommonElements::WriteElements();

			sResult += _T("</v:background>");

			return sResult;
		}
		EElementType CBackground::getType() const
		{
			return OOX::et_v_background;
		}
		void CBackground::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_ReadSingle( oReader, _T("o:targetscreensize"), m_oTargetScreenSize)
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// CFormulas 14.1.2.6 (Part4)
		//--------------------------------------------------------------------------------
		CFormulas::CFormulas(OOX::Document *pMain) : WritingElementWithChilds<OOX::Vml::CF>(pMain)
		{
		}
		CFormulas::~CFormulas()
		{
		}
		void CFormulas::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CFormulas::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CFormulas::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while ( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( _T("v:f") == sName )
				{
					OOX::Vml::CF *oF = new OOX::Vml::CF(oReader);
					if (oF) m_arrItems.push_back( oF );
				}
			}
		}
		std::wstring CFormulas::toXML() const
		{
			std::wstring sResult = _T("<v:formulas>");

			for ( size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (  m_arrItems[i] )
				{
					sResult += m_arrItems[i]->toXML();
				}
			}

			sResult += _T("</v:formulas>");

			return sResult;
		}
		EElementType CFormulas::getType() const
			{
				return OOX::et_v_formulas;
			}

		//--------------------------------------------------------------------------------
		// CStroke 14.1.2.21 (Part4)
		//--------------------------------------------------------------------------------
		CStroke::CStroke(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CStroke::~CStroke()
		{
		}
		void CStroke::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CStroke::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CStroke::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while ( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = oReader.GetName();

				if ( _T("o:left") == sName )
					m_oLeft = oReader;
				else if ( _T("o:top") == sName )
					m_oTop = oReader;
				else if ( _T("o:right") == sName )
					m_oRight = oReader;
				else if ( _T("o:bottom") == sName )
					m_oBottom = oReader;
				else if ( _T("o:column") == sName )
					m_oColumn = oReader;
			}
		}
		EElementType CStroke::getType() const
		{
			return OOX::et_v_stroke;
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
					if      (L"color"     == wsName ) m_oColor     = oReader.GetText();
					else if (L"color2"    == wsName ) m_oColor2    = oReader.GetText();
					break;
				case 'd':
					if      (L"dashstyle" == wsName ) m_oDahsStyle = oReader.GetText();
					break;
				case 'e':
					if      (L"endarrow"       == wsName ) m_oEndArrow       = oReader.GetText();
					else if (L"endarrowlength" == wsName ) m_oEndArrowLength = oReader.GetText();
					else if (L"endarrowwidth"  == wsName ) m_oEndArrowWidth  = oReader.GetText();
					else if (L"endcap"         == wsName ) m_oEndCap         = oReader.GetText();
					break;
				case 'f':
					if      (L"filltype" == wsName ) m_oFillType = oReader.GetText();
					break;
				case 'i':
					if      (L"id"              == wsName ) m_oId              = oReader.GetText();
					else if (L"imagealignshape" == wsName ) m_oImageAlignShape = oReader.GetText();
					else if (L"imageaspect"     == wsName ) m_oImageAspect     = oReader.GetText();
					else if (L"imagesize"       == wsName ) m_oImageSize       = oReader.GetText();
					else if (L"insetpen"        == wsName ) m_oInsetPen        = oReader.GetText();
					break;
				case 'j':
					if      (L"joinstyle" == wsName ) m_oJoinStyle = oReader.GetText();
					break;
				case 'l':
					if      (L"linestyle" == wsName ) m_oLineStyle = oReader.GetText();
					break;
				case 'm':
					if      (L"miterlimit" == wsName ) m_oMiterLimit = oReader.GetText();
					break;
				case 'o':
					if      (L"o:althref"   == wsName ) m_sAltHref   = oReader.GetText();
					else if (L"o:forcedash" == wsName ) m_oForceDash = oReader.GetText();
					else if (L"o:href"      == wsName ) m_sHref      = oReader.GetText();
					else if (L"on"          == wsName ) m_oOn        = oReader.GetText();
					else if (L"opacity"     == wsName ) m_oOpacity   = oReader.GetText();
					else if (L"o:relid"     == wsName ) m_oRelId     = oReader.GetText();
					else if (L"o:title"     == wsName ) m_sTitle     = oReader.GetText();
					break;
				case 'r':
					if      (L"r:id" == wsName ) m_rId = oReader.GetText();
					else if (L"relationships:id" == wsName ) m_rId = oReader.GetText();
					break;
				case 's':
					if      (L"src"              == wsName ) m_sSrc              = oReader.GetText();
					else if (L"startarrow"       == wsName ) m_oStartArrow       = oReader.GetText();
					else if (L"startarrowlength" == wsName ) m_oStartArrowLength = oReader.GetText();
					else if (L"startarrowwidth"  == wsName ) m_oStartArrowWidth  = oReader.GetText();
					break;
				case 'w':
					if      (L"weight" == wsName ) m_oWeight = oReader.GetText();
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
			std::wstring sResult = L"<v:stroke ";

			ComplexTypes_WriteAttribute3(L"id=\"", m_oId );
			if (m_oOn.IsInit() && !m_oOn->GetBool())
				sResult += L"on=\"false\" ";

			if ( m_oWeight.IsInit() )
				sResult += L"weight=\"" + std::to_wstring(*m_oWeight) + L"\" ";

			//if ( SimpleTypes::colortypeBlack != m_oColor.GetValue() )
			//	sResult += (L"color=\"" + m_oColor.ToString() + (L"\" ";
			ComplexTypes_WriteAttribute (L"color=\"", m_oColor );

			if (m_oOpacity.IsInit() )
				sResult += L"opacity=\"" + std::to_wstring(*m_oOpacity) + L"\" ";

			//if ( SimpleTypes::strokelinestyleSingle != m_oLineStyle.GetValue() )
			//	sResult += L"linestyle=\"" + m_oLineStyle.ToString() + L"\" ";
			ComplexTypes_WriteAttribute (L"linestyle=\"", m_oLineStyle );
			
			if ( m_oMiterLimit.IsInit())
				sResult += L"miterlimit=\"" + m_oMiterLimit->ToString() + L"\" ";

			if ( m_oJoinStyle.IsInit())
				sResult += L"joinstyle=\"" + m_oJoinStyle->ToString() + L"\" ";

			if ( m_oEndCap.IsInit())
				sResult += L"endcap=\"" + m_oEndCap->ToString() + L"\" ";

			if ( m_oDahsStyle.IsInit())
				sResult += L"dashstyle=\"" + m_oDahsStyle->ToString() + L"\" ";

			if ( m_oFillType.IsInit())
				sResult += L"filltype=\"" + m_oFillType->ToString() + L"\" ";

			ComplexTypes_WriteAttribute3(L"src=\"", m_sSrc );

			if ( m_oImageAspect.IsInit())
				sResult += L"imageaspect=\"" + m_oImageAspect->ToString() + L"\" ";

			ComplexTypes_WriteAttribute (L"imagesize=\"", m_oImageSize );

			if ( m_oOn.IsInit() && !m_oOn->GetBool())
				sResult += L"imagealignshape=\"false\" ";

			ComplexTypes_WriteAttribute (L"color2=\"", m_oColor2 );

			if ( m_oStartArrow.IsInit())
				sResult += L"startarrow=\"" + m_oStartArrow->ToString() + L"\" ";

			if ( m_oStartArrowWidth.IsInit())
				sResult += L"startarrowwidth=\"" + m_oStartArrowWidth->ToString() + L"\" ";

			if ( m_oStartArrowLength.IsInit())
				sResult += L"startarrowlength=\"" + m_oStartArrowLength->ToString() + L"\" ";

			if ( m_oEndArrow.IsInit())
				sResult += L"endarrow=\"" + m_oEndArrow->ToString() + L"\" ";

			if ( m_oEndArrowWidth.IsInit())
				sResult += L"endarrowwidth=\"" + m_oEndArrowWidth->ToString() + L"\" ";

			if ( m_oEndArrowLength.IsInit())
				sResult += L"endarrowlength=\"" + m_oEndArrowLength->ToString() + L"\" ";

			ComplexTypes_WriteAttribute3(L"o:href=\"",    m_sHref );
			ComplexTypes_WriteAttribute3(L"o:althref=\"", m_sAltHref );
			ComplexTypes_WriteAttribute3(L"o:title=\"",   m_sTitle );

			if ( m_oForceDash.IsInit() && m_oForceDash->GetBool())
				sResult += L"o:forcedash=\"true\" ";

			ComplexTypes_WriteAttribute (L"r:id=\"",     m_rId );
			ComplexTypes_WriteAttribute (L"insetpen=\"", m_oInsetPen );
			ComplexTypes_WriteAttribute (L"o:relid=\"",  m_oRelId );

			sResult += L">";

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

			sResult += L"</v:stroke>";

			return sResult;
		}

		//--------------------------------------------------------------------------------
		// CPath 14.1.2.14 (Part4)
		//--------------------------------------------------------------------------------
		CPath::CPath(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CPath::~CPath()
		{
		}
		void CPath::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CPath::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CPath::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		EElementType CPath::getType() const
		{
			return OOX::et_v_path;
		}
		void CPath::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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
				case 'a':
					if      (L"arrowok" == wsName ) m_oArrowOk = oReader.GetText();
					break;

				case 'f':
					if      (L"fillok" == wsName ) m_oFillOk = oReader.GetText();
					break;

				case 'g':
					if      (L"gradientshapeok" == wsName ) m_oGradientShapeOk = oReader.GetText();
					break;

				case 'i':
					if      (L"id"         == wsName ) m_oId         = oReader.GetText();
					else if (L"insetpenok" == wsName ) m_oInsetPenOk = oReader.GetText();
					break;

				case 'l':
					if      (L"limo"       == wsName ) m_oLimo         = oReader.GetText();
					break;

				case 'o':
					{
						wchar_t wsChar2 = wsName[2]; // o:_
						switch ( wsChar2 )
						{
						case 'c':
							if      (L"o:connectangles" == wsName ) m_oConnectAngles = oReader.GetText();
							else if (L"o:connectlocs"   == wsName ) m_oConnectLocs   = oReader.GetText();
							else if (L"o:connecttype"   == wsName ) m_oConnectType   = oReader.GetText();
							break;
						case 'e':
							if      (L"o:extrusionok"   == wsName ) m_oExtrusionOk   = oReader.GetText();
							break;
						}

					break;
					}

				case 's':
					if      (L"shadowok"    == wsName ) m_oShadowOk    = oReader.GetText();
					else if (L"strokeok"    == wsName ) m_oStrokeOk    = oReader.GetText();
					break;

				case 't':
					if      (L"textboxrect" == wsName ) m_oTextBoxRect = oReader.GetText();
					else if (L"textpathok"  == wsName ) m_oTextPathOk  = oReader.GetText();
					break;

				case 'v':
					if      (L"v"           == wsName ) m_oV           = oReader.GetText();
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
			std::wstring sResult = L"<v:path ";

			ComplexTypes_WriteAttribute3(L"id=\"", m_oId );
			ComplexTypes_WriteAttribute (L"v=\"",  m_oV );

			if ( m_oLimo.IsInit() )
				sResult += L"limo=\"" + m_oLimo->ToString() + L"\" ";

			ComplexTypes_WriteAttribute (L"textboxrect=\"", m_oTextBoxRect );

			if (m_oFillOk.IsInit() && !m_oFillOk->GetBool())
				sResult += L"fillok=\"f\" ";

			if ( m_oStrokeOk.IsInit() && !m_oStrokeOk->GetBool())
				sResult += L"strokeok=\"f\" ";

			if ( m_oShadowOk.IsInit() && !m_oShadowOk->GetBool())
				sResult += L"shadowok=\"f\" ";

			if (m_oArrowOk.IsInit() && m_oArrowOk->GetBool())
				sResult += L"arrowok=\"t\" ";

			if (m_oGradientShapeOk.IsInit() && m_oGradientShapeOk->GetBool())
				sResult += L"gradientshapeok=\"t\" ";

			if (m_oTextPathOk.IsInit() && m_oTextPathOk->GetBool())
				sResult += L"textpathok=\"t\" ";

			if (m_oInsetPenOk.IsInit() && m_oInsetPenOk->GetBool())
				sResult += L"insetpenok=\"t\" ";

			if ( m_oConnectType.IsInit() )
				sResult += L"o:connecttype=\"" + m_oConnectType->ToString() + L"\" ";

			ComplexTypes_WriteAttribute3(L"o:connectlocs=\"",   m_oConnectLocs );
			ComplexTypes_WriteAttribute3(L"o:connectangles=\"", m_oConnectAngles );

			if ( m_oExtrusionOk.IsInit() && !m_oExtrusionOk->GetBool())
				sResult += L"o:extrusionok=\"f\" ";

			sResult += L"/>";

			return sResult;
		}

		//--------------------------------------------------------------------------------
		// CImage 14.1.2.10 (Part4)
		//--------------------------------------------------------------------------------
		CImage::CImage(OOX::Document *pMain) : CVmlCommonElements(pMain)
		{
		}
		CImage::~CImage(){}
		void CImage::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CImage::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CImage::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );
			CVmlCommonElements::ReadAttributes( oReader );
			CVmlCommonElements::ReadElements( oReader );
		}
		EElementType CImage::getType() const
		{
			return OOX::et_v_image;
		}
		void CImage::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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
				case 'b':
					if      (L"bilevel"    == wsName ) m_pBiLevel    = oReader.GetText();
					else if (L"blacklevel" == wsName ) m_pBlackLevel = oReader.GetText();
					break;

				case 'c':
					if      (L"chromakey"  == wsName ) m_oChromaKey  = oReader.GetText();
					else if (L"cropleft"   == wsName ) m_pCropLeft   = oReader.GetText();
					else if (L"croptop"    == wsName ) m_pCropTop    = oReader.GetText();
					else if (L"cropright"  == wsName ) m_pCropRight  = oReader.GetText();
					else if (L"cropbottom" == wsName ) m_pCropBottom = oReader.GetText();
					break;

				case 'g':
					if      (L"gain"       == wsName ) m_pGain       = oReader.GetText();
					if      (L"gamma"      == wsName ) m_pGamma      = oReader.GetText();
					if      (L"grayscale"  == wsName ) m_pGrayscale  = oReader.GetText();
					break;

				case 's':
					if      (L"src" == wsName ) m_pSrc = oReader.GetText();
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
			std::wstring sResult = L"<v:image";

			sResult += CVmlCommonElements::WriteAttributes();

			if ( m_pSrc.IsInit())
				sResult += L" src=\"" + XmlUtils::EncodeXmlString(*m_pSrc) + L"\"";

			if ( m_pCropLeft.IsInit())
				sResult += L" cropleft=\"" + m_pCropLeft->ToString() + L"\"";

			if ( m_pCropTop.IsInit())
				sResult += L" croptop=\"" + m_pCropTop->ToString() + L"\"";

			if ( m_pCropRight.IsInit())
				sResult += L"cropright=\"" + m_pCropRight->ToString() + L"\"";

			if ( m_pCropBottom.IsInit())
				sResult += L" cropbottom=\"" + m_pCropBottom->ToString() + L"\"";

			if ( m_pGain.IsInit() )
				sResult += L" gain=\"" + std::to_wstring(*m_pGain) + L"\"";

			if ( m_pBlackLevel.IsInit() )
				sResult += L" blacklevel=\"" + std::to_wstring(*m_pBlackLevel) + L"\"";

			if ( m_pGamma.IsInit())
				sResult += L" gamma=\"" + std::to_wstring(*m_pGamma) + L"\"";

			if ( m_pGrayscale.IsInit() && m_pGrayscale->GetBool())
				sResult += L" grayscale=\"t\"";

			if (m_pBiLevel.IsInit() && m_pBiLevel->GetBool() )
				sResult += L" bilevel=\"t\"";

			sResult += L">";

			sResult += CVmlCommonElements::WriteElements();

			sResult += L"</v:image>";

			return sResult;
		}

		//--------------------------------------------------------------------------------
		// CImageData 14.1.2.11 (Part4)
		//--------------------------------------------------------------------------------
		CImageData::CImageData(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CImageData::~CImageData()
		{
		}
		void CImageData::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CImageData::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CImageData::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		EElementType CImageData::getType() const
		{
			return OOX::et_v_imagedata;
		}
		void CImageData::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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
				case 'a':
					if      (L"althref" == wsName ) m_sAltHref    = oReader.GetText();
					break;

				case 'b':
					if      (L"bilevel" == wsName ) m_oBiLevel    = oReader.GetText();
					else if (L"blacklevel" == wsName ) m_oBlackLevel = oReader.GetText();
					break;

				case 'c':
					if      (L"cropleft"   == wsName ) m_oCropLeft   = oReader.GetText();
					else if (L"croptop"    == wsName ) m_oCropTop    = oReader.GetText();
					else if (L"cropright"  == wsName ) m_oCropRight  = oReader.GetText();
					else if (L"cropbottom" == wsName ) m_oCropBottom = oReader.GetText();
					break;

				case 'e':
					if      (L"embosscolor"== wsName ) m_oEmbossColor= oReader.GetText();
					break;

				case 'g':
					if      (L"gain"       == wsName ) m_oGain       = oReader.GetText();
					if      (L"gamma"      == wsName ) m_oGamma      = oReader.GetText();
					if      (L"grayscale"  == wsName ) m_oGrayscale  = oReader.GetText();
					break;

				case 'i':
					if      (L"id" == wsName ) m_oId = oReader.GetText();
					break;

				case 'o':
					{
						wchar_t wsChar2 = wsName[2]; // o:_
						switch ( wsChar2 )
						{
						case 'd':
							if      (L"o:detectmouseclick" == wsName ) m_oDetectMouseClick = oReader.GetText();
							break;
						case 'h':
							if      (L"o:href"   == wsName ) m_oHref    = oReader.GetText();
							break;
						case 'm':
							if      (L"o:movie"  == wsName ) m_oMovie   = oReader.GetText();
							break;
						case 'o':
							if      (L"o:oleid"  == wsName ) m_oOleId   = oReader.GetText();
							break;
						case 'r':
							if      (L"o:relid"  == wsName ) m_oRelId   = oReader.GetText();
							break;
						case 't':
							if      (L"title"    == wsName ) m_sTitle   = oReader.GetText();
							break;
						}

					break;
					}

				case 'r':
					if      (L"r:href"        == wsName ) m_rHref          = oReader.GetText();
					else if (L"r:id"          == wsName ) m_rId            = oReader.GetText();
					else if (L"relationships:id" == wsName ) m_rId         = oReader.GetText();
					else if (L"r:pict"        == wsName ) m_rPict          = oReader.GetText();
					else if (L"recolortarget" == wsName ) m_oRecolorTarget = oReader.GetText();
					break;

				case 's':
					if      (L"src"           == wsName ) m_sSrc           = oReader.GetText();
					break;
				}

				if ( !oReader.MoveToNextAttribute() )
					break;

				wsName = oReader.GetName();
			}
			oReader.MoveToElement();

			CDocxFlat* docx_flat = dynamic_cast<CDocxFlat*>(m_pMainDocument);
			if (m_sSrc.IsInit())
			{
				if (false == m_sTitle.IsInit()) m_sTitle = m_sSrc;
				if (docx_flat)
				{
					OOX::RId rId;
					std::map<std::wstring, NSCommon::smart_ptr<OOX::File>>::iterator pFind = docx_flat->m_mapImages.find(*m_sSrc);
					if (pFind != docx_flat->m_mapImages.end())
					{
						rId = docx_flat->m_currentContainer->Add(pFind->second);
					}
					else
					{
						smart_ptr<OOX::File> oImageLinkFile = smart_ptr<OOX::File>(new OOX::Image(m_pMainDocument, OOX::CPath(*m_sSrc, false), true));
						rId = docx_flat->m_currentContainer->Add(oImageLinkFile);
					}
					if (!m_rId.IsInit())
						m_rId = rId.get();
				}
				m_sSrc.reset();
			}
		}
		std::wstring CImageData::toXML() const
		{
			std::wstring sResult = L"<v:imagedata ";

			ComplexTypes_WriteAttribute3(L"id=\"", m_oId );

			if (m_sSrc.IsInit())
				sResult += L"src=\"" + XmlUtils::EncodeXmlString(*m_sSrc) + L"\" ";

			ComplexTypes_WriteAttribute(L"cropleft=\"",		m_oCropLeft);
			ComplexTypes_WriteAttribute(L"croptop=\"",		m_oCropTop);
			ComplexTypes_WriteAttribute(L"cropright=\"",	m_oCropRight);
			ComplexTypes_WriteAttribute(L"cropbottom=\"",	m_oCropBottom);

			if ( m_oGain.IsInit() )
				sResult += L"gain=\"" + std::to_wstring(*m_oGain) + L"\" ";

			if ( m_oBlackLevel.IsInit() )
				sResult += L"blacklevel=\"" + std::to_wstring(*m_oBlackLevel) + L"\" ";

			if ( m_oGamma.IsInit())
				sResult += L"gamma=\"" + std::to_wstring(*m_oGamma) + L"\" ";

			if ( m_oGrayscale.IsInit() && m_oGrayscale->GetBool())
				sResult += L"grayscale=\"t\" ";

			if ( m_oBiLevel.IsInit()  && m_oBiLevel->GetBool())
				sResult += L"bilevel=\"t\" ";

			ComplexTypes_WriteAttribute (L"chromakey=\"",    m_oChromaKey );
			ComplexTypes_WriteAttribute (L"embosscolor=\"",  m_oEmbossColor );

			ComplexTypes_WriteAttribute3(L"o:href=\"",             m_oHref );
			ComplexTypes_WriteAttribute3(L"o:althref=\"",          m_sAltHref );
			ComplexTypes_WriteAttribute3(L"o:title=\"",            m_sTitle );
			ComplexTypes_WriteAttribute3(L"o:oleid=\"",            m_oOleId );
			ComplexTypes_WriteAttribute (L"o:detectmouseclick=\"", m_oDetectMouseClick );
			if (m_oMovie.IsInit())
				sResult += L"o:movie=\"" + std::to_wstring(*m_oMovie) + L"\" ";
			ComplexTypes_WriteAttribute (L"o:relid=\"",	m_oRelId );

			ComplexTypes_WriteAttribute (L"r:id=\"",               m_rId );
			ComplexTypes_WriteAttribute (L"r:pict=\"",             m_rPict );
			ComplexTypes_WriteAttribute (L"r:href=\"",             m_rHref );

			sResult += L"/>";

			return sResult;
		}

		//--------------------------------------------------------------------------------
		// CLine 14.1.2.12 (Part4)
		//--------------------------------------------------------------------------------

		CLine::CLine(OOX::Document *pMain) : CVmlCommonElements(pMain)
		{
		}
		CLine::~CLine(){}
		void CLine::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CLine::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CLine::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );
			CVmlCommonElements::ReadAttributes( oReader );
			CVmlCommonElements::ReadElements( oReader );
		}
		std::wstring CLine::toXML() const
		{
			std::wstring sResult = _T("<v:line ");

			sResult += CVmlCommonElements::WriteAttributes();

			sResult += _T("from=\"") + m_oFrom.ToString() + _T("\" ");
			sResult += _T("to=\"")   + m_oTo.ToString()   + _T("\">");

			sResult += CVmlCommonElements::WriteElements();

			sResult += _T("</v:line>");

			return sResult;
		}
		EElementType CLine::getType() const
		{
			return OOX::et_v_line;
		}
		void CLine::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Выставляем значения по умолчанию
				m_oFrom.SetValue( 0, 0 );
				m_oTo.SetValue( 10, 10 );

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
						if      ( _T("from") == wsName ) m_oFrom = oReader.GetText();
						break;

					case 't':
						if      ( _T("to")   == wsName ) m_oTo   = oReader.GetText();
						break;
					}

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}
				oReader.MoveToElement();
			}

		//--------------------------------------------------------------------------------
		// COval 14.1.2.13 (Part4)
		//--------------------------------------------------------------------------------

		COval::COval(OOX::Document *pMain) : CVmlCommonElements(pMain)
		{
		}
		COval::~COval(){}
		void COval::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать COval::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void COval::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			CVmlCommonElements::ReadAttributes( oReader );
			CVmlCommonElements::ReadElements( oReader );
		}
		std::wstring COval::toXML() const
		{
			std::wstring sResult = _T("<v:oval ");

			sResult += CVmlCommonElements::WriteAttributes();

			sResult += _T(">");

			sResult += CVmlCommonElements::WriteElements();

			sResult += _T("</v:oval>");

			return sResult;
		}
		EElementType COval::getType() const
			{
				return OOX::et_v_oval;
			}

		//--------------------------------------------------------------------------------
		// CH 14.1.2.8 (Part4)
		//--------------------------------------------------------------------------------
		CH::CH()
		{
		}
		CH::~CH()
		{
		}
		void CH::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CH::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CH::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		EElementType CH::getType() const
		{
			return OOX::et_v_h;
		}
		void CH::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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
				case 'i':
					if      (L"invx" == wsName ) m_oInvX = oReader.GetText();
					else if (L"invy" == wsName ) m_oInvY = oReader.GetText();
					break;

				case 'm':
					if      (L"map" == wsName ) m_oMap = oReader.GetText();
					break;

				case 'p':
					if      (L"position" == wsName ) m_oPosition = oReader.GetText();
					else if (L"polar"    == wsName ) m_oPolar    = oReader.GetText();
					break;

				case 'r':
					if      (L"radiusrange" == wsName ) m_oRadiusRange = oReader.GetText();
					break;

				case 's':
					if      (L"switch" == wsName ) m_oSwitch = oReader.GetText();
					break;

				case 'x':
					if      (L"xrange" == wsName ) m_oXRange = oReader.GetText();
					break;
				case 'y':
					if      (L"yrange" == wsName ) m_oYRange = oReader.GetText();
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
			std::wstring sResult = L"<v:h ";

			if ( SimpleTypes::Vml::vmlvector2dposConstant != m_oPosition.GetTypeX() || SimpleTypes::Vml::vmlvector2dposConstant != m_oPosition.GetTypeY() || 0 != m_oPosition.GetX() || 0 != m_oPosition.GetY() )
				sResult += L"position=\"" + m_oPosition.ToString() + L"\" ";
							
			ComplexTypes_WriteAttribute (L"polar=\"", m_oPolar );

			if ( 0 != m_oMap.GetX() || 1000 != m_oMap.GetY() )
				sResult += L"map=\"" + m_oMap.ToString() + L"\" ";

			if (m_oInvX.GetValue() )
				sResult += L"invx=\"t\" ";

			if (m_oInvY.GetValue() )
				sResult += L"invy=\"t\" ";

			if (m_oSwitch.GetValue() )
				sResult += L"switch=\"t\" ";

			if ( 0 != m_oXRange.GetX() || 0 != m_oXRange.GetY() )
				sResult += L"xrange=\"" + m_oXRange.ToString() + L"\" ";

			if ( 0 != m_oYRange.GetX() || 0 != m_oYRange.GetY() )
				sResult += L"yrange=\"" + m_oYRange.ToString() + L"\" ";

			if ( 0 != m_oRadiusRange.GetX() || 0 != m_oRadiusRange.GetY() )
				sResult += L"radiusrange=\"" + m_oRadiusRange.ToString() + L"\" ";

			sResult += L"/>";

			return sResult;
		}

		//--------------------------------------------------------------------------------
		// CHandles 14.1.2.9 (Part4)
		//--------------------------------------------------------------------------------
		CHandles::CHandles(OOX::Document *pMain) : WritingElementWithChilds<OOX::Vml::CH>(pMain)
		{
		}
		CHandles::~CHandles()
		{
		}
		void CHandles::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CHandles::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CHandles::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while ( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( _T("v:h") == sName )
				{
					OOX::Vml::CH *oH = new OOX::Vml::CH(oReader);
					if (oH) m_arrItems.push_back( oH );
				}
			}
		}
		std::wstring CHandles::toXML() const
		{
			std::wstring sResult = _T("<v:handles>");

			for ( size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (  m_arrItems[i] )
				{
					sResult += m_arrItems[i]->toXML();
				}
			}
			sResult += _T("</v:handles>");

			return sResult;
		}
		EElementType CHandles::getType() const
			{
				return OOX::et_v_handles;
			}

	} // Vml
	namespace VmlOffice
	{
		CShapeDefaults::CShapeDefaults(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CShapeDefaults::~CShapeDefaults()
		{
		}
		void CShapeDefaults::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CShapeDefaults::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CShapeDefaults::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while ( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if (L"v:fill" == sName )
					m_oVmlFill = oReader;
				else if (L"v:stroke" == sName )
					m_oVmlStroke = oReader;
				else if (L"v:textbox" == sName )
					m_oVmlTextbox = oReader;
				else if (L"v:shadow" == sName )
					m_oVmlShadow = oReader;
				else if (L"o:skew" == sName )
					m_oSkew = oReader;
				else if (L"o:extrusion" == sName )
					m_oExtrusion = oReader;
				else if (L"o:callout" == sName )
					m_oCallout = oReader;
				else if (L"o:lock" == sName )
					m_oLock = oReader;
				else if (L"o:colormru" == sName )
					m_oColorMru = oReader;
				else if (L"o:colormenu" == sName )
					m_oColorMenu = oReader;
			}
		}
		void CShapeDefaults::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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
				case 'f':
					if      (L"fill"          == wsName ) m_oFill        = oReader.GetText();
					else if (L"fillcolor"     == wsName ) m_oFillColor   = oReader.GetText();
					break;
				case 'o':
					if      (L"o:allowincell" == wsName ) m_oAllowInCell = oReader.GetText();
					break;
				case 's':
					if      (L"spidmax"       == wsName ) m_oSpIdMax     = oReader.GetText();
					else if (L"style"         == wsName ) m_oStyle       = oReader.GetText();
					else if (L"stroke"        == wsName ) m_oStroke      = oReader.GetText();
					else if (L"strokecolor"   == wsName ) m_oStrokeColor = oReader.GetText();
					break;
				case 'v':
					if      (L"v:ext"         == wsName ) m_oExt         = oReader.GetText();
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
			std::wstring sResult = L"<o:shapedefaults ";

			ComplexTypes_WriteAttribute (L"v:ext=\"", m_oExt );

			if ( 0 != m_oSpIdMax.GetValue() )
				sResult += L"spidmax=\"" + m_oSpIdMax.ToString() + L"\" ";

			ComplexTypes_WriteAttribute (L"style=\"", m_oStyle );

			if ( SimpleTypes::booleanTrue != m_oFill.GetValue() )
				sResult += L"fill=\"f\" ";

			ComplexTypes_WriteAttribute (L"fillcolor=\"", m_oFillColor );

			if ( SimpleTypes::booleanTrue != m_oStroke.GetValue() )
				sResult += L"stroke=\"f\" ";

			if ( SimpleTypes::colortypeBlack != m_oStrokeColor.GetValue() )
				sResult += L"strokecolor=\"" + m_oStrokeColor.ToString() + L"\" ";

			if (m_oAllowInCell.GetValue() )
				sResult += L"o:allowincell=\"t\" ";

			sResult += L">";

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

			sResult += L"</o:shapedefaults>";

			return sResult;
		}
		EElementType CShapeDefaults::getType() const
		{
			return OOX::et_o_shapedefaults;
		}

	}// VmlOffice
} // OOX
