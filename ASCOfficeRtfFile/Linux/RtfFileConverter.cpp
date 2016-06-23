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

#include "../RtfFormatLib/source/ConvertationManager.h"
#include "../../../ServerComponents/ASCOfficeUtils/ASCOfficeUtilsLib/OfficeUtils.h"

#include <iostream>
#include "../version.h"


typedef enum tagTConversionDirection
{
    TCD_ERROR,
    TCD_AUTO,
    TCD_RTF2DOCX,
    TCD_DOCX2RTF
} TConversionDirection;

static std::wstring utf8_to_unicode(const char *src)
{
    if (src == NULL) return _T("");
    std::string temp = src;

    unsigned int nLength = temp.length();

    UTF32 *pStrUtf32 = new UTF32 [nLength+1];
    memset ((void *) pStrUtf32, 0, sizeof (UTF32) * (nLength+1));


    UTF8 *pStrUtf8 = (UTF8 *) src;

    // this values will be modificated
    const UTF8 *pStrUtf8_Conv = pStrUtf8;
    UTF32 *pStrUtf32_Conv = pStrUtf32;

    ConversionResult eUnicodeConversionResult = ConvertUTF8toUTF32 (&pStrUtf8_Conv,
                                         &pStrUtf8[nLength]
                    , &pStrUtf32_Conv
                    , &pStrUtf32 [nLength]
                    , strictConversion);

            if (conversionOK != eUnicodeConversionResult)
            {
                delete [] pStrUtf32;
                return L"";
            }
            std::wstring wsEntryName ((wchar_t *) pStrUtf32);

            delete [] pStrUtf32;
            return wsEntryName;
}


static const TConversionDirection getConversionDirectionFromExt (const CString &sFile1, const CString &sFile2)
{
    TConversionDirection res = TCD_ERROR;

    int nExt1Pos = sFile1.ReverseFind(_T('.'));
    int nExt2Pos = sFile2.ReverseFind(_T('.'));

    // check for directory (zip task)
    int nSeparator1Pos = sFile1.ReverseFind(_T('/'));
    if (-1 == nSeparator1Pos)
    {
        nSeparator1Pos = sFile1.ReverseFind(_T('\\'));
    }

    // check for directory (unzip task)
    int nSeparator2Pos = sFile2.ReverseFind(_T('/'));
    if (-1 == nSeparator2Pos)
    {
        nSeparator2Pos = sFile2.ReverseFind(_T('\\'));
    }

    // there are no directories in paths, both paths are paths to files
    if (-1 != nExt1Pos && -1 != nExt2Pos)
    {
        CString sExt1 = sFile1.Mid(nExt1Pos);
        CString sExt2 = sFile2.Mid(nExt2Pos);

        sExt1.ToLower();
        sExt2.ToLower();

        if ((0 == sExt1.CompareNoCase(_T(".rtf"))) && (0 == sExt2.CompareNoCase(_T(".docx"))))
        {
            res = TCD_RTF2DOCX;
        }
        else if ((0 == sExt1.CompareNoCase(_T(".docx"))) && (0 == sExt2.CompareNoCase(_T(".rtf"))))
        {
            res = TCD_DOCX2RTF;
        }
     }

    return res;
}

int main(int argc, char *argv[])
{
   // check arguments
    if (argc < 3)
    {
        // print out help topic

        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << "-------------------------------------------------------------------------------" << std::endl;
        std::cout << "\t\tRtf/Docx file converter. Version: " << STRVER  << std::endl;
        std::cout << "-------------------------------------------------------------------------------" << std::endl;
        std::cout << std::endl;
        std::cout << "USAGE: ASCRtfConverter \"path_to_file_1\" \"path_to_file_2\" " << std::endl;
        std::cout << "WHERE:" << std::endl;
        std::cout << "\t\"path_to_file_1\" is a path to file to be converted" << std::endl;
        std::cout << "\t\"path_to_file_2\" is a path to the corresponding output file" << std::endl << std::endl;
       return 1;
    }
    CString sArg1, sArg2, sExePath;

    sExePath    = utf8_to_unicode(argv [0]);
    sArg1       = utf8_to_unicode(argv [1]);
    sArg2       = utf8_to_unicode(argv [2]);


    CString sXMLOptions = _T("");

    CString sTempDir = FileSystem::Directory::GetFolderPath(sArg2) + FILE_SEPARATOR_STR + "Temp";
    FileSystem::Directory::CreateDirectory(string2std_string(sTempDir));

    TConversionDirection convDirect = getConversionDirectionFromExt(sArg1, sArg2);

    ConvertationManager  rtfFile;
    rtfFile.m_sTempFolder = sTempDir;

    HRESULT hRes = S_FALSE;
    switch ( convDirect )
    {
        case TCD_RTF2DOCX:
        {
            CString sTempUnpackedDOCX = sTempDir + FILE_SEPARATOR_STR + _T("docx_unpacked");
            FileSystem::Directory::CreateDirectory(sTempUnpackedDOCX);

            hRes = rtfFile.ConvertRtfToOOX(sArg1, sTempUnpackedDOCX, sXMLOptions);

            if (hRes == S_OK)
            {
                COfficeUtils oCOfficeUtils(NULL);
                hRes = oCOfficeUtils.CompressFileOrDirectory (string2std_string(sTempUnpackedDOCX), string2std_string(sArg2), -1);
            }
        }break;
        case TCD_DOCX2RTF:
        {
            CString sTempUnpackedDOCX = sTempDir + FILE_SEPARATOR_STR + _T("docx_unpacked");
            FileSystem::Directory::CreateDirectory(sTempUnpackedDOCX);

            COfficeUtils oCOfficeUtils(NULL);
            hRes = oCOfficeUtils.ExtractToDirectory(string2std_string(sArg1), string2std_string(sTempUnpackedDOCX), NULL, 0);

            if (hRes == S_OK)
            {
                hRes = rtfFile.ConvertOOXToRtf( sArg2, sTempUnpackedDOCX, sXMLOptions);
            }
        }break;
    }
    // delete temp dir
    FileSystem::Directory::DeleteDirectory(string2std_string(sTempDir));

    if (hRes != S_OK)return 2;

    return 0;
}
