/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include "lib/crypt.h"

#include "lib/docx.h"
#include "lib/xlsx.h"
#include "lib/pptx.h"

#include "lib/doc.h"

#include "lib/rtf.h"
#include "lib/txt.h"

#include "lib/ppt.h"

#include "lib/xls.h"
#include "lib/csv.h"

#include "lib/html.h"

#include "lib/odf.h"

#include "lib/pdf_image.h"
#include "lib/pdf_oform.h"

#include "lib/iwork.h"

#include "lib/hwp.h"

#include "../../DesktopEditor/doctrenderer/docbuilder.h"
#include "../../MsBinaryFile/Common/Vba/VbaReader.h"

namespace NExtractTools
{
	// vba
	_UINT32 msVbaProject2Xml(const std::wstring &sFrom, const std::wstring &sTo, InputParams &params, ConvertParams& convertParams)
	{
		CVbaReader vbaReader(sFrom, sTo);

		if (false == vbaReader.write())
			return AVS_FILEUTILS_ERROR_CONVERT;

		return 0;
	}

	// detect macroses
	_UINT32 detectMacroInFile(InputParams& oInputParams)
	{
		_UINT32 nRes = 0; // no macro
		std::wstring sFileFrom = *oInputParams.m_sFileFrom;

		COfficeFileFormatChecker OfficeFileFormatChecker;

		if (OfficeFileFormatChecker.isOfficeFile(sFileFrom))
		{
			if (OfficeFileFormatChecker.bMacroEnabled)
			{
				nRes = AVS_ERROR_MACRO;
			}
		}

		return nRes;
	}

	// create js cache (for builder)
	void createJSCaches()
	{
		NSDoctRenderer::CDocBuilder::Initialize();

		NSDoctRenderer::CDoctrenderer oDoctRenderer;
		oDoctRenderer.CreateCache(L"", L"");

		NSDoctRenderer::CDocBuilder::Dispose();
	}

	void createJSSnapshots()
	{
		NSDoctRenderer::CDocBuilder::Initialize();

		NSDoctRenderer::CDoctrenderer oDoctRenderer;
		oDoctRenderer.CreateSnapshots();

		NSDoctRenderer::CDocBuilder::Dispose();
	}

	// mailmerge
	_UINT32 convertmailmerge(const InputParamsMailMerge& oMailMergeSend,
							 const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		if (NULL == oMailMergeSend.mailFormat || NULL == oMailMergeSend.recordFrom || NULL == oMailMergeSend.recordTo)
			return AVS_FILEUTILS_ERROR_CONVERT;

		_UINT32 nRes = 0;

		std::wstring sFileFromDir = NSSystemPath::GetDirectoryName(sFrom);
		std::wstring sFileToDir = NSSystemPath::GetDirectoryName(sTo);
		std::wstring sImagesDirectory = sFileFromDir + FILE_SEPARATOR_STR + _T("media");
		NSDoctRenderer::DoctRendererFormat::FormatFile eTypeTo;
		switch (*oMailMergeSend.mailFormat)
		{
		case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX:
		case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM:
		case AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM:
		case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCXF:
			eTypeTo = NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT;
			break;
		case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF:
			eTypeTo = NSDoctRenderer::DoctRendererFormat::FormatFile::PDF;
			break;
		case AVS_OFFICESTUDIO_FILE_OTHER_HTMLZIP:
			eTypeTo = NSDoctRenderer::DoctRendererFormat::FormatFile::HTML;
			break;
		default:
			eTypeTo = NSDoctRenderer::DoctRendererFormat::FormatFile::HTML;
			break;
		}
		std::wstring sJsonPath = sFileFromDir + FILE_SEPARATOR_STR + _T("Editor.json");
		int recordTo = *oMailMergeSend.recordFrom + 4;
		if (recordTo > *oMailMergeSend.recordTo)
			recordTo = *oMailMergeSend.recordTo;
		NSDoctRenderer::CDoctrenderer oDoctRenderer(NULL != params.m_sAllFontsPath ? *params.m_sAllFontsPath : _T(""));
		std::wstring sMailMergeXml = getMailMergeXml(sJsonPath, *oMailMergeSend.recordFrom, recordTo, *oMailMergeSend.to);
		// посылаем выходную папку sFileFromDir, чтобы файлы лежали на одном уровне с папкой media, важно для дальнейшей конвертации в docx, pdf
		std::wstring sXml = getDoctXml(NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT, eTypeTo, sFrom, sFileFromDir, sImagesDirectory, convertParams.m_sThemesDir, -1, sMailMergeXml, params);
		std::wstring sResult;
		oDoctRenderer.Execute(sXml, sResult);
		if (-1 != sResult.find(_T("error")))
		{
			std::wcerr << _T("DoctRenderer:") << sResult << std::endl;
			nRes = AVS_FILEUTILS_ERROR_CONVERT;
		}
		else
		{
			nRes = AVS_FILEUTILS_ERROR_CONVERT;
			XmlUtils::CXmlNode oRoot;
			if (TRUE == oRoot.FromXmlString(sResult))
			{
				XmlUtils::CXmlNode oMailMergeFields = oRoot.ReadNode(_T("MailMergeFields"));
				std::vector<XmlUtils::CXmlNode> oXmlNodes;
				if (TRUE == oMailMergeFields.GetChilds(oXmlNodes))
				{
					for (size_t i = 0; i < oXmlNodes.size(); ++i)
					{
						XmlUtils::CXmlNode &oXmlNode = oXmlNodes[i];
						if (oXmlNode.IsValid())
						{
							std::wstring sFilePathIn = oXmlNode.GetAttribute(L"path");
							std::wstring wsFilePathInFilename = NSSystemPath::GetFileName(sFilePathIn);
							std::wstring sFilePathOut = sFileToDir + FILE_SEPARATOR_STR + wsFilePathInFilename;
							if (NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT == eTypeTo)
							{
								sFilePathOut += L".docx";

								std::wstring sTempDocx = convertParams.m_sTempDir + FILE_SEPARATOR_STR + wsFilePathInFilename + L"_DOCX";
								NSDirectory::CreateDirectory(sTempDocx);

								BinDocxRW::CDocxSerializer m_oCDocxSerializer;
								m_oCDocxSerializer.setIsNoBase64(params.getIsNoBase64());
								m_oCDocxSerializer.setFontDir(params.getFontPath());

								std::wstring sXmlOptions;
								std::wstring sThemePath; // will be filled by 'CreateDocxFolders' method
								std::wstring sMediaPath; // will be filled by 'CreateDocxFolders' method
								std::wstring sEmbedPath; // will be filled by 'CreateDocxFolders' method

								m_oCDocxSerializer.CreateDocxFolders(sTempDocx, sThemePath, sMediaPath, sEmbedPath);
								nRes = m_oCDocxSerializer.loadFromFile(sFilePathIn, sTempDocx, sXmlOptions, sThemePath, sMediaPath, sEmbedPath) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
								if (SUCCEEDED_X2T(nRes))
								{
									std::wstring sTempUnencrypted = convertParams.m_sTempDir + FILE_SEPARATOR_STR + wsFilePathInFilename + L"_unencrypted";
									NSDirectory::CreateDirectory(sTempUnencrypted);

									std::wstring sOldTempDir = convertParams.m_sTempDir;
									convertParams.m_sTempDir = sTempUnencrypted;
									nRes = dir2zipMscrypt(sTempDocx, sFilePathOut, params, convertParams);
									convertParams.m_sTempDir = sOldTempDir;
								}
							}
							else if (NSDoctRenderer::DoctRendererFormat::FormatFile::PDF == eTypeTo)
							{
								sFilePathOut += _T(".pdf");

								NSFonts::IApplicationFonts* pApplicationFonts = createApplicationFonts(params);
								CPdfFile pdfWriter(pApplicationFonts);
								pdfWriter.CreatePdf(params.getIsPDFA());
								pdfWriter.SetTempDirectory(convertParams.m_sTempDir);
								pdfWriter.SetDocumentInfo(params.getTitle(), L"", L"", L"");

								CConvertFromBinParams oBufferParams;
								oBufferParams.m_sThemesDirectory = convertParams.m_sThemesDir;

								std::wstring documentID = params.getDocumentID();
								if (false == documentID.empty())
									pdfWriter.SetDocumentID(documentID);

								std::wstring password = params.getSavePassword();
								if (false == password.empty())
									pdfWriter.SetPassword(password);

								int nReg = (convertParams.m_bIsPaid == false) ? 0 : 1;
								nRes = (S_OK == pdfWriter.OnlineWordToPdfFromBinary(sFilePathIn, sFilePathOut, &oBufferParams)) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
								RELEASEOBJECT(pApplicationFonts);
							}
							else if (NSDoctRenderer::DoctRendererFormat::FormatFile::HTML == eTypeTo)
							{
								sFilePathOut += _T(".html");
								nRes = NSFile::CFileBinary::Copy(sFilePathIn, sFilePathOut) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
							}
							if (0 != nRes)
								break;
							sResult = string_replaceAll(sResult, L"\"" + sFilePathIn + L"\"", L"\"" + NSSystemPath::GetFileName(sFilePathOut) + L"\"");
						}
					}
				}
			}
			if (SUCCEEDED_X2T(nRes))
				NSFile::CFileBinary::SaveToFile(sTo, sResult);
		}
		return nRes;
	}

	// from docxDir
	_UINT32 fromDocxDir(const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, InputParams &params, ConvertParams& convertParams)
	{
		_UINT32 nRes = 0;
		std::wstring sFromWithChanges = sFrom;
		bool bIsNeedDoct = false;
		switch (nFormatTo)
		{
		// перечислить все "документные" форматы, которым нужна конвертация через doct
		case AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML:
		case AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML_IN_CONTAINER:
		case AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2:
		case AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB:
		case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF:
			bIsNeedDoct = true;
			break;
		default:
			if (AVS_OFFICESTUDIO_FILE_IMAGE & nFormatTo)
				bIsNeedDoct = true;
			break;
		}

		if (0 != (AVS_OFFICESTUDIO_FILE_DOCUMENT & nFormatTo) && !bIsNeedDoct)
		{
			if (AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX == nFormatTo ||
				AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM == nFormatTo ||
				AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX == nFormatTo ||
				AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM == nFormatTo ||
				AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM == nFormatTo ||
				AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCXF == nFormatTo)
			{
				if (AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM == nFormatTo)
				{
					std::wstring sCT = L"<Default Extension=\"oform\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.document.oform\"/>";
					nRes = addContentType(sFromWithChanges, sCT);
				}
				else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCXF == nFormatTo)
				{
					std::wstring sCT = L"<Default Extension=\"docxf\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.document.docxf\"/>";
					nRes = addContentType(sFromWithChanges, sCT);
				}
				else
				{
					if ((params.m_nFormatFrom) && (AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM == *params.m_nFormatFrom))
					{
						std::wstring sCT = L"<Default Extension=\"oform\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.document.oform\"/>";
						replaceContentType(sFromWithChanges, sCT, L"");
					}
					else if ((params.m_nFormatFrom) && (AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCXF == *params.m_nFormatFrom))
					{
						std::wstring sCT = L"<Default Extension=\"docxf\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.document.docxf\"/>";
						replaceContentType(sFromWithChanges, sCT, L"");
					}
					if (AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM == nFormatTo ||
						AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX == nFormatTo ||
						AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM == nFormatTo)
					{
						std::wstring sCTFrom = L"application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml";
						switch (*params.m_nFormatFrom)
						{
						case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM:
							sCTFrom = L"application/vnd.ms-word.document.macroEnabled.main+xml";
							break;
						case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX:
							sCTFrom = L"application/vnd.openxmlformats-officedocument.wordprocessingml.template.main+xml";
							break;
						case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM:
							sCTFrom = L"application/vnd.ms-word.template.macroEnabledTemplate.main+xml";
							break;
						}
						std::wstring sCTTo;
						switch (nFormatTo)
						{
						case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM:
							sCTTo = L"application/vnd.ms-word.document.macroEnabled.main+xml";
							break;
						case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX:
							sCTTo = L"application/vnd.openxmlformats-officedocument.wordprocessingml.template.main+xml";
							break;
						case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM:
							sCTTo = L"application/vnd.ms-word.template.macroEnabledTemplate.main+xml";
							break;
						}
						nRes = replaceContentType(sFromWithChanges, sCTFrom, sCTTo);
					}
				}
				if (SUCCEEDED_X2T(nRes))
				{
					nRes = dir2zipMscrypt(sFromWithChanges, sTo, params, convertParams);
				}
			}
			else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM_PDF == nFormatTo)
			{
				nRes = docx_dir2pdfoform(sFromWithChanges, sTo, params, convertParams);
			}
			else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC == nFormatTo)
			{
				nRes = docx_dir2doc(sFromWithChanges, sTo, params, convertParams);
			}
			else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT == nFormatTo)
			{
				convertParams.m_bIsTemplate = false;
				nRes = docx_dir2odt(sFromWithChanges, sTo, params, convertParams);
			}
			else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_OTT == nFormatTo)
			{
				convertParams.m_bIsTemplate = true;
				nRes = docx_dir2odt(sFromWithChanges, sTo, params, convertParams);
			}
			else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF == nFormatTo)
			{
				nRes = docx_dir2rtf(sFromWithChanges, sTo, params, convertParams);
			}
			else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT == nFormatTo)
			{
				nRes = docx_dir2txt(sFromWithChanges, sTo, params, convertParams);
			}
			else
				nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
		}
		else if (AVS_OFFICESTUDIO_FILE_OTHER_OOXML == nFormatTo)
		{
			nRes = dir2zipMscrypt(sFrom, sTo, params, convertParams);
		}
		else if (AVS_OFFICESTUDIO_FILE_CANVAS_WORD == nFormatTo)
		{
			nRes = docx_dir2doct_bin(sFromWithChanges, sTo, params, convertParams);
		}
		else if (AVS_OFFICESTUDIO_FILE_TEAMLAB_DOCY == nFormatTo)
		{
			nRes = docx_dir2doct(sFromWithChanges, sTo, params, convertParams);
		}
		else if (bIsNeedDoct)
		{
			if (params.needConvertToOrigin(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX))
			{
				std::wstring sToRender = convertParams.m_sTempParamOOXMLFile;
				if (sToRender.empty())
				{
					sToRender = combinePath(convertParams.m_sTempDir, L"toRender.docx");
					nRes = dir2zip(sFromWithChanges, sToRender);
				}
				NSDoctRenderer::DoctRendererFormat::FormatFile eFromType = NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT;
				convertParams.m_sInternalMediaDirectory = sFrom;
				if (AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB == nFormatTo)
				{
					nRes = doct_bin2epub(sToRender, sTo, params, convertParams);
				}
				else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2 == nFormatTo)
				{
					nRes = doct_bin2fb(sToRender, sTo, params, convertParams);
				}
				else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML == nFormatTo)
				{
					nRes = doct_bin2html(sToRender, sTo, params, convertParams);
				}
				else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML_IN_CONTAINER == nFormatTo)
				{
					nRes = doct_bin2html_zip(sToRender, sTo, params, convertParams);
				}
				else if (AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatTo)
				{
					nRes = doct_bin2pdf(eFromType, sToRender, sTo, params, convertParams);
				}
				else if (0 != (AVS_OFFICESTUDIO_FILE_IMAGE & nFormatTo))
				{
					nRes = doct_bin2image(eFromType, sToRender, sTo, params, convertParams);
				}
				else
					nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
			}
			else
			{
				std::wstring sDoctDir = combinePath(convertParams.m_sTempDir, L"doct_unpacked");
				NSDirectory::CreateDirectory(sDoctDir);
				std::wstring sTFile = combinePath(sDoctDir, L"Editor.bin");

				nRes = docx_dir2doct_bin(sFromWithChanges, sTFile, params, convertParams);

				if (SUCCEEDED_X2T(nRes))
				{
					nRes = fromDoctBin(sTFile, sTo, nFormatTo, params, convertParams);
				}
			}
		}
		else
			nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
		return nRes;
	}

	_UINT32 fromDoctBin(const std::wstring& sFrom, const std::wstring& sTo, int nFormatTo, InputParams& params, ConvertParams& convertParams)
	{
		_UINT32 nRes = 0;
		if (AVS_OFFICESTUDIO_FILE_TEAMLAB_DOCY == nFormatTo)
		{
			std::wstring sFromDir = NSDirectory::GetFolderPath(sFrom);
			nRes = dir2zip(sFromDir, sTo);
		}
		else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB == nFormatTo)
		{
			nRes = doct_bin2epub(sFrom, sTo, params, convertParams);
		}
		else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2 == nFormatTo)
		{
			nRes = doct_bin2fb(sFrom, sTo, params, convertParams);
		}
		else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML == nFormatTo)
		{
			nRes = doct_bin2html(sFrom, sTo, params, convertParams);
		}
		else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML_IN_CONTAINER == nFormatTo)
		{
			nRes = doct_bin2html_zip(sFrom, sTo, params, convertParams);
		}
		else if (AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatTo)
		{
			NSDoctRenderer::DoctRendererFormat::FormatFile eFromType = NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT;
			nRes = doct_bin2pdf(eFromType, sFrom, sTo, params, convertParams);
		}
		else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM_PDF == nFormatTo)
		{
			nRes = doct_bin2pdfoform(sFrom, sTo, params, convertParams);
		}
		else if (0 != (AVS_OFFICESTUDIO_FILE_IMAGE & nFormatTo))
		{
			NSDoctRenderer::DoctRendererFormat::FormatFile eFromType = NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT;
			nRes = doct_bin2image(eFromType, sFrom, sTo, params, convertParams);
		}
		else if (0 != (AVS_OFFICESTUDIO_FILE_DOCUMENT & nFormatTo) || 
						AVS_OFFICESTUDIO_FILE_OTHER_OOXML == nFormatTo ||
						AVS_OFFICESTUDIO_FILE_OTHER_ODF == nFormatTo)
		{
			std::wstring sDocxDir = combinePath(convertParams.m_sTempDir, L"docx_unpacked");
			if (true == NSDirectory::CreateDirectory(sDocxDir))
			{
				params.m_bMacro = AVS_OFFICESTUDIO_FILE_OTHER_OOXML == nFormatTo || AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM == nFormatTo || AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM == nFormatTo;

				convertParams.m_sTempResultOOXMLDirectory = sDocxDir;
				nRes = doct_bin2docx_dir(sFrom, sTo, params, convertParams);
				if (SUCCEEDED_X2T(nRes))
				{
					std::wstring sFileToCurrent = *params.m_sFileTo;
					params.changeFormatFromPost(*params.m_nFormatFrom, params.m_bMacro);

					if (NULL != params.m_nFormatTo)
						nFormatTo = *params.m_nFormatTo;

					nRes = fromDocxDir(sDocxDir, *params.m_sFileTo, nFormatTo, params, convertParams);
				}
			}
			else
			{
				nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
			}
		}
		else
			nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
		return nRes;
	}
	_UINT32 fromDocument(const std::wstring& sFrom, int nFormatFrom, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sTo = *params.m_sFileTo;
		int nFormatTo = AVS_OFFICESTUDIO_FILE_UNKNOWN;
		if (NULL != params.m_nFormatTo)
			nFormatTo = *params.m_nFormatTo;

		_UINT32 nRes = 0;
		if (AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX_FLAT == nFormatFrom)
		{
			if (AVS_OFFICESTUDIO_FILE_CANVAS_WORD == nFormatTo)
			{
				nRes = docxflat2doct_bin(sFrom, sTo, params, convertParams);
			}
			else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT == nFormatTo)
			{
				nRes = docxflat2odt(sFrom, sTo, params, convertParams);
			}
			else if (	AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX == nFormatTo || 
						AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM == nFormatTo || 
						AVS_OFFICESTUDIO_FILE_OTHER_OOXML == nFormatTo)
			{
				nRes = docxflat2docx(sFrom, sTo, params, convertParams);
			}
			else
			{
				std::wstring sDoctDir = combinePath(convertParams.m_sTempDir, L"doct_unpacked");
				NSDirectory::CreateDirectory(sDoctDir);
				std::wstring sTFile = combinePath(sDoctDir, L"Editor.bin");

				if (AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX_FLAT == nFormatFrom)
					nRes = docxflat2doct_bin(sFrom, sTFile, params, convertParams);
				else
					nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;

				if (SUCCEEDED_X2T(nRes))
				{
					nRes = fromDoctBin(sTFile, sTo, nFormatTo, params, convertParams);
				}
			}
		}
		else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_PAGES == nFormatFrom)
		{
			std::wstring wsTempFile = combinePath(convertParams.m_sTempDir, L"IntermediateFile.odf");

			int nIntermediateResult = pages2odf(sFrom, wsTempFile, params, convertParams);

			if (S_OK != nIntermediateResult)
				return nIntermediateResult;

			nRes = fromDocument(wsTempFile, AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT_FLAT, params, convertParams);
		}
		else
		{
			std::wstring sDocxDir = combinePath(convertParams.m_sTempDir, L"docx_unpacked");
			NSDirectory::CreateDirectory(sDocxDir);

			if (AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX == nFormatFrom ||
				AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM == nFormatFrom ||
				AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCXF == nFormatFrom)
			{
				convertParams.m_sTempParamOOXMLFile = sFrom;
				if (params.getFromChanges())
				{
					params.setFromChanges(false);
					nRes = apply_changes(sFrom, sTo, NSDoctRenderer::DoctRendererFormat::FormatFile::DOCT, convertParams.m_sTempParamOOXMLFile, params, convertParams);
				}
				nRes = zip2dir(convertParams.m_sTempParamOOXMLFile, sDocxDir);

				if (false == SUCCEEDED_X2T(nRes))
				{
					if (NSDirectory::GetFilesCount(sDocxDir, true) > 3)
					{
						nRes = 0;
					}
				}
			}
			else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM == nFormatFrom)
			{
				if (AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX == nFormatTo || AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX == nFormatTo || AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCXF == nFormatTo ||
					AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM == nFormatTo)
				{
					nRes = docm2docx_dir(sFrom, sDocxDir, params, convertParams);
				}
				else
				{
					nRes = zip2dir(sFrom, sDocxDir);
				}
				if (false == SUCCEEDED_X2T(nRes))
				{
					if (NSDirectory::GetFilesCount(sDocxDir, true) > 3)
					{
						nRes = 0;
					}
				}
			}
			else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX == nFormatFrom)
			{
				nRes = dotx2docx_dir(sFrom, sDocxDir, params, convertParams);
			}
			else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM == nFormatFrom)
			{
				if (AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX == nFormatTo || AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX == nFormatTo || AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCXF == nFormatTo ||
					AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM == nFormatTo)
				{
					nRes = dotm2docx_dir(sFrom, sDocxDir, params, convertParams);
				}
				else
				{
					nRes = dotm2docm_dir(sFrom, sDocxDir, params, convertParams);
				}
			}
			else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC == nFormatFrom || 
					AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC_FLAT == nFormatFrom)
			{
				if (params.m_bMacro)
				{
					nRes = doc2docm_dir(sFrom, sDocxDir, params, convertParams);
				}
				else
				{
					nRes = doc2docx_dir(sFrom, sDocxDir, params, convertParams);
				}
			}
			else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT == nFormatFrom || AVS_OFFICESTUDIO_FILE_DOCUMENT_OTT == nFormatFrom)
			{
				nRes = odf2oox_dir(sFrom, sDocxDir, params, convertParams);
			}
			else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT_FLAT == nFormatFrom)
			{
				nRes = odf_flat2oox_dir(sFrom, sDocxDir, params, convertParams);
			}
			else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF == nFormatFrom)
			{
				nRes = rtf2docx_dir(sFrom, sDocxDir, params, convertParams);
			}
			else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT == nFormatFrom || AVS_OFFICESTUDIO_FILE_DOCUMENT_XML == nFormatFrom)
			{
				nRes = txt2docx_dir(sFrom, sDocxDir, params, convertParams);
			}
			else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2 == nFormatFrom)
			{
				nRes = fb2docx_dir(sFrom, sDocxDir, params, convertParams);
			}
			else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB == nFormatFrom)
			{
				nRes = epub2docx_dir(sFrom, sDocxDir, params, convertParams);
			}
			else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML == nFormatFrom)
			{
				nRes = html2docx_dir(sFrom, sDocxDir, params, convertParams);
			}
			else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML_IN_CONTAINER == nFormatFrom)
			{
				nRes = html_zip2docx_dir(sFrom, sDocxDir, params, convertParams);
			}
			else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX_PACKAGE == nFormatFrom)
			{
				nRes = package2ooxml_dir(sFrom, sDocxDir, params, convertParams);
			}
			else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT == nFormatFrom)
			{
				nRes = mht2docx_dir(sFrom, sDocxDir, params, convertParams);
			}
			else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM_PDF == nFormatFrom)
			{
				nRes = pdfoform2docx_dir(sFrom, sDocxDir, params, convertParams);
			}
			else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_HWP == nFormatFrom)
			{
				nRes = hwp2docx_dir(sFrom, sDocxDir, params, convertParams);
			}
			else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_HWPX == nFormatFrom)
			{
				nRes = hwpx2docx_dir(sFrom, sDocxDir, params, convertParams);
			}
			else
				nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
			
			if (SUCCEEDED_X2T(nRes))
			{
				std::wstring sFileToCurrent = *params.m_sFileTo;
				params.changeFormatFromPost(*params.m_nFormatFrom, params.m_bMacro);

				if (NULL != params.m_nFormatTo)
					nFormatTo = *params.m_nFormatTo;

				nRes = fromDocxDir(sDocxDir, *params.m_sFileTo, nFormatTo, params, convertParams);
			}
		}
		return nRes;
	}

	// from xlsxDir
	_UINT32 fromXlsbXlsxDir(const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, InputParams &params, ConvertParams& convertParams)
	{
		_UINT32 nRes = S_OK;
		if (AVS_OFFICESTUDIO_FILE_OTHER_OOXML == nFormatTo)
		{
			nRes = dir2zipMscrypt(sFrom, sTo, params, convertParams);
		}
		else if (AVS_OFFICESTUDIO_FILE_OTHER_JSON == nFormatTo)
		{
			convertParams.m_bTempIsXmlOptions = true;
			nRes = xlsx_dir2xlst_bin(sFrom, sTo, params, convertParams);
		}
		else if (AVS_OFFICESTUDIO_FILE_CANVAS_SPREADSHEET == nFormatTo)
		{
			convertParams.m_bTempIsXmlOptions = true;
			nRes = xlsx_dir2xlst_bin(sFrom, sTo, params, convertParams);
		}
		else if (AVS_OFFICESTUDIO_FILE_TEAMLAB_XLSY == nFormatTo)
		{
			convertParams.m_bTempIsXmlOptions = true;
			nRes = xlsx_dir2xlst(sFrom, sTo, params, convertParams);
		}
		else if ((0 != (AVS_OFFICESTUDIO_FILE_IMAGE & nFormatTo)) || AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatTo || AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV == nFormatTo)
		{
			if (params.needConvertToOrigin(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX) &&
				((0 != (AVS_OFFICESTUDIO_FILE_IMAGE & nFormatTo)) || AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatTo))
			{
				std::wstring sToRender = convertParams.m_sTempParamOOXMLFile;
				if (sToRender.empty())
				{
					sToRender = combinePath(convertParams.m_sTempDir, L"toRender.xlsx");
					nRes = dir2zip(sFrom, sToRender);
				}
				NSDoctRenderer::DoctRendererFormat::FormatFile eFromType = NSDoctRenderer::DoctRendererFormat::FormatFile::XLST;
				if (AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatTo)
				{
					nRes = doct_bin2pdf(eFromType, sToRender, sTo, params, convertParams);
				}
				else if (0 != (AVS_OFFICESTUDIO_FILE_IMAGE & nFormatTo))
				{
					nRes = doct_bin2image(eFromType, sToRender, sTo, params, convertParams);
				}
				else
					nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
			}
			else
			{
				std::wstring sXlstDir = combinePath(convertParams.m_sTempDir, L"xlst_unpacked");
				NSDirectory::CreateDirectory(sXlstDir);
				std::wstring sTFile = combinePath(sXlstDir, L"Editor.bin");

				if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV == nFormatTo)
				{
					convertParams.m_bTempIsXmlOptions = false;
					nRes = xlsx_dir2xlst_bin(sFrom, sTFile, params, convertParams);
				}
				else
				{
					convertParams.m_bTempIsXmlOptions = true;
					nRes = xlsx_dir2xlst_bin(sFrom, sTFile, params, convertParams);
				}
				if (SUCCEEDED_X2T(nRes))
				{
					nRes = fromXlstBin(sTFile, sTo, nFormatTo, params, convertParams);
				}
			}
		}
		else
			nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
		return nRes;
	}
	_UINT32 fromXlsxDir(const std::wstring& sFrom, const std::wstring& sTo, int nFormatTo, InputParams& params, ConvertParams& convertParams)
	{
		_UINT32 nRes = 0;
		if (0 != (AVS_OFFICESTUDIO_FILE_SPREADSHEET & nFormatTo) && AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV != nFormatTo)
		{
			if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX == nFormatTo ||
				AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM == nFormatTo ||
				AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX == nFormatTo ||
				AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM == nFormatTo)
			{
				if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM == nFormatTo ||
					AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX == nFormatTo ||
					AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM == nFormatTo)
				{
					std::wstring sCTFrom = _T("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml");
					switch (*params.m_nFormatFrom)
					{
					case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM:
						sCTFrom = _T("application/vnd.ms-excel.sheet.macroEnabled.main+xml");
						break;
					case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX:
						sCTFrom = _T("application/vnd.openxmlformats-officedocument.spreadsheetml.template.main+xml");
						break;
					case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM:
						sCTFrom = _T("application/vnd.ms-excel.template.macroEnabled.main+xml");
						break;
					}
					std::wstring sCTTo;
					switch (nFormatTo)
					{
					case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM:
						sCTTo = _T("application/vnd.ms-excel.sheet.macroEnabled.main+xml");
						break;
					case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX:
						sCTTo = _T("application/vnd.openxmlformats-officedocument.spreadsheetml.template.main+xml");
						break;
					case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM:
						sCTTo = _T("application/vnd.ms-excel.template.macroEnabled.main+xml");
						break;
					}
					nRes = replaceContentType(sFrom, sCTFrom, sCTTo);
				}
				if (SUCCEEDED_X2T(nRes))
				{
					nRes = dir2zipMscrypt(sFrom, sTo, params, convertParams);
				}
			}
			else if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS == nFormatTo)
			{
				convertParams.m_bIsTemplate = false;
				nRes = xlsx_dir2ods(sFrom, sTo, params, convertParams);
			}
			else if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSB == nFormatTo)
			{
				nRes = xlsx_dir2xlsb(sFrom, sTo, params, convertParams);
			}
			// else if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV == nFormatTo)
			//{
			//	nRes = xlsx_dir2csv(sFrom, sTo, sTemp, params);
			// }
			else if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_OTS == nFormatTo)
			{
				convertParams.m_bIsTemplate = true;
				nRes = xlsx_dir2ods(sFrom, sTo, params, convertParams);
			}
			else
				nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
		}
		else
		{
			nRes = fromXlsbXlsxDir(sFrom, sTo, nFormatTo, params, convertParams);
		}
		return nRes;
	}
	_UINT32 fromXlstBin(const std::wstring& sFrom, const std::wstring& sTo, int nFormatTo, InputParams& params, ConvertParams& convertParams)
	{
		_UINT32 nRes = 0;
		if (AVS_OFFICESTUDIO_FILE_TEAMLAB_XLSY == nFormatTo)
		{
			std::wstring sFromDir = NSDirectory::GetFolderPath(sFrom);
			nRes = dir2zip(sFromDir, sTo);
		}
		else if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV == nFormatTo)
		{
			nRes = xlst_bin2csv(sFrom, sTo, params, convertParams);
		}
		else if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSB == nFormatTo)
		{
			nRes = xlst_bin2xlsb(sFrom, sTo, params, convertParams);
		}
		else if (AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatTo)
		{
			NSDoctRenderer::DoctRendererFormat::FormatFile eFromType = NSDoctRenderer::DoctRendererFormat::FormatFile::XLST;
			nRes = doct_bin2pdf(eFromType, sFrom, sTo, params, convertParams);
		}
		else if (0 != (AVS_OFFICESTUDIO_FILE_IMAGE & nFormatTo))
		{
			NSDoctRenderer::DoctRendererFormat::FormatFile eFromType = NSDoctRenderer::DoctRendererFormat::FormatFile::XLST;
			nRes = doct_bin2image(eFromType, sFrom, sTo, params, convertParams);
		}
		else if (0 != (AVS_OFFICESTUDIO_FILE_SPREADSHEET & nFormatTo) ||
						AVS_OFFICESTUDIO_FILE_OTHER_OOXML == nFormatTo ||
						AVS_OFFICESTUDIO_FILE_OTHER_ODF == nFormatTo)
		{
			std::wstring sXlsxDir = combinePath(convertParams.m_sTempDir, L"xlsx_unpacked");
			if (true == NSDirectory::CreateDirectory(sXlsxDir))
			{
				params.m_bMacro = AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM == nFormatTo || AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM == nFormatTo;
				convertParams.m_sTempResultOOXMLDirectory = sXlsxDir;
				nRes = xlst_bin2xlsx_dir(sFrom, sTo, params, convertParams);
				if (SUCCEEDED_X2T(nRes))
				{
					std::wstring sFileToCurrent = *params.m_sFileTo;
					params.changeFormatFromPost(*params.m_nFormatFrom, params.m_bMacro);

					if (NULL != params.m_nFormatTo)
						nFormatTo = *params.m_nFormatTo;

					nRes = fromXlsxDir(sXlsxDir, *params.m_sFileTo, nFormatTo, params, convertParams);
				}
			}
			else
			{
				nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
			}
		}
		else
			nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
		return nRes;
	}
	_UINT32 fromSpreadsheet(const std::wstring& sFrom, int nFormatFrom, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sTo = *params.m_sFileTo;
		int nFormatTo = AVS_OFFICESTUDIO_FILE_UNKNOWN;
		if (NULL != params.m_nFormatTo)
			nFormatTo = *params.m_nFormatTo;

		if (nFormatTo == AVS_OFFICESTUDIO_FILE_UNKNOWN)
		{
			return AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
		}

		_UINT32 nRes = 0;
		if ((AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV == nFormatFrom) &&
			(AVS_OFFICESTUDIO_FILE_CANVAS_SPREADSHEET == nFormatTo || AVS_OFFICESTUDIO_FILE_OTHER_JSON == nFormatTo))
		{
			nRes = csv2xlst_bin(sFrom, sTo, params, convertParams);
		}
		else
		{
			std::wstring sXlsxDir = combinePath(convertParams.m_sTempDir, L"xlsx_unpacked");
			NSDirectory::CreateDirectory(sXlsxDir);

			if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSB == nFormatFrom &&
				!((AVS_OFFICESTUDIO_FILE_CANVAS & nFormatTo) || (AVS_OFFICESTUDIO_FILE_IMAGE & nFormatTo) || (AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatTo)))
			{
				nRes = xlsb2xlsx_dir(sFrom, sXlsxDir, params, convertParams);
			}
			else if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX == nFormatFrom || AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSB == nFormatFrom)
			{
				convertParams.m_sTempParamOOXMLFile = sFrom;
				if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX == nFormatFrom && params.getFromChanges())
				{
					params.setFromChanges(false);
					nRes = apply_changes(sFrom, sTo, NSDoctRenderer::DoctRendererFormat::FormatFile::XLST, convertParams.m_sTempParamOOXMLFile, params, convertParams);
				}
				nRes = zip2dir(convertParams.m_sTempParamOOXMLFile, sXlsxDir);
				if (!SUCCEEDED_X2T(nRes))
				{
					// check crypt
					COfficeFileFormatChecker OfficeFileFormatChecker;
					if (OfficeFileFormatChecker.isOfficeFile(sFrom))
					{
						if (OfficeFileFormatChecker.nFileType == AVS_OFFICESTUDIO_FILE_OTHER_MS_OFFCRYPTO)
						{
							std::wstring sResultDecryptFile = combinePath(convertParams.m_sTempDir, L"uncrypt_file.oox");
							// test protect
							bool isOldPassword = params.hasPassword();
							const std::wstring sOldPassword = params.getPassword();

							if (isOldPassword)
								delete params.m_sPassword;
							params.m_sPassword = new std::wstring(L"VelvetSweatshop");

							nRes = mscrypt2oox(sFrom, sResultDecryptFile, params, convertParams);
							if (SUCCEEDED_X2T(nRes))
							{
								nRes = zip2dir(sResultDecryptFile, sXlsxDir);
								if (SUCCEEDED_X2T(nRes))
								{
									convertParams.m_sTempParamOOXMLFile = sResultDecryptFile;
								}
							}
							else
							{
								delete params.m_sPassword;
								if (isOldPassword)
									params.m_sPassword = new std::wstring(sOldPassword);
							}
						}
						else if (OfficeFileFormatChecker.nFileType == AVS_OFFICESTUDIO_FILE_OTHER_MS_MITCRYPTO)
							nRes = mitcrypt2oox(sFrom, sTo, params, convertParams);
					}
				}
			}
			else if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM == nFormatFrom)
			{
				if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX == nFormatTo || AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX == nFormatTo)
				{
					nRes = xlsm2xlsx_dir(sFrom, sXlsxDir, params, convertParams);
				}
				else
				{
					nRes = zip2dir(sFrom, sXlsxDir);
				}
			}
			else if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX == nFormatFrom)
			{
				nRes = xltx2xlsx_dir(sFrom, sXlsxDir, params, convertParams);
			}
			else if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM == nFormatFrom)
			{
				if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX == nFormatTo || AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX == nFormatTo)
				{
					nRes = xltm2xlsx_dir(sFrom, sXlsxDir, params, convertParams);
				}
				else
				{
					nRes = xltm2xlsm_dir(sFrom, sXlsxDir, params, convertParams);
				}
			}
			else if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS == nFormatFrom)
			{
				if (params.m_bMacro)
				{
					nRes = xls2xlsm_dir(sFrom, sXlsxDir, params, convertParams);
				}
				else
				{
					nRes = xls2xlsx_dir(sFrom, sXlsxDir, params, convertParams);
				}
			}
			else if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX_FLAT == nFormatFrom)
			{
				nRes = xlsxflat2xlsx_dir(sFrom, sXlsxDir, params, convertParams);
			}
			else if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS == nFormatFrom || AVS_OFFICESTUDIO_FILE_SPREADSHEET_OTS == nFormatFrom)
			{
				nRes = odf2oox_dir(sFrom, sXlsxDir, params, convertParams);
			}
			else if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS_FLAT == nFormatFrom)
			{
				nRes = odf_flat2oox_dir(sFrom, sXlsxDir, params, convertParams);
			}
			else if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX_PACKAGE == nFormatFrom)
			{
				nRes = package2ooxml_dir(sFrom, sXlsxDir, params, convertParams);
			}
			else if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV == nFormatFrom)
			{
				nRes = csv2xlsx_dir(sFrom, sXlsxDir, params, convertParams);
				*params.m_nFormatFrom = AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX;
			}
			else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_XML == nFormatFrom)
			{
				nRes = xml2xlsx_dir(sFrom, sXlsxDir, params, convertParams);
			}
			else if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_NUMBERS == nFormatFrom)
			{
				std::wstring wsTempFile = combinePath(convertParams.m_sTempDir, L"IntermediateFile.odf");

				int nIntermediateResult = numbers2odf(sFrom, wsTempFile, params, convertParams);

				if (S_OK != nIntermediateResult)
					return nIntermediateResult;

				nRes = fromSpreadsheet(wsTempFile, AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS_FLAT, params, convertParams);
			}
			else
				nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
			
			if (SUCCEEDED_X2T(nRes))
			{
				std::wstring sFileToCurrent = *params.m_sFileTo;
				params.changeFormatFromPost(*params.m_nFormatFrom, params.m_bMacro);

				if (NULL != params.m_nFormatTo)
					nFormatTo = *params.m_nFormatTo;
				
				nRes = fromXlsxDir(sXlsxDir, *params.m_sFileTo, nFormatTo, params, convertParams);
			}
		}
		return nRes;
	}

	// from pptx
	_UINT32 fromPptxDir(const std::wstring& sFrom, const std::wstring& sTo, int nFormatTo, InputParams& params, ConvertParams& convertParams)
	{
		_UINT32 nRes = 0;
		if (0 != (AVS_OFFICESTUDIO_FILE_PRESENTATION & nFormatTo))
		{
			if (AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX == nFormatTo ||
				AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM == nFormatTo ||
				AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX == nFormatTo ||
				AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX == nFormatTo ||
				AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM == nFormatTo ||
				AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM == nFormatTo)
			{
				if (AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM == nFormatTo ||
					AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX == nFormatTo ||
					AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX == nFormatTo ||
					AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM == nFormatTo ||
					AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM == nFormatTo)
				{
					std::wstring sCTFrom = _T("application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml");
					switch (*params.m_nFormatFrom)
					{
					case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM:
						sCTFrom = _T("application/vnd.ms-powerpoint.presentation.macroEnabled.main+xml");
						break;
					case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX:
						sCTFrom = _T("application/vnd.openxmlformats-officedocument.presentationml.slideshow.main+xml");
						break;
					case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX:
						sCTFrom = _T("application/vnd.openxmlformats-officedocument.presentationml.template.main+xml");
						break;
					case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM:
						sCTFrom = _T("application/vnd.ms-powerpoint.template.macroEnabled.main+xml");
						break;
					case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM:
						sCTFrom = _T("application/vnd.ms-powerpoint.slideshow.macroEnabled.main+xml");
						break;
					}
					std::wstring sCTTo;
					switch (nFormatTo)
					{
					case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM:
						sCTTo = _T("application/vnd.ms-powerpoint.presentation.macroEnabled.main+xml");
						break;
					case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX:
						sCTTo = _T("application/vnd.openxmlformats-officedocument.presentationml.slideshow.main+xml");
						break;
					case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX:
						sCTTo = _T("application/vnd.openxmlformats-officedocument.presentationml.template.main+xml");
						break;
					case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM:
						sCTTo = _T("application/vnd.ms-powerpoint.template.macroEnabled.main+xml");
						break;
					case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM:
						sCTTo = _T("application/vnd.ms-powerpoint.slideshow.macroEnabled.main+xml");
						break;
					}
					nRes = replaceContentType(sFrom, sCTFrom, sCTTo);
				}
				if (SUCCEEDED_X2T(nRes))
				{
					nRes = dir2zipMscrypt(sFrom, sTo, params, convertParams);
				}
			}
			// else if (AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT == nFormatTo)
			else if (AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP == nFormatTo)
			{
				convertParams.m_bIsTemplate = false;
				nRes = pptx_dir2odp(sFrom, sTo, params, convertParams);
			}
			else if (AVS_OFFICESTUDIO_FILE_PRESENTATION_OTP == nFormatTo)
			{
				convertParams.m_bIsTemplate = true;
				nRes = pptx_dir2odp(sFrom, sTo, params, convertParams);
			}
			else
				nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
		}
		else if (AVS_OFFICESTUDIO_FILE_OTHER_OOXML == nFormatTo)
		{
			nRes = dir2zipMscrypt(sFrom, sTo, params, convertParams);
		}
		else if (AVS_OFFICESTUDIO_FILE_CANVAS_PRESENTATION == nFormatTo)
		{
			nRes = pptx_dir2pptt_bin(sFrom, sTo, params, convertParams);
		}
		else if (AVS_OFFICESTUDIO_FILE_TEAMLAB_PPTY == nFormatTo)
		{
			nRes = pptx_dir2pptt(sFrom, sTo, params, convertParams);
		}
		else if ((0 != (AVS_OFFICESTUDIO_FILE_IMAGE & nFormatTo)) || AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatTo)
		{
			if (params.needConvertToOrigin(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX))
			{
				std::wstring sToRender = convertParams.m_sTempParamOOXMLFile;
				if (sToRender.empty())
				{
					sToRender = combinePath(convertParams.m_sTempDir, L"toRender.pptx");
					nRes = dir2zip(sFrom, sToRender);
				}
				NSDoctRenderer::DoctRendererFormat::FormatFile eFromType = NSDoctRenderer::DoctRendererFormat::FormatFile::PPTT;
				if (AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatTo)
				{
					convertParams.m_sInternalMediaDirectory = sFrom;
					nRes = doct_bin2pdf(eFromType, sToRender, sTo, params, convertParams);
				}
				else if (0 != (AVS_OFFICESTUDIO_FILE_IMAGE & nFormatTo))
				{
					convertParams.m_sInternalMediaDirectory = sFrom;
					nRes = doct_bin2image(eFromType, sToRender, sTo, params, convertParams);
				}
				else
					nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
			}
			else
			{
				std::wstring sPpttDir = combinePath(convertParams.m_sTempDir, L"pptt_unpacked");
				NSDirectory::CreateDirectory(sPpttDir);
				std::wstring sTFile = combinePath(sPpttDir, L"Editor.bin");

				nRes = pptx_dir2pptt_bin(sFrom, sTFile, params, convertParams);
				if (SUCCEEDED_X2T(nRes))
				{
					nRes = fromPpttBin(sTFile, sTo, nFormatTo, params, convertParams);
				}
			}
		}
		else
			nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
		return nRes;
	}
	_UINT32 fromPpttBin(const std::wstring& sFrom, const std::wstring& sTo, int nFormatTo, InputParams& params, ConvertParams& convertParams)
	{
		_UINT32 nRes = 0;
		if (AVS_OFFICESTUDIO_FILE_TEAMLAB_PPTY == nFormatTo)
		{
			std::wstring sFromDir = NSDirectory::GetFolderPath(sFrom);
			nRes = dir2zip(sFromDir, sTo);
		}
		else if (AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatTo)
		{
			NSDoctRenderer::DoctRendererFormat::FormatFile eFromType = NSDoctRenderer::DoctRendererFormat::FormatFile::PPTT;
			nRes = doct_bin2pdf(eFromType, sFrom, sTo, params, convertParams);
		}
		else if (0 != (AVS_OFFICESTUDIO_FILE_IMAGE & nFormatTo))
		{
			NSDoctRenderer::DoctRendererFormat::FormatFile eFromType = NSDoctRenderer::DoctRendererFormat::FormatFile::PPTT;
			nRes = doct_bin2image(eFromType, sFrom, sTo, params, convertParams);
		}
		else if (0 != (AVS_OFFICESTUDIO_FILE_PRESENTATION & nFormatTo) ||
						AVS_OFFICESTUDIO_FILE_OTHER_OOXML == nFormatTo ||
						AVS_OFFICESTUDIO_FILE_OTHER_ODF == nFormatTo)
		{
			std::wstring sPptxDir = combinePath(convertParams.m_sTempDir, L"pptx_unpacked");

			if (true == NSDirectory::CreateDirectory(sPptxDir))
			{
				params.m_bMacro = AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM == nFormatTo ||
								  AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM == nFormatTo ||
								  AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM == nFormatTo;

				convertParams.m_sTempResultOOXMLDirectory = sPptxDir;
				nRes = pptt_bin2pptx_dir(sFrom, sTo, params, convertParams);
				if (SUCCEEDED_X2T(nRes))
				{
					std::wstring sFileToCurrent = *params.m_sFileTo;
					params.changeFormatFromPost(*params.m_nFormatFrom, params.m_bMacro);

					if (NULL != params.m_nFormatTo)
						nFormatTo = *params.m_nFormatTo;

					nRes = fromPptxDir(sPptxDir, *params.m_sFileTo, nFormatTo, params, convertParams);
				}
			}
			else
			{
				nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
			}
		}
		else
			nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
		return nRes;
	}
	_UINT32 fromPresentation(const std::wstring& sFrom, int nFormatFrom, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sTo	= *params.m_sFileTo;

		int nFormatTo = AVS_OFFICESTUDIO_FILE_UNKNOWN;
		if (NULL != params.m_nFormatTo)
			nFormatTo = *params.m_nFormatTo;

		_UINT32 nRes = 0;
		std::wstring sPptxDir = combinePath(convertParams.m_sTempDir, L"pptx_unpacked");
		NSDirectory::CreateDirectory(sPptxDir);

		if (AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX == nFormatFrom)
		{
			convertParams.m_sTempParamOOXMLFile = sFrom;
			if (params.getFromChanges())
			{
				params.setFromChanges(false);
				nRes = apply_changes(sFrom, sTo, NSDoctRenderer::DoctRendererFormat::FormatFile::PPTT, convertParams.m_sTempParamOOXMLFile, params, convertParams);
			}
			nRes = zip2dir(convertParams.m_sTempParamOOXMLFile, sPptxDir);
		}
		else if (AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM == nFormatFrom)
		{
			if (AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX == nFormatTo || AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX == nFormatTo)
			{
				nRes = pptm2pptx_dir(sFrom, sPptxDir, params, convertParams);
			}
			else
			{
				nRes = zip2dir(sFrom, sPptxDir);
			}
		}
		else if (AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM == nFormatFrom)
		{
			if (AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX == nFormatTo || AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX == nFormatTo)
			{
				nRes = ppsm2pptx_dir(sFrom, sPptxDir, params, convertParams);
			}
			else
			{
				nRes = ppsm2pptm_dir(sFrom, sPptxDir, params, convertParams);
			}
		}
		else if (AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT == nFormatFrom)
		{
			if (params.m_bMacro)
			{
				nRes = ppt2pptm_dir(sFrom, sPptxDir, params, convertParams);
			}
			else
			{
				nRes = ppt2pptx_dir(sFrom, sPptxDir, params, convertParams);
			}
		}
		else if (AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP == nFormatFrom || AVS_OFFICESTUDIO_FILE_PRESENTATION_OTP == nFormatFrom)
		{
			nRes = odf2oox_dir(sFrom, sPptxDir, params, convertParams);
		}
		else if (AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP_FLAT == nFormatFrom)
		{
			nRes = odf_flat2oox_dir(sFrom, sPptxDir, params, convertParams);
		}
		else if (AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX == nFormatFrom)
		{
			nRes = ppsx2pptx_dir(sFrom, sPptxDir, params, convertParams);
		}
		else if (AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX == nFormatFrom)
		{
			nRes = potx2pptx_dir(sFrom, sPptxDir, params, convertParams);
		}
		else if (AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM == nFormatFrom)
		{
			if (AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX == nFormatTo || AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX == nFormatTo)
			{
				nRes = potm2pptx_dir(sFrom, sPptxDir, params, convertParams);
			}
			else
			{
				nRes = potm2pptm_dir(sFrom, sPptxDir, params, convertParams);
			}
		}
		else if (AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX_PACKAGE == nFormatFrom)
		{
			nRes = package2ooxml_dir(sFrom, sPptxDir, params, convertParams);
		}
		else if (AVS_OFFICESTUDIO_FILE_PRESENTATION_KEY == nFormatFrom)
		{
			std::wstring wsTempFile = combinePath(convertParams.m_sTempDir, L"IntermediateFile.odf");

			int nIntermediateResult = key2odf(sFrom, wsTempFile, params, convertParams);

			if (S_OK != nIntermediateResult)
				return nIntermediateResult;

			nRes = fromPresentation(wsTempFile, AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP_FLAT, params, convertParams);
		}
		else
			nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
		
		if (SUCCEEDED_X2T(nRes))
		{
			std::wstring sFileToCurrent = *params.m_sFileTo;
			params.changeFormatFromPost(*params.m_nFormatFrom, params.m_bMacro);

			if (NULL != params.m_nFormatTo)
				nFormatTo = *params.m_nFormatTo;

			nRes = fromPptxDir(sPptxDir, *params.m_sFileTo, nFormatTo, params, convertParams);
		}
		return nRes;
	}

	// from T
	_UINT32 fromT(const std::wstring& sFrom, int nFormatFrom, const std::wstring& sTo, int nFormatTo, InputParams& params, ConvertParams& convertParams)
	{
		_UINT32 nRes = 0;
		if (0 != (AVS_OFFICESTUDIO_FILE_CANVAS & nFormatTo))
		{
			std::wstring sTDir = NSDirectory::GetFolderPath(sTo);
			nRes = zip2dir(sFrom, sTDir);
		}
		else
		{
			std::wstring sTDir = combinePath(convertParams.m_sTempDir, L"doct_unpacked");
			NSDirectory::CreateDirectory(sTDir);
			std::wstring sTFile = combinePath(sTDir, L"Editor.bin");
			nRes = zip2dir(sFrom, sTDir);
			if (SUCCEEDED_X2T(nRes))
			{
				if (AVS_OFFICESTUDIO_FILE_TEAMLAB_DOCY == nFormatFrom)
					nRes = fromDoctBin(sTFile, sTo, nFormatTo, params, convertParams);
				else if (AVS_OFFICESTUDIO_FILE_TEAMLAB_XLSY == nFormatFrom)
					nRes = fromXlstBin(sTFile, sTo, nFormatTo, params, convertParams);
				else if (AVS_OFFICESTUDIO_FILE_TEAMLAB_PPTY == nFormatFrom)
					nRes = fromPpttBin(sTFile, sTo, nFormatTo, params, convertParams);
				else
					nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
			}
		}
		return nRes;
	}

	// visio
	_UINT32 fromVsdxDir(const std::wstring& sFrom, const std::wstring& sTo, int nFormatTo, InputParams& params, ConvertParams& convertParams)
	{
		_UINT32 nRes = 0;
		if (0 != (AVS_OFFICESTUDIO_FILE_DRAW & nFormatTo))
		{
			if (AVS_OFFICESTUDIO_FILE_DRAW_VSDX == nFormatTo)
			{
				if (SUCCEEDED_X2T(nRes))
				{
					nRes = dir2zipMscrypt(sFrom, sTo, params, convertParams);
				}
			}
			else
				nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
		}
		else if ((0 != (AVS_OFFICESTUDIO_FILE_IMAGE & nFormatTo)) || AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatTo)
		{
			std::wstring sToRender = convertParams.m_sTempParamOOXMLFile;
			if (sToRender.empty())
			{
				sToRender = combinePath(convertParams.m_sTempDir, L"toRender.vsdx");
				nRes = dir2zip(sFrom, sToRender);
			}
			NSDoctRenderer::DoctRendererFormat::FormatFile eFromType = NSDoctRenderer::DoctRendererFormat::FormatFile::VSDT;
			if (AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatTo)
			{
				convertParams.m_sInternalMediaDirectory = sFrom;
				nRes = doct_bin2pdf(eFromType, sToRender, sTo, params, convertParams);
			}
			else if (0 != (AVS_OFFICESTUDIO_FILE_IMAGE & nFormatTo))
			{
				convertParams.m_sInternalMediaDirectory = sFrom;
				nRes = doct_bin2image(eFromType, sToRender, sTo, params, convertParams);
			}
			else
				nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
		}
		else
			nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
		return nRes;
	}

	_UINT32 fromDraw(const std::wstring& sFrom, int nFormatFrom, InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sTo	= *params.m_sFileTo;
		int nFormatTo = AVS_OFFICESTUDIO_FILE_UNKNOWN;
		if (NULL != params.m_nFormatTo)
			nFormatTo = *params.m_nFormatTo;

		_UINT32 nRes = 0;
		std::wstring sVsdxDir = combinePath(convertParams.m_sTempDir, L"xsdx_unpacked");
		NSDirectory::CreateDirectory(sVsdxDir);

		if (0 != (AVS_OFFICESTUDIO_FILE_DRAW & nFormatFrom))
		{
			convertParams.m_sTempParamOOXMLFile = sFrom;
			if (params.getFromChanges())
			{
				params.setFromChanges(false);
				nRes = apply_changes(sFrom, sTo, NSDoctRenderer::DoctRendererFormat::FormatFile::VSDT, convertParams.m_sTempParamOOXMLFile, params, convertParams);
			}
			nRes = zip2dir(sFrom, sVsdxDir);
		}
		else
			nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
		if (SUCCEEDED_X2T(nRes))
		{
			std::wstring sFileToCurrent = *params.m_sFileTo;
			params.changeFormatFromPost(*params.m_nFormatFrom, params.m_bMacro);

			if (NULL != params.m_nFormatTo)
				nFormatTo = *params.m_nFormatTo;
			
			nRes = fromVsdxDir(sVsdxDir, sTo, nFormatTo, params, convertParams);
		}
		return nRes;
	}

	//------------------------------------------------------------------------------------------------------------------

	_UINT32 fromInputParams(InputParams& oInputParams)
	{
		TConversionDirection conversion = oInputParams.getConversionDirection();
		
		std::wstring sFileFrom = oInputParams.m_sFileFrom  ? *oInputParams.m_sFileFrom : L"";
		std::wstring sFileTo = oInputParams.m_sFileTo ? *oInputParams.m_sFileTo : L"";

		int nFormatFrom = AVS_OFFICESTUDIO_FILE_UNKNOWN;
		if (NULL != oInputParams.m_nFormatFrom)
			nFormatFrom = *oInputParams.m_nFormatFrom;
		int nFormatTo = AVS_OFFICESTUDIO_FILE_UNKNOWN;
		if (NULL != oInputParams.m_nFormatTo)
			nFormatTo = *oInputParams.m_nFormatTo;

		if (TCD_ERROR == conversion)
		{
			if (AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT == nFormatFrom || AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV == nFormatFrom)
				return AVS_FILEUTILS_ERROR_CONVERT_NEED_PARAMS;
			else
			{
				// print out conversion direction error
				std::cerr << "Couldn't recognize conversion direction from an argument" << std::endl;
				return AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
			}
		}

		if (sFileFrom.empty() || sFileTo.empty())
		{
			std::cerr << "Empty sFileFrom or sFileTo" << std::endl;
			return AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
		}

		ConvertParams oConvertParams;

		if (NULL != oInputParams.m_bPaid)
			oConvertParams.m_bIsPaid = *oInputParams.m_bPaid;

		if (NULL != oInputParams.m_sThemeDir)
			oConvertParams.m_sThemesDir = *oInputParams.m_sThemeDir;

		InputParamsMailMerge *oMailMerge = NULL;
		if (NULL != oInputParams.m_oMailMergeSend)
			oMailMerge = oInputParams.m_oMailMergeSend;

		bool bExternalTempDir = false;
		if (NULL != oInputParams.m_sTempDir)
		{
			bExternalTempDir = true;
			oConvertParams.m_sTempDir = *oInputParams.m_sTempDir;
		}
		else
		{
			oConvertParams.m_sTempDir = NSDirectory::CreateDirectoryWithUniqueName(NSDirectory::GetFolderPath(sFileTo));
		}
		if (oConvertParams.m_sTempDir.empty())
		{
			std::cerr << "Couldn't create temp folder" << std::endl;
			return AVS_FILEUTILS_ERROR_UNKNOWN;
		}

		std::wstring sGlobalTempDir = L"";
		if (NSFile::CFileBinary::IsGlobalTempPathUse())
			sGlobalTempDir = NSFile::CFileBinary::GetTempPath();

		NSFile::CFileBinary::SetTempPath(oConvertParams.m_sTempDir);

		if (!oInputParams.checkInputLimits())
		{
			if (!sGlobalTempDir.empty())
				NSFile::CFileBinary::SetTempPath(sGlobalTempDir);
			return AVS_FILEUTILS_ERROR_CONVERT_LIMITS;
		}

#ifndef BUILD_X2T_AS_LIBRARY_DYLIB
		NSDoctRenderer::CDocBuilder::Initialize();
#endif

		_UINT32 result = 0;
		switch (conversion)
		{
		case TCD_NON_AUTO:
		{
			result = AVS_FILEUTILS_ERROR_CONVERT_DETECT;
		}
		break;
		case TCD_DOC2DOCT:
		{
			result = doc2doct(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_DOCX2DOCT:
		{
			result = docx2doct(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_DOCXFLAT2DOCT:
		{
			result = docxflat2doct(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_DOCXFLAT2DOCT_BIN:
		{
			result = docxflat2doct_bin(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_PKG2BIN_T:
		{
			result = package2bin_t(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_PKG2BIN:
		{
			result = package2bin(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_PKG2OOXML:
		{
			oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX);
			result = package2ooxml(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_DOCT2DOCX:
		{
			oInputParams.m_bMacro = false;
			oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX);
			result = doct2docx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_DOCT2DOTX:
		{
			oInputParams.m_bMacro = false;
			oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX);
			result = doct2docx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_DOCT2OFORM:
		{
			oInputParams.m_bMacro = false;
			oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM);
			result = doct2docx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_DOCT2DOCXF:
		{
			oInputParams.m_bMacro = false;
			oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCXF);
			result = doct2docx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_DOCT2DOCM:
		{
			oInputParams.m_bMacro = true;
			oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM);
			result = doct2docx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_XLSX2XLST:
		{
			result = xlsx2xlst(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_XLSB2XLST:
		{
			oInputParams.m_nFormatFrom = new int(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSB);
			result = xlsx2xlst(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_XLSXFLAT2XLST:
		{
			result = xlsxflat2xlst(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_XLSXFLAT2XLST_BIN:
		{
			result = xlsxflat2xlst_bin(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_XLSXFLAT2XLSX:
		{
			result = xlsxflat2xlsx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_XLST2XLSX:
		{
			oInputParams.m_bMacro = false;
			oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX);
			result = xlst2xlsx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_XLST2XLSM:
		{
			oInputParams.m_bMacro = true;
			oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM);
			result = xlst2xlsx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_XLST2XLSB:
		{
			oInputParams.m_bMacro = true;
			oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSB);
			result = xlst2xlsb(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_XLST2XLTX:
		{
			oInputParams.m_bMacro = false;
			oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX);
			result = xlst2xlsx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_PPTX2PPTT:
		{
			result = pptx2pptt(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_PPTT2PPTX:
		{
			oInputParams.m_bMacro = false;
			oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX);
			result = pptt2pptx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_PPTT2PPTM:
		{
			oInputParams.m_bMacro = true;
			oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM);
			result = pptt2pptx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_PPTT2POTX:
		{
			oInputParams.m_bMacro = false;
			oInputParams.m_nFormatTo = new int(AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX);
			result = pptt2pptx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_DOTX2DOCX:
		{
			result = dotx2docx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_DOCM2DOCX:
		{
			result = docm2docx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_DOTM2DOCX:
		{
			result = dotm2docx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_DOTM2DOCM:
		{
			result = dotm2docm(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_XLTX2XLSX:
		{
			result = xltx2xlsx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_XLSM2XLSX:
		{
			result = xltx2xlsx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_XLTM2XLSX:
		{
			result = xltm2xlsx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_XLTM2XLSM:
		{
			result = xltm2xlsm(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_PPSX2PPTX:
		{
			result = ppsx2pptx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_POTX2PPTX:
		{
			result = potx2pptx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_POTM2PPTX:
		{
			result = potm2pptx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_PPSM2PPTX:
		{
			result = ppsm2pptx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_POTM2PPTM:
		{
			result = potm2pptm(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_PPSM2PPTM:
		{
			result = ppsm2pptm(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_PPTM2PPTX:
		{
			result = pptm2pptx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_ZIPDIR:
		{
			result = dir2zip(sFileFrom, sFileTo, false, 8, -1, true);
		}
		break;
		case TCD_UNZIPDIR:
		{
			result = zip2dir(sFileFrom, sFileTo);
		}
		break;
		case TCD_XML2XLSX:
		{
			result = xml2xlsx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_CSV2XLSX:
		{
			result = csv2xlsx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_CSV2XLST:
		{
			result = csv2xlst(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_XLSX2CSV:
		{
			result = xlsx2csv(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_XLST2CSV:
		{
			result = xlst2csv(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_DOCX2DOCT_BIN:
		{
			result = docx2doct_bin(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_DOCT_BIN2DOCX:
		{
			result = doct_bin2docx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_XLSX2XLST_BIN:
		{
			result = xlsx2xlst_bin(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_XLST_BIN2XLSX:
		{
			result = xlst_bin2xlsx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_PPTX2PPTT_BIN:
		{
			result = pptx2pptt_bin(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_PPTT_BIN2PPTX:
		{
			result = pptt_bin2pptx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_BIN2PDF:
		{
			result = bin2pdf(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_BIN2T:
		{
			result = dir2zip(NSDirectory::GetFolderPath(sFileFrom), sFileTo);
		}
		break;
		case TCD_T2BIN:
		{
			result = zip2dir(sFileFrom, NSDirectory::GetFolderPath(sFileTo));
		}
		break;
		case TCD_PPT2PPTX:
		{
			result = ppt2pptx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_PPT2PPTM:
		{
			result = ppt2pptm(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_PPT2PPTT:
		{
			result = ppt2pptt(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_PPT2PPTT_BIN:
		{
			result = ppt2pptt_bin(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_RTF2DOCX:
		{
			result = rtf2docx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_RTF2DOCT:
		{
			result = rtf2doct(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_RTF2DOCT_BIN:
		{
			result = rtf2doct_bin(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_DOCX2RTF:
		{
			result = docx2rtf(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_DOCX2TXT:
		{
			result = docx2txt(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_DOC2DOCX:
		{
			result = doc2docx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_DOC2DOCM:
		{
			result = doc2docm(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_DOCT2RTF:
		{
			result = doct2rtf(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_DOCT_BIN2RTF:
		{
			result = doct_bin2rtf(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_TXT2DOCX:
		{
			result = txt2docx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_TXT2DOCT:
		{
			result = txt2doct(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_TXT2DOCT_BIN:
		{
			result = txt2doct_bin(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_XLS2XLSX:
		{
			result = xls2xlsx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_XLS2XLSM:
		{
			result = xls2xlsm(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_XLS2XLST:
		{
			result = xls2xlst(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_XLS2XLST_BIN:
		{
			result = xls2xlst_bin(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_OTF2ODF:
		{
			result = otf2odf(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_ODF2OOX:
		{
			result = odf2oox(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_ODF2OOT:
		{
			result = odf2oot(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_ODF2OOT_BIN:
		{
			result = odf2oot_bin(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_ODF_FLAT2OOX:
		{
			result = odf_flat2oox(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_ODF_FLAT2OOT:
		{
			result = odf_flat2oot(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_ODF_FLAT2OOT_BIN:
		{
			result = odf_flat2oot_bin(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_DOCX2ODT:
		{
			result = docx2odt(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_XLSX2ODS:
		{
			result = xlsx2ods(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_PPTX2ODP:
		{
			result = pptx2odp(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_MAILMERGE:
		{
			result = convertmailmerge(*oMailMerge, sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_DOCUMENT2:
		{
			oInputParams.m_bMacro = (	nFormatTo == AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM || nFormatTo == AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM ||
										nFormatTo == AVS_OFFICESTUDIO_FILE_OTHER_OOXML);
			result = fromDocument(sFileFrom, nFormatFrom, oInputParams, oConvertParams);
		}
		break;
		case TCD_SPREADSHEET2:
		{
			oInputParams.m_bMacro = (	nFormatTo == AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM || nFormatTo == AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM ||
										nFormatTo == AVS_OFFICESTUDIO_FILE_OTHER_OOXML);
			result = fromSpreadsheet(sFileFrom, nFormatFrom, oInputParams, oConvertParams);
		}
		break;
		case TCD_PRESENTATION2:
		{
			oInputParams.m_bMacro = (	nFormatTo == AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM || nFormatTo == AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSM ||
										nFormatTo == AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM || nFormatTo == AVS_OFFICESTUDIO_FILE_OTHER_OOXML);
			result = fromPresentation(sFileFrom, nFormatFrom, oInputParams, oConvertParams);
		}
		break;
		case TCD_T2:
		{
			result = fromT(sFileFrom, nFormatFrom, sFileTo, nFormatTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_DOCT_BIN2:
		{
			result = fromDoctBin(sFileFrom, sFileTo, nFormatTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_XLST_BIN2:
		{
			result = fromXlstBin(sFileFrom, sFileTo, nFormatTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_PPTT_BIN2:
		{
			result = fromPpttBin(sFileFrom, sFileTo, nFormatTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_CROSSPLATFORM2:
		{
			result = fromCrossPlatform(sFileFrom, nFormatFrom, sFileTo, nFormatTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_CANVAS_PDF2:
		{
			result = fromCanvasPdf(sFileFrom, nFormatFrom, sFileTo, nFormatTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_MSCRYPT2:
		{
			result = fromMscrypt(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_MSCRYPT2_RAW:
		{
			result = mscrypt2oox(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_2MSCRYPT_RAW:
		{
			result = oox2mscrypt(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_MSCRYPT2DOCT:
		case TCD_MSCRYPT2XLST:
		case TCD_MSCRYPT2PPTT:
		{
			result = mscrypt2oot(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_MSCRYPT2BIN:
		{
			result = mscrypt2oot_bin(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_MITCRYPT2:
		{
			result = fromMitcrypt(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_HTML2DOCX:
		{
			result = html2docx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_HTMLZIP2DOCX:
		{
			result = html_zip2docx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_HTML2DOCT:
		{
			result = html2doct(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_HTMLZIP2DOCT:
		{
			result = html_zip2doct(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_HTML2DOCT_BIN:
		{
			result = html2doct_bin(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_HTMLZIP2DOCT_BIN:
		{
			result = html_zip2doct_bin(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_FB22DOCX:
		{
			result = fb2docx(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_VBAPROJECT2XML:
		{
			result = msVbaProject2Xml(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}
		break;
		case TCD_DRAW2:
		{
			result = fromDraw(sFileFrom, nFormatFrom, oInputParams, oConvertParams);
		}break;
		case TCD_XLSX2XLSB:
		{
			result = xlsx2xlsb(sFileFrom, sFileTo, oInputParams, oConvertParams);
		}break;
		// TCD_FB22DOCT,
		// TCD_FB22DOCT_BIN,
		// TCD_EPUB2DOCX,
		// TCD_EPUB2DOCT,
		// TCD_EPUB2DOCT_BIN,
		default:
		{
			result = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
		}break;
		}

		// delete temp dir
		if (!bExternalTempDir)
		{
			NSDirectory::DeleteDirectory(oConvertParams.m_sTempDir);
		}

		if (!sGlobalTempDir.empty())
			NSFile::CFileBinary::SetTempPath(sGlobalTempDir);

		// clean up v8
#ifndef BUILD_X2T_AS_LIBRARY_DYLIB
		NSDoctRenderer::CDocBuilder::Dispose();
#endif
		if (SUCCEEDED_X2T(result) && oInputParams.m_bOutputConvertCorrupted)
		{
			return AVS_FILEUTILS_ERROR_CONVERT_CORRUPTED;
		}
		else
		{
			return result;
		}
	}

	_UINT32 FromFile(const std::wstring &file)
	{
		InputParams oInputParams;
		if (oInputParams.FromXmlFile(file))
		{
			return fromInputParams(oInputParams);
		}
		else
		{
			return AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
		}
	}

	_UINT32 FromXml(const std::wstring &xml)
	{
		InputParams oInputParams;
		if (oInputParams.FromXml(xml))
		{
			return fromInputParams(oInputParams);
		}
		else
		{
			return AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
		}
	}
} // namespace NExtractTools
