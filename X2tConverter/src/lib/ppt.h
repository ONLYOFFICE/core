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
#pragma once

#include "../../../MsBinaryFile/PptFile/Main/PPTFormatLib.h"
#include "../../OOXML/PPTXFormat/DrawingConverter/ASCOfficePPTXFile.h"
#include "common.h"

namespace NExtractTools
{
	inline _UINT32 ppt2pptx_dir_macro(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams, const bool& bIsMacro)
	{
		COfficePPTFile pptFile;
		pptFile.put_TempDirectory(convertParams.m_sTempDir);

		params.m_bMacro = bIsMacro;
		long nRes = pptFile.LoadFromFile(sFrom, sTo, params.getPassword(), params.m_bMacro);
		nRes = processEncryptionError(nRes, sFrom, params);
		return nRes;
	}

	_UINT32 ppt2pptx_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return ppt2pptx_dir_macro(sFrom, sTo, params, convertParams, false);
	}
	_UINT32 ppt2pptm_dir(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return ppt2pptx_dir_macro(sFrom, sTo, params, convertParams, true);
	}

	_UINT32 ppt2pptt_bin(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		// unzip pptx to temp folder
		std::wstring sTempUnpackedPPTX = combinePath(convertParams.m_sTempDir, L"pptx_unpacked") + FILE_SEPARATOR_STR; // leading slash is very important!
		NSDirectory::CreateDirectory(sTempUnpackedPPTX);

		COfficePPTFile pptFile;
		pptFile.put_TempDirectory(convertParams.m_sTempDir);

		params.m_bMacro = true;
		_UINT32 nRes = pptFile.LoadFromFile(sFrom, sTempUnpackedPPTX, params.getPassword(), params.m_bMacro);

		nRes = processEncryptionError(nRes, sFrom, params);
		if (SUCCEEDED_X2T(nRes))
		{
			// convert unzipped pptx to unzipped pptt
			CPPTXFile *pptx_file = new CPPTXFile();

			if (pptx_file)
			{
				pptx_file->SetFontDir(params.getFontPath());
				nRes = (S_OK == pptx_file->OpenFileToPPTY(sTempUnpackedPPTX, sTo)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;

				delete pptx_file;
			}
			return nRes;
		}
		return nRes;
	}

	_UINT32 ppt2pptx(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2ooxml(sFrom, sTo, params, convertParams, L"pptx", ppt2pptx_dir);
	}
	_UINT32 ppt2pptm(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2ooxml(sFrom, sTo, params, convertParams, L"pptx", ppt2pptm_dir);
	}

	_UINT32 ppt2pptt(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		return NSCommon::format2oot(sFrom, sTo, params, convertParams, L"pptt", ppt2pptt_bin);
	}
}
