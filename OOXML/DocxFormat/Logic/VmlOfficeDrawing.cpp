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

#include "VmlOfficeDrawing.h"

namespace OOX
{
	namespace VmlOffice
	{
		//--------------------------------------------------------------------------------
		// CStrokeChild 14.2.2.1;14.2.2.6;14.2.2.16;14.2.2.26;14.2.2.32 (Part 4)
		//--------------------------------------------------------------------------------

		CStrokeChild::CStrokeChild(OOX::Document *pMain) : WritingElement(pMain)
		{
			m_eType = et_Unknown;
		}
		CStrokeChild::~CStrokeChild()
		{
		}
		void CStrokeChild::fromXML(XmlUtils::CXmlNode& oNode)
		{
			m_eType = et_Unknown;
			// TO DO: Реализовать CStrokeChild::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CStrokeChild::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_eType = et_Unknown;
			std::wstring sName = oReader.GetName();

			if ( L"o:bottom" == sName )
				m_eType = et_o_bottom;
			else if ( L"o:column" == sName )
				m_eType = et_o_column;
			else if ( L"o:left" == sName )
				m_eType = et_o_left;
			else if ( L"o:right" == sName )
				m_eType = et_o_right;
			else if ( L"o:top" == sName )
				m_eType = et_o_top;
			else
				return;

			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CStrokeChild::toXML() const
		{
			std::wstring sResult;

			switch( m_eType )
			{
			case et_o_bottom: sResult = L"<o:bottom "; break;
			case et_o_column: sResult = L"<o:column "; break;
			case et_o_left  : sResult = L"<o:left ";   break;
			case et_o_right : sResult = L"<o:right ";  break;
			case et_o_top   : sResult = L"<o:top ";    break;
			default: return L"";
			}

			ComplexTypes_WriteAttribute ( L"v:ext=\"",            m_oExt );
			ComplexTypes_WriteAttribute ( L"on=\"",               m_oOn );
			ComplexTypes_WriteAttribute2( L"weight=\"",           m_sWeight )
			ComplexTypes_WriteAttribute ( L"color=\"",            m_oColor )
			ComplexTypes_WriteAttribute ( L"color2=\"",           m_oColor2 )
			ComplexTypes_WriteAttribute2( L"opacity=\"",          m_sOpacity )
			ComplexTypes_WriteAttribute ( L"linestyle=\"",        m_oLineStyle )
			ComplexTypes_WriteAttribute ( L"miterlimit=\"",       m_oMiterLimit )
			ComplexTypes_WriteAttribute ( L"joinstyle=\"",        m_oJoinStyle )
			ComplexTypes_WriteAttribute ( L"endcap=\"",           m_oEndCap )
			ComplexTypes_WriteAttribute ( L"dashstyle=\"",        m_oDashStyle )
			ComplexTypes_WriteAttribute ( L"insetpen=\"",         m_oInsetPen )
			ComplexTypes_WriteAttribute ( L"filltype=\"",         m_oFillType )
			ComplexTypes_WriteAttribute2( L"src=\"",              m_sSrc )
			ComplexTypes_WriteAttribute ( L"imageaspect=\"",      m_oImageAspect )
			ComplexTypes_WriteAttribute2( L"imagesize=\"",        m_sImageSize )
			ComplexTypes_WriteAttribute ( L"imagealignshape=\"",  m_oImageAlignShape )
			ComplexTypes_WriteAttribute ( L"startarrow=\"",       m_oStartArrow )
			ComplexTypes_WriteAttribute ( L"startarrowwidth=\"",  m_oStartArrowWidth )
			ComplexTypes_WriteAttribute ( L"startarrowlength=\"", m_oStartArrowLength )
			ComplexTypes_WriteAttribute ( L"endarrow=\"",         m_oEndArrow )
			ComplexTypes_WriteAttribute ( L"endarrowwidth=\"",    m_oEndArrowWidth )
			ComplexTypes_WriteAttribute ( L"endarrowlength=\"",   m_oEndArrowLength )
			ComplexTypes_WriteAttribute2( L"o:href=\"",           m_sHref )
			ComplexTypes_WriteAttribute2( L"althref=\"",          m_sAlthref )
			ComplexTypes_WriteAttribute2( L"o:title=\"",          m_sTitle )
			ComplexTypes_WriteAttribute ( L"o:forcedash=\"",      m_oForceDash )

			sResult += L"/>";

			return sResult;
		}
		EElementType CStrokeChild::getType() const
		{
			return m_eType;
		}
		void CStrokeChild::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"althref",          m_sAlthref )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"color",            m_oColor )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"color2",           m_oColor2 )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"dashstyle",        m_oDashStyle )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"endarrow",         m_oEndArrow )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"endarrowlength",   m_oEndArrowLength )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"endarrowwidth",    m_oEndArrowWidth )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"endcap",           m_oEndCap )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"v:ext",            m_oExt )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"filltype",         m_oFillType )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"o:forcedash",      m_oForceDash )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"o:href",           m_sHref )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"imagealignshape",  m_oImageAlignShape )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"imageaspect",      m_oImageAspect )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"imagesize",        m_sImageSize )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"insetpen",         m_oInsetPen )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"joinstyle",        m_oJoinStyle )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"linestyle",        m_oLineStyle )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"miterlimit",       m_oMiterLimit )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"on",               m_oOn )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"opacity",          m_sOpacity )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"src",              m_sSrc )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"startarrow",       m_oStartArrow )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"startarrowlength", m_oStartArrowLength )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"startarrowwidth",  m_oStartArrowWidth )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"o:title",          m_sTitle )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"weight",           m_sWeight )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// CCallout 14.2.2.2 (Part 4)
		//--------------------------------------------------------------------------------

		CCallout::CCallout(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CCallout::~CCallout()
		{
		}
		void CCallout::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CCallout::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CCallout::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CCallout::toXML() const
		{
			std::wstring sResult = L"<o:callout ";

			ComplexTypes_WriteAttribute ( L"v:ext=\"",           m_oExt )
			ComplexTypes_WriteAttribute ( L"on=\"",              m_oOn )
			ComplexTypes_WriteAttribute ( L"type=\"",            m_oType )
			ComplexTypes_WriteAttribute ( L"gap=\"",             m_oGap )
			ComplexTypes_WriteAttribute ( L"angle=\"",           m_oAngle )
			ComplexTypes_WriteAttribute ( L"dropauto=\"",        m_oDropAuto )
			ComplexTypes_WriteAttribute ( L"drop=\"",            m_oDrop )
			ComplexTypes_WriteAttribute ( L"distance=\"",        m_oDistance )
			ComplexTypes_WriteAttribute ( L"lengthspecified=\"", m_oLengthSpecified )
			ComplexTypes_WriteAttribute ( L"length=\"",          m_oLength )
			ComplexTypes_WriteAttribute ( L"accentbar=\"",       m_oAccentbar )
			ComplexTypes_WriteAttribute ( L"textborder=\"",      m_oTextBorder )
			ComplexTypes_WriteAttribute ( L"minusx=\"",          m_oMinusX )
			ComplexTypes_WriteAttribute ( L"minusy=\"",          m_oMinusY )

			sResult += L"/>";

			return sResult;
		}
		EElementType CCallout::getType() const
		{
			return OOX::et_o_callout;
		}
		void CCallout::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"accentbar",       m_oAccentbar )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"angle",           m_oAngle )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"distance",        m_oDistance )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"drop",            m_oDrop )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"dropauto",        m_oDropAuto )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"v:ext",           m_oExt )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"gap",             m_oGap )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"length",          m_oLength )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"lengthspecified", m_oLengthSpecified )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"minusx",          m_oMinusX )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"minusy",          m_oMinusY )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"on",              m_oOn )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"textborder",      m_oTextBorder )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"type",            m_oType )
			WritingElement_ReadAttributes_End( oReader )
		}

		//--------------------------------------------------------------------------------
		// CClipPath 14.2.2.3 (Part 4)
		//--------------------------------------------------------------------------------

		CClipPath::CClipPath(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CClipPath::~CClipPath()
		{
		}
		void CClipPath::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CClipPath::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CClipPath::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CClipPath::toXML() const
		{
			std::wstring sResult = L"<o:clippath o:v=\"" + m_oV.ToString() + L"\"/>";
			return sResult;
		}
		EElementType CClipPath::getType() const
		{
			return OOX::et_o_clippath;
		}
		void CClipPath::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"o:v", m_oV )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// CColorMenu 14.2.2.4 (Part 4)
		//--------------------------------------------------------------------------------

		CColorMenu::CColorMenu()
		{
		}
		CColorMenu::~CColorMenu()
		{
		}
		void CColorMenu::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CColorMenu::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CColorMenu::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CColorMenu::toXML() const
		{
			std::wstring sResult = L"<o:colormenu ";

			ComplexTypes_WriteAttribute ( L"v:ext=\"",           m_oExt );

			sResult += L"extrusioncolor=\"" + m_oExtrusionColor.ToString() + L"\" ";
			sResult += L"fillcolor=\""      + m_oFillColor.ToString()      + L"\" ";
			sResult += L"shadowcolor=\""    + m_oShadowColor.ToString()    + L"\" ";
			sResult += L"strokecolor=\""    + m_oStrokeColor.ToString()    + L"\"/>";

			return sResult;
		}
		EElementType CColorMenu::getType() const
		{
			return OOX::et_o_colormenu;
		}
		void CColorMenu::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Устанавливаем значения по умолчанию
				m_oExtrusionColor = L"#000000";
				m_oFillColor      = L"#0000FF";
				m_oShadowColor    = L"#80800C";
				m_oStrokeColor    = L"#FFFF00";

				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"v:ext",          m_oExt )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"extrusioncolor", m_oExtrusionColor )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"fillcolor",      m_oFillColor )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"shadowcolor",    m_oShadowColor )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"strokecolor",    m_oStrokeColor )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// CColorMru 14.2.2.5 (Part 4)
		//--------------------------------------------------------------------------------

		CColorMru::CColorMru(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CColorMru::~CColorMru()
		{
			for ( unsigned int nIndex = 0; nIndex < m_arrColors.size(); nIndex++ )
			{
				if ( m_arrColors[nIndex] ) delete m_arrColors[nIndex];
				m_arrColors[nIndex] = NULL;
			}
			m_arrColors.clear();
		}
		void CColorMru::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CColorMru::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CColorMru::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CColorMru::toXML() const
		{
			std::wstring sResult = L"<o:colormru ";

			ComplexTypes_WriteAttribute ( L"v:ext=\"", m_oExt );
			sResult += L"colors=\"";

			for (unsigned int nIndex = 0; nIndex < m_arrColors.size(); nIndex++ )
			{
				if (m_arrColors[nIndex])
				{
					sResult += m_arrColors[nIndex]->ToString() + L",";
				}
			}

			sResult += L"\"/>";

			return sResult;
		}
		EElementType CColorMru::getType() const
		{
			return OOX::et_o_colormru;
		}
		void CColorMru::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Устанавливаем значения по умолчанию
				std::wstring sColors;

				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"v:ext",  m_oExt )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"colors", sColors )
				WritingElement_ReadAttributes_End( oReader )

				if ( L"" != sColors )
				{
					int nStartPos = 0;
					int nEndPos = -1;
					std::wstring sColor;
					while ( -1 != ( nEndPos =(int)sColors.find( L",", nStartPos )  ) )
					{
						sColor = sColors.substr( nStartPos, nEndPos - nStartPos );
						SimpleTypes::CColorType* oColor = new SimpleTypes::CColorType();
						if (oColor)
						{
							oColor->FromString(sColor);
							m_arrColors.push_back( oColor );
						}
						nStartPos = nEndPos + 1;
					}

					nEndPos = (int)sColors.length();
					sColor = sColors.substr( nStartPos, nEndPos - nStartPos );
					SimpleTypes::CColorType* oColor = new SimpleTypes::CColorType();
					if (oColor )
					{
						oColor->FromString(sColor);
						if (SimpleTypes::colortypeNone != oColor->GetValue()) m_arrColors.push_back( oColor );
					}
				}
			}

		//--------------------------------------------------------------------------------
		// CComplex 14.2.2.7 (Part 4)
		//--------------------------------------------------------------------------------

		CComplex::CComplex(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CComplex::~CComplex()
		{
		}
		void CComplex::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CComplex::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CComplex::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CComplex::toXML() const
		{
			std::wstring sResult = L"<o:complex ";
			ComplexTypes_WriteAttribute ( L"v:ext=\"",  m_oExt );
			sResult += L"\"/>";

			return sResult;
		}
		EElementType CComplex::getType() const
		{
			return OOX::et_o_complex;
		}
		void CComplex::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle( oReader, L"v:ext",  m_oExt )
			WritingElement_ReadAttributes_End( oReader )
		}

		//--------------------------------------------------------------------------------
		// CRelation 14.2.2.24 (Part 4)
		//--------------------------------------------------------------------------------

		CRelation::CRelation()
		{
		}
		CRelation::~CRelation()
		{
		}
		void CRelation::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CRelation::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CRelation::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CRelation::toXML() const
		{
			std::wstring sResult = L"<o:rel ";

			ComplexTypes_WriteAttribute ( L"v:ext=\"",  m_oExt );
			ComplexTypes_WriteAttribute2( L"idcntr=\"", m_sIdCntr );
			ComplexTypes_WriteAttribute2( L"iddest=\"", m_sIdDest );
			ComplexTypes_WriteAttribute2( L"idsrc=\"",  m_sIdSrc );

			sResult += L"\"/>";

			return sResult;
		}
		EElementType CRelation::getType() const
		{
			return OOX::et_o_rel;
		}
		void CRelation::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"v:ext",  m_oExt )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"idcntr", m_sIdCntr )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"iddest", m_sIdDest )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"idsrc",  m_sIdSrc )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// CRelationTable 14.2.2.25 (Part 4)
		//--------------------------------------------------------------------------------

		CRelationTable::CRelationTable()
		{
		}
		CRelationTable::~CRelationTable()
		{
			for ( unsigned int nIndex = 0; nIndex < m_arrRel.size(); nIndex++ )
			{
				if ( m_arrRel[nIndex] ) delete m_arrRel[nIndex];
				m_arrRel[nIndex] = NULL;
			}
			m_arrRel.clear();
		}
		void CRelationTable::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CRelationTable::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CRelationTable::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while ( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( L"o:rel" == sName )
				{
					OOX::VmlOffice::CRelation *oRel = new OOX::VmlOffice::CRelation(oReader);
					if (oRel) m_arrRel.push_back( oRel );
				}
			}
		}
		std::wstring CRelationTable::toXML() const
		{
			std::wstring sResult = L"<o:relationtable ";

			ComplexTypes_WriteAttribute ( L"v:ext=\"",  m_oExt );

			sResult += L"\">";

			for ( unsigned int nIndex = 0; nIndex < m_arrRel.size(); nIndex++ )
			{
				if (m_arrRel[nIndex])
					sResult += m_arrRel[nIndex]->toXML();
			}

			sResult += L"</o:relationtable>";

			return sResult;
		}
		EElementType CRelationTable::getType() const
		{
			return OOX::et_o_relationtable;
		}
		void CRelationTable::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"v:ext",  m_oExt )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// CDiagram 14.2.2.8 (Part 4)
		//--------------------------------------------------------------------------------

		CDiagram::CDiagram(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CDiagram::~CDiagram()
		{
		}
		void CDiagram::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CDiagram::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CDiagram::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while ( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( L"o:relationtable" == sName )
					m_oRelationTable = oReader;
			}
		}
		std::wstring CDiagram::toXML() const
		{
			std::wstring sResult = L"<o:diagram ";

			ComplexTypes_WriteAttribute ( L"v:ext=\"",            m_oExt )
			ComplexTypes_WriteAttribute ( L"dgmstyle=\"",         m_oDmgStyle )
			ComplexTypes_WriteAttribute ( L"autoformat=\"",       m_oAutoFormat )
			ComplexTypes_WriteAttribute ( L"reverse=\"",          m_oReverse )
			ComplexTypes_WriteAttribute ( L"autolayout=\"",       m_oAutoLayout )
			ComplexTypes_WriteAttribute ( L"dgmscalex=\"",        m_oDmgScaleX )
			ComplexTypes_WriteAttribute ( L"dgmscaley=\"",        m_oDmgScaleY )
			ComplexTypes_WriteAttribute ( L"dgmfontsize=\"",      m_oDmgFontSize )
			ComplexTypes_WriteAttribute2( L"constrainbounds=\"",  m_sConstrainbounds )
			ComplexTypes_WriteAttribute ( L"dgmbasetextscale=\"", m_oDmgBaseTextScale )

			sResult += L"\">";

			if ( m_oRelationTable.IsInit() )
				sResult += m_oRelationTable->toXML();

			sResult += L"</o:diagram>";

			return sResult;
		}
		EElementType CDiagram::getType() const
		{
			return OOX::et_o_diagram;
		}
		void CDiagram::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"autoformat",       m_oAutoFormat )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"autolayout",       m_oAutoLayout )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"constrainbounds",  m_sConstrainbounds )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"dgmbasetextscale", m_oDmgBaseTextScale )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"dgmfontsize",      m_oDmgFontSize )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"dgmscalex",        m_oDmgScaleX )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"dgmscaley",        m_oDmgScaleY )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"dgmstyle",         m_oDmgStyle )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"v:ext",            m_oExt )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"reverse",          m_oReverse )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// CEntry 14.2.2.9 (Part 4)
		//--------------------------------------------------------------------------------

		CEntry::CEntry()
		{
		}
		CEntry::~CEntry()
		{
		}
		void CEntry::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CEntry::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CEntry::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CEntry::toXML() const
		{
			std::wstring sResult = L"<o:entry ";

			ComplexTypes_WriteAttribute ( L"new=\"", m_oNew );
			ComplexTypes_WriteAttribute ( L"old=\"", m_oOld );

			sResult += L"\"/>";

			return sResult;
		}
		EElementType CEntry::getType() const
		{
			return OOX::et_o_entry;
		}
		void CEntry::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"new", m_oNew )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"old", m_oOld )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// CEquationXml 14.2.2.10 (Part 4)
		//--------------------------------------------------------------------------------

		CEquationXml::CEquationXml(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CEquationXml::~CEquationXml()
		{
		}
		void CEquationXml::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CEquationXml::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CEquationXml::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while ( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = oReader.GetName();

				// TO DO: Сделать чтение формул OOMML и Wc3MathML
			}
		}
		std::wstring CEquationXml::toXML() const
		{
			std::wstring sResult = L"<o:equationXml ";
			ComplexTypes_WriteAttribute ( L"contentType=\"", m_oContentType );
			sResult += L"\">";

			sResult += L"</o:equationXml>";

			return sResult;
		}
		EElementType CEquationXml::getType() const
		{
			return OOX::et_o_equationXml;
		}
		void CEquationXml::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"contentType", m_oContentType )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// CExtrusion 14.2.2.11 (Part 4)
		//--------------------------------------------------------------------------------

		CExtrusion::CExtrusion(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CExtrusion::~CExtrusion()
		{
		}
		void CExtrusion::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CExtrusion::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CExtrusion::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CExtrusion::toXML() const
		{
			std::wstring sResult = L"<o:extrusion ";

			ComplexTypes_WriteAttribute ( L"v:ext=\"", m_oExt );

			if ( SimpleTypes::booleanFalse != m_oOn.GetValue() )
				sResult += L"on=\"" + m_oOn.ToString() + L"\" ";

			if ( SimpleTypes::extrusiontypeParallel != m_oType.GetValue() )
				sResult += L"type=\"" + m_oType.ToString() + L"\" ";

			if ( SimpleTypes::extrusionrenderSolid != m_oRender.GetValue() )
				sResult += L"render=\"" + m_oRender.ToString() + L"\" ";

			if ( 0.5 != m_oViewPointOrigin.GetX() || -0.5 != m_oViewPointOrigin.GetY() )
				sResult += L"viewpointorigin=\"" + m_oRender.ToString() + L"\" ";

			if ( 0 != m_oViewPoint.GetX() || 0 != m_oViewPoint.GetY() || 0 != m_oViewPoint.GetZ() )
				sResult += L"viewpoint=\"" + m_oViewPoint.ToString() + L"\" ";

			if ( SimpleTypes::extrusionplaneXY != m_oPlane.GetValue() )
				sResult += L"plane=\"" + m_oPlane.ToString() + L"\" ";

			if ( 225 != m_oSkewAngle.GetValue() )
				sResult += L"skewangle=\"" + m_oSkewAngle.ToString() + L"\" ";

			if ( 50 != m_oSkewAmt.GetValue() )
				sResult += L"skewamt=\"" + m_oSkewAmt.ToString() + L"\" ";

			if ( 0 != m_oForeDepth.GetValue() )
				sResult += L"foredepth=\"" + m_oForeDepth.ToString() + L"\" ";

			if ( 36 != m_oBackDepth.GetValue() )
				sResult += L"backdepth=\"" + m_oBackDepth.ToString() + L"\" ";

			if ( 100 != m_oOrientation.GetX() || 0 != m_oOrientation.GetY() || 0 != m_oOrientation.GetZ() )
				sResult += L"orientation=\"" + m_oOrientation.ToString() + L"\" ";

			if ( 0 != m_oOrientationAngle.GetValue() )
				sResult += L"orientationangle=\"" + m_oOrientationAngle.ToString() + L"\" ";

			if ( SimpleTypes::booleanTrue != m_oLockRotationCenter.GetValue() )
				sResult += L"lockrotationcenter=\"" + m_oLockRotationCenter.ToString() + L"\" ";

			if ( SimpleTypes::booleanFalse != m_oAutoRotationCenter.GetValue() )
				sResult += L"autorotationcenter=\"" + m_oAutoRotationCenter.ToString() + L"\" ";

			if ( 0 != m_oRotationCenter.GetX() || 0 != m_oRotationCenter.GetY() || 0 != m_oRotationCenter.GetZ() )
				sResult += L"rotationcenter=\"" + m_oRotationCenter.ToString() + L"\" ";

			if ( 0 != m_oRotationAngle.GetX() || 0 != m_oRotationAngle.GetY() )
				sResult += L"rotationangle=\"" + m_oRotationAngle.ToString() + L"\" ";

			if ( SimpleTypes::colormodeAuto != m_oColorMode.GetValue() )
				sResult += L"colormode=\"" + m_oColorMode.ToString() + L"\" ";

			if ( m_oColor.IsInit() )
				sResult += L"color=\"" + m_oColor->ToString() + L"\" ";

			if ( 5 != m_oShininess.GetValue() )
				sResult += L"shininess=\"" + m_oShininess.ToString() + L"\" ";

			if ( 0 != m_oSpecularity.GetValue() )
				sResult += L"specularity=\"" + m_oSpecularity.ToString() + L"\" ";

			if ( 1 != m_oDiffusity.GetValue() )
				sResult += L"diffusity=\"" + m_oDiffusity.ToString() + L"\" ";

			if ( SimpleTypes::booleanFalse != m_oMetal.GetValue() )
				sResult += L"metal=\"" + m_oMetal.ToString() + L"\" ";

			if ( 1 != m_oEdge.GetValue() )
				sResult += L"edge=\"" + m_oEdge.ToString() + L"\" ";

			if ( 30000 != m_oFacet.GetValue() )
				sResult += L"facet=\"" + m_oFacet.ToString() + L"\" ";

			if ( SimpleTypes::booleanTrue != m_oLightFace.GetValue() )
				sResult += L"lightface=\"" + m_oLightFace.ToString() + L"\" ";

			if ( 0.3 != m_oBrightness.GetValue() )
				sResult += L"brightness=\"" + m_oBrightness.ToString() + L"\" ";

			if ( 50000 != m_oLightPosition.GetX() || 0 != m_oLightPosition.GetY() || 10000 != m_oLightPosition.GetZ() )
				sResult += L"lightposition=\"" + m_oLightPosition.ToString() + L"\" ";

			if ( 0.6 != m_oLightLevel.GetValue() )
				sResult += L"lightlevel=\"" + m_oLightLevel.ToString() + L"\" ";

			if ( SimpleTypes::booleanTrue != m_oLightHarsh.GetValue() )
				sResult += L"lightharsh=\"" + m_oLightHarsh.ToString() + L"\" ";

			if ( 50000 != m_oLightPosition2.GetX() || 0 != m_oLightPosition2.GetY() || 10000 != m_oLightPosition2.GetZ() )
				sResult += L"lightposition2=\"" + m_oLightPosition2.ToString() + L"\" ";

			if ( 0.6 != m_oLightLevel2.GetValue() )
				sResult += L"lightlevel2=\"" + m_oLightLevel2.ToString() + L"\" ";

			if ( SimpleTypes::booleanFalse != m_oLightHarsh2.GetValue() )
				sResult += L"lightharsh2=\"" + m_oLightHarsh2.ToString() + L"\" ";

			sResult += L"/>";

			return sResult;
		}
		EElementType CExtrusion::getType() const
		{
			return OOX::et_o_extrusion;
		}
		void CExtrusion::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Устанавливаем значения по умолчанию

				m_oBackDepth.SetValue( 36 );
				m_oBrightness.SetValue( 0.3 );
				m_oDiffusity.SetValue( (double)1.0 );
				m_oEdge.SetValue( 1 );
				m_oForeDepth.SetValue( 0 );
				m_oLightLevel.SetValue( 0.6 );
				m_oLightLevel2.SetValue( 0.6 );
				m_oLightPosition.SetValue( 50000, 0, 10000 );
				m_oLightPosition2.SetValue( 50000, 0, 10000 );
				m_oOrientation.SetValue( 100, 0, 0 );
				m_oRotationAngle.SetValue( 0, 0 );
				m_oRotationCenter.SetValue( 0, 0, 0 );
				m_oSkewAmt.SetValue( 50 );
				m_oSpecularity.SetValue( 0 );
				m_oViewPoint.SetValue( 0, 0, 0 );
				m_oViewPointOrigin.SetValue( 0.5, -0.5 );

				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"autorotationcenter", m_oAutoRotationCenter )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"backdepth",          m_oBackDepth )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"brightness",         m_oBrightness )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"color",              m_oColor )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"colormode",          m_oColorMode )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"diffusity",          m_oDiffusity )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"edge",               m_oEdge )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"v:ext",              m_oExt )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"facet",              m_oFacet )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"foredepth",          m_oForeDepth )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"lightface",          m_oLightFace )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"lightharsh",         m_oLightHarsh )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"lightharsh2",        m_oLightHarsh2 )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"lightlevel",         m_oLightLevel )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"lightlevel2",        m_oLightLevel2 )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"lightposition",      m_oLightPosition )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"lightposition2",     m_oLightPosition2 )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"lockrotationcenter", m_oLockRotationCenter )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"metal",              m_oMetal )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"on",                 m_oOn )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"orientation",        m_oOrientation )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"orientationangle",   m_oOrientationAngle )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"plane",              m_oPlane )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"render",             m_oRender )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"rotationangle",      m_oRotationAngle )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"rotationcenter",     m_oRotationCenter )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"shininess",          m_oShininess )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"skewamt",            m_oSkewAmt )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"skewangle",          m_oSkewAngle )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"specularity",        m_oSpecularity )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"type",               m_oType )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"viewpoint",          m_oViewPoint )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"viewpointorigin",    m_oViewPointOrigin )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// CFieldCodes 14.2.2.12 (Part 4)
		//--------------------------------------------------------------------------------

		CFieldCodes::CFieldCodes()
		{
		}
		CFieldCodes::~CFieldCodes()
		{
		}
		void CFieldCodes::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CFieldCodes::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CFieldCodes::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			m_sText = oReader.GetText2();
		}
		std::wstring CFieldCodes::toXML() const
		{
			std::wstring sResult = L"<o:FieldCodes>" + m_sText + L"</o:FieldCodes>";
			return sResult;
		}
		EElementType CFieldCodes::getType() const
		{
			return OOX::et_o_FieldCodes;
		}

		//--------------------------------------------------------------------------------
		// CFill 14.2.2.13 (Part 4)
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

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CFill::toXML() const
		{
			std::wstring sResult = L"<o:fill ";

			ComplexTypes_WriteAttribute ( L"v:ext=\"", m_oExt );
			ComplexTypes_WriteAttribute ( L"type=\"",  m_oType );

			sResult += L"/>";

			return sResult;
		}
		EElementType CFill::getType() const
		{
			return OOX::et_o_fill;
		}
		void CFill::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"v:ext", m_oExt )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"type",  m_oType )
			WritingElement_ReadAttributes_End( oReader )
		}

		//--------------------------------------------------------------------------------
		// CIdMap 14.2.2.14 (Part 4)
		//--------------------------------------------------------------------------------

		CIdMap::CIdMap()
		{
		}
		CIdMap::~CIdMap()
		{
		}
		void CIdMap::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CIdMap::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CIdMap::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CIdMap::toXML() const
		{
			std::wstring sResult = L"<o:idmap ";

			ComplexTypes_WriteAttribute ( L"v:ext=\"", m_oExt );
			sResult += m_sData.ToAttribute( L"data");

			sResult += L"/>";

			return sResult;
		}
		EElementType CIdMap::getType() const
		{
			return OOX::et_o_idmap;
		}
		void CIdMap::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"v:ext", m_oExt )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"data",  m_sData )
			WritingElement_ReadAttributes_End( oReader )
		}

		//--------------------------------------------------------------------------------
		// CInk 14.2.2.15 (Part 4)
		//--------------------------------------------------------------------------------

		CInk::CInk(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CInk::~CInk()
		{
		}
		void CInk::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CInk::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CInk::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CInk::toXML() const
		{
			std::wstring sResult = L"<o:ink ";

			ComplexTypes_WriteAttribute2( L"i=\"",           m_sI );
			ComplexTypes_WriteAttribute ( L"annotation=\"",  m_oAnnotation );
			ComplexTypes_WriteAttribute ( L"contentType=\"", m_oContentType );

			sResult += L"/>";

			return sResult;
		}
		EElementType CInk::getType() const
		{
			return OOX::et_o_ink;
		}
		void CInk::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"annotation",  m_oAnnotation )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"contentType", m_oContentType )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"i",           m_sI )
			WritingElement_ReadAttributes_End( oReader )
		}

		//--------------------------------------------------------------------------------
		// CLinkType 14.2.2.17 (Part 4)
		//--------------------------------------------------------------------------------

		CLinkType::CLinkType()
		{
		}
		CLinkType::~CLinkType()
		{
		}
		void CLinkType::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CLinkType::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CLinkType::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			std::wstring sText = oReader.GetText2();
			m_oValue = sText;
		}
		std::wstring CLinkType::toXML() const
		{
			std::wstring sResult = L"<o:LinkType>" + m_oValue.ToString() + L"</o:LinkType>";
			return sResult;
		}
		EElementType CLinkType::getType() const
		{
			return OOX::et_o_LinkType;
		}

		//--------------------------------------------------------------------------------
		// CLock 14.2.2.18 (Part 4)
		//--------------------------------------------------------------------------------

		CLock::CLock(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CLock::~CLock()
		{
		}
		void CLock::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CLock::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CLock::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CLock::toXML() const
		{
			std::wstring sResult = L"<o:lock ";

			ComplexTypes_WriteAttribute ( L"v:ext=\"", m_oExt );

			if ( SimpleTypes::booleanFalse != m_oPosition.GetValue() )
				sResult += L"position=\"" + m_oPosition.ToString() + L"\" ";

			if ( SimpleTypes::booleanFalse != m_oSelection.GetValue() )
				sResult += L"selection=\"" + m_oSelection.ToString() + L"\" ";

			if ( SimpleTypes::booleanFalse != m_oGrouping.GetValue() )
				sResult += L"grouping=\"" + m_oGrouping.ToString() + L"\" ";

			if ( SimpleTypes::booleanFalse != m_oUnGrouping.GetValue() )
				sResult += L"ungrouping=\"" + m_oUnGrouping.ToString() + L"\" ";

			if ( SimpleTypes::booleanFalse != m_oRotation.GetValue() )
				sResult += L"rotation=\"" + m_oRotation.ToString() + L"\" ";

			if ( SimpleTypes::booleanFalse != m_oCropping.GetValue() )
				sResult += L"cropping=\"" + m_oCropping.ToString() + L"\" ";

			if ( SimpleTypes::booleanFalse != m_oVerticies.GetValue() )
				sResult += L"verticies=\"" + m_oVerticies.ToString() + L"\" ";

			if ( SimpleTypes::booleanFalse != m_oAdjustHandles.GetValue() )
				sResult += L"adjusthandles=\"" + m_oAdjustHandles.ToString() + L"\" ";

			if ( SimpleTypes::booleanFalse != m_oText.GetValue() )
				sResult += L"text=\"" + m_oText.ToString() + L"\" ";

			if ( SimpleTypes::booleanFalse != m_oAspectRatio.GetValue() )
				sResult += L"aspectratio=\"" + m_oAspectRatio.ToString() + L"\" ";

			if ( SimpleTypes::booleanFalse != m_oShapeType.GetValue() )
				sResult += L"shapetype=\"" + m_oShapeType.ToString() + L"\" ";

			sResult += L"/>";

			return sResult;
		}
		EElementType CLock::getType() const
		{
			return OOX::et_o_lock;
		}
		void CLock::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"adjusthandles", m_oAdjustHandles )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"aspectratio",   m_oAspectRatio )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"cropping",      m_oCropping )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"v:ext",         m_oExt )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"grouping",      m_oGrouping )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"position",      m_oPosition )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"rotation",      m_oRotation )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"selection",     m_oSelection )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"shapetype",     m_oShapeType )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"text",          m_oText )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"ungrouping",    m_oUnGrouping )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"verticies",     m_oVerticies )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// CLockedField 14.2.2.19 (Part 4)
		//--------------------------------------------------------------------------------

		CLockedField::CLockedField()
		{
		}
		CLockedField::~CLockedField()
		{
		}
		void CLockedField::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CLockedField::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CLockedField::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			std::wstring sText = oReader.GetText2();
			m_oValue = sText;
		}
		std::wstring CLockedField::toXML() const
		{
			std::wstring sResult = L"<o:LockedField>" + m_oValue.ToString() + L"</o:LockedField>";
			return sResult;
		}
		EElementType CLockedField::getType() const
		{
			return OOX::et_o_LockedField;
		}

		//--------------------------------------------------------------------------------
		// COLEObject 14.2.2.20 (Part 4)
		//--------------------------------------------------------------------------------

		COLEObject::COLEObject(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		COLEObject::~COLEObject()
		{
		}
		void COLEObject::fromXML(XmlUtils::CXmlNode& oNode)
		{
			if ( L"o:OLEObject" != oNode.GetName() )
				return;

			XmlMacroReadAttributeBase( oNode, L"DrawAspect", m_oDrawAspect );
			XmlMacroReadAttributeBase( oNode, L"r:id", m_oId );
			XmlMacroReadAttributeBase( oNode, L"ObjectID", m_sObjectId );
			XmlMacroReadAttributeBase( oNode, L"ProgID", m_sProgId );
			XmlMacroReadAttributeBase( oNode, L"ShapeID", m_sShapeId );
			XmlMacroReadAttributeBase( oNode, L"Type", m_oType );
			XmlMacroReadAttributeBase( oNode, L"UpdateMode", m_oUpdateMode );

			if (false == m_oId.IsInit())
			{
				XmlMacroReadAttributeBase( oNode, L"relationships:id", m_oId );
			}
			//todo FieldCodes, LinkType, LockedField
		}
		void COLEObject::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while ( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( L"o:FieldCodes" == sName )
					m_oFieldCodes = oReader;
				else if ( L"o:LinkType" == sName )
					m_oLinkType = oReader;
				else if ( L"o:LockedField" == sName )
					m_oLockedField = oReader;
			}
		}
		std::wstring COLEObject::toXML() const
		{
			std::wstring sResult = L"<o:OLEObject ";

			ComplexTypes_WriteAttribute ( L"Type=\"",       m_oType )
			ComplexTypes_WriteAttribute3( L"ProgID=\"",     m_sProgId )
			ComplexTypes_WriteAttribute3( L"ShapeID=\"",    m_sShapeId )
			ComplexTypes_WriteAttribute ( L"DrawAspect=\"", m_oDrawAspect )
			ComplexTypes_WriteAttribute3( L"ObjectID=\"",   m_sObjectId )
			ComplexTypes_WriteAttribute ( L"r:id=\"",       m_oId )
			ComplexTypes_WriteAttribute ( L"UpdateMode=\"", m_oUpdateMode )

			sResult += L">";

			if ( m_oLinkType.IsInit() )
				sResult += m_oLinkType->toXML();

			if ( m_oLockedField.IsInit() )
				sResult += m_oLockedField->toXML();

			if ( m_oFieldCodes.IsInit() )
				sResult += m_oFieldCodes->toXML();

			sResult += L"</o:OLEObject>";

			return sResult;
		}
		EElementType COLEObject::getType() const
		{
			return OOX::et_o_OLEObject;
		}
		void COLEObject::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, L"DrawAspect", m_oDrawAspect )
					WritingElement_ReadAttributes_Read_else_if( oReader, L"r:id",       m_oId )
					WritingElement_ReadAttributes_Read_else_if( oReader, L"relationships:id", m_oId )
					WritingElement_ReadAttributes_Read_else_if( oReader, L"ObjectID",   m_sObjectId )
					WritingElement_ReadAttributes_Read_else_if( oReader, L"ProgID",     m_sProgId )
					WritingElement_ReadAttributes_Read_else_if( oReader, L"ShapeID",    m_sShapeId )
					WritingElement_ReadAttributes_Read_else_if( oReader, L"Type",       m_oType )
					WritingElement_ReadAttributes_Read_else_if( oReader, L"UpdateMode", m_oUpdateMode )
				//w:objectEmbed
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:drawAspect", m_oDrawAspect)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:progID",	m_sProgId)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:shapeID",	m_sShapeId)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"w:fieldCodes", m_oFieldCodes2)
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// CProxy 14.2.2.21 (Part 4)
		//--------------------------------------------------------------------------------

		CProxy::CProxy()
		{
		}
		CProxy::~CProxy()
		{
		}
		void CProxy::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CProxy::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CProxy::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CProxy::toXML() const
		{
			std::wstring sResult = L"<o:proxy ";

			sResult += L"start=\""      + m_oStart.ToString()      + L"\" ";
			sResult += L"end=\""        + m_oEnd.ToString()        + L"\" ";
			sResult += L"idref=\""      + m_sIdRef                 + L"\" ";
			sResult += L"connectloc=\"" + m_oConnectLoc.ToString() + L"\"/>";

			return sResult;
		}
		EElementType CProxy::getType() const
		{
			return OOX::et_o_proxy;
		}
		void CProxy::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, L"connectloc", m_oConnectLoc )
					WritingElement_ReadAttributes_Read_else_if( oReader, L"end",        m_oEnd )
					WritingElement_ReadAttributes_Read_else_if( oReader, L"idref",      m_sIdRef )
					WritingElement_ReadAttributes_Read_else_if( oReader, L"start",      m_oStart )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// CR 14.2.2.22 (Part 4)
		//--------------------------------------------------------------------------------

		CR::CR()
		{
		}
		CR::~CR()
		{
			for ( unsigned int nIndex = 0; nIndex < m_arrProxy.size(); nIndex++ )
			{
				if ( m_arrProxy[nIndex] ) delete m_arrProxy[nIndex];
				m_arrProxy[nIndex] = NULL;
			}
			m_arrProxy.clear();
		}
		void CR::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CR::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CR::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while ( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( L"o:proxy" == sName )
				{
					OOX::VmlOffice::CProxy *oProxy = new OOX::VmlOffice::CProxy(oReader);
					if (oProxy )m_arrProxy.push_back( oProxy );
				}
			}
		}
		std::wstring CR::toXML() const
		{
			std::wstring sResult = L"<o:r id=\"" + m_sId + L"\" ";

			ComplexTypes_WriteAttribute ( L"type=\"",  m_oType )
			ComplexTypes_WriteAttribute ( L"how=\"",   m_oHow )
			ComplexTypes_WriteAttribute2( L"idref=\"", m_sIdRef )

			sResult += L">";

			for ( unsigned int nIndex = 0; nIndex < m_arrProxy.size(); nIndex++ )
			{
				if (m_arrProxy[nIndex])
					sResult += m_arrProxy[nIndex]->toXML();
			}

			sResult += L"</o:r>";

			return sResult;
		}
		EElementType CR::getType() const
		{
			return OOX::et_o_r;
		}
		void CR::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"how",   m_oHow )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"id",    m_sId )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"idref", m_sIdRef )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"type",  m_oType )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// CRegroupTable 14.2.2.23 (Part 4)
		//--------------------------------------------------------------------------------

		CRegroupTable::CRegroupTable()
		{
		}
		CRegroupTable::~CRegroupTable()
		{
			for ( unsigned int nIndex = 0; nIndex < m_arrEntry.size(); nIndex++ )
			{
				if ( m_arrEntry[nIndex] ) delete m_arrEntry[nIndex];
				m_arrEntry[nIndex] = NULL;
			}
			m_arrEntry.clear();
		}
		void CRegroupTable::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CRegroupTable::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CRegroupTable::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while ( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( L"o:entry" == sName )
				{
					OOX::VmlOffice::CEntry *oEntry = new OOX::VmlOffice::CEntry (oReader);
					if (oEntry) m_arrEntry.push_back( oEntry );
				}
			}
		}
		std::wstring CRegroupTable::toXML() const
		{
			std::wstring sResult = L"<o:regrouptable ";

			ComplexTypes_WriteAttribute ( L"v:ext=\"", m_oExt );

			sResult += L">";

			for ( unsigned int nIndex = 0; nIndex < m_arrEntry.size(); nIndex++ )
			{
				if (m_arrEntry[nIndex])
					sResult += m_arrEntry[nIndex]->toXML();
			}

			sResult += L"</o:regrouptable>";

			return sResult;
		}
		EElementType CRegroupTable::getType() const
		{
			return OOX::et_o_regrouptable;
		}
		void CRegroupTable::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, L"v:ext", m_oExt )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// CRules 14.2.2.27 (Part 4)
		//--------------------------------------------------------------------------------

		CRules::CRules()
		{
		}
		CRules::~CRules()
		{
			for ( unsigned int nIndex = 0; nIndex < m_arrR.size(); nIndex++ )
			{
				if ( m_arrR[nIndex] ) delete m_arrR[nIndex];
				m_arrR[nIndex] = NULL;
			}
			m_arrR.clear();
		}
		void CRules::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CRules::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CRules::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while ( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( L"o:r" == sName )
				{
					OOX::VmlOffice::CR  *oR = new OOX::VmlOffice::CR (oReader);
					if ( oR) m_arrR.push_back( oR );
				}
			}
		}
		std::wstring CRules::toXML() const
		{
			std::wstring sResult = L"<o:rules ";

			ComplexTypes_WriteAttribute ( L"v:ext=\"", m_oExt );

			sResult += L">";

			for ( unsigned int nIndex = 0; nIndex < m_arrR.size(); nIndex++ )
			{
				if (m_arrR[nIndex])
					sResult += m_arrR[nIndex]->toXML();
			}

			sResult += L"</o:rules>";

			return sResult;
		}
		EElementType CRules::getType() const
		{
			return OOX::et_o_rules;
		}
		void CRules::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle( oReader, L"v:ext", m_oExt )
			WritingElement_ReadAttributes_End( oReader )
		}

		//--------------------------------------------------------------------------------
		// CShapeLayout 14.2.2.29 (Part 4)
		//--------------------------------------------------------------------------------

		CShapeLayout::CShapeLayout(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CShapeLayout:: ~CShapeLayout()
		{
		}
		void CShapeLayout::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CShapeLayout::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CShapeLayout::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while ( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( L"o:idmap" == sName )
					m_oIdMap = oReader;
				else if ( L"o:regrouptable" == sName )
					m_oRegroupTable = oReader;
				else if ( L"o:rules" == sName )
					m_oRules = oReader;
			}
		}
		std::wstring CShapeLayout::toXML() const
		{
			std::wstring sResult = L"<o:shapelayout ";

			ComplexTypes_WriteAttribute ( L"v:ext=\"", m_oExt );

			sResult += L">";

			if ( m_oIdMap.IsInit() )
				sResult += m_oIdMap->toXML();

			if ( m_oRegroupTable.IsInit() )
				sResult += m_oRegroupTable->toXML();

			if ( m_oRules.IsInit() )
				sResult += m_oRules->toXML();

			sResult += L"</o:shapelayout>";

			return sResult;
		}
		EElementType CShapeLayout::getType() const
		{
			return OOX::et_o_shapelayout;
		}
		void CShapeLayout::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle( oReader, L"v:ext", m_oExt )
			WritingElement_ReadAttributes_End( oReader )
		}

		//--------------------------------------------------------------------------------
		// CSignatureLine 14.2.2.30 (Part 4)
		//--------------------------------------------------------------------------------

		CSignatureLine::CSignatureLine(OOX::Document *pMain) : PPTX::WrapperWritingElement(pMain)
		{
		}
		CSignatureLine::~CSignatureLine()
		{
		}
		void CSignatureLine::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase(oNode, L"o:addlxml", m_sAddXml);
			XmlMacroReadAttributeBase(oNode, L"allowcomments", m_oAllowComments);
			XmlMacroReadAttributeBase(oNode, L"v:ext", m_oExt);
			XmlMacroReadAttributeBase(oNode, L"id", m_oId);
			XmlMacroReadAttributeBase(oNode, L"issignatureline", m_oIsSignatureLine);
			XmlMacroReadAttributeBase(oNode, L"provid", m_oProvId);
			XmlMacroReadAttributeBase(oNode, L"showsigndate", m_oShowSignDate);
			XmlMacroReadAttributeBase(oNode, L"o:signinginstructions", m_sSigningInstructions);
			XmlMacroReadAttributeBase(oNode, L"signinginstructionsset", m_oSigningInstructionsSet);
			XmlMacroReadAttributeBase(oNode, L"o:sigprovurl", m_sSigProvUrl);
			XmlMacroReadAttributeBase(oNode, L"o:suggestedsigner", m_sSuggestedSigner);
			XmlMacroReadAttributeBase(oNode, L"o:suggestedsigner2", m_sSuggestedSigner2);
			XmlMacroReadAttributeBase(oNode, L"o:suggestedsigneremail", m_sSuggestedSignerEmail);
		}
		void CSignatureLine::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CSignatureLine::toXML() const
		{
			std::wstring sResult = L"<o:signatureline ";

			ComplexTypes_WriteAttribute ( L"v:ext=\"", m_oExt );

			if ( m_oIsSignatureLine.IsInit() )
				sResult += L"issignatureline=\"" + m_oIsSignatureLine->ToString2(SimpleTypes::onofftostringT) + L"\" ";

			ComplexTypes_WriteAttribute ( L"id=\"",     m_oId );
			ComplexTypes_WriteAttribute ( L"provid=\"", m_oProvId );

			if ( m_oSigningInstructionsSet.IsInit() )
				sResult += L"signinginstructionsset=\"" + m_oSigningInstructionsSet->ToString2(SimpleTypes::onofftostringT) + L"\" ";

			if ( m_oAllowComments.IsInit() )
				sResult += L"allowcomments=\"" + m_oAllowComments->ToString2(SimpleTypes::onofftostringT) + L"\" ";

			if ( m_oShowSignDate.IsInit() )
				sResult += L"showsigndate=\"" + m_oShowSignDate->ToString2(SimpleTypes::onofftostringT) + L"\" ";

			ComplexTypes_WriteAttribute2Encode( L"o:suggestedsigner=\"",      m_sSuggestedSigner );
			ComplexTypes_WriteAttribute2Encode( L"o:suggestedsigner2=\"",     m_sSuggestedSigner2 );
			ComplexTypes_WriteAttribute2Encode( L"o:suggestedsigneremail=\"", m_sSuggestedSignerEmail );
			ComplexTypes_WriteAttribute2Encode( L"o:signinginstructions=\"",  m_sSigningInstructions );
			ComplexTypes_WriteAttribute2Encode( L"o:addlxml=\"",              m_sAddXml );
			ComplexTypes_WriteAttribute2Encode( L"o:sigprovurl=\"",           m_sSigProvUrl );

			sResult += L"/>";

			return sResult;
		}
		EElementType CSignatureLine::getType() const
		{
			return OOX::et_o_signatureline;
		}
		void CSignatureLine::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				switch (_at)
				{
					case 0:
					{
						m_sAddXml = pReader->GetString2();
						break;
					}
					case 1:
					{
						m_oAllowComments.Init();
						m_oAllowComments->FromBool(pReader->GetBool());
						break;
					}
					case 2:
					{
						m_oExt.Init();
						m_oExt->SetValue((SimpleTypes::EExt)pReader->GetUChar());
						break;
					}
					case 3:
					{
						m_oId = pReader->GetString2();
						break;
					}
					case 4:
					{
						m_oIsSignatureLine.Init();
						m_oIsSignatureLine->FromBool(pReader->GetBool());
						break;
					}
					case 5:
					{
						m_oProvId = pReader->GetString2();
						break;
					}
					case 6:
					{
						m_oShowSignDate.Init();
						m_oShowSignDate->FromBool(pReader->GetBool());
						break;
					}
					case 7:
					{
						m_sSigningInstructions = pReader->GetString2();
						break;
					}
					case 8:
					{
						m_oSigningInstructionsSet.Init();
						m_oSigningInstructionsSet->FromBool(pReader->GetBool());
						break;
					}
					case 9:
					{
						m_sSigProvUrl = pReader->GetString2();
						break;
					}
					case 10:
					{
						m_sSuggestedSigner = pReader->GetString2();
						break;
					}
					case 11:
					{
						m_sSuggestedSigner2 = pReader->GetString2();
						break;
					}
					case 12:
					{
						m_sSuggestedSignerEmail = pReader->GetString2();
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void CSignatureLine::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			if (m_sAddXml.IsInit())
				pWriter->WriteString1(0, m_sAddXml.get());
			if (m_oAllowComments.IsInit())
				pWriter->WriteBool1(1, m_oAllowComments->ToBool());
			if (m_oExt.IsInit())
			{
				pWriter->WriteBYTE(2);
				pWriter->WriteBYTE(m_oExt->GetValue());
			}
			if (m_oId.IsInit())
				pWriter->WriteString1(3, m_oId->ToString());
			if (m_oIsSignatureLine.IsInit())
				pWriter->WriteBool1(4, m_oIsSignatureLine->ToBool());
			if (m_oProvId.IsInit())
				pWriter->WriteString1(5, m_oProvId->ToString());
			if (m_oShowSignDate.IsInit())
				pWriter->WriteBool1(6, m_oShowSignDate->ToBool());
			if (m_sSigningInstructions.IsInit())
				pWriter->WriteString1(7, m_sSigningInstructions.get());
			if (m_oSigningInstructionsSet.IsInit())
				pWriter->WriteBool1(8, m_oSigningInstructionsSet->ToBool());
			if (m_sSigProvUrl.IsInit())
				pWriter->WriteString1(9, m_sSigProvUrl.get());
			if (m_sSuggestedSigner.IsInit())
				pWriter->WriteString1(10, m_sSuggestedSigner.get());
			if (m_sSuggestedSigner2.IsInit())
				pWriter->WriteString1(11, m_sSuggestedSigner2.get());
			if (m_sSuggestedSignerEmail.IsInit())
				pWriter->WriteString1(12, m_sSuggestedSignerEmail.get());
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void CSignatureLine::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->WriteString(toXML());
		}
		void CSignatureLine::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"o:addlxml",              m_sAddXml )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"allowcomments",          m_oAllowComments )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"v:ext",                  m_oExt )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"id",                     m_oId )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"issignatureline",        m_oIsSignatureLine )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"provid",                 m_oProvId )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"showsigndate",           m_oShowSignDate )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"o:signinginstructions",  m_sSigningInstructions )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"signinginstructionsset", m_oSigningInstructionsSet )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"o:sigprovurl",           m_sSigProvUrl )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"o:suggestedsigner",      m_sSuggestedSigner )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"o:suggestedsigner2",     m_sSuggestedSigner2 )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"o:suggestedsigneremail", m_sSuggestedSignerEmail )
				WritingElement_ReadAttributes_End( oReader )
			}

		//--------------------------------------------------------------------------------
		// CSkew 14.2.2.31 (Part 4)
		//--------------------------------------------------------------------------------

		CSkew::CSkew(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CSkew::~CSkew()
		{
		}
		void CSkew::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать CSkew::fromXML(XmlUtils::CXmlNode& oNode)
		}
		void CSkew::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CSkew::toXML() const
		{
			std::wstring sResult = L"<o:skew ";

			ComplexTypes_WriteAttribute ( L"v:ext=\"", m_oExt );
			ComplexTypes_WriteAttribute2( L"id=\"",    m_sId );

			if ( SimpleTypes::booleanFalse != m_oOn.GetValue() )
				sResult += L"on=\"" + m_oOn.ToString() + L"\" ";

			ComplexTypes_WriteAttribute2( L"offset=\"", m_sOffset );
			ComplexTypes_WriteAttribute2( L"origin=\"", m_sOrigin );
			ComplexTypes_WriteAttribute2( L"matrix=\"", m_sMatrix );

			sResult += L"/>";

			return sResult;
		}
		EElementType CSkew::getType() const
		{
			return OOX::et_o_skew;
		}
		void CSkew::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"v:ext",  m_oExt )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"id",     m_sId )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"matrix", m_sMatrix )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"offset", m_sOffset )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"on",     m_oOn )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"origin", m_sOrigin )
				WritingElement_ReadAttributes_End( oReader )

				// TO DO: OOX::Vml::CSkew сделать нормальный парсер параметров
			}

	} // namespace Vml
} // namespace OOX
