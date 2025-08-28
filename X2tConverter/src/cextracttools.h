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
#ifndef CEXTRACTTOOLS_H
#define CEXTRACTTOOLS_H

#include "../../Common/OfficeFileErrorDescription.h"
#include "../../Common/OfficeFileFormatChecker.h"
#include "../../OOXML/Base/Unit.h"
#include "../../OOXML/SystemUtility/SystemUtility.h"

#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/common/Path.h"
#include "../../DesktopEditor/common/StringBuilder.h"
#include "../../DesktopEditor/doctrenderer/doctrenderer.h"
#include "../../DesktopEditor/common/ProcessEnv.h"

#include <boost/algorithm/string.hpp>
#include <boost/unordered_map.hpp>

#include <fstream>
#include <iostream>

#define SUCCEEDED_X2T(nRes) (0 == (nRes) || AVS_FILEUTILS_ERROR_CONVERT_ROWLIMITS == (nRes) || AVS_FILEUTILS_ERROR_CONVERT_CELLLIMITS == (nRes))

namespace NExtractTools
{
	typedef enum tagTConversionDirection
	{
		TCD_ERROR,
		TCD_AUTO,
		TCD_NON_AUTO,

		TCD_DOCX2DOCT,
		TCD_DOCT2DOCX,
		TCD_DOCT2DOTX,
		TCD_DOCT2DOCM,
		TCD_DOCT2OFORM,
		TCD_DOCT2DOCXF,
		TCD_DOCX2DOCT_BIN,
		TCD_DOCT_BIN2DOCX,
		TCD_DOTX2DOCX,
		TCD_DOCM2DOCX,
		TCD_DOTM2DOCX,
		TCD_DOTM2DOCM,

		TCD_DOCXFLAT2DOCT,
		TCD_DOCXFLAT2DOCT_BIN,

		TCD_PKG2OOXML,
		TCD_PKG2BIN,
		TCD_PKG2BIN_T,

		TCD_XLSXFLAT2XLST,
		TCD_XLSXFLAT2XLST_BIN,
		TCD_XLSXFLAT2XLSX,

		TCD_XLSX2XLST,
		TCD_XLST2XLSX,
		TCD_XLST2XLSM,
		TCD_XLST2XLTX,
		TCD_XLSX2XLST_BIN,
		TCD_XLST_BIN2XLSX,
		TCD_XLTX2XLSX,
		TCD_XLSM2XLSX,
		TCD_XLTM2XLSX,
		TCD_XLTM2XLSM,
		TCD_XLSB2XLST,
		TCD_XLSX2XLSB,
		TCD_XLST2XLSB,

		TCD_PPTX2PPTT,
		TCD_PPTT2PPTX,
		TCD_PPTT2POTX,
		TCD_PPTT2PPTM,
		TCD_PPTX2PPTT_BIN,
		TCD_PPTT_BIN2PPTX,
		TCD_PPSX2PPTX,
		TCD_POTX2PPTX,
		TCD_PPTM2PPTX,
		TCD_POTM2PPTX,
		TCD_PPSM2PPTX,
		TCD_POTM2PPTM,
		TCD_PPSM2PPTM,

		TCD_VSDX2VSDT,
		TCD_VSDX2VSDT_BIN,
		TCD_VSDT2VSDX,
		TCD_VSDT2VSDM,
		TCD_VSDT2VSTX,
		TCD_VSDT2VSTM,
		TCD_VSDT_BIN2VSDX,

		TCD_ZIPDIR,
		TCD_UNZIPDIR,

		TCD_CSV2XLSX,
		TCD_XLSX2CSV,

		TCD_CSV2XLST,
		TCD_XLST2CSV,
		TCD_CSV2XLST_BIN,
		TCD_XLST_BIN2CSV,

		TCD_BIN2PDF,

		TCD_T2BIN,
		TCD_BIN2T,

		// ppt 2
		TCD_PPT2PPTX,
		TCD_PPT2PPTT,
		TCD_PPT2PPTT_BIN,
		TCD_PPT2PPTM,
		// doc 2
		TCD_DOC2DOCT,
		TCD_DOC2DOCT_BIN,
		TCD_DOC2DOCX,
		TCD_DOC2DOCM,
		// xls 2
		TCD_XLS2XLST,
		TCD_XLS2XLST_BIN,
		TCD_XLS2XLSX,
		TCD_XLS2XLSM,
		// rtf 2
		TCD_RTF2DOCX,
		TCD_RTF2DOCT,
		TCD_RTF2DOCT_BIN,
		// 2 rtf
		TCD_DOCT2RTF,
		TCD_DOCX2RTF,
		TCD_DOCT_BIN2RTF,
		// xml 2
		TCD_XML2XLSX,
		// txt 2
		TCD_TXT2DOCX,
		TCD_TXT2DOCT,
		TCD_TXT2DOCT_BIN,
		// 2 txt
		TCD_DOCX2TXT,
		TCD_DOCT2TXT,
		TCD_DOCT_BIN2TXT,
		// odf 2
		TCD_ODF2OOX,
		TCD_ODF2OOT,
		TCD_ODF2OOT_BIN,
		TCD_OTF2ODF,
		// odf flat 2
		TCD_ODF_FLAT2OOX,
		TCD_ODF_FLAT2OOT,
		TCD_ODF_FLAT2OOT_BIN,
		TCD_ODF_FLAT2ODF, // todooo напрямую
		// 2 odt
		TCD_DOCX2ODT,
		TCD_DOCT2ODT,
		TCD_DOCT_BIN2ODT,
		// 2 ods
		TCD_XLSX2ODS,
		TCD_XLST2ODS,
		TCD_XLST_BIN2ODS,

		// 2 odp
		TCD_PPTX2ODP,
		TCD_PPTT2ODP,
		TCD_PPTX_BIN2ODP,

		TCD_XML2DOCX,
		TCD_DOCX2XML,
		//
		TCD_MSCRYPT2,
		TCD_MSCRYPT2DOCT,
		TCD_MSCRYPT2XLST,
		TCD_MSCRYPT2PPTT,
		TCD_MSCRYPT2BIN,

		TCD_MSCRYPT2_RAW,
		TCD_2MSCRYPT_RAW,

		TCD_MITCRYPT2,

		TCD_VBAPROJECT2XML,

		//
		TCD_HTML2DOCX,
		TCD_HTML2DOCT,
		TCD_HTML2DOCT_BIN,

		TCD_HTMLZIP2DOCX,
		TCD_HTMLZIP2DOCT,
		TCD_HTMLZIP2DOCT_BIN,

		TCD_FB22DOCX,
		TCD_FB22DOCT,
		TCD_FB22DOCT_BIN,

		TCD_EPUB2DOCX,
		TCD_EPUB2DOCT,
		TCD_EPUB2DOCT_BIN,
        TCD_MAILMERGE,
        TCD_T2,
        TCD_DOCT_BIN2,
        TCD_XLST_BIN2,
        TCD_PPTT_BIN2,
		TCD_VSDT_BIN2,
        TCD_DOCUMENT2,
        TCD_SPREADSHEET2,
        TCD_PRESENTATION2,
        TCD_CROSSPLATFORM2,
        TCD_CANVAS_PDF2,
        TCD_DRAW2
	} TConversionDirection;

	typedef enum tagTCsvDelimiter
	{
		TCSVD_NONE = 0,
		TCSVD_TAB = 1,
		TCSVD_SEMICOLON = 2,
		TCSVD_COLON = 3,
		TCSVD_COMMA = 4,
		TCSVD_SPACE = 5
	} TCsvDelimiter;

	int getReturnErrorCode(_UINT32 nDefine);

	const TConversionDirection getConversionDirectionFromExt(const std::wstring& sFile1, const std::wstring& sFile2);
	const TConversionDirection getConversionDirection(const std::wstring& sArg3);

	bool copyOrigin(const std::wstring& sFileFrom, const std::wstring& sFileTo);

	/**
	 * @brief Mail merge related parameters.
	 * @details Parsed from the <m_oMailMergeSend> section in the input XML.
	 */
	class InputParamsMailMerge
	{
	public:
		/**
		 * @brief File name.
		 */
		std::wstring* fileName;
		/**
		 * @brief Sender's email address.
		 */
		std::wstring* from;
		/**
		 * @brief [Deprecated].
		 */
		std::wstring* jsonKey;
		/**
		 * @brief Attachment format docx, pdf, html (see \ref AVS_OFFICESTUDIO_FILE_DOCUMENT).
		 */
		int* mailFormat;
		/**
		 * @brief Email body.
		 */
		std::wstring* message;
		/**
		 * @brief Number of records.
		 */
		int* recordCount;
		/**
		 * @brief Starting record index.
		 */
		int* recordFrom;
		/**
		 * @brief Ending record index.
		 */
		int* recordTo;
		/**
		 * @brief Email subject.
		 */
		std::wstring* subject;
		/**
		 * @brief Recipient's email address.
		 */
		std::wstring* to;
		/**
		 * @brief URL.
		 */
		std::wstring* url;
		/**
		 * @brief User ID.
		 */
		std::wstring* userid;
		InputParamsMailMerge()
		{
			fileName = NULL;
			from = NULL;
			jsonKey = NULL;
			mailFormat = NULL;
			message = NULL;
			recordCount = NULL;
			recordFrom = NULL;
			recordTo = NULL;
			subject = NULL;
			to = NULL;
			url = NULL;
			userid = NULL;
		}
		~InputParamsMailMerge()
		{
			RELEASEOBJECT(fileName);
			RELEASEOBJECT(from);
			RELEASEOBJECT(jsonKey);
			RELEASEOBJECT(mailFormat);
			RELEASEOBJECT(message);
			RELEASEOBJECT(recordCount);
			RELEASEOBJECT(recordFrom);
			RELEASEOBJECT(recordTo);
			RELEASEOBJECT(subject);
			RELEASEOBJECT(to);
			RELEASEOBJECT(url);
			RELEASEOBJECT(userid);
		}

		bool FromXmlNode(XmlUtils::CXmlNode& oNode)
		{
			std::vector<XmlUtils::CXmlNode> oXmlNodes;
			if (TRUE == oNode.GetChilds(oXmlNodes))
			{
				for (size_t i = 0; i < oXmlNodes.size(); ++i)
				{
					XmlUtils::CXmlNode& oXmlNode = oXmlNodes[i];
					if (oXmlNode.IsValid())
					{
						std::wstring sValue;
						if (oXmlNode.GetTextIfExist(sValue))
						{
							std::wstring sName = oXmlNode.GetName();

							if (_T("fileName") == sName)
								fileName = new std::wstring(sValue);
							else if (_T("from") == sName)
								from = new std::wstring(sValue);
							else if (_T("jsonKey") == sName)
								jsonKey = new std::wstring(sValue);
							else if (_T("mailFormat") == sName)
								mailFormat = new int(XmlUtils::GetInteger(sValue));
							else if (_T("message") == sName)
								message = new std::wstring(sValue);
							else if (_T("recordCount") == sName)
								recordCount = new int(XmlUtils::GetInteger(sValue));
							else if (_T("recordFrom") == sName)
								recordFrom = new int(XmlUtils::GetInteger(sValue));
							else if (_T("recordTo") == sName)
								recordTo = new int(XmlUtils::GetInteger(sValue));
							else if (_T("subject") == sName)
								subject = new std::wstring(sValue);
							else if (_T("to") == sName)
								to = new std::wstring(sValue);
							else if (_T("url") == sName)
								url = new std::wstring(sValue);
							else if (_T("userid") == sName)
								userid = new std::wstring(sValue);
						}
					}
				}
			}
			return true;
		}
	};

	/**
	 * @brief Thumbnail generation parameters.
	 * @details Parsed from the <m_oThumbnail> element in the input XML.
	 *          Members map to child nodes: format, aspect, first, zip, width, height.
	 */
	class InputParamsThumbnail
	{
	public:
		/** @brief Output image format for thumbnails (bmp = 1, gif = 2, jpg = 3, png = 4). */
		int* format;
		/** @brief Fit mode: 0=stretch to width/height; 1=keep aspect; 2=use page metrics at 96 dpi (default). */
		int* aspect;
		/** @brief If true, generate a thumbnail for the first page only (default). If false, generate for all pages. */
		bool* first;
		/** @brief When generating thumbnails for all pages (first=false), output a ZIP archive with per-page images. */
		bool* zip;
		/** @brief Thumbnail width in pixels (default: 100). */
		int* width;
		/** @brief Thumbnail height in pixels (default: 100). */
		int* height;
		InputParamsThumbnail()
		{
			format = NULL;
			aspect = NULL;
			first = NULL;
			zip = NULL;
			width = NULL;
			height = NULL;
		}
		~InputParamsThumbnail()
		{
			RELEASEOBJECT(format);
			RELEASEOBJECT(aspect);
			RELEASEOBJECT(first);
			RELEASEOBJECT(zip);
			RELEASEOBJECT(width);
			RELEASEOBJECT(height);
		}

		bool FromXmlNode(XmlUtils::CXmlNode& oNode)
		{
			std::vector<XmlUtils::CXmlNode> oXmlNodes;
			if (TRUE == oNode.GetChilds(oXmlNodes))
			{
				for (size_t i = 0; i < oXmlNodes.size(); ++i)
				{
					XmlUtils::CXmlNode& oXmlNode = oXmlNodes[i];
					if (oXmlNode.IsValid())
					{
						std::wstring sValue;
						if (oXmlNode.GetTextIfExist(sValue))
						{
							std::wstring sName = oXmlNode.GetName();
							if (_T("format") == sName)
								format = new int(XmlUtils::GetInteger(sValue));
							else if (_T("aspect") == sName)
								aspect = new int(XmlUtils::GetInteger(sValue));
							else if (_T("first") == sName)
								first = new bool(XmlUtils::GetBoolean2(sValue));
							else if (_T("zip") == sName)
								zip = new bool(XmlUtils::GetBoolean2(sValue));
							else if (_T("width") == sName)
								width = new int(XmlUtils::GetInteger(sValue));
							else if (_T("height") == sName)
								height = new int(XmlUtils::GetInteger(sValue));
						}
					}
				}
			}
			return true;
		}
	};

	/**
	 * @brief Text-conversion related parameters.
	 * @details Parsed from the <m_oTextParams> element in the input XML.
	 */
	class InputParamsText
	{
	public:
		/** @brief [Optional] Text association type (see \ref NSDocxRenderer::TextAssociationType). XML: <m_nTextAssociationType>. */
		int* m_nTextAssociationType;
		InputParamsText()
		{
			m_nTextAssociationType = NULL;
		}
		~InputParamsText()
		{
			RELEASEOBJECT(m_nTextAssociationType);
		}

		bool FromXmlNode(XmlUtils::CXmlNode& oNode)
		{
			std::vector<XmlUtils::CXmlNode> oXmlNodes;
			if (TRUE == oNode.GetChilds(oXmlNodes))
			{
				for (size_t i = 0; i < oXmlNodes.size(); ++i)
				{
					XmlUtils::CXmlNode& oXmlNode = oXmlNodes[i];
					if (oXmlNode.IsValid())
					{
						std::wstring sValue;
						if (oXmlNode.GetTextIfExist(sValue))
						{
							std::wstring sName = oXmlNode.GetName();

							if (_T("m_nTextAssociationType") == sName)
								m_nTextAssociationType = new int(XmlUtils::GetInteger(sValue));
						}
					}
				}
			}
			return true;
		}
	};

	/**
	 * @brief Input constraints for particular input file types.
	 * @details Used for ZIP entries: compressed/uncompressed sizes and a name template.
	 */
	class InputLimit
	{
	public:
		/** @brief [Optional] Defines the total compressed file size for text documents/spreadsheets/presentations. */
		ULONG64 compressed;
		/** @brief [Optional] Defines the total uncompressed file size for text documents/spreadsheets/presentations. */
		ULONG64 uncompressed;
		/** @brief [Optional] Defines the name template for files which sizes are counted.(Example: "*.xml") */
		std::wstring pattern;
		InputLimit()
		{
			compressed = 0;
			uncompressed = 0;
		}
	};

	class ConvertParams
	{
	public:
		std::wstring m_sTempDir;
		std::wstring m_sThemesDir;

		std::wstring m_sEditorWithChanges;

		// [docx_dir2doct_bin, xlsx_dir2xlst_bin, pptx_dir2pptt_bin] methods
		std::wstring m_sTempParamOOXMLFile;

		// for doct_bin2docx_dir, xlst_bin2xlsx_dir, pptt_bin2pptx_dir
		std::wstring m_sTempResultOOXMLDirectory;

		// xlsx_dir2xlst_bin
		bool m_bTempIsXmlOptions = false;

		bool m_bIsTemplate = false;
		bool m_bIsPaid = false;

		std::wstring m_sMediaDirectory;
		std::wstring m_sInternalMediaDirectory;

		std::string m_sPrintPages;

		std::wstring m_sPdfOformMetaName;
		std::wstring m_sPdfOformMetaData;
	};

	/**
	 * @brief Container for all input parameters parsed from XML.
	 * @details Holds file paths, formats, CSV/TXT options, mail merge, thumbnail and
	 *          text parameters, passwords, temporary paths, input limits, and flags
	 *          controlling conversion behavior.
	 */
	class InputParams
	{
	public:
		/// \name File paths and metadata
		/// @{
		/** @brief [Optional] Contains the document ID. For debugging purposes only. XML: <m_sKey>. */
		std::wstring* m_sKey;
		/** @brief [Required] Source file path. XML: <m_sFileFrom>. */
		std::wstring* m_sFileFrom;
		/** @brief [Required] Destination file path. XML: <m_sFileTo>. */
		std::wstring* m_sFileTo;
		/** @brief [Optional] Document title. Used for metadata in some output formats. XML: <m_sTitle>. */
		std::wstring* m_sTitle;
		/// @}

		/// \name Formats
		/// @{
		/** @brief [Optional] Source format code. See \ref AVS_OFFICESTUDIO_FILE_DOCUMENT and related macros. Inferred from the file body or name when not specified. XML: <m_nFormatFrom>. */
		int* m_nFormatFrom;
		/** @brief [Required] Destination format code. See \ref AVS_OFFICESTUDIO_FILE_DOCUMENT and related macros. XML: <m_nFormatTo>. */
		int* m_nFormatTo;
		/// @}

		/// \name CSV/TXT options
		/// @{
		/** @brief [Optional] Character encoding (code page) for CSV/TXT (see \ref NSUnicodeConverter::Encodings "Encodings table"). XML: <m_nCsvTxtEncoding>. */
		int* m_nCsvTxtEncoding;
		/** @brief [Optional] CSV delimiter type (see \ref TCsvDelimiter). XML: <m_nCsvDelimiter>. */
		int* m_nCsvDelimiter;
		/** @brief [Optional] Custom delimiter character(s); overrides m_nCsvDelimiter. XML: <m_nCsvDelimiterChar>. */
		std::wstring* m_sCsvDelimiterChar;
		/** @brief [Optional] Locale identifier (LCID) used in DOC and RTF to interpret strings when not specified in the file. XML: <m_nLcid>. */
		int* m_nLcid;
		/// @}

		/// \name Processing flags
		/// @{
		/** @brief [Deprecated] Paid conversion flag. XML: <m_bPaid>. */
		bool* m_bPaid;
		/** @brief [Optional] Apply changes from the "changes" subfolder if present. XML: <m_bFromChanges>. */
		bool* m_bFromChanges;
		/** @brief [Optional] Suppress saving additional artifacts (such as the original file and changes.zip). XML: <m_bDontSaveAdditional>. */
		bool* m_bDontSaveAdditional;
		/// @}

		/// \name Fonts and themes
		/// @{
		/** @brief [Deprecated] Path to AllFonts.js. XML: <m_sAllFontsPath>. */
		std::wstring* m_sAllFontsPath;
		/** @brief [Optional] Path to the directory containing font_selection.bin. XML: <m_sFontDir>. */
		std::wstring* m_sFontDir;
		/** @brief [Optional] Built-in themes directory path. XML: <m_sThemeDir>. */
		std::wstring* m_sThemeDir;
		/// @}

		/// \name Nested parameter objects
		/// @{
		/** @brief [Optional] Mail merge configuration. XML: <m_oMailMergeSend>. See \ref InputParamsMailMerge. */
		InputParamsMailMerge* m_oMailMergeSend;
		/** @brief [Optional] Thumbnail configuration. XML: <m_oThumbnail>. See \ref InputParamsThumbnail. */
		InputParamsThumbnail* m_oThumbnail;
		/** @brief [Optional] Text configuration. XML: <m_oTextParams>. See \ref InputParamsText. */
		InputParamsText* m_oTextParams;
		/// @}

		/// \name Additional input options
		/// @{
		/** @brief [Optional] JSON renderer options; supports keys like documentLayout, spreadsheetLayout, and watermark. See https://api.onlyoffice.com/docs/docs-api/additional-api/conversion-api/request/. XML: <m_sJsonParams>. */
		std::wstring* m_sJsonParams;
		/// @}

		/// \name Security and identifiers
		/// @{
		/** @brief [Optional] Source file open password. XML: <m_sPassword>. */
		std::wstring* m_sPassword;
		/** @brief [Optional] Destination file save password. XML: <m_sSavePassword>. */
		std::wstring* m_sSavePassword;
		/** @brief [Optional] Document identifier. XML: <m_sDocumentID>. */
		std::wstring* m_sDocumentID;
		/// @}

		/// \name Temp and encoding
		/// @{
		/** @brief [Optional] Explicit temp directory. XML: <m_sTempDir>. */
		std::wstring* m_sTempDir;
		/** @brief [Optional] Treat Editor.bin as not Base64-encoded when true (default when absent). XML: <m_bIsNoBase64>. */
		bool* m_bIsNoBase64;
		/// @}

		/// \name Input constraints
		/// @{
		/** @brief [Optional] Defines the OOXML file types for which the limits are specified (text documents/spreadsheets/presentations). This does not include other objects, like images. XML: <m_oInputLimits>; values are \ref InputLimit. */
		boost::unordered_map<int, std::vector<InputLimit>> m_mapInputLimits;
		/// @}

		/// \name Output preferences
		/// @{
		/** @brief [Optional] Request PDF/A output when applicable. XML: <m_bIsPDFA>. */
		bool* m_bIsPDFA;
		/** @brief [Optional] List of extensions to also produce in the original format (Open in browser). XML: <m_sConvertToOrigin>. */
		std::wstring* m_sConvertToOrigin;
		/// @}

		mutable bool m_bOutputConvertCorrupted;
		mutable bool m_bMacro;

	public:
		InputParams()
		{
			m_sKey = NULL;
			m_sFileFrom = NULL;
			m_sFileTo = NULL;
			m_sTitle = NULL;
			m_nFormatFrom = new int(AVS_OFFICESTUDIO_FILE_UNKNOWN);
			m_nFormatTo = NULL;
			m_nCsvTxtEncoding = NULL;
			m_nCsvDelimiter = NULL;
			m_sCsvDelimiterChar = NULL;
			m_nLcid = NULL;
			m_bPaid = NULL;
			m_bFromChanges = NULL;
			m_bDontSaveAdditional = NULL;
			m_sAllFontsPath = NULL;
			m_sFontDir = NULL;
			m_sThemeDir = NULL;
			m_oMailMergeSend = NULL;
			m_oThumbnail = NULL;
			m_oTextParams = NULL;
			m_sJsonParams = NULL;
			m_sPassword = NULL;
			m_sSavePassword = NULL;
			m_sDocumentID = NULL;
			m_sTempDir = NULL;
			m_bIsNoBase64 = NULL;
			m_bIsPDFA = NULL;
			m_sConvertToOrigin = NULL;

			m_bOutputConvertCorrupted = false;
			m_bMacro = false;
		}
		~InputParams()
		{
			RELEASEOBJECT(m_sKey);
			RELEASEOBJECT(m_sFileFrom);
			RELEASEOBJECT(m_sFileTo);
			RELEASEOBJECT(m_sTitle);
			RELEASEOBJECT(m_nFormatFrom);
			RELEASEOBJECT(m_nFormatTo);
			RELEASEOBJECT(m_nCsvTxtEncoding);
			RELEASEOBJECT(m_nCsvDelimiter);
			RELEASEOBJECT(m_sCsvDelimiterChar);
			RELEASEOBJECT(m_nLcid);
			RELEASEOBJECT(m_bPaid);
			RELEASEOBJECT(m_bFromChanges);
			RELEASEOBJECT(m_bDontSaveAdditional);
			RELEASEOBJECT(m_sAllFontsPath);
			RELEASEOBJECT(m_sFontDir);
			RELEASEOBJECT(m_sThemeDir);
			RELEASEOBJECT(m_oMailMergeSend);
			RELEASEOBJECT(m_oThumbnail);
			RELEASEOBJECT(m_oTextParams);
			RELEASEOBJECT(m_sJsonParams);
			RELEASEOBJECT(m_sPassword);
			RELEASEOBJECT(m_sSavePassword);
			RELEASEOBJECT(m_sDocumentID);
			RELEASEOBJECT(m_sTempDir);
			RELEASEOBJECT(m_bIsNoBase64);
			RELEASEOBJECT(m_bIsPDFA);
			RELEASEOBJECT(m_sConvertToOrigin);
		}

		bool FromXmlFile(const std::wstring& sFilename)
		{
			XmlUtils::CXmlNode oRoot;
			if (TRUE == oRoot.FromXmlFile(sFilename))
			{
				return FromXmlNode(oRoot);
			}
			else
			{
				return false;
			}
		}
		bool FromXml(const std::wstring& sXml)
		{
			XmlUtils::CXmlNode oRoot;
			if (TRUE == oRoot.FromXmlString(sXml))
			{
				return FromXmlNode(oRoot);
			}
			else
			{
				return false;
			}
		}
		bool FromXmlNode(XmlUtils::CXmlNode& oRoot)
		{
			std::vector<XmlUtils::CXmlNode> oXmlNodes;
			if (TRUE == oRoot.GetChilds(oXmlNodes))
			{
				for (size_t i = 0; i < oXmlNodes.size(); ++i)
				{
					XmlUtils::CXmlNode& oXmlNode = oXmlNodes[i];
					if (oXmlNode.IsValid())
					{
						std::wstring sName = oXmlNode.GetName();
						if (_T("m_oMailMergeSend") == sName)
						{
							RELEASEOBJECT(m_oMailMergeSend);
							m_oMailMergeSend = new InputParamsMailMerge();
							m_oMailMergeSend->FromXmlNode(oXmlNode);
						}
						else if (_T("m_oThumbnail") == sName)
						{
							RELEASEOBJECT(m_oThumbnail);
							m_oThumbnail = new InputParamsThumbnail();
							m_oThumbnail->FromXmlNode(oXmlNode);
						}
						else if (_T("m_oTextParams") == sName)
						{
							RELEASEOBJECT(m_oTextParams);
							m_oTextParams = new InputParamsText();
							m_oTextParams->FromXmlNode(oXmlNode);
						}
						else if (_T("m_oInputLimits") == sName)
						{
							FromLimitsNode(oXmlNode);
						}
						else if (_T("options") == sName)
						{
							NSProcessEnv::Load(oXmlNode);
						}
						else
						{
							std::wstring sValue;
							if (oXmlNode.GetTextIfExist(sValue))
							{
								if (_T("m_sKey") == sName)
								{
									RELEASEOBJECT(m_sKey);
									m_sKey = new std::wstring(sValue);
								}
								else if (_T("m_sFileFrom") == sName)
								{
									RELEASEOBJECT(m_sFileFrom);
									m_sFileFrom = new std::wstring(sValue);
								}
								else if (_T("m_sFileTo") == sName)
								{
									RELEASEOBJECT(m_sFileTo);
									m_sFileTo = new std::wstring(sValue);
								}
								else if (_T("m_sTitle") == sName)
								{
									RELEASEOBJECT(m_sTitle);
									m_sTitle = new std::wstring(sValue);
								}
								else if (_T("m_nFormatFrom") == sName)
								{
									RELEASEOBJECT(m_nFormatFrom);
									m_nFormatFrom = new int(XmlUtils::GetInteger(sValue));
								}
								else if (_T("m_nFormatTo") == sName)
								{
									RELEASEOBJECT(m_nFormatTo);
									m_nFormatTo = new int(XmlUtils::GetInteger(sValue));
								}
								else if (_T("m_nCsvTxtEncoding") == sName)
								{
									RELEASEOBJECT(m_nCsvTxtEncoding);
									m_nCsvTxtEncoding = new int(XmlUtils::GetInteger(sValue));
								}
								else if (_T("m_nCsvDelimiter") == sName)
								{
									RELEASEOBJECT(m_nCsvDelimiter);
									m_nCsvDelimiter = new int(XmlUtils::GetInteger(sValue));
								}
								else if (_T("m_nCsvDelimiterChar") == sName)
								{
									RELEASEOBJECT(m_sCsvDelimiterChar);
									m_sCsvDelimiterChar = new std::wstring(sValue);
								}
								else if (_T("m_nLcid") == sName)
								{
									RELEASEOBJECT(m_nLcid);
									m_nLcid = new int(XmlUtils::GetInteger(sValue));
								}
								else if (_T("m_bPaid") == sName)
								{
									RELEASEOBJECT(m_bPaid);
									m_bPaid = new bool(XmlUtils::GetBoolean2(sValue));
								}
								else if (_T("m_bFromChanges") == sName)
								{
									RELEASEOBJECT(m_bFromChanges);
									m_bFromChanges = new bool(XmlUtils::GetBoolean2(sValue));
								}
								else if (_T("m_sAllFontsPath") == sName)
								{
									RELEASEOBJECT(m_sAllFontsPath);
									m_sAllFontsPath = new std::wstring(sValue);
								}
								else if (_T("m_sFontDir") == sName)
								{
									RELEASEOBJECT(m_sFontDir);
									m_sFontDir = new std::wstring(sValue);
								}
								else if (_T("m_sThemeDir") == sName)
								{
									RELEASEOBJECT(m_sThemeDir);
									m_sThemeDir = new std::wstring(sValue);
								}
								else if (_T("m_bDontSaveAdditional") == sName)
								{
									RELEASEOBJECT(m_bDontSaveAdditional);
									m_bDontSaveAdditional = new bool(XmlUtils::GetBoolean2(sValue));
								}
								else if (_T("m_sJsonParams") == sName)
								{
									RELEASEOBJECT(m_sJsonParams);
									m_sJsonParams = new std::wstring(sValue);
								}
								else if (_T("m_sPassword") == sName)
								{
									RELEASEOBJECT(m_sPassword);
									m_sPassword = new std::wstring(sValue);
								}
								else if (_T("m_sSavePassword") == sName)
								{
									RELEASEOBJECT(m_sSavePassword);
									m_sSavePassword = new std::wstring(sValue);
								}
								else if (_T("m_sDocumentID") == sName)
								{
									RELEASEOBJECT(m_sDocumentID);
									m_sDocumentID = new std::wstring(sValue);
								}
								else if (_T("m_sTempDir") == sName)
								{
									RELEASEOBJECT(m_sTempDir);
									m_sTempDir = new std::wstring(sValue);
								}
								else if (_T("m_bIsNoBase64") == sName)
								{
									RELEASEOBJECT(m_bIsNoBase64);
									m_bIsNoBase64 = new bool(XmlUtils::GetBoolean2(sValue));
								}
								else if (_T("m_bIsPDFA") == sName)
								{
									RELEASEOBJECT(m_bIsPDFA);
									m_bIsPDFA = new bool(XmlUtils::GetBoolean2(sValue));
								}
								else if (_T("m_sConvertToOrigin") == sName)
								{
									RELEASEOBJECT(m_sConvertToOrigin);
									m_sConvertToOrigin = new std::wstring(sValue);
								}
							}
							else if (_T("m_nCsvDelimiterChar") == sName)
							{
								std::wstring sNil;
								if (!oXmlNode.GetAttributeIfExist(L"xsi:nil", sNil))
								{
									RELEASEOBJECT(m_sCsvDelimiterChar);
									m_sCsvDelimiterChar = new std::wstring(L"");
								}
							}
						}
					}
				}
			}
			return true;
		}

		bool FromLimitsNode(XmlUtils::CXmlNode& oXmlNode)
		{
			std::vector<XmlUtils::CXmlNode> oLimitsNode;
			if (oXmlNode.GetNodes(L"m_oInputLimit", oLimitsNode))
			{
				for (size_t i = 0; i < oLimitsNode.size(); ++i)
				{
					XmlUtils::CXmlNode& oLimitNode = oLimitsNode[i];
					if (oLimitNode.IsValid())
					{
						std::wstring sType;
						if (oLimitNode.GetAttributeIfExist(L"type", sType))
						{
							std::vector<std::wstring> aTypes;
							boost::algorithm::split(aTypes, sType, boost::algorithm::is_any_of(L";"), boost::algorithm::token_compress_on);

							InputLimit oLimit;
							XmlUtils::CXmlNode oZipNode;
							if (oLimitNode.GetNode(L"m_oZip", oZipNode))
							{
								oLimit.compressed = std::stoull(oZipNode.GetAttribute(L"compressed", L"0"));
								oLimit.uncompressed = std::stoull(oZipNode.GetAttribute(L"uncompressed", L"0"));
								oLimit.pattern = oZipNode.GetAttribute(L"template", L"");
							}
							for (size_t j = 0; j < aTypes.size(); ++j)
							{
								int nFormat = COfficeFileFormatChecker::GetFormatByExtension(L"." + aTypes[j]);
								m_mapInputLimits[nFormat].push_back(oLimit);
							}
						}
					}
				}
			}
			return true;
		}
		bool hasPassword() const
		{
			return NULL != m_sPassword;
		}
		std::wstring getPassword() const
		{
			return (NULL != m_sPassword) ? (*m_sPassword) : L"";
		}
		bool hasSavePassword() const
		{
			return NULL != m_sSavePassword;
		}
		std::wstring getSavePassword() const
		{
			return (NULL != m_sSavePassword) ? (*m_sSavePassword) : L"";
		}
		std::wstring getDocumentID() const
		{
			return (NULL != m_sDocumentID) ? (*m_sDocumentID) : L"";
		}
		std::wstring getFontPath() const
		{
			return (NULL != m_sFontDir) ? (*m_sFontDir) : L"";
		}
		bool getIsNoBase64() const
		{
			return (NULL != m_bIsNoBase64) ? (*m_bIsNoBase64) : true;
		}
		bool getIsPDFA() const
		{
			return (NULL != m_bIsPDFA) ? (*m_bIsPDFA) : false;
		}
		std::wstring getConvertToOrigin() const
		{
			return (NULL != m_sConvertToOrigin) ? (*m_sConvertToOrigin) : L"";
		}
		bool needConvertToOrigin(long nFormatFrom) const
		{
			COfficeFileFormatChecker FileFormatChecker;
			std::wstring sExt = FileFormatChecker.GetExtensionByType(nFormatFrom);
			size_t index = getConvertToOrigin().find(sExt);
			return std::wstring::npos != index;
		}
		std::wstring getXmlOptions()
		{
			std::wstring sRes;
			int nCsvEncoding = 46; // 65001 utf8
			std::wstring cDelimiter = L",";
            int LcId = -1;

			if (NULL != m_nCsvTxtEncoding)
				nCsvEncoding = *m_nCsvTxtEncoding;
			if (NULL != m_nCsvDelimiter)
			{
				switch (*m_nCsvDelimiter)
				{
				case TCSVD_TAB:
					cDelimiter = L"\t";
					break;
				case TCSVD_SEMICOLON:
					cDelimiter = L";";
					break;
				case TCSVD_COLON:
					cDelimiter = L":";
					break;
				case TCSVD_COMMA:
					cDelimiter = L",";
					break;
				case TCSVD_SPACE:
					cDelimiter = L" ";
					break;
				}
			}
			if (NULL != m_sCsvDelimiterChar)
			{
				cDelimiter = *m_sCsvDelimiterChar;
			}

            if(m_nLcid != NULL)
            {
                LcId = *m_nLcid;
            }

			int nFileType = 1;
			if (NULL != m_nFormatFrom && AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV == *m_nFormatFrom)
				nFileType = 2;
			else if (NULL != m_nFormatFrom && AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSB == *m_nFormatFrom)
				nFileType = 4;

			std::wstring sSaveType;
			if (NULL != m_nFormatTo)
			{
				if (AVS_OFFICESTUDIO_FILE_OTHER_JSON == *m_nFormatTo)
					sSaveType = _T(" saveFileType='3'");
				else if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV == *m_nFormatTo)
					nFileType = 2;
				else if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSB == *m_nFormatTo)
					nFileType = 4;
			}
			sRes = L"<xmlOptions><fileOptions fileType='" + std::to_wstring(nFileType);
			sRes += L"' codePage='" + std::to_wstring(nCsvEncoding);
			if (m_bMacro)
				sRes += L"' macro='1";
            if(LcId != -1)
                sRes += L"' Lcid='" + std::to_wstring(LcId);
			sRes += L"' delimiter='" + XmlUtils::EncodeXmlStringExtend(cDelimiter) + L"' " + sSaveType;
			sRes += L"/><TXTOptions><Encoding>" + std::to_wstring(nCsvEncoding) + L"</Encoding></TXTOptions></xmlOptions>";

			return sRes;
		}
		TConversionDirection getConversionDirection()
		{
			TConversionDirection eRes = TCD_AUTO;
			if ((NULL == m_nFormatFrom || AVS_OFFICESTUDIO_FILE_UNKNOWN == *m_nFormatFrom) && NULL == m_nFormatTo && NULL != m_sFileFrom && NULL != m_sFileTo)
			{
				eRes = getConversionDirectionFromExt(*m_sFileFrom, *m_sFileTo);
				if (TCD_ERROR != eRes)
					return eRes;
				COfficeFileFormatChecker FileFormatChecker;
				RELEASEOBJECT(m_nFormatFrom);
				m_nFormatFrom = new int(FileFormatChecker.GetFormatByExtension(L"." + NSFile::GetFileExtention(*m_sFileFrom)));
				RELEASEOBJECT(m_nFormatTo);
				m_nFormatTo = new int(FileFormatChecker.GetFormatByExtension(L"." + NSFile::GetFileExtention(*m_sFileTo)));
			}

			if (NULL != m_nFormatFrom && NULL != m_nFormatTo)
			{
				int nFormatFrom = *m_nFormatFrom;
				int nFormatTo = *m_nFormatTo;

				COfficeFileFormatChecker FileFormatChecker;

				if ((m_sFileFrom) && (FileFormatChecker.isOfficeFile(*m_sFileFrom)))
				{
					if (nFormatFrom != FileFormatChecker.nFileType && FileFormatChecker.nFileType != AVS_OFFICESTUDIO_FILE_UNKNOWN)
					{
						nFormatFrom = FileFormatChecker.nFileType;
						bool bOFormAsPdf = nFormatFrom == AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM_PDF && 
							NSProcessEnv::IsPresent(NSProcessEnv::Converter::gc_oformAsPdf) && 
							NSProcessEnv::GetBoolValue(NSProcessEnv::Converter::gc_oformAsPdf);
						if (bOFormAsPdf)
						{
							nFormatFrom = AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF;
						}
						
						*m_nFormatFrom = nFormatFrom;
						
						 changeFormatFromPrev(nFormatFrom); 
					}
				}
				eRes = processDownloadFile();
				if (TCD_AUTO != eRes)
					return eRes;

				if ((nFormatFrom == AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCXF ||
					 nFormatFrom == AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM ||
					 nFormatFrom == AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM_PDF) &&
					 nFormatTo == AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF)
				{
					nFormatTo = AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM_PDF;
					*m_nFormatTo = AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM_PDF;
				}

				if (NULL != m_oMailMergeSend)
					eRes = TCD_MAILMERGE;
				else if ((AVS_OFFICESTUDIO_FILE_DOCUMENT_XML == nFormatFrom) && 0 != (AVS_OFFICESTUDIO_FILE_OTHER & nFormatTo))
					eRes = TCD_NON_AUTO; // параметры требуют уточнений
				else if ((AVS_OFFICESTUDIO_FILE_DOCUMENT_XML == nFormatFrom) && (0 != (AVS_OFFICESTUDIO_FILE_SPREADSHEET & nFormatTo) || (AVS_OFFICESTUDIO_FILE_CANVAS_SPREADSHEET == nFormatTo)))
					eRes = TCD_SPREADSHEET2;
				else if (0 != (AVS_OFFICESTUDIO_FILE_DOCUMENT & nFormatFrom))
					eRes = TCD_DOCUMENT2;
				else if (0 != (AVS_OFFICESTUDIO_FILE_SPREADSHEET & nFormatFrom))
					eRes = TCD_SPREADSHEET2;
				else if (0 != (AVS_OFFICESTUDIO_FILE_PRESENTATION & nFormatFrom))
					eRes = TCD_PRESENTATION2;
				else if (0 != (AVS_OFFICESTUDIO_FILE_DRAW & nFormatFrom))
					eRes = TCD_DRAW2;
				else if (0 != (AVS_OFFICESTUDIO_FILE_TEAMLAB & nFormatFrom))
					eRes = TCD_T2;
				else if (AVS_OFFICESTUDIO_FILE_CANVAS_WORD == nFormatFrom)
					eRes = TCD_DOCT_BIN2;
				else if (AVS_OFFICESTUDIO_FILE_CANVAS_SPREADSHEET == nFormatFrom)
					eRes = TCD_XLST_BIN2;
				else if (AVS_OFFICESTUDIO_FILE_CANVAS_PRESENTATION == nFormatFrom)
					eRes = TCD_PPTT_BIN2;
				else if (AVS_OFFICESTUDIO_FILE_CANVAS_DRAW == nFormatFrom)
					eRes = TCD_VSDT_BIN2;
				else if (0 != (AVS_OFFICESTUDIO_FILE_CROSSPLATFORM & nFormatFrom))
					eRes = TCD_CROSSPLATFORM2;
				else if (AVS_OFFICESTUDIO_FILE_CANVAS_PDF == nFormatFrom)
					eRes = TCD_CANVAS_PDF2;
				else if (AVS_OFFICESTUDIO_FILE_OTHER_MS_OFFCRYPTO == nFormatFrom)
					eRes = TCD_MSCRYPT2;
				else if (AVS_OFFICESTUDIO_FILE_OTHER_MS_MITCRYPTO == nFormatFrom)
					eRes = TCD_MITCRYPT2;
				else if (AVS_OFFICESTUDIO_FILE_OTHER_ZIP == nFormatFrom && AVS_OFFICESTUDIO_FILE_UNKNOWN == nFormatTo)
					eRes = TCD_UNZIPDIR;
				else if (AVS_OFFICESTUDIO_FILE_OTHER_MS_VBAPROJECT == nFormatFrom && AVS_OFFICESTUDIO_FILE_UNKNOWN == nFormatTo)
					eRes = TCD_VBAPROJECT2XML;
				else if (AVS_OFFICESTUDIO_FILE_UNKNOWN == nFormatFrom && AVS_OFFICESTUDIO_FILE_OTHER_ZIP == nFormatTo)
					eRes = TCD_ZIPDIR;
			}
			else
				eRes = TCD_ERROR;
			return eRes;
		}
		TConversionDirection processDownloadFile()
		{
			TConversionDirection eRes = TCD_AUTO;
			int nFormatFrom = *m_nFormatFrom;
			if (AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT == nFormatFrom && NULL == m_nCsvTxtEncoding)
			{
				int nCodePage = getEncodingByContent();
				if (nCodePage < 0)
				{
					if (!getDontSaveAdditional())
					{
						copyOrigin(*m_sFileFrom, *m_sFileTo);
					}
					eRes = TCD_ERROR;
				}
				else
				{
					m_nCsvTxtEncoding = new int(nCodePage);
				}
			}
			else if (AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV == nFormatFrom && (NULL == m_nCsvTxtEncoding || (NULL == m_nCsvDelimiter && NULL == m_sCsvDelimiterChar)))
			{
				if (isEmptyFile())
				{
					if (!m_nCsvTxtEncoding)
					{
						m_nCsvTxtEncoding = new int(getEncodingByContent()); //-1 ???
					}
					if (!m_nCsvDelimiter && !m_sCsvDelimiterChar)
					{
						m_sCsvDelimiterChar = new std::wstring(L",");
					}
					eRes = TCD_ERROR; // ???
				}
				else
				{
					if (false/*not used*/ && !getDontSaveAdditional())
					{
						int nCodePage = getEncodingByContent();
						if (nCodePage < 0)
							nCodePage = 46; // 65001 Unicode (UTF-8)
						int nDelimiter = getDelimiterByContent();
						NSStringUtils::CStringBuilder oBuilder;
						oBuilder.WriteString(_T("{\"codepage\":"));
						oBuilder.AddInt(nCodePage);
						oBuilder.WriteString(_T(",\"delimiter\":"));
						oBuilder.AddInt(nDelimiter);
						oBuilder.WriteString(_T("}"));
						std::wstring sFilePath = NSSystemPath::GetDirectoryName(*m_sFileTo) + FILE_SEPARATOR_STR + _T("settings.json");
						NSFile::CFileBinary::SaveToFile(sFilePath, oBuilder.GetData());
					}

					if (!getDontSaveAdditional())
					{
						copyOrigin(*m_sFileFrom, *m_sFileTo);
					}
					eRes = TCD_ERROR;
				}
			}
			return eRes;
		}
		int isEmptyFile()
		{
			NSFile::CFileBinary file;
			return file.OpenFile(*m_sFileFrom) && 0 == file.GetFileSize();
		}
		int getEncodingByContent()
		{
			int nRes = -1;
			// todo использовать codepage
			NSFile::CFileBinary file;
			if (file.OpenFile(*m_sFileFrom))
			{
				DWORD dwBytesRead = 0;
				BYTE pBuffer[3];
				file.ReadFile(pBuffer, 3, dwBytesRead);
				if (dwBytesRead == 0)
				{
					// empty file
					nRes = 46; // 65001 Unicode (UTF-8)
				}
				if (dwBytesRead >= 2)
				{
					if (pBuffer[0] == 0xFF && pBuffer[1] == 0xFE)
						nRes = 48; // 1200 Unicode (UTF-16)
					else if (pBuffer[0] == 0xFE && pBuffer[1] == 0xFF)
						nRes = 49; // 1201 Unicode (UTF-16 Big Endian)
				}
				if (dwBytesRead >= 3)
				{
					if (pBuffer[0] == 0xEF && pBuffer[1] == 0xBB && pBuffer[2] == 0xBF)
						nRes = 46; // 65001 Unicode (UTF-8)
				}
				file.CloseFile();
			}
			return nRes;
		}
		int getDelimiterByContent()
		{
			int nRes = TCSVD_NONE;
			NSFile::CFileBinary file;
			if (file.OpenFile(*m_sFileFrom))
			{
				DWORD dwBytesRead = 0;
				BYTE pBuffer[1000];
				file.ReadFile(pBuffer, 1000, dwBytesRead);

				int nDelimitersCount = 6;
				int aDelimiters[6] = {0, 0, 0, 0, 0, 0};
				for (DWORD i = 0; i < dwBytesRead; ++i)
				{
					char cCurChar = pBuffer[i];
					if ('\n' == cCurChar)
					{
						break;
					}
					else if ('\t' == cCurChar)
						aDelimiters[TCSVD_TAB]++;
					else if (';' == cCurChar)
						aDelimiters[TCSVD_SEMICOLON]++;
					else if (':' == cCurChar)
						aDelimiters[TCSVD_COLON]++;
					else if (',' == cCurChar)
						aDelimiters[TCSVD_COMMA]++;
					else if (' ' == cCurChar)
						aDelimiters[TCSVD_SPACE]++;
				}
				int nMaxVal = 0;
				int nMaxIndex = 0;
				for (int i = 0; i < nDelimitersCount; ++i)
				{
					if (nMaxVal < aDelimiters[i])
					{
						nMaxVal = aDelimiters[i];
						nMaxIndex = i;
					}
				}
				nRes = nMaxIndex;

				file.CloseFile();
			}
			return nRes;
		}
		void changeFormatFromPrev(int formatFrom)
		{
			*m_nFormatFrom = formatFrom;
			int toFormat = *m_nFormatTo;

			if (AVS_OFFICESTUDIO_FILE_CANVAS == toFormat)
			{
				if (AVS_OFFICESTUDIO_FILE_TEAMLAB_XLSY == formatFrom || 0 != (AVS_OFFICESTUDIO_FILE_SPREADSHEET & formatFrom))
				{
					toFormat = AVS_OFFICESTUDIO_FILE_CANVAS_SPREADSHEET;
				}
				else if (AVS_OFFICESTUDIO_FILE_TEAMLAB_PPTY == formatFrom || 0 != (AVS_OFFICESTUDIO_FILE_PRESENTATION & formatFrom))
				{
					toFormat = AVS_OFFICESTUDIO_FILE_CANVAS_PRESENTATION;
				}
				else if (AVS_OFFICESTUDIO_FILE_TEAMLAB_DOCY == formatFrom || 0 != (AVS_OFFICESTUDIO_FILE_DOCUMENT & formatFrom))
				{
					toFormat = AVS_OFFICESTUDIO_FILE_CANVAS_WORD;
				}
			}
			else if (AVS_OFFICESTUDIO_FILE_OTHER_OOXML == toFormat || AVS_OFFICESTUDIO_FILE_OTHER_ODF == toFormat)
			{
				if (formatFrom == AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX ||
					formatFrom == AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX || 
					formatFrom == AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF ||
					formatFrom == AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT ||
					formatFrom == AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML ||
					formatFrom == AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT ||
					formatFrom == AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB ||
					formatFrom == AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2 ||
					formatFrom == AVS_OFFICESTUDIO_FILE_DOCUMENT_MOBI ||
					formatFrom == AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC_FLAT ||
					formatFrom == AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML_IN_CONTAINER ||
					formatFrom == AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM_PDF)
				{
					if (AVS_OFFICESTUDIO_FILE_OTHER_ODF == toFormat)
					{
						toFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT;
					}
					else
					{
						toFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX;
					}
				}
				else if (AVS_OFFICESTUDIO_FILE_CANVAS_SPREADSHEET == formatFrom || 
						AVS_OFFICESTUDIO_FILE_TEAMLAB_XLSY == formatFrom || 0 != (AVS_OFFICESTUDIO_FILE_SPREADSHEET & formatFrom))
				{
					if (AVS_OFFICESTUDIO_FILE_OTHER_ODF == toFormat)
					{
						toFormat = AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS;
					}
				}
				else if (AVS_OFFICESTUDIO_FILE_CANVAS_PRESENTATION == formatFrom || 
						AVS_OFFICESTUDIO_FILE_TEAMLAB_PPTY == formatFrom || 0 != (AVS_OFFICESTUDIO_FILE_PRESENTATION & formatFrom))
				{
					if (AVS_OFFICESTUDIO_FILE_OTHER_ODF == toFormat)
					{
						toFormat = AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP;
					}
				}
				else if (AVS_OFFICESTUDIO_FILE_DOCUMENT_XML == formatFrom)
				{
					return;
				}
				else if (AVS_OFFICESTUDIO_FILE_CANVAS_WORD == formatFrom || 
						AVS_OFFICESTUDIO_FILE_TEAMLAB_DOCY == formatFrom || 0 != (AVS_OFFICESTUDIO_FILE_DOCUMENT & formatFrom))
				{
					if (AVS_OFFICESTUDIO_FILE_OTHER_ODF == toFormat)
					{
						toFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT;
					}
				}
				COfficeFileFormatChecker FileFormatChecker;
				std::wstring sNewExt = FileFormatChecker.GetExtensionByType(toFormat);
				
				size_t nIndex = m_sFileTo->rfind('.');
				if (false == sNewExt.empty())
				{
					if (std::wstring::npos != nIndex)
						m_sFileTo->replace(nIndex, std::wstring::npos, sNewExt);
					else
						m_sFileTo->append(sNewExt);
				}
			}
			*m_nFormatTo = toFormat;
		}
		void changeFormatFromPost(int formatFrom, bool bMacroEnabled)
		{
			*m_nFormatFrom = formatFrom;
			int toFormat = *m_nFormatTo;

			if (AVS_OFFICESTUDIO_FILE_OTHER_OOXML == toFormat)
			{
				if (AVS_OFFICESTUDIO_FILE_CANVAS_SPREADSHEET == formatFrom || AVS_OFFICESTUDIO_FILE_TEAMLAB_XLSY == formatFrom || 0 != (AVS_OFFICESTUDIO_FILE_SPREADSHEET & formatFrom))
				{
					if (bMacroEnabled)
					{
						toFormat = AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM;
					}
					else
					{
						toFormat = AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX;
					}
				}
				else if (AVS_OFFICESTUDIO_FILE_CANVAS_PRESENTATION == formatFrom || AVS_OFFICESTUDIO_FILE_TEAMLAB_PPTY == formatFrom || 0 != (AVS_OFFICESTUDIO_FILE_PRESENTATION & formatFrom))
				{
					if (bMacroEnabled)
					{
						toFormat = AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM;
					}
					else
					{
						toFormat = AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX;
					}
				}
				else if (AVS_OFFICESTUDIO_FILE_CANVAS_WORD == formatFrom || AVS_OFFICESTUDIO_FILE_TEAMLAB_DOCY == formatFrom || 0 != (AVS_OFFICESTUDIO_FILE_DOCUMENT & formatFrom))
				{
					if (bMacroEnabled)
					{
						toFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM;
					}
					else
					{
						toFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX;
					}
				}
				size_t nIndex = m_sFileTo->rfind('.');
				COfficeFileFormatChecker FileFormatChecker;
				if (-1 != nIndex)
					m_sFileTo->replace(nIndex, std::wstring::npos, FileFormatChecker.GetExtensionByType(toFormat));
				else
					m_sFileTo->append(FileFormatChecker.GetExtensionByType(toFormat));
			}
			*m_nFormatTo = toFormat;
		}
		bool getDontSaveAdditional() const
		{
			return NULL != m_bDontSaveAdditional && *m_bDontSaveAdditional;
		}
		bool checkInputLimits();
		bool getFromChanges()
		{
			return (NULL != m_bFromChanges) ? (*m_bFromChanges) : false;
		}
		void setFromChanges(bool bVal)
		{
			RELEASEOBJECT(m_bFromChanges);
			m_bFromChanges = new bool(bVal);
		}
		std::wstring getTitle() const
		{
			return (NULL != m_sTitle) ? (*m_sTitle) : L"";
		}
	};

	// utils
	std::wstring string_replaceAll(std::wstring str, const std::wstring& from, const std::wstring& to);
	bool compare_string_by_length(const std::wstring& x, const std::wstring& y);

	std::wstring getMailMergeXml(const std::wstring& sJsonPath, int nRecordFrom, int nRecordTo, const std::wstring& sField);

	std::wstring getDoctXml(
		NSDoctRenderer::DoctRendererFormat::FormatFile eFromType,
		NSDoctRenderer::DoctRendererFormat::FormatFile eToType,
		const std::wstring& sTFileSrc, const std::wstring& sPdfBinFile,
		const std::wstring& sImagesDirectory, const std::wstring& sThemeDir,
		int nTopIndex,
		const std::wstring& sMailMerge,
		const InputParams& params);

	_UINT32 apply_changes(
		const std::wstring& sBinFrom, const std::wstring& sToResult,
		NSDoctRenderer::DoctRendererFormat::FormatFile eType,
		std::wstring& sBinTo,
		const InputParams& params, const ConvertParams& convertParams);

	bool copyImagesFromChanges(NSDoctRenderer::CDoctrenderer* pDoctRenderer,
							   const std::wstring& sSrcImagesDir, const std::wstring& sChangesDir,
							   const std::wstring& sResultDirectory);

} // namespace NExtractTools
#endif // CEXTRACTTOOLS_H
