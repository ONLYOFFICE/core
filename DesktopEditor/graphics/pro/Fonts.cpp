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

#include "Fonts.h"
#include "../../common/File.h"
#include "../../fontengine/ApplicationFonts.h"

namespace NSFonts
{
    CLibrary::CLibrary()
    {
        m_internal = new CLibrary_private();
        m_internal->m_library = NULL;
    }
    CLibrary::~CLibrary()
    {
        RELEASEOBJECT(m_internal);
    }
}

namespace NSFonts
{
    namespace NSFontPath
    {
        IFontPath* Create()
        {
            return new CFontPath();
        }
    }

    namespace NSStream
    {
        IFontStream* Create()
        {
            return new CFontStream();
        }
    }

    namespace NSApplicationFontStream
    {
        IApplicationFontStreams* Create()
        {
            return new CApplicationFontStreams();
        }
    }

    namespace NSFontFile
    {
        IFontFile* Create()
        {
            return new CFontFile();
        }
    }

    namespace NSFontCache
    {
        IFontsCache* Create()
        {
            return new CFontsCache();
        }
    }

    namespace NSFontManager
    {
        IFontManager* Create()
        {
            return new CFontManager();
        }
    }

    namespace NSApplication
    {
        IApplicationFonts* Create()
        {
            return new CApplicationFonts();
        }
    }
}

namespace NSFonts
{
    CFontInfo* FromBuffer(BYTE*& pBuffer, std::wstring strDir)
    {
        // name
        int lLen = *((int*)pBuffer);
        pBuffer += sizeof(int);

        int len2 = lLen >> 1;
        wchar_t* sName = new wchar_t[len2 + 1];
        for (int i = 0; i < len2; ++i)
        {
            sName[i] = (wchar_t)(pBuffer[2 * i] | (pBuffer[2 * i + 1] << 8));
            if (sName[i] == wchar_t('\\'))
                sName[i] = wchar_t('/');
            if (0 == sName[i])
            {
                len2 = i;
                break;
            }
        }
        sName[len2] = 0;

        std::wstring strName(sName, len2);
        pBuffer += lLen;

        RELEASEARRAYOBJECTS(sName);

        // path
        lLen = *((int*)pBuffer);
        pBuffer += sizeof(int);

        len2 = lLen >> 1;
        sName = new wchar_t[len2 + 1];
        for (int i = 0; i < len2; ++i)
        {
            sName[i] = (wchar_t)(pBuffer[2 * i] | (pBuffer[2 * i + 1] << 8));
            if (sName[i] == wchar_t('\\'))
                sName[i] = wchar_t('/');
            if (0 == sName[i])
            {
                len2 = i;
                break;
            }
        }
        sName[len2] = 0;

        std::wstring strPath(sName, len2);
        pBuffer += lLen;

        RELEASEARRAYOBJECTS(sName);

        // index
        LONG lIndex = *((int*)pBuffer);
        pBuffer += sizeof(int);

        // italic
        INT bItalic = *((INT*)pBuffer);
        pBuffer += sizeof(INT);

        // bold
        INT bBold = *((INT*)pBuffer);
        pBuffer += sizeof(INT);

        // FixedWidth
        INT bFixedWidth = *((INT*)pBuffer);
        pBuffer += sizeof(INT);

        // Panose
        lLen = *((int*)pBuffer); // должно быть равно 10
        pBuffer += sizeof(int);

        BYTE pPanose[10];
        memcpy( (void *)pPanose, (const void *)pBuffer, 10 );
        pBuffer += lLen;

        // ulUnicodeRange1
        UINT ulRange1 = *((UINT*)pBuffer);
        pBuffer += sizeof(UINT);

        // ulUnicodeRange2
        UINT ulRange2 = *((UINT*)pBuffer);
        pBuffer += sizeof(UINT);

        // ulUnicodeRange3
        UINT ulRange3 = *((UINT*)pBuffer);
        pBuffer += sizeof(UINT);

        // ulUnicodeRange4
        UINT ulRange4 = *((UINT*)pBuffer);
        pBuffer += sizeof(UINT);

        // ulCodePageRange1
        UINT ulCodeRange1 = *((UINT*)pBuffer);
        pBuffer += sizeof(UINT);

        // ulCodePageRange2
        ULONG ulCodeRange2 = *((UINT*)pBuffer);
        pBuffer += sizeof(UINT);

        // usWeightClass
        USHORT usWeight = *((USHORT*)pBuffer);
        pBuffer += sizeof(USHORT);

        // usWidthClass
        USHORT usWidth = *((USHORT*)pBuffer);
        pBuffer += sizeof(USHORT);

        // sFamilyClass
        SHORT sFamilyClass = *((SHORT*)pBuffer);
        pBuffer += sizeof(SHORT);

        // FontFormat
        SHORT sFormat = *((SHORT*)pBuffer);
        pBuffer += sizeof(SHORT);

        // AvgCharWidth
        SHORT shAvgCharWidth = *((SHORT*)pBuffer);
        pBuffer += sizeof(SHORT);

        // Ascent
        SHORT shAscent = *((SHORT*)pBuffer);
        pBuffer += sizeof(SHORT);

        // Descent
        SHORT shDescent = *((SHORT*)pBuffer);
        pBuffer += sizeof(SHORT);

        // LineGap
        SHORT shLineGap = *((SHORT*)pBuffer);
        pBuffer += sizeof(SHORT);

        // XHeight
        SHORT shXHeight = *((SHORT*)pBuffer);
        pBuffer += sizeof(SHORT);

        // CapHeight
        SHORT shCapHeight = *((SHORT*)pBuffer);
        pBuffer += sizeof(SHORT);

            if (strPath.find(wchar_t('/')) == std::wstring::npos && strPath.find(wchar_t('\\')) == std::wstring::npos)
                strPath = strDir + strPath;

        CFontInfo* pInfo = new CFontInfo(strName,
            L"",
            strPath,
            lIndex,
            bBold,
            bItalic,
            bFixedWidth,
            (BYTE*)pPanose,
            ulRange1,
            ulRange2,
            ulRange3,
            ulRange4,
            ulCodeRange1,
            ulCodeRange2,
            usWeight,
            usWidth,
            sFamilyClass,
            (EFontFormat)sFormat,
            shAvgCharWidth,
            shAscent,
            shDescent,
            shLineGap,
            shXHeight,
            shCapHeight );

        return pInfo;
    }
    LONG GetBufferLen(CFontInfo* pInfo, std::wstring strDirectory, bool bIsOnlyFileName)
    {
        std::wstring sPath = pInfo->m_wsFontPath;
        if (0 != strDirectory.length())
        {
            if (0 == sPath.find(strDirectory))
            {
                sPath = sPath.substr(strDirectory.length());
            }
        }
        else if (bIsOnlyFileName)
        {
            size_t pos1 = sPath.find_last_of(wchar_t('/'));
            size_t pos2 = sPath.find_last_of(wchar_t('\\'));

            size_t pos = std::wstring::npos;
            if (pos1 != std::wstring::npos)
                pos = pos1;

            if (pos2 != std::wstring::npos)
            {
                if (pos == std::wstring::npos)
                    pos = pos2;
                else if (pos2 > pos)
                    pos = pos2;
            }

            if (pos != std::wstring::npos)
            {
                sPath = sPath.substr(pos + 1);
            }
        }
        //return 4 * g_lSizeofLONG + 3 * g_lSizeofBOOL + (m_wsFontName.GetLength() + sPath.GetLength() + 2) * g_lSizeofWCHAR + 2 * g_lSizeofUSHORT + 6 * g_lSizeofULONG + 10 + 8 * g_lSizeofSHORT;
        if (2 == sizeof(wchar_t))
        {
            return 4 * 4 + 3 * 4 + (pInfo->m_wsFontName.length() + sPath.length() + 2) * 2 + 2 * 2 + 6 * 4 + 10 + 8 * 2;
        }

        NSFile::CStringUtf16 s1;
        NSFile::CUtf8Converter::GetUtf16StringFromUnicode_4bytes2(pInfo->m_wsFontName.c_str(), pInfo->m_wsFontName.length(), s1);

        NSFile::CStringUtf16 s2;
        NSFile::CUtf8Converter::GetUtf16StringFromUnicode_4bytes2(sPath.c_str(), sPath.length(), s2);

        return 4 * 4 + 3 * 4 + (s1.Length + s2.Length + 2) * 2 + 2 * 2 + 6 * 4 + 10 + 8 * 2;
    }

    void ToBuffer(CFontInfo* pInfo, BYTE*& pBuffer, std::wstring strDirectory, bool bIsOnlyFileName)
    {
        // name
        int lLen = 0;

        if (2 == sizeof(wchar_t))
        {
            lLen = (pInfo->m_wsFontName.length() + 1) * 2;

            *((int*)(pBuffer))	= lLen;
            pBuffer += 4;

            memcpy(pBuffer, pInfo->m_wsFontName.c_str(), lLen);
            pBuffer += lLen;

            // path
            std::wstring sPath = pInfo->m_wsFontPath;
            if (0 != strDirectory.length())
            {
                if (0 == sPath.find(strDirectory))
                {
                    sPath = sPath.substr(strDirectory.length());
                }
            }
            else if (bIsOnlyFileName)
            {
                size_t pos1 = sPath.find_last_of(wchar_t('/'));
                size_t pos2 = sPath.find_last_of(wchar_t('\\'));

                size_t pos = std::wstring::npos;
                if (pos1 != std::wstring::npos)
                    pos = pos1;

                if (pos2 != std::wstring::npos)
                {
                    if (pos == std::wstring::npos)
                        pos = pos2;
                    else if (pos2 > pos)
                        pos = pos2;
                }

                if (pos != std::wstring::npos)
                {
                    sPath = sPath.substr(pos + 1);
                }
            }

            lLen = (sPath.length() + 1) * 2;

            *((INT*)(pBuffer))	= lLen;
            pBuffer += sizeof(INT);

            memcpy(pBuffer, sPath.c_str(), lLen);
            pBuffer += lLen;
        }
        else
        {
            NSFile::CStringUtf16 s1;
            NSFile::CUtf8Converter::GetUtf16StringFromUnicode_4bytes2(pInfo->m_wsFontName.c_str(), pInfo->m_wsFontName.length(), s1);

            lLen = s1.Length + 2;

            *((int*)(pBuffer))	= lLen;
            pBuffer += 4;

            memcpy(pBuffer, s1.Data, lLen);
            pBuffer += lLen;

            // path
            std::wstring sPath = pInfo->m_wsFontPath;
            if (0 != strDirectory.length())
            {
                if (0 == sPath.find(strDirectory))
                {
                    sPath = sPath.substr(strDirectory.length());
                }
            }
            else if (bIsOnlyFileName)
            {
                size_t pos1 = sPath.find_last_of(wchar_t('/'));
                size_t pos2 = sPath.find_last_of(wchar_t('\\'));

                size_t pos = std::wstring::npos;
                if (pos1 != std::wstring::npos)
                    pos = pos1;

                if (pos2 != std::wstring::npos)
                {
                    if (pos == std::wstring::npos)
                        pos = pos2;
                    else if (pos2 > pos)
                        pos = pos2;
                }

                if (pos != std::wstring::npos)
                {
                    sPath = sPath.substr(pos + 1);
                }
            }

            NSFile::CStringUtf16 s2;
            NSFile::CUtf8Converter::GetUtf16StringFromUnicode_4bytes2(sPath.c_str(), sPath.length(), s2);

            lLen = s2.Length + 2;

            *((INT*)(pBuffer))	= lLen;
            pBuffer += sizeof(INT);

            memcpy(pBuffer, s2.Data, lLen);
            pBuffer += lLen;
        }

        // index
        *((INT*)(pBuffer))	= (INT)pInfo->m_lIndex;
        pBuffer += sizeof(INT);

        // italic
        *((INT*)(pBuffer))	= pInfo->m_bItalic;
        pBuffer += sizeof(INT);

        // bold
        *((INT*)(pBuffer))	= pInfo->m_bBold;
        pBuffer += sizeof(INT);

        // FixedWidth
        *((INT*)pBuffer) = pInfo->m_bIsFixed;
        pBuffer += sizeof(INT);

        // Panose
        lLen = 10;

        *((INT*)(pBuffer))	= lLen;
        pBuffer += sizeof(INT);

        memcpy( (void *)pBuffer, (const void *)pInfo->m_aPanose, lLen );
        pBuffer += lLen;

        // ulUnicodeRange1
        *((UINT*)pBuffer) = (UINT)pInfo->m_ulUnicodeRange1;
        pBuffer += sizeof(UINT);

        // ulUnicodeRange2
        *((UINT*)pBuffer) = (UINT)pInfo->m_ulUnicodeRange2;
        pBuffer += sizeof(UINT);

        // ulUnicodeRange3
        *((UINT*)pBuffer) = (UINT)pInfo->m_ulUnicodeRange3;
        pBuffer += sizeof(UINT);

        // ulUnicodeRange4
        *((UINT*)pBuffer) = (UINT)pInfo->m_ulUnicodeRange4;
        pBuffer += sizeof(UINT);

        // ulCodePageRange1
        *((UINT*)pBuffer) = (UINT)pInfo->m_ulCodePageRange1;
        pBuffer += sizeof(UINT);

        // ulCodePageRange2
        *((UINT*)pBuffer) = (UINT)pInfo->m_ulCodePageRange2;
        pBuffer += sizeof(UINT);

        // usWeightClass
        *((USHORT*)pBuffer) = pInfo->m_usWeigth;
        pBuffer += sizeof(USHORT);

        // usWidthClass
        *((USHORT*)pBuffer) = pInfo->m_usWidth;
        pBuffer += sizeof(USHORT);

        // sFamilyClass
        *((SHORT*)pBuffer) = pInfo->m_sFamilyClass;
        pBuffer += sizeof(SHORT);

        // FontFormat
        *((SHORT*)pBuffer) = (SHORT)pInfo->m_eFontFormat;
        pBuffer += sizeof(SHORT);

        // AvgCharWidth
        *((SHORT*)pBuffer) = (SHORT)pInfo->m_shAvgCharWidth;
        pBuffer += sizeof(SHORT);

        // Ascent
        *((SHORT*)pBuffer) = (SHORT)pInfo->m_shAscent;
        pBuffer += sizeof(SHORT);

        // Descent
        *((SHORT*)pBuffer) = (SHORT)pInfo->m_shDescent;
        pBuffer += sizeof(SHORT);

        // LineGap
        *((SHORT*)pBuffer) = (SHORT)pInfo->m_shLineGap;
        pBuffer += sizeof(SHORT);

        // XHeight
        *((SHORT*)pBuffer) = (SHORT)pInfo->m_shXHeight;
        pBuffer += sizeof(SHORT);

        // CapHeight
        *((SHORT*)pBuffer) = (SHORT)pInfo->m_shCapHeight;
        pBuffer += sizeof(SHORT);
    }
}
