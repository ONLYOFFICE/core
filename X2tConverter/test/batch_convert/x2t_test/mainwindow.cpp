#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

#include <string>
#include <map>
#include <thread>
#include <list>

#include <QDir>
#include <QDirIterator>

#include "../../../../Common/OfficeFileFormats.h"
#include "../../../../Common/OfficeFileFormatChecker.h"

#include "../../../../DesktopEditor/graphics/Timer.h"
#include "../../../../DesktopEditor/graphics/TemporaryCS.h"
#include "../../../../DesktopEditor/common/File.h"
#include "../../../../DesktopEditor/common/Directory.h"
#include "../../../../DesktopEditor/common/StringBuilder.h"

#include "../../../../DesktopEditor/fontengine/application_generate_fonts.h"

#include <QDateTime>

class CDirectoryParse : public NSThreads::CBaseThread
{
public:
    std::wstring                m_sDirectory;
    std::vector<std::wstring>   m_files;

    MainWindow* m_window;

    CDirectoryParse()
    {
        m_window = NULL;
    }
    virtual ~CDirectoryParse()
    {
    }

    virtual DWORD ThreadProc()
    {
        m_files = NSDirectory::GetFiles(m_sDirectory, true);

        // fonts!!!
        std::vector<std::string> strFonts;
        std::wstring strDirectory = NSFile::GetProcessDirectory() + L"/fonts";

        std::wstring strAllFontsJSPath = strDirectory + L"/AllFonts.js";
        std::wstring strThumbnailsFolder = strDirectory;
        std::wstring strFontsSelectionBin = strDirectory + L"/font_selection.bin";

        if (true)
        {
            NSFile::CFileBinary oFile;
            if (oFile.OpenFile(strDirectory + L"/fonts.log"))
            {
                int nSize = oFile.GetFileSize();
                char* pBuffer = new char[nSize];
                DWORD dwReaden = 0;
                oFile.ReadFile((BYTE*)pBuffer, nSize, dwReaden);
                oFile.CloseFile();

                int nStart = 0;
                int nCur = nStart;
                for (; nCur < nSize; ++nCur)
                {
                    if (pBuffer[nCur] == '\n')
                    {
                        int nEnd = nCur - 1;
                        if (nEnd > nStart)
                        {
                            std::string s(pBuffer + nStart, nEnd - nStart + 1);
                            strFonts.push_back(s);
                        }
                        nStart = nCur + 1;
                    }
                }

                delete[] pBuffer;
            }
        }

        CApplicationFonts oApplication;
        std::vector<std::wstring> strFontsW_Cur = oApplication.GetSetupFontFiles();

#if defined(_LINUX)
        std::wstring sHome = GetHomeDirectory();
        if (!sHome.empty())
        {
#ifdef _MAC
            NSDirectory::GetFiles2(sHome + L"/Library/Fonts", strFontsW_Cur, true);
#else
            NSDirectory::GetFiles2(sHome + L"/.fonts", strFontsW_Cur, true);
            NSDirectory::GetFiles2(sHome + L"/.local/share/fonts", strFontsW_Cur, true);
#endif
        }
#endif

        bool bIsEqual = true;
        if (strFonts.size() != strFontsW_Cur.size())
            bIsEqual = false;

        if (bIsEqual)
        {
            int nCount = (int)strFonts.size();
            for (int i = 0; i < nCount; ++i)
            {
                if (strFonts[i] != NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(strFontsW_Cur[i].c_str(), strFontsW_Cur[i].length()))
                {
                    bIsEqual = false;
                    break;
                }
            }
        }

        if (bIsEqual)
        {
            if (!NSFile::CFileBinary::Exists(strFontsSelectionBin))
                bIsEqual = false;
        }

        if (!bIsEqual)
        {
            if (NSFile::CFileBinary::Exists(strAllFontsJSPath))
                NSFile::CFileBinary::Remove(strAllFontsJSPath);
            if (NSFile::CFileBinary::Exists(strFontsSelectionBin))
                NSFile::CFileBinary::Remove(strFontsSelectionBin);

            if (strFonts.size() != 0)
                NSFile::CFileBinary::Remove(strDirectory + L"/fonts.log");

            NSFile::CFileBinary oFile;
            oFile.CreateFileW(strDirectory + L"/fonts.log");
            int nCount = (int)strFontsW_Cur.size();
            for (int i = 0; i < nCount; ++i)
            {
                oFile.WriteStringUTF8(strFontsW_Cur[i]);
                oFile.WriteFile((BYTE*)"\n", 1);
            }
            oFile.CloseFile();

            int nFlag = 0;
#if 1
            nFlag = 2;
#endif

            oApplication.InitializeFromArrayFiles(strFontsW_Cur, nFlag);

            NSCommon::SaveAllFontsJS(oApplication, strAllFontsJSPath, strThumbnailsFolder, strFontsSelectionBin);
        }

        m_bRunThread = FALSE;
        m_window->send_onDirectoryChecked();
        return 0;
    }
};

class CFormatLogger
{
public:
    std::wstring m_name;
    bool m_need;

    std::list<std::wstring> m_greenSrc;
    std::list<std::wstring> m_greenDst;

    std::list<std::wstring> m_yellowSrc;
    std::list<std::wstring> m_yellowDsc;

    std::list<std::wstring> m_redSrc;
    std::list<std::wstring> m_redDst;

public:
    CFormatLogger(const std::wstring& name)
    {
        m_name = name;
        clear();
    }
    ~CFormatLogger()
    {
    }

    void clear()
    {
        m_greenSrc.clear();
        m_greenDst.clear();

        m_yellowSrc.clear();
        m_yellowDsc.clear();

        m_redSrc.clear();
        m_redDst.clear();
    }

    void Add(const int& code, const std::wstring& src, const std::wstring& dst)
    {
        if (0 == code)
        {
            m_greenSrc.push_back(src);
            m_greenDst.push_back(dst);
        }
        else if (89 == code || 90 == code || 91 == code)
        {
            m_yellowSrc.push_back(src);
            m_yellowDsc.push_back(dst);
        }
        else
        {
            m_redSrc.push_back(src);
            m_redDst.push_back(dst);
        }
    }

    std::wstring GetMainHtml()
    {
        if (!m_need)
            return L"";

        int nCountG = m_greenSrc.size();
        int nCountY = m_yellowSrc.size();
        int nCountR = m_redSrc.size();

        int nCount = nCountG + nCountY + nCountR;

        if (0 == nCount)
            return L"";

        int nPercentY = 100 * nCountY / nCount; if (nCountY != 0 && nPercentY == 0) nPercentY = 1;
        int nPercentR = 100 * nCountR / nCount; if (nCountR != 0 && nPercentR == 0) nPercentR = 1;
        int nPercentG = 100 - nPercentY - nPercentR;


        NSStringUtils::CStringBuilder oBuilder;
        oBuilder.WriteString(L"<tr style=\"height:30px;\"><td style=\"width:200px;\">");

        oBuilder.WriteString(m_name);
        oBuilder.WriteString(L" (");
        oBuilder.AddInt(nPercentG);
        oBuilder.WriteString(L":");
        oBuilder.AddInt(nCountG);
        oBuilder.WriteString(L" | ");
        oBuilder.AddInt(nPercentY);
        oBuilder.WriteString(L":");
        oBuilder.AddInt(nCountY);
        oBuilder.WriteString(L" | ");
        oBuilder.AddInt(nPercentR);
        oBuilder.WriteString(L":");
        oBuilder.AddInt(nCountR);
        oBuilder.WriteString(L")</td><td style=\"width:800px;\">");

        oBuilder.WriteString(L"<div style=\"font-size: 0;width:800px;height:20px;margin:0;padding:0;\">");

        oBuilder.WriteString(L"<div style=\"display:inline-block;margin:0;padding:0;width:");
        oBuilder.AddInt(nPercentG);
        oBuilder.WriteString(L"%;height:100%;background:#00FF00;\"></div>");

        oBuilder.WriteString(L"<div style=\"display:inline-block;margin:0;padding:0;width:");
        oBuilder.AddInt(nPercentY);
        oBuilder.WriteString(L"%;height:100%;background:#FFFF00;\"></div>");

        oBuilder.WriteString(L"<div style=\"display:inline-block;margin:0;padding:0;width:");
        oBuilder.AddInt(nPercentR);
        oBuilder.WriteString(L"%;height:100%;background:#FF0000;\"></div>");

        oBuilder.WriteString(L"</div>");

        oBuilder.WriteString(L"</td></tr>");

        return oBuilder.GetData();
    }

    std::wstring GetHtml()
    {
        if (!m_need)
            return L"";

        int nCountG = m_greenSrc.size();
        int nCountY = m_yellowSrc.size();
        int nCountR = m_redSrc.size();

        int nCount = nCountG + nCountY + nCountR;

        if (0 == nCount)
            return L"";

        NSStringUtils::CStringBuilder oBuilder;
        oBuilder.WriteString(L"<table border=\"1\"><caption>");
        oBuilder.WriteString(m_name);
        oBuilder.WriteString(L"</caption>");

        std::list<std::wstring>::iterator i, j;
        for (i = m_greenSrc.begin(), j = m_greenDst.begin(); i != m_greenSrc.end(); i++, j++)
        {
            oBuilder.WriteString(L"<tr style=\"height:20px;background:#00FF00;\"><td style=\"width:500px;\">");
            oBuilder.WriteEncodeXmlString(*i);
            oBuilder.WriteString(L"</td><td style=\"width:500px;\">");
            oBuilder.WriteEncodeXmlString(*j);
            oBuilder.WriteString(L"</td></tr>");
        }
        for (i = m_yellowSrc.begin(), j = m_yellowDsc.begin(); i != m_yellowSrc.end(); i++, j++)
        {
            oBuilder.WriteString(L"<tr style=\"height:20px;background:#FFFF00;\"><td style=\"width:500px;\">");
            oBuilder.WriteEncodeXmlString(*i);
            oBuilder.WriteString(L"</td><td style=\"width:500px;\">");
            oBuilder.WriteEncodeXmlString(*j);
            oBuilder.WriteString(L"</td></tr>");
        }
        for (i = m_redSrc.begin(), j = m_redDst.begin(); i != m_redSrc.end(); i++, j++)
        {
            oBuilder.WriteString(L"<tr style=\"height:20px;background:#FF0000;\"><td style=\"width:500px;\">");
            oBuilder.WriteEncodeXmlString(*i);
            oBuilder.WriteString(L"</td><td style=\"width:500px;\">");
            oBuilder.WriteEncodeXmlString(*j);
            oBuilder.WriteString(L"</td></tr>");
        }

        oBuilder.WriteString(L"</table>");

        return oBuilder.GetData();
    }
};

class CConverter;
class CInternalWorker
{
public:
    std::map<int, bool> m_formats;
    std::map<int, CFormatLogger> m_logger;

    CDirectoryParse* m_pParser;

    int m_dst_format;

    int m_nCount;
    int m_nCurrent;
    int m_nCurrentConvert;

    NSCriticalSection::CRITICAL_SECTION m_oCS;

public:
    CInternalWorker()
    {
        m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX, true));
        m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC, true));
        m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT, true));
        m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF, true));
        m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT, true));
        m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML, true));

        m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX, true));
        m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX, true));
        m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT, true));
        m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP, true));

        m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX, true));
        m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS, true));
        m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS, true));
        m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV, true));

        m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF, true));
        m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU, true));
        m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS, true));

        m_logger.insert(std::make_pair<int, CFormatLogger>(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX, CFormatLogger(L"DOCX")));
        m_logger.insert(std::make_pair<int, CFormatLogger>(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC, CFormatLogger(L"DOC")));
        m_logger.insert(std::make_pair<int, CFormatLogger>(AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT, CFormatLogger(L"ODT")));
        m_logger.insert(std::make_pair<int, CFormatLogger>(AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF, CFormatLogger(L"RTF")));
        m_logger.insert(std::make_pair<int, CFormatLogger>(AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT, CFormatLogger(L"TXT")));
        m_logger.insert(std::make_pair<int, CFormatLogger>(AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML, CFormatLogger(L"HTML")));

        m_logger.insert(std::make_pair<int, CFormatLogger>(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX, CFormatLogger(L"PPTX")));
        m_logger.insert(std::make_pair<int, CFormatLogger>(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX, CFormatLogger(L"PPSX")));
        m_logger.insert(std::make_pair<int, CFormatLogger>(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT, CFormatLogger(L"PPT")));
        m_logger.insert(std::make_pair<int, CFormatLogger>(AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP, CFormatLogger(L"ODP")));

        m_logger.insert(std::make_pair<int, CFormatLogger>(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX, CFormatLogger(L"XLSX")));
        m_logger.insert(std::make_pair<int, CFormatLogger>(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS, CFormatLogger(L"XLS")));
        m_logger.insert(std::make_pair<int, CFormatLogger>(AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS, CFormatLogger(L"ODS")));
        m_logger.insert(std::make_pair<int, CFormatLogger>(AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV, CFormatLogger(L"CSV")));

        m_logger.insert(std::make_pair<int, CFormatLogger>(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF, CFormatLogger(L"PDF")));
        m_logger.insert(std::make_pair<int, CFormatLogger>(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU, CFormatLogger(L"DJVU")));
        m_logger.insert(std::make_pair<int, CFormatLogger>(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS, CFormatLogger(L"XPS")));

        m_pParser = new CDirectoryParse();

        m_dst_format = 0;

        m_nCount = 0;
        m_nCurrent = 0;

        m_oCS.InitializeCriticalSection();
    }
    ~CInternalWorker()
    {
        RELEASEOBJECT(m_pParser);

        m_oCS.DeleteCriticalSection();
    }

    void setDestinationInternal(bool isTrue = true)
    {
        if (isTrue)
        {
            m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML)->second = true;

            m_formats.find(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP)->second = true;

            m_formats.find(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV)->second = true;

            m_formats.find(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS)->second = true;
        }
    }

    void setDestinationDocument(bool isTrue = true)
    {
        if (isTrue)
        {
            m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML)->second = true;
        }

        m_formats.find(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX)->second = false;
        m_formats.find(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX)->second = false;
        m_formats.find(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT)->second = false;
        m_formats.find(AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP)->second = false;

        m_formats.find(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX)->second = false;
        m_formats.find(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS)->second = false;
        m_formats.find(AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS)->second = false;
        m_formats.find(AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV)->second = false;

        m_formats.find(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF)->second = false;
        m_formats.find(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU)->second = false;
        m_formats.find(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS)->second = false;
    }

    void setDestinationPresentation(bool isTrue = true)
    {
        m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX)->second = false;
        m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC)->second = false;
        m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT)->second = false;
        m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF)->second = false;
        m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT)->second = false;
        m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML)->second = false;

        if (isTrue)
        {
            m_formats.find(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP)->second = true;
        }

        m_formats.find(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX)->second = false;
        m_formats.find(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS)->second = false;
        m_formats.find(AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS)->second = false;
        m_formats.find(AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV)->second = false;

        m_formats.find(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF)->second = false;
        m_formats.find(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU)->second = false;
        m_formats.find(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS)->second = false;
    }

    void setDestinationSpreadsheet(bool isTrue = true)
    {
        m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX)->second = false;
        m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC)->second = false;
        m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT)->second = false;
        m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF)->second = false;
        m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT)->second = false;
        m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML)->second = false;

        m_formats.find(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX)->second = false;
        m_formats.find(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX)->second = false;
        m_formats.find(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT)->second = false;
        m_formats.find(AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP)->second = false;

        if (isTrue)
        {
            m_formats.find(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV)->second = true;
        }

        m_formats.find(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF)->second = false;
        m_formats.find(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU)->second = false;
        m_formats.find(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS)->second = false;
    }

    void setDestinationPDF(bool isTrue = true)
    {
        if (isTrue)
        {
            m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML)->second = true;

            m_formats.find(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP)->second = true;

            m_formats.find(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS)->second = true;
            m_formats.find(AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV)->second = true;
        }

        m_formats.find(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF)->second = false;
        m_formats.find(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU)->second = false;
        m_formats.find(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS)->second = false;
    }

    CConverter* GetNextConverter();
    void OnConvertFile(CConverter* pConverter, int nCode);
    void Start(int nCores);
    void Cancel();

    void Log()
    {
        NSStringUtils::CStringBuilder oBuilder;

        oBuilder.WriteString(L"<!DOCTYPE html>\
<html style=\"background:#FFFFFF;\">\
<head></head>\
<body style=\"width:100%height:100%;\">\
<table border=\"1\" style=\"height:100%\">\
<caption>Summary</caption>");

        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX)->second.GetMainHtml());
        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC)->second.GetMainHtml());
        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT)->second.GetMainHtml());
        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF)->second.GetMainHtml());
        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT)->second.GetMainHtml());
        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML)->second.GetMainHtml());

        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX)->second.GetMainHtml());
        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX)->second.GetMainHtml());
        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT)->second.GetMainHtml());
        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP)->second.GetMainHtml());

        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX)->second.GetMainHtml());
        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS)->second.GetMainHtml());
        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS)->second.GetMainHtml());
        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV)->second.GetMainHtml());

        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF)->second.GetMainHtml());
        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU)->second.GetMainHtml());
        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS)->second.GetMainHtml());

        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX)->second.GetHtml());
        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC)->second.GetHtml());
        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT)->second.GetHtml());
        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF)->second.GetHtml());
        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT)->second.GetHtml());
        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML)->second.GetHtml());

        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX)->second.GetHtml());
        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX)->second.GetHtml());
        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT)->second.GetHtml());
        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP)->second.GetHtml());

        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX)->second.GetHtml());
        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS)->second.GetHtml());
        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS)->second.GetHtml());
        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV)->second.GetHtml());

        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF)->second.GetHtml());
        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU)->second.GetHtml());
        oBuilder.WriteString(m_logger.find(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS)->second.GetHtml());

        oBuilder.WriteString(L"</table></body></html>");

        std::wstring sTime = QDateTime::currentDateTime().toString().toStdWString();
        NSCommon::string_replace(sTime, L":", L".");

        std::wstring sDirectory = NSFile::GetProcessDirectory() + L"/report";
        if (!NSDirectory::Exists(sDirectory))
            NSDirectory::CreateDirectory(sDirectory);

        NSFile::CFileBinary::SaveToFile(sDirectory + L"/" + sTime + L".html", oBuilder.GetData(), true);
    }
};

class CConverter : public NSThreads::CBaseThread
{
public:
    CInternalWorker* m_pInternal;
    std::wstring m_file;
    std::wstring m_file_dst;
    int m_format;

public:
    CConverter(CInternalWorker* pWorker) : NSThreads::CBaseThread()
    {
        m_pInternal = pWorker;
    }
    virtual ~CConverter()
    {
        Stop();
    }

    virtual DWORD ThreadProc()
    {
        COfficeFileFormatChecker oChecker;
        if (!oChecker.isOfficeFile(m_file))
        {
            m_bRunThread = FALSE;
            m_pInternal->OnConvertFile(this, -1);
            return 0;
        }

        m_format = oChecker.nFileType;

        std::map<int, bool>::iterator find = m_pInternal->m_formats.find(m_format);
        if ((find == m_pInternal->m_formats.end()) || (find->second == false))
        {
            m_bRunThread = FALSE;
            m_pInternal->OnConvertFile(this, -1);
            return 0;
        }

        std::wstring sProcess = NSFile::GetProcessDirectory();

        NSCommon::string_replace(sProcess, L"\\", L"/");
        NSCommon::string_replace(m_file, L"\\", L"/");

        std::wstring sDirectoryDst = NSDirectory::CreateDirectoryWithUniqueName(sProcess + L"/Result");
        NSCommon::string_replace(sDirectoryDst, L"\\", L"/");

#ifdef WIN32
        NSCommon::string_replace(m_file, L"//", L"\\\\");
#endif

        NSDirectory::CreateDirectory(sDirectoryDst);

        NSStringUtils::CStringBuilder oBuilder;
        oBuilder.WriteString(L"<?xml version=\"1.0\" encoding=\"utf-8\"?><TaskQueueDataConvert><m_sFileFrom>");

        std::wstring sSrcCopy = sDirectoryDst + L"/" + NSCommon::GetFileName(m_file);
        NSFile::CFileBinary::Copy(m_file, sSrcCopy);

        oBuilder.WriteEncodeXmlString(sSrcCopy);
        oBuilder.WriteString(L"</m_sFileFrom><m_sFileTo>");

        std::wstring sExt = oChecker.GetExtensionByType(m_pInternal->m_dst_format);
        if (sExt.empty())
            sExt = L".bin";

        std::wstring sFileDst = sDirectoryDst + L"/result" + sExt;

        m_file_dst = sFileDst;

        oBuilder.WriteEncodeXmlString(sFileDst);
        oBuilder.WriteString(L"</m_sFileTo><m_nFormatTo>");
        oBuilder.WriteString(std::to_wstring(m_pInternal->m_dst_format));
        oBuilder.WriteString(L"</m_nFormatTo><m_sThemeDir>../</m_sThemeDir><m_bDontSaveAdditional>true</m_bDontSaveAdditional><m_sAllFontsPath>");
        oBuilder.WriteString(sProcess + L"/fonts");
        oBuilder.WriteString(L"/AllFonts.js</m_sAllFontsPath><m_nCsvTxtEncoding>46</m_nCsvTxtEncoding><m_nCsvDelimiter>4</m_nCsvDelimiter>");

        oBuilder.WriteString(L"<m_sFontDir>");
        oBuilder.WriteEncodeXmlString(sProcess + L"/fonts");
        oBuilder.WriteString(L"</m_sFontDir>");

        oBuilder.WriteString(L"</TaskQueueDataConvert>");

        std::wstring sXmlConvert = oBuilder.GetData();

        std::wstring sTempFileForParams = sDirectoryDst + L"/params.xml";
        NSFile::CFileBinary::SaveToFile(sTempFileForParams, sXmlConvert, true);

        std::wstring sConverterExe = sProcess + L"/x2t";
        int nReturnCode = 0;

#ifdef WIN32
        std::wstring sApp = L"x2t32 ";

        if (NSFile::CFileBinary::Exists(sConverterExe + L".exe"))
        {
            sApp = L"x2t ";
            sConverterExe += L".exe";
        }
        else
            sConverterExe += L"32.exe";

        STARTUPINFOW sturtupinfo;
        ZeroMemory(&sturtupinfo,sizeof(STARTUPINFO));
        sturtupinfo.cb = sizeof(STARTUPINFO);

        sApp += (L"\"" + sTempFileForParams + L"\"");
        wchar_t* pCommandLine = NULL;
        if (true)
        {
            pCommandLine = new wchar_t[sApp.length() + 1];
            memcpy(pCommandLine, sApp.c_str(), sApp.length() * sizeof(wchar_t));
            pCommandLine[sApp.length()] = (wchar_t)'\0';
        }

        HANDLE ghJob = CreateJobObject(NULL, NULL);

        if (ghJob)
        {
            JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli = { 0 };

            // Configure all child processes associated with the job to terminate when the
            jeli.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
            if ( 0 == SetInformationJobObject( ghJob, JobObjectExtendedLimitInformation, &jeli, sizeof(jeli)))
            {
                CloseHandle(ghJob);
                ghJob = NULL;
            }
        }

        PROCESS_INFORMATION processinfo;
        ZeroMemory(&processinfo,sizeof(PROCESS_INFORMATION));
        BOOL bResult = CreateProcessW(sConverterExe.c_str(), pCommandLine,
                                   NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &sturtupinfo, &processinfo);

        if (bResult && ghJob)
        {
            AssignProcessToJobObject(ghJob, processinfo.hProcess);
        }

        ::WaitForSingleObject(processinfo.hProcess, INFINITE);

        RELEASEARRAYOBJECTS(pCommandLine);

        //get exit code
        DWORD dwExitCode = 0;
        if (GetExitCodeProcess(processinfo.hProcess, &dwExitCode))
        {
            nReturnCode = (int)dwExitCode;
        }

        CloseHandle(processinfo.hProcess);
        CloseHandle(processinfo.hThread);

        if (ghJob)
        {
            CloseHandle(ghJob);
            ghJob = NULL;
        }

#endif

#ifdef LINUX
        pid_t pid = fork(); // create child process
        int status;

        std::string sProgramm = U_TO_UTF8(sConverterExe);
        std::string sXmlA = U_TO_UTF8(sTempFileForParams);

        switch (pid)
        {
        case -1: // error
            break;

        case 0: // child process
        {
            std::string sLibraryDir = sProgramm;
            std::string sPATH = sProgramm;
            if (std::string::npos != sProgramm.find_last_of('/'))
            {
                sLibraryDir = "LD_LIBRARY_PATH=" + sProgramm.substr(0, sProgramm.find_last_of('/'));
                sPATH = "PATH=" + sProgramm.substr(0, sProgramm.find_last_of('/'));
            }

#ifdef _MAC
            sLibraryDir = "DY" + sLibraryDir;
#endif

            const char* nargs[3];
            nargs[0] = sProgramm.c_str();
            nargs[1] = sXmlA.c_str();
            nargs[2] = NULL;

#ifndef _MAC
            const char* nenv[2];
            nenv[0] = sLibraryDir.c_str();
            nenv[1] = NULL;
#else
            const char* nenv[3];
            nenv[0] = sLibraryDir.c_str();
            nenv[1] = sPATH.c_str();
            nenv[2] = NULL;
#endif

            execve(sProgramm.c_str(),
                   (char * const *)nargs,
                   (char * const *)nenv);
            exit(EXIT_SUCCESS);
            break;
        }
        default: // parent process, pid now contains the child pid
            while (-1 == waitpid(pid, &status, 0)); // wait for child to complete
            if (WIFEXITED(status))
            {
                nReturnCode =  WEXITSTATUS(status);
            }
            break;
        }
#endif

        //NSFile::CFileBinary::Remove(sTempFileForParams);

        m_bRunThread = FALSE;

        m_pInternal->OnConvertFile(this, nReturnCode);
        return 0;
    }
};

CConverter* CInternalWorker::GetNextConverter()
{
    if (m_nCurrent >= m_nCount)
        return NULL;

    CConverter* pConverter = new CConverter(this);
    pConverter->m_file = m_pParser->m_files[m_nCurrent];
    ++m_nCurrent;

    pConverter->Start(0);
    return pConverter;
}

void CInternalWorker::OnConvertFile(CConverter* pConverter, int nCode)
{
    CTemporaryCS oCS(&m_oCS);

    if (-1 != nCode)
    {
        std::map<int, CFormatLogger>::iterator _logger = m_logger.find(pConverter->m_format);
        if (_logger != m_logger.end())
            _logger->second.Add(nCode, pConverter->m_file, pConverter->m_file_dst);
    }

    ++m_nCurrentConvert;

    RELEASEOBJECT(pConverter);
    GetNextConverter();

    m_pParser->m_window->send_onFileConverted(m_nCurrentConvert);
}

void CInternalWorker::Start(int nCores)
{
    CTemporaryCS oCS(&m_oCS);

    int nSizeInit = nCores;
    if (nSizeInit > m_nCount)
        nSizeInit = m_nCount;

    for (std::map<int, CFormatLogger>::iterator i = m_logger.begin(); i != m_logger.end(); i++)
    {
        i->second.clear();
        i->second.m_need = m_formats.find(i->first)->second;
    }

    for (int i = 0; i < nSizeInit; ++i)
        GetNextConverter();
}

void CInternalWorker::Cancel()
{
    CTemporaryCS oCS(&m_oCS);
    m_nCount = m_nCurrent;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("x2t");

    QObject::connect( ui->pushButtonDirectory, SIGNAL( clicked() ), this, SLOT( pushButtonDirectoryClicked() ), Qt::QueuedConnection );
    QObject::connect( ui->comboBoxOUTPUT, SIGNAL( currentIndexChanged(int) ), this, SLOT( outputIndexChanged(int) ), Qt::QueuedConnection );

    QObject::connect( ui->checkBoxDOCX, SIGNAL( toggled(bool) ), this, SLOT( stateChanged_DOCX(bool) ), Qt::QueuedConnection );
    QObject::connect( ui->checkBoxDOC, SIGNAL( toggled(bool) ), this, SLOT( stateChanged_DOC(bool) ), Qt::QueuedConnection );
    QObject::connect( ui->checkBoxODT, SIGNAL( toggled(bool) ), this, SLOT( stateChanged_ODT(bool) ), Qt::QueuedConnection );
    QObject::connect( ui->checkBoxRTF, SIGNAL( toggled(bool) ), this, SLOT( stateChanged_RTF(bool) ), Qt::QueuedConnection );
    QObject::connect( ui->checkBoxTXT, SIGNAL( toggled(bool) ), this, SLOT( stateChanged_TXT(bool) ), Qt::QueuedConnection );
    QObject::connect( ui->checkBoxHTML, SIGNAL( toggled(bool) ), this, SLOT( stateChanged_HTML(bool) ), Qt::QueuedConnection );
    QObject::connect( ui->checkBoxDOC_ALL, SIGNAL( toggled(bool) ), this, SLOT( stateChanged_DOC_ALL(bool) ), Qt::QueuedConnection );

    QObject::connect( ui->checkBoxPPTX, SIGNAL( toggled(bool) ), this, SLOT( stateChanged_PPTX(bool) ), Qt::QueuedConnection );
    QObject::connect( ui->checkBoxPPT, SIGNAL( toggled(bool) ), this, SLOT( stateChanged_PPT(bool) ), Qt::QueuedConnection );
    QObject::connect( ui->checkBoxODP, SIGNAL( toggled(bool) ), this, SLOT( stateChanged_ODP(bool) ), Qt::QueuedConnection );
    QObject::connect( ui->checkBoxPPT_ALL, SIGNAL( toggled(bool) ), this, SLOT( stateChanged_PPT_ALL(bool) ), Qt::QueuedConnection );

    QObject::connect( ui->checkBoxXLSX, SIGNAL( toggled(bool) ), this, SLOT( stateChanged_XLSX(bool) ), Qt::QueuedConnection );
    QObject::connect( ui->checkBoxXLS, SIGNAL( toggled(bool) ), this, SLOT( stateChanged_XLS(bool) ), Qt::QueuedConnection );
    QObject::connect( ui->checkBoxODS, SIGNAL( toggled(bool) ), this, SLOT( stateChanged_ODS(bool) ), Qt::QueuedConnection );
    QObject::connect( ui->checkBoxCSV, SIGNAL( toggled(bool) ), this, SLOT( stateChanged_CSV(bool) ), Qt::QueuedConnection );
    QObject::connect( ui->checkBoxXLS_ALL, SIGNAL( toggled(bool) ), this, SLOT( stateChanged_XLS_ALL(bool) ), Qt::QueuedConnection );

    QObject::connect( ui->checkBoxPDF, SIGNAL( toggled(bool) ), this, SLOT( stateChanged_PDF(bool) ), Qt::QueuedConnection );
    QObject::connect( ui->checkBoxXPS, SIGNAL( toggled(bool) ), this, SLOT( stateChanged_XPS(bool) ), Qt::QueuedConnection );
    QObject::connect( ui->checkBoxDJVU, SIGNAL( toggled(bool) ), this, SLOT( stateChanged_DJVU(bool) ), Qt::QueuedConnection );
    QObject::connect( ui->checkBoxDJVU_ALL, SIGNAL( toggled(bool) ), this, SLOT( stateChanged_DJVU_ALL(bool) ), Qt::QueuedConnection );

    QObject::connect( ui->pushButtonConvert, SIGNAL( clicked() ), this, SLOT( pushButtonConvertClicked() ), Qt::QueuedConnection );

    QObject::connect( this, SIGNAL( signal_onDirectoryChecked() ), this, SLOT( slot_onDirectoryChecked() ), Qt::QueuedConnection );
    QObject::connect( this, SIGNAL( signal_onFileConverted(int) ), this, SLOT( slot_onFileConverted(int) ), Qt::QueuedConnection );

    ui->textEditDirectory->setEnabled(false);

    m_pWorker = new CInternalWorker();

    ui->groupBoxINPUT->setTitle("Input");
    ui->groupBoxOUTPUT->setTitle("Output");

    ui->spinBoxThreads->setMinimum(1);
    int nCount = (int)std::thread::hardware_concurrency();
    if (nCount == 0)
        nCount = 1;
    ui->spinBoxThreads->setMaximum((int)(1.5 * nCount + 0.5));
    ui->spinBoxThreads->setValue(nCount);

    ui->checkBoxDOCX->setText("docx");
    ui->checkBoxDOC->setText("doc");
    ui->checkBoxODT->setText("odt");
    ui->checkBoxRTF->setText("rtf");
    ui->checkBoxTXT->setText("txt");
    ui->checkBoxHTML->setText("html");
    ui->checkBoxDOC_ALL->setText("all");

    ui->checkBoxPPTX->setText("pptx");
    ui->checkBoxPPT->setText("ppt");
    ui->checkBoxODP->setText("odp");
    ui->checkBoxPPT_ALL->setText("all");

    ui->checkBoxXLSX->setText("xlsx");
    ui->checkBoxXLS->setText("xls");
    ui->checkBoxODS->setText("ods");
    ui->checkBoxCSV->setText("csv");
    ui->checkBoxXLS_ALL->setText("all");

    ui->checkBoxPDF->setText("pdf");
    ui->checkBoxXPS->setText("xps");
    ui->checkBoxDJVU->setText("djvu");
    ui->checkBoxDJVU_ALL->setText("all");

    ui->labelThreads->setText("Cores");

    ui->comboBoxOUTPUT->addItem("internal");
    ui->comboBoxOUTPUT->addItem("docx");
    ui->comboBoxOUTPUT->addItem("doc");
    ui->comboBoxOUTPUT->addItem("odt");
    ui->comboBoxOUTPUT->addItem("rtf");
    ui->comboBoxOUTPUT->addItem("txt");

    ui->comboBoxOUTPUT->addItem("pptx");
    ui->comboBoxOUTPUT->addItem("odp");

    ui->comboBoxOUTPUT->addItem("xlsx");
    ui->comboBoxOUTPUT->addItem("xls");
    ui->comboBoxOUTPUT->addItem("ods");
    ui->comboBoxOUTPUT->addItem("csv");

    ui->comboBoxOUTPUT->addItem("pdf");

    ui->labelOutput->setText("Format");

    ui->pushButtonConvert->setText("CONVERT");

    this->setMinimumSize(500, 600);
    this->resize(500, 600);

    ui->textEdit->setEnabled(false);

    ui->textEdit->setVisible(false);
    ui->progressBar->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    int nW = event->size().width();
    int nH = event->size().height();

    ui->textEditDirectory->setGeometry(20, 20, nW - 110, 30);
    ui->pushButtonDirectory->setGeometry(nW - 70, 20, 50, 30);

    int nCheckBoxW = 50;
    int nCheckBoxH = 20;
    int nCheckBoxDeltaX = (nW - 40 - 40 - 4 * nCheckBoxW) / 3;

    int nCheckBoxX1 = 20;
    int nCheckBoxX2 = nCheckBoxX1 + nCheckBoxDeltaX + nCheckBoxW;
    int nCheckBoxX3 = nCheckBoxX2 + nCheckBoxDeltaX + nCheckBoxW;
    int nCheckBoxX4 = nCheckBoxX3 + nCheckBoxDeltaX + nCheckBoxW;
    int nCheckBoxY = 20;

    int nCheckBoxDeltaY = 10;

    ui->groupBoxINPUT->setGeometry(20, 70, nW - 40, 2 * nCheckBoxY + 7 * nCheckBoxH + 7 * nCheckBoxDeltaY);

    ui->checkBoxDOCX->setGeometry(nCheckBoxX1, nCheckBoxY, nCheckBoxW, nCheckBoxH);
    ui->checkBoxDOC->setGeometry(nCheckBoxX1, nCheckBoxY + nCheckBoxH + nCheckBoxDeltaY, nCheckBoxW, nCheckBoxH);
    ui->checkBoxODT->setGeometry(nCheckBoxX1, nCheckBoxY + 2 * (nCheckBoxH + nCheckBoxDeltaY), nCheckBoxW, nCheckBoxH);
    ui->checkBoxRTF->setGeometry(nCheckBoxX1, nCheckBoxY + 3 * (nCheckBoxH + nCheckBoxDeltaY), nCheckBoxW, nCheckBoxH);
    ui->checkBoxTXT->setGeometry(nCheckBoxX1, nCheckBoxY + 4 * (nCheckBoxH + nCheckBoxDeltaY), nCheckBoxW, nCheckBoxH);
    ui->checkBoxHTML->setGeometry(nCheckBoxX1, nCheckBoxY + 5 * (nCheckBoxH + nCheckBoxDeltaY), nCheckBoxW, nCheckBoxH);
    ui->checkBoxDOC_ALL->setGeometry(nCheckBoxX1, nCheckBoxY + 6 * (nCheckBoxH + nCheckBoxDeltaY) + nCheckBoxDeltaY, nCheckBoxW, nCheckBoxH);

    ui->checkBoxPPTX->setGeometry(nCheckBoxX2, nCheckBoxY, nCheckBoxW, nCheckBoxH);
    ui->checkBoxPPT->setGeometry(nCheckBoxX2, nCheckBoxY + nCheckBoxH + nCheckBoxDeltaY, nCheckBoxW, nCheckBoxH);
    ui->checkBoxODP->setGeometry(nCheckBoxX2, nCheckBoxY + 2 * (nCheckBoxH + nCheckBoxDeltaY), nCheckBoxW, nCheckBoxH);
    ui->checkBoxPPT_ALL->setGeometry(nCheckBoxX2, nCheckBoxY + 6 * (nCheckBoxH + nCheckBoxDeltaY) + nCheckBoxDeltaY, nCheckBoxW, nCheckBoxH);

    ui->checkBoxXLSX->setGeometry(nCheckBoxX3, nCheckBoxY, nCheckBoxW, nCheckBoxH);
    ui->checkBoxXLS->setGeometry(nCheckBoxX3, nCheckBoxY + nCheckBoxH + nCheckBoxDeltaY, nCheckBoxW, nCheckBoxH);
    ui->checkBoxODS->setGeometry(nCheckBoxX3, nCheckBoxY + 2 * (nCheckBoxH + nCheckBoxDeltaY), nCheckBoxW, nCheckBoxH);
    ui->checkBoxCSV->setGeometry(nCheckBoxX3, nCheckBoxY + 3 * (nCheckBoxH + nCheckBoxDeltaY), nCheckBoxW, nCheckBoxH);
    ui->checkBoxXLS_ALL->setGeometry(nCheckBoxX3, nCheckBoxY + 6 * (nCheckBoxH + nCheckBoxDeltaY) + nCheckBoxDeltaY, nCheckBoxW, nCheckBoxH);

    ui->checkBoxPDF->setGeometry(nCheckBoxX4, nCheckBoxY, nCheckBoxW, nCheckBoxH);
    ui->checkBoxXPS->setGeometry(nCheckBoxX4, nCheckBoxY + nCheckBoxH + nCheckBoxDeltaY, nCheckBoxW, nCheckBoxH);
    ui->checkBoxDJVU->setGeometry(nCheckBoxX4, nCheckBoxY + 2 * (nCheckBoxH + nCheckBoxDeltaY), nCheckBoxW, nCheckBoxH);
    ui->checkBoxDJVU_ALL->setGeometry(nCheckBoxX4, nCheckBoxY + 6 * (nCheckBoxH + nCheckBoxDeltaY) + nCheckBoxDeltaY, nCheckBoxW, nCheckBoxH);

    ui->groupBoxOUTPUT->setGeometry(20, 60 + ui->textEditDirectory->height() + ui->groupBoxINPUT->height(), nW - 40, 60);

    ui->labelOutput->setGeometry(20, 20, 50, 20);
    ui->comboBoxOUTPUT->setGeometry(100, 20, 100, 20);

    ui->labelThreads->setGeometry(250, 20, 50, 20);
    ui->spinBoxThreads->setGeometry(320, 20, 30, 20);

    int nOffset = 20 + ui->pushButtonDirectory->height() + 20 + ui->groupBoxINPUT->height() + 20 + ui->groupBoxOUTPUT->height() + 20;

    ui->pushButtonConvert->setGeometry((nW - 80) >> 1, nOffset, 80, 30);

    ui->progressBar->setGeometry(20, nOffset + 50, nW - 40, 30);

    nOffset = nOffset + 100;
    ui->textEdit->setGeometry(20, nOffset, nW - 40, nH - 20 - nOffset);
}

void MainWindow::pushButtonDirectoryClicked()
{
    ui->textEditDirectory->setText(QFileDialog::getExistingDirectory(this, tr("Open Directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks));
}

void MainWindow::pushButtonConvertClicked()
{
    if (ui->pushButtonConvert->text() == "CANCEL")
    {
        m_pWorker->Cancel();
        return;
    }

    std::wstring sProcess = NSFile::GetProcessDirectory();
    if (!NSDirectory::Exists(sProcess + L"/fonts"))
        NSDirectory::CreateDirectory(sProcess + L"/fonts");
    if (!NSDirectory::Exists(sProcess + L"/Result"))
        NSDirectory::CreateDirectory(sProcess + L"/Result");

    m_pWorker->m_pParser->m_sDirectory = ui->textEditDirectory->toPlainText().toStdWString();
    m_pWorker->m_pParser->m_window = this;

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(0);
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(true);

    m_pWorker->m_pParser->Start(0);

    ui->pushButtonConvert->setText("CANCEL");
}

void MainWindow::send_onDirectoryChecked()
{
    emit signal_onDirectoryChecked();
}

void MainWindow::send_onFileConverted(int nProgress)
{
    emit signal_onFileConverted(nProgress);
}

void MainWindow::slot_onFileConverted(int nProgress)
{
    ui->progressBar->setValue(nProgress);

    if (nProgress == m_pWorker->m_nCount)
    {
        m_pWorker->Log();
        ui->pushButtonConvert->setText("CONVERT");
        ui->progressBar->setVisible(false);
    }
}

void MainWindow::slot_onDirectoryChecked()
{
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum((int)m_pWorker->m_pParser->m_files.size());

    ui->progressBar->setValue(0);

    if (0 == (int)m_pWorker->m_pParser->m_files.size())
    {
        ui->pushButtonConvert->setEnabled(true);
        ui->progressBar->setVisible(false);
    }
    else
    {
        std::wstring sValue = ui->comboBoxOUTPUT->currentText().toStdWString();

        int nFormat = 0;
        if (sValue == L"docx")
            nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX;
        else if (sValue == L"doc")
            nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC;
        else if (sValue == L"odt")
            nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT;
        else if (sValue == L"rtf")
            nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF;
        else if (sValue == L"txt")
            nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT;
        else if (sValue == L"pptx")
            nFormat = AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX;
        else if (sValue == L"odp")
            nFormat = AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP;
        else if (sValue == L"xlsx")
            nFormat = AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX;
        else if (sValue == L"xls")
            nFormat = AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS;
        else if (sValue == L"ods")
            nFormat = AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS;
        else if (sValue == L"csv")
            nFormat = AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV;
        else if (sValue == L"pdf")
            nFormat = AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF;
        else
            nFormat = 8192;

        m_pWorker->m_dst_format = nFormat;

        m_pWorker->m_nCurrent = 0;
        m_pWorker->m_nCount = (int)m_pWorker->m_pParser->m_files.size();
        m_pWorker->m_nCurrentConvert = 0;

        m_pWorker->Start(ui->spinBoxThreads->value());
    }
}

void MainWindow::outputIndexChanged(int value)
{
    value;

    QString sValue = ui->comboBoxOUTPUT->currentText();

    ui->checkBoxDOCX->setEnabled(true);
    ui->checkBoxDOC->setEnabled(true);
    ui->checkBoxODT->setEnabled(true);
    ui->checkBoxRTF->setEnabled(true);
    ui->checkBoxTXT->setEnabled(true);
    ui->checkBoxHTML->setEnabled(true);
    ui->checkBoxDOC_ALL->setEnabled(true);

    ui->checkBoxPPTX->setEnabled(true);
    ui->checkBoxPPT->setEnabled(true);
    ui->checkBoxODP->setEnabled(true);
    ui->checkBoxPPT_ALL->setEnabled(true);

    ui->checkBoxXLSX->setEnabled(true);
    ui->checkBoxXLS->setEnabled(true);
    ui->checkBoxODS->setEnabled(true);
    ui->checkBoxCSV->setEnabled(true);
    ui->checkBoxXLS_ALL->setEnabled(true);

    ui->checkBoxPDF->setEnabled(true);
    ui->checkBoxXPS->setEnabled(true);
    ui->checkBoxDJVU->setEnabled(true);
    ui->checkBoxDJVU_ALL->setEnabled(true);

    if (sValue == "internal")
    {
        m_pWorker->setDestinationInternal();
    }
    else if (sValue == "docx" ||
             sValue == "doc" ||
             sValue == "odt" ||
             sValue == "rtf" ||
             sValue == "txt")
    {
        m_pWorker->setDestinationDocument();

        ui->checkBoxPPTX->setEnabled(false);
        ui->checkBoxPPT->setEnabled(false);
        ui->checkBoxODP->setEnabled(false);
        ui->checkBoxPPT_ALL->setEnabled(false);

        ui->checkBoxXLSX->setEnabled(false);
        ui->checkBoxXLS->setEnabled(false);
        ui->checkBoxODS->setEnabled(false);
        ui->checkBoxCSV->setEnabled(false);
        ui->checkBoxXLS_ALL->setEnabled(false);

        ui->checkBoxPDF->setEnabled(false);
        ui->checkBoxXPS->setEnabled(false);
        ui->checkBoxDJVU->setEnabled(false);
        ui->checkBoxDJVU_ALL->setEnabled(false);
    }
    else if (sValue == "pptx" ||
             sValue == "odp")
    {
        m_pWorker->setDestinationPresentation();

        ui->checkBoxDOCX->setEnabled(false);
        ui->checkBoxDOC->setEnabled(false);
        ui->checkBoxODT->setEnabled(false);
        ui->checkBoxRTF->setEnabled(false);
        ui->checkBoxTXT->setEnabled(false);
        ui->checkBoxHTML->setEnabled(false);
        ui->checkBoxDOC_ALL->setEnabled(false);

        ui->checkBoxXLSX->setEnabled(false);
        ui->checkBoxXLS->setEnabled(false);
        ui->checkBoxODS->setEnabled(false);
        ui->checkBoxCSV->setEnabled(false);
        ui->checkBoxXLS_ALL->setEnabled(false);

        ui->checkBoxPDF->setEnabled(false);
        ui->checkBoxXPS->setEnabled(false);
        ui->checkBoxDJVU->setEnabled(false);
        ui->checkBoxDJVU_ALL->setEnabled(false);
    }
    else if (sValue == "xlsx" ||
             sValue == "xls" ||
             sValue == "ods" ||
             sValue == "csv")
    {
        m_pWorker->setDestinationSpreadsheet();

        ui->checkBoxDOCX->setEnabled(false);
        ui->checkBoxDOC->setEnabled(false);
        ui->checkBoxODT->setEnabled(false);
        ui->checkBoxRTF->setEnabled(false);
        ui->checkBoxTXT->setEnabled(false);
        ui->checkBoxHTML->setEnabled(false);
        ui->checkBoxDOC_ALL->setEnabled(false);

        ui->checkBoxPPTX->setEnabled(false);
        ui->checkBoxPPT->setEnabled(false);
        ui->checkBoxODP->setEnabled(false);
        ui->checkBoxPPT_ALL->setEnabled(false);

        ui->checkBoxPDF->setEnabled(false);
        ui->checkBoxXPS->setEnabled(false);
        ui->checkBoxDJVU->setEnabled(false);
        ui->checkBoxDJVU_ALL->setEnabled(false);
    }
    else
    {
        m_pWorker->setDestinationPDF();

        ui->checkBoxPDF->setEnabled(false);
        ui->checkBoxXPS->setEnabled(false);
        ui->checkBoxDJVU->setEnabled(false);
        ui->checkBoxDJVU_ALL->setEnabled(false);
    }

    CorrectCheckBoxes();
}

void MainWindow::CorrectCheckBoxes()
{
    ui->checkBoxDOCX->setChecked(ui->checkBoxDOCX->isEnabled());
    ui->checkBoxDOC->setChecked(ui->checkBoxDOC->isEnabled());
    ui->checkBoxODT->setChecked(ui->checkBoxODT->isEnabled());
    ui->checkBoxRTF->setChecked(ui->checkBoxRTF->isEnabled());
    ui->checkBoxTXT->setChecked(ui->checkBoxTXT->isEnabled());
    ui->checkBoxHTML->setChecked(ui->checkBoxHTML->isEnabled());
    ui->checkBoxDOC_ALL->setChecked(ui->checkBoxDOC_ALL->isEnabled());

    ui->checkBoxPPTX->setChecked(ui->checkBoxPPTX->isEnabled());
    ui->checkBoxPPT->setChecked(ui->checkBoxPPT->isEnabled());
    ui->checkBoxODP->setChecked(ui->checkBoxODP->isEnabled());
    ui->checkBoxPPT_ALL->setChecked(ui->checkBoxPPT_ALL->isEnabled());

    ui->checkBoxXLSX->setChecked(ui->checkBoxXLSX->isEnabled());
    ui->checkBoxXLS->setChecked(ui->checkBoxXLS->isEnabled());
    ui->checkBoxODS->setChecked(ui->checkBoxODS->isEnabled());
    ui->checkBoxCSV->setChecked(ui->checkBoxCSV->isEnabled());
    ui->checkBoxXLS_ALL->setChecked(ui->checkBoxXLS_ALL->isEnabled());

    ui->checkBoxPDF->setChecked(ui->checkBoxPDF->isEnabled());
    ui->checkBoxXPS->setChecked(ui->checkBoxXPS->isEnabled());
    ui->checkBoxDJVU->setChecked(ui->checkBoxDJVU->isEnabled());
    ui->checkBoxDJVU_ALL->setChecked(ui->checkBoxDJVU_ALL->isEnabled());
}

void MainWindow::stateChanged_DOCX(bool)
{
    if (ui->checkBoxDOCX->isEnabled())
        m_pWorker->m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX)->second = ui->checkBoxDOCX->isChecked();
}

void MainWindow::stateChanged_DOC(bool)
{
    if (ui->checkBoxDOC->isEnabled())
        m_pWorker->m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC)->second = ui->checkBoxDOC->isChecked();
}

void MainWindow::stateChanged_ODT(bool)
{
    if (ui->checkBoxODT->isEnabled())
        m_pWorker->m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT)->second = ui->checkBoxODT->isChecked();
}

void MainWindow::stateChanged_RTF(bool)
{
    if (ui->checkBoxRTF->isEnabled())
        m_pWorker->m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF)->second = ui->checkBoxRTF->isChecked();
}

void MainWindow::stateChanged_TXT(bool)
{
    if (ui->checkBoxTXT->isEnabled())
        m_pWorker->m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT)->second = ui->checkBoxTXT->isChecked();
}

void MainWindow::stateChanged_HTML(bool)
{
    if (ui->checkBoxHTML->isEnabled())
        m_pWorker->m_formats.find(AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML)->second = ui->checkBoxHTML->isChecked();
}

void MainWindow::stateChanged_DOC_ALL(bool)
{
    bool bIsVal = ui->checkBoxDOC_ALL->isChecked();
    ui->checkBoxDOCX->setChecked(bIsVal);
    ui->checkBoxDOC->setChecked(bIsVal);
    ui->checkBoxODT->setChecked(bIsVal);
    ui->checkBoxRTF->setChecked(bIsVal);
    ui->checkBoxTXT->setChecked(bIsVal);
    ui->checkBoxHTML->setChecked(bIsVal);
}

void MainWindow::stateChanged_PPTX(bool)
{
    if (ui->checkBoxPPTX->isEnabled())
        m_pWorker->m_formats.find(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX)->second = ui->checkBoxPPTX->isChecked();
}

void MainWindow::stateChanged_PPT(bool)
{
    if (ui->checkBoxPPT->isEnabled())
        m_pWorker->m_formats.find(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT)->second = ui->checkBoxPPT->isChecked();
}

void MainWindow::stateChanged_ODP(bool)
{
    if (ui->checkBoxODP->isEnabled())
        m_pWorker->m_formats.find(AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP)->second = ui->checkBoxODP->isChecked();
}

void MainWindow::stateChanged_PPT_ALL(bool)
{
    bool bIsVal = ui->checkBoxPPT_ALL->isChecked();
    ui->checkBoxPPTX->setChecked(bIsVal);
    ui->checkBoxPPT->setChecked(bIsVal);
    ui->checkBoxODP->setChecked(bIsVal);
}

void MainWindow::stateChanged_XLSX(bool)
{
    if (ui->checkBoxXLSX->isEnabled())
        m_pWorker->m_formats.find(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX)->second = ui->checkBoxXLSX->isChecked();
}

void MainWindow::stateChanged_XLS(bool)
{
    if (ui->checkBoxXLS->isEnabled())
        m_pWorker->m_formats.find(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS)->second = ui->checkBoxXLS->isChecked();
}

void MainWindow::stateChanged_ODS(bool)
{
    if (ui->checkBoxODS->isEnabled())
        m_pWorker->m_formats.find(AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS)->second = ui->checkBoxODS->isChecked();
}

void MainWindow::stateChanged_CSV(bool)
{
    if (ui->checkBoxCSV->isEnabled())
        m_pWorker->m_formats.find(AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV)->second = ui->checkBoxCSV->isChecked();
}

void MainWindow::stateChanged_XLS_ALL(bool)
{
    bool bIsVal = ui->checkBoxXLS_ALL->isChecked();
    ui->checkBoxXLSX->setChecked(bIsVal);
    ui->checkBoxXLS->setChecked(bIsVal);
    ui->checkBoxODS->setChecked(bIsVal);
    ui->checkBoxCSV->setChecked(bIsVal);
}

void MainWindow::stateChanged_PDF(bool)
{
    if (ui->checkBoxPDF->isEnabled())
        m_pWorker->m_formats.find(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF)->second = ui->checkBoxPDF->isChecked();
}

void MainWindow::stateChanged_XPS(bool)
{
    if (ui->checkBoxXPS->isEnabled())
        m_pWorker->m_formats.find(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS)->second = ui->checkBoxXPS->isChecked();
}

void MainWindow::stateChanged_DJVU(bool)
{
    if (ui->checkBoxDJVU->isEnabled())
        m_pWorker->m_formats.find(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU)->second = ui->checkBoxDJVU->isChecked();
}

void MainWindow::stateChanged_DJVU_ALL(bool)
{
    bool bIsVal = ui->checkBoxDJVU_ALL->isChecked();
    ui->checkBoxPDF->setChecked(bIsVal);
    ui->checkBoxXPS->setChecked(bIsVal);
    ui->checkBoxDJVU->setChecked(bIsVal);
}
