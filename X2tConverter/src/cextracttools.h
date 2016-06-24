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
#ifndef CEXTRACTTOOLS_H
#define CEXTRACTTOOLS_H

#include "../../Common/OfficeFileErrorDescription.h"
#include "../../Common/OfficeFileFormatChecker.h"
#include "../../Common/DocxFormat/Source/SystemUtility/SystemUtility.h"
#include "../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"
#include "../../Common/DocxFormat/Source/XML/stringcommon.h"
#include "../../Common/DocxFormat/Source/XML/xmlutils.h"
#include "../../DesktopEditor/doctrenderer/doctrenderer.h"
#include "../../DesktopEditor/common/StringBuilder.h"
#include "../../DesktopEditor/common/Path.h"

#include <iostream>
#include <fstream>

#define SUCCEEDED_X2T(nRes) (0 == (nRes) || AVS_FILEUTILS_ERROR_CONVERT_CORRUPTED == (nRes))

namespace NExtractTools
{
    static const TCHAR* gc_sDoctRendererXml = _T("<Settings><SrcFileType>%d</SrcFileType><DstFileType>%d</DstFileType><SrcFilePath>%ls</SrcFilePath><DstFilePath>%ls</DstFilePath><FontsDirectory>%ls</FontsDirectory><ImagesDirectory>%ls</ImagesDirectory><ThemesDirectory>%ls</ThemesDirectory><Changes TopItem=\"%d\">%ls</Changes>%ls</Settings>");
    static const TCHAR* gc_sDoctRendererMailMergeXml = _T("<MailMergeData DatabasePath=\"%ls\" Start=\"%d\" End=\"%d\" Field=\"%ls\" />");

    typedef enum tagTConversionDirection
    {
        TCD_ERROR,
        TCD_AUTO,

        TCD_DOCX2DOCT,
        TCD_DOCT2DOCX,
        TCD_DOCX2DOCT_BIN,
        TCD_DOCT_BIN2DOCX,

        TCD_XLSX2XLST,
        TCD_XLST2XLSX,
        TCD_XLSX2XLST_BIN,
        TCD_XLST_BIN2XLSX,

        TCD_PPTX2PPTT,
        TCD_PPTT2PPTX,
        TCD_PPTX2PPTT_BIN,
        TCD_PPTT_BIN2PPTX,

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

        TCD_PPSX2PPTX,
//ppt 2
        TCD_PPT2PPTX,
        TCD_PPT2PPTT,
        TCD_PPT2PPTT_BIN,
//doc 2
        TCD_DOC2DOCT,
		TCD_DOC2DOCT_BIN,
		TCD_DOC2DOCX,
//doc 2
        TCD_XLS2XLST,
        TCD_XLS2XLST_BIN,
        TCD_XLS2XLSX,
//rtf 2
        TCD_RTF2DOCX,
		TCD_RTF2DOCT,
        TCD_RTF2DOCT_BIN,
//2 rtf
        TCD_DOCT2RTF,
        TCD_DOCX2RTF,
        TCD_DOCT_BIN2RTF,
//txt 2
		TCD_TXT2DOCX,
		TCD_TXT2DOCT,
		TCD_TXT2DOCT_BIN,
//2 txt
        TCD_DOCX2TXT,
        TCD_DOCT2TXT,
        TCD_DOCT_BIN2TXT,
//odf 2
        TCD_ODF2OOX,
        TCD_ODF2OOT,
        TCD_ODF2OOT_BIN,
//2 odt
        TCD_DOCX2ODT,
        TCD_DOCT2ODT,
        TCD_DOCT_BIN2ODT,
//2 ods
        TCD_XLSX2ODS,
        TCD_XLST2ODS,
        TCD_XLST_BIN2ODS,

        TCD_XML2DOCX,
        TCD_DOCX2XML,

        TCD_MAILMERGE,
        TCD_T2,
        TCD_DOCT_BIN2,
        TCD_XLST_BIN2,
        TCD_PPTT_BIN2,
        TCD_DOCUMENT2,
        TCD_SPREADSHEET2,
        TCD_PRESENTATION2,
        TCD_CROSSPLATFORM2,
        TCD_CANVAS_PDF2
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

    static bool copyOrigin(const std::wstring& sFileFrom, const std::wstring& sFileTo)
    {
        size_t nIndex = sFileFrom.rfind('.');
        return NSFile::CFileBinary::Copy(sFileFrom, NSSystemPath::GetDirectoryName(sFileTo) + FILE_SEPARATOR_STR + _T("origin") + sFileFrom.substr(nIndex));
    }

    class InputParamsMailMerge
    {
    public:
        std::wstring* fileName;
        std::wstring* from;
        std::wstring* jsonKey ;
        int* mailFormat;
        std::wstring* message;
        int* recordCount;
        int* recordFrom;
        int* recordTo;
        std::wstring* subject;
        std::wstring* to;
        std::wstring* url;
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
                XmlUtils::CXmlNodes oXmlNodes;
                if(TRUE == oNode.GetChilds(oXmlNodes))
                {
                    for(int i = 0; i < oXmlNodes.GetCount(); ++i)
                    {
                        XmlUtils::CXmlNode oXmlNode;
                        if(oXmlNodes.GetAt(i, oXmlNode))
                        {
                            CString sValue;
                            if(oXmlNode.GetTextIfExist(sValue))
                            {
                                std::wstring sName = oXmlNode.GetName();
                                if(_T("fileName") == sName)
                                    fileName = new std::wstring(sValue);
                                else if(_T("from") == sName)
                                    from = new std::wstring(sValue);
                                else if(_T("jsonKey") == sName)
                                    jsonKey = new std::wstring(sValue);
                                else if(_T("mailFormat") == sName)
                                    mailFormat = new int(XmlUtils::GetInteger(sValue));
                                else if(_T("message") == sName)
                                    message = new std::wstring(sValue);
                                else if(_T("recordCount") == sName)
                                    recordCount = new int(XmlUtils::GetInteger(sValue));
                                else if(_T("recordFrom") == sName)
                                    recordFrom = new int(XmlUtils::GetInteger(sValue));
                                else if(_T("recordTo") == sName)
                                    recordTo = new int(XmlUtils::GetInteger(sValue));
                                else if(_T("subject") == sName)
                                    subject = new std::wstring(sValue);
                                else if(_T("to") == sName)
                                    to = new std::wstring(sValue);
                                else if(_T("url") == sName)
                                    url = new std::wstring(sValue);
                                else if(_T("userid") == sName)
                                    userid = new std::wstring(sValue);
                            }
                        }
                    }
                }
            return true;
        }
    };

	class InputParams
	{
	public:
		std::wstring* m_sKey;
		std::wstring* m_sFileFrom;
		std::wstring* m_sFileTo;
		int* m_nFormatFrom;
		int* m_nFormatTo;
		int* m_nCsvTxtEncoding;
		int* m_nCsvDelimiter;
		bool* m_bPaid;
		bool* m_bFromChanges;
		bool* m_bDontSaveAdditional;
		std::wstring* m_sAllFontsPath;
		std::wstring* m_sFontDir;
		std::wstring* m_sThemeDir;
        InputParamsMailMerge* m_oMailMergeSend;
		int* m_nDoctParams;
		std::wstring* m_sHtmlFileInternalPath;
		std::wstring* m_sPassword;
	public:
		InputParams()
		{
			m_sKey = NULL;
			m_sFileFrom = NULL;
			m_sFileTo = NULL;
            m_nFormatFrom = new int(AVS_OFFICESTUDIO_FILE_UNKNOWN);
			m_nFormatTo = NULL;
			m_nCsvTxtEncoding = NULL;
			m_nCsvDelimiter = NULL;
			m_bPaid = NULL;
			m_bFromChanges = NULL;
			m_bDontSaveAdditional = NULL;
			m_sAllFontsPath = NULL;
			m_sFontDir = NULL;
			m_sThemeDir = NULL;
            m_oMailMergeSend = NULL;
			m_nDoctParams = NULL;
			m_sHtmlFileInternalPath = NULL;
			m_sPassword = NULL;
		}
		~InputParams()
		{
			RELEASEOBJECT(m_sKey);
			RELEASEOBJECT(m_sFileFrom);
			RELEASEOBJECT(m_sFileTo);
			RELEASEOBJECT(m_nFormatFrom);
			RELEASEOBJECT(m_nFormatTo);
			RELEASEOBJECT(m_nCsvTxtEncoding);
			RELEASEOBJECT(m_nCsvDelimiter);
			RELEASEOBJECT(m_bPaid);
			RELEASEOBJECT(m_bFromChanges);
			RELEASEOBJECT(m_bDontSaveAdditional);
			RELEASEOBJECT(m_sAllFontsPath);
			RELEASEOBJECT(m_sFontDir);
			RELEASEOBJECT(m_sThemeDir);
            RELEASEOBJECT(m_oMailMergeSend);
			RELEASEOBJECT(m_nDoctParams);
			RELEASEOBJECT(m_sHtmlFileInternalPath);
			RELEASEOBJECT(m_sPassword);
		}
		
		bool FromXmlFile(std::wstring sFilename)
		{
			XmlUtils::CXmlNode oRoot;
			if(TRUE == oRoot.FromXmlFile2(std_string2string(sFilename)))
			{
				XmlUtils::CXmlNodes oXmlNodes;
				if(TRUE == oRoot.GetChilds(oXmlNodes))
				{
					for(int i = 0; i < oXmlNodes.GetCount(); ++i)
					{
						XmlUtils::CXmlNode oXmlNode;
						if(oXmlNodes.GetAt(i, oXmlNode))
						{
                            std::wstring sName = oXmlNode.GetName();
                            if(_T("m_oMailMergeSend") == sName)
                            {
                                m_oMailMergeSend = new InputParamsMailMerge();
                                m_oMailMergeSend->FromXmlNode(oXmlNode);
                            }
                            else
                            {
                                CString sValue;
                                if(oXmlNode.GetTextIfExist(sValue))
                                {
                                    if(_T("m_sKey") == sName)
                                        m_sKey = new std::wstring(sValue);
                                    else if(_T("m_sFileFrom") == sName)
                                        m_sFileFrom = new std::wstring(sValue);
                                    else if(_T("m_sFileTo") == sName)
                                        m_sFileTo = new std::wstring(sValue);
                                    else if(_T("m_nFormatFrom") == sName)
                                        m_nFormatFrom = new int(XmlUtils::GetInteger(sValue));
                                    else if(_T("m_nFormatTo") == sName)
                                        m_nFormatTo = new int(XmlUtils::GetInteger(sValue));
                                    else if(_T("m_nCsvTxtEncoding") == sName)
                                        m_nCsvTxtEncoding = new int(XmlUtils::GetInteger(sValue));
                                    else if(_T("m_nCsvDelimiter") == sName)
                                        m_nCsvDelimiter = new int(XmlUtils::GetInteger(sValue));
                                    else if(_T("m_bPaid") == sName)
                                        m_bPaid = new bool(XmlUtils::GetBoolean2(sValue));
                                    else if(_T("m_bFromChanges") == sName)
                                        m_bFromChanges = new bool(XmlUtils::GetBoolean2(sValue));
                                    else if(_T("m_sAllFontsPath") == sName)
                                        m_sAllFontsPath = new std::wstring(sValue);
                                    else if(_T("m_sFontDir") == sName)
                                        m_sFontDir = new std::wstring(sValue);
                                    else if(_T("m_sThemeDir") == sName)
                                        m_sThemeDir = new std::wstring(sValue);
                                    else if(_T("m_bDontSaveAdditional") == sName)
                                        m_bDontSaveAdditional = new bool(XmlUtils::GetBoolean2(sValue));
                                    else if(_T("m_nDoctParams") == sName)
                                        m_nDoctParams = new int(XmlUtils::GetInteger(sValue));
                                    else if(_T("m_sHtmlFileInternalPath") == sName)
                                        m_sHtmlFileInternalPath = new std::wstring(sValue);
                                    else if(_T("m_sPassword") == sName)
                                        m_sPassword = new std::wstring(sValue);
                                }
                            }
						}
					}
				}
			}
			return true;
		}
		std::wstring getPassword() const
        {
            return (NULL != m_sPassword) ? (*m_sPassword) : L"";
        }

        std::wstring getXmlOptions()
		{
			CString sRes;
			int nCsvEncoding = 65001; //utf8
			char cDelimiter = ',';
			if(NULL != m_nCsvTxtEncoding)
				nCsvEncoding = *m_nCsvTxtEncoding;
			if(NULL != m_nCsvDelimiter)
			{
				switch (*m_nCsvDelimiter)
				{
				case TCSVD_TAB: cDelimiter = '\t'; break;
				case TCSVD_SEMICOLON: cDelimiter = ';'; break;
				case TCSVD_COLON: cDelimiter = ':'; break;
				case TCSVD_COMMA: cDelimiter = ','; break;
				case TCSVD_SPACE: cDelimiter = ' '; break;
				}
			}
            int nFileType = 1;
            if(NULL != m_nFormatFrom && AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV == *m_nFormatFrom)
                nFileType = 2;
            CString sSaveType;
            if(NULL != m_nFormatTo)
            {
                if(AVS_OFFICESTUDIO_FILE_OTHER_JSON == *m_nFormatTo)
                    sSaveType = _T(" saveFileType='3'");
                else if(AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV == *m_nFormatTo)
                    nFileType = 2;
            }
            sRes.Format(_T("<xmlOptions><fileOptions fileType='%d' codePage='%d' delimiter='%c' %ls /><TXTOptions><Encoding>%d</Encoding></TXTOptions></xmlOptions>"), nFileType, nCsvEncoding, cDelimiter, sSaveType, nCsvEncoding);
            return sRes.GetBuffer();
		}
		TConversionDirection getConversionDirection()
		{
			TConversionDirection eRes = TCD_AUTO;

			if(NULL != m_nFormatFrom && NULL != m_nFormatTo)
			{
				int nFormatFrom	= *m_nFormatFrom;
				int nFormatTo	= *m_nFormatTo;

                COfficeFileFormatChecker FileFormatChecker;

                if ((m_sFileFrom) && (FileFormatChecker.isOfficeFile(*m_sFileFrom)))
                {
                    if (nFormatFrom != FileFormatChecker.nFileType &&
                                FileFormatChecker.nFileType != AVS_OFFICESTUDIO_FILE_UNKNOWN)
                    {
                        nFormatFrom = FileFormatChecker.nFileType;
                        *m_nFormatFrom = nFormatFrom;
                    }
                }
                eRes = processDownloadFile();
                if(TCD_AUTO != eRes)
                    return eRes;
                processInnerFormats();

                if(NULL != m_oMailMergeSend)
                    eRes = TCD_MAILMERGE;
                else if(0 != (AVS_OFFICESTUDIO_FILE_DOCUMENT & nFormatFrom))
                    eRes = TCD_DOCUMENT2;
                else if(0 != (AVS_OFFICESTUDIO_FILE_SPREADSHEET & nFormatFrom))
                    eRes = TCD_SPREADSHEET2;
                else if(0 != (AVS_OFFICESTUDIO_FILE_PRESENTATION & nFormatFrom))
                    eRes = TCD_PRESENTATION2;
                else if(0 != (AVS_OFFICESTUDIO_FILE_TEAMLAB & nFormatFrom))
                    eRes = TCD_T2;
                else if(AVS_OFFICESTUDIO_FILE_CANVAS_WORD == nFormatFrom)
                    eRes = TCD_DOCT_BIN2;
                else if(AVS_OFFICESTUDIO_FILE_CANVAS_SPREADSHEET == nFormatFrom)
                    eRes = TCD_XLST_BIN2;
                else if(AVS_OFFICESTUDIO_FILE_CANVAS_PRESENTATION == nFormatFrom)
                    eRes = TCD_PPTT_BIN2;
                else if(0 != (AVS_OFFICESTUDIO_FILE_CROSSPLATFORM & nFormatFrom))
                    eRes = TCD_CROSSPLATFORM2;
                else if(AVS_OFFICESTUDIO_FILE_CANVAS_PDF == nFormatFrom)
                    eRes = TCD_CANVAS_PDF2;
                else if(AVS_OFFICESTUDIO_FILE_OTHER_ZIP			== nFormatFrom	&& AVS_OFFICESTUDIO_FILE_UNKNOWN			== nFormatTo)
                    eRes = TCD_UNZIPDIR;
                else if(AVS_OFFICESTUDIO_FILE_UNKNOWN			== nFormatFrom	&& AVS_OFFICESTUDIO_FILE_OTHER_ZIP			== nFormatTo)
                    eRes = TCD_ZIPDIR;
            }
			else if(NULL != m_sFileFrom && NULL != m_sFileTo)
			{
				eRes = TCD_AUTO;
			}
			else
				eRes = TCD_ERROR;
			return eRes;
		}
        TConversionDirection processDownloadFile()
        {
            TConversionDirection eRes = TCD_AUTO;
            int nFormatFrom	= *m_nFormatFrom;
            if(AVS_OFFICESTUDIO_FILE_OTHER_MS_OFFCRYPTO == nFormatFrom)
                eRes = TCD_ERROR;
            else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT == nFormatFrom && NULL == m_nCsvTxtEncoding)
            {
                int nCodePage = getEncodingByContent();
                if(nCodePage < 0)
                {
                    if(!getDontSaveAdditional())
                    {
                        copyOrigin(*m_sFileFrom, *m_sFileTo);
                    }
                    eRes = TCD_ERROR;
                }
            }
            else if(AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV == nFormatFrom && (NULL == m_nCsvTxtEncoding || NULL == m_nCsvDelimiter))
            {
                if(!getDontSaveAdditional())
                {
                    int nCodePage = getEncodingByContent();
                    if(nCodePage < 0)
                        nCodePage = 46;//65001 Unicode (UTF-8)
                    int nDelimiter = getDelimiterByContent();
                    NSStringUtils::CStringBuilder oBuilder;
                    oBuilder.WriteString(_T("{\"codepage\":"));
                    oBuilder.AddInt(nCodePage);
                    oBuilder.WriteString(_T(",\"delimiter\":"));
                    oBuilder.AddInt(nDelimiter);
                    oBuilder.WriteString(_T("}"));
                    std::wstring sFilePath = NSSystemPath::GetDirectoryName(*m_sFileTo) + FILE_SEPARATOR_STR + _T("settings.json");
                    NSFile::CFileBinary::SaveToFile(sFilePath, oBuilder.GetData());
                    copyOrigin(*m_sFileFrom, *m_sFileTo);
                }
                eRes = TCD_ERROR;
            }
            return eRes;
        }
        int getEncodingByContent()
        {
            int nRes = -1;
            //todo использовать codepage
            NSFile::CFileBinary file;
            if(file.OpenFile(*m_sFileFrom))
            {
                DWORD dwBytesRead = 0;
                BYTE pBuffer[3];
                file.ReadFile(pBuffer, 3, dwBytesRead);
                if(dwBytesRead >= 2)
                {
                    if (pBuffer[0] == 0xFF && pBuffer[1] == 0xFE)
                        nRes = 48;//1200 Unicode (UTF-16)
                    else if (pBuffer[0] == 0xFE && pBuffer[1] == 0xFF)
                        nRes = 49;//1201 Unicode (UTF-16 Big Endian)
                }
                if(dwBytesRead >= 3)
                {
                    if (pBuffer[0] == 0xEF && pBuffer[1] == 0xBB && pBuffer[2] == 0xBF)
                        nRes = 46;//65001 Unicode (UTF-8)
                }
                file.CloseFile();
            }
            return nRes;
        }
        int getDelimiterByContent()
        {
            int nRes = TCSVD_NONE;
            NSFile::CFileBinary file;
            if(file.OpenFile(*m_sFileFrom))
            {
                DWORD dwBytesRead = 0;
                BYTE pBuffer[1000];
                file.ReadFile(pBuffer, 1000, dwBytesRead);

                int nDelimitersCount = 6;
                int aDelimiters[6] = { 0, 0, 0, 0, 0, 0 };
                for(int i = 0; i < dwBytesRead; ++i)
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
                for(int i = 0; i < nDelimitersCount; ++i)
                {
                    if(nMaxVal < aDelimiters[i])
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
        void processInnerFormats()
        {
          int toFormat = *m_nFormatTo;
          int formatFrom = *m_nFormatFrom;

          if (AVS_OFFICESTUDIO_FILE_CANVAS == toFormat) {
            if (AVS_OFFICESTUDIO_FILE_TEAMLAB_XLSY == formatFrom || 0 != (AVS_OFFICESTUDIO_FILE_SPREADSHEET & formatFrom)) {
              toFormat = AVS_OFFICESTUDIO_FILE_CANVAS_SPREADSHEET;
            } else if (AVS_OFFICESTUDIO_FILE_TEAMLAB_PPTY == formatFrom || 0 != (AVS_OFFICESTUDIO_FILE_PRESENTATION & formatFrom)) {
              toFormat = AVS_OFFICESTUDIO_FILE_CANVAS_PRESENTATION;
            } else {
              toFormat = AVS_OFFICESTUDIO_FILE_CANVAS_WORD;
            }
          } else if (AVS_OFFICESTUDIO_FILE_OTHER_TEAMLAB_INNER == toFormat) {
            if (AVS_OFFICESTUDIO_FILE_CANVAS_SPREADSHEET == formatFrom || AVS_OFFICESTUDIO_FILE_TEAMLAB_XLSY == formatFrom || 0 != (AVS_OFFICESTUDIO_FILE_SPREADSHEET & formatFrom)) {
              toFormat = AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX;
            } else if (AVS_OFFICESTUDIO_FILE_CANVAS_PRESENTATION == formatFrom || AVS_OFFICESTUDIO_FILE_TEAMLAB_PPTY == formatFrom || 0 != (AVS_OFFICESTUDIO_FILE_PRESENTATION & formatFrom)) {
              toFormat = AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX;
            } else {
              toFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX;
            }
            size_t nIndex = m_sFileTo->rfind('.');
            COfficeFileFormatChecker FileFormatChecker;
            if(-1 != nIndex)
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
	};

    static std::wstring string_replaceAll(std::wstring str, const std::wstring& from, const std::wstring& to)
    {
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::wstring::npos)
        {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        return str;
    }
	static int getReturnErrorCode(int nDefine)
	{
        return 0 == nDefine ? 0 : nDefine - AVS_ERROR_FIRST - AVS_FILEUTILS_ERROR_FIRST;
	}
	
    static std::wstring getXMLOptionsFromFile(std::wstring xmlFileName)
	{
        std::wstring sXMLOptions;

		std::wifstream options_stream;
#if defined(_WIN32) || defined (_WIN64)
        options_stream.open(xmlFileName.c_str());
#else
        options_stream.open(stringWstingToUtf8String(xmlFileName));
#endif
		if (options_stream.is_open())
		{		
			while(true)
			{
				std::wstring line;
                std::getline(options_stream,line);
				
				if (line.size() < 1)break;
                sXMLOptions += line;
			}
			options_stream.close();
		}
		return sXMLOptions;
	}

	static const TConversionDirection getConversionDirection (const std::wstring &sArg3)
    {
        TConversionDirection res = TCD_ERROR;
        if (0 == sArg3.compare(_T("auto"))) {
            res = TCD_AUTO;
        }
        else if (0 == sArg3.compare(_T("docx2doct"))) {
            res = TCD_DOCX2DOCT;
        }
        else if (0 == sArg3.compare(_T("doct2docx"))) {
            res = TCD_DOCT2DOCX;
        }
        else if (0 == sArg3.compare(_T("docx2doct_bin"))) {
            res = TCD_DOCX2DOCT_BIN;
        }
        else if (0 == sArg3.compare(_T("doct_bin2docx"))) {
            res = TCD_DOCT_BIN2DOCX;
        }
        else if (0 == sArg3.compare(_T("xslx2xlst"))) {
            res = TCD_XLSX2XLST;
        }
        else if (0 == sArg3.compare(_T("xslt2xlsx"))) {
            res = TCD_XLST2XLSX;
        }
        else if (0 == sArg3.compare(_T("xslx2xlst_bin"))) {
            res = TCD_XLSX2XLST_BIN;
        }
        else if (0 == sArg3.compare(_T("xslt_bin2xlsx"))) {
            res = TCD_XLST_BIN2XLSX;
        }
        else if (0 == sArg3.compare(_T("pptx2pptt"))) {
            res = TCD_PPTX2PPTT;
        }
        else if (0 == sArg3.compare(_T("pptt2pptx"))) {
            res = TCD_PPTT2PPTX;
        }
        else if (0 == sArg3.compare(_T("pptx2pptt_bin"))) {
            res = TCD_PPTX2PPTT_BIN;
        }
        else if (0 == sArg3.compare(_T("pptt_bin2pptx"))) {
            res = TCD_PPTT_BIN2PPTX;
        }
        else if (0 == sArg3.compare(_T("zip2dir"))) {
            res = TCD_ZIPDIR;
        }
        else if (0 == sArg3.compare(_T("dir2zip"))) {
            res = TCD_UNZIPDIR;
        }
        else if (0 == sArg3.compare(_T("csv2xlsx"))) {
            res = TCD_CSV2XLSX;
        }
        else if (0 == sArg3.compare(_T("csv2xlst"))) {
            res = TCD_CSV2XLST;
        }
        else if (0 == sArg3.compare(_T("xlsx2csv"))) {
            res = TCD_XLSX2CSV;
        }
        else if (0 == sArg3.compare(_T("xlst2csv"))) {
            res = TCD_XLST2CSV;
        }
        else if (0 == sArg3.compare(_T("bin2pdf"))) {
            res = TCD_BIN2PDF;
        }
        else if (0 == sArg3.compare(_T("bin2t"))) {
            res = TCD_BIN2T;
        }
        else if (0 == sArg3.compare(_T("t2bin"))) {
            res = TCD_T2BIN;
        }
        else if (0 == sArg3.compare(_T("ppsx2pptx"))) {
            res = TCD_PPSX2PPTX;
        }
        else if (0 == sArg3.compare(_T("ppt2pptx"))) {
            res = TCD_PPT2PPTX;
        }
        else if (0 == sArg3.compare(_T("doc2docx"))) {
            res = TCD_DOC2DOCX;
        }
        else if (0 == sArg3.compare(_T("rtf2docx"))) {
            res = TCD_RTF2DOCX;
        }
        else if (0 == sArg3.compare(_T("docx2rtf"))) {
            res = TCD_DOCX2RTF;
        }
        else if (0 == sArg3.compare(_T("txt2docx"))) {
            res = TCD_TXT2DOCX;
        }
        else if (0 == sArg3.compare(_T("docx2txt"))) {
            res = TCD_DOCX2TXT;
        }
		return res;
    }

	const TConversionDirection getConversionDirectionFromExt (const std::wstring &sFile1, const std::wstring &sFile2);
    static bool compare_string_by_length (const std::wstring &x, const std::wstring &y)
	{
		if (!x.empty() && !y.empty())
		{
			if (x.length() == y.length())
                return x.compare(y) <= 0;
			else
                return ((int)(x.length()) - (int)(y.length())) <= 0;
		}
		else
		{
			if (!x.empty())
                return false;
			else if (!y.empty())
                return true;
		}
        return true;
	}
    std::wstring getMailMergeXml(const std::wstring& sJsonPath, int nRecordFrom, int nRecordTo, const std::wstring& sField);
    std::wstring getDoctXml(NSDoctRenderer::DoctRendererFormat::FormatFile eFromType, NSDoctRenderer::DoctRendererFormat::FormatFile eToType,
                            const std::wstring& sTFileDir, const std::wstring& sPdfBinFile, const std::wstring& sFontPath, const std::wstring& sImagesDirectory,
                            const std::wstring& sThemeDir, int nTopIndex, const std::wstring& sMailMerge, const InputParams& params);
    int apply_changes(const std::wstring &sBinFrom, const std::wstring &sToResult, NSDoctRenderer::DoctRendererFormat::FormatFile eType, const std::wstring &sFontDir, const std::wstring &sThemeDir, std::wstring &sBinTo, const InputParams& params);
}
#endif // CEXTRACTTOOLS_H
