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

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		public:
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

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;

		public:
			nullable_string				m_strVersion;
			nullable_string				m_strContent;
		protected:
			virtual void FillParentPointersForChilds(){};
		};

		class CVariant : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(CVariant)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			void toXmlWriterContent(NSBinPptxRW::CXmlWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			eVariantType getVariantType() const;
			static eVariantType getTypeByString(const std::wstring& sName);
			static std::wstring getStringByType(const eVariantType& eType);
		public:
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

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			eVariantType getVariantType() const;
		public:
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

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			eVariantType getVariantType() const;
		public:
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

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;

		public:
			nullable_string							m_strFmtid;
			nullable_string							m_strLinkTarget;
			nullable_string							m_strName;
			nullable_int							m_nPid;

			nullable<CVariant>						m_oContent;
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
