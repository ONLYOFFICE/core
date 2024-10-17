﻿/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "File.h"
#include "../Base/Nullable.h"

namespace PPTX
{
	class Core;
}
namespace OOX
{
	class CCore : public OOX::File
	{
	public:
		CCore(OOX::Document* pMain);
		CCore(OOX::Document* pMain, const CPath& oPath);
		virtual ~CCore();

		virtual void read(const CPath& oPath);
		virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;

		virtual const FileType type() const;

		virtual const CPath DefaultDirectory() const;
		virtual const CPath DefaultFileName() const;

		void SetDefaults();
		void SetRequiredDefaults();
		void SetCreator(std::wstring sVal);
		void SetLastModifiedBy(std::wstring sVal);

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		nullable_string m_sCategory;
		nullable_string m_sContentStatus;
		nullable_string m_sCreated;
		nullable_string m_sCreator;
		nullable_string m_sDescription;
		nullable_string m_sIdentifier;
		nullable_string m_sKeywords;
		nullable_string m_sLanguage;
		nullable_string m_sLastModifiedBy;
		nullable_string m_sLastPrinted;
		nullable_string m_sModified;
		nullable_string m_sRevision;
		nullable_string m_sSubject;
		nullable_string m_sTitle;
		nullable_string m_sVersion;
	};
} // namespace OOX
