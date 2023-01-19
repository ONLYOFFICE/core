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

#include "CommonInclude.h"
#include "Xlsx.h"

namespace OOX
{
    namespace Spreadsheet
    {
        class WorkbookComments : public OOX::File, public IFileContainer
	{
		public:
            WorkbookComments(OOX::Document* pMain) : OOX::File(pMain), OOX::IFileContainer(pMain)
            {
 				CXlsx* xlsx = dynamic_cast<CXlsx*>(pMain);
				if (xlsx)
				{
					xlsx->m_pWorkbookComments = this;
				}
			}
            WorkbookComments(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain), OOX::IFileContainer(pMain)
            {
				CXlsx* xlsx = dynamic_cast<CXlsx*>(pMain);
				if (xlsx)
				{
					xlsx->m_pWorkbookComments = this;
				}
				read( oRootPath, oPath );
			}
			virtual ~WorkbookComments()
			{
			}
            virtual void read(const CPath& oPath)
            {
				//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
				CPath oRootPath;
				read(oRootPath, oPath);
            }
            virtual void read(const CPath& oRootPath, const CPath& oPath)
            {
				m_oReadPath = oPath;
				IFileContainer::Read( oRootPath, oPath );
            }
            virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
            {
            }
			virtual const FileType type() const
			{
				return Spreadsheet::FileTypes::WorkbookComments;
			}
			virtual const CPath DefaultDirectory() const
			{
				return type().DefaultDirectory();
			}
			virtual const CPath DefaultFileName() const
			{
				return type().DefaultFileName();
			}

            CPath m_oReadPath;
	};
    }
} // namespace OOX
