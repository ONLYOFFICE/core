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

#include "BinaryWriterV.h"
#include "BinaryReaderV.h"

#include "../../../Common/OfficeFileFormats.h"
#include "../../../Common/Base64.h"
#include "../../../Common/OfficeFileErrorDescription.h"

#include "../Presentation/FontCutter.h"
#include "../../PPTXFormat/Logic/HeadingVariant.h"
#include "../../PPTXFormat/Logic/Shape.h"

#include "../../VsdxFormat/Vsdx.h"
#include "../../VsdxFormat/VisioDocument.h"

#include "../../SystemUtility/SystemUtility.h"
#include "../../DocxFormat/Media/OleObject.h"
#include "../../DocxFormat/Media/ActiveX.h"
#include "../../DocxFormat/Media/VbaProject.h"
#include "../../DocxFormat/App.h"
#include "../../DocxFormat/Core.h"
//#include "../../DocxFormat/CustomXml.h"

#include "../../../DesktopEditor/common/Directory.h"
#include "../../../Common/OfficeFileFormatChecker.h"
#include "../../../OfficeUtils/src/OfficeUtils.h"

namespace BinVsdxRW 
{
	BinaryCommonWriter::BinaryCommonWriter(NSBinPptxRW::CBinaryFileWriter& oCBufferedStream) :m_oStream(oCBufferedStream)
	{
	}
	int BinaryCommonWriter::WriteItemStart(BYTE type)
	{
		m_oStream.WriteBYTE(type);
		return WriteItemWithLengthStart();
	}
	void BinaryCommonWriter::WriteItemEnd(int nStart)
	{
		WriteItemWithLengthEnd(nStart);
	}
	int BinaryCommonWriter::WriteItemWithLengthStart()
	{
		int nStartPos = m_oStream.GetPosition();
		m_oStream.Skip(4);
		return nStartPos;
	}
	void BinaryCommonWriter::WriteItemWithLengthEnd(int nStart)
	{
		int nEnd = m_oStream.GetPosition();
		m_oStream.SetPosition(nStart);
		m_oStream.WriteLONG(nEnd - nStart - 4);
		m_oStream.SetPosition(nEnd);
	}
	void BinaryCommonWriter::WriteBytesArray(BYTE* pData, long nDataSize)
	{
		int nCurPos = WriteItemWithLengthStart();
		m_oStream.WriteBYTEArray(pData, nDataSize);
		WriteItemWithLengthEnd(nCurPos);
	}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
BinaryFileWriter::BinaryFileWriter(DocWrapper::FontProcessor& oFontProcessor) : m_oBcw(NULL), m_oFontProcessor(oFontProcessor)
{
	m_nRealTableCount = 0;
}
BinaryFileWriter::~BinaryFileWriter()
{
	RELEASEOBJECT(m_oBcw);
}
_UINT32 BinaryFileWriter::Open(const std::wstring& sInputDir, const std::wstring& sFileDst, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter, bool bIsNoBase64)
{
	_UINT32 result = 0;

	OOX::CPath pathDst(sFileDst);
//создаем папку для media
    std::wstring mediaDir = pathDst.GetDirectory() + L"media";
	NSDirectory::CreateDirectory(mediaDir);

    pOfficeDrawingConverter->SetDstPath(pathDst.GetDirectory() + FILE_SEPARATOR_STR + L"visio");
    pOfficeDrawingConverter->SetMediaDstPath(mediaDir);

	NSBinPptxRW::CBinaryFileWriter& oBufferedStream = *pOfficeDrawingConverter->m_pBinaryWriter;
	oBufferedStream.m_strMainFolder = pathDst.GetDirectory();

	m_oBcw = new BinaryCommonWriter(oBufferedStream);

	OOX::Draw::CVsdx *pVsdx = new OOX::Draw::CVsdx(OOX::CPath(sInputDir));

	if (0 != result)
	{
		RELEASEOBJECT(pVsdx);
		return result;
	}

	if (bIsNoBase64)
	{
		oBufferedStream.WriteStringUtf8(WriteFileHeader(0, g_nFormatVersionNoBase64));
	}
	int nHeaderLen = oBufferedStream.GetPosition();

	WriteMainTableStart(oBufferedStream);
	WriteContent(pVsdx, pOfficeDrawingConverter);
	WriteMainTableEnd();

	BYTE* pbBinBuffer = oBufferedStream.GetBuffer();
	int nBinBufferLen = oBufferedStream.GetPosition();
	if (bIsNoBase64)
	{
		NSFile::CFileBinary oFile;
		oFile.CreateFileW(sFileDst);

		//write header and main table
		oFile.WriteFile(pbBinBuffer, nBinBufferLen);
		oFile.CloseFile();
	}
	else
	{
		int nBase64BufferLen = Base64::Base64EncodeGetRequiredLength(nBinBufferLen, Base64::B64_BASE64_FLAG_NOCRLF);
		BYTE* pbBase64Buffer = new BYTE[nBase64BufferLen + 64];
		if (true == Base64_1::Base64Encode(pbBinBuffer, nBinBufferLen, pbBase64Buffer, &nBase64BufferLen))
		{
			NSFile::CFileBinary oFile;
			oFile.CreateFileW(sFileDst);
			oFile.WriteStringUTF8(WriteFileHeader(nBinBufferLen, g_nFormatVersion));
			oFile.WriteFile(pbBase64Buffer, nBase64BufferLen);
			oFile.CloseFile();
		}
		else
		{
			result = AVS_FILEUTILS_ERROR_CONVERT;
		}
		RELEASEARRAYOBJECTS(pbBase64Buffer);
	}


	RELEASEOBJECT(pVsdx);

	return result;
}

void BinaryFileWriter::WriteContent(OOX::Document *pDocument, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter)
{
	OOX::Draw::CVsdx *pVsdx = dynamic_cast<OOX::Draw::CVsdx*>(pDocument);

	if (!pVsdx) return;

	int nCurPos = 0;

	if (pVsdx->m_pApp.IsInit())
	{
		nCurPos = WriteTableStart(c_oSerTableTypes::App);
		pVsdx->m_pApp->toPPTY(&m_oBcw->m_oStream);
		this->WriteTableEnd(nCurPos);
	}

	if (pVsdx->m_pCore.IsInit())
	{
		nCurPos = WriteTableStart(c_oSerTableTypes::Core);
		pVsdx->m_pCore->toPPTY(&m_oBcw->m_oStream);
		this->WriteTableEnd(nCurPos);
	}

	smart_ptr<OOX::File> pFile = pVsdx->Find(OOX::FileTypes::CustomProperties);
	PPTX::CustomProperties *pCustomProperties = dynamic_cast<PPTX::CustomProperties*>(pFile.GetPointer());
	if (pCustomProperties)
	{
		nCurPos = WriteTableStart(c_oSerTableTypes::CustomProperties);
		pCustomProperties->toPPTY(&m_oBcw->m_oStream);
		this->WriteTableEnd(nCurPos);
	}

	if (pVsdx->m_pDocument.IsInit())
	{
		nCurPos = WriteTableStart(c_oSerTableTypes::Document);
		pVsdx->m_pDocument->toPPTY(&m_oBcw->m_oStream);
		WriteTableEnd(nCurPos);
	}
}

std::wstring BinaryFileWriter::WriteFileHeader(int nDataSize, int version)
{
    std::wstring sHeader = std::wstring(g_sFormatSignature) + L";v" + std::to_wstring(version)+ L";" + std::to_wstring(nDataSize) + L";";
	return sHeader;
}
void BinaryFileWriter::WriteMainTableStart(NSBinPptxRW::CBinaryFileWriter &oBufferedStream)
{
	if (!m_oBcw)
		m_oBcw = new BinaryCommonWriter(oBufferedStream);

	m_nRealTableCount = 0;
	m_nMainTableStart = m_oBcw->m_oStream.GetPosition();
	//вычисляем с какой позиции можно писать таблицы
	m_nLastFilePos = m_nMainTableStart + GetMainTableSize();
	//Write mtLen
	m_oBcw->m_oStream.WriteBYTE(0);
}
int BinaryFileWriter::GetMainTableSize()
{
	return 128 * 5;//128 items of 5 bytes
}
void BinaryFileWriter::WriteMainTableEnd()
{
	m_oBcw->m_oStream.SetPosition(m_nMainTableStart);
	m_oBcw->m_oStream.WriteBYTE(m_nRealTableCount);

	m_oBcw->m_oStream.SetPosition(m_nLastFilePos);
}
int BinaryFileWriter::WriteTableStart(BYTE type, int nStartPos)
{
	if(-1 != nStartPos)
		m_oBcw->m_oStream.SetPosition(nStartPos);
	//Write mtItem
	//Write mtiType
	m_oBcw->m_oStream.WriteBYTE(type);
	//Write mtiOffBits
	m_oBcw->m_oStream.WriteLONG(m_nLastFilePos);

	//Write table
	//Запоминаем позицию в MainTable
	int nCurPos = m_oBcw->m_oStream.GetPosition();
	//Seek в свободную область
	m_oBcw->m_oStream.SetPosition(m_nLastFilePos);
	return nCurPos;
}
void BinaryFileWriter::WriteTableEnd(int nCurPos)
{
	//сдвигаем позицию куда можно следующую таблицу
	m_nLastFilePos = m_oBcw->m_oStream.GetPosition();
	m_nRealTableCount++;
	//Seek вобратно в MainTable
	m_oBcw->m_oStream.SetPosition(nCurPos);
}

}
