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

#include "OleObject.h"
#include "Image.h"

namespace OOX
{
	OleObject::OleObject(OOX::Document *pMain, bool bMsPackage, bool bDocument) : OOX::IFileContainer(pMain), Media (pMain, bDocument)
	{
		m_bMsPackage = bMsPackage;
	}
	OleObject::OleObject(OOX::Document *pMain, const OOX::CPath& filename, bool bMsPackage) : OOX::IFileContainer(pMain), Media (pMain)
	{
		m_bMsPackage = bMsPackage;
		read(filename);
	}
	void OleObject::read(const CPath& oFilePath)
	{
		CPath oRootPath;
		read(oRootPath, oFilePath);
	}
	void OleObject::read(const CPath& oRootPath, const CPath& oFilePath)
	{
		IFileContainer::Read(oRootPath, oFilePath);
		
		Media::read(oFilePath);

		smart_ptr<OOX::File> pFile = this->Find(OOX::FileTypes::Image);
		Image* pImage = dynamic_cast<Image*>(pFile.GetPointer());
		if (pImage)
		{
			m_filenameCache = pImage->filename();
		}
	}
	void OleObject::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		Media::write(oFilePath, oDirectory, oContent);
		IFileContainer::Write(oFilePath, oDirectory, oContent);
	}
	const FileType OleObject::type() const
	{
		if (m_bMsPackage)	return OOX::FileTypes::MicrosoftOfficeUnknown;
		else				return OOX::FileTypes::OleObject;
	}
	const CPath OleObject::DefaultDirectory() const
	{
		if (m_bDocument) return type().DefaultDirectory();
		else	return L"../" + type().DefaultDirectory();
	}
	const CPath OleObject::DefaultFileName() const
	{
		return m_filename.GetFilename();
	}
	void OleObject::set_filename_cache(const std::wstring & file_path)
	{
		m_filenameCache = file_path;
	}
	void OleObject::set_filename_cache(CPath & file_path)
	{
		m_filenameCache = file_path;
	}
	void OleObject::set_MsPackage(bool val)
	{
		m_bMsPackage = val;
	}
	CPath OleObject::filename_cache()
	{
		return m_filenameCache;
	}
	bool OleObject::isMsPackage()
	{
		return m_bMsPackage;
	}

} // namespace OOX
