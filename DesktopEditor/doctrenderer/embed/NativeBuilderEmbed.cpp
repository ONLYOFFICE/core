#include "NativeBuilderEmbed.h"
#include "NativeBuilderDocumentEmbed.h"
#include "./../docbuilder_p.h"

#include "../../common/Directory.h"

JSSmart<CJSValue> CBuilderEmbed::OpenFile(JSSmart<CJSValue> sPath, JSSmart<CJSValue> sParams)
{
	std::wstring Path = sPath->toStringW();
	std::wstring Params = sParams->toStringW();

	// Since we now use snapshots, and we can't always determine which editor is needed
	// (since the code may be BEFORE opening the file). And if the opening came from
	// builderJS.OpenFile - in this case we skip reopening.
	NSDoctRenderer::CDocBuilder_Private* pPrivate = GetPrivate();
	if (pPrivate->m_pWorker->IsSimpleJSInstance())
		pPrivate->m_bIsOpenedFromSimpleJS = true;

	int ret = m_pBuilder->OpenFile(Path.c_str(), Params.c_str());

	if (pPrivate->m_pWorker->IsSimpleJSInstance())
	{
		JSSmart<CJSContext> current = CJSContext::GetCurrent();
		current->runScript("throw 0;");
	}

	return CJSContext::createInt(ret);
}

JSSmart<CJSValue> CBuilderEmbed::CreateFile(JSSmart<CJSValue> type)
{
	int nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX;
	if (type->isString())
		nFormat = NSDoctRenderer::GetFormatByTexExtention(type->toStringW());
	else
		nFormat = type->toInt32();

	// Since we now use snapshots, and we can't always determine which editor is needed
	// (since the code may be BEFORE opening the file). And if the opening came from
	// builderJS.OpenFile - in this case we skip reopening.
	NSDoctRenderer::CDocBuilder_Private* pPrivate = GetPrivate();
	if (pPrivate->m_pWorker->IsSimpleJSInstance())
		pPrivate->m_bIsOpenedFromSimpleJS = true;

	bool ret = m_pBuilder->CreateFile(nFormat);

	if (pPrivate->m_pWorker->IsSimpleJSInstance())
	{
		JSSmart<CJSContext> current = CJSContext::GetCurrent();
		current->runScript("throw 0;");
	}

	return CJSContext::createBool(ret);
}

JSSmart<CJSValue> CBuilderEmbed::SetTmpFolder(JSSmart<CJSValue> path)
{
	std::wstring sPath = path->toStringW();
	m_pBuilder->SetTmpFolder(sPath.c_str());
	return NULL;
}

JSSmart<CJSValue> CBuilderEmbed::SaveFile(JSSmart<CJSValue> type, JSSmart<CJSValue> path, JSSmart<CJSValue> params)
{
	int nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX;
	if (type->isString())
		nFormat = NSDoctRenderer::GetFormatByTexExtention(type->toStringW());
	else
		nFormat = type->toInt32();

	std::wstring sPath = path->toStringW();
	std::wstring sParams = params->toStringW();
	int ret = GetPrivate()->SaveFile(nFormat, sPath.c_str(), sParams.empty() ? NULL : sParams.c_str(), false);
	return CJSContext::createInt(ret);
}

JSSmart<CJSValue> CBuilderEmbed::CloseFile()
{
	std::cout << "Warning: use builderJS.CloseFile(); separately!";
	m_pBuilder->GetPrivate()->CloseFile(false);
	return NULL;
}

JSSmart<CJSValue> CBuilderEmbed::OpenTmpFile(JSSmart<CJSValue> path, JSSmart<CJSValue> params)
{
	std::wstring sPath = path->toStringW();
	std::wstring sParams = params->toStringW();
	JSSmart<CJSObject> oBuilderTmpDoc = CJSContext::createEmbedObject("CBuilderDocumentEmbed");
	CBuilderDocumentEmbed* pBuilderTmpDocNative = static_cast<CBuilderDocumentEmbed*>(oBuilderTmpDoc->getNative());
	pBuilderTmpDocNative->m_pBuilder = m_pBuilder;
	pBuilderTmpDocNative->_OpenFile(sPath, sParams);
	return oBuilderTmpDoc->toValue();
}
