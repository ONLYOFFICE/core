#include "NativeBuilderEmbed.h"

#include "../../../common/Directory.h"
#include "../../../fontengine/application_generate_fonts_common.h"

JSSmart<CJSValue> CBuilderEmbed::builder_OpenFile(JSSmart<CJSValue> sPath, JSSmart<CJSValue> sParams)
{
    std::wstring Path = sPath->toStringW();
    std::wstring Params = sParams->toStringW();
    int ret = m_pBuilder->OpenFile(Path.c_str(), Params.c_str());
    return CJSContext::createInt(ret);
}
JSSmart<CJSValue> CBuilderEmbed::builder_CreateFile(JSSmart<CJSValue> type)
{
    bool ret = m_pBuilder->CreateFile(type->toInt32());
    return CJSContext::createBool(ret);
}
JSSmart<CJSValue> CBuilderEmbed::builder_SetTmpFolder(JSSmart<CJSValue> path)
{
    std::wstring sPath = path->toStringW();
    m_pBuilder->SetTmpFolder(sPath.c_str());
    return NULL;
}
JSSmart<CJSValue> CBuilderEmbed::builder_SaveFile(JSSmart<CJSValue> t, JSSmart<CJSValue> path, JSSmart<CJSValue> params)
{
    int type = t->toInt32();
    std::wstring sPath = path->toStringW();
    std::wstring sParams = params->toStringW();
    int ret = m_pBuilder->SaveFile(type, sPath.c_str(), sParams.empty() ? NULL : sParams.c_str());
    return CJSContext::createInt(ret);
}
JSSmart<CJSValue> CBuilderEmbed::builder_CloseFile()
{
    m_pBuilder->CloseFile();
    return NULL;
}


JSSmart<CJSValue> CBuilderDocumentEmbed::builder_doc_IsValid()
{
    return CJSContext::createBool(m_bIsValid);
}
JSSmart<CJSValue> CBuilderDocumentEmbed::builder_doc_GetBinary()
{
    BYTE* pData = NULL;
    DWORD dwSize = 0;
    NSFile::CFileBinary::ReadAllBytes(m_sFolder + L"/Editor.bin", &pData, dwSize);

    return 0 == dwSize ? NULL : CJSContext::createUint8Array(pData, (int)dwSize);
}
JSSmart<CJSValue> CBuilderDocumentEmbed::builder_doc_GetFolder()
{
    return CJSContext::createString(m_sFolder);
}
JSSmart<CJSValue> CBuilderDocumentEmbed::builder_doc_CloseFile()
{
    CloseFile();
    return NULL;
}
JSSmart<CJSValue> CBuilderDocumentEmbed::builder_doc_GetImageMap()
{
    std::vector<std::wstring> files = NSDirectory::GetFiles(m_sFolder + L"/media");

    JSSmart<CJSObject> obj = CJSContext::createObject();
    for (std::vector<std::wstring>::iterator i = files.begin(); i != files.end(); i++)
    {
        std::wstring sFile = *i;
        NSCommon::string_replace(sFile, L"\\", L"/");
        std::wstring sName = L"media/" + NSFile::GetFileName(sFile);

        obj->set(U_TO_UTF8(sName).c_str(), CJSContext::createString(sFile));
    }

    return obj->toValue();
}
