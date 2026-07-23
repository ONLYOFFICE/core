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

#include "../WritingElement.h"
#include "../../Base/Nullable.h"
#include "../FileTypes_Spreadsheet.h"
#include "../../DocxFormat/IFileContainer.h"

namespace SimpleTypes
{
	class CUnsignedDecimalNumber;
}

namespace OOX
{
	namespace Drawing
	{
		class COfficeArtExtensionList;
	}

	namespace Spreadsheet
	{
		class CPivotCacheRecord : public WritingElementWithChilds<WritingElement>
		{
		public:
			WritingElement_AdditionMethods(CPivotCacheRecord)
            WritingElement_XlsbConstructors(CPivotCacheRecord)
			CPivotCacheRecord(){}
			virtual ~CPivotCacheRecord() {}

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
            void fromBin(XLS::StreamCacheReaderPtr& reader);
			XLS::BaseObjectPtr toBin();
			XLS::BaseObjectPtr toXLS();
            void toBin(XLS::StreamCacheWriterPtr& writer);
			virtual EElementType getType () const
			{
				return et_x_PivotCacheRecord;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader){}
		};
		class CPivotCacheRecords : public WritingElementWithChilds<CPivotCacheRecord>
		{
		public:
			WritingElement_AdditionMethods(CPivotCacheRecords)
            WritingElement_XlsbConstructors(CPivotCacheRecords)
			CPivotCacheRecords()
			{
			}
			virtual ~CPivotCacheRecords()
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
            void fromBin(XLS::StreamCacheReaderPtr& reader);
			XLS::BaseObjectPtr toBin();
            void toBin(XLS::StreamCacheWriterPtr& writer);
			virtual EElementType getType () const
			{
				return et_x_PivotCacheRecords;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oCount;
			nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;

			std::wstring m_strOutputXml;
		};

		class CPivotCacheRecordsFile : public OOX::FileGlobalEnumerated, public OOX::IFileContainer
		{
		public:
			CPivotCacheRecordsFile(OOX::Document* pMain) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
			{
				m_bSpreadsheets = true;

				m_pData = NULL;
				m_nDataLength = 0;
			}
			CPivotCacheRecordsFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
			{
				m_bSpreadsheets = true;

				m_pData = NULL;
				m_nDataLength = 0;

				read( oRootPath, oPath );
			}
			virtual ~CPivotCacheRecordsFile()
			{
				m_nDataLength = 0;
				RELEASEARRAYOBJECTS(m_pData)
            }
			virtual void read(const CPath& oPath)
			{
				//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
				CPath oRootPath;
				read(oRootPath, oPath);
			}
           void setData(BYTE* pData, long length)
            {
				m_nDataLength = length;
				m_pData = new BYTE[length];
				memcpy(m_pData, pData, length);
            }
            void readBin(const CPath& oPath);
			XLS::BaseObjectPtr WriteBin() const;
            void WriteBin(XLS::StreamCacheWriterPtr& writer) const;
			virtual void read(const CPath& oRootPath, const CPath& oPath);
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;
			virtual const OOX::FileType type() const;
			virtual const CPath DefaultDirectory() const
			{
				return type().DefaultDirectory();
			}
			virtual const CPath DefaultFileName() const
			{
				return type().DefaultFileName();
			}
			const CPath& GetReadPath()
			{
				return m_oReadPath;
			}
//---------------------------------------------------------------------
			nullable<CPivotCacheRecords> m_oPivotCacheRecords;
	//---------
			BYTE *m_pData = NULL;
			DWORD m_nDataLength = 0;
		private:
			CPath m_oReadPath;
		};
	} //Spreadsheet
} // namespace OOX
