/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
