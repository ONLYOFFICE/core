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
