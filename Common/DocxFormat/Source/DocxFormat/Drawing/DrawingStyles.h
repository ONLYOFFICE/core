/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#pragma once
#ifndef OOX_LOGIC_DRAWING_STYLES_INCLUDE_H_
#define OOX_LOGIC_DRAWING_STYLES_INCLUDE_H_

#include "../../Base/Nullable.h"
#include "../../Common/SimpleTypes_Drawing.h"
#include "../../Common/SimpleTypes_Shared.h"

#include "../WritingElement.h"

#include "DrawingStyles2.h"
#include "DrawingExt.h"
#include "DrawingEffects.h"
#include "DrawingRun.h"
#include "DrawingTextProperties.h"
#include "DrawingCoreInfo.h"

namespace OOX
{
	namespace Drawing
	{
		//-----------------------------------------------------------------------
        // CStyleColor 20.1.4.1.1;20.1.4.1.2;20.1.4.1.3;20.1.4.1.4;20.1.4.1.5;
		//             20.1.4.1.6;20.1.4.1.9;20.1.4.1.10;20.1.4.1.15;20.1.4.1.19;
		//             20.1.4.1.22;20.1.4.1.23;
        //-----------------------------------------------------------------------				
		class CStyleColor : public OOX::Drawing::CColor
		{
		public:
			WritingElement_AdditionConstructors(CStyleColor)
			CStyleColor()
			{
				m_eType = et_Unknown;
			}
			virtual ~CStyleColor()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				m_eType = et_Unknown;
				CColor::fromXML( oNode );
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				m_eType = et_Unknown;
                CString sName = oReader.GetName();
				wchar_t wChar2 = sName[2];

				switch ( wChar2 )
				{
				case 'a':
					if ( _T("a:accent1") == sName )
						m_eType = et_a_accent1;
					else if ( _T("a:accent2") == sName )
						m_eType = et_a_accent2;
					else if ( _T("a:accent3") == sName )
						m_eType = et_a_accent3;
					else if ( _T("a:accent4") == sName )
						m_eType = et_a_accent4;
					else if ( _T("a:accent5") == sName )
						m_eType = et_a_accent5;
					else if ( _T("a:accent6") == sName )
						m_eType = et_a_accent6;
					break;

				case 'd':
					if ( _T("a:dk1") == sName )
						m_eType = et_a_dk1;
					else if ( _T("a:dk2") == sName )
						m_eType = et_a_dk2;
					break;

				case 'f':
					if ( _T("a:folHlink") == sName )
						m_eType = et_a_folHlink;
					break;

				case 'h':
					if ( _T("a:hlink") == sName )
						m_eType = et_a_hlink;
					break;

				case 'l':
					if ( _T("a:lt1") == sName )
						m_eType = et_a_lt1;
					else if ( _T("a:lt2") == sName )
						m_eType = et_a_lt2;
					break;

				default:
					return;
				}

				CColor::fromXML( oReader );
			}

			virtual CString      toXML() const
			{
				CString sResult;
				
				switch ( m_eType )
				{
				case et_a_accent1:  sResult = _T("<a:accent1>"); break;
				case et_a_accent2:  sResult = _T("<a:accent2>"); break;
				case et_a_accent3:  sResult = _T("<a:accent3>"); break;
				case et_a_accent4:  sResult = _T("<a:accent4>"); break;
				case et_a_accent5:  sResult = _T("<a:accent5>"); break;
				case et_a_accent6:  sResult = _T("<a:accent6>"); break;
				case et_a_dk1:      sResult = _T("<a:dk1>"); break;
				case et_a_dk2:      sResult = _T("<a:dk2>"); break;
				case et_a_folHlink: sResult = _T("<a:folHlink>"); break;
				case et_a_hlink:    sResult = _T("<a:hlink>"); break;
				case et_a_lt1:      sResult = _T("<a:lt1>"); break;
				case et_a_lt2:      sResult = _T("<a:lt2>"); break;
				default: return _T("");
				}

				sResult += CColor::toXML();

				switch ( m_eType )
				{
				case et_a_accent1:  sResult += _T("</a:accent1>"); break;
				case et_a_accent2:  sResult += _T("</a:accent2>"); break;
				case et_a_accent3:  sResult += _T("</a:accent3>"); break;
				case et_a_accent4:  sResult += _T("</a:accent4>"); break;
				case et_a_accent5:  sResult += _T("</a:accent5>"); break;
				case et_a_accent6:  sResult += _T("</a:accent6>"); break;
				case et_a_dk1:      sResult += _T("</a:dk1>"); break;
				case et_a_dk2:      sResult += _T("</a:dk2>"); break;
				case et_a_folHlink: sResult += _T("</a:folHlink>"); break;
				case et_a_hlink:    sResult += _T("</a:hlink>"); break;
				case et_a_lt1:      sResult += _T("</a:lt1>"); break;
				case et_a_lt2:      sResult += _T("</a:lt2>"); break;
				}

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_accent1;
			}

		public:

			EElementType m_eType;

		};


 		//--------------------------------------------------------------------------------
		// CBackgroundFillStyleList 20.1.4.1.7 (Part 1)
		//--------------------------------------------------------------------------------	
		class CBackgroundFillStyleList : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CBackgroundFillStyleList)
			CBackgroundFillStyleList()
			{
			}
			CBackgroundFillStyleList(const CBackgroundFillStyleList& oOther)
			{
				for (unsigned int nIndex = 0; nIndex < oOther.m_arrItems.size(); nIndex++ )
				{
					OOX::EElementType eType = oOther.m_arrItems[nIndex]->getType();

					WritingElement *pItem = NULL;
					switch ( eType )
					{
					case et_a_blipFill: pItem = new CBlipFillProperties      ( (const CBlipFillProperties&) *oOther.m_arrItems[nIndex] ); break;
					case et_a_gradFill: pItem = new CGradientFillProperties  ( (const CGradientFillProperties&) *oOther.m_arrItems[nIndex] ); break;
					case et_a_grpFill:  pItem = new CGroupFillProperties     ( (const CGroupFillProperties&) *oOther.m_arrItems[nIndex] ); break;
					case et_a_noFill:   pItem = new CNoFillProperties        ( (const CNoFillProperties&) *oOther.m_arrItems[nIndex] ); break;
					case et_a_pattFill: pItem = new CPatternFillProperties   ( (const CPatternFillProperties&) *oOther.m_arrItems[nIndex] ); break;
					case et_a_solidFill:pItem = new CSolidColorFillProperties( (const CSolidColorFillProperties&) *oOther.m_arrItems[nIndex] ); break;
					}

					if ( NULL != pItem )
						m_arrItems.push_back( pItem );
				}
			}
			virtual ~CBackgroundFillStyleList()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CBackgroundFillStyleList::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{		
				ClearItems();

				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
                    CString sName = oReader.GetName();
					WritingElement *pItem = NULL;

					if ( _T("a:blipFill") == sName )
						pItem = new OOX::Drawing::CBlipFillProperties( oReader );
					else if ( _T("a:gradFill") == sName )
						pItem = new OOX::Drawing::CGradientFillProperties( oReader );
					else if ( _T("a:grpFill") == sName )
						pItem = new OOX::Drawing::CGroupFillProperties( oReader );
					else if ( _T("a:noFill") == sName )
						pItem = new OOX::Drawing::CNoFillProperties( oReader );
					else if ( _T("a:pattFill") == sName )
						pItem = new OOX::Drawing::CPatternFillProperties( oReader );
					else if ( _T("a:solidFill") == sName )
						pItem = new OOX::Drawing::CSolidColorFillProperties( oReader );

					if ( pItem )
						m_arrItems.push_back( pItem );
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:bgFillStyleLst>"); 

				for ( unsigned int nIndex = 0; nIndex < m_arrItems.size(); nIndex++ )
				{
					if ( m_arrItems[nIndex] )
					{
						sResult += m_arrItems[nIndex]->toXML();
					}
				}
				
				sResult += _T("</a:bgFillStyleLst>"); 

				return sResult;
			}
			virtual EElementType getType() const
			{
				return et_a_bgFillStyleLst;
			}


		public:

			// Childs
		};
		//-----------------------------------------------------------------------
        // CCustomColor 20.1.4.1.8
        //-----------------------------------------------------------------------				
		class CCustomColor : public OOX::Drawing::CColor
		{
		public:
			WritingElement_AdditionConstructors(CCustomColor)
			CCustomColor()
			{
			}
			virtual ~CCustomColor()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				CColor::fromXML( oNode );
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				CColor::fromXML( oReader );
			}

			virtual CString      toXML() const
			{
				CString sResult = _T("<a:custClr name=\"") + m_sName + _T("\">");

				sResult += CColor::toXML();

				sResult += _T("</a:custClr>");
				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_custClr;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("name"), m_sName )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			CString m_sName;

		};
		//--------------------------------------------------------------------------------
		// CEffectStyleItem 20.1.4.1.11 (Part 1)
		//--------------------------------------------------------------------------------	
		class CEffectStyleItem : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CEffectStyleItem)
			CEffectStyleItem()
			{
				m_eEffectType = effecttypeUnknown;
			}
			virtual ~CEffectStyleItem()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				m_eEffectType = effecttypeUnknown;
				// TO DO: Реализовать CDefaultShapeDefinition::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				m_eEffectType = effecttypeUnknown;

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();
					if ( _T("a:effectDag") == sName )
					{
						m_oEffectDag  = oReader;
						m_eEffectType = effecttypeDag;
					}
					else if ( _T("a:effectLst") == sName )
					{
						m_oEffectList = oReader;
						m_eEffectType = effecttypeLst;
					}
					else if ( _T("a:scene3d") == sName )
						m_oScene3D = oReader;
					else if ( _T("a:sp3d") == sName )
						m_oSp3D = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:effectStyle>"); 

				switch ( m_eEffectType )
				{
				case effecttypeDag:

					if ( m_oEffectDag.IsInit() )
						sResult += m_oEffectDag->toXML();
					break;

				case effecttypeLst:

					if ( m_oEffectList.IsInit() )
						sResult += m_oEffectList->toXML();
					break;
				}

				if ( m_oScene3D.IsInit() )
					sResult += m_oScene3D->toXML();

				if ( m_oSp3D.IsInit() )
					sResult += m_oSp3D->toXML();

				sResult += _T("</a:effectStyle>"); 

				return sResult;
			}
			virtual EElementType getType() const
			{
				return et_a_effectStyle;
			}

		public:

			// Childs
			EEffectType                                       m_eEffectType; // Tип контейнера эффектов
			nullable<OOX::Drawing::CEffectContainer>          m_oEffectDag;
			nullable<OOX::Drawing::CEffectList>               m_oEffectList;

			nullable<OOX::Drawing::CScene3D>                  m_oScene3D;
			nullable<OOX::Drawing::CShape3D>                  m_oSp3D;
		};

		//--------------------------------------------------------------------------------
		// CEffectStyleList 20.1.4.1.12 (Part 1)
		//--------------------------------------------------------------------------------	
		class CEffectStyleList : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CEffectStyleList)
			CEffectStyleList()
			{
			}
			virtual ~CEffectStyleList()
			{
				for (unsigned int nIndex = 0; nIndex < m_arrEffectStyle.size(); nIndex++ )
				{
					if ( m_arrEffectStyle[nIndex] )
						delete m_arrEffectStyle[nIndex];

					m_arrEffectStyle[nIndex] = NULL;
				}

				m_arrEffectStyle.clear();
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CEffectStyleList::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();
					if ( _T("a:effectStyle") == sName )
					{
						OOX::Drawing::CEffectStyleItem *oEffectStyle = new OOX::Drawing::CEffectStyleItem(oReader);
						if (oEffectStyle )m_arrEffectStyle.push_back( oEffectStyle );
					}
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:effectStyleLst>"); 

				for (unsigned int nIndex = 0; nIndex < m_arrEffectStyle.size(); nIndex++ )
				{
					if (m_arrEffectStyle[nIndex])
						sResult += m_arrEffectStyle[nIndex]->toXML();
				}

				sResult += _T("</a:effectStyleLst>"); 

				return sResult;
			}
			virtual EElementType getType() const
			{
				return et_a_effectStyleLst;
			}

		public:

			// Childs
			std::vector<OOX::Drawing::CEffectStyleItem*> m_arrEffectStyle;
		};

 		//--------------------------------------------------------------------------------
		// CFillStyleList 20.1.4.1.13 (Part 1)
		//--------------------------------------------------------------------------------	
		class CFillStyleList : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CFillStyleList)
			CFillStyleList()
			{
			}
			CFillStyleList(const CFillStyleList& oOther)
			{
				for (unsigned  int nIndex = 0; nIndex < oOther.m_arrItems.size(); nIndex++ )
				{
					OOX::EElementType eType = oOther.m_arrItems[nIndex]->getType();

					WritingElement *pItem = NULL;
					switch ( eType )
					{
					case et_a_blipFill: pItem = new CBlipFillProperties      ( (const CBlipFillProperties&) *oOther.m_arrItems[nIndex] ); break;
					case et_a_gradFill: pItem = new CGradientFillProperties  ( (const CGradientFillProperties&) *oOther.m_arrItems[nIndex] ); break;
					case et_a_grpFill:  pItem = new CGroupFillProperties     ( (const CGroupFillProperties&) *oOther.m_arrItems[nIndex] ); break;
					case et_a_noFill:   pItem = new CNoFillProperties        ( (const CNoFillProperties&) *oOther.m_arrItems[nIndex] ); break;
					case et_a_pattFill: pItem = new CPatternFillProperties   ( (const CPatternFillProperties&) *oOther.m_arrItems[nIndex] ); break;
					case et_a_solidFill:pItem = new CSolidColorFillProperties( (const CSolidColorFillProperties&) *oOther.m_arrItems[nIndex] ); break;
					}

					if ( NULL != pItem )
						m_arrItems.push_back( pItem );
				}
			}
			virtual ~CFillStyleList()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CFillStyleList::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{	
				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
                    CString sName = oReader.GetName();
					WritingElement *pItem = NULL;

					if ( _T("a:blipFill") == sName )
						pItem = new OOX::Drawing::CBlipFillProperties( oReader );
					else if ( _T("a:gradFill") == sName )
						pItem = new OOX::Drawing::CGradientFillProperties( oReader );
					else if ( _T("a:grpFill") == sName )
						pItem = new OOX::Drawing::CGroupFillProperties( oReader );
					else if ( _T("a:noFill") == sName )
						pItem = new OOX::Drawing::CNoFillProperties( oReader );
					else if ( _T("a:pattFill") == sName )
						pItem = new OOX::Drawing::CPatternFillProperties( oReader );
					else if ( _T("a:solidFill") == sName )
						pItem = new OOX::Drawing::CSolidColorFillProperties( oReader );

					if ( pItem )
						m_arrItems.push_back( pItem );
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:fillStyleLst>"); 

				for (unsigned  int nIndex = 0; nIndex < m_arrItems.size(); nIndex++ )
				{
					if ( m_arrItems[nIndex] )
					{
						sResult += m_arrItems[nIndex]->toXML();
					}
				}
				
				sResult += _T("</a:fillStyleLst>"); 

				return sResult;
			}
			virtual EElementType getType() const
			{
				return et_a_fillStyleLst;
			}

		public:

			// Childs
		};
		//--------------------------------------------------------------------------------
		// CSupplementalFont 20.1.4.1.16 (Part 1)
		//--------------------------------------------------------------------------------	
		class CSupplementalFont : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSupplementalFont)
			CSupplementalFont()
			{
			}
			virtual ~CSupplementalFont()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CSupplementalFont::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{			
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:font script=\"") + m_sScript + _T("\" typeface=\"") + m_oTypeFace.ToString() + _T("\"/>");
				return sResult;
			}
			virtual EElementType getType() const
			{
				return et_a_font;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				if ( oReader.GetAttributesCount() <= 0 )
					return;
				
				if ( !oReader.MoveToFirstAttribute() )
					return;
				
                CString wsName = oReader.GetName();
                while( !wsName.IsEmpty() )
				{
					wchar_t wsChar0 = wsName[0]; 

					switch ( wsChar0 )
					{
					case 's':
						if      ( _T("script")   == wsName ) m_sScript   = oReader.GetText();
						break;

					case 't':
						if      ( _T("typeface") == wsName ) m_oTypeFace = oReader.GetText();
						break;
					}

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}
				oReader.MoveToElement();
			}

		public:

			EElementType m_eType;

			// Attributes
			CString                    m_sScript;
			SimpleTypes::CTextTypeface m_oTypeFace;
		};
		//--------------------------------------------------------------------------------
		// CDefaultShapeDefinition 20.1.4.1.20 (Part 1)
		//--------------------------------------------------------------------------------	
		class CDefaultShapeDefinition : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDefaultShapeDefinition)
			CDefaultShapeDefinition()
			{
				m_eType = et_Unknown;
			}
			virtual ~CDefaultShapeDefinition()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				m_eType = et_Unknown;
				// TO DO: Реализовать CDefaultShapeDefinition::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				m_eType = et_Unknown;
                CString sName = oReader.GetName();
				if ( _T("a:lnDef") == sName )
					m_eType = et_a_lnDef;
				else if ( _T("a:spDef") == sName )
					m_eType = et_a_spDef;
				else if ( _T("a:txDef") == sName )
					m_eType = et_a_txDef;
				else 
					return;

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();
					if ( _T("a:bodyPr") == sName )
						m_oBodyPr = oReader;
					else if ( _T("a:extLst") == sName )
						m_oExtLst = oReader;
					//else if ( _T("a:lstStyle") == sName )
					//	m_oLstStyle = oReader;
					else if ( _T("a:spPr") == sName )
						m_oSpPr = oReader;
					else if ( _T("a:style") == sName )
						m_oStyle = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult;
				
				switch ( m_eType )
				{
				case et_a_lnDef: sResult = _T("<a:lnDef>"); break;
				case et_a_spDef: sResult = _T("<a:spDef>"); break;
				case et_a_txDef: sResult = _T("<a:txDef>"); break;
				default: return _T("");
				}

				sResult += m_oSpPr.toXML();
				sResult += m_oBodyPr.toXML();
				//sResult += m_oLstStyle.toXML();
				
				if ( m_oStyle.IsInit() )
					sResult += m_oStyle->toXML();

				if ( m_oExtLst.IsInit() )
					sResult += m_oExtLst->toXML();

				switch ( m_eType )
				{
				case et_a_lnDef: sResult += _T("</a:lnDef>"); break;
				case et_a_spDef: sResult += _T("</a:spDef>"); break;
				case et_a_txDef: sResult += _T("</a:txDef>"); break;
				}

				return sResult;
			}
			virtual EElementType getType() const
			{
				return m_eType;
			}

		public:

			EElementType                                    m_eType;

			// Childs
			OOX::Drawing::CTextBodyProperties               m_oBodyPr;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
			// TO DO: Написать класс, читающий a:lstStyle
			//OOX::Drawing::CTextListStyle                    m_oLstStyle;
			OOX::Drawing::CShapeProperties                  m_oSpPr;
			nullable<OOX::Drawing::CShapeStyle>             m_oStyle;
		};


		//--------------------------------------------------------------------------------
		// CLineStyleList 20.1.4.1.21 (Part 1)
		//--------------------------------------------------------------------------------	
		class CLineStyleList : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CLineStyleList)
			CLineStyleList()
			{
			}
			virtual ~CLineStyleList()
			{
				for ( unsigned int nIndex = 0; nIndex < m_arrLn.size(); nIndex++ )
				{
					if ( m_arrLn[nIndex] ) delete m_arrLn[nIndex];
					m_arrLn[nIndex] = NULL;
				}
				m_arrLn.clear();
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CLineStyleList::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();
					if ( _T("a:ln") == sName )
					{
						OOX::Drawing::CLineProperties *oLP = new OOX::Drawing::CLineProperties(oReader);
						m_arrLn.push_back( oLP );
					}
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:lnStyleLst>"); 

				for (unsigned int nIndex = 0; nIndex < m_arrLn.size(); nIndex++ )
				{
					if (m_arrLn[nIndex])
						sResult += m_arrLn[nIndex]->toXML();
				}

				sResult += _T("</a:lnStyleLst>"); 

				return sResult;
			}
			virtual EElementType getType() const
			{
				return et_a_lnStyleLst;
			}

		public:

			// Childs
			std::vector<OOX::Drawing::CLineProperties*> m_arrLn;
		};

		//--------------------------------------------------------------------------------
		// CFontCollection 20.1.4.1.24;20.1.4.1.25 (Part 1)
		//--------------------------------------------------------------------------------	
		class CFontCollection : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFontCollection)
			CFontCollection()
			{
				m_eType = et_Unknown;
			}
			virtual ~CFontCollection()
			{
	            for ( unsigned int nIndex = 0; nIndex < m_arrFont.size(); nIndex++ )
				{
					if ( m_arrFont[nIndex] )
						delete m_arrFont[nIndex];

					m_arrFont[nIndex] = NULL;
				}

				m_arrFont.clear();
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				m_eType = et_Unknown;
				// TO DO: Реализовать CDefaultShapeDefinition::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				m_eType = et_Unknown;
                CString sName = oReader.GetName();
				if ( _T("a:majorFont") == sName )
					m_eType = et_a_majorFont;
				else if ( _T("a:minorFont") == sName )
					m_eType = et_a_minorFont;
				else 
					return;

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();
					if ( _T("a:cs") == sName )
						m_oCs = oReader;
					else if ( _T("a:ea") == sName )
						m_oEa = oReader;
					else if ( _T("a:extLst") == sName )
						m_oExtLst = oReader;
					else if ( _T("a:font") == sName )
					{
						OOX::Drawing::CSupplementalFont *oFont = new OOX::Drawing::CSupplementalFont(oReader);
						if (oFont) m_arrFont.push_back( oFont );
					}
					else if ( _T("a:latin") == sName )
						m_oLatin = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult;
				
				switch ( m_eType )
				{
				case et_a_majorFont: sResult = _T("<a:majorFont>"); break;
				case et_a_minorFont: sResult = _T("<a:minorFont>"); break;
				default: return _T("");
				}

				sResult += m_oLatin.toXML();
				sResult += m_oEa.toXML();
				sResult += m_oCs.toXML();

				for (unsigned int nIndex = 0; nIndex < m_arrFont.size(); nIndex++ )
				{
					if (m_arrFont[nIndex])
						sResult += m_arrFont[nIndex]->toXML();
				}

				if ( m_oExtLst.IsInit() )
					sResult += m_oExtLst->toXML();

				switch ( m_eType )
				{
				case et_a_majorFont: sResult += _T("</a:majorFont>"); break;
				case et_a_minorFont: sResult += _T("</a:minorFont>"); break;
				}

				return sResult;
			}
			virtual EElementType getType() const
			{
				return m_eType;
			}

		public:

			EElementType                                    m_eType;

			// Childs
			OOX::Drawing::CTextFont                         m_oCs;
			OOX::Drawing::CTextFont                         m_oEa;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
			std::vector<OOX::Drawing::CSupplementalFont*>	m_arrFont;
			OOX::Drawing::CTextFont                         m_oLatin;
		};


		//--------------------------------------------------------------------------------
		// CStyleMatrix 20.1.4.1.14 (Part 1)
		//--------------------------------------------------------------------------------	
		class CStyleMatrix : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CStyleMatrix)
			CStyleMatrix()
			{
			}
			virtual ~CStyleMatrix()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CStyleMatrix::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();
					if ( _T("a:bgFillStyleLst") == sName )
						m_oBgFillStyleLst = oReader;
					else if ( _T("a:effectStyleLst") == sName )
						m_oEffectStyleLst = oReader;
					else if ( _T("a:fillStyleLst") == sName )
						m_oFillStyleLst = oReader;
					else if ( _T("a:lnStyleLst") == sName )
						m_oLineStyleLst = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:fmtScheme name=\">") + m_sName + _T("\">"); 

				sResult += m_oFillStyleLst.toXML();
				sResult += m_oLineStyleLst.toXML();
				sResult += m_oEffectStyleLst.toXML();
				sResult += m_oBgFillStyleLst.toXML();

				sResult += _T("</a:fmtScheme>"); 

				return sResult;
			}
			virtual EElementType getType() const
			{
				return et_a_fmtScheme;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
								// Читаем атрибуты
				if ( oReader.GetAttributesCount() <= 0 )
					return;
				
				if ( !oReader.MoveToFirstAttribute() )
					return;
				
                CString wsName = oReader.GetName();
                while( !wsName.IsEmpty() )
				{
					if ( _T("name") == wsName ) 
						m_sName = oReader.GetText();

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}
				oReader.MoveToElement();
			}

		public:

			// Attributes
			CString                                m_sName;

			// Childs
			OOX::Drawing::CBackgroundFillStyleList m_oBgFillStyleLst;
			OOX::Drawing::CEffectStyleList         m_oEffectStyleLst;
			OOX::Drawing::CFillStyleList           m_oFillStyleLst;
			OOX::Drawing::CLineStyleList           m_oLineStyleLst;
		};


		//--------------------------------------------------------------------------------
		// CFontScheme 20.1.4.1.18 (Part 1)
		//--------------------------------------------------------------------------------	
		class CFontScheme : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFontScheme)
			CFontScheme()
			{
			}
			virtual ~CFontScheme()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать CFontScheme::fromXML(XmlUtils::CXmlNode& oNode)
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = oReader.GetName();
					if ( _T("a:extLst") == sName )
						m_oExtLst = oReader;
					else if ( _T("a:majorFont") == sName )
						m_oMajorFont = oReader;
					else if ( _T("a:minorFont") == sName )
						m_oMinorFont = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:fontScheme name=\">") + m_sName + _T("\">"); 

				sResult += m_oMajorFont.toXML();
				sResult += m_oMinorFont.toXML();

				if ( m_oExtLst.IsInit() )
					sResult += m_oExtLst->toXML();

				sResult += _T("</a:fontScheme>"); 

				return sResult;
			}
			virtual EElementType getType() const
			{
				return et_a_fontScheme;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
								// Читаем атрибуты
				if ( oReader.GetAttributesCount() <= 0 )
					return;
				
				if ( !oReader.MoveToFirstAttribute() )
					return;
				
                CString wsName = oReader.GetName();
                while( !wsName.IsEmpty() )
				{
					if ( _T("name") == wsName ) 
						m_sName = oReader.GetText();

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}
				oReader.MoveToElement();
			}

		public:

			// Attributes
			CString                                         m_sName;

			// Childs
			OOX::Drawing::CFontCollection                   m_oMajorFont;
			OOX::Drawing::CFontCollection                   m_oMinorFont;
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
		};


	} // namespace Drawing
} // namespace OOX

#endif // OOX_LOGIC_DRAWING_STYLES_INCLUDE_H_
