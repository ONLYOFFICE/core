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
			std::wstring ext = filename.GetExtention();
			if (false == ext.empty()) content.AddDefault(ext.substr(1));
			
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
	}
	void Media::set_filename(const std::wstring & file_path, bool bExternal)
	{
		read(file_path);
			
		m_bExternal			= bExternal;
		m_sOutputFilename	= m_filename.GetFilename();
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

