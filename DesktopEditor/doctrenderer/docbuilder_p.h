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
#ifndef DOC_BUILDER_PRIVATE
#define DOC_BUILDER_PRIVATE

#include "./editors.h"
#include "docbuilder.h"
#include "doctrenderer.h"

#include <iostream>
#include <list>

#include "../../Common/OfficeFileFormats.h"
#include "../../Common/OfficeFileFormatChecker.h"

#include "js_internal/js_base.h"
#include "embed/NativeBuilderEmbed.h"
#include "embed/NativeBuilderDocumentEmbed.h"
#include "embed/NativeControlEmbed.h"
#include "embed/GraphicsEmbed.h"
#include "embed/Default.h"
#ifdef LINUX
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#endif

#ifdef BUIDLER_OPEN_DOWNLOAD_ENABLED
#include "../../Common/Network/FileTransporter/include/FileTransporter.h"
#endif

#include "../fontengine/ApplicationFontsWorker.h"
#include "../../OfficeUtils/src/OfficeUtils.h"

#include "../common/ProcessEnv.h"

#ifdef CreateFile
#undef CreateFile
#endif

namespace NSDoctRenderer
{
	static int GetFormatByTexExtention(const std::wstring& sExt)
	{
		int nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX;
		if (L"docx" == sExt)
			nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX;
		else if (L"doc" == sExt)
			nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC;
		else if (L"odt" == sExt)
			nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT;
		else if (L"rtf" == sExt)
			nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF;
		else if (L"txt" == sExt)
			nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT;
		else if (L"pptx" == sExt)
			nFormat = AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX;
		else if (L"odp" == sExt)
			nFormat = AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP;
		else if (L"xlsx" == sExt)
			nFormat = AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX;
		else if (L"xls" == sExt)
			nFormat = AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS;
		else if (L"ods" == sExt)
			nFormat = AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS;
		else if (L"csv" == sExt)
			nFormat = AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV;
		else if (L"pdf" == sExt)
			nFormat = AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF;
		else if (L"image" == sExt)
			nFormat = AVS_OFFICESTUDIO_FILE_IMAGE_PNG;
		else if (L"jpg" == sExt)
			nFormat = AVS_OFFICESTUDIO_FILE_IMAGE_JPG;
		else if (L"png" == sExt)
			nFormat = AVS_OFFICESTUDIO_FILE_IMAGE_PNG;
		else if (L"vsdx" == sExt)
			nFormat = AVS_OFFICESTUDIO_FILE_DRAW_VSDX;
		else if (L"docxf" == sExt)
			nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCXF;
		else if (L"oform" == sExt)
			nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM;
		else if (L"html" == sExt)
			nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML_IN_CONTAINER;
		else if (L"form" == sExt)
			nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM_PDF;
		return nFormat;
	}
}

namespace NSDoctRenderer
{
	class CString_Private
	{
	public:
		wchar_t* m_data;

	public:
		CString_Private()
		{
			m_data = NULL;
		}
		~CString_Private()
		{
			if (m_data)
				delete [] m_data;
		}

		void Attach(wchar_t* data)
		{
			m_data = data;
		}

		void Copy(CString_Private* copy)
		{
			if (copy->m_data)
			{
				delete [] copy->m_data;
				copy->m_data = NULL;
			}

			if (m_data == NULL)
				return;

			size_t len = wcslen(m_data);
			copy->m_data = new wchar_t[len + 1];
			memcpy(copy->m_data, m_data, (len + 1) * sizeof(wchar_t));
		}
	};
}

class CJSContextData;
namespace NSDoctRenderer
{
	class CDocBuilderContextScopeWrap
	{
	public:
		JSSmart<CJSContextScope> m_scope;

	public:
		CDocBuilderContextScopeWrap() : m_scope() {}
		~CDocBuilderContextScopeWrap() { Close(); }

		void Close() { m_scope.Release(); }
	};

	class CDocBuilderContextScope_Private
	{
	public:
		JSSmart<CDocBuilderContextScopeWrap> m_scope_wrap;
		CJSContextData* m_context_data;

	public:
		CDocBuilderContextScope_Private() : m_scope_wrap() { m_context_data = NULL; }
		~CDocBuilderContextScope_Private() {}
	};

	class CDocBuilderContext_Private
	{
	public:
		JSSmart<CJSContext> m_context;
		CJSContextData* m_context_data;

		CDocBuilderContext_Private() : m_context() { m_context_data = NULL; }
		~CDocBuilderContext_Private() { m_context.Release(); }
	};
}

namespace NSDoctRenderer
{
	class CDocBuilderValue_Private
	{
	public:
		class CParentValueInfo
		{
		public:
			JSSmart<CJSValue> m_parent;
			int m_parent_index;
			std::string m_parent_prop_name;

		public:
			CParentValueInfo() : m_parent(), m_parent_index(-1), m_parent_prop_name("")
			{
			}
		};
	public:
		JSSmart<CJSContext> m_context;
		JSSmart<CJSValue> m_value;

		// for operator [index]/["name"] and setter without references
		JSSmart<CParentValueInfo> m_parent;

		enum PrimitiveType
		{
			ptUndefined     = 0,
			ptNull          = 1,
			ptBool          = 2,
			ptInt           = 3,
			ptUInt          = 4,
			ptDouble        = 5,
			ptString        = 6
		};

		union PrimitiveValue
		{
			bool bValue;
			int nValue;
			unsigned int unValue;
			double dValue;
			wchar_t* sValue;
		};

		PrimitiveType m_nativeType;
		PrimitiveValue m_nativeValue;

	public:
		CDocBuilderValue_Private() : m_context(NULL)
		{
			m_nativeType = ptUndefined;
		}
		~CDocBuilderValue_Private()
		{

		}
		void Clear()
		{
			m_value.Release();
			ClearNative();
		}

		// native
		void CreateUndefined()
		{
			m_nativeType = ptUndefined;
		}
		void CreateNull()
		{
			m_nativeType = ptNull;
		}
		void CreateBool(const bool& value)
		{
			m_nativeType = ptBool;
			m_nativeValue.bValue = value;
		}
		void CreateInt(const int& value)
		{
			m_nativeType = ptInt;
			m_nativeValue.nValue = value;
		}
		void CreateUInt(const unsigned int& value)
		{
			m_nativeType = ptUInt;
			m_nativeValue.unValue = value;
		}
		void CreateDouble(const double& value)
		{
			m_nativeType = ptDouble;
			m_nativeValue.dValue = value;
		}
		void CreateString(const wchar_t*& value)
		{
			size_t len = wcslen(value) + 1;
			m_nativeType = ptString;
			m_nativeValue.sValue = new wchar_t[len];
			memcpy(m_nativeValue.sValue, value, len * sizeof(wchar_t));
		}
		void CreateString(const char*& value)
		{
			std::wstring sValue = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)value, strlen(value));
			const wchar_t* strTmp = sValue.c_str();
			CreateString(strTmp);
		}

		void ClearNative()
		{
			if (m_nativeType == ptString)
			{
				delete [] m_nativeValue.sValue;
			}
			m_nativeType = ptUndefined;
		}

		void CheckNative()
		{
			if (m_value.is_init())
				return;

			switch (m_nativeType)
			{
			case ptUndefined:
			{
				m_value = NSJSBase::CJSContext::createUndefined();
				break;
			}
			case ptNull:
			{
				m_value = NSJSBase::CJSContext::createNull();
				break;
			}
			case ptBool:
			{
				m_value = NSJSBase::CJSContext::createBool(m_nativeValue.bValue);
				break;
			}
			case ptInt:
			{
				m_value = NSJSBase::CJSContext::createInt(m_nativeValue.nValue);
				break;
			}
			case ptUInt:
			{
				m_value = NSJSBase::CJSContext::createUInt(m_nativeValue.unValue);
				break;
			}
			case ptDouble:
			{
				m_value = NSJSBase::CJSContext::createDouble(m_nativeValue.dValue);
				break;
			}
			case ptString:
			{
				m_value = NSJSBase::CJSContext::createString(m_nativeValue.sValue);
				break;
			}
			default:
				break;
			}

			ClearNative();
		}
	};
}

class CJSContextData
{
private:
	std::vector<JSSmart<NSDoctRenderer::CDocBuilderContextScopeWrap>> m_scopes;

public:
	CJSContextData() : m_scopes()
	{
	}
	~CJSContextData()
	{
		Clear();
	}

	void Clear()
	{
		for (std::vector<JSSmart<NSDoctRenderer::CDocBuilderContextScopeWrap>>::iterator iter = m_scopes.begin(); iter != m_scopes.end(); iter++)
		{
			(*iter)->Close();
		}
		m_scopes.clear();
	}

	void RemoveScope(JSSmart<NSDoctRenderer::CDocBuilderContextScopeWrap>& scope)
	{
		if (!scope.is_init())
			return;
		for (std::vector<JSSmart<NSDoctRenderer::CDocBuilderContextScopeWrap>>::iterator iter = m_scopes.begin(); iter != m_scopes.end(); iter++)
		{
			if (scope.GetPointer() == iter->GetPointer())
			{
				m_scopes.erase(iter);
				return;
			}
		}
	}

	void AddScope(JSSmart<NSDoctRenderer::CDocBuilderContextScopeWrap>& scope)
	{
		m_scopes.push_back(scope);
	}
};

class CV8RealTimeWorker
{
public:
	JSSmart<CJSContext> m_context;

	int m_nFileType;
	std::string m_sUtf8ArgumentJSON;
	std::string m_sGlobalVariable;

	CJSContextData m_oContextData;

public:

	CV8RealTimeWorker(NSDoctRenderer::CDocBuilder* pBuilder, const NSDoctRenderer::DoctRendererEditorType& type, NSDoctRenderer::CDoctRendererConfig* config);
	~CV8RealTimeWorker();

public:

	static void _LOGGING_ERROR_(const std::wstring& strType, const std::wstring& strError);

	bool ExecuteCommand(const std::wstring& command, NSDoctRenderer::CDocBuilderValue* retValue = NULL);

	std::string GetGlobalVariable();
	std::wstring GetJSVariable(std::wstring sParam);

	bool OpenFile(const std::wstring& sBasePath, const std::wstring& path, const NSDoctRenderer::DoctRendererEditorType& editorType, NSDoctRenderer::CDoctRendererConfig* config, CV8Params* pParams = NULL);
	bool SaveFileWithChanges(int type, const std::wstring& _path, const std::wstring& sJsonParams = L"");
	bool InitVariables();
};

namespace NSDoctRenderer
{
	class CDocBuilder_Private : public CDoctRendererConfig
	{
	public:
		std::wstring m_strEditorType;
		std::wstring m_strFilePath;

		std::wstring m_sTmpFolder;
		std::wstring m_sFileDir;
		int m_nFileType;
		bool m_bJavascriptBeforeEditor;

		std::wstring m_sX2tPath;

		CV8RealTimeWorker* m_pWorker;

		CAdditionalData* m_pAdditionalData;

		CDocBuilderParams m_oParams;
		bool m_bIsInit;

		bool m_bIsServerSafeVersion;
		std::wstring m_sFolderForSaveOnlyUseNames;

		std::string m_sGlobalVariable;
		bool m_bIsGlobalVariableUse;

		NSDoctRenderer::CDocBuilder* m_pParent;

		static std::wstring m_sExternalDirectory;
	public:
		CDocBuilder_Private() : CDoctRendererConfig(), m_sTmpFolder(NSFile::CFileBinary::GetTempPath()), m_nFileType(-1),
			m_pWorker(NULL), m_pAdditionalData(NULL), m_bIsInit(false), m_bIsServerSafeVersion(false),
			m_sGlobalVariable(""), m_bIsGlobalVariableUse(false), m_pParent(NULL), m_bJavascriptBeforeEditor(false)
		{
		}

		void Init()
		{
			if (m_bIsInit)
				return;

			std::wstring sWorkDir = m_oParams.m_sWorkDir;
			if (sWorkDir.empty() || !NSDirectory::Exists(sWorkDir))
			{
				sWorkDir = NSFile::GetProcessDirectory();
				if (!m_oParams.m_sWorkDir.empty())
				{
					std::wstring sCheck = sWorkDir;
					if (0 != m_oParams.m_sWorkDir.find('/'))
						sCheck += L"/";
					sCheck += m_oParams.m_sWorkDir;
					if (NSDirectory::Exists(sCheck))
						sWorkDir = sCheck;
				}
			}
			else
			{
				std::wstring sNatural = NSFile::GetProcessDirectory();
				if (0 != sWorkDir.find('/'))
					sNatural += L"/";
				sNatural += sWorkDir;
				if (NSDirectory::Exists(sNatural))
					sWorkDir = sNatural;
			}

			m_sX2tPath = sWorkDir;

			CDoctRendererConfig::Parse(sWorkDir);

			CheckFonts(m_oParams.m_bCheckFonts);

			m_bIsInit = true;
		}

		~CDocBuilder_Private()
		{
			CloseFile();

			RELEASEOBJECT(m_pAdditionalData);
		}

		void CheckFonts(bool bIsCheckFonts)
		{
			std::wstring sDirectory = NSFile::GetDirectoryName(m_strAllFonts);
			std::wstring strFontsSelectionBin = sDirectory + L"/font_selection.bin";

			if (!bIsCheckFonts && NSFile::CFileBinary::Exists(strFontsSelectionBin))
				return;

			CApplicationFontsWorker oWorker;
			oWorker.m_bIsUseSystemFonts = m_oParams.m_bIsSystemFonts;
			oWorker.m_arAdditionalFolders = m_oParams.m_arFontDirs;
			oWorker.m_bIsNeedThumbnails = false;
			oWorker.m_sDirectory = sDirectory;
			// это не рабочая папка, где только шрифты
			oWorker.m_bIsCleanDirectory = false;
			NSFonts::IApplicationFonts* pFonts = oWorker.Check();
			if(pFonts)
				pFonts->Release();
		}

		void CheckFileDir()
		{
			m_sFileDir = NSFile::CFileBinary::CreateTempFileWithUniqueName(m_sTmpFolder, L"DE_");
			if (NSFile::CFileBinary::Exists(m_sFileDir))
				NSFile::CFileBinary::Remove(m_sFileDir);

			NSStringUtils::string_replace(m_sFileDir, L"\\", L"/");

			std::wstring::size_type nPosPoint = m_sFileDir.rfind('.');
			if (nPosPoint != std::wstring::npos && nPosPoint > m_sTmpFolder.length())
			{
				m_sFileDir = m_sFileDir.substr(0, nPosPoint);
			}

			m_nFileType = -1;

			NSDirectory::CreateDirectory(m_sFileDir);
		}

		bool CreateFile(int type)
		{
			Init();
#if 1
			CheckFileDir();

			std::wstring sEmptyPath = m_sX2tPath + L"/empty/";

#if 0
			if (type & AVS_OFFICESTUDIO_FILE_DOCUMENT)
			{
				sEmptyPath = sEmptyPath + L"docx.bin";
				m_nFileType = 0;
			}
			else if (type & AVS_OFFICESTUDIO_FILE_PRESENTATION)
			{
				sEmptyPath = sEmptyPath + L"pptx.bin";
				m_nFileType = 1;
			}
			else if (type & AVS_OFFICESTUDIO_FILE_SPREADSHEET)
			{
				sEmptyPath = sEmptyPath + L"xlsx.bin";
				m_nFileType = 2;
			}
			else if (type & AVS_OFFICESTUDIO_FILE_DRAW)
			{
				sEmptyPath = sEmptyPath + L"vsdx.bin";
				m_nFileType = 7;
			}
			else
				return false;

			bool bRet = NSFile::CFileBinary::Copy(sEmptyPath, m_sFileDir + L"/Editor.bin");
			if (bRet)
			{
				NSDirectory::CreateDirectory(m_sFileDir + L"/media");
				NSDirectory::CreateDirectory(m_sFileDir + L"/changes");
			}
#endif

			if (type & AVS_OFFICESTUDIO_FILE_DOCUMENT)
			{
				if (type == AVS_OFFICESTUDIO_FILE_DOCUMENT_OFORM_PDF)
					sEmptyPath = sEmptyPath + L"new.pdf";
				else
					sEmptyPath = sEmptyPath + L"new.docx";
				m_nFileType = 0;
			}
			else if (type & AVS_OFFICESTUDIO_FILE_PRESENTATION)
			{
				sEmptyPath = sEmptyPath + L"new.pptx";
				m_nFileType = 1;
			}
			else if (type & AVS_OFFICESTUDIO_FILE_SPREADSHEET)
			{
				sEmptyPath = sEmptyPath + L"new.xlsx";
				m_nFileType = 2;
			}
			else if (type & AVS_OFFICESTUDIO_FILE_DRAW)
			{
				sEmptyPath = sEmptyPath + L"new.vsdx";
				m_nFileType = 7;
			}
			else
				return false;

			bool bRet = (0 == ConvertToInternalFormat(m_sFileDir, sEmptyPath, L"")) ? true : false;
			if (bRet)
			{
				NSDirectory::CreateDirectory(m_sFileDir + L"/media");
				NSDirectory::CreateDirectory(m_sFileDir + L"/changes");
			}

			if (m_bJavascriptBeforeEditor)
				CheckWorkerAfterOpen();

			return bRet;
#else
			std::wstring sPath = m_sX2tPath + L"/empty/new.";
			if (type & AVS_OFFICESTUDIO_FILE_DOCUMENT)
				sPath += L"docx";
			else if (type & AVS_OFFICESTUDIO_FILE_PRESENTATION)
				sPath += L"pptx";
			else if (type & AVS_OFFICESTUDIO_FILE_SPREADSHEET)
				sPath += L"xlsx";
			else if (type & AVS_OFFICESTUDIO_FILE_DRAW)
				sPath += L"vsdx";
			return this->OpenFile(sPath, L"");
#endif
		}

		void MoveFileOpen(const std::wstring& from, const std::wstring& to)
		{
#ifdef BUIDLER_OPEN_DOWNLOAD_ENABLED
			int n1 = (int)from.find (L"www");
			int n2 = (int)from.find (L"http");
			int n3 = (int)from.find (L"ftp");
			int n4 = (int)from.find (L"https");

			//если nI сранивать не с 0, то будут проблемы
			//потому что в инсталяции мы кладем файлы в /var/www...
			if (0 == n1 || 0 == n2 || 0 == n3 || 0 == n4)
			{
				NSNetwork::NSFileTransport::CFileDownloader oDownloader(from, false);
				oDownloader.SetFilePath(to);
				if (oDownloader.DownloadSync())
					return;
			}
#endif

#ifdef BUIDLER_OPEN_BASE64_ENABLED
			if (0 == from.find(L"data:"))
			{
				std::wstring::size_type findBase64 = from.find(L"base64,");
				if (std::wstring::npos != findBase64)
				{
					int nStartBase64 = (int)findBase64;
					if (50 > nStartBase64)
					{
						nStartBase64 += 7;
						const wchar_t* pStart = from.c_str() + nStartBase64;
						int nDataLen = (int)from.length() - nStartBase64;

						std::string sBase64 = NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(pStart, (LONG)nDataLen, false);

						BYTE* pDataDst = NULL;
						int nDataDstLen = 0;
						if (NSFile::CBase64Converter::Decode(sBase64.c_str(), (int)sBase64.length(), pDataDst, nDataLen))
						{
							NSFile::CFileBinary oFileDst;
							if (oFileDst.CreateFileW(to))
							{
								oFileDst.WriteFile(pDataDst, (DWORD)nDataDstLen);
								oFileDst.CloseFile();

								RELEASEARRAYOBJECTS(pDataDst);
								return;
							}

							RELEASEARRAYOBJECTS(pDataDst);
						}
					}
				}
			}
#endif

			// не открываем локальные файлы в серверной версии.
			if (m_bIsServerSafeVersion)
				return;

			NSFile::CFileBinary::Copy(from, to);
		}

		int ConvertToInternalFormat(const std::wstring& sFolder, const std::wstring& sFile, const std::wstring& sParams)
		{
			NSStringUtils::CStringBuilder oBuilder;
			oBuilder.WriteString(L"<?xml version=\"1.0\" encoding=\"utf-8\"?><TaskQueueDataConvert><m_sFileFrom>");
			oBuilder.WriteEncodeXmlString(sFile);
			oBuilder.WriteString(L"</m_sFileFrom><m_sFileTo>");
			oBuilder.WriteEncodeXmlString(sFolder);
			oBuilder.WriteString(L"/Editor.bin</m_sFileTo><m_nFormatTo>8192</m_nFormatTo>");

			if (!m_bIsNotUseConfigAllFontsDir)
			{
				oBuilder.WriteString(L"<m_sFontDir>");
				oBuilder.WriteEncodeXmlString(m_sX2tPath + L"/sdkjs/common");
				oBuilder.WriteString(L"</m_sFontDir>");
			}
			else
			{
				oBuilder.WriteString(L"<m_sFontDir>");
				oBuilder.WriteEncodeXmlString(NSFile::GetDirectoryName(m_strAllFonts));
				oBuilder.WriteString(L"</m_sFontDir>");

				oBuilder.WriteString(L"<m_sAllFontsPath>");
				oBuilder.WriteEncodeXmlString(m_strAllFonts);
				oBuilder.WriteString(L"</m_sAllFontsPath>");
			}

			oBuilder.WriteString(L"<m_bIsNoBase64>true</m_bIsNoBase64>");
			oBuilder.WriteString(L"<m_sThemeDir>./sdkjs/slide/themes</m_sThemeDir><m_bDontSaveAdditional>true</m_bDontSaveAdditional>");
			oBuilder.WriteString(sParams);

			std::string sOptions = NSProcessEnv::Save();
			if (!sOptions.empty())
				oBuilder.WriteString(UTF8_TO_U(sOptions));

			oBuilder.WriteString(L"</TaskQueueDataConvert>");

			std::wstring sXmlConvert = oBuilder.GetData();

			std::wstring sConverterExe = m_sX2tPath + L"/x2t";

			int nReturnCode = 0;

			std::wstring sTempFileForParams = sFolder + L"/params_from.xml";
			NSFile::CFileBinary::SaveToFile(sTempFileForParams, sXmlConvert, true);

#ifdef WIN32
			std::wstring sApp = L"x2t32 ";

			if (NSFile::CFileBinary::Exists(sConverterExe + L".exe"))
			{
				sApp = L"x2t ";
				sConverterExe += L".exe";
			}
			else
				sConverterExe += L"32.exe";

			STARTUPINFO sturtupinfo;
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

			PROCESS_INFORMATION processinfo;
			ZeroMemory(&processinfo,sizeof(PROCESS_INFORMATION));
			BOOL bResult = CreateProcessW(sConverterExe.c_str(), pCommandLine,
										  NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &sturtupinfo, &processinfo);

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

			NSFile::CFileBinary::Remove(sTempFileForParams);

			return nReturnCode;
		}

		std::wstring GetFileCopyExt(const std::wstring& path)
		{
			std::wstring sExtCopy = NSFile::GetFileExtention(path);

			if (true)
			{
				// для файлов по ссылке - расширение может быть плохим.
				const wchar_t* sExtCopyPtr = sExtCopy.c_str();
				int nExtCopyLen = sExtCopy.length();
				int nValidIndex = 0;
				while (nValidIndex < nExtCopyLen)
				{
					wchar_t c = sExtCopyPtr[nValidIndex];
					if ((c >= 'a' && c <= 'z') ||
							(c >= 'A' && c <= 'Z') ||
							(c >= '0' && c <= '9'))
					{
						++nValidIndex;
					}
					else
						break;
				}

				if (nValidIndex > 0)
				{
					sExtCopy = sExtCopy.substr(0, nValidIndex);
				}
				else
				{
					sExtCopy = L"tmp";
				}
			}

			return sExtCopy;
		}

		int OpenFile(const std::wstring& path, const std::wstring& params)
		{
			Init();

			LOGGER_SPEED_START();

			CheckFileDir();
			NSDirectory::CreateDirectory(m_sFileDir + L"/changes");

			std::wstring sExtCopy = GetFileCopyExt(path);
			std::wstring sFileCopy = m_sFileDir + L"/origin." + sExtCopy;
			MoveFileOpen(path, sFileCopy);

			COfficeFileFormatChecker oChecker;
			if (!oChecker.isOfficeFile(sFileCopy))
				return false;

			if (oChecker.nFileType & AVS_OFFICESTUDIO_FILE_DOCUMENT)
				m_nFileType = 0;
			if (oChecker.nFileType & AVS_OFFICESTUDIO_FILE_PRESENTATION)
				m_nFileType = 1;
			if (oChecker.nFileType & AVS_OFFICESTUDIO_FILE_SPREADSHEET)
				m_nFileType = 2;
			if (oChecker.nFileType & AVS_OFFICESTUDIO_FILE_DRAW)
				m_nFileType = 7;

			int nReturnCode = ConvertToInternalFormat(m_sFileDir, sFileCopy, params);

			LOGGER_SPEED_LAP("open_convert");

			if (0 == nReturnCode)
			{
				if (m_bJavascriptBeforeEditor)
					CheckWorkerAfterOpen();
				return 0;
			}

			NSDirectory::DeleteDirectory(m_sFileDir);
			m_sFileDir = L"";
			m_nFileType = -1;

			std::wstring sErrorLog = L"open file error (" + std::to_wstring(nReturnCode) + L")";
			CV8RealTimeWorker::_LOGGING_ERROR_(L"error: ", sErrorLog);
			return nReturnCode;
		}

		void CloseFile(bool bIsDestroyJS = true)
		{
			Init();

			if (NSDirectory::Exists(m_sFileDir))
				NSDirectory::DeleteDirectory(m_sFileDir);

			m_sFileDir = L"";
			m_nFileType = -1;

			if (m_pWorker)
				m_sGlobalVariable = m_pWorker->GetGlobalVariable();

			if (bIsDestroyJS)
				RELEASEOBJECT(m_pWorker);
		}

		std::wstring GetSaveFilePath(const std::wstring& path)
		{
			std::wstring _path = path;
			if (!m_sFolderForSaveOnlyUseNames.empty())
			{
				_path = m_sFolderForSaveOnlyUseNames;
				wchar_t last = m_sFolderForSaveOnlyUseNames.c_str()[m_sFolderForSaveOnlyUseNames.length() - 1];
				if (last != '/' && last != '\\')
					_path += L"/";
				_path += NSFile::GetFileName(path);
			}

			std::wstring sDstFileDir = NSFile::GetDirectoryName(_path);
			if ((sDstFileDir != _path) && !NSDirectory::Exists(sDstFileDir))
				NSDirectory::CreateDirectories(sDstFileDir);

			return _path;
		}

		int SaveFile(const int& type, const std::wstring& path, const wchar_t* params = NULL)
		{
			Init();

			if (-1 == m_nFileType)
			{
				CV8RealTimeWorker::_LOGGING_ERROR_(L"error (save)", L"file not opened!");
				return 1;
			}

			LOGGER_SPEED_START();

			std::wstring sConvertionParams = L"";
			if (NULL != params)
			{
				sConvertionParams = std::wstring(params);
				NSStringUtils::string_replace(sConvertionParams, L"\'", L"&quot;");
			}

			std::wstring sFileBin = L"/Editor.bin";

			if (!m_oParams.m_bSaveWithDoctrendererMode && m_pWorker)
			{
				std::wstring sJsonParams = sConvertionParams;
				if (!sJsonParams.empty())
				{
					std::wstring::size_type pos1 = sJsonParams.find(L">");
					std::wstring::size_type pos2 = sJsonParams.find(L"</");
					if (std::wstring::npos != pos1 && std::wstring::npos != pos2)
					{
						sJsonParams = sJsonParams.substr(pos1 + 1, pos2 - pos1 - 1);
						NSStringUtils::string_replace(sJsonParams, L"&quot;", L"\"");

						if (0 != sJsonParams.find(L"{"))
							sJsonParams = L"";
					}
					else
					{
						sJsonParams = L"";
					}
				}

				this->m_pWorker->SaveFileWithChanges(type, m_sFileDir + L"/Editor2.bin", sJsonParams);
				sFileBin = L"/Editor2.bin";
			}

			NSStringUtils::CStringBuilder oBuilder;

			std::wstring _path = GetSaveFilePath(path);

			oBuilder.WriteString(L"<?xml version=\"1.0\" encoding=\"utf-8\"?><TaskQueueDataConvert><m_sFileFrom>");
			oBuilder.WriteEncodeXmlString(m_sFileDir);
			oBuilder.WriteString(sFileBin + L"</m_sFileFrom><m_sFileTo>");
			oBuilder.WriteEncodeXmlString(_path);
			oBuilder.WriteString(L"</m_sFileTo><m_nFormatTo>");
			oBuilder.WriteString(std::to_wstring(type));
			oBuilder.WriteString(L"</m_nFormatTo><m_sThemeDir>");
			oBuilder.WriteEncodeXmlString(L"./sdkjs/slide/themes");
			if (!m_oParams.m_bSaveWithDoctrendererMode)
				oBuilder.WriteString(L"</m_sThemeDir><m_bDontSaveAdditional>true</m_bDontSaveAdditional>");
			else
				oBuilder.WriteString(L"</m_sThemeDir><m_bFromChanges>true</m_bFromChanges><m_bDontSaveAdditional>true</m_bDontSaveAdditional>");
			oBuilder.WriteString(L"<m_nCsvTxtEncoding>46</m_nCsvTxtEncoding><m_nCsvDelimiter>4</m_nCsvDelimiter>");

			if (!m_bIsNotUseConfigAllFontsDir)
			{
				oBuilder.WriteString(L"<m_sFontDir>");
				oBuilder.WriteEncodeXmlString(m_sX2tPath + L"/sdkjs/common");
				oBuilder.WriteString(L"</m_sFontDir>");
			}
			else
			{
				oBuilder.WriteString(L"<m_sFontDir>");
				oBuilder.WriteEncodeXmlString(NSFile::GetDirectoryName(m_strAllFonts));
				oBuilder.WriteString(L"</m_sFontDir>");

				oBuilder.WriteString(L"<m_sAllFontsPath>");
				oBuilder.WriteEncodeXmlString(m_strAllFonts);
				oBuilder.WriteString(L"</m_sAllFontsPath>");
			}

			if (!sConvertionParams.empty())
			{
				oBuilder.WriteString(sConvertionParams);
			}

			std::wstring sDstTmpDir = NSDirectory::CreateDirectoryWithUniqueName(m_sFileDir);

			oBuilder.WriteString(L"<m_sTempDir>");
			oBuilder.WriteEncodeXmlString(sDstTmpDir);
			oBuilder.WriteString(L"</m_sTempDir>");

			std::string sOptions = NSProcessEnv::Save();
			if (!sOptions.empty())
				oBuilder.WriteString(UTF8_TO_U(sOptions));

			oBuilder.WriteString(L"</TaskQueueDataConvert>");

			std::wstring sXmlConvert = oBuilder.GetData();

			std::wstring sConverterExe = m_sX2tPath + L"/x2t";

			int nReturnCode = 0;

			std::wstring sTempFileForParams = m_sFileDir + L"/params_to.xml";
			NSFile::CFileBinary::SaveToFile(sTempFileForParams, sXmlConvert, true);

#ifdef WIN32
			std::wstring sApp = L"x2t32 ";

			if (NSFile::CFileBinary::Exists(sConverterExe + L".exe"))
			{
				sApp = L"x2t ";
				sConverterExe += L".exe";
			}
			else
				sConverterExe += L"32.exe";

			STARTUPINFO sturtupinfo;
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

			PROCESS_INFORMATION processinfo;
			ZeroMemory(&processinfo,sizeof(PROCESS_INFORMATION));
			BOOL bResult = CreateProcessW(sConverterExe.c_str(), pCommandLine,
										  NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &sturtupinfo, &processinfo);

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

			// html correct (TODO: move to x2t)
			if (0 == nReturnCode && type == AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML_IN_CONTAINER)
			{
				COfficeUtils oUtils;
				if (S_OK == oUtils.IsArchive(_path))
				{
					std::wstring sTmpFile = sDstTmpDir + L"/tmp_html";
					NSDirectory::CreateDirectory(sTmpFile);
					if (S_OK == oUtils.ExtractToDirectory(_path, sTmpFile, NULL, 0))
					{
						std::vector<std::wstring> arFiles = NSDirectory::GetFiles(sTmpFile);
						if (arFiles.size() == 1)
						{
							NSFile::CFileBinary::Remove(_path);
							NSFile::CFileBinary::Move(arFiles[0], _path);
						}
					}
				}
			}


			NSDirectory::DeleteDirectory(sDstTmpDir);
			NSFile::CFileBinary::Remove(sTempFileForParams);

			LOGGER_SPEED_LAP("save_convert");

			if (0 == nReturnCode)
				return 0;

			std::wstring sErrorLog = L"save file error (" + std::to_wstring(nReturnCode) + L")";
			CV8RealTimeWorker::_LOGGING_ERROR_(L"error: ", sErrorLog);
			return nReturnCode;
		}

		bool CheckWorker()
		{
			if (NULL == m_pWorker)
			{
				m_pWorker = new CV8RealTimeWorker(m_pParent, GetEditorType(), this);
				m_pWorker->m_sUtf8ArgumentJSON = m_oParams.m_sArgumentJSON;
				m_pWorker->m_sGlobalVariable = m_sGlobalVariable;

				return CheckWorkerAfterOpen();
			}
			return true;
		}

		bool CheckWorkerAfterOpen()
		{
			if (!m_pWorker)
				return false;

			m_pWorker->m_nFileType = m_nFileType;
			if (-1 == m_nFileType)
			{
				m_bJavascriptBeforeEditor = true;
				return false;
			}

			m_bJavascriptBeforeEditor = false;

			CV8Params oParams;
			oParams.IsServerSaveVersion = m_bIsServerSafeVersion;
			oParams.DocumentDirectory = m_sFileDir;

			return m_pWorker->OpenFile(m_sX2tPath, m_sFileDir, GetEditorType(), this, &oParams);
		}

		int SaveFile(const std::wstring& ext, const std::wstring& path, const wchar_t* params = NULL)
		{
			int nType = GetFormatByTexExtention(ext);
			return SaveFile(nType, path, params);
		}

		bool ExecuteCommand(const std::wstring& command, CDocBuilderValue* retValue = NULL)
		{
			if (command.length() < 7 && !retValue) // minimum command (!!!)
				return true;

			Init();

			bool bRes = CheckWorker();

			if (!bRes && m_pWorker && m_bJavascriptBeforeEditor)
				m_pWorker->InitVariables();

			return m_pWorker->ExecuteCommand(command, retValue);
		}

		CDocBuilderContext GetContext()
		{
			CDocBuilderContext ctx;

			CheckWorker();

			ctx.m_internal->m_context = m_pWorker->m_context;
			ctx.m_internal->m_context_data = &m_pWorker->m_oContextData;
			return ctx;
		}

		NSDoctRenderer::DoctRendererEditorType GetEditorType()
		{
			switch (m_nFileType)
			{
			case 0:
				return NSDoctRenderer::DoctRendererEditorType::WORD;
			case 1:
				return NSDoctRenderer::DoctRendererEditorType::SLIDE;
			case 2:
				return NSDoctRenderer::DoctRendererEditorType::CELL;
			case 7:
				return NSDoctRenderer::DoctRendererEditorType::VISIO;
			default:
				break;
			}
			return NSDoctRenderer::DoctRendererEditorType::INVALID;
		}

		void WriteData(const wchar_t* path, const wchar_t* value, const bool& append)
		{
			std::wstring sValue(value);
			std::string sValueA = U_TO_UTF8(sValue);
			NSStringUtils::string_replaceA(sValueA, "%", "%%");

			std::wstring _sFile(path);
			std::wstring sFile = GetSaveFilePath(_sFile);

			if (!append && NSFile::CFileBinary::Exists(sFile))
				NSFile::CFileBinary::Remove(sFile);

			NSFile::CFileBinary oFile;
			FILE* pFile = oFile.OpenFileNative(sFile, append ? L"a+" : L"a");
			if (pFile)
			{
				fprintf(pFile, sValueA.c_str());
				fclose(pFile);
			}
		}
	};
}

#endif // DOC_BUILDER_PRIVATE
