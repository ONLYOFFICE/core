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

#include "DrawingFileEmbed.h"
#include "../drawingfile.h"
#include "../../raster/BgraFrame.h"

JSSmart<CJSValue> WasmMemoryToJS(BYTE* pWasmData)
{
	if (NULL == pWasmData)
		return CJSContext::createNull();

	int nLen = 0;
	memcpy(&nLen, pWasmData, sizeof(int));
	if (4 >= nLen)
		return CJSContext::createNull();

	return NSJSBase::CJSContext::createUint8Array(pWasmData + 4, nLen - 4, true);
}

CDrawingFileEmbed::CDrawingFileEmbed()
{
	m_pFile = NULL;
}
CDrawingFileEmbed::~CDrawingFileEmbed()
{
	RELEASEOBJECT(m_pFile);
}

JSSmart<CJSValue> CDrawingFileEmbed::OpenFile(JSSmart<CJSValue> sFile, JSSmart<CJSValue> sPassword)
{
	std::wstring wsPassword = sPassword->isString() ? sPassword->toStringW() : L"";
	bool bResult = m_pFile->OpenFile(sFile->toStringW(), sPassword->isString() ? wsPassword.c_str() : NULL);
	return CJSContext::createBool(bResult);
}
JSSmart<CJSValue> CDrawingFileEmbed::CloseFile()
{
	m_pFile->CloseFile();
	return CJSContext::createUndefined();
}

JSSmart<CJSValue> CDrawingFileEmbed::GetType()
{
	return CJSContext::createInt(m_pFile->GetType());
}
JSSmart<CJSValue> CDrawingFileEmbed::GetErrorCode()
{
	return CJSContext::createInt(m_pFile->GetErrorCode());
}

JSSmart<CJSValue> CDrawingFileEmbed::GetInfo()
{
	return WasmMemoryToJS(m_pFile->GetInfo());
}

JSSmart<CJSValue> CDrawingFileEmbed::GetPixmap(JSSmart<CJSValue> nPageIndex, JSSmart<CJSValue> nRasterW, JSSmart<CJSValue> nRasterH, JSSmart<CJSValue> nBackgroundColor)
{
	int nW = nRasterW->toInt32();
	int nH = nRasterH->toInt32();
	BYTE* pData = m_pFile->GetPixmap(nPageIndex->toInt32(), nW, nH, nBackgroundColor->toInt32());

	if (NULL == pData)
		return CJSContext::createNull();

	return NSJSBase::CJSContext::createUint8Array(pData, 4 * nW * nH, true);
}

JSSmart<CJSValue> CDrawingFileEmbed::DestroyPixmap(JSSmart<CJSValue> typedArray)
{
	if (!typedArray->isTypedArray())
		return NULL;

	CBgraFrame oFrame;
	oFrame.put_Data(typedArray->toTypedArray()->getData().Data);
	// free on destructor
	return NULL;
}

JSSmart<CJSValue> CDrawingFileEmbed::GetFontBinary(JSSmart<CJSValue> Id)
{
	if (0 != m_pFile->GetType())
		return NULL;

	std::wstring sName = Id->toStringW();
	std::wstring sFile = m_pFile->GetFontBinaryNative(sName);
	if (sFile.empty())
		return NULL;

	return CJSContext::createUint8Array(sFile);
}

JSSmart<CJSValue> CDrawingFileEmbed::GetGlyphs(JSSmart<CJSValue> nPageIndex)
{
	return WasmMemoryToJS(m_pFile->GetGlyphs(nPageIndex->toInt32()));
}
JSSmart<CJSValue> CDrawingFileEmbed::GetLinks(JSSmart<CJSValue> nPageIndex)
{
	return WasmMemoryToJS(m_pFile->GetLinks(nPageIndex->toInt32()));
}
JSSmart<CJSValue> CDrawingFileEmbed::GetStructure()
{
	return WasmMemoryToJS(m_pFile->GetStructure());
}
JSSmart<CJSValue> CDrawingFileEmbed::GetInteractiveFormsInfo()
{
	return WasmMemoryToJS(m_pFile->GetInteractiveFormsInfo());
}
JSSmart<CJSValue> CDrawingFileEmbed::GetInteractiveFormsFonts(JSSmart<CJSValue> nTypeFonts)
{
	return WasmMemoryToJS(m_pFile->GetInteractiveFormsFonts(nTypeFonts->toInt32()));
}

JSSmart<CJSValue> CDrawingFileEmbed::GetInteractiveFormsAP(JSSmart<CJSValue> nRasterW, JSSmart<CJSValue> nRasterH, JSSmart<CJSValue> nBackgroundColor,
														   JSSmart<CJSValue> nPageIndex,
														   JSSmart<CJSValue> nWidget, JSSmart<CJSValue> nView, JSSmart<CJSValue> nButtonView)
{
	return WasmMemoryToJS(m_pFile->GetInteractiveFormsAP(nRasterW->toInt32(), nRasterH->toInt32(), nBackgroundColor->toInt32(),
														 nPageIndex->toInt32(),
														 nWidget->toInt32(), nView->toInt32(), nButtonView->toInt32()));
}
JSSmart<CJSValue> CDrawingFileEmbed::GetButtonIcons(JSSmart<CJSValue> nBackgroundColor, JSSmart<CJSValue> nPageIndex, JSSmart<CJSValue> bBase64,
													JSSmart<CJSValue> nButtonWidget, JSSmart<CJSValue> nIconView)
{
	return WasmMemoryToJS(m_pFile->GetButtonIcons(nBackgroundColor->toInt32(), nPageIndex->toInt32(), bBase64->toInt32(),
												  nButtonWidget->toInt32(), nIconView->toInt32()));
}
JSSmart<CJSValue> CDrawingFileEmbed::GetAnnotationsInfo(JSSmart<CJSValue> nPageIndex)
{
	return WasmMemoryToJS(m_pFile->GetAnnotationsInfo(nPageIndex->toInt32()));
}
JSSmart<CJSValue> CDrawingFileEmbed::GetAnnotationsAP(JSSmart<CJSValue> nRasterW, JSSmart<CJSValue> nRasterH, JSSmart<CJSValue> nBackgroundColor,
													  JSSmart<CJSValue> nPageIndex, JSSmart<CJSValue> nAnnot, JSSmart<CJSValue> nView)
{
	return WasmMemoryToJS(m_pFile->GetAnnotationsAP(nRasterW->toInt32(), nRasterH->toInt32(), nBackgroundColor->toInt32(),
													nPageIndex->toInt32(), nAnnot->toInt32(), nView->toInt32()));
}

JSSmart<CJSValue> CDrawingFileEmbed::DestroyTextInfo()
{
	m_pFile->DestroyTextInfo();
	return CJSContext::createUndefined();
}
JSSmart<CJSValue> CDrawingFileEmbed::GetGIDByUnicode(JSSmart<CJSValue> sId)
{
	return WasmMemoryToJS(m_pFile->GetGIDByUnicode(sId->toStringW()));
}
JSSmart<CJSValue> CDrawingFileEmbed::IsNeedCMap()
{
	return CJSContext::createBool(false);
}
JSSmart<CJSValue> CDrawingFileEmbed::ScanPage(JSSmart<CJSValue> nPageIndex, JSSmart<CJSValue> mode)
{
	return WasmMemoryToJS(m_pFile->ScanPage(nPageIndex->toInt32(), mode->toInt32()));
}
JSSmart<CJSValue> CDrawingFileEmbed::SetScanPageFonts(JSSmart<CJSValue> arrPageIndexes)
{
	JSSmart<CJSArray> arrPages = arrPageIndexes->toArray();
	int nCountPages = arrPages->getCount();
	int* pPages = NULL;
	if (0 < nCountPages)
		pPages = new int[nCountPages];

	for (int i = 0; i < nCountPages; i++)
		pPages[i] = arrPages->get(i)->toInt32();

	JSSmart<CJSValue> res = WasmMemoryToJS(m_pFile->SetScanPageFonts(pPages, nCountPages));
	if (pPages)
		delete [] pPages;
	return res;
}

JSSmart<CJSValue> CDrawingFileEmbed::GetImageBase64(JSSmart<CJSValue> rId)
{
	std::string* pData = (std::string*)m_pFile->GetImageBase64(rId->toInt32());
	if (!pData)
		return CJSContext::createNull();
	JSSmart<CJSValue> ret = CJSContext::createString(*pData);
	*pData = "";
	return ret;
}

JSSmart<CJSValue> CDrawingFileEmbed::FreeWasmData(JSSmart<CJSValue> typedArray)
{
	if (!typedArray->isTypedArray())
		return NULL;
	BYTE* data = typedArray->toTypedArray()->getData().Data;
	typedArray->toTypedArray()->Detach();
	data -= 4; // sizeof int (length in NSWasm::Data)
	free(data);
	return NULL;
}

JSSmart<CJSValue> CDrawingFileEmbed::SplitPages(JSSmart<CJSValue> arrPageIndexes, JSSmart<CJSValue> data)
{
	JSSmart<CJSArray> arrPages = arrPageIndexes->toArray();
	CJSDataBuffer changes;
	if (data->isTypedArray())
		changes = data->toTypedArray()->getData();

	int nCountPages = arrPages->getCount();
	int* pPages = NULL;
	if (0 < nCountPages)
		pPages = new int[nCountPages];

	for (int i = 0; i < nCountPages; i++)
		pPages[i] = arrPages->get(i)->toInt32();

	JSSmart<CJSValue> res = WasmMemoryToJS(m_pFile->SplitPages(pPages, nCountPages, changes.Data, (LONG)changes.Len));
	if (pPages)
		delete [] pPages;
	return res;
}
JSSmart<CJSValue> CDrawingFileEmbed::MergePages(JSSmart<CJSValue> data, JSSmart<CJSValue> nMaxID, JSSmart<CJSValue> sPrefixForm)
{
	bool result = false;
	if (m_pFile)
	{
		JSSmart<CJSTypedArray> dataPtr = data->toTypedArray();
		int maxID = nMaxID->toInt32();
		std::string prefix = sPrefixForm->toStringA();
		CJSDataBuffer buffer = dataPtr->getData();
		result = m_pFile->MergePages(buffer.Data, (LONG)buffer.Len, maxID, prefix, true);
		if (buffer.IsExternalize)
			buffer.Free();
	}
	return CJSContext::createBool(result);
}
JSSmart<CJSValue> CDrawingFileEmbed::UnmergePages()
{
	if (m_pFile)
		return CJSContext::createBool(m_pFile->UnmergePages());
	return CJSContext::createBool(false);
}
JSSmart<CJSValue> CDrawingFileEmbed::RedactPage(JSSmart<CJSValue> nPageIndex, JSSmart<CJSValue> arrRedactBox, JSSmart<CJSValue> dataFiller)
{
	bool result = false;
	if (m_pFile)
	{
		int pageIndex = nPageIndex->toInt32();

		JSSmart<CJSArray> arrBox = arrRedactBox->toArray();
		int nCountBox = arrBox->getCount();
		double* pBox = NULL;
		if (0 < nCountBox)
			pBox = new double[nCountBox];

		for (int i = 0; i < nCountBox; i++)
			pBox[i] = arrBox->get(i)->toDouble();

		JSSmart<CJSTypedArray> dataPtr = dataFiller->toTypedArray();
		CJSDataBuffer buffer = dataPtr->getData();

		result = m_pFile->RedactPage(pageIndex, pBox, nCountBox / 8, buffer.Data, (int)buffer.Len, true);

		if (pBox)
			delete[] pBox;
		if (buffer.IsExternalize)
			buffer.Free();
	}
	return CJSContext::createBool(result);
}
JSSmart<CJSValue> CDrawingFileEmbed::UndoRedact()
{
	if (m_pFile)
		return CJSContext::createBool(m_pFile->UndoRedact());
	return CJSContext::createBool(false);
}

JSSmart<CJSValue> CDrawingFileEmbed::CheckOwnerPassword(JSSmart<CJSValue> sPassword)
{
	std::wstring wsPassword = sPassword->isString() ? sPassword->toStringW() : L"";
	bool bResult = m_pFile->CheckOwnerPassword(sPassword->isString() ? wsPassword.c_str() : NULL);
	return CJSContext::createBool(bResult);
}
JSSmart<CJSValue> CDrawingFileEmbed::CheckPerm(JSSmart<CJSValue> nPerm)
{
	return CJSContext::createBool(m_pFile->CheckPerm(nPerm->toInt32()));
}

bool EmbedDrawingFile(JSSmart<NSJSBase::CJSContext>& context, IOfficeDrawingFile* pFile)
{
	CJSContext::Embed<CDrawingFileEmbed>(false);

	JSSmart<CJSObject> oNativeDrawingFile = CJSContext::createEmbedObject("CDrawingFileEmbed");
	context->GetGlobal()->set("g_native_drawing_file", oNativeDrawingFile);

	CDrawingFile* pDrFile = new CDrawingFile(pFile->GetFonts());
	pDrFile->SetInternalFile(pFile);
	((CDrawingFileEmbed*)oNativeDrawingFile->getNative())->m_pFile = pDrFile;

	return true;
}
