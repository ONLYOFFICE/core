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

#include "./../common_deploy.h"
#include "../docbuilder.h"
#include "../../common/File.h"
#include "../../common/SystemUtils.h"

#ifdef LINUX
#include "../../../DesktopEditor/common/File.h"
#endif

#include <iostream>

#include <string>
#define VALUE_TO_STRING(x) #x
#define VALUE(x) VALUE_TO_STRING(x)

#ifdef WIN32
void parse_args(NSDoctRenderer::CDocBuilder* builder, int argc, wchar_t *argv[])
#else
void parse_args(NSDoctRenderer::CDocBuilder* builder, int argc, char *argv[])
#endif
{
	for (int i = 0; i < argc; ++i)
	{
#ifdef WIN32
		std::wstring sW(argv[i]);
		std::string sParam = U_TO_UTF8(sW);
#else
		std::string sParam(argv[i]);
#endif

		if (sParam.find("--") == 0)
		{
			std::string::size_type _pos = sParam.find('=');
			if (std::string::npos == _pos)
				builder->SetProperty(sParam.c_str(), L"");
			else
			{
				std::string sName = sParam.substr(0, _pos);
				std::string sValue = sParam.substr(_pos + 1);

				std::wstring sValueW = UTF8_TO_U(sValue);
				builder->SetProperty(sName.c_str(), sValueW.c_str());
			}
		}
		else
			continue;
	}
}

bool CheckLicense(const std::wstring& sSrc, const std::wstring& sDst)
{
	if (sDst.empty())
		return false;
	NSFile::CFileBinary::Remove(sDst);
	NSFile::CFileBinary::Copy(sSrc, sDst);
	return NSFile::CFileBinary::Exists(sDst);
}

#ifdef WIN32
int wmain(int argc, wchar_t *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	if (argc <= 0)
		return 0;

	bool bIsHelp = false;
	bool bIsFonts = false;
	bool bIsLicense = false;
	for (int i = 0; i < argc; ++i)
	{
#ifdef WIN32
		std::wstring sW(argv[i]);
		std::string sParam(sW.begin(), sW.end());
#else
		std::string sParam(argv[i]);
#endif
		if (sParam == "-v" || sParam == "-version")
		{
			std::cout << "v" VALUE(INTVER) << std::endl;
			NSDoctRenderer::CDocBuilder oBuilder;
			oBuilder.ExecuteCommand(L"checkL");

			char* sSdkVer = oBuilder.GetVersion();
			if (NULL != sSdkVer)
			{
				std::string sSdkVerStd(sSdkVer);
				std::cout << "sdk version: " << sSdkVerStd << std::endl;
				delete [] sSdkVer;
			}

			return 0;
		}
		if (sParam == "-h" || sParam == "-help")
		{
			bIsHelp = true;
		}
		else if (sParam == "-f" || sParam == "-fonts")
		{
			bIsFonts = true;
		}
		else if (sParam == "-register")
		{
			bIsLicense = true;
		}
		else
		{
			if (bIsLicense)
			{
				std::wstring sLicensePathSrc = UTF8_TO_U(sParam);
				if (!NSFile::CFileBinary::Exists(sLicensePathSrc))
					return 1;

				std::wstring sLicensePath = NSSystemUtils::GetEnvVariable(L"ONLYOFFICE_BUILDER_LICENSE");
				if (CheckLicense(sLicensePathSrc, sLicensePath))
					return 0;

				sLicensePath = NSFile::GetProcessDirectory() + L"/license.xml";
				if (CheckLicense(sLicensePathSrc, sLicensePath))
					return 0;

				sLicensePath = NSSystemUtils::GetAppDataDir() + L"/docbuilder/license.xml";
				if (CheckLicense(sLicensePathSrc, sLicensePath))
					return 0;

				return 1;
			}
		}
	}

	if (bIsFonts)
	{
		NSDoctRenderer::CDocBuilder oBuilder;
		parse_args(&oBuilder, argc, argv);

		oBuilder.ExecuteCommand(L"checkL");
		return 0;
	}

	if (argc < 2 || bIsHelp)
	{
		std::cout << "USAGE: documentbuilder \"path_to_script_file\"" << std::endl;
		NSDoctRenderer::CDocBuilder oBuilder;
		oBuilder.ExecuteCommand(L"checkL");
		return 0;
	}

#ifdef WIN32
	std::wstring sBuildFile(argv[argc - 1]);
#else
	std::string sBuildFileA(argv[argc - 1]);
	std::wstring sBuildFile = UTF8_TO_U(sBuildFileA);
#endif

	bool bResult = true;
	if (true)
	{
		NSDoctRenderer::CDocBuilder oBuilder;

		// if this option is disabled - it will be disabled on parse_args
		oBuilder.SetProperty("--check-fonts", L"");

		//oBuilder.SetProperty("--use-doctrenderer-scheme", L"");
		//oBuilder.SetProperty("--work-directory", L"builder");

		parse_args(&oBuilder, argc - 1, argv);

		bResult = oBuilder.Run(sBuildFile.c_str());
	}

	NSDoctRenderer::CDocBuilder::Dispose();
	return bResult ? 0 : 1;
}
