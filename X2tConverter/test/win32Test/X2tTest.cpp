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

#if defined(_WIN64)
	#if defined(_DEBUG)
		#pragma comment(lib, "../../../build/lib/win_64/DEBUG/doctrenderer.lib")
		#pragma comment(lib, "../../../build/lib/win_64/DEBUG/HtmlRenderer.lib")
		#pragma comment(lib, "../../../build/lib/win_64/DEBUG/PdfReader.lib")
		#pragma comment(lib, "../../../build/lib/win_64/DEBUG/PdfWriter.lib")
		#pragma comment(lib, "../../../build/lib/win_64/DEBUG/XpsFile.lib")
		#pragma comment(lib, "../../../build/lib/win_64/DEBUG/EpubFile.lib")
		#pragma comment(lib, "../../../build/lib/win_64/DEBUG/DjVuFile.lib")
		#pragma comment(lib, "../../../build/lib/win_64/DEBUG/Fb2File.lib")
		#pragma comment(lib, "../../../build/lib/win_64/DEBUG/HtmlFile2.lib")
		#pragma comment(lib, "../../../build/lib/win_64/DEBUG/graphics.lib")
		#pragma comment(lib, "../../../build/lib/win_64/DEBUG/kernel_network.lib")
		#pragma comment(lib, "../../../build/lib/win_64/DEBUG/kernel.lib")
		#pragma comment(lib, "../../../build/lib/win_64/DEBUG/UnicodeConverter.lib")
#else
		#pragma comment(lib, "../../../build/lib/win_64/doctrenderer.lib")
		#pragma comment(lib, "../../../build/lib/win_64/HtmlRenderer.lib")
		#pragma comment(lib, "../../../build/lib/win_64/PdfFile.lib")
		#pragma comment(lib, "../../../build/lib/win_64/XpsFile.lib")
		#pragma comment(lib, "../../../build/lib/win_64/EpubFile.lib")
		#pragma comment(lib, "../../../build/lib/win_64/DjVuFile.lib")
		#pragma comment(lib, "../../../build/lib/win_64/Fb2File.lib")
		#pragma comment(lib, "../../../build/lib/win_64/HtmlFile2.lib")
		#pragma comment(lib, "../../../build/lib/win_64/graphics.lib")
		#pragma comment(lib, "../../../build/lib/win_64/kernel_network.lib")
		#pragma comment(lib, "../../../build/lib/win_64/kernel.lib")
		#pragma comment(lib, "../../../build/lib/win_64/UnicodeConverter.lib")
	#endif
	#pragma comment(lib, "../../../build/bin/icu/win_64/icuuc.lib")
#elif defined (_WIN32)
	#if defined(_DEBUG)
		#pragma comment(lib, "../../../build/lib/win_32/DEBUG/doctrenderer.lib")
		#pragma comment(lib, "../../../build/lib/win_32/DEBUG/HtmlRenderer.lib")
		#pragma comment(lib, "../../../build/lib/win_32/DEBUG/DocxRenderer.lib")
		#pragma comment(lib, "../../../build/lib/win_32/DEBUG/PdfFile.lib")
		#pragma comment(lib, "../../../build/lib/win_32/DEBUG/XpsFile.lib")
		#pragma comment(lib, "../../../build/lib/win_32/DEBUG/EpubFile.lib")
		#pragma comment(lib, "../../../build/lib/win_32/DEBUG/DjVuFile.lib")
		#pragma comment(lib, "../../../build/lib/win_32/DEBUG/Fb2File.lib")
		#pragma comment(lib, "../../../build/lib/win_32/DEBUG/HtmlFile2.lib")
		#pragma comment(lib, "../../../build/lib/win_32/DEBUG/graphics.lib")
		#pragma comment(lib, "../../../build/lib/win_32/DEBUG/kernel.lib")
		#pragma comment(lib, "../../../build/lib/win_32/DEBUG/kernel_network.lib")
		#pragma comment(lib, "../../../build/lib/win_32/DEBUG/UnicodeConverter.lib")
	#else
		#pragma comment(lib, "../../../build/lib/win_32/doctrenderer.lib")
		#pragma comment(lib, "../../../build/lib/win_32/HtmlRenderer.lib")
		#pragma comment(lib, "../../../build/lib/win_32/DocxRenderer.lib")
		#pragma comment(lib, "../../../build/lib/win_32/PdfFile.lib")
		#pragma comment(lib, "../../../build/lib/win_32/XpsFile.lib")
		#pragma comment(lib, "../../../build/lib/win_32/EpubFile.lib")
		#pragma comment(lib, "../../../build/lib/win_32/DjVuFile.lib")
		#pragma comment(lib, "../../../build/lib/win_32/Fb2File.lib")
		#pragma comment(lib, "../../../build/lib/win_32/HtmlFile2.lib")
		#pragma comment(lib, "../../../build/lib/win_32/graphics.lib")
		#pragma comment(lib, "../../../build/lib/win_32/kernel_network.lib")
		#pragma comment(lib, "../../../build/lib/win_32/kernel.lib")
		#pragma comment(lib, "../../../build/lib/win_32/UnicodeConverter.lib")
	#endif
		#pragma comment(lib, "../../../build/bin/icu/win_32/icuuc.lib")
#endif

#include "../../src/main.cpp"