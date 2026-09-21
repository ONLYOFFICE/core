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

#include "./SpTree.h"
#include "CNvPr.h"
#include "NvPr.h"

namespace PPTX
{
	namespace Logic
	{
		class CNvContentPartPr : public WrapperWritingElement
		{
		public:

			WritingElement_AdditionMethods(CNvContentPartPr)

			CNvContentPartPr(std::wstring ns = L"p");
			CNvContentPartPr& operator=(const CNvContentPartPr& oSrc);

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributesLocks(XmlUtils::CXmlLiteReader& oReader);

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			void toXmlWriter2(const std::wstring& strNS, NSBinPptxRW::CXmlWriter* pWriter) const;
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			std::wstring m_namespace;

			nullable_bool isComment;
			nullable_bool noChangeAspect;
			nullable_bool noGrp;
			nullable_bool noMove;
			nullable_bool noResize;
			nullable_bool noRot;
			nullable_bool noSelect;
			nullable_bool noEditPoints;
			nullable_bool noAdjustHandles;
			nullable_bool noChangeArrowheads;
			nullable_bool noChangeShapeType;
		protected:
			virtual void FillParentPointersForChilds();
		};
		class NvContentPartPr : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionMethods(NvContentPartPr)
			NvContentPartPr(std::wstring ns = L"p");

			NvContentPartPr& operator=(const NvContentPartPr& oSrc);

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void fromXML(XmlUtils::CXmlNode& node);

			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			void toXmlWriter2(const std::wstring& strNS, bool bWriteNvPr, NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			std::wstring m_namespace;

			CNvPr cNvPr;
			CNvContentPartPr cNvContentPartPr;
			NvPr nvPr;
		protected:
			virtual void FillParentPointersForChilds();
		};
		class ContentPart : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionMethods(ContentPart)

			ContentPart();
			ContentPart& operator=(const ContentPart& oSrc);

			virtual OOX::EElementType getType () const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void fromXML(XmlUtils::CXmlNode& node);

			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			std::wstring m_namespace;
			nullable<PPTX::Logic::Xfrm> xfrm;
			nullable<PPTX::Logic::NvPr> nvPr;
			nullable<PPTX::Logic::NvContentPartPr> nvContentPartPr;
			nullable<OOX::RId> id;

		protected:
			virtual void FillParentPointersForChilds();

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
	} // namespace Logic
} // namespace PPTX
