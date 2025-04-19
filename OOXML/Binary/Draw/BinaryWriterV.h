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
