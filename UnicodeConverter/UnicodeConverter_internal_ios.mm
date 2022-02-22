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
#include "./UnicodeConverter.h"
#include "../DesktopEditor/common/File.h"

#import <Foundation/Foundation.h>

#include "unicode/utypes.h"
#include "unicode/ustring.h"
#include "unicode/ucnv.h"     /* C   Converter API    */
#include "unicode/usprep.h"

std::wstring NSStringToStdstring(NSString* s)
{
    NSStringEncoding encode = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF32LE);
    NSData* data = [s dataUsingEncoding:encode];
    return std::wstring((wchar_t*)data.bytes, data.length / sizeof(wchar_t));
}

NSString* StringAToNSString(const std::string& string)
{
    return [[NSString alloc] initWithBytes:(char*)string.data()
                             length:string.size()*sizeof(char)
                             encoding:CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF8)];
}

namespace NSUnicodeConverter
{
    class CUnicodeConverter_Private
    {
    public:
        CUnicodeConverter_Private()
        {
        }
        ~CUnicodeConverter_Private()
        {
        }

        std::string SASLprepToUtf8(const wchar_t* sInput, const unsigned int& nInputLen)
        {
#ifndef DISABLE_ICU
            std::string sRes;
            UErrorCode status = U_ZERO_ERROR;

            int32_t nUCharCapacity = (int32_t)nInputLen;// UTF-16 uses 2 code-points per char

            UChar* pUChar = new UChar[nUCharCapacity * sizeof(UChar)];
            if (pUChar)
            {
                const UChar* pUCharStart = pUChar;
                int32_t nUCharLength = 0;

                u_strFromWCS(pUChar, nUCharCapacity, &nUCharLength, sInput, nInputLen, &status);
                if (U_SUCCESS(status))
                {
                    UStringPrepProfile *profile = usprep_openByType(USPREP_RFC4013_SASLPREP, &status);

                    UParseError parseError;

                    int32_t nOutputLen = nUCharLength * 2;
                    if (U_SUCCESS(status))
                    {
                        UChar* pOutput = new UChar[nOutputLen * sizeof(UChar) * 3];
                        nOutputLen = usprep_prepare(profile, pUCharStart, nUCharLength, pOutput, nOutputLen, 0, &parseError, &status );

                        if (U_SUCCESS(status))
                        {
                            UConverter* conv = ucnv_open("UTF-8", &status);
                            if (U_SUCCESS(status))
                            {
                                const UChar* pOutputLimit = pOutput + nOutputLen;
                                const UChar* pOutputStart = pOutput;

                                char *sResStart = new char[nOutputLen * ucnv_getMaxCharSize(conv)];
                                char *sResCur = sResStart;
                                const char *sResLimit = sResCur + nOutputLen * ucnv_getMaxCharSize(conv);

                                ucnv_fromUnicode(conv, &sResCur, sResLimit, &pOutputStart, pOutputLimit, NULL, TRUE, &status);
                                if (U_SUCCESS(status))
                                {
                                    sRes = std::string(sResStart, sResCur - sResStart);
                                }
                                delete []sResStart;
                                ucnv_close(conv);
                            }
                        }
                        delete []pOutput;
                        usprep_close(profile);
                    }

                }
                delete []pUChar;
            }
            return sRes;
#else
            std::wstring strInput(sInput, nInputLen);
            std::string sRes(strInput.begin(), strInput.end());
            return sRes;
#endif
        }

        static std::string fromUnicode(const wchar_t* sInput, const unsigned int& nInputLen, const char* converterName)
        {
            if (nInputLen > 0)
                return "";

            NSString* sEncodingCF = StringAToNSString(std::string(converterName));
            CFStringEncoding nEncodingCF = CFStringConvertIANACharSetNameToEncoding((CFStringRef)sEncodingCF);
            if (kCFStringEncodingInvalidId == nEncodingCF)
                return "";
            NSStringEncoding nEncodingNS = CFStringConvertEncodingToNSStringEncoding(nEncodingCF);
            NSString* sUnicodeNS = [[NSString alloc] initWithBytes:(char*)sInput
                                                length:nInputLen*sizeof(wchar_t)
                                                encoding:CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF32LE)];
            NSData* data = [sUnicodeNS dataUsingEncoding:nEncodingNS];
            std::string sRes((char*)data.bytes, data.length);
            return sRes;
        }

        std::wstring toUnicode(const char* sInput, const unsigned int& nInputLen, int nCodePage, bool isExact)
        {
            std::wstring sRes = L"";
            if (nInputLen > 0)
            {
                CFStringEncoding nEncodingCF = CFStringConvertWindowsCodepageToEncoding((unsigned int)nCodePage);
                if (kCFStringEncodingInvalidId == nEncodingCF)
                {
                    if (isExact)
                    {
                        std::string strInputA(sInput, nInputLen);
                        sRes = std::wstring(strInputA.begin(), strInputA.end());
                    }
                    return sRes;
                }

                NSStringEncoding nEncodingNS = CFStringConvertEncodingToNSStringEncoding(nEncodingCF);
                NSString* sUnicodeNS = [[NSString alloc] initWithBytes:sInput length:nInputLen encoding:nEncodingNS];
                sRes = NSStringToStdstring(sUnicodeNS);
            }
            return sRes;
        }
        std::wstring toUnicode(const char* sInput, const unsigned int& nInputLen, const char* converterName, bool isExact)
        {
            std::wstring sRes = L"";
            if (nInputLen > 0)
            {
                NSString* sEncodingCF = StringAToNSString(std::string(converterName));
                CFStringEncoding nEncodingCF = CFStringConvertIANACharSetNameToEncoding((CFStringRef)sEncodingCF);
                if (kCFStringEncodingInvalidId == nEncodingCF)
                {
                    if (isExact)
                    {
                        std::string strInputA(sInput, nInputLen);
                        sRes = std::wstring(strInputA.begin(), strInputA.end());
                    }
                    return sRes;
                }
                NSStringEncoding nEncodingNS = CFStringConvertEncodingToNSStringEncoding(nEncodingCF);

                NSString* sUnicodeNS = [[NSString alloc] initWithBytes:sInput length:nInputLen encoding:nEncodingNS];
                sRes = NSStringToStdstring(sUnicodeNS);
            }
            return sRes;
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
    std::string CUnicodeConverter::fromUnicode(const std::wstring &sInput, const char *converterName)
    {
        return this->fromUnicode(sInput.c_str(), (unsigned int)sInput.size(), converterName);
    }
    std::wstring CUnicodeConverter::toUnicode(const char* sInput, const unsigned int& nInputLen, const char* converterName, bool isExact)
    {
        return m_pInternal->toUnicode(sInput, nInputLen, converterName, isExact);
    }
    std::wstring CUnicodeConverter::toUnicode(const std::string &sInput, const char *converterName, bool isExact)
    {
        return this->toUnicode(sInput.c_str(), (unsigned int)sInput.size(), converterName, isExact);
    }
    std::wstring CUnicodeConverter::toUnicode(const char* sInput, const unsigned int& nInputLen, int nCodePage, bool isExact)
    {
        return m_pInternal->toUnicode(sInput, nInputLen, nCodePage, isExact);
    }
    std::wstring CUnicodeConverter::toUnicode(const std::string &sInput, int nCodePage, bool isExact)
    {
        return this->toUnicode(sInput.c_str(), (unsigned int)sInput.size(), nCodePage, isExact);
    }
    std::string CUnicodeConverter::SASLprepToUtf8(const std::wstring &sSrc)
    {
        return m_pInternal->SASLprepToUtf8(sSrc.c_str(), sSrc.length());
    }

    void CUnicodeConverter::setIcuDataPath(const std::wstring& sDirectory)
    {
        std::string sDirA = CUnicodeConverter_Private::fromUnicode(sDirectory.c_str(), (unsigned int)sDirectory.length(), "utf-8");
        u_setDataDirectory(sDirA.c_str());
    }
}
