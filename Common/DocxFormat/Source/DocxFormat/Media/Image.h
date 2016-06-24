/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#ifndef OOX_IMAGE_INCLUDE_H_
#define OOX_IMAGE_INCLUDE_H_

#include "Media.h"

namespace OOX
{
	class Image : public Media
	{
	public:
		Image()
		{
		}
		Image(const CPath& filename)
		{
			read(filename);
		}
		virtual ~Image()
		{
		}

	public:
		virtual void write(const CPath& filename, const CPath& directory, CContentTypes& content) const
		{
			CString newFilename = filename.GetFilename();
			CPath newFilePath = filename.GetDirectory();

			newFilename.Replace((TCHAR)' ', (TCHAR)'_');
			if (CSystemUtility::IsFileExist(m_filename) && !CSystemUtility::IsFileExist(newFilePath/newFilename))	
			{
				//if (m_filename.GetExtention(true) == _T(".svm"))
				//{
				//	ConvertSvmToImage(m_filename, replace_extension(newFilePath/newFilename, L"png"));
				//}
				//else if (m_filename.extension() != L"")
				//{
				//	boost::filesystem::copy_file(m_filename, newFilePath/newFilename);
				//}
				//else
				//{
				//	newFilename += L".wmf";
				//	boost::filesystem::copy_file(m_filename, newFilePath/newFilename);
				//}
			}
			//if (m_filename.extension() == L".svm")
			//	content.Default->add(replace_extension(newFilePath/newFilename, L"png"));
			//else
			//	content.Default->add(newFilePath/newFilename);
		}

	public:
		virtual const FileType type() const
		{
			return FileTypes::Image;
		}
		virtual const CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const CPath DefaultFileName() const
		{
			return m_filename.GetFilename();
		}
	};
} // namespace OOX

#endif // OOX_IMAGE_INCLUDE_H_