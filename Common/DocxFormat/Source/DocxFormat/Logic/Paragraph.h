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
#ifndef OOX_LOGIC_PARAGRAPH_INCLUDE_H_
#define OOX_LOGIC_PARAGRAPH_INCLUDE_H_

#include "../../Base/Nullable.h"
#include "../WritingElement.h"
#include "../../Common/SimpleTypes_Word.h"

#include "RunProperty.h"
#include "Run.h"
#include "Hyperlink.h"
#include "SmartTag.h"
#include "Dir.h"
#include "Bdo.h"
#include "../Math/oMathPara.h"

namespace OOX
{
	namespace Logic
	{
		class CParagraphProperty;
		//--------------------------------------------------------------------------------
		// CParagraph 17.3.1.22 (Part 1)
		//--------------------------------------------------------------------------------	
		class CParagraph : public WritingElementWithChilds<>
		{
		public:
			CParagraph()
			{
				m_oParagraphProperty = NULL;
			}
			CParagraph(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
			}
			CParagraph(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			virtual ~CParagraph()
			{
				m_oParagraphProperty = NULL;
			}

		public:

			const CParagraph &operator =(const XmlUtils::CXmlNode& oNode)
			{
				Clear();
				fromXML( (XmlUtils::CXmlNode&)oNode );
				return *this;
			}

			const CParagraph &operator =(const XmlUtils::CXmlLiteReader& oReader)
			{
				Clear();
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
				return *this;
			}

		public:

			void Clear()
			{
				m_oRsidDel.reset();
				m_oRsidP.reset();
				m_oRsidR.reset();
				m_oRsidRDefault.reset();
				m_oRsidRPr.reset();
				
				for (unsigned int nIndex = 0; nIndex < m_arrItems.size(); nIndex++ )
				{
					if ( m_arrItems[nIndex] )
						delete m_arrItems[nIndex];

					m_arrItems[nIndex] = NULL;
				}

				m_arrItems.clear();
				m_oParagraphProperty = NULL;
			}

			void AddRun(CRun *pRun);
			void AddText(CString& sText);
            void AddText(CString& sText, CRunProperty *pProperty);
			void AddTab();
			void AddTab(CRunProperty *pProperty);
			void AddBreak(SimpleTypes::EBrType eType);
			void AddSpace(const int nCount);
			void AddSpace(const int nCount, CRunProperty *pProperty);
			void AddBookmarkStart(int nId, CString& sName);
			void AddBookmarkEnd  (int nId);
		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode);
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual CString      toXML() const;
			virtual EElementType getType() const
			{
				return et_w_p;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CLongHexNumber<> > m_oParaId;
			nullable<SimpleTypes::CLongHexNumber<> > m_oTextId;

			// Attributes
			nullable<SimpleTypes::CLongHexNumber<> > m_oRsidDel;
			nullable<SimpleTypes::CLongHexNumber<> > m_oRsidP;
			nullable<SimpleTypes::CLongHexNumber<> > m_oRsidR;
			nullable<SimpleTypes::CLongHexNumber<> > m_oRsidRDefault;
			nullable<SimpleTypes::CLongHexNumber<> > m_oRsidRPr;

			// Childs
			CParagraphProperty *					 m_oParagraphProperty; // копия того что в m_arrItems...  - для быстрого доступа/анализа
			// по идее нужно сделать как в Drawing::Paragraph - то есть единственные подобъекты вынести отдельно
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_PARAGRAPH_INCLUDE_H_
