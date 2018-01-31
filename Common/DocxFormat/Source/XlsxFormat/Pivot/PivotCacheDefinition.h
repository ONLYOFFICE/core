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
#ifndef OOX_PIVOTCACHEDEFINITION_FILE_INCLUDE_H_
#define OOX_PIVOTCACHEDEFINITION_FILE_INCLUDE_H_

#include "../CommonInclude.h"


namespace OOX
{
	namespace Spreadsheet
	{
		class CPivotCacheDefinition : public OOX::File, public OOX::IFileContainer
		{
		public:
			CPivotCacheDefinition(OOX::Document* pMain) : OOX::File(pMain), OOX::IFileContainer(pMain)
			{
                m_bSpreadsheets = true;
                bIsWritten = false;
                m_pData = NULL;
                m_nDataLength = 0;
            }
            CPivotCacheDefinition(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain), OOX::IFileContainer(pMain)
            {
                m_bSpreadsheets = true;
                bIsWritten = false;
                m_pData = NULL;
                m_nDataLength = 0;
                read( oRootPath, oPath );
			}
			virtual ~CPivotCacheDefinition()
			{
                            ClearItems();
			}
		public:

			virtual void read(const CPath& oPath)
			{
			}
			virtual void read(const CPath& oRootPath, const CPath& oPath)
			{	
			}
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
			{
                            if(m_nDataLength > 0 && !bIsWritten)
                            {
                                NSFile::CFileBinary oFile;
                                oFile.CreateFileW(oPath.GetPath());
                                oFile.WriteFile(m_pData, m_nDataLength);
                                oFile.CloseFile();

                                oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
                                IFileContainer::Write( oPath, oDirectory, oContent );

                                //prevent repeated write
                                bIsWritten = true;
                            }
			}
                        virtual const OOX::FileType type() const
                        {
                            return OOX::Spreadsheet::FileTypes::PivotCacheDefinition;
                        }
                        virtual const CPath DefaultDirectory() const
                        {
                            return type().DefaultDirectory();
                        }
                        virtual const CPath DefaultFileName() const
                        {
                            return type().DefaultFileName();
                        }
			const CPath& GetReadPath()
			{
                            return m_oReadPath;
			}
                        void setData(BYTE* pData, long length, const std::wstring& srIdRecords)
                        {
                            if(srIdRecords.length() > 0)
                            {
                                const std::string srIdRecordsA( srIdRecords.begin(), srIdRecords.end() );
                                std::string rIdAttr = " r:id=\""+ srIdRecordsA +"\"";
                                m_nDataLength = length + rIdAttr.length();
                                m_pData = new BYTE[m_nDataLength];
                                long nTreshold = 220;
                                memcpy(m_pData, pData, nTreshold);
                                memcpy(m_pData + nTreshold, rIdAttr.c_str(), rIdAttr.length());
                                memcpy(m_pData + nTreshold + rIdAttr.length(), pData + nTreshold, length - nTreshold);
                            }
                            else
                            {
                                m_nDataLength = length;
                                m_pData = new BYTE[m_nDataLength];
                                memcpy(m_pData, pData, length);
                            }
                        }
		private:
			CPath m_oReadPath;

                        void ClearItems()
                        {
                            m_nDataLength = 0;
                            RELEASEARRAYOBJECTS(m_pData)
                        }

		public:
                        mutable bool bIsWritten;
			BYTE* m_pData;
			long m_nDataLength;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_PIVOTCACHEDEFINITION_FILE_INCLUDE_H_
