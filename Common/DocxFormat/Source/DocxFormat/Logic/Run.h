﻿/*
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
#pragma once

#include "RunProperty.h"
#include "RunContent.h"
#include "FldChar.h"
#include "FldSimple.h"

#include "../Drawing/Drawing.h"
#include "../Comments.h"
#include "Pict.h"
#include "Annotations.h"
#include "AlternateContent.h"
#include "../../../../../DesktopEditor/common/StringExt.h"

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// CRun 17.3.2.25 (Part 1)
		//--------------------------------------------------------------------------------	
		class CRun : public WritingElementWithChilds<>
		{
		public:
			CRun(OOX::Document *pMain = NULL) : WritingElementWithChilds<>(pMain) 
			{
				m_oRunProperty = NULL;
			}
			CRun(XmlUtils::CXmlNode &oNode) : WritingElementWithChilds<>(NULL)
			{
				fromXML( oNode );
			}
			CRun(XmlUtils::CXmlLiteReader& oReader) : WritingElementWithChilds<>(NULL)
			{
				fromXML( oReader );
			}
			virtual ~CRun()
			{
				ClearItems();
			}
			const CRun &operator =(const XmlUtils::CXmlNode& oNode)
			{
				ClearItems();
				
				fromXML( (XmlUtils::CXmlNode&)oNode );
				return *this;
			}
			const CRun &operator =(const XmlUtils::CXmlLiteReader& oReader)
			{
				ClearItems();
				
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
				return *this;
			}

			virtual void ClearItems();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;

			void fromXMLElems(XmlUtils::CXmlLiteReader& oReader);
			WritingElement* fromXMLElem(XmlUtils::CXmlLiteReader& oReader);
			void fromXMLText(XmlUtils::CXmlLiteReader& oReader);
			
			virtual EElementType getType() const
			{
				return et_w_r;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable<SimpleTypes::CLongHexNumber<> > m_oRsidDel;
			nullable<SimpleTypes::CLongHexNumber<> > m_oRsidR;
			nullable<SimpleTypes::CLongHexNumber<> > m_oRsidRPr;
			OOX::Logic::CRunProperty				*m_oRunProperty;	// копия того что в m_arrItems...  - для быстрого доступа/анализа
			// по идее нужно сделать как в Drawing::Run - то есть единственные подобъекты вынести отдельно
		};
	} // namespace Logic
} // namespace OOX
