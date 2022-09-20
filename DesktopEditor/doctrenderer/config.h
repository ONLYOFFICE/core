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
#ifndef DOC_BUILDER_CONFIG
#define DOC_BUILDER_CONFIG

#include "../xml/include/xmlutils.h"
#include "../common/File.h"
#include "../common/Directory.h"
#include "../common/SystemUtils.h"

namespace NSDoctRenderer
{
	class CDoctRendererConfig
	{
	public:
		std::vector<std::wstring> m_arrFiles;

		std::vector<std::wstring> m_arDoctSDK;
		std::vector<std::wstring> m_arPpttSDK;
		std::vector<std::wstring> m_arXlstSDK;

		std::wstring m_strAllFonts;
		bool m_bIsNotUseConfigAllFontsDir;

		std::wstring m_sConsoleLogFile;
		std::wstring m_sErrorsLogFile;

	public:
		CDoctRendererConfig() : m_bIsNotUseConfigAllFontsDir(false)
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
			XmlUtils::CXmlNodes oNodes;
			if (oNode.GetNodes(L"file", oNodes))
			{
				int nCount = oNodes.GetCount();
				XmlUtils::CXmlNode node;
				for (int i = 0; i < nCount; ++i)
				{
					oNodes.GetAt(i, node);
					files.push_back(private_GetFile(sConfigDir, node.GetText()));
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
			m_arDoctSDK.clear();
			m_arPpttSDK.clear();
			m_arXlstSDK.clear();

			std::wstring sConfigDir = sWorkDir + L"/";
			std::wstring sConfigPath = sConfigDir + L"DoctRenderer.config";

			XmlUtils::CXmlNode oNode;
			if (oNode.FromXmlFile(sConfigPath))
			{
				XmlUtils::CXmlNodes oNodes;
				if (oNode.GetNodes(L"file", oNodes))
				{
					int nCount = oNodes.GetCount();
					XmlUtils::CXmlNode node;
					for (int i = 0; i < nCount; ++i)
					{
						oNodes.GetAt(i, node);
						m_arrFiles.push_back(private_GetFile(sConfigDir, node.GetText()));
					}
				}

				if (!m_bIsNotUseConfigAllFontsDir)
				{
					std::wstring sAllFontsPath = oNode.ReadNodeText(L"allfonts");
					if (!sAllFontsPath.empty())
					{
						m_strAllFonts = private_GetFile(sConfigDir, sAllFontsPath);

						// на папку может не быть прав
						if (!NSFile::CFileBinary::Exists(m_strAllFonts))
						{
							FILE* pFileNative = NSFile::CFileBinary::OpenFileNative(m_strAllFonts, L"wb");
							if (!pFileNative)
							{
								std::wstring sAppDir = NSSystemUtils::GetAppDataDir();
								if (NSDirectory::CreateDirectory(sAppDir + L"/docbuilder"))
									m_strAllFonts = sAppDir + L"/docbuilder/AllFonts.js";
							}
							else
							{
								fclose(pFileNative);
							}
						}
					}
				}
				m_arrFiles.push_back(private_GetFile(sConfigDir, m_strAllFonts));
			}

			std::wstring sSdkPath = oNode.ReadNodeText(L"sdkjs");
			if (!sSdkPath.empty())
			{
				if (!NSDirectory::Exists(sSdkPath))
					sSdkPath = sConfigDir + sSdkPath;

				std::wstring sFontsPath = sSdkPath + L"/common/libfont/engine";
				if (!sFontsPath.empty())
				{
#ifdef SUPPORT_HARFBUZZ_SHAPER
					sFontsPath += L"/fonts_native.js";
#else
					sFontsPath += L"/fonts_ie.js";
#endif
				}

				m_arDoctSDK.push_back(sSdkPath + L"/word/sdk-all-min.js");
				m_arDoctSDK.push_back(sFontsPath);
				m_arDoctSDK.push_back(sSdkPath + L"/word/sdk-all.js");

				m_arPpttSDK.push_back(sSdkPath + L"/slide/sdk-all-min.js");
				m_arPpttSDK.push_back(sFontsPath);
				m_arPpttSDK.push_back(sSdkPath + L"/slide/sdk-all.js");

				m_arXlstSDK.push_back(sSdkPath + L"/cell/sdk-all-min.js");
				m_arXlstSDK.push_back(sFontsPath);
				m_arXlstSDK.push_back(sSdkPath + L"/cell/sdk-all.js");
			}

			m_sConsoleLogFile = oNode.ReadNodeText(L"LogFileConsoleLog");
			m_sErrorsLogFile = oNode.ReadNodeText(L"LogFileErrors");

			if (!m_sConsoleLogFile.empty())
				m_sConsoleLogFile = private_GetFile(sConfigDir, m_sConsoleLogFile);
			if (!m_sErrorsLogFile.empty())
				m_sErrorsLogFile = private_GetFile(sConfigDir, m_sErrorsLogFile);
		}
	};
}

#endif // DOC_BUILDER_CONFIG
