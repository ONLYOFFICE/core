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
#ifndef OOX_XLSXIFILE_CONTAINER_INCLUDE_H_
#define OOX_XLSXIFILE_CONTAINER_INCLUDE_H_

#include "RId.h"
#include "UnknowTypeFile.h"
#include "IFileBuilder.h"
#include <map>

namespace OOX 
{
	class File;
	class FileType;
	class CRels;
	class CContentTypes;
	
	class Image;
	class HyperLink;
	class OleObject;
} // OOX

namespace OOX
{
	class IFileContainer
	{
	public:

		IFileContainer()
		{
			m_lMaxRid = 0;
		}
	protected:

        std::map<CString, smart_ptr<OOX::File>> m_mContainer;
		size_t                                m_lMaxRid;

	protected:

		void Read (const OOX::CPath& oRootPath, const OOX::CPath& oPath);
		void Read (const OOX::CRels& oRels, const OOX::CPath& oRootPath, const CPath& oPath);
		void Write(const OOX::CPath& oFileName, const CPath& oDir, OOX::CContentTypes& oContent) const;
		void Write(OOX::CRels& oRels, const CPath& oCurrent, const CPath& oDir, OOX::CContentTypes& oContent) const;

	protected:
		void Commit  (const CPath& oPath);
		void Finalize(const CPath& oFilefilename, const CPath& oDir, OOX::CContentTypes& oContent);
		void Finalize(OOX::CRels& oRels, const CPath& oCurrent, const CPath& oDir, OOX::CContentTypes& oContent);

	public:

		void ExtractPictures(const OOX::CPath& oPath) const;

	public:

		virtual smart_ptr<Image>     GetImage    (const RId& rId) const;
		virtual smart_ptr<HyperLink> GetHyperlink(const RId& rId) const;
		virtual smart_ptr<OleObject> GetOleObject(const RId& rId) const;

	public:

		template<typename T>
		const bool IsExist() const;
		const bool IsExist(const FileType& oType) const;
		const bool IsExist(const OOX::RId& rId) const;
		const bool IsExternal(const OOX::RId& rId) const;

		smart_ptr<OOX::File> Get(const FileType& oType);
		const RId Add(smart_ptr<OOX::File>& pFile);
		void      Add(const OOX::RId& rId, const smart_ptr<OOX::File>& pFile);

		template<typename T> 
		T&                   Find();
		smart_ptr<OOX::File> Find(const FileType& type) const;
		smart_ptr<OOX::File> Find(const OOX::RId& type) const;

		smart_ptr<OOX::File> operator [](const OOX::RId rId);
		smart_ptr<OOX::File> operator [](const FileType& oType);


	protected:

		static UnknowTypeFile Unknown;

	private:

		const RId GetMaxRId();
	};

} // namespace OOX

#endif // OOX_XLSXIFILE_CONTAINER_INCLUDE_H_
