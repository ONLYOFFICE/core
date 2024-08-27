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
#ifndef DOC_BUILDER_EDITORS_CONFIG
#define DOC_BUILDER_EDITORS_CONFIG

#include "./config.h"
#include "../js_internal/js_base.h"
#include "../common/StringBuilder.h"

#ifdef CreateFile
#undef CreateFile
#endif

namespace NSDoctRenderer
{
	enum class DoctRendererEditorType
	{
		WORD      = 0,
		CELL      = 1,
		SLIDE     = 2,
		VISIO     = 3,
		PDF       = 4,

		INVALID = 255
	};

	static void AppendScript(NSStringUtils::CStringBuilderA* builder, const std::wstring& path)
	{
		NSFile::CFileBinary oFile;

		if (!oFile.OpenFile(path))
			return;

		int size = (int)oFile.GetFileSize();
		if (size < 3)
			return;

		BYTE* pData = new BYTE[size];
		DWORD dwReadSize = 0;
		oFile.ReadFile(pData, (DWORD)size, dwReadSize);
		oFile.CloseFile();

		int nOffset = 0;
		if (pData[0] == 0xEF && pData[1] == 0xBB && pData[2] == 0xBF)
			nOffset = 3;

		builder->WriteString((char*)(pData + nOffset), size - nOffset);
		builder->WriteString("\n\n");
		RELEASEARRAYOBJECTS(pData);
	}

	static bool RunScript(JSSmart<NSJSBase::CJSContext>& context, JSSmart<NSJSBase::CJSTryCatch>& try_catch, const std::wstring& path)
	{
		NSFile::CFileBinary oFile;

		if (!oFile.OpenFile(path))
			return "";

		int size = (int)oFile.GetFileSize();
		if (size < 3)
			return "";

		BYTE* pData = new BYTE[size];
		DWORD dwReadSize = 0;
		oFile.ReadFile(pData, (DWORD)size, dwReadSize);
		oFile.CloseFile();

		int nOffset = 0;
		if (pData[0] == 0xEF && pData[1] == 0xBB && pData[2] == 0xBF)
			nOffset = 3;

		context->runScript(std::string((char*)(pData + nOffset), size - nOffset), try_catch);
		RELEASEARRAYOBJECTS(pData);

		return try_catch->Check();
	}

	static std::wstring GetAllScript(NSStringUtils::CStringBuilderA* builder, const DoctRendererEditorType& type, CDoctRendererConfig* config)
	{
		for (std::vector<std::wstring>::const_iterator i = config->m_arrFiles.cbegin(); i != config->m_arrFiles.cend(); i++)
			AppendScript(builder, *i);

		std::wstring sFontsPath = config->m_strSdkPath + L"/common/libfont/engine";
#ifdef SUPPORT_HARFBUZZ_SHAPER
		sFontsPath += L"/fonts_native.js";
#else
		sFontsPath += L"/fonts_ie.js";
#endif

		switch (type)
		{
		case DoctRendererEditorType::WORD:
		{
			AppendScript(builder, config->m_strSdkPath + L"/word/sdk-all-min.js");
			AppendScript(builder, sFontsPath);
			AppendScript(builder, config->m_strSdkPath + L"/word/sdk-all.js");
			return config->m_strSdkPath + L"/word/sdk-all.cache";
		}
		case DoctRendererEditorType::SLIDE:
		{
			AppendScript(builder, config->m_strSdkPath + L"/slide/sdk-all-min.js");
			AppendScript(builder, sFontsPath);
			AppendScript(builder, config->m_strSdkPath + L"/slide/sdk-all.js");
			return config->m_strSdkPath + L"/slide/sdk-all.cache";
		}
		case DoctRendererEditorType::CELL:
		{
			AppendScript(builder, config->m_strSdkPath + L"/cell/sdk-all-min.js");
			AppendScript(builder, sFontsPath);
			AppendScript(builder, config->m_strSdkPath + L"/cell/sdk-all.js");
			builder->WriteString("\n$.ready();", 11);
			return config->m_strSdkPath + L"/cell/sdk-all.cache";
		}
		case DoctRendererEditorType::VISIO:
		{
			if (!NSFile::CFileBinary::Exists(config->m_strSdkPath + L"/draw/sdk-all-min.js"))
				return L"";
			AppendScript(builder, config->m_strSdkPath + L"/draw/sdk-all-min.js");
			AppendScript(builder, sFontsPath);
			AppendScript(builder, config->m_strSdkPath + L"/draw/sdk-all.js");
			return config->m_strSdkPath + L"/draw/sdk-all.cache";
		}
		case DoctRendererEditorType::PDF:
		{
			AppendScript(builder, config->m_strSdkPath + L"/word/sdk-all-min.js");
			AppendScript(builder, sFontsPath);
			AppendScript(builder, config->m_strSdkPath + L"/word/sdk-all.js");
			AppendScript(builder, config->m_strSdkPath + L"/pdf/src/engine/drawingfile_native.js");
			return config->m_strSdkPath + L"/pdf/sdk-all.cache";
		}
		default:
			break;
		}
		return L"";
	}

	static bool RunEditor(const DoctRendererEditorType& type, JSSmart<NSJSBase::CJSContext>& context, JSSmart<NSJSBase::CJSTryCatch>& try_catch, CDoctRendererConfig* config)
	{
		if (!RunScript(context, try_catch, config->m_strAllFonts))
			return false;

		NSStringUtils::CStringBuilderA builder;
		builder.AddSize(10 * 1024 * 1024);
		std::wstring sCachePath = GetAllScript(&builder, type, config);

		if (!sCachePath.empty())
			context->runScript(builder.GetData(), try_catch, config->m_bIsUseCache ? sCachePath : L"");

		return try_catch->Check();
	}
}

#endif // DOC_BUILDER_EDITORS_CONFIG
