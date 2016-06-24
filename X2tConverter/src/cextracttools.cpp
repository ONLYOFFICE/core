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
#include "../../DesktopEditor/common/StringBuilder.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/common/Path.h"
#include "../../OfficeUtils/src/OfficeUtils.h"

namespace NExtractTools
{
	const TConversionDirection getConversionDirectionFromExt (const std::wstring &sFile1, const std::wstring &sFile2)
	{
		TConversionDirection res = TCD_ERROR;

		int nExt1Pos = sFile1.rfind(_T('.'));
		int nExt2Pos = sFile2.rfind(_T('.'));

		// check for directory (zip task)
		int nSeparator1Pos = sFile1.rfind(_T('/'));
		if (-1 == nSeparator1Pos)
		{
			nSeparator1Pos = sFile1.rfind(_T('\\'));
		}

		// check for directory (unzip task)
		int nSeparator2Pos = sFile2.rfind(_T('/'));
		if (-1 == nSeparator2Pos)
		{
			nSeparator2Pos = sFile2.rfind(_T('\\'));
		}


		// check for directory in name
		bool bIsFile1Directory = ((-1 != nSeparator1Pos) && (nExt1Pos < nSeparator1Pos));   // i.e: /root1/child1.lalala/folder
		bool bIsFile2Directory = ((-1 != nSeparator2Pos) && (nExt2Pos < nSeparator2Pos));

		// automatic zip/unzip task detection
		if (bIsFile1Directory)
		{
			if (bIsFile2Directory)
			{
				// Couldn't process 2 directories
				return TCD_ERROR;
			}
			else
			{
                // dir ( ??? файл без расширения???) -> zip

                if (NSDirectory::Exists(sFile1))
                        return TCD_ZIPDIR;
			}
		}
		else if (bIsFile2Directory)
		{
			if (bIsFile1Directory)
			{
				// Couldn't process 2 directories
				return TCD_ERROR;
			}
			else
			{
				// zip -> dir
				return TCD_UNZIPDIR;
			}
		}

		// there are no directories in paths, both paths are paths to files
		if (-1 != nExt1Pos && -1 != nExt2Pos)
		{
			std::wstring sExt1 = sFile1.substr(nExt1Pos);
			std::wstring sExt2 = sFile2.substr(nExt2Pos);

			std::transform(sExt1.begin(), sExt1.end(), sExt1.begin(), tolower);
			std::transform(sExt2.begin(), sExt2.end(), sExt2.begin(), tolower);

			COfficeFileFormatChecker OfficeFileFormatChecker;

			if (OfficeFileFormatChecker.isOfficeFile(sFile1))
            {
                switch (OfficeFileFormatChecker.nFileType)
				{
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX:
					{
							 if (0 == sExt2.compare(_T(".doct")))		res = TCD_DOCX2DOCT;
						else if (0 == sExt2.compare(_T(".bin")))		res = TCD_DOCX2DOCT_BIN;
						else if (0 == sExt2.compare(_T(".rtf")))		res = TCD_DOCX2RTF;
                        else if (0 == sExt2.compare(_T(".odt")))		res = TCD_DOCX2ODT;
                    }break;
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX:
					{
							 if (0 == sExt2.compare(_T(".xlst")))		res = TCD_XLSX2XLST;
						else if (0 == sExt2.compare(_T(".bin")))		res = TCD_XLSX2XLST_BIN;
						else if (0 == sExt2.compare(_T(".csv")))		res = TCD_XLSX2CSV;
                        else if (0 == sExt2.compare(_T(".ods")))		res = TCD_XLSX2ODS;
                    }break;
                case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX:
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX:
					{
							 if (0 == sExt2.compare(_T(".bin")))		res = TCD_PPTX2PPTT_BIN;
						else if (0 == sExt2.compare(_T(".pptt")))		res = TCD_PPTX2PPTT;
						else if (0 == sExt2.compare(_T(".pptx")))		res = TCD_PPSX2PPTX;
					}break;
				case AVS_OFFICESTUDIO_FILE_TEAMLAB_DOCY:
					{
							 if (0 == sExt2.compare(_T(".docx")))		res = TCD_DOCT2DOCX;
						else if (0 == sExt2.compare(_T(".bin")))		res = TCD_T2BIN;
						else if (0 == sExt2.compare(_T(".rtf")))		res = TCD_DOCT2RTF;
					}break;
				case AVS_OFFICESTUDIO_FILE_TEAMLAB_XLSY:
					{
							 if (0 == sExt2.compare(_T(".xlsx")))		res = TCD_XLST2XLSX;
						else if (0 == sExt2.compare(_T(".bin")))		res = TCD_T2BIN;
						else if (0 == sExt2.compare(_T(".csv")))		res = TCD_XLST2CSV;
					}break;
				case AVS_OFFICESTUDIO_FILE_TEAMLAB_PPTY:
					{
							 if (0 == sExt2.compare(_T(".pptx")))		res = TCD_PPTT2PPTX;	
						else if (0 == sExt2.compare(_T(".bin")))		res = TCD_T2BIN;
					}break;
				case AVS_OFFICESTUDIO_FILE_CANVAS_WORD:
					{
							 if (0 == sExt2.compare(_T(".docx")))		res = TCD_DOCT_BIN2DOCX;
						else if (0 == sExt2.compare(_T(".doct")))		res = TCD_BIN2T;
						else if (0 == sExt2.compare(_T(".rtf")))		res = TCD_DOCT_BIN2RTF;
					}break;
				case AVS_OFFICESTUDIO_FILE_CANVAS_SPREADSHEET:
					{
							 if (0 == sExt2.compare(_T(".xlsx")))		res = TCD_XLST_BIN2XLSX;
						else if (0 == sExt2.compare(_T(".xlst")))		res = TCD_BIN2T;
						else if (0 == sExt2.compare(_T(".csv")))		res = TCD_XLST_BIN2CSV;
					}break;
				case AVS_OFFICESTUDIO_FILE_CANVAS_PRESENTATION:
					{
							 if (0 == sExt2.compare(_T(".pptx")))		res = TCD_PPTT_BIN2PPTX;
						else if (0 == sExt2.compare(_T(".pptt")))		res = TCD_BIN2T;
					}break;
				case AVS_OFFICESTUDIO_FILE_CANVAS_PDF:
					{
						if (0 == sExt2.compare(_T(".pdf")))					res = TCD_BIN2PDF;
					}break;
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV:
					{
							 if (0 == sExt2.compare(_T(".xlsx")))			res = TCD_CSV2XLSX;
						else if (0 == sExt2.compare(_T(".xlst")))			res = TCD_CSV2XLST;
						else if (0 == sExt2.compare(_T(".bin")))			res = TCD_CSV2XLST_BIN;
					}break;
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF:
					{
							 if (0 == sExt2.compare(_T(".docx")))			res = TCD_RTF2DOCX;
						else if (0 == sExt2.compare(_T(".doct")))			res = TCD_RTF2DOCT;
						else if (0 == sExt2.compare(_T(".bin")))			res = TCD_RTF2DOCT_BIN;
					}break;
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC:
					{
							 if (0 == sExt2.compare(_T(".docx")))			res = TCD_DOC2DOCX;
						else if (0 == sExt2.compare(_T(".doct")))			res = TCD_DOC2DOCT;
						else if (0 == sExt2.compare(_T(".bin")))			res = TCD_DOC2DOCT_BIN;
					}break;
                case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS:
                    {
                             if (0 == sExt2.compare(_T(".xlsx")))			res = TCD_XLS2XLSX;
                        else if (0 == sExt2.compare(_T(".xlst")))			res = TCD_XLS2XLST;
                        else if (0 == sExt2.compare(_T(".bin")))			res = TCD_XLS2XLST_BIN;
                    }break;
                case AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT:
					{
							 if (0 == sExt2.compare(_T(".docx")))			res = TCD_TXT2DOCX;
						else if (0 == sExt2.compare(_T(".doct")))			res = TCD_TXT2DOCT;
						else if (0 == sExt2.compare(_T(".bin")))			res = TCD_TXT2DOCT_BIN;
					}break;
                case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT:
                    {
                             if (0 == sExt2.compare(_T(".pptx")))           res = TCD_PPT2PPTX;
                        else if (0 == sExt2.compare(_T(".bin")))            res = TCD_PPT2PPTT_BIN;
                        else if (0 == sExt2.compare(_T(".pptt")))           res = TCD_PPT2PPTT;
                    }break;
                case AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT:
                case AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS:
                case AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP:
                    {
                             if (0 == sExt2.compare(_T(".bin")))		res = TCD_ODF2OOT_BIN;
                        else if (0 == sExt2.compare(_T(".doct")) ||
                                 0 == sExt2.compare(_T(".xlst")) ||
                                 0 == sExt2.compare(_T(".pptt")))		res = TCD_ODF2OOT;
                        else if (0 == sExt2.compare(_T(".docx")) ||
                                 0 == sExt2.compare(_T(".xlsx")) ||
                                 0 == sExt2.compare(_T(".pptx")))		res = TCD_ODF2OOX;
                    }break;
                }
            }
		}

		return res;
    }
    std::wstring getMailMergeXml(const std::wstring& sJsonPath, int nRecordFrom, int nRecordTo, const std::wstring& sField)
    {
        NSStringUtils::CStringBuilder oBuilder;
        oBuilder.WriteString(_T("<MailMergeData DatabasePath=\""));
        oBuilder.WriteEncodeXmlString(sJsonPath.c_str());
        oBuilder.WriteString(_T("\" Start=\""));
        oBuilder.AddInt(nRecordFrom);
        oBuilder.WriteString(_T("\" End=\""));
        oBuilder.AddInt(nRecordTo);
        oBuilder.WriteString(_T("\" Field=\""));
        oBuilder.WriteEncodeXmlString(sField.c_str());
        oBuilder.WriteString(_T("\" />"));
        return oBuilder.GetData();
    }
    std::wstring getDoctXml(NSDoctRenderer::DoctRendererFormat::FormatFile eFromType, NSDoctRenderer::DoctRendererFormat::FormatFile eToType,
                            const std::wstring& sTFileDir, const std::wstring& sPdfBinFile, const std::wstring& sFontPath, const std::wstring& sImagesDirectory,
                            const std::wstring& sThemeDir, int nTopIndex, const std::wstring& sMailMerge, const InputParams& params)
    {
        NSStringUtils::CStringBuilder oBuilder;
        oBuilder.WriteString(_T("<Settings><SrcFileType>"));
        oBuilder.AddInt((int)eFromType);
        oBuilder.WriteString(_T("</SrcFileType><DstFileType>"));
        oBuilder.AddInt((int)eToType);
        oBuilder.WriteString(_T("</DstFileType><SrcFilePath>"));
        oBuilder.WriteEncodeXmlString(sTFileDir.c_str());
        oBuilder.WriteString(_T("</SrcFilePath><DstFilePath>"));
        oBuilder.WriteEncodeXmlString(sPdfBinFile.c_str());
        oBuilder.WriteString(_T("</DstFilePath><FontsDirectory>"));
        oBuilder.WriteEncodeXmlString(sFontPath.c_str());
        oBuilder.WriteString(_T("</FontsDirectory><ImagesDirectory>"));
        oBuilder.WriteEncodeXmlString(sImagesDirectory.c_str());
        oBuilder.WriteString(_T("</ImagesDirectory><ThemesDirectory>"));
        oBuilder.WriteEncodeXmlString(sThemeDir.c_str());
        oBuilder.WriteString(_T("</ThemesDirectory>"));
        if(NULL != params.m_nDoctParams)
        {
            oBuilder.WriteString(_T("<DoctParams>"));
            oBuilder.AddInt(*params.m_nDoctParams);
            oBuilder.WriteString(_T("</DoctParams>"));
        }
        oBuilder.WriteString(_T("<Changes TopItem=\""));
        oBuilder.AddInt(nTopIndex);
        oBuilder.WriteString(_T("\">"));
        std::wstring sChangesDir = sTFileDir + FILE_SEPARATOR_STR + _T("changes");
        if (NSDirectory::Exists(sChangesDir))
        {
            std::vector<std::wstring> aChangesFiles;
            CArray<std::wstring> oArray;
            NSDirectory::GetFiles2(sChangesDir, oArray, false);
            std::vector<std::wstring> aFiles;
            for(int i = 0; i < oArray.GetCount(); ++i)
            {
                aChangesFiles.push_back(oArray[i]);
            }
            std::sort (aChangesFiles.begin(), aChangesFiles.end(), compare_string_by_length);

            for(int i = 0; i < aChangesFiles.size(); ++i)
            {
                oBuilder.WriteString(_T("<Change>"));
                oBuilder.WriteEncodeXmlString(aChangesFiles[i]);
                oBuilder.WriteString(_T("</Change>"));
            }
        }
        oBuilder.WriteString(_T("</Changes>"));
        oBuilder.WriteString(sMailMerge);
        oBuilder.WriteString(_T("</Settings>"));
        return oBuilder.GetData();
    }
    int apply_changes(const std::wstring &sBinFrom, const std::wstring &sToResult, NSDoctRenderer::DoctRendererFormat::FormatFile eType, const std::wstring &sFontDir, const std::wstring &sThemeDir, std::wstring &sBinTo, const InputParams& params)
	{
        int nRes = 0;
		std::wstring sBinDir = FileSystem::Directory::GetFolderPath(sBinFrom);
		std::wstring sChangesDir = sBinDir + FILE_SEPARATOR_STR + _T("changes");
        if (NSDirectory::Exists(sChangesDir))
        {
			sBinTo = sBinDir + FILE_SEPARATOR_STR + _T("EditorWithChanges.bin");
			std::wstring sImagesDirectory = sBinDir + FILE_SEPARATOR_STR + _T("media");
           
			NSDoctRenderer::CDoctrenderer oDoctRenderer(NULL != params.m_sAllFontsPath ? *params.m_sAllFontsPath : _T(""));
            int nChangeIndex = -1;
            while (true)
            {
                std::wstring sXml = getDoctXml(eType, eType, sBinDir, sBinTo, sFontDir, sImagesDirectory, sThemeDir, nChangeIndex, _T(""), params);
				std::wstring sResult;
                oDoctRenderer.Execute(sXml, sResult);
                bool bContinue = false;
                if (!sResult.empty() && -1 != sResult.find(_T("error")))
                {
                    std::wcerr << _T("DoctRenderer:") << sResult << std::endl;
                    nRes = AVS_FILEUTILS_ERROR_CONVERT_CORRUPTED;
                    int nErrorIndex = -1;
                    int nErrorIndexStart = sResult.find(_T("index"));
                    if (-1 != nErrorIndexStart)
                    {
                        nErrorIndexStart = sResult.find(_T("\""), nErrorIndexStart + 1);
                        int nErrorIndexEnd = sResult.find(_T("\""), nErrorIndexStart + 1);
						nErrorIndex = _wtoi(sResult.substr(nErrorIndexStart + 1, nErrorIndexEnd - nErrorIndexStart - 1).c_str());
                    }
                    if (nErrorIndex > 0 && nChangeIndex != nErrorIndex)
                    {
                        nChangeIndex = nErrorIndex;
                        bContinue = true;
                    }
                    else
					{
                        sBinTo = sBinFrom;
					}
                }
                if (!bContinue)
                {
                    if(!sToResult.empty() && !params.getDontSaveAdditional())
                    {
                        std::vector<std::wstring> aImages = oDoctRenderer.GetImagesInChanges();
                        //todo сделать interface у COfficeUtils, чтобы можно было делать архив из файлов в разных папках.
                        for(int i = 0; i < aImages.size(); ++i)
                        {
                            std::wstring sImageName = aImages[i];
                            std::wstring sImage = sImagesDirectory + FILE_SEPARATOR_STR + sImageName;
                            std::wstring sImageCopy = sChangesDir + FILE_SEPARATOR_STR + sImageName;
                            NSFile::CFileBinary::Copy(sImage, sImageCopy);
                        }
                        std::wstring sToResultDir = FileSystem::Directory::GetFolderPath(sToResult);
                        std::wstring sTo = sToResultDir + FILE_SEPARATOR_STR + _T("changes.zip");
                        COfficeUtils oCOfficeUtils(NULL);
                        oCOfficeUtils.CompressFileOrDirectory(sChangesDir, sTo, -1);
                    }
                    break;
                }
            }
        }
		else
			sBinTo = sBinFrom;
        return nRes;
    }
}
