/*
 * (c) Copyright Ascensio System SIA 2010-2021
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

#include "../../XlsxFormat/WritingElement.h"
#include "../Biff12_structures/DBType.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/BiffRecord.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/BIFF12/XLWideString.h"

namespace XLSB
{
    // Logical representation of BrtBeginExtConnection record in BIFF12
    class BeginExtConnection: public XLS::BiffRecord
    {
            BIFF_RECORD_DEFINE_TYPE_INFO(BeginExtConnection)
            BASE_OBJECT_DEFINE_CLASS_NAME(BeginExtConnection)
        public:
            BeginExtConnection();
            ~BeginExtConnection();

            XLS::BaseObjectPtr clone();

            void readFields(XLS::CFRecord& record) override;
			void writeFields(XLS::CFRecord& record) override;

            static const XLS::ElementType	type = XLS::typeBeginExtConnection;

            BYTE         bVerRefreshed = 0;
            BYTE         bVerRefreshableMin = 0;
            BYTE         pc = 1;
            _UINT16      wInterval = 0;
            bool         fMaintain = false;
            bool         fNewQuery = false;
            bool         fDeleted = false;
            bool         fAlwaysUseConnectionFile = false;
            bool         fBackgroundQuery = false;
            bool         fRefreshOnLoad = false;
            bool         fSaveData = false;
            bool         fLoadSourceDataFile = false;
            bool         fLoadSourceConnectionFile = false;
            bool         fLoadConnectionDesc = false;
            bool         fLoadSSOApplicationID = false;
            DBType       idbtype;
            _UINT32      irecontype = 1;
            _UINT32      dwConnID = 1;
            BYTE         iCredMethod = 0;
            XLWideString stDataFile;
            XLWideString stConnectionFile;
            XLWideString stConnDesc;
            XLWideString stConnName;
            XLWideString stSso;
    };

} // namespace XLSB

