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
#ifndef OOX_LOGIC_DRAWING_BODY_INCLUDE_H_
#define OOX_LOGIC_DRAWING_BODY_INCLUDE_H_

#include "../../Base/Nullable.h"
#include "../../Common/SimpleTypes_Drawing.h"
#include "../../Common/SimpleTypes_Shared.h"

#include "../WritingElement.h"

#include "DrawingTextProperties.h"
#include "DrawingParagraph.h"

namespace OOX
{
	namespace Drawing
	{
		//--------------------------------------------------------------------------------
		// 20.1.2.2.40 txBody (Shape Text Body)
		//--------------------------------------------------------------------------------	
		class CTxBody : public WritingElementWithChilds<Drawing::CParagraph>
		{
		public:
			WritingElement_AdditionConstructors(CTxBody)
			CTxBody()
			{
			}
			virtual ~CTxBody()
			{
			}
			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
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

					if ( _T("a:bodyPr") == sName )
						m_oBodyPr = oReader;
					//if ( _T("a:lstStyle") == sName )
					//	m_oLstStyle = oReader;
					else if ( _T("a:p") == sName )
						m_arrItems.push_back( new Drawing::CParagraph( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_a_TextShapeBody;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start	( oReader )
				WritingElement_ReadAttributes_End	( oReader )
			}
		public:
			nullable<OOX::Drawing::CTextBodyProperties>	m_oBodyPr;
			//nullable<OOX::Drawing::CTextListStyle>	m_oLstStyle;
		};
		//--------------------------------------------------------------------------------
		// 20.1.2.2.41 txSp (Text Shape)
		//--------------------------------------------------------------------------------	
		class CTxSp : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTxSp)
			CTxSp()
			{
			}
			virtual ~CTxSp()
			{
			}
			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать fromXML(XmlUtils::CXmlNode& oNode)
			}
		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
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

					if ( _T("a:txBody") == sName )
						m_oTxBody = oReader;
					//if ( _T("a:lstStyle") == sName )
					//	m_oLstStyle = oReader;

				}
			}

			virtual EElementType getType () const
			{
				return et_a_TextShape;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start	( oReader )
				WritingElement_ReadAttributes_End	( oReader )
			}
		public:
			nullable<OOX::Drawing::CTxBody>		m_oTxBody;
//extLst (Extension List) І20.1.2.2.15
//useSpRect (Use Shape Text Rectangle) І20.1.2.2.42
//xfrm (2D Transform for Individual Objects) І20.1.7.6
		};
	} // namespace Drawing
} // namespace OOX

#endif // OOX_LOGIC_DRAWING_BODY_INCLUDE_H_
