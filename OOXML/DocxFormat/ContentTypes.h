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

#include "../SystemUtility/SystemUtility.h"

#include "WritingElement.h"
#include <unordered_map>
#include <map>

namespace OOX
{
	namespace ContentTypes
	{
		class CExtensionTable
		{
		public:
			CExtensionTable();

			const std::wstring operator[] (const std::wstring& sExtension) const;

		private:
            std::map <std::wstring, std::wstring> m_mTable;
		};

		class CDefault : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CDefault)
			CDefault();
			CDefault(const std::wstring& sExtension);
			virtual ~CDefault();

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

            std::wstring m_sExtension;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};

		class COverride : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(COverride)
			COverride();
			COverride(const std::wstring& sType, const CPath& oPath);
			virtual ~COverride();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			const std::wstring type() const;
			const OOX::CPath filename() const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
          
			std::wstring	m_sType;
			OOX::CPath		m_oPart;
		};
	} // namespace ContentTypes

    static const CPath c_oContentTypeFileName (L"[Content_Types].xml");

	class CContentTypes
	{
	public:
		CContentTypes();
		CContentTypes(const CPath& oPath);
		~CContentTypes();

		bool Read (const CPath& oDirPath);
		bool ReadFromString (std::wstring& sXml);

		void Merge(CContentTypes * pSrcContentTypes);
		bool Write(const CPath& oDirPath) const;

		void Registration(const std::wstring& sType, const CPath& oDirectory, const CPath& oFilename);
		void Registration(const std::wstring& sType, const std::wstring& sDirectory, const std::wstring& sFilename);

		void AddDefault(const std::wstring& sExtension);

	private:
		bool ReadFromReader (XmlUtils::CXmlLiteReader& oReader);
		void AddOverride(const std::wstring& sType, std::wstring sPath);

	public:
        std::unordered_map<std::wstring, ContentTypes::CDefault> m_mapDefaults;
        std::unordered_map<std::wstring, smart_ptr<ContentTypes::COverride>> m_mapOverrides;
		
		std::multimap<std::wstring, smart_ptr<ContentTypes::COverride>> m_mapOverridesByType;
	};

} // namespace OOX
