/*
 * (c) Copyright Ascensio System SIA 2010-2018
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

#include "File.h"

#ifdef _IOS

#import <Foundation/Foundation.h>

static const char* fileSystemRepresentation(const std::wstring& sFileName)
{
    NSString *path = [[NSString alloc] initWithBytes:(char*)sFileName.data()
                                              length:sFileName.size()* sizeof(wchar_t)
                                            encoding:CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF32LE)];
    
    return (const char*)[path fileSystemRepresentation];
}

namespace NSFile
{
    bool CFileBinary::OpenFile(const std::wstring& sFileName, bool bRewrite)
    {
        m_pFile = fopen(fileSystemRepresentation(sFileName), bRewrite ? "rb+" : "rb");
        
        if (NULL == m_pFile) {
#if DEBUG
            printf ("NSFile::OpenFile - error open file : %s\n",strerror(errno));
#endif
            return false;
        }
        
        fseek(m_pFile, 0, SEEK_END);
        m_lFileSize = ftell(m_pFile);
        fseek(m_pFile, 0, SEEK_SET);
        
        m_lFilePosition = 0;
        
        if (0 < sFileName.length())
        {
            if (((wchar_t)'/') == sFileName.c_str()[sFileName.length() - 1])
                m_lFileSize = 0x7FFFFFFF;
        }
        
        unsigned int err = 0x7FFFFFFF;
        unsigned int cur = (unsigned int)m_lFileSize;
        if (err == cur)
        {
            CloseFile();
            return false;
        }
        
        return true;
    }
    
    bool CFileBinary::CreateFileW(const std::wstring& sFileName)
    {
        m_pFile = fopen(fileSystemRepresentation(sFileName), "wb");
        
        if (NULL == m_pFile) {
#if DEBUG
            printf ("NSFile::CreateFileW - error create file : %s\n",strerror(errno));
#endif
            return false;
        }
        
        m_lFilePosition = 0;
        return true;
    }
}

#endif
