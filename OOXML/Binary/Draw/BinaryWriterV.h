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
#pragma once

#include "BinReaderWriterDefines.h"
#include "../Presentation/BinaryFileReaderWriter.h"
#include "../Presentation/BinReaderWriterDefines.h"
#include "../Document/DocWrapper/FontProcessor.h"

#include "../../PPTXFormat/DrawingConverter/ASCOfficeDrawingConverter.h"
#include "../../VsdxFormat/Vsdx.h"

#include "../../../DesktopEditor/common/Directory.h"

namespace OOX
{
	namespace Visio
	{
		class CVsdx;
		class CDocument;
	}
}

namespace BinVsdxRW 
{
	class BinaryCommonWriter
	{
	public:
		NSBinPptxRW::CBinaryFileWriter& m_oStream;
		BinaryCommonWriter(NSBinPptxRW::CBinaryFileWriter& oCBufferedStream);
		int WriteItemStart(BYTE type);
		void WriteItemEnd(int nStart);
		int WriteItemWithLengthStart();
		void WriteItemWithLengthEnd(int nStart);
		void WriteBytesArray(BYTE* pData, long nDataSize);
	};

	class BinaryFileWriter 
	{
	private:
		BinaryCommonWriter* m_oBcw;
		int m_nLastFilePos;
		int m_nMainTableStart;
		int m_nRealTableCount = 0;
		DocWrapper::FontProcessor& m_oFontProcessor;
	public:
		BinaryFileWriter(DocWrapper::FontProcessor& oFontProcessor);
		~BinaryFileWriter();
       
		_UINT32 Open(const std::wstring& sInputDir, const std::wstring& sFileDst, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter, bool bIsNoBase64);
		
		void WriteMainTableStart(NSBinPptxRW::CBinaryFileWriter &oBufferedStream);
		void WriteContent(OOX::Document *pDocument, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter);
		void WriteMainTableEnd();

		std::wstring WriteFileHeader(int nDataSize, int version);
		int GetMainTableSize();

	private:
		int WriteTableStart(BYTE type, int nStartPos = -1);
		void WriteTableEnd(int nCurPos);
	};
}
