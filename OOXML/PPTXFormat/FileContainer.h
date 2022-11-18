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
#ifndef PPTX_IFILE_CONTAINER_INCLUDE_H_
#define PPTX_IFILE_CONTAINER_INCLUDE_H_

#include "FileMap.h"
#include "PPTXEvent.h"
#include "../DocxFormat/IFileContainer.h"
#include "../DocxFormat/Media/Image.h"
#include "../DocxFormat/Media/OleObject.h"
#include "../DocxFormat/External/External.h"

namespace PPTX
{
	//class LegacyDiagramText;

	class FileContainer : public OOX::IFileContainer
	{
	public:
		FileContainer(OOX::Document *pMain) : OOX::IFileContainer(pMain)
		{
		}
		virtual ~FileContainer()
		{
		}
		const bool IsExist(const OOX::FileType& oType) const;

		smart_ptr<OOX::File>	Get(const OOX::FileType& oType);
		void					Get(const OOX::FileType& oType, std::vector<smart_ptr<OOX::File>> & files);
		
		virtual std::wstring GetImagePathNameFromRId(const OOX::RId& rid)const;
		virtual std::wstring GetLinkFromRId			(const OOX::RId& rid)const;
		virtual std::wstring GetOleFromRId			(const OOX::RId& rid)const;
	protected:
		void read(const OOX::CPath& filename);
		void read(const OOX::CRels& rels, const OOX::CPath& path);

		void read(const OOX::CPath& filename, FileMap& map);
		void read(const OOX::CRels& rels, const OOX::CPath& path, FileMap& map);
		
		void write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::CContentTypes& content) const;
		void write(OOX::CRels& rels, const OOX::CPath& current, const OOX::CPath& directory, OOX::CContentTypes& content) const;

		void WrittenSetFalse();
		OOX::CPath CorrectPathRels(const OOX::CPath& path, OOX::Rels::CRelationShip* relation );
	};

} // namespace PPTX

#endif // PPTX_IFILE_CONTAINER_INCLUDE_H_
