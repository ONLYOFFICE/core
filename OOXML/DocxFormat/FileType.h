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

namespace OOX
{
	class FileType
	{
	public:
		FileType(const CPath& defaultDirectory, const CPath& defaultFileName,
							const std::wstring& overrideType, 
							const std::wstring& relationType,
							const std::wstring& enumerateType = L"", bool bEnumerated = false, bool bEnumeratedGlobal = false );


		FileType(const std::wstring& defaultDirectory, const std::wstring& defaultFileName,
							const std::wstring& overrideType, 
							const std::wstring& relationType,
							const std::wstring& enumerateType = L"", bool bEnumerated = false, bool bEnumeratedGlobal = false );

		~FileType();

		const bool operator ==(const FileType& rhs) const;

		inline const std::wstring OverrideType() const
		{
			return m_overrideType;
		}
		inline const std::wstring EnumerateType() const
		{
			return m_enumerateType;
		}
		inline const std::wstring RelationType() const
		{
			return m_relationType;
		}
		inline const CPath DefaultDirectory() const
		{
			return m_defaultDirectory;
		}
		inline const CPath DefaultFileName() const
		{
			return m_defaultFileName;
		}
		inline const bool Enumerated() const
		{
			return m_bEnumerated;
		}
		inline const bool EnumeratedGlobal() const
		{
			return m_bEnumeratedGlobal;
		}

	private:
		std::wstring		m_overrideType;
		std::wstring		m_relationType;
		std::wstring		m_enumerateType;
		CPath				m_defaultDirectory;
		CPath				m_defaultFileName;
		bool				m_bEnumerated;
		bool				m_bEnumeratedGlobal;
	};

	static const bool operator ==(const std::wstring& type, const FileType& file)
	{
		//RelationType
		//http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument
		//http://purl.oclc.org/ooxml/officeDocument/relationships/officeDocument
		//is valid and equal so compare tail
        int nIndexType = (int)type.rfind('/');
		std::wstring relationType = file.RelationType();
        int nIndexFile = (int)relationType.rfind('/');
        
		std::wstring tempType = (-1 == nIndexType) ? type : type.substr(nIndexType);
        std::wstring tempFile = (-1 == nIndexFile) ? relationType : relationType.substr(nIndexFile);
		return (tempType == tempFile);
	}
	static const bool operator ==(const FileType& file, const std::wstring& type)
	{
		return type == file;
	}
} // namespace OOX
