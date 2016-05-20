
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
		oInputParams.m_sFileFrom = new std::wstring(sArg1);
		oInputParams.m_sFileTo = new std::wstring(sArg2);
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
    if (TCD_DOCX2DOCT == conversion)
    {
        result = docx2doct (sFileFrom, sFileTo, sTempDir, sFontPath);
    }
    else if (TCD_DOCT2DOCX == conversion)
    {
        result =  doct2docx (sFileFrom, sFileTo, sTempDir, sFontPath, bFromChanges, sThemeDir, oInputParams);
    }
    else if (TCD_XLSX2XLST == conversion)
    {
        result =  xlsx2xlst (sFileFrom, sFileTo, sTempDir, sFontPath, sXmlOptions);
    }
    else if (TCD_XLST2XLSX == conversion)
    {
        result =  xlst2xlsx (sFileFrom, sFileTo, sTempDir, sFontPath, bFromChanges, sThemeDir, oInputParams);
    }
    else if (TCD_PPTX2PPTT == conversion)
    {
        result =  pptx2pptt (sFileFrom, sFileTo, sTempDir, sFontPath);
    }
    else if (TCD_PPTT2PPTX == conversion)
    {
        result =  pptt2pptx (sFileFrom, sFileTo, sTempDir, sFontPath, bFromChanges, sThemeDir, oInputParams);
    }
    else if (TCD_ZIPDIR == conversion)
    {
        result =  dir2zip (sFileFrom, sFileTo);
    }
    else if (TCD_UNZIPDIR == conversion)
    {
        result =  zip2dir (sFileFrom, sFileTo);
    }
    else if (TCD_CSV2XLSX == conversion)
    {
        result =  csv2xlsx (sFileFrom, sFileTo, sXmlOptions, sTempDir, sFontPath);
    }
    else if (TCD_CSV2XLST == conversion)
    {
        result =  csv2xlst (sFileFrom, sFileTo, sXmlOptions, sTempDir, sFontPath);
    }
    else if (TCD_XLSX2CSV == conversion)
    {
        result =  xlsx2csv (sFileFrom, sFileTo, sXmlOptions, sTempDir, sFontPath);
    }
    else if (TCD_XLST2CSV == conversion)
    {
        result =  xlst2csv (sFileFrom, sFileTo, sXmlOptions, sTempDir, sFontPath);
    }
    else if (TCD_DOCX2DOCT_BIN == conversion)
    {
        result = docx2doct_bin (sFileFrom, sFileTo, sTempDir, sFontPath);
    }
    else if (TCD_DOCT_BIN2DOCX == conversion)
    {
        result =  doct_bin2docx (sFileFrom, sFileTo, sTempDir, sFontPath, bFromChanges, sThemeDir, oInputParams);
    }
    else if (TCD_XLSX2XLST_BIN == conversion)
    {
        result =  xlsx2xlst_bin (sFileFrom, sFileTo, sTempDir, sFontPath, sXmlOptions);
    }
    else if (TCD_XLST_BIN2XLSX == conversion)
    {
        result =  xlst_bin2xlsx (sFileFrom, sFileTo, sTempDir, sFontPath, bFromChanges, sThemeDir, oInputParams);
    }
    else if (TCD_PPTX2PPTT_BIN == conversion)
    {
        result =  pptx2pptt_bin (sFileFrom, sFileTo, sTempDir, sFontPath);
    }
    else if (TCD_PPTT_BIN2PPTX == conversion)
    {
        result =  pptt_bin2pptx (sFileFrom, sFileTo, sTempDir, sFontPath, bFromChanges, sThemeDir, oInputParams);
    }
    else if (TCD_BIN2PDF == conversion)
    {
        result =  bin2pdf (sFileFrom, sFileTo, sTempDir, sFontPath, bPaid, sThemeDir);
    }
    else if (TCD_BIN2T == conversion)
    {
        result =  dir2zip (FileSystem::Directory::GetFolderPath(sFileFrom), sFileTo);
    }
    else if (TCD_T2BIN == conversion)
    {
        result =  zip2dir (sFileFrom, FileSystem::Directory::GetFolderPath(sFileTo));
    }
    else if (TCD_PPSX2PPTX == conversion)
    {
        result =  ppsx2pptx (sFileFrom, sFileTo, sTempDir);
    }
    else if (TCD_PPT2PPTX == conversion)
    {
        result =  ppt2pptx (sFileFrom, sFileTo, sTempDir);
    }
    else if (TCD_PPT2PPTT == conversion)
    {
        result =  ppt2pptt (sFileFrom, sFileTo, sTempDir, sFontPath);
    }
    else if (TCD_PPT2PPTT_BIN == conversion)
    {
        result =  ppt2pptt_bin (sFileFrom, sFileTo, sTempDir, sFontPath);
    }
    else if (TCD_RTF2DOCX == conversion)
    {
        result =  rtf2docx (sFileFrom, sFileTo, sTempDir);
    }
    else if (TCD_RTF2DOCT == conversion)
    {
        result = rtf2doct (sFileFrom, sFileTo, sTempDir, sFontPath);
    }
    else if (TCD_RTF2DOCT_BIN == conversion)
    {
        result = rtf2doct_bin (sFileFrom, sFileTo, sTempDir, sFontPath);
    }
    else if (TCD_DOCX2RTF == conversion)
    {
        result =  docx2rtf (sFileFrom, sFileTo, sTempDir);
    }
    else if (TCD_DOC2DOCX == conversion)
    {
        result =  doc2docx (sFileFrom, sFileTo, sTempDir);
    }
    else if (TCD_DOCT2RTF == conversion)
    {
        result =  doct2rtf (sFileFrom, sFileTo, sTempDir, sFontPath, bFromChanges, sThemeDir, oInputParams);
    }
    else if (TCD_DOCT_BIN2RTF == conversion)
    {
        result =  doct_bin2rtf (sFileFrom, sFileTo, sTempDir, sFontPath, bFromChanges, sThemeDir, oInputParams);
    }
    else if (TCD_TXT2DOCX == conversion)
    {
        result =  txt2docx (sFileFrom, sFileTo, sXmlOptions, sTempDir);
    }
    else if (TCD_TXT2DOCT == conversion)
    {
        result = txt2doct (sFileFrom, sFileTo, sXmlOptions, sTempDir, sFontPath);
    }
    else if (TCD_TXT2DOCT_BIN == conversion)
    {
        result = txt2doct_bin (sFileFrom, sFileTo, sXmlOptions, sTempDir, sFontPath);
    }
    else if (TCD_XLS2XLSX == conversion)
    {
        result =  xls2xlsx (sFileFrom, sFileTo, sTempDir, sFontPath, oInputParams);
    }
    else if (TCD_XLS2XLST == conversion)
    {
        result = xls2xlst (sFileFrom, sFileTo, sPassword, sTempDir, sFontPath, sXmlOptions);
    }
    else if (TCD_XLS2XLST_BIN == conversion)
    {
        result = xls2xlst_bin (sFileFrom, sFileTo, sPassword, sTempDir, sFontPath, sXmlOptions);
    }
    else if (TCD_ODF2OOX == conversion)
    {
        result =  odf2oox (sFileFrom, sFileTo, sTempDir, sFontPath);
    }
    else if (TCD_ODF2OOT == conversion)
    {
        result = odf2oot (sFileFrom, sFileTo,  sTempDir, sFontPath);
    }
    else if (TCD_ODF2OOT_BIN == conversion)
    {
        result = odf2oot_bin (sFileFrom, sFileTo, sTempDir, sFontPath);
    }
    else if (TCD_DOCX2ODT == conversion)
    {
        result =  docx2odt (sFileFrom, sFileTo, sTempDir, sFontPath);
    }
    else if (TCD_XLSX2ODS == conversion)
    {
        result =  xlsx2ods (sFileFrom, sFileTo, sTempDir, sFontPath);
    }
    else if (TCD_MAILMERGE == conversion)
    {
        result = convertmailmerge(*oMailMerge, sFileFrom, sFileTo, sTempDir, sFontPath, bPaid, sThemeDir, oInputParams);
    }
    else if (TCD_DOCUMENT2 == conversion)
    {
        result = fromDocument(sFileFrom, nFormatFrom, sFileTo, nFormatTo, sTempDir, sFontPath, sXmlOptions, sThemeDir, bFromChanges, bPaid, oInputParams);
    }
    else if (TCD_SPREADSHEET2 == conversion)
    {
        result = fromSpreadsheet(sFileFrom, nFormatFrom, sFileTo, nFormatTo, sTempDir, sFontPath, sXmlOptions, sThemeDir, bFromChanges, bPaid, oInputParams);
    }
    else if (TCD_PRESENTATION2 == conversion)
    {
        result = fromPresentation(sFileFrom, nFormatFrom, sFileTo, nFormatTo, sTempDir, sFontPath, sXmlOptions, sThemeDir, bFromChanges, bPaid, oInputParams);
    }
    else if (TCD_T2 == conversion)
    {
        result = fromT(sFileFrom, nFormatFrom, sFileTo, nFormatTo, sTempDir, sFontPath, sXmlOptions, sThemeDir, bFromChanges, bPaid, oInputParams);
    }
    else if (TCD_DOCT_BIN2 == conversion)
    {
        result = fromDoctBin(sFileFrom, sFileTo, nFormatTo, sTempDir, sFontPath, sXmlOptions, sThemeDir, bFromChanges, bPaid, oInputParams);
    }
    else if (TCD_XLST_BIN2 == conversion)
    {
        result = fromXlstBin(sFileFrom, sFileTo, nFormatTo, sTempDir, sFontPath, sXmlOptions, sThemeDir, bFromChanges, bPaid, oInputParams);
    }
    else if (TCD_PPTT_BIN2 == conversion)
    {
        result = fromPpttBin(sFileFrom, sFileTo, nFormatTo, sTempDir, sFontPath, sXmlOptions, sThemeDir, bFromChanges, bPaid, oInputParams);
    }
    else if (TCD_CROSSPLATFORM2 == conversion)
    {
        result = fromCrossPlatform(sFileFrom, nFormatFrom, sFileTo, nFormatTo, sTempDir, sFontPath, sXmlOptions, sThemeDir, bFromChanges, bPaid, oInputParams);
    }
    else if (TCD_CANVAS_PDF2 == conversion)
    {
        result = fromCanvasPdf(sFileFrom, nFormatFrom, sFileTo, nFormatTo, sTempDir, sFontPath, sXmlOptions, sThemeDir, bFromChanges, bPaid);
    }

	// delete temp dir
    NSDirectory::DeleteDirectory(sTempDir);

	//clean up v8
	NSDoctRenderer::CDocBuilder::Dispose();

    return getReturnErrorCode(result);
}
