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
#ifndef PPTXOOX_MEDIA_INCLUDE_H_
#define PPTXOOX_MEDIA_INCLUDE_H_

#include "../File.h"
#include "../FileTypes.h"

#ifdef AVS_OFFICE_SVM_FILE
#include "OfficeSvmFile.h"
#include "SvmConverter.h"
#endif


namespace PPTX
{
	class Media : public File
	{
	public:
		Media()
		{
		}
		Media(const OOX::CPath& filename)
		{
			read(filename);
		}
		virtual ~Media()
		{
		}

	public:
		virtual void read(const OOX::CPath& filename)
		{
			m_filename = filename;
		}
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const
		{
			//std::wstring newFilename = filename.filename();
			//boost::filesystem::wpath newFilePath = filename.parent_path();
			//boost::replace_all(newFilename, L" ", L"_");
			//if (boost::filesystem::exists(m_filename) && !boost::filesystem::exists(newFilePath/newFilename))	
			//{
			//	if (m_filename.extension() == L".svm")
			//		ConvertSvmToImage(m_filename, replace_extension(newFilePath/newFilename, L"png"));
			//	else
			//		boost::filesystem::copy_file(m_filename, newFilePath/newFilename);
			//}
			//if (m_filename.extension() == L".svm")
			//	content.Default->add(replace_extension(newFilePath/newFilename, L"png"));
			//else
			//	content.Default->add(m_filename);
		}

	public:
		const OOX::CPath filename() const
		{
			return m_filename;
		}
		void copy_to(const OOX::CPath& path) const
		{
			//std::wstring newFilename = m_filename.filename();
			//boost::replace_all(newFilename, L" ", L"_");

			//if (boost::filesystem::exists(m_filename) && !boost::filesystem::exists(path/m_filename.filename()))
			//	boost::filesystem::copy_file(m_filename, path/newFilename);
		}

	protected:
		OOX::CPath	m_filename;
	};
} // namespace PPTX

#endif // PPTXOOX_MEDIA_INCLUDE_H_
