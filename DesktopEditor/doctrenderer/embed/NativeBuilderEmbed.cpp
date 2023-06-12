#include "NativeBuilderEmbed.h"
#include "NativeBuilderDocumentEmbed.h"
#include "./../docbuilder_p.h"

#include "../../common/Directory.h"

JSSmart<CJSValue> CBuilderEmbed::OpenFile(JSSmart<CJSValue> sPath, JSSmart<CJSValue> sParams)
{
	std::wstring Path = sPath->toStringW();
	std::wstring Params = sParams->toStringW();
	int ret = m_pBuilder->OpenFile(Path.c_str(), Params.c_str());
	return CJSContext::createInt(ret);
}

JSSmart<CJSValue> CBuilderEmbed::CreateFile(JSSmart<CJSValue> type)
{
	int nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX;
	if (type->isString())
		nFormat = NSDoctRenderer::GetFormatByTexExtention(type->toStringW());
	else
		nFormat = type->toInt32();

	bool ret = m_pBuilder->CreateFile(nFormat);
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
	int ret = m_pBuilder->SaveFile(nFormat, sPath.c_str(), sParams.empty() ? NULL : sParams.c_str());
	return CJSContext::createInt(ret);
}

JSSmart<CJSValue> CBuilderEmbed::CloseFile()
{
	m_pBuilder->CloseFile();
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
