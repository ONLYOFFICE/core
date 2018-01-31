﻿/*
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
#ifndef OOX_OLE_OBJECT_INCLUDE_H_
#define OOX_OLE_OBJECT_INCLUDE_H_

#include "Media.h"
#include "../../XlsxFormat/FileTypes_Spreadsheet.h"

namespace OOX
{
	class OleObject : public Media
	{
	public:
		OleObject(OOX::Document *pMain, bool bMsPackage = false, bool bDocument = true) : Media (pMain, bDocument)
		{
			m_bMsPackage = bMsPackage;
		}
		OleObject(OOX::Document *pMain, const OOX::CPath& filename, bool bMsPackage = false) : Media (pMain)
		{
			m_bMsPackage = bMsPackage;
			read(filename);
		}
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, CContentTypes& content) const
		{
		}
		virtual const FileType type() const
		{
			if (m_bMsPackage)	return OOX::FileTypes::MicrosoftOfficeUnknown;
			else				return OOX::FileTypes::OleObject;
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
		void set_filename_cache(const std::wstring & file_path)
		{
			m_filenameCache = file_path;
		}
		void set_filename_cache(CPath & file_path)
		{
			m_filenameCache = file_path;
		}
		CPath filename_cache()
		{
			return m_filenameCache;
		}
		bool isMsPackage() {return m_bMsPackage;}
	protected:
		CPath	m_filenameCache; //image
		bool	m_bMsPackage;
	};
} // namespace OOX

#endif // OOX_OLE_OBJECT_INCLUDE_H_
