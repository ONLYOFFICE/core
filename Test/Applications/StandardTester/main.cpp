#include <iostream>

#include "../../../X2tConverter/src/run.h"

#include "../../../Common/OfficeFileFormats.h"
#include "../../../Common/OfficeFileFormatChecker.h"

#include "../../../DesktopEditor/graphics/Timer.h"
#include "../../../DesktopEditor/graphics/TemporaryCS.h"

#include "../../../DesktopEditor/common/File.h"
#include "../../../DesktopEditor/common/Directory.h"
#include "../../../DesktopEditor/raster/BgraFrame.h"
#include "../../../DesktopEditor/fontengine/ApplicationFontsWorker.h"

#include "../../../OfficeUtils/src/OfficeUtils.h"

#include <map>

enum CheckResultCode
{
	crcEqual     = 0,
	crcPageCount = 1,
	crcPageSize  = 2,
	crcPageDiffs = 4
};

bool g_save_x2t_xml = false;

int GetPagesCount(const std::wstring& dir)
{
	int nCount = 0;
	std::vector<std::wstring> files = NSDirectory::GetFiles(dir, false);
	for (std::vector<std::wstring>::iterator i = files.begin(); i != files.end(); i++)
	{
		std::wstring sExt = NSFile::GetFileExtention(*i);
		if (sExt == L"png")
			++nCount;
	}
	return nCount;
}

class CConverter;
class CInternalWorker
{
public:
	class CFileLog
	{
	public:
		std::wstring File;
		int Error;
		int Time;
		int CheckCode;

		CFileLog(const std::wstring& _file = L"", const int& _error = 0, const int& _time = 0, const int& _check_code = 0)
		{
			File = _file;
			Error = _error;
			Time = _time;
			CheckCode = _check_code;
		}
	};

public:
	std::map<int, bool>         m_formats;
	std::vector<std::wstring>   m_files;

	int m_nCount;
	int m_nCurrent;
	int m_nCurrentComplete;

	std::wstring m_sInputFolder;
	std::wstring m_sOutputFolder;
	std::wstring m_sDirToCheck;

	std::wstring m_sConvertParams;

	bool m_bIsStandard;
	bool m_bIsDiffAllInOne;
	bool m_bDiffOnly{false};
	bool m_bTroughDocx{false};
	bool m_bTroughDocxWas{false};

	NSCriticalSection::CRITICAL_SECTION m_oCS;
	NSCriticalSection::CRITICAL_SECTION m_oCountCS;
	NSCriticalSection::CRITICAL_SECTION m_oCS_OfficeUtils;

	std::vector<CFileLog> m_arFileLogs;
	std::vector<CFileLog> m_arFileLogsError;
	size_t m_nProgressPrevLen;

public:
	CInternalWorker()
	{
		m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX, true));
		m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC, true));
		m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT, true));
		m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF, true));
		m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT, true));
		m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCXF, true));
		m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM, true));

		m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX, true));
		m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX, true));
		m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT, true));
		m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP, true));

		m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX, true));
		m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS, true));
		m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS, true));
		m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV, true));

		m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF, true));
		m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS, true));
		m_formats.insert(std::make_pair<int, bool>(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU, true));

		m_nCount = 0;
		m_nCurrent = 0;
		m_nCurrentComplete = 0;

		m_bIsDiffAllInOne = true;

		m_oCS.InitializeCriticalSection();
		m_oCountCS.InitializeCriticalSection();
		m_oCS_OfficeUtils.InitializeCriticalSection();

		m_nProgressPrevLen = 0;
	}
	~CInternalWorker()
	{
		m_oCS.DeleteCriticalSection();
		m_oCountCS.DeleteCriticalSection();
		m_oCS_OfficeUtils.DeleteCriticalSection();
	}

	CConverter* GetNextConverter();
	void OnConvertFile(CConverter* pConverter, int nCode, int nTime, int nCheckCode);
	void Start(int nCores);
	void Cancel();

	void OpenDir(std::wstring sDir)
	{
		m_sInputFolder = sDir;
		m_sDirToCheck = m_sInputFolder;
		if (m_bDiffOnly)
		{
			m_files = NSDirectory::GetDirectories(m_sInputFolder);
			m_nCount = (int)m_files.size();
			return;
		}

		std::vector<std::wstring> arFiles = NSDirectory::GetFiles(sDir, true);
		for (std::vector<std::wstring>::iterator iter = arFiles.begin(); iter != arFiles.end(); iter++)
		{
			std::wstring sExt = NSFile::GetFileExtention(*iter);

			if (sExt == L"docx" || sExt == L"doc" || sExt == L"odt" || sExt == L"rtf" || sExt == L"docxf" || sExt == L"oform" ||
				sExt == L"pptx" || sExt == L"ppt" || sExt == L"odp" ||
				sExt == L"xlsx" || sExt == L"xls" || sExt == L"ods" ||
				sExt == L"pdf" || sExt == L"xps" || sExt == L"djvu")
			{
				m_files.push_back(*iter);
			}
		}
		m_nCount = (int)m_files.size();
	}

	bool IsWork()
	{
		CTemporaryCS oCS(&m_oCS);
		return (m_nCurrentComplete < m_nCount);
	}

	void OnReportFile(const std::wstring& sFile, int nReturnCode, int nTime, int nCheckReport)
	{
		if (nReturnCode == 0)
			m_arFileLogs.push_back(CFileLog(sFile, nReturnCode, nTime, nCheckReport));
		else
			m_arFileLogsError.push_back(CFileLog(sFile, nReturnCode, nCheckReport));

		 if (m_nProgressPrevLen > 0)
			 std::cout << "\r";

		std::wstring sFileLog = GetFileNameForLog(sFile);

		std::string sFileLogUtf8 = U_TO_UTF8(sFileLog);
		size_t nDiff = sFileLogUtf8.length() - sFileLog.length();

		std::string message = "[" + std::to_string(m_nCurrentComplete + 1) + "/" + std::to_string(m_nCount) +
				" (errors: " + std::to_string(m_arFileLogsError.size()) + ")] " + sFileLogUtf8;

		size_t nOldLen = m_nProgressPrevLen + nDiff;
		size_t nNewLen = message.length();
		m_nProgressPrevLen = nNewLen;

		while (nNewLen < nOldLen)
		{
			message += " ";
			++nNewLen;
		}

		 std::cout << message;
	}

	std::wstring GetFileNameForLog(const std::wstring& sFile)
	{
		std::wstring::size_type pos = sFile.rfind(L"/documents/");

		if (std::wstring::npos != pos)
		{
			return sFile.substr(pos + 11);
		}

		pos = sFile.rfind(L"/master/");
		if (std::wstring::npos != pos)
		{
			std::wstring sCheckPath = sFile.substr(pos + 8);

			std::wstring sTempDir = NSFile::GetDirectoryName(sCheckPath);
			std::wstring sTempFile = NSFile::GetFileName(sCheckPath);
			if (sTempDir == sTempFile)
				return sTempFile;

			return sCheckPath;
		}

		return sFile;
	}

	void WriteReport(const std::wstring& sLogFile, int nTime)
	{
		if (NSFile::CFileBinary::Exists(sLogFile))
			NSFile::CFileBinary::Remove(sLogFile);

		std::wstring sLogContent;

		sLogContent += L"GOOD:\n";
		for (std::vector<CFileLog>::iterator iter = m_arFileLogs.begin(); iter != m_arFileLogs.end(); iter++)
		{
			std::wstring sFileForLog = GetFileNameForLog(iter->File);
			size_t nFileNameSize = sFileForLog.length();

			sLogContent += sFileForLog;
			while (nFileNameSize < 100)
			{
				sLogContent += L" ";
				++nFileNameSize;
			}

			if (iter->CheckCode & ((int)crcPageCount))
			{
				sLogContent += L"PAGES_COUNT";
			}
			else if (iter->CheckCode & ((int)crcPageSize))
			{
				sLogContent += L"PAGES_SIZE ";
			}
			else if (iter->CheckCode & ((int)crcPageDiffs))
			{
				sLogContent += L"DIFFS      ";
			}
			else
			{
				sLogContent += L"           ";
			}

			sLogContent += L"          ";

			sLogContent += std::to_wstring(iter->Time);
			sLogContent += L"\n";
		}

		sLogContent += L"\n\n";
		sLogContent += L"BAD:\n";
		for (std::vector<CFileLog>::iterator iter = m_arFileLogsError.begin(); iter != m_arFileLogsError.end(); iter++)
		{
			std::wstring sFileForLog = GetFileNameForLog(iter->File);
			size_t nFileNameSize = sFileForLog.length();

			sLogContent += sFileForLog;
			while (nFileNameSize < 100)
			{
				sLogContent += L" ";
				++nFileNameSize;
			}

			sLogContent += std::to_wstring(iter->Error);
			sLogContent += L"\n";
		}

		sLogContent += L"\n\n";

		sLogContent += L"TIME: ";
		sLogContent += std::to_wstring(nTime);
		sLogContent += L"\n";

		NSFile::CFileBinary::SaveToFile(sLogFile, sLogContent, true);
	}


	int GenerateDiff(const std::wstring& strDirIn, const std::wstring& strDirOut, const std::wstring& strDiffs)
	{
		auto dir_in = CorrectPathW(strDirIn);
		auto dir_out = CorrectPathW(strDirOut);
		auto dir_diffs = CorrectPathW(strDiffs);

		int nCountInPages = GetPagesCount(dir_in);
		int nCountOutPages = GetPagesCount(dir_out);
		int checkCode = crcEqual;


		if (nCountInPages != nCountOutPages)
		{
			if (nCountInPages > nCountOutPages)
				nCountInPages = nCountOutPages;

			if (!NSDirectory::Exists(dir_diffs))
				NSDirectory::CreateDirectories(CorrectPathW(dir_diffs));

			std::wstring sFilePagesDiff = CorrectPathW(dir_diffs + L"/pages_count");
			NSFile::CFileBinary oFile;
			oFile.CreateFileW(sFilePagesDiff);
			oFile.CloseFile();

			checkCode |= crcPageCount;
		}

		for (int nPage = 0; nPage < nCountInPages; ++nPage)
		{
			std::wstring sPageI = dir_in + FILE_SEPARATOR_STR + L"image" + std::to_wstring(nPage + 1) + L".png";
			std::wstring sPageO = dir_out + FILE_SEPARATOR_STR + L"image" + std::to_wstring(nPage + 1) + L".png";
			std::wstring sPageDiff = dir_diffs + FILE_SEPARATOR_STR + L"image" + std::to_wstring(nPage + 1) + L".png";

			CBgraFrame frameI;
			frameI.OpenFile(sPageI);

			CBgraFrame frameO;
			frameO.OpenFile(sPageO);

			int nW_I = frameI.get_Width();
			int nH_I = frameI.get_Height();

			int nW_O = frameO.get_Width();
			int nH_O = frameO.get_Height();

			if (nW_I != nW_O || nH_I != nH_O)
			{
				if (!NSDirectory::Exists(dir_diffs))
					NSDirectory::CreateDirectories(CorrectPathW(dir_diffs));

				std::wstring sFilePagesDiff = sPageDiff;
				NSFile::CFileBinary oFile;
				oFile.CreateFileW(sPageDiff);
				oFile.WriteStringUTF8(L"sizes!");
				oFile.CloseFile();

				checkCode |= crcPageSize;
				continue;
			}

			BYTE* pDataI = frameI.get_Data();
			BYTE* pDataO = frameO.get_Data();
			size_t sizeMemory = 4 * nW_I * nH_I;

			if (0 == memcmp(pDataI, pDataO, sizeMemory))
				continue;

			sizeMemory = nW_I * nH_I;

			int nEpsilonEps = 3;
			int nEpsilonNatural = 5;

			int nDivExist = 0;
			for (int indexPixH = 0; indexPixH < nH_I; indexPixH++)
			{
				for (int indexPixW = 0; indexPixW < nW_I; indexPixW++)
				{
					if (pDataI[0] != pDataO[0] || pDataI[1] != pDataO[1] || pDataI[2] != pDataO[2])
					{
						// test epsilon natural
						if ((abs(pDataI[0] - pDataO[0]) < nEpsilonNatural) &&
								(abs(pDataI[1] - pDataO[1]) < nEpsilonNatural) &&
								(abs(pDataI[2] - pDataO[2]) < nEpsilonNatural))
						{
							pDataI += 4;
							pDataO += 4;
							continue;
						}

						// test epsilon left, right, top, bottom
						int nEpsUp = nEpsilonEps;
						if (indexPixH > 0)
						{
							BYTE* pByteI = frameI.get_Data() + 4 * (indexPixH - 1) * nW_I + 4 * indexPixW;

							if ((abs(pByteI[0] - pDataO[0]) < nEpsilonEps) &&
									(abs(pByteI[1] - pDataO[1]) < nEpsilonEps) &&
									(abs(pByteI[2] - pDataO[2]) < nEpsilonEps))
							{
								nEpsUp = nEpsilonEps - 1;
							}
						}

						int nEpsDown = nEpsilonEps;
						if (indexPixH < (nH_I - 1))
						{
							BYTE* pByteI = frameI.get_Data() + 4 * (indexPixH + 1) * nW_I + 4 * indexPixW;

							if ((abs(pByteI[0] - pDataO[0]) < nEpsilonEps) &&
									(abs(pByteI[1] - pDataO[1]) < nEpsilonEps) &&
									(abs(pByteI[2] - pDataO[2]) < nEpsilonEps))
							{
								nEpsDown = nEpsilonEps - 1;
							}
						}

						int nEpsLeft = nEpsilonEps;
						if (indexPixW > 0)
						{
							BYTE* pByteI = pDataI - 4;

							if ((abs(pByteI[0] - pDataO[0]) < nEpsilonEps) &&
									(abs(pByteI[1] - pDataO[1]) < nEpsilonEps) &&
									(abs(pByteI[2] - pDataO[2]) < nEpsilonEps))
							{
								nEpsLeft = nEpsilonEps - 1;
							}
						}

						int nEpsRight = nEpsilonEps;
						if (indexPixW < (nW_I - 1))
						{
							BYTE* pByteI = pDataI + 4;

							if ((abs(pByteI[0] - pDataO[0]) < nEpsilonEps) &&
									(abs(pByteI[1] - pDataO[1]) < nEpsilonEps) &&
									(abs(pByteI[2] - pDataO[2]) < nEpsilonEps))
							{
								nEpsRight = nEpsilonEps - 1;
							}
						}

						if ((nEpsLeft < nEpsilonEps) ||
								(nEpsRight < nEpsilonEps) ||
								(nEpsUp < nEpsilonEps) ||
								(nEpsDown < nEpsilonEps))
						{
							pDataI += 4;
							pDataO += 4;
							continue;
						}

						++nDivExist;

						if (pDataO[0] == 0x00 && pDataO[1] == 0x00 && pDataO[2] == 0xFF)
						{
							pDataO[0] = 0xFF;
							pDataO[1] = 0x00;
							pDataO[2] = 0x00;
						}
						else
						{
							pDataO[0] = 0x00;
							pDataO[1] = 0x00;
							pDataO[2] = 0xFF;
						}
					}
					pDataI += 4;
					pDataO += 4;
				}
			}

			if (nDivExist > 7)
			{
				if (!NSDirectory::Exists(dir_diffs))
					NSDirectory::CreateDirectories(CorrectPathW(dir_diffs));

				if (!m_bIsDiffAllInOne)
				{
					frameO.SaveFile(sPageDiff, 4);
				}
				else
				{
					CBgraFrame frameOSrc;
					frameOSrc.OpenFile(sPageO);

					BYTE* pData1 = frameI.get_Data();
					BYTE* pData2 = frameOSrc.get_Data();
					BYTE* pData3 = frameO.get_Data();

					int nRowW = 4 * nW_I;
					BYTE* pDataAll = new BYTE[3 * nRowW * nH_I];
					BYTE* pDataAllSrc = pDataAll;
					for (int j = 0; j < nH_I; j++)
					{
						memcpy(pDataAll, pData1, nRowW);
						pDataAll += nRowW;
						pData1 += nRowW;

						memcpy(pDataAll, pData2, nRowW);
						pDataAll += nRowW;
						pData2 += nRowW;

						memcpy(pDataAll, pData3, nRowW);
						pDataAll += nRowW;
						pData3 += nRowW;
					}

					CBgraFrame oFrameAll;
					oFrameAll.put_Data(pDataAllSrc);
					oFrameAll.put_Width(3 * nW_I);
					oFrameAll.put_Height(nH_I);
					oFrameAll.put_Stride(-3 * nRowW);
					oFrameAll.SaveFile(sPageDiff, 4);
				}

				checkCode |= crcPageDiffs;
			}
		}
		return checkCode;
	}
};

class CConverter : public NSThreads::CBaseThread
{
public:
	CInternalWorker* m_pInternal;
	std::wstring m_file;
	std::wstring m_folder_dst;
	std::wstring m_dir_to_check;
	int m_format;
	bool m_bDiffOnly{false};
	bool m_docx{false};

private:
	std::wstring GetXml(const std::wstring& directory,
		int format,
		const std::wstring& process,
		const std::wstring& additional)
	{
		NSStringUtils::CStringBuilder oBuilder;
		oBuilder.WriteString(L"<?xml version=\"1.0\" encoding=\"utf-8\"?><TaskQueueDataConvert><m_sFileFrom>");

		oBuilder.WriteEncodeXmlString(m_file);
		oBuilder.WriteString(L"</m_sFileFrom><m_sFileTo>");

		oBuilder.WriteEncodeXmlString(directory);
		if(format & AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX)
		{
			auto filename_ext = NSFile::GetFileName(m_file);
			auto ext = NSFile::GetFileExtention(m_file);
			auto filename = filename_ext.substr(0, filename_ext.size() - ext.size() - 1);
			oBuilder.WriteEncodeXmlString(FILE_SEPARATOR_STR + filename + L".docx");
		}
		oBuilder.WriteString(L"</m_sFileTo><m_nFormatTo>");
		oBuilder.WriteString(std::to_wstring(format));
		oBuilder.WriteString(L"</m_nFormatTo><m_sThemeDir>./</m_sThemeDir><m_bDontSaveAdditional>true</m_bDontSaveAdditional><m_sAllFontsPath>");
		oBuilder.WriteString(process + L"/fonts");
		oBuilder.WriteString(L"/AllFonts.js</m_sAllFontsPath><m_nCsvTxtEncoding>46</m_nCsvTxtEncoding><m_nCsvDelimiter>4</m_nCsvDelimiter>");

		oBuilder.WriteString(L"<m_sFontDir>");
		oBuilder.WriteEncodeXmlString(process + L"/fonts");
		oBuilder.WriteString(L"</m_sFontDir>");

//		oBuilder.WriteString(L"<m_oTextParams><m_nTextAssociationType>3</m_nTextAssociationType></m_oTextParams>");
		oBuilder.WriteString(L"<m_oThumbnail><format>4</format><aspect>2</aspect><first>false</first><zip>false</zip><width>1000</width><height>1000</height></m_oThumbnail>");
		oBuilder.WriteString(L"<m_sJsonParams>{&quot;spreadsheetLayout&quot;:{&quot;gridLines&quot;:true,&quot;headings&quot;:true,&quot;fitToHeight&quot;:1,&quot;fitToWidth&quot;:1,&quot;orientation&quot;:&quot;landscape&quot;}}</m_sJsonParams>");

		oBuilder.WriteString(additional);
		oBuilder.WriteString(L"</TaskQueueDataConvert>");

		return oBuilder.GetData();
	}

public:
	CConverter(CInternalWorker* pWorker) : NSThreads::CBaseThread()
	{
		m_pInternal = pWorker;
		m_format = -1;
	}
	virtual ~CConverter()
	{
		Stop();
	}

	virtual DWORD ThreadProc()
	{
		if (m_bDiffOnly)
		{
			std::wstring strDirIn = m_file;
			std::wstring strDirOut = m_folder_dst;

			std::wstring strDiffsMain = NSFile::GetDirectoryName(strDirOut) + L"/" + L"DIFF";
			std::wstring strDiffs = strDiffsMain + L"/" + NSFile::GetFileName(m_file);

			int checkCode = m_pInternal->GenerateDiff(strDirIn, strDirOut, strDiffs);

			m_bRunThread = FALSE;
			m_pInternal->OnConvertFile(this, 0, 0, checkCode);

			return 0;
		}
		bool bIsOfficeFile = true;
		if (true)
		{
			CTemporaryCS oCS(&m_pInternal->m_oCS_OfficeUtils);
			COfficeFileFormatChecker oChecker;
			bIsOfficeFile = oChecker.isOfficeFile(m_file);
			if (bIsOfficeFile)
				m_format = oChecker.nFileType;
		}

		if (!bIsOfficeFile)
		{
			m_bRunThread = FALSE;
			m_pInternal->OnConvertFile(this, -1, 0, 0);
			return 0;
		}

#if 0
		std::map<int, bool>::iterator find = m_pInternal->m_formats.find(m_format);
		if ((find == m_pInternal->m_formats.end()) || (find->second == false))
		{
			m_bRunThread = FALSE;
			m_pInternal->OnConvertFile(this, -1);
			return 0;
		}
#endif

		DWORD dwTime1 = NSTimers::GetTickCount();

		std::wstring sProcess = NSFile::GetProcessDirectory();

		NSStringUtils::string_replace(sProcess, L"\\", L"/");
		NSStringUtils::string_replace(m_file, L"\\", L"/");

		std::wstring sDirectoryDst = m_folder_dst;
		NSStringUtils::string_replace(sDirectoryDst, L"\\", L"/");

#ifdef WIN32
		NSStringUtils::string_replace(m_file, L"//", L"\\\\");
#endif

		NSDirectory::CreateDirectories(CorrectPathW(sDirectoryDst));

		std::wstring additional = L"";
		if (!m_pInternal->m_sConvertParams.empty())
			additional = m_pInternal->m_sConvertParams;

		int format = m_docx ? AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX : AVS_OFFICESTUDIO_FILE_IMAGE;
		std::wstring sXmlConvert = GetXml(sDirectoryDst, format, sProcess, additional);
		std::wstring sTempFileForParams = sDirectoryDst + L"/" + L"params.xml";
		NSFile::CFileBinary::SaveToFile(sTempFileForParams, sXmlConvert, true);

		if (NSDirectory::Exists(sProcess + L"/converter"))
			sProcess += L"/converter";

		int nReturnCode = NSX2T::Convert(sProcess, sTempFileForParams);

		if (0 != nReturnCode)
		{
			m_bRunThread = FALSE;
			m_pInternal->OnConvertFile(this, nReturnCode, 0, 0);
			return 0;
		}

		if (g_save_x2t_xml != true)
			NSFile::CFileBinary::Remove(sTempFileForParams);

		DWORD dwTime2 = NSTimers::GetTickCount();

		int checkCode = crcEqual;
		if (!m_pInternal->m_bIsStandard && !m_docx)
		{
			// смотрим разницу
			std::wstring strDirIn = m_dir_to_check;
			std::wstring strDirOut = sDirectoryDst;

			std::wstring strDiffsMain = NSFile::GetDirectoryName(strDirOut) + L"/" + L"DIFF";
			std::wstring strDiffs = strDiffsMain + L"/" + NSFile::GetFileName(m_file);
			checkCode = m_pInternal->GenerateDiff(strDirIn, strDirOut, strDiffs);
		}

		m_pInternal->OnConvertFile(this, nReturnCode, (int)(dwTime2 - dwTime1), checkCode);
		return 0;
	}
};

CConverter* CInternalWorker::GetNextConverter()
{
	const std::wstring wsep = FILE_SEPARATOR_STR;
	if (m_nCurrent >= m_nCount)
			return NULL;

	if ((NSFile::GetFileExtention(m_files[m_nCurrent]) != L"pdf" && m_bTroughDocx) ||
			(NSFile::GetFileExtention(m_files[m_nCurrent]) != L"docx" && m_bTroughDocxWas))
	{
		{
			++m_nCurrent;
			++m_nCurrentComplete;
		}
		GetNextConverter();
		return NULL;
	}

	std::wstring sName = NSFile::GetFileName(m_files[m_nCurrent]);
	std::wstring sExt = NSFile::GetFileExtention(sName);
	std::wstring sFilenameNoExt = sName.substr(0, sName.size() - sExt.size() - 1);

	CConverter* pConverter = new CConverter(this);
	pConverter->DestroyOnFinish();
	pConverter->m_file = m_files[m_nCurrent];
	pConverter->m_bDiffOnly = m_bDiffOnly;
	pConverter->m_docx = m_bTroughDocx;
	++m_nCurrent;

	if (m_bTroughDocxWas)
			pConverter->m_dir_to_check = m_sDirToCheck + wsep + L"DOCX" + wsep + sFilenameNoExt + L".pdf";
		else
			pConverter->m_dir_to_check = NSFile::GetDirectoryName(pConverter->m_file);

	if (m_bTroughDocx)
		pConverter->m_folder_dst = m_sOutputFolder + wsep + L"DOCX" + wsep + sName;
	else if (m_bTroughDocxWas) // all in the same folder
		pConverter->m_folder_dst = m_sOutputFolder + wsep + L"DOCX" + wsep + sFilenameNoExt + L".pdf";
	else
		pConverter->m_folder_dst = m_sOutputFolder + wsep + sName;

	if (m_bIsStandard)
	{
		NSDirectory::CreateDirectories(CorrectPathW(pConverter->m_folder_dst));
		NSFile::CFileBinary::Copy(pConverter->m_file, pConverter->m_folder_dst + wsep + sName);
	}

	pConverter->Start(0);
	return pConverter;
}

void CInternalWorker::OnConvertFile(CConverter* pConverter, int nCode, int nTime, int nCheckCode)
{
	CTemporaryCS oCS(&m_oCS);

	OnReportFile(pConverter->m_file, nCode, nTime, nCheckCode);

	++m_nCurrentComplete;
	GetNextConverter();
}

void CInternalWorker::Start(int nCores)
{
	CTemporaryCS oCS(&m_oCS);

	int nSizeInit = nCores;
	if (nSizeInit > m_nCount)
		nSizeInit = m_nCount;

	for (int i = 0; i < nSizeInit; ++i)
		GetNextConverter();

	oCS.LeaveCS();

	while (IsWork())
		NSThreads::Sleep(500);

	if (m_bTroughDocx)
	{
		m_nCurrent = 0;
		m_nCurrentComplete = 0;
		m_files.clear();
		m_bTroughDocx = false;
		m_bTroughDocxWas = true;
		auto copy = m_sDirToCheck;
		OpenDir(m_sOutputFolder + FILE_SEPARATOR_STR + L"DOCX");
		m_sDirToCheck = copy;
	}

	for (int i = 0; i < nSizeInit; ++i)
		GetNextConverter();

	while (IsWork())
		NSThreads::Sleep(500);

}

void CInternalWorker::Cancel()
{
	CTemporaryCS oCS(&m_oCS);
	m_nCount = m_nCurrent;
}

void CheckFonts(const bool& bIsUseSystemFonts, std::vector<std::wstring>& arDirs)
{
	CApplicationFontsWorker oWorker;
	oWorker.m_sDirectory = NSFile::GetProcessDirectory() + L"/fonts";
	if (!NSDirectory::Exists(oWorker.m_sDirectory))
		NSDirectory::CreateDirectory(oWorker.m_sDirectory);

	oWorker.m_bIsUseSystemFonts = bIsUseSystemFonts;

	for (std::vector<std::wstring>::iterator i = arDirs.begin(); i != arDirs.end(); i++)
		oWorker.m_arAdditionalFolders.push_back(*i);

	oWorker.m_bIsNeedThumbnails = false;
	NSFonts::IApplicationFonts* pFonts = oWorker.Check();
	if (pFonts)
		pFonts->Release();
}

std::wstring CorrectDir(const std::wstring& sDir)
{
	if (sDir.empty())
		return L"";

	const wchar_t* data = sDir.c_str();

	std::wstring::size_type pos1 = (data[0] == '\"') ? 1 : 0;
	std::wstring::size_type pos2 = sDir.length();

	if (data[pos2 - 1] == '\"')
		--pos2;

	if (pos2 > 0 && ((data[pos2 - 1] == '\\') || (data[pos2 - 1] == '/')))
		--pos2;

	return sDir.substr(pos1, pos2 - pos1);
}

/*
 *
 * --input="input-standard-files-dir"
 * --output="output-dir"
 * --standard // generate standarts
 * --use-system-fonts="0/1/false/true"
 * --font-dirs="C:\\Windows\\Fonts;/usr/share/fonts;"
 * --cores=4
 *
 */

#ifdef WIN32
int wmain(int argc, wchar_t** argv)
#else
int main(int argc, char** argv)
#endif
{
	std::vector<std::wstring> arFontsDirs;
	bool bIsStandard = false;
	bool bDiffOnly = false;
	bool bTroughDocx = false;
	std::wstring strInputFolder = L"";
	std::wstring strOutputFolder = L"";
	bool bIsUseSystemFonts = true;
	int nCores = 1;
	std::wstring strAdditionalParams;

	for (int i = 0; i < argc; ++i)
	{
#ifdef WIN32
		std::wstring sParam(argv[i]);
#else
		std::string sParamA(argv[i]);
		std::wstring sParam = UTF8_TO_U(sParamA);
#endif

		if (sParam.find(L"--") == 0)
		{
			std::wstring sKey = L"";
			std::wstring sValue = L"";

			std::wstring::size_type _pos = sParam.find('=');
			if (std::wstring::npos == _pos)
			{
				sKey = sParam;
			}
			else
			{
				sKey = sParam.substr(0, _pos);
				sValue = sParam.substr(_pos + 1);
			}

			if (sKey == L"--input")
			{
				strInputFolder = CorrectDir(sValue);
			}
			else if (sKey == L"--output")
			{
				strOutputFolder = CorrectDir(sValue);
			}
			else if (sKey == L"--standard")
			{
				bIsStandard = true;
			}
			else if (sKey == L"--trough-docx")
			{
				bTroughDocx = true;
			}
			else if (sKey == L"--diff-only")
			{
				bDiffOnly = true;
				bTroughDocx = false;
			}
			else if (sKey == L"--use-system-fonts")
			{
				if (sValue == L"0" || sValue == L"false")
					bIsUseSystemFonts = false;
			}
			else if (sKey == L"--font-dirs")
			{
				const wchar_t* src = sValue.c_str();
				const wchar_t* limit = src + sValue.length();

				const wchar_t* srcPrev = src;
				while (src < limit)
				{
					if (*src == ';')
					{
						if (srcPrev != src)
						{
							arFontsDirs.push_back(std::wstring(srcPrev, src - srcPrev));
						}
						src++;
						srcPrev = src;
					}
					else
						src++;
				}

				if (src > srcPrev)
				{
					arFontsDirs.push_back(std::wstring(srcPrev, src - srcPrev));
				}
			}
			else if (sKey == L"--cores")
			{
				nCores = std::stoi(sValue);
				if (nCores < 1)
					nCores = 1;
			}
			else if (sKey == L"--additional-params")
			{
				strAdditionalParams = sValue;
			}
			else if (sKey == L"--x2t-debug")
			{
				g_save_x2t_xml = true;
			}
		}
	}

	DWORD dwTime1 = NSTimers::GetTickCount();

	CheckFonts(bIsUseSystemFonts, arFontsDirs);

#if 0
	if (true)
	{
		strInputFolder = L"D:\\standard";
		strOutputFolder = L"D:\\standard\\out";
		bIsStandard = true;
	}
	else
	{
		strInputFolder = L"D:\\standard\\out";
		strOutputFolder = L"D:\\standard\\check";
		bIsStandard = false;
	}
#endif

#ifdef _WIN32
	UINT currConsoleCP = GetConsoleOutputCP();
	SetConsoleOutputCP(CP_UTF8);
#endif

	CInternalWorker oWorker;
	oWorker.m_bDiffOnly = bDiffOnly;
	oWorker.OpenDir(strInputFolder);
	oWorker.m_sOutputFolder = strOutputFolder;
	oWorker.m_bIsStandard = bIsStandard;
	oWorker.m_bTroughDocx = bTroughDocx;
	oWorker.m_sConvertParams = strAdditionalParams;

	if (!NSDirectory::Exists(strOutputFolder))
		NSDirectory::CreateDirectories(CorrectPathW(strOutputFolder));

	oWorker.Start(nCores);

	DWORD dwTime2 = NSTimers::GetTickCount();

	DWORD dwTimeDelta = (dwTime2 - dwTime1) / 1000;
	std::cout << std::endl << "time: " << dwTimeDelta << std::endl;

	oWorker.WriteReport(NSFile::GetProcessDirectory() + L"/../report.txt", (int)(dwTime2 - dwTime1));

#ifdef _WIN32
	SetConsoleOutputCP(currConsoleCP);
#endif

	return 0;
}
