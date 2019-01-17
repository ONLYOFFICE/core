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
#ifndef OOX_MEDIA_INCLUDE_H_
#define OOX_MEDIA_INCLUDE_H_

#include "../File.h"
#include "../FileTypes.h"


namespace OOX
{
	class Media : public File
	{
	public:
		Media(OOX::Document *pMain, bool bDocument = true) : File(pMain)
		{
			m_bExist	= false;
			m_bExternal = false;
			m_bDocument = bDocument;
		}
		Media(OOX::Document *pMain, const CPath& filename, bool bExternal = false) : File(pMain)
		{
			m_bExternal	= bExternal;			
			read(filename);
		}
		virtual ~Media()
		{
		}
		virtual const FileType type() const
		{
			return FileTypes::Media;
		}
		virtual void read(const CPath& filename)
		{
			m_filename = filename;
			m_bExist = NSFile::CFileBinary::Exists(m_filename.GetPath());
		}
		virtual void write(const CPath& filename, const CPath& directory, CContentTypes& content) const
		{
		}
		void set_filename(const std::wstring & file_path, bool bExternal)
		{
			read(file_path);
			
			m_bExternal			= bExternal;
			m_sOutputFilename	= m_filename.GetFilename();
		}
		void set_filename(CPath & file_path, bool bExternal)
		{
			m_bExternal			= bExternal;
			m_filename			= file_path;
			m_sOutputFilename	= file_path.GetFilename();
			m_bExist = NSFile::CFileBinary::Exists(m_filename.GetPath());
		}
		bool IsExist()
		{
			return m_bExist;
		}
		bool IsExternal()
		{
			return m_bExternal;
		}
		CPath filename()
		{
			return m_filename;
		}
		virtual void copy_to(const CPath& path) const
		{
			if (m_bExternal) return;

			OOX::CPath pathSaveItem =  path + FILE_SEPARATOR_STR + m_filename.GetFilename();
            
			NSFile::CFileBinary::Copy(m_filename.GetPath(), pathSaveItem.GetPath());
		}
		virtual const CPath DefaultDirectory() const
		{
			if (m_bDocument) return type().DefaultDirectory();
			else	return L"../" + type().DefaultDirectory();
		}
		virtual const CPath DefaultFileName() const
		{
			return m_filename.GetFilename();
		}
	protected:
		CPath	m_filename;
		bool	m_bExist;
		bool	m_bExternal;
		bool	m_bDocument; //for upper/lower level rels (defaultDirectory)
	};
} // namespace OOX

#endif // OOX_MEDIA_INCLUDE_H_
