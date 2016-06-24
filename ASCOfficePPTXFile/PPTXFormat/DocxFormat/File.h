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
#ifndef OOX_FILE_INCLUDE_H_
#define OOX_FILE_INCLUDE_H_

#include "../../../Common/DocxFormat/Source/XML/XmlSimple.h"
#include "../../../Common/DocxFormat/Source/SystemUtility/SystemUtility.h"

#include "FileType.h"
#include "ContentTypes/File.h"

namespace PPTX
{
	class File
	{
	public:
		File(){}
		virtual ~File(){}

	public:
		virtual void read(const OOX::CPath& filename)																	= 0;
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const		= 0;

	public:
		virtual const PPTX::FileType type() const																	= 0;
		virtual const OOX::CPath DefaultDirectory() const																= 0;
		virtual const OOX::CPath DefaultFileName() const																	= 0;
	};
} // namespace PPTX

#endif // OOX_FILE_INCLUDE_H_