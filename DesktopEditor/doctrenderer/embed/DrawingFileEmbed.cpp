#include "DrawingFileEmbed.h"

JSSmart<CJSValue> MoveMemoryToJS(BYTE* pWasmData, const bool& isFree = true)
{
	if (NULL == pWasmData)
		return CJSContext::createNull();

	int nLen = 0;
	memcpy(&nLen, pWasmData, sizeof(int));
	if (4 >= nLen)
		return CJSContext::createNull();

	size_t nBufferSize = (size_t)(nLen - 4);
	BYTE* pMemory = NSJSBase::NSAllocator::Alloc(nBufferSize);
	memcpy(pMemory, pWasmData + 4, nBufferSize);

	if (isFree)
		free(pWasmData);

	return NSJSBase::CJSContext::createUint8Array(pMemory, (int)nBufferSize, false);
}

JSSmart<CJSValue> CDrawingFileEmbed::OpenFile(JSSmart<CJSValue> sFile, JSSmart<CJSValue> sPassword)
{
	bool bResult = m_pFile->OpenFile(sFile->toStringW(), sPassword->isString() ? sPassword->toStringW() : L"");
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
	return MoveMemoryToJS(m_pFile->GetInfo());
}

JSSmart<CJSValue> CDrawingFileEmbed::GetPixmap(JSSmart<CJSValue> nPageIndex, JSSmart<CJSValue> nRasterW, JSSmart<CJSValue> nRasterH, JSSmart<CJSValue> nBackgroundColor)
{
	return MoveMemoryToJS(m_pFile->GetPixmap(nPageIndex->toInt32(), nRasterW->toInt32(), nRasterH->toInt32(), nBackgroundColor->toInt32()));
}

JSSmart<CJSValue> CDrawingFileEmbed::GetGlyphs(JSSmart<CJSValue> nPageIndex)
{
	return MoveMemoryToJS(m_pFile->GetGlyphs(nPageIndex->toInt32()), false);
}
JSSmart<CJSValue> CDrawingFileEmbed::GetLinks(JSSmart<CJSValue> nPageIndex)
{
	return MoveMemoryToJS(m_pFile->GetLinks(nPageIndex->toInt32()));
}
JSSmart<CJSValue> CDrawingFileEmbed::GetStructure()
{
	return MoveMemoryToJS(m_pFile->GetStructure());
}
JSSmart<CJSValue> CDrawingFileEmbed::GetInteractiveFormsInfo()
{
	return MoveMemoryToJS(m_pFile->GetInteractiveFormsInfo());
}
JSSmart<CJSValue> CDrawingFileEmbed::GetInteractiveFormsFonts(JSSmart<CJSValue> nTypeFonts)
{
	return MoveMemoryToJS(m_pFile->GetInteractiveFormsFonts(nTypeFonts->toInt32()));
}

JSSmart<CJSValue> CDrawingFileEmbed::GetInteractiveFormsAP(JSSmart<CJSValue> nRasterW, JSSmart<CJSValue> nRasterH, JSSmart<CJSValue> nBackgroundColor,
														   JSSmart<CJSValue> nPageIndex,
														   JSSmart<CJSValue> nWidget, JSSmart<CJSValue> nView, JSSmart<CJSValue> nButtonView)
{
	return MoveMemoryToJS(m_pFile->GetInteractiveFormsAP(nRasterW->toInt32(), nRasterH->toInt32(), nBackgroundColor->toInt32(),
														 nPageIndex->toInt32(),
														 nWidget->toInt32(), nView->toInt32(), nButtonView->toInt32()));
}
JSSmart<CJSValue> CDrawingFileEmbed::GetButtonIcons(JSSmart<CJSValue> nBackgroundColor, JSSmart<CJSValue> nPageIndex, JSSmart<CJSValue> bBase64,
													JSSmart<CJSValue> nButtonWidget, JSSmart<CJSValue> nIconView)
{
	return MoveMemoryToJS(m_pFile->GetButtonIcons(nBackgroundColor->toInt32(), nPageIndex->toInt32(), bBase64->toInt32(),
												  nButtonWidget->toInt32(), nIconView->toInt32()));
}
JSSmart<CJSValue> CDrawingFileEmbed::GetAnnotationsInfo(JSSmart<CJSValue> nPageIndex)
{
	return MoveMemoryToJS(m_pFile->GetAnnotationsInfo(nPageIndex->toInt32()));
}
JSSmart<CJSValue> CDrawingFileEmbed::GetAnnotationsAP(JSSmart<CJSValue> nRasterW, JSSmart<CJSValue> nRasterH, JSSmart<CJSValue> nBackgroundColor,
													  JSSmart<CJSValue> nPageIndex, JSSmart<CJSValue> nAnnot, JSSmart<CJSValue> nView)
{
	return MoveMemoryToJS(m_pFile->GetAnnotationsAP(nRasterW->toInt32(), nRasterH->toInt32(), nBackgroundColor->toInt32(),
														 nPageIndex->toInt32(), nAnnot->toInt32(), nView->toInt32()));
}

JSSmart<CJSValue> CDrawingFileEmbed::DestroyTextInfo()
{
	m_pFile->DestroyTextInfo();
	return CJSContext::createUndefined();
}
JSSmart<CJSValue> CDrawingFileEmbed::IsNeedCMap()
{
	return CJSContext::createBool(false);
}
JSSmart<CJSValue> CDrawingFileEmbed::ScanPage(JSSmart<CJSValue> nPageIndex, JSSmart<CJSValue> mode)
{
	return MoveMemoryToJS(m_pFile->ScanPage(nPageIndex->toInt32(), mode->toInt32()));
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
