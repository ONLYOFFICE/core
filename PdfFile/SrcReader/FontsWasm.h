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
#ifndef _FONTS_WASM_H
#define _FONTS_WASM_H

#ifndef TEST_CPP_BINARY
#include "emscripten.h"
EM_JS(char*, js_get_stream_id, (unsigned char* data, unsigned char* status), {
	return self.AscViewer.CheckStreamId(data, status);
});
EM_JS(int, js_free_id, (unsigned char* data), {
	self.AscViewer.Free(data);
	return 1;
});
#endif

#include "../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../DesktopEditor/common/File.h"

namespace NSWasm
{
	bool IsJSEnv()
	{
#ifdef TEST_CPP_BINARY
		return false;
#else
		return true;
#endif
	}

	std::wstring LoadFont(const std::wstring& sFontPath, int bBold, int bItalic)
	{
	#ifndef TEST_CPP_BINARY
		BYTE nStatus = 0;
		NSWasm::CData oRes;
		oRes.SkipLen();
		std::string sNameA = U_TO_UTF8(sFontPath);
		oRes.WriteString((unsigned char*)sNameA.c_str(), (unsigned int)sNameA.length());
		oRes.AddInt(bBold);
		oRes.AddInt(bItalic);
		oRes.WriteLen();
		char* pFontId = js_get_stream_id(oRes.GetBuffer(), &nStatus);
		std::wstring sRes;
		if (nStatus)
		{
			std::string wsFileNameA(pFontId);
			sRes = UTF8_TO_U(wsFileNameA);
		}
		js_free_id((unsigned char*)pFontId);
		return sRes;
	#else
		// пока заглушка - тут надо прочитать в стрим, чтобы дальше правильно сработать с кодировками
		if (!NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage()->Get(sFontPath))
		{
			DWORD dwSize = 0;
			BYTE* pData = NULL;
			if (NSFile::CFileBinary::ReadAllBytes(sFontPath, &pData, dwSize))
				NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage()->Add(sFontPath, pData, (LONG)dwSize, true);
			else
				return std::wstring();
		}
		return sFontPath;
	#endif
	}
}

#endif // _FONTS_WASM_H
