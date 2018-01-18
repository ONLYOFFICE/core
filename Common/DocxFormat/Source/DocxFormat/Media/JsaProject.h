/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
#ifndef OOX_ONLY_JSA_PROJECT_INCLUDE_H_
#define OOX_ONLY_JSA_PROJECT_INCLUDE_H_

#include "Media.h"
#include "../../XlsxFormat/FileTypes_Spreadsheet.h"

namespace OOX
{
	class JsaProject : public Media
	{
	public:
		JsaProject( OOX::Document *pMain ) : Media(pMain)
		{
		}
		JsaProject(OOX::Document *pMain, const CPath& filename) : Media(pMain)
		{
			read(filename);
		}
		virtual ~JsaProject()
		{
		}
		virtual const FileType type() const
		{
			return FileTypes::JsaProject;
		}
		virtual const CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}
		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			BYTE* pData = NULL;
			DWORD nBytesCount;
			if(NSFile::CFileBinary::ReadAllBytes(m_filename.GetPath(), &pData, nBytesCount))
			{
				pWriter->WriteBYTEArray(pData, nBytesCount);
			}
		}
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _length = pReader->GetLong();
			LONG _end_rec = pReader->GetPos() + _length;

			if (_length > 0)
			{
				BYTE* pData = pReader->GetPointer(_length);
				std::wstring filePath = pReader->m_pRels->m_pManager->GetDstFolder() + FILE_SEPARATOR_STR + OOX::FileTypes::JsaProject.DefaultFileName().GetPath();

				NSFile::CFileBinary oFile;
				oFile.CreateFileW(filePath);
				oFile.WriteFile(pData, _length);
				oFile.CloseFile();

				pReader->m_pRels->m_pManager->m_pContentTypes->AddDefault(OOX::FileTypes::JsaProject.DefaultFileName().GetExtention(false));

				set_filename(filePath, false);
			}
			pReader->Seek(_end_rec);
		}
	};
} // namespace OOX

#endif // OOX_ONLY_JSA_PROJECT_INCLUDE_H_
