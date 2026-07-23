/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#ifndef DOC_BUILDER_CONFIG
#define DOC_BUILDER_CONFIG

#include "../common/Directory.h"
#include "../common/File.h"
#include "../common/SystemUtils.h"
#include "../xml/include/xmlutils.h"
#include "../fontengine/TextHyphen.h"

#define VALUE_TO_STRING(x) #x
#define VALUE(x) VALUE_TO_STRING(x)

namespace NSDoctRenderer
{
	class CAdditionalData
	{
	public:
		CAdditionalData() {}
		virtual ~CAdditionalData() {}
		virtual std::string getParam(const std::wstring& name) { return ""; }
	};

	class CDocBuilderParams
	{
	public:
		CDocBuilderParams() :
			  m_bCheckFonts(false),
			  m_sWorkDir(L""),
			  m_bSaveWithDoctrendererMode(false),
			  m_sArgumentJSON(""),
			  m_bIsSystemFonts(true)
		{
		}

	public:
		bool m_bCheckFonts;
		std::wstring m_sWorkDir;
		bool m_bSaveWithDoctrendererMode;
		std::string m_sArgumentJSON;

		bool m_bIsSystemFonts;
		std::vector<std::wstring> m_arFontDirs;
	};

	class CDoctRendererConfig
	{
	public:
		std::wstring m_strSdkPath;

		std::vector<std::wstring> m_arrFiles;

		std::wstring m_strAllFonts;
		bool m_bIsNotUseConfigAllFontsDir;

		bool m_bIsUseCache;

		std::wstring m_sConsoleLogFile;
		std::wstring m_sErrorsLogFile;

	public:
		CDoctRendererConfig() : m_bIsNotUseConfigAllFontsDir(false), m_bIsUseCache(true)
		{
		}

	private:
		std::wstring private_GetFile(const std::wstring& sConfigDir, const std::wstring& sFile)
		{
			if (NSFile::CFileBinary::Exists(sFile) && !NSFile::CFileBinary::Exists(sConfigDir + sFile))
				return sFile;
			return sConfigDir + sFile;
		}
		void private_LoadSDK_scripts(XmlUtils::CXmlNode& oNode, std::vector<std::wstring>& files, const std::wstring& sConfigDir)
		{
			std::vector<XmlUtils::CXmlNode> oNodes;
			if (oNode.GetNodes(L"file", oNodes))
			{
				size_t nCount = oNodes.size();
				XmlUtils::CXmlNode node;
				for (size_t i = 0; i < nCount; ++i)
				{
					files.push_back(private_GetFile(sConfigDir, oNodes[i].GetText()));
				}
			}
		}

	public:
		void SetAllFontsExternal(const std::wstring& sFilePath)
		{
			m_strAllFonts = private_GetFile(NSFile::GetProcessDirectory() + L"/", sFilePath);
			m_bIsNotUseConfigAllFontsDir = true;
		}

		void Parse(const std::wstring& sWorkDir)
		{
			m_arrFiles.clear();

			std::wstring sConfigDir = sWorkDir + L"/";
			std::wstring sConfigPath = sConfigDir + L"DoctRenderer.config";

			XmlUtils::CXmlNode oNode;
			if (oNode.FromXmlFile(sConfigPath))
			{
				std::vector<XmlUtils::CXmlNode> oNodes;
				if (oNode.GetNodes(L"file", oNodes))
				{
					size_t nCount = oNodes.size();
					XmlUtils::CXmlNode node;
					for (size_t i = 0; i < nCount; ++i)
					{
						m_arrFiles.push_back(private_GetFile(sConfigDir, oNodes[i].GetText()));
					}
				}

				XmlUtils::CXmlNode oNodeDict;
				if (oNode.GetNode(L"dictionaries", oNodeDict))
				{
					NSHyphen::CEngine::Init(private_GetFile(sConfigDir, oNodeDict.GetText()));
				}

				if (!m_bIsNotUseConfigAllFontsDir)
				{
					std::wstring sAllFontsPath = oNode.ReadNodeText(L"allfonts");
					if (!sAllFontsPath.empty())
					{
						if (NSFile::CFileBinary::Exists(sConfigDir + sAllFontsPath))
							m_strAllFonts = sConfigDir + sAllFontsPath;
						else if (NSFile::CFileBinary::Exists(sAllFontsPath))
							m_strAllFonts = sAllFontsPath;
						else
						{
							std::wstring sAllFontsDir = NSFile::GetDirectoryName(sAllFontsPath);
							if (NSDirectory::Exists(sConfigDir + sAllFontsDir))
								m_strAllFonts = sConfigDir + sAllFontsPath;
							else
								m_strAllFonts = sAllFontsPath;
						}

						// folder may not have permissions
						if (!NSFile::CFileBinary::Exists(m_strAllFonts))
						{
							FILE* pFileNative = NSFile::CFileBinary::OpenFileNative(m_strAllFonts, L"wb");
							if (!pFileNative)
							{
								std::wstring sAppDir = NSSystemUtils::GetAppDataDir();
								if (NSDirectory::CreateDirectory(sAppDir + L"/docbuilder"))
								{
									m_strAllFonts = sAppDir + L"/docbuilder/AllFonts.js";
								}
							}
							else
							{
								fclose(pFileNative);
								NSFile::CFileBinary::Remove(m_strAllFonts);
							}
						}
					}
				}
			}

			m_strSdkPath = oNode.ReadNodeText(L"sdkjs");
			if (!m_strSdkPath.empty())
			{
				if (0 == m_strSdkPath.find(L"./") || !NSDirectory::Exists(m_strSdkPath))
					m_strSdkPath = sConfigDir + m_strSdkPath;
			}

			m_sConsoleLogFile = oNode.ReadNodeText(L"LogFileConsoleLog");
			m_sErrorsLogFile = oNode.ReadNodeText(L"LogFileErrors");

			if (!m_sConsoleLogFile.empty())
				m_sConsoleLogFile = private_GetFile(sConfigDir, m_sConsoleLogFile);
			if (!m_sErrorsLogFile.empty())
				m_sErrorsLogFile = private_GetFile(sConfigDir, m_sErrorsLogFile);
		}

		char* GetVersion()
		{
			std::string sVersion = VALUE(INTVER);

			size_t sSrcLen = sVersion.size();
			if (sSrcLen == 0)
				return NULL;

			char* sRet = new char[sSrcLen + 1];
			memcpy(sRet, sVersion.c_str(), sSrcLen);
			sRet[sSrcLen] = '\0';
			return sRet;
		}
	};
}

#endif // DOC_BUILDER_CONFIG
