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
#ifndef PPTX_SLIDES_HANDOUTMASTER_INCLUDE_H_
#define PPTX_SLIDES_HANDOUTMASTER_INCLUDE_H_

#include "WrapperFile.h"
#include "FileContainer.h"
#include "FileTypes.h"

#include "Logic/ClrMap.h"
#include "Logic/CSld.h"
#include "Logic/Hf.h"


namespace PPTX
{
	class HandoutMaster : public WrapperFile, public PPTX::FileContainer
	{
	public:
		HandoutMaster(OOX::Document* pMain): WrapperFile(pMain), PPTX::FileContainer(pMain)
		{
		}
		HandoutMaster(OOX::Document* pMain, const OOX::CPath& filename, FileMap& map) : WrapperFile(pMain), PPTX::FileContainer(pMain)
		{
			read(filename, map);
		}
		virtual ~HandoutMaster()
		{
		}

	public:
		virtual void read(const OOX::CPath& filename, FileMap& map)
		{
			//FileContainer::read(filename, map);
			XmlUtils::CXmlNode oNode;
			oNode.FromXmlFile(filename.m_strFilename);

			cSld = oNode.ReadNode(_T("p:cSld"));
			cSld.SetParentFilePointer(this);

			clrMap = oNode.ReadNode(_T("p:clrMap"));
			clrMap.SetParentFilePointer(this);

			hf = oNode.ReadNode(_T("p:hf"));

			if (hf.is_init())
				hf->SetParentFilePointer(this);
		}
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::CContentTypes& content)const
		{
			WrapperFile::write(filename, directory, content);
			FileContainer::write(filename, directory, content);
		}

	public:
		virtual const OOX::FileType type() const
		{
			return OOX::Presentation::FileTypes::HandoutMaster;
		}
		virtual const OOX::CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const OOX::CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}

	public:
		Logic::CSld			cSld;
		Logic::ClrMap		clrMap;
		nullable<Logic::HF> hf;
	};
} // namespace PPTX

#endif // PPTX_SLIDES_HANDOUTMASTER_INCLUDE_H_
