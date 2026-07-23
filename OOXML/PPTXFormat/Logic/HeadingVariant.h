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

#include "./../WrapperFile.h"
#include "./../FileContainer.h"
#include "./../WrapperWritingElement.h"
#include "./../Limit/VariantType.h"

namespace PPTX
{
	namespace Logic
	{
		class CVariantVector;
		class CVariantArray;

		enum eVariantType {vtEmpty = 0, vtNull, vtVariant, vtVector, vtArray, vtVStream, vtBlob, vtOBlob, vtI1, vtI2,
		vtI4, vtI8, vtInt, vtUi1, vtUi2, vtUi4, vtUi8, vtUint, vtR4, vtR8, vtDecimal, vtLpstr, vtLpwstr, vtBstr,
		vtDate, vtFiletime, vtBool, vtCy, vtError, vtStream, vtOStream, vtStorage, vtOStorage, vtClsid};

		class HeadingVariant : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(HeadingVariant)

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			nullable_limit<Limit::VariantType>		m_type;
			nullable_string							m_strContent;
			nullable_int							m_iContent;

		protected:
			virtual void FillParentPointersForChilds();
		};

		class CVariantVStream : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(CVariantVStream)

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;

			nullable_string				m_strVersion;
			nullable_string				m_strContent;
		protected:
			virtual void FillParentPointersForChilds(){};
		};

		class CVariant : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(CVariant)

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			void toXmlWriterContent(NSBinPptxRW::CXmlWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			eVariantType getVariantType() const;
			static eVariantType getTypeByString(const std::wstring& sName);
			static std::wstring getStringByType(const eVariantType& eType);

			nullable<eVariantType>					m_eType;
			nullable_string							m_strContent;
			nullable_int							m_iContent;
			nullable_uint							m_uContent;
			nullable_double							m_dContent;
			nullable_bool							m_bContent;
			nullable<CVariant>						m_oVariant;
			nullable<CVariantVector>				m_oVector;
			nullable<CVariantArray>					m_oArray;
			nullable<CVariantVStream>				m_oVStream;
		protected:
			virtual void FillParentPointersForChilds(){};
		};

		class CVariantVector : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(CVariantVector)

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			eVariantType getVariantType() const;

			nullable<eVariantType>					m_eBaseType;
			nullable_int							m_nSize;

			std::vector<CVariant>					arrVariants;
		protected:
			virtual void FillParentPointersForChilds(){};
		};

		class CVariantArray : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(CVariantArray)

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			eVariantType getVariantType() const;
			
			nullable<eVariantType>					m_eBaseType;
			nullable_string							m_strLBounds;
			nullable_string							m_strUBounds;

			std::vector<CVariant>					arrVariants;
		protected:
			virtual void FillParentPointersForChilds(){};
		};

		class CustomProperty : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(CustomProperty)

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;

			nullable_string m_strFmtid;
			nullable_string m_strLinkTarget;
			nullable_string m_strName;
			nullable_int m_nPid;

			nullable<CVariant> m_oContent;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

namespace PPTX
{
	class CustomProperties : public WrapperFile
	{
	public:
		CustomProperties(OOX::Document* pMain);
		CustomProperties(OOX::Document* pMain, const OOX::CPath& filename, FileMap& map);
		virtual ~CustomProperties();
		virtual void read(const OOX::CPath& filename, FileMap& map);
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::CContentTypes& content)const;
		virtual const OOX::FileType type() const;
		virtual const OOX::CPath DefaultDirectory() const;
		virtual const OOX::CPath DefaultFileName() const;
		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

		std::vector<PPTX::Logic::CustomProperty>	m_arProperties;
	};
} // namespace PPTX
