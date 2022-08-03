#include "NativeBuilderEmbed.h"
#include "./../docbuilder_p.h"

#include "../../common/Directory.h"

JSSmart<CJSValue> CBuilderEmbed::builder_OpenFile(JSSmart<CJSValue> sPath, JSSmart<CJSValue> sParams)
{
    std::wstring Path = sPath->toStringW();
    std::wstring Params = sParams->toStringW();
    int ret = m_pBuilder->OpenFile(Path.c_str(), Params.c_str());
    return CJSContext::createInt(ret);
}
JSSmart<CJSValue> CBuilderEmbed::builder_CreateFile(JSSmart<CJSValue> type)
{
    int nFormat = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX;
    if (type->isString())
        nFormat = NSDoctRenderer::GetFormatByTexExtention(type->toStringW());
    else
        nFormat = type->toInt32();

    bool ret = m_pBuilder->CreateFile(nFormat);
    return CJSContext::createBool(ret);
}
JSSmart<CJSValue> CBuilderEmbed::builder_SetTmpFolder(JSSmart<CJSValue> path)
{
    std::wstring sPath = path->toStringW();
    m_pBuilder->SetTmpFolder(sPath.c_str());
    return NULL;
}
JSSmart<CJSValue> CBuilderEmbed::builder_SaveFile(JSSmart<CJSValue> type, JSSmart<CJSValue> path, JSSmart<CJSValue> params)
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
    return CJSContext::createUint8Array(m_sFolder + L"/Editor.bin");
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
        NSStringUtils::string_replace(sFile, L"\\", L"/");
        std::wstring sName = L"media/" + NSFile::GetFileName(sFile);

        obj->set(U_TO_UTF8(sName).c_str(), CJSContext::createString(sFile));
    }

    return obj->toValue();
}

void CBuilderDocumentEmbed::OpenFile(const std::wstring& sFile, const std::wstring& sParams)
{
    NSDoctRenderer::CDocBuilder_Private* pBuilder = GetPrivate(m_pBuilder);

    std::wstring sTmpDir = pBuilder->m_sTmpFolder;

    m_sFolder = NSFile::CFileBinary::CreateTempFileWithUniqueName(sTmpDir, L"DE_");
    if (NSFile::CFileBinary::Exists(m_sFolder))
        NSFile::CFileBinary::Remove(m_sFolder);

    NSStringUtils::string_replace(m_sFolder, L"\\", L"/");

    std::wstring::size_type nPosPoint = m_sFolder.rfind('.');
    if (nPosPoint != std::wstring::npos && nPosPoint > sTmpDir.length())
    {
        m_sFolder = m_sFolder.substr(0, nPosPoint);
    }

    NSDirectory::CreateDirectory(m_sFolder);

    std::wstring sExtCopy = pBuilder->GetFileCopyExt(sFile);
    std::wstring sFileCopy = m_sFolder + L"/origin." + sExtCopy;

    pBuilder->MoveFileOpen(sFile, sFileCopy);
    int nConvertResult = pBuilder->ConvertToInternalFormat(m_sFolder, sFileCopy, sParams);

    if (0 == nConvertResult)
        m_bIsValid = true;
}
void CBuilderDocumentEmbed::CloseFile()
{
    if (!m_sFolder.empty())
        NSDirectory::DeleteDirectory(m_sFolder);
    m_bIsValid = false;
    m_sFolder = L"";
}
