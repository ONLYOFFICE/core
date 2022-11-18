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

		size_t nExt1Pos = sFile1.rfind(_T('.'));
		size_t nExt2Pos = sFile2.rfind(_T('.'));

		// check for directory (zip task)
		size_t nSeparator1Pos = sFile1.rfind(_T('/'));
		if (std::wstring::npos == nSeparator1Pos)
		{
			nSeparator1Pos = sFile1.rfind(_T('\\'));
		}

		// check for directory (unzip task)
		int nSeparator2Pos = sFile2.rfind(_T('/'));
		if (std::wstring::npos == nSeparator2Pos)
		{
			nSeparator2Pos = sFile2.rfind(_T('\\'));
		}

		// check for directory in name
		bool bIsFile1Directory = (std::wstring::npos != nExt1Pos) ? ((std::wstring::npos != nSeparator1Pos) && (nExt1Pos < nSeparator1Pos)) : (std::wstring::npos != nSeparator1Pos);   // i.e: /root1/child1.lalala/folder
		bool bIsFile2Directory = (std::wstring::npos != nExt2Pos) ? ((std::wstring::npos != nSeparator2Pos) && (nExt2Pos < nSeparator2Pos)) : (std::wstring::npos != nSeparator2Pos);

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
				int &type = OfficeFileFormatChecker.nFileType;
               
				switch (OfficeFileFormatChecker.nFileType)
				{
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX:
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM:
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX:
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM:
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM:
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCXF:
				{
							 if (0 == sExt2.compare(_T(".doct")))		res = TCD_DOCX2DOCT;
						else if (0 == sExt2.compare(_T(".bin")))		res = TCD_DOCX2DOCT_BIN;
						else if (0 == sExt2.compare(_T(".rtf")))		res = TCD_DOCX2RTF;
                        else if (0 == sExt2.compare(_T(".odt")))		res = TCD_DOCX2ODT;
                        else if (0 == sExt2.compare(_T(".docx")))
						{
							if (OfficeFileFormatChecker.nFileType == AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX)
								res = TCD_DOTX2DOCX;
							if (OfficeFileFormatChecker.nFileType == AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM)
								res = TCD_DOCM2DOCX;
							if (OfficeFileFormatChecker.nFileType == AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM)
								res = TCD_DOTM2DOCX;
							//oform 2 docx ???
							//docxf 2 docx ???
						}
                        else if (0 == sExt2.compare(_T(".docm")))	res = TCD_DOTM2DOCM;
						else if (0 == sExt2.compare(_T(".txt")))	res = TCD_DOCX2TXT;
				}break;
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX_PACKAGE:
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX_PACKAGE:
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX_PACKAGE:
					{
							 if (0 == sExt2.compare(_T(".doct")))		res = TCD_PKG2BIN_T;
						else if (0 == sExt2.compare(_T(".pptt")))		res = TCD_PKG2BIN_T;
						else if (0 == sExt2.compare(_T(".xlst")))		res = TCD_PKG2BIN_T;
						else if (0 == sExt2.compare(_T(".docx")))		res = TCD_PKG2OOXML;
						else if (0 == sExt2.compare(_T(".xlsx")))		res = TCD_PKG2OOXML;
						else if (0 == sExt2.compare(_T(".pptx")))		res = TCD_PKG2OOXML;
						else if (0 == sExt2.compare(_T(".bin")))		res = TCD_PKG2BIN;
				}break;
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX_FLAT:
				{
						 if (0 == sExt2.compare(_T(".doct")))		res = TCD_DOCXFLAT2DOCT;
					else if (0 == sExt2.compare(_T(".bin")))		res = TCD_DOCXFLAT2DOCT_BIN;
				}break;
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX_FLAT:
					{
							 if (0 == sExt2.compare(_T(".xlst")))		res = TCD_XLSXFLAT2XLST;
						else if (0 == sExt2.compare(_T(".bin")))		res = TCD_XLSXFLAT2XLST_BIN;
						else if (0 == sExt2.compare(_T(".xlsx")))		res = TCD_XLSXFLAT2XLSX;
				}break;
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX:
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM:
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX:
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM:				
                {
                         if (0 == sExt2.compare(_T(".xlst")))		res = TCD_XLSX2XLST;
                    else if (0 == sExt2.compare(_T(".bin")))		res = TCD_XLSX2XLST_BIN;
                    else if (0 == sExt2.compare(_T(".csv")))		res = TCD_XLSX2CSV;
                    else if (0 == sExt2.compare(_T(".ods")))		res = TCD_XLSX2ODS;
                    else if (0 == sExt2.compare(_T(".xlsx")))
                    {
                        if (OfficeFileFormatChecker.nFileType == AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX)
                            res = TCD_XLTX2XLSX;
                        if (OfficeFileFormatChecker.nFileType == AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM)
                            res = TCD_XLSM2XLSX;
                        if (OfficeFileFormatChecker.nFileType == AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM)
                            res = TCD_XLTM2XLSX;
                    }
                    else if (0 == sExt2.compare(_T(".xlsm")))		res = TCD_XLTM2XLSM;
                }break;
                case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSB:
                {
                    if (0 == sExt2.compare(_T(".xlst")))		res = TCD_XLSB2XLST;
                }
                case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX:
                case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM:
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX:
                case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX:
                case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM:
				case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM:
					{
							 if (0 == sExt2.compare(_T(".bin")))		res = TCD_PPTX2PPTT_BIN;
						else if (0 == sExt2.compare(_T(".pptt")))		res = TCD_PPTX2PPTT;
						else if (0 == sExt2.compare(_T(".pptx")))
						{
							if (OfficeFileFormatChecker.nFileType == AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX)
								res = TCD_PPSX2PPTX;
							if (OfficeFileFormatChecker.nFileType == AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX)
								res = TCD_POTX2PPTX;
							if (OfficeFileFormatChecker.nFileType == AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM)
								res = TCD_PPTM2PPTX;
							if (OfficeFileFormatChecker.nFileType == AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM)
								res = TCD_POTM2PPTX;
							if (OfficeFileFormatChecker.nFileType == AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM)
								res = TCD_PPSM2PPTX;
						}
						else if (0 == sExt2.compare(_T(".pptm")))
						{
							if (OfficeFileFormatChecker.nFileType == AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM)
								res = TCD_PPSM2PPTM;
							if (OfficeFileFormatChecker.nFileType == AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM)
								res = TCD_POTM2PPTM;
						}
                        else if (0 == sExt2.compare(_T(".odp")))		res = TCD_PPTX2ODP;
					}break;
				case AVS_OFFICESTUDIO_FILE_TEAMLAB_DOCY:
					{
							 if (0 == sExt2.compare(_T(".oform")))		res = TCD_DOCT2OFORM;
						else if (0 == sExt2.compare(_T(".docxf")))		res = TCD_DOCT2DOCXF;
						else if (0 == sExt2.compare(_T(".docx")))		res = TCD_DOCT2DOCX;
						else if (0 == sExt2.compare(_T(".docm")))		res = TCD_DOCT2DOCM;
						else if (0 == sExt2.compare(_T(".dotx")))		res = TCD_DOCT2DOTX;
						else if (0 == sExt2.compare(_T(".bin")))		res = TCD_T2BIN;
						else if (0 == sExt2.compare(_T(".rtf")))		res = TCD_DOCT2RTF;
				}break;
				case AVS_OFFICESTUDIO_FILE_TEAMLAB_XLSY:
					{
							 if (0 == sExt2.compare(_T(".xlsx")))		res = TCD_XLST2XLSX;
						else if (0 == sExt2.compare(_T(".xlsm")))		res = TCD_XLST2XLSM;
						else if (0 == sExt2.compare(_T(".xltx")))		res = TCD_XLST2XLTX;
						else if (0 == sExt2.compare(_T(".bin")))		res = TCD_T2BIN;
						else if (0 == sExt2.compare(_T(".csv")))		res = TCD_XLST2CSV;
					}break;
				case AVS_OFFICESTUDIO_FILE_TEAMLAB_PPTY:
					{
							 if (0 == sExt2.compare(_T(".pptx")))		res = TCD_PPTT2PPTX;	
						else if (0 == sExt2.compare(_T(".pptm")))		res = TCD_PPTT2PPTM;
						else if (0 == sExt2.compare(_T(".potx")))		res = TCD_PPTT2POTX;
						else if (0 == sExt2.compare(_T(".bin")))		res = TCD_T2BIN;
					}break;
				case AVS_OFFICESTUDIO_FILE_CANVAS_WORD:
					{
							 if (0 == sExt2.compare(_T(".docx")))		res = TCD_DOCT_BIN2DOCX;
						else if (0 == sExt2.compare(_T(".docm")))		res = TCD_DOCT_BIN2DOCX;
						else if (0 == sExt2.compare(_T(".oform")))		res = TCD_DOCT_BIN2DOCX;
						else if (0 == sExt2.compare(_T(".dotx")))		res = TCD_DOCT_BIN2DOCX;
						else if (0 == sExt2.compare(_T(".doct")))		res = TCD_BIN2T;
						else if (0 == sExt2.compare(_T(".rtf")))		res = TCD_DOCT_BIN2RTF;
					}break;
				case AVS_OFFICESTUDIO_FILE_CANVAS_SPREADSHEET:
					{
							 if (0 == sExt2.compare(_T(".xlsx")))		res = TCD_XLST_BIN2XLSX;
						else if (0 == sExt2.compare(_T(".xlsm")))		res = TCD_XLST_BIN2XLSX;
						else if (0 == sExt2.compare(_T(".xltx")))		res = TCD_XLST_BIN2XLSX;
						else if (0 == sExt2.compare(_T(".xlst")))		res = TCD_BIN2T;
						else if (0 == sExt2.compare(_T(".csv")))		res = TCD_XLST_BIN2CSV;
					}break;
				case AVS_OFFICESTUDIO_FILE_CANVAS_PRESENTATION:
					{
							 if (0 == sExt2.compare(_T(".pptx")))		res = TCD_PPTT_BIN2PPTX;
						else if (0 == sExt2.compare(_T(".pptm")))		res = TCD_PPTT_BIN2PPTX;
						else if (0 == sExt2.compare(_T(".potx")))		res = TCD_PPTT_BIN2PPTX;
						else if (0 == sExt2.compare(_T(".pptt")))		res = TCD_BIN2T;
					}break;
				case AVS_OFFICESTUDIO_FILE_CANVAS_PDF:
					{
						if (0 == sExt2.compare(_T(".pdf")))				res = TCD_BIN2PDF;
					}break;
				case AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV:
					{
							 if (0 == sExt2.compare(_T(".xlsx")))		res = TCD_CSV2XLSX;
						else if (0 == sExt2.compare(_T(".xlsm")))		res = TCD_CSV2XLSX;
						else if (0 == sExt2.compare(_T(".xlst")))		res = TCD_CSV2XLST;
						else if (0 == sExt2.compare(_T(".bin")))		res = TCD_CSV2XLST_BIN;
					}break;
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF:
					{
							 if (0 == sExt2.compare(_T(".docx")))		res = TCD_RTF2DOCX;
						else if (0 == sExt2.compare(_T(".docm")))		res = TCD_RTF2DOCX;
						else if (0 == sExt2.compare(_T(".doct")))		res = TCD_RTF2DOCT;
						else if (0 == sExt2.compare(_T(".bin")))		res = TCD_RTF2DOCT_BIN;
					}break;
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC:
                case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC_FLAT:
                    {
							 if (0 == sExt2.compare(_T(".docx")))		res = TCD_DOC2DOCX;
						else if (0 == sExt2.compare(_T(".docm")))		res = TCD_DOC2DOCM;
						else if (0 == sExt2.compare(_T(".doct")))		res = TCD_DOC2DOCT;
						else if (0 == sExt2.compare(_T(".bin")))		res = TCD_DOC2DOCT_BIN;
					}break;
                case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS:
                    {
                             if (0 == sExt2.compare(_T(".xlsx")))		res = TCD_XLS2XLSX;
                        else if (0 == sExt2.compare(_T(".xlsm")))		res = TCD_XLS2XLSM;
                        else if (0 == sExt2.compare(_T(".xlst")))		res = TCD_XLS2XLST;
                        else if (0 == sExt2.compare(_T(".bin")))		res = TCD_XLS2XLST_BIN;
                    }break;
                case AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT:
					{
							 if (0 == sExt2.compare(_T(".docx")))		res = TCD_TXT2DOCX;
						else if (0 == sExt2.compare(_T(".docm")))		res = TCD_TXT2DOCX;
						else if (0 == sExt2.compare(_T(".doct")))		res = TCD_TXT2DOCT;
						else if (0 == sExt2.compare(_T(".bin")))		res = TCD_TXT2DOCT_BIN;
					}break;
                case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT:
                    {//pot,pps - by extension - potx(potm), ppsx(ppsm)
                             if (0 == sExt2.compare(_T(".pptx")))		res = TCD_PPT2PPTX;
                        else if (0 == sExt2.compare(_T(".pptm")))		res = TCD_PPT2PPTM;
                        else if (0 == sExt2.compare(_T(".bin")))		res = TCD_PPT2PPTT_BIN;
                        else if (0 == sExt2.compare(_T(".pptt")))		res = TCD_PPT2PPTT;
                    }break;
                case AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT:
                case AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS:
                case AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP:
                case AVS_OFFICESTUDIO_FILE_DOCUMENT_OTT:
                case AVS_OFFICESTUDIO_FILE_SPREADSHEET_OTS:
                case AVS_OFFICESTUDIO_FILE_PRESENTATION_OTP:
                   {
                             if (0 == sExt2.compare(_T(".bin")))		res = TCD_ODF2OOT_BIN;
                        else if (0 == sExt2.compare(_T(".doct")) ||
                                 0 == sExt2.compare(_T(".xlst")) ||
                                 0 == sExt2.compare(_T(".pptt")))		res = TCD_ODF2OOT;
                        else if (0 == sExt2.compare(_T(".docx")) ||
                                 0 == sExt2.compare(_T(".xlsx")) ||
                                 0 == sExt2.compare(_T(".pptx")))		res = TCD_ODF2OOX;
                        else if (0 == sExt2.compare(_T(".docm")) ||
                                 0 == sExt2.compare(_T(".xlsm")) ||
                                 0 == sExt2.compare(_T(".pptm")))		res = TCD_ODF2OOX;
						else if (0 == sExt2.compare(_T(".odt")) 
							&& type == AVS_OFFICESTUDIO_FILE_DOCUMENT_OTT)		res = TCD_OTF2ODF;
 						else if (0 == sExt2.compare(_T(".ods")) 
							&& type == AVS_OFFICESTUDIO_FILE_SPREADSHEET_OTS)	res = TCD_OTF2ODF;
						else if (0 == sExt2.compare(_T(".odp")) 
							&& type == AVS_OFFICESTUDIO_FILE_PRESENTATION_OTP)	res = TCD_OTF2ODF;
                  }break;
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT_FLAT:
                case AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS_FLAT:
                case AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP_FLAT:
                   {
                             if (0 == sExt2.compare(_T(".bin")))		res = TCD_ODF_FLAT2OOT_BIN;
                        else if (0 == sExt2.compare(_T(".doct")) ||
                                 0 == sExt2.compare(_T(".xlst")) ||
                                 0 == sExt2.compare(_T(".pptt")))		res = TCD_ODF_FLAT2OOT;
                        else if (0 == sExt2.compare(_T(".docx")) ||
                                 0 == sExt2.compare(_T(".xlsx")) ||
                                 0 == sExt2.compare(_T(".pptx")))		res = TCD_ODF_FLAT2OOX;
                        else if (0 == sExt2.compare(_T(".docm")) ||
                                 0 == sExt2.compare(_T(".xlsm")) ||
                                 0 == sExt2.compare(_T(".pptm")))		res = TCD_ODF_FLAT2OOX;
                    }break;
				case AVS_OFFICESTUDIO_FILE_OTHER_MS_VBAPROJECT:
					{
						res = TCD_VBAPROJECT2XML;
					}break;
				case AVS_OFFICESTUDIO_FILE_OTHER_MS_OFFCRYPTO:
					{
						if (0 == sExt2.compare(_T(".doct")))			res = TCD_MSCRYPT2DOCT;
						else if (0 == sExt2.compare(_T(".xlst")))		res = TCD_MSCRYPT2XLST;
						else if (0 == sExt2.compare(_T(".pptt")))		res = TCD_MSCRYPT2PPTT;
						else if (0 == sExt2.compare(_T(".bin")))		res = TCD_MSCRYPT2BIN;
					}break;
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML_IN_CONTAINER:
					{
							 if (0 == sExt2.compare(_T(".docx")))		res = TCD_HTMLZIP2DOCX;
						else if (0 == sExt2.compare(_T(".doct")))		res = TCD_HTMLZIP2DOCT;
						else if (0 == sExt2.compare(_T(".bin")))		res = TCD_HTMLZIP2DOCT_BIN;
					}break;
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML:
					{
							 if (0 == sExt2.compare(_T(".docx")))		res = TCD_HTML2DOCX;
						else if (0 == sExt2.compare(_T(".doct")))		res = TCD_HTML2DOCT;
						else if (0 == sExt2.compare(_T(".bin")))		res = TCD_HTML2DOCT_BIN;
					}break;
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2:
					{
							 if (0 == sExt2.compare(_T(".docx")))		res = TCD_FB22DOCX;
						else if (0 == sExt2.compare(_T(".doct")))		res = TCD_FB22DOCT;
						else if (0 == sExt2.compare(_T(".bin")))		res = TCD_FB22DOCT_BIN;
					}break;
				case AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB:
					{
							 if (0 == sExt2.compare(_T(".docx")))		res = TCD_EPUB2DOCX;
						else if (0 == sExt2.compare(_T(".doct")))		res = TCD_EPUB2DOCT;
						else if (0 == sExt2.compare(_T(".bin")))		res = TCD_EPUB2DOCT_BIN;
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
                            const std::wstring& sTFileSrc, const std::wstring& sPdfBinFile, const std::wstring& sImagesDirectory,
                            const std::wstring& sThemeDir, int nTopIndex, const std::wstring& sMailMerge, const InputParams& params)
    {
        NSStringUtils::CStringBuilder oBuilder;
        oBuilder.WriteString(_T("<Settings><SrcFileType>"));
        oBuilder.AddInt((int)eFromType);
        oBuilder.WriteString(_T("</SrcFileType><DstFileType>"));
        oBuilder.AddInt((int)eToType);
        oBuilder.WriteString(_T("</DstFileType><SrcFilePath>"));
        oBuilder.WriteEncodeXmlString(sTFileSrc.c_str());
        oBuilder.WriteString(_T("</SrcFilePath><DstFilePath>"));
        oBuilder.WriteEncodeXmlString(sPdfBinFile.c_str());
        oBuilder.WriteString(_T("</DstFilePath><FontsDirectory>"));
        oBuilder.WriteEncodeXmlString(params.getFontPath().c_str());
        oBuilder.WriteString(_T("</FontsDirectory><ImagesDirectory>"));
        oBuilder.WriteEncodeXmlString(sImagesDirectory.c_str());
        oBuilder.WriteString(_T("</ImagesDirectory><ThemesDirectory>"));
        oBuilder.WriteEncodeXmlString(sThemeDir.c_str());
        oBuilder.WriteString(_T("</ThemesDirectory>"));
		if(NULL != params.m_nLcid)
		{
			oBuilder.WriteString(_T("<Lcid>"));
			oBuilder.AddInt(*params.m_nLcid);
			oBuilder.WriteString(_T("</Lcid>"));
		}
		std::wstring sJsonParams;
		bool bOnlyOnePage = NULL != params.m_oThumbnail && (NULL == params.m_oThumbnail->first || true == *params.m_oThumbnail->first);
		if (NULL != params.m_sJsonParams)
		{
			sJsonParams = *params.m_sJsonParams;
			if (bOnlyOnePage){
				sJsonParams.insert(1, L"\"printOptions\":{\"onlyFirstPage\":true},");
			}
		}
		else if (bOnlyOnePage)
		{
			sJsonParams = L"{\"printOptions\":{\"onlyFirstPage\":true}}";
		}
		if (!sJsonParams.empty())
		{
			oBuilder.WriteString(_T("<JsonParams>"));
			oBuilder.WriteEncodeXmlString(sJsonParams);
			oBuilder.WriteString(_T("</JsonParams>"));
		}
		if (NULL != params.m_sScriptsCacheDirectory)
		{
			oBuilder.WriteString(_T("<ScriptsCacheDirectory>"));
			oBuilder.WriteEncodeXmlString(*params.m_sScriptsCacheDirectory);
			oBuilder.WriteString(_T("</ScriptsCacheDirectory>"));
		}
        oBuilder.WriteString(_T("<Changes TopItem=\""));
        oBuilder.AddInt(nTopIndex);
        oBuilder.WriteString(_T("\">"));
        std::wstring sChangesDir = NSDirectory::GetFolderPath(sTFileSrc) + FILE_SEPARATOR_STR + _T("changes");
        if (NSDirectory::Exists(sChangesDir))
        {
            std::vector<std::wstring> aChangesFiles;
            std::vector<std::wstring> oArray;
            std::vector<std::wstring> aFiles;

            NSDirectory::GetFiles2(sChangesDir, oArray, false);

            for(size_t i = 0; i < oArray.size(); ++i)
            {
                aChangesFiles.push_back(oArray[i]);
            }
            std::sort (aChangesFiles.begin(), aChangesFiles.end(), compare_string_by_length);

            for(size_t i = 0; i < aChangesFiles.size(); ++i)
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
    _UINT32 apply_changes(const std::wstring &sBinFrom, const std::wstring &sToResult, NSDoctRenderer::DoctRendererFormat::FormatFile eType, const std::wstring &sThemeDir, std::wstring &sBinTo, const InputParams& params)
	{
        std::wstring sBinDir = NSDirectory::GetFolderPath(sBinFrom);
		std::wstring sChangesDir = sBinDir + FILE_SEPARATOR_STR + _T("changes");
        if (NSDirectory::Exists(sChangesDir))
        {
			std::wstring sBinFromFileName = NSFile::GetFileName(sBinFrom);
			std::wstring sBinFromExt = NSFile::GetFileExtention(sBinFromFileName);
			sBinTo = sBinDir + FILE_SEPARATOR_STR + sBinFromFileName.substr(0, sBinFromFileName.length() - sBinFromExt.length() - 1) + _T("WithChanges.") + sBinFromExt;
			std::wstring sImagesDirectory = sBinDir + FILE_SEPARATOR_STR + _T("media");
           
			NSDoctRenderer::CDoctrenderer oDoctRenderer(NULL != params.m_sAllFontsPath ? *params.m_sAllFontsPath : _T(""));
            int nChangeIndex = -1;
            while (true)
            {
                std::wstring sXml = getDoctXml(eType, eType, sBinFrom, sBinTo, sImagesDirectory, sThemeDir, nChangeIndex, _T(""), params);
				std::wstring sResult;
                oDoctRenderer.Execute(sXml, sResult);
                bool bContinue = false;
                if (!sResult.empty() && -1 != sResult.find(_T("error")))
                {
                    std::wcerr << _T("DoctRenderer:") << sResult << std::endl;
					params.m_bOutputConvertCorrupted = true;
                    int nErrorIndex = -1;
                    int nErrorIndexStart = sResult.find(_T("index"));
                    if (-1 != nErrorIndexStart)
                    {
                        nErrorIndexStart = sResult.find(_T("\""), nErrorIndexStart + 1);
                        int nErrorIndexEnd = sResult.find(_T("\""), nErrorIndexStart + 1);
                        nErrorIndex = XmlUtils::GetInteger(sResult.substr(nErrorIndexStart + 1, nErrorIndexEnd - nErrorIndexStart - 1));
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
                        for(size_t i = 0; i < aImages.size(); ++i)
                        {
                            std::wstring sImageName = aImages[i];
                            std::wstring sImage = sImagesDirectory + FILE_SEPARATOR_STR + sImageName;
                            std::wstring sImageCopy = sChangesDir + FILE_SEPARATOR_STR + sImageName;
                            NSFile::CFileBinary::Copy(sImage, sImageCopy);
                        }
						////copy doct for version history
						// std::wstring sBinCopy = sChangesDir + FILE_SEPARATOR_STR + NSSystemPath::GetFileName(sBinFrom);
						// NSFile::CFileBinary::Copy(sBinFrom, sBinCopy);

                        std::wstring sToResultDir = NSDirectory::GetFolderPath(sToResult);
                        std::wstring sTo = sToResultDir + FILE_SEPARATOR_STR + _T("changes.zip");
                        COfficeUtils oCOfficeUtils(NULL);
                        oCOfficeUtils.CompressFileOrDirectory(sChangesDir, sTo);
                    }
                    break;
                }
            }
        }
		else
			sBinTo = sBinFrom;
		return 0;
    }
	bool InputParams::checkInputLimits()
	{
		std::wstring& sFrom = *this->m_sFileFrom;
		int nFormatFrom = *this->m_nFormatFrom;
		boost::unordered_map<int, std::vector<InputLimit>>::const_iterator itLimit = this->m_mapInputLimits.find(nFormatFrom);
		if(itLimit != this->m_mapInputLimits.end())
		{
			for (size_t i = 0; i < itLimit->second.size(); ++i)
			{
				const InputLimit& oLimit = itLimit->second[i];
				if(oLimit.compressed > 0 || oLimit.uncompressed > 0)
				{
					ULONG64 nCompressed = 0;
					ULONG64 nUncompressed = 0;
					COfficeUtils oCOfficeUtils(NULL);
					oCOfficeUtils.GetFilesSize(sFrom, oLimit.pattern, nCompressed, nUncompressed);
					if((oLimit.compressed > 0 && nCompressed > oLimit.compressed) || (oLimit.uncompressed > 0 && nUncompressed > oLimit.uncompressed))
					{
						return false;
					}
				}
			}
		}
		return true;
	}
}
