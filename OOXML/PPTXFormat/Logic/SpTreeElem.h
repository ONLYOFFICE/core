/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#pragma once

#include "../WrapperWritingElement.h"
#include "../Theme.h"

#include "SpPr.h"
#include "ShapeStyle.h"

#include <boost/smart_ptr/shared_array.hpp>

namespace OOX
{
	namespace Logic
	{
		class CBinData;
	}
}
namespace PPTX
{
	namespace Logic
	{
        void CalculateFill(BYTE lDocType, PPTX::Logic::SpPr& oSpPr, nullable<ShapeStyle>& pShapeStyle, smart_ptr<PPTX::Theme>& oTheme,
				smart_ptr<PPTX::Logic::ClrMap>& oClrMap, OOX::IFileContainer* pContainer, std::wstring& strAttr, std::wstring& strNode, bool bOle = false, bool bSignature = false);

        void CalculateLine(BYTE lDocType, PPTX::Logic::SpPr& oSpPr, nullable<ShapeStyle>& pShapeStyle,
				smart_ptr<PPTX::Theme>& oTheme, smart_ptr<PPTX::Logic::ClrMap>& oClrMap, std::wstring& strAttr, std::wstring& strNode, bool bOle = false);

		class SpTreeElem : public WrapperWritingElement
		{
		public:
			SpTreeElem();
			virtual ~SpTreeElem();

			virtual OOX::EElementType getType () const;

			explicit SpTreeElem(XmlUtils::CXmlNode& node);
			const SpTreeElem& operator =(XmlUtils::CXmlNode& node);

			explicit SpTreeElem(XmlUtils::CXmlLiteReader& oReader);
			const SpTreeElem& operator =(XmlUtils::CXmlLiteReader& oReader);

			SpTreeElem& operator=(const SpTreeElem& oSrc);

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributesRequires(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
            virtual bool is_init() const {return (m_elem.IsInit());}

			template<class T> AVSINLINE const bool	is() const	{ return m_elem.is<T>(); }
			template<class T> AVSINLINE T&			as()		{ return m_elem.as<T>(); }
			template<class T> AVSINLINE const T&	as() const 	{ return m_elem.as<T>(); }

			void InitElem(WrapperWritingElement* pElem);

			virtual void fromPPTY (NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY (NSBinPptxRW::CBinaryFileWriter* pWriter) const;

			void toXmlWriterVML (NSBinPptxRW::CXmlWriter* pWriter, smart_ptr<PPTX::Theme>& oTheme, smart_ptr<PPTX::Logic::ClrMap>& oClrMap, OOX::IFileContainer* pContainer) const;
			virtual void toXmlWriter (NSBinPptxRW::CXmlWriter* pWriter) const;

			std::wstring GetUriElem();			
			smart_ptr<WrapperWritingElement> GetElem();
			smart_ptr<WrapperWritingElement> GetElemAlternative();

			virtual void SetParentPointer(const WrapperWritingElement* pParent);
			std::wstring GetRequires();

			nullable<OOX::Logic::CBinData> m_binaryData;
			std::wstring m_sRequires;//from mc:Choice
			bool m_bAlternative = false;

		private:
			smart_ptr<WrapperWritingElement> m_elem;
			smart_ptr<WrapperWritingElement> m_elem_alternative;

		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX
