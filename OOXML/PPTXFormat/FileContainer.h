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
#pragma once

#include "FileMap.h"
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

