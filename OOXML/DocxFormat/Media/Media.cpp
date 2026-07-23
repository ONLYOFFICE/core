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

#include "Media.h"
#include "../Document.h"

namespace OOX
{
	Media::Media(OOX::Document *pMain, bool bDocument) : File(pMain)
	{
		m_bExist = false;
		m_bExternal = false;
		m_bDocument = bDocument;
	}
	Media::Media(OOX::Document *pMain, const CPath& filename, bool bExternal) : File(pMain)
	{
		m_bExist = false;
		m_bDocument = (NULL != dynamic_cast<OOX::CDocument*>(pMain));
		m_bExternal = bExternal;

		read(filename);
	}
	Media::~Media()
	{
	}
	const FileType Media::type() const
	{
		return FileTypes::Media;
	}
	bool Media::IsExist()
	{
		return m_bExist;
	}
	bool Media::IsExternal()
	{
		return m_bExternal;
	}
	CPath Media::filename()
	{
		return m_filename;
	}
	const CPath Media::DefaultDirectory() const
	{
		if (m_bDocument) return type().DefaultDirectory();
		else	return L"../" + type().DefaultDirectory();
	}
	const CPath Media::DefaultFileName() const
	{
		return m_filename.GetFilename();
	}
	void Media::read(const CPath& filename)
	{
		m_filename = filename;
		m_bExist = NSFile::CFileBinary::Exists(m_filename.GetPath());

		if (m_bExist)
			m_sOutputFilename = filename.GetFilename();
	}
	void Media::write(const CPath& filename, const CPath& directory, CContentTypes& content) const
	{
		if (m_bExternal) return;

		std::wstring newFilename = filename.GetFilename();
		CPath newFilePath = filename.GetDirectory();
		XmlUtils::replace_all(newFilename, L" ", L"_");

		if (!CSystemUtility::IsFileExist(newFilePath / newFilename))
		{
			if (false == m_Data.empty())
			{
				NSFile::CFileBinary file;
				if (file.CreateFileW((newFilePath / newFilename).GetPath()))
				{
					file.WriteFile(m_Data.data(), m_Data.size());
					file.CloseFile();
				}
			}
			else if (CSystemUtility::IsFileExist(m_filename))
			{
				copy_to(newFilePath);
			}
		}
		std::wstring ext = filename.GetExtention();
		if (false == ext.empty()) content.AddDefault(ext.substr(1));
	}
	void Media::set_filename(const std::wstring & file_path, bool bExternal)
	{
		read(OOX::CPath(file_path, !bExternal));
			
		m_bExternal = bExternal;
		m_sOutputFilename = m_filename.GetFilename();
	}
	void Media::set_filename(CPath & file_path, bool bExternal, bool bDefault)
	{
		m_bExternal = bExternal;
		m_filename = file_path;
		
		if (!bDefault) m_sOutputFilename = m_filename.GetFilename();
		
		m_bExist = NSFile::CFileBinary::Exists(m_filename.GetPath());
	}
	void Media::copy_to(const CPath& path) const
	{
		if (m_bExternal) return;

		OOX::CPath pathSaveItem =  path + FILE_SEPARATOR_STR + m_filename.GetFilename();
            
		NSFile::CFileBinary::Copy(m_filename.GetPath(), pathSaveItem.GetPath());
	}

} // namespace OOX

