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

#include "cextracttools.h"
#include "ASCConverters.h"

#include "../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/doctrenderer/docbuilder.h"

//#include <iostream>

#define VALUE_TO_STRING(x) #x
#define VALUE(x) VALUE_TO_STRING(x)

using namespace NExtractTools;

#if !defined(_WIN32) && !defined (_WIN64)
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
#endif
int main(int argc, char *argv[])
{

   // check arguments
    if (argc < 2)
    {
        // print out help topic

        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << "-------------------------------------------------------------------------------" << std::endl;
        std::cout << "\t\tOOX/binary file converter. Version: " VALUE(INTVER)  << std::endl;
        std::cout << "-------------------------------------------------------------------------------" << std::endl;
        std::cout << std::endl;
		std::cout << "USAGE: x2t \"path_to_params_xml\"" << std::endl;
		std::cout << "or" << std::endl;
        std::cout << "USAGE: x2t \"path_to_file_1\" \"path_to_file_2\" [\"path_to_font_selection\"] [conversion_direction] [XmlOptionsFile]" << std::endl;
        std::cout << "WHERE:" << std::endl;
        std::cout << "\t\"path_to_file_1\" is a path to file to be converted" << std::endl;
        std::cout << "\t\"path_to_file_2\" is a path to the corresponding output file" << std::endl;
        std::cout << "\t\"path_to_font_selection\" is a path to 'font_selection.bin' location" << std::endl << std::endl;
        std::cout << "NOTE: if 'conversion_direction' is empty or 'auto' conversion direction will be calculated from file extensions" << std::endl << std::endl;

        return getReturnErrorCode(AVS_FILEUTILS_ERROR_CONVERT_PARAMS);
    }
    std::wstring            sArg1, sExePath;
    InputParams             oInputParams;
    TConversionDirection    conversion = TCD_AUTO;
    std::wstring            sXmlOptions;
    std::wstring            sPassword;

#if !defined(_WIN32) && !defined (_WIN64)
    sExePath    = utf8_to_unicode(argv [0]);
    sArg1       = utf8_to_unicode(argv [1]);
#else
    sExePath    = CA2T(argv [0]);
    sArg1       = CA2T(argv [1]);
#endif

    std::wstring sXmlExt = _T(".xml");
    if(sXmlExt == sArg1.substr(sArg1.length() - sXmlExt.length(), sXmlExt.length()))
	{
		oInputParams.FromXmlFile(sArg1);
        conversion  = oInputParams.getConversionDirection();
        sXmlOptions = oInputParams.getXmlOptions();
        sPassword   = oInputParams.getPassword();
	}
	else
	{
		std::wstring sArg2, sArg3, sArg4, sArg5;

#if !defined(_WIN32) && !defined (_WIN64)
		if (argc >= 3) sArg2   = utf8_to_unicode(argv [2]);
		if (argc >= 4) sArg3   = utf8_to_unicode(argv [3]);
		if (argc >= 5) sArg4   = utf8_to_unicode(argv [4]);
		if (argc >= 6) sArg5   = utf8_to_unicode(argv [5]);
#else
		if (argc >= 3) sArg2 = CA2T(argv [2]);
		if (argc >= 4) sArg3 = CA2T(argv [3]);
		if (argc >= 5) sArg4 = CA2T(argv [4]);
		if (argc >= 6) sArg5 = CA2T(argv [5]);
#endif
		oInputParams.m_sFileFrom	= new std::wstring(sArg1);
		oInputParams.m_sFileTo		= new std::wstring(sArg2);
		oInputParams.m_sPassword	= new std::wstring(L"password");
		
		// get conversion direction from 3rd argument
		if (argc > 3)
		{
			oInputParams.m_sFontDir = new std::wstring(sArg3);
		}
		if (argc > 5)
		{
            sXmlOptions = getXMLOptionsFromFile(sArg5);
		}

		if (argc > 4)
		{
			conversion = getConversionDirection (sArg4);

            if (TCD_ERROR == conversion && argc < 5 )
			 {
                sXmlOptions = getXMLOptionsFromFile(sArg4);
				conversion = TCD_AUTO;
			 }
		}
		sPassword = *oInputParams.m_sPassword;
	}

    std::wstring sFileFrom	= *oInputParams.m_sFileFrom;
    std::wstring sFileTo	= *oInputParams.m_sFileTo;
   
	int nFormatFrom = AVS_OFFICESTUDIO_FILE_UNKNOWN;
    if(NULL != oInputParams.m_nFormatFrom)
        nFormatFrom = *oInputParams.m_nFormatFrom;
    int nFormatTo = AVS_OFFICESTUDIO_FILE_UNKNOWN;
    if(NULL != oInputParams.m_nFormatTo)
        nFormatTo = *oInputParams.m_nFormatTo;

    if (TCD_ERROR == conversion)
    {
        if(AVS_OFFICESTUDIO_FILE_OTHER_MS_OFFCRYPTO == nFormatFrom)
            return getReturnErrorCode(AVS_FILEUTILS_ERROR_CONVERT_MS_OFFCRYPTO);
        else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT == nFormatFrom || AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV == nFormatFrom)
            return getReturnErrorCode(AVS_FILEUTILS_ERROR_CONVERT_NEED_PARAMS);
        else{
            // print out conversion direction error
            std::cerr << "Couldn't recognize conversion direction from an argument" << std::endl;
            return getReturnErrorCode(AVS_FILEUTILS_ERROR_CONVERT_PARAMS);
        }
    }

    if (sFileFrom.empty() || sFileTo.empty())
    {
        std::cerr << "Empty sFileFrom or sFileTo" << std::endl;
        return getReturnErrorCode(AVS_FILEUTILS_ERROR_CONVERT_PARAMS);
    }

    // get conversion direction from file extensions
    if (TCD_AUTO == conversion)
    {
        conversion = getConversionDirectionFromExt (sFileFrom, sFileTo);
    }

    if (TCD_ERROR == conversion)
    {
        // print out conversion direction error
        std::cerr << "Couldn't automatically recognize conversion direction from extensions" << std::endl;
        return getReturnErrorCode(AVS_FILEUTILS_ERROR_CONVERT_PARAMS);
    }
    std::wstring sFontPath;
    if(NULL != oInputParams.m_sFontDir)
        sFontPath = *oInputParams.m_sFontDir;
	bool bFromChanges = false;
	if(NULL != oInputParams.m_bFromChanges)
		bFromChanges = *oInputParams.m_bFromChanges;
	bool bPaid = true;
	if(NULL != oInputParams.m_bPaid)
		bPaid = *oInputParams.m_bPaid;
	std::wstring sThemeDir;
	if(NULL != oInputParams.m_sThemeDir)
		sThemeDir = *oInputParams.m_sThemeDir;
    InputParamsMailMerge* oMailMerge = NULL;
    if(NULL != oInputParams.m_oMailMergeSend)
        oMailMerge = oInputParams.m_oMailMergeSend;

    std::wstring sTempDir = FileSystem::Directory::GetFolderPath(sFileFrom) + FILE_SEPARATOR_STR + _T("Temp");
	NSDirectory::CreateDirectory(sTempDir);

	if (sTempDir.empty())
    {
        std::cerr << "Couldn't create temp folder" << std::endl;
        return getReturnErrorCode(AVS_FILEUTILS_ERROR_UNKNOWN);
    }

    int result = 0;
	switch(conversion)
	{
		case TCD_DOCX2DOCT:
		{
			result = docx2doct (sFileFrom, sFileTo, sTempDir, sFontPath);
		}break;
		case TCD_DOCT2DOCX:
		{
			result =  doct2docx (sFileFrom, sFileTo, sTempDir, sFontPath, bFromChanges, sThemeDir, oInputParams);
		}break;
		case TCD_XLSX2XLST:
		{
			result =  xlsx2xlst (sFileFrom, sFileTo, sTempDir, sFontPath, sXmlOptions);
		}break;
		case TCD_XLST2XLSX:
		{
			result =  xlst2xlsx (sFileFrom, sFileTo, sTempDir, sFontPath, bFromChanges, sThemeDir, oInputParams);
		}break;
		case TCD_PPTX2PPTT:
		{
			result =  pptx2pptt (sFileFrom, sFileTo, sTempDir, sFontPath);
		}break;
		case TCD_PPTT2PPTX:
		{
			result =  pptt2pptx (sFileFrom, sFileTo, sTempDir, sFontPath, bFromChanges, sThemeDir, oInputParams);
		}break;
		case TCD_ZIPDIR:
		{
			result =  dir2zip (sFileFrom, sFileTo);
		}break;
		case TCD_UNZIPDIR:
		{
			result =  zip2dir (sFileFrom, sFileTo);
		}break;
		case TCD_CSV2XLSX:
		{
			result =  csv2xlsx (sFileFrom, sFileTo, sXmlOptions, sTempDir, sFontPath);
		}break;
		case TCD_CSV2XLST:
		{
			result =  csv2xlst (sFileFrom, sFileTo, sXmlOptions, sTempDir, sFontPath);
		}break;
		case TCD_XLSX2CSV:
		{
			result =  xlsx2csv (sFileFrom, sFileTo, sXmlOptions, sTempDir, sFontPath);
		}break;
		case TCD_XLST2CSV:
		{
			result =  xlst2csv (sFileFrom, sFileTo, sXmlOptions, sTempDir, sFontPath);
		}break;
		case TCD_DOCX2DOCT_BIN:
		{
			result = docx2doct_bin (sFileFrom, sFileTo, sTempDir, sFontPath);
		}break;
		case TCD_DOCT_BIN2DOCX:
		{
			result =  doct_bin2docx (sFileFrom, sFileTo, sTempDir, sFontPath, bFromChanges, sThemeDir, oInputParams);
		}break;
		case TCD_XLSX2XLST_BIN:
		{
			result =  xlsx2xlst_bin (sFileFrom, sFileTo, sTempDir, sFontPath, sXmlOptions);
		}break;
		case TCD_XLST_BIN2XLSX:
		{
			result =  xlst_bin2xlsx (sFileFrom, sFileTo, sTempDir, sFontPath, bFromChanges, sThemeDir, oInputParams);
		}break;
		case TCD_PPTX2PPTT_BIN:
		{
			result =  pptx2pptt_bin (sFileFrom, sFileTo, sTempDir, sFontPath);
		}break;
		case TCD_PPTT_BIN2PPTX:
		{
			result =  pptt_bin2pptx (sFileFrom, sFileTo, sTempDir, sFontPath, bFromChanges, sThemeDir, oInputParams);
		}break;
		case TCD_BIN2PDF:
		{
			result =  bin2pdf (sFileFrom, sFileTo, sTempDir, sFontPath, bPaid, sThemeDir);
		}break;
		case TCD_BIN2T:
		{
			result =  dir2zip (FileSystem::Directory::GetFolderPath(sFileFrom), sFileTo);
		}break;
		case TCD_T2BIN:
		{
			result =  zip2dir (sFileFrom, FileSystem::Directory::GetFolderPath(sFileTo));
		}break;
		case TCD_PPSX2PPTX:
		{
			result =  ppsx2pptx (sFileFrom, sFileTo, sTempDir);
		}break;
		case TCD_PPT2PPTX:
		{
			result =  ppt2pptx (sFileFrom, sFileTo, sTempDir);
		}
		case TCD_PPT2PPTT:
		{
			result =  ppt2pptt (sFileFrom, sFileTo, sTempDir, sFontPath);
		}break;
		case TCD_PPT2PPTT_BIN:
		{
			result =  ppt2pptt_bin (sFileFrom, sFileTo, sTempDir, sFontPath);
		}break;
		case TCD_RTF2DOCX:
		{
			result =  rtf2docx (sFileFrom, sFileTo, sTempDir);
		}break;
		case TCD_RTF2DOCT:
		{
			result = rtf2doct (sFileFrom, sFileTo, sTempDir, sFontPath);
		}break;
		case TCD_RTF2DOCT_BIN:
		{
			result = rtf2doct_bin (sFileFrom, sFileTo, sTempDir, sFontPath);
		}break;
		case TCD_DOCX2RTF:
		{
			result =  docx2rtf (sFileFrom, sFileTo, sTempDir);
		}break;
		case TCD_DOC2DOCX:
		{
			result =  doc2docx (sFileFrom, sFileTo, sTempDir, oInputParams);
		}break;
		case TCD_DOCT2RTF:
		{
			result =  doct2rtf (sFileFrom, sFileTo, sTempDir, sFontPath, bFromChanges, sThemeDir, oInputParams);
		}break;
		case TCD_DOCT_BIN2RTF:
		{
			result =  doct_bin2rtf (sFileFrom, sFileTo, sTempDir, sFontPath, bFromChanges, sThemeDir, oInputParams);
		}break;
		case TCD_TXT2DOCX:
		{
			result =  txt2docx (sFileFrom, sFileTo, sXmlOptions, sTempDir);
		}break;
		case TCD_TXT2DOCT:
		{
			result = txt2doct (sFileFrom, sFileTo, sXmlOptions, sTempDir, sFontPath);
		}break;
		case TCD_TXT2DOCT_BIN:
		{
			result = txt2doct_bin (sFileFrom, sFileTo, sXmlOptions, sTempDir, sFontPath);
		}break;
		case TCD_XLS2XLSX:
		{
			result =  xls2xlsx (sFileFrom, sFileTo, sTempDir, sFontPath, oInputParams);
		}break;
		case TCD_XLS2XLST:
		{
			result = xls2xlst (sFileFrom, sFileTo, sPassword, sTempDir, sFontPath, sXmlOptions);
		}break;
		case TCD_XLS2XLST_BIN:
		{
			result = xls2xlst_bin (sFileFrom, sFileTo, sPassword, sTempDir, sFontPath, sXmlOptions);
		}break;
		case TCD_ODF2OOX:
		{
			result =  odf2oox (sFileFrom, sFileTo, sTempDir, sFontPath);
		}break;
		case TCD_ODF2OOT:
		{
			result = odf2oot (sFileFrom, sFileTo,  sTempDir, sFontPath);
		}break;
		case TCD_ODF2OOT_BIN:
		{
			result = odf2oot_bin (sFileFrom, sFileTo, sTempDir, sFontPath);
		}
		case TCD_DOCX2ODT:
		{
			result =  docx2odt (sFileFrom, sFileTo, sTempDir, sFontPath);
		}break;
		case TCD_XLSX2ODS:
		{
			result =  xlsx2ods (sFileFrom, sFileTo, sTempDir, sFontPath);
		}break;
		case TCD_MAILMERGE:
		{
			result = convertmailmerge(*oMailMerge, sFileFrom, sFileTo, sTempDir, sFontPath, bPaid, sThemeDir, oInputParams);
		}break;
		case TCD_DOCUMENT2:
		{
			result = fromDocument(sFileFrom, nFormatFrom, sFileTo, nFormatTo, sTempDir, sFontPath, sXmlOptions, sThemeDir, bFromChanges, bPaid, oInputParams);
		}break;
		case TCD_SPREADSHEET2:
		{
			result = fromSpreadsheet(sFileFrom, nFormatFrom, sFileTo, nFormatTo, sTempDir, sFontPath, sXmlOptions, sThemeDir, bFromChanges, bPaid, oInputParams);
		}break;
		case TCD_PRESENTATION2:
		{
			result = fromPresentation(sFileFrom, nFormatFrom, sFileTo, nFormatTo, sTempDir, sFontPath, sXmlOptions, sThemeDir, bFromChanges, bPaid, oInputParams);
		}break;
		case TCD_T2:
		{
			result = fromT(sFileFrom, nFormatFrom, sFileTo, nFormatTo, sTempDir, sFontPath, sXmlOptions, sThemeDir, bFromChanges, bPaid, oInputParams);
		}break;
		case TCD_DOCT_BIN2:
		{
			result = fromDoctBin(sFileFrom, sFileTo, nFormatTo, sTempDir, sFontPath, sXmlOptions, sThemeDir, bFromChanges, bPaid, oInputParams);
		}break;
		case TCD_XLST_BIN2:
		{
			result = fromXlstBin(sFileFrom, sFileTo, nFormatTo, sTempDir, sFontPath, sXmlOptions, sThemeDir, bFromChanges, bPaid, oInputParams);
		}break;
		case TCD_PPTT_BIN2:
		{
			result = fromPpttBin(sFileFrom, sFileTo, nFormatTo, sTempDir, sFontPath, sXmlOptions, sThemeDir, bFromChanges, bPaid, oInputParams);
		}break;
		case TCD_CROSSPLATFORM2:
		{
			result = fromCrossPlatform(sFileFrom, nFormatFrom, sFileTo, nFormatTo, sTempDir, sFontPath, sXmlOptions, sThemeDir, bFromChanges, bPaid, oInputParams);
		}break;
		case TCD_CANVAS_PDF2:
		{
			result = fromCanvasPdf(sFileFrom, nFormatFrom, sFileTo, nFormatTo, sTempDir, sFontPath, sXmlOptions, sThemeDir, bFromChanges, bPaid);
		}break;
		case TCD_MSCRYPT2DOCT:
		case TCD_MSCRYPT2XLST:
		case TCD_MSCRYPT2PPTT:
		{
			result = mscrypt2oot (sFileFrom, sFileTo, sTempDir, sFontPath, oInputParams);
		}break;
		case TCD_MSCRYPT2BIN:
			result =  mscrypt2oot_bin (sFileFrom, sFileTo, sTempDir, sFontPath, oInputParams);
		{
		}break;
	}

	// delete temp dir
    NSDirectory::DeleteDirectory(sTempDir);

	//clean up v8
	NSDoctRenderer::CDocBuilder::Dispose();

    return getReturnErrorCode(result);
}
