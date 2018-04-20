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
#include "./UnicodeConverter.h"

#include "unicode/utypes.h"
#include "unicode/ustring.h"
#include "unicode/ucnv.h"     /* C   Converter API    */

#include "../DesktopEditor/common/File.h"

namespace NSUnicodeConverter
{
    class CUnicodeConverter_Private
    {
    public:
        CUnicodeConverter_Private()
        {
#if 0
            std::wstring sDumpPath = NSFile::GetProcessDirectory() + L"/codepages.txt";
            NSFile::CFileBinary oFile;
            oFile.CreateFileW(sDumpPath);

            int32_t count = ucnv_countAvailable();

            for (int i = 0; i < count; ++i)
            {
                std::string sCodePage = ucnv_getAvailableName(i);

                UErrorCode _error = U_ZERO_ERROR;
                int nCountAliases = ucnv_countAliases(sCodePage.c_str(), &_error);

                char** palices = new char*[nCountAliases];
                ucnv_getAliases(sCodePage.c_str(), (const char**)palices, &_error);

                oFile.WriteStringUTF8(L"-----------------------------\r\n");
                oFile.WriteFile((BYTE*)sCodePage.c_str(), sCodePage.length());
                oFile.WriteStringUTF8(L"\r\n");

                for (int j = 0; j < nCountAliases; ++j)
                {
                    oFile.WriteFile((BYTE*)palices[j], strlen(palices[j]));
                    oFile.WriteStringUTF8(L"\r\n");
                }
            }

            oFile.CloseFile();
#endif
        }
        ~CUnicodeConverter_Private()
        {
        }

        std::string fromUnicode(const wchar_t* sInput, const unsigned int& nInputLen, const char* converterName)
        {
            std::string sRes = "";
            UErrorCode status = U_ZERO_ERROR;
            UConverter* conv = ucnv_open(converterName, &status);
            if (U_SUCCESS(status))
            {
                int32_t nUCharCapacity = (int32_t)nInputLen;// UTF-16 uses 2 code-points per char

                UChar* pUChar = new UChar[nUCharCapacity * sizeof(UChar)];
                if (pUChar)
                {
                    const UChar* pUCharStart = pUChar;
                    int32_t nUCharLength = 0;

                    u_strFromWCS(pUChar, nUCharCapacity, &nUCharLength, sInput, nInputLen, &status);
                    if (U_SUCCESS(status))
                    {
                        const UChar* pUCharLimit = pUCharStart + nUCharLength;
                        sRes.resize(nUCharLength * ucnv_getMaxCharSize(conv));// UTF-16 uses 2 code-points per char
                        char *sResStart = &sRes[0];
                        char *sResCur = sResStart;
                        const char *sResLimit = sResCur + sRes.size();

                        ucnv_fromUnicode(conv, &sResCur, sResLimit, &pUCharStart, pUCharLimit, NULL, TRUE, &status);
                        if (U_SUCCESS(status))
                        {
                            sRes.resize(sResCur - sResStart);
                        }
                        else
                        {
                            sRes.clear();
                        }
                    }
                    delete []pUCharStart;
                }
                ucnv_close(conv);
            }

            if (sRes.empty() && nInputLen > 0)
            {
                std::wstring ws(sInput, nInputLen);
                sRes = std::string(ws.begin(), ws.end());
            }
            return sRes;
        }

        std::string fromUnicode(const std::wstring& sInput, const char* converterName)
        {
            return fromUnicode(sInput.c_str(), (unsigned int)sInput.size(), converterName);
        }

        std::wstring toUnicode(const char* sInput, const unsigned int& nInputLen, int nCodePage)
        {
            std::wstring sRes = L"";
            UErrorCode status = U_ZERO_ERROR;
            UConverter* conv = ucnv_openCCSID(nCodePage, UCNV_UNKNOWN, &status);
            if (U_SUCCESS(status))
            {
                std::string sss = ucnv_getName(conv, &status);
                int iii = ucnv_getCCSID(conv, &status);

                //UConverter* conv = ucnv_openCCSID(5347, UCNV_IBM, &status);
                if (U_SUCCESS(status))
                {
                    const char* source = sInput;
                    const char* sourceLimit = source + nInputLen;

                    unsigned int uBufSize = (nInputLen / ucnv_getMinCharSize(conv));

                    UChar* targetStart = new UChar[uBufSize * sizeof(UChar)];
                    if (targetStart)
                        {
                        UChar* target = targetStart;
                        UChar* targetLimit = target + uBufSize;

                        ucnv_toUnicode(conv, &target, targetLimit, &source, sourceLimit, NULL, TRUE, &status);
                        if (U_SUCCESS(status))
                        {
                            unsigned int nTargetSize = target - targetStart;
                            sRes.resize(nTargetSize * 2);// UTF-16 uses 2 code-points per char
                            int32_t nResLen = 0;

                            u_strToWCS(&sRes[0], sRes.size(), &nResLen, targetStart, nTargetSize, &status);
                            if (U_SUCCESS(status))
                            {
                                sRes.resize(nResLen);
                            }
                            else
                            {
                                sRes.clear();
                            }
                        }
                        delete []targetStart;
                    }
                    ucnv_close(conv);
                }
            }

            if (sRes.empty() && nInputLen > 0)
            {
                std::string ws(sInput, nInputLen);
                sRes = std::wstring(ws.begin(), ws.end());
            }
            return sRes;
        }
        std::wstring toUnicode(const char* sInput, const unsigned int& nInputLen, const char* converterName)
        {
            std::wstring sRes = L"";
            UErrorCode status = U_ZERO_ERROR;
            UConverter* conv = ucnv_open(converterName, &status);
            if (U_SUCCESS(status))
            {
                std::string sss = ucnv_getName(conv, &status);
                int iii = ucnv_getCCSID(conv, &status);

                //UConverter* conv = ucnv_openCCSID(5347, UCNV_IBM, &status);
                if (U_SUCCESS(status))
                {
                    const char* source = sInput;
                    const char* sourceLimit = source + nInputLen;

                    unsigned int uBufSize = (nInputLen / ucnv_getMinCharSize(conv));
                    
                    UChar* targetStart = new UChar[uBufSize * sizeof(UChar)];
                    if (targetStart)
                    {
                        UChar* target = targetStart;
                        UChar* targetLimit = target + uBufSize;

                        ucnv_toUnicode(conv, &target, targetLimit, &source, sourceLimit, NULL, TRUE, &status);
                        if (U_SUCCESS(status))
                        {
                            unsigned int nTargetSize = target - targetStart;
                            sRes.resize(nTargetSize * 2);// UTF-16 uses 2 code-points per char
                            int32_t nResLen = 0;

                            u_strToWCS(&sRes[0], sRes.size(), &nResLen, targetStart, nTargetSize, &status);
                            if (U_SUCCESS(status))
                            {
                                sRes.resize(nResLen);
                            }
                            else
                            {
                                sRes.clear();
                            }
                        }
                        delete []targetStart;
                    }
                    ucnv_close(conv);
                }
            }

            if (sRes.empty() && nInputLen > 0)
            {
                std::string ws(sInput, nInputLen);
                sRes = std::wstring(ws.begin(), ws.end());
            }
            return sRes;
        }

        std::wstring toUnicodeExact(const char* sInput, const unsigned int& nInputLen, const char* converterName)
        {
            std::wstring sRes = L"";
            UErrorCode status = U_ZERO_ERROR;
            UConverter* conv = ucnv_open(converterName, &status);
            if (U_SUCCESS(status))
            {
                std::string sss = ucnv_getName(conv, &status);
                int iii = ucnv_getCCSID(conv, &status);

                //UConverter* conv = ucnv_openCCSID(5347, UCNV_IBM, &status);
                if (U_SUCCESS(status))
                {
                    const char* source = sInput;
                    const char* sourceLimit = source + nInputLen;

                    unsigned int uBufSize = (nInputLen / ucnv_getMinCharSize(conv));

                    UChar* targetStart = new UChar[uBufSize * sizeof(UChar)];
                    if (targetStart)
                    {
                        UChar* target = targetStart;
                        UChar* targetLimit = target + uBufSize;

                        ucnv_toUnicode(conv, &target, targetLimit, &source, sourceLimit, NULL, TRUE, &status);
                        if (U_SUCCESS(status))
                        {
                            unsigned int nTargetSize = target - targetStart;
                            sRes.resize(nTargetSize * 2);// UTF-16 uses 2 code-points per char
                            int32_t nResLen = 0;

                            u_strToWCS(&sRes[0], sRes.size(), &nResLen, targetStart, nTargetSize, &status);
                            if (U_SUCCESS(status))
                            {
                                sRes.resize(nResLen);
                            }
                            else
                            {
                                sRes.clear();
                            }
                        }
                        delete []targetStart;
                     }
                    ucnv_close(conv);
                }
            }

            return sRes;
        }
        std::wstring toUnicodeExact(const char* sInput, const unsigned int& nInputLen, int nCodePage)
        {
            std::wstring sRes = L"";
            UErrorCode status = U_ZERO_ERROR;
            UConverter* conv = ucnv_openCCSID(nCodePage, UCNV_UNKNOWN, &status);
            if (U_SUCCESS(status))
            {
                std::string sss = ucnv_getName(conv, &status);
                int iii = ucnv_getCCSID(conv, &status);

                //UConverter* conv = ucnv_openCCSID(5347, UCNV_IBM, &status);
                if (U_SUCCESS(status))
                {
                    const char* source = sInput;
                    const char* sourceLimit = source + nInputLen;

                    unsigned int uBufSize = (nInputLen / ucnv_getMinCharSize(conv));

                    UChar* targetStart = new UChar[uBufSize * sizeof(UChar)];
                    if (targetStart)
                    {
                        UChar* target = targetStart;
                        UChar* targetLimit = target + uBufSize;

                        ucnv_toUnicode(conv, &target, targetLimit, &source, sourceLimit, NULL, TRUE, &status);
                        if (U_SUCCESS(status))
                        {
                            unsigned int nTargetSize = target - targetStart;
                            sRes.resize(nTargetSize * 2);// UTF-16 uses 2 code-points per char
                            int32_t nResLen = 0;

                            u_strToWCS(&sRes[0], sRes.size(), &nResLen, targetStart, nTargetSize, &status);
                            if (U_SUCCESS(status))
                            {
                                sRes.resize(nResLen);
                            }
                            else
                            {
                                sRes.clear();
                            }
                        }
                        delete []targetStart;
                    }
                    ucnv_close(conv);
                }
            }

            return sRes;
        }
        inline std::wstring toUnicode(const std::string& sInput, const char* converterName)
        {
            return toUnicode(sInput.c_str(), (unsigned int)sInput.size(), converterName);
        }
        inline std::wstring toUnicode(const std::string& sInput, int nCodePage)
        {
            return toUnicode(sInput.c_str(), (unsigned int)sInput.size(), nCodePage);
        }
    };
}

namespace NSUnicodeConverter
{
    CUnicodeConverter::CUnicodeConverter()
    {
        m_pInternal = new CUnicodeConverter_Private();
    }
    CUnicodeConverter::~CUnicodeConverter()
    {
        delete m_pInternal;
    }

    std::string CUnicodeConverter::fromUnicode(const wchar_t* sInput, const unsigned int& nInputLen, const char* converterName)
    {
        return m_pInternal->fromUnicode(sInput, nInputLen, converterName);
    }
    std::string CUnicodeConverter::fromUnicode(const std::wstring &sSrc, const char *sCodePage)
    {
        return m_pInternal->fromUnicode(sSrc, sCodePage);
    }
    std::wstring CUnicodeConverter::toUnicodeExact(const char* sInput, const unsigned int& nInputLen, const char* converterName)
    {
        return m_pInternal->toUnicodeExact(sInput, nInputLen, converterName);
    }
    std::wstring CUnicodeConverter::toUnicode(const char* sInput, const unsigned int& nInputLen, const char* converterName)
    {
        return m_pInternal->toUnicode(sInput, nInputLen, converterName);
    }
    std::wstring CUnicodeConverter::toUnicode(const std::string &sSrc, const char *sCodePage)
    {
        return m_pInternal->toUnicode(sSrc, sCodePage);
    }
    std::wstring CUnicodeConverter::toUnicode(const char* sInput, const unsigned int& nInputLen, int nCodePage)
    {
        return m_pInternal->toUnicode(sInput, nInputLen, nCodePage);
    }
    std::wstring CUnicodeConverter::toUnicode(const std::string &sSrc, int nCodePage)
    {
        return m_pInternal->toUnicode(sSrc, nCodePage);
    }
    std::wstring CUnicodeConverter::toUnicodeExact(const char* sInput, const unsigned int& nInputLen, int nCodePage)
    {
        return m_pInternal->toUnicodeExact(sInput, nInputLen, nCodePage);
    }
}
