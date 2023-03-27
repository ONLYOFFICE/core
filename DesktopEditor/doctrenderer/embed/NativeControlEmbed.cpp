#include "NativeControlEmbed.h"

JSSmart<CJSValue> CNativeControlEmbed::SetFilePath(JSSmart<CJSValue> path)
{
    m_pInternal->SetFilePath(path->toStringW());
    return NULL;
}

JSSmart<CJSValue> CNativeControlEmbed::GetFilePath()
{
    return CJSContext::createString(m_pInternal->GetFilePath());
}

JSSmart<CJSValue> CNativeControlEmbed::SetFileId(JSSmart<CJSValue> fileId)
{
    m_pInternal->SetFileId(fileId->toStringW());
    return NULL;
}

JSSmart<CJSValue> CNativeControlEmbed::GetFileId()
{
    return CJSContext::createString(m_pInternal->GetFileId());
}

JSSmart<CJSValue> CNativeControlEmbed::GetFileBinary(JSSmart<CJSValue> file)
{
    return CJSContext::createUint8Array(file->toStringW());
}

JSSmart<CJSValue> CNativeControlEmbed::GetFontBinary(JSSmart<CJSValue> file)
{
    // TODO:
    // по идее файлы могут совпадать по имени, но лежать в разных директориях.
    // и поэтому в AllFonts.js надо бы писать пути полные.
    // пока оставим поддержку старой версии тоже
    std::wstring sFile = file->toStringW();
    bool bIsFullFilePath = (std::wstring::npos != sFile.find('\\') || std::wstring::npos != sFile.find('/'));
    if (bIsFullFilePath)
    {
        bIsFullFilePath = NSFile::CFileBinary::Exists(sFile);
    }

    if (!bIsFullFilePath)
    {
        std::map<std::wstring, std::wstring>::iterator pair = m_pInternal->m_map_fonts.find(sFile);
        if (pair != m_pInternal->m_map_fonts.end())
            sFile = pair->second;
        else
            sFile = m_pInternal->m_sDefaultFont;
    }

    return CJSContext::createUint8Array(sFile);
}

JSSmart<CJSValue> CNativeControlEmbed::GetFontsDirectory()
{
    return CJSContext::createString(m_pInternal->m_strFontsDirectory);
}

JSSmart<CJSValue> CNativeControlEmbed::GetFileString(JSSmart<CJSValue> file)
{
    return CJSContext::createUint8Array(file->toStringW());
}

JSSmart<CJSValue> CNativeControlEmbed::GetEditorType()
{
    return CJSContext::createString(m_pInternal->m_strEditorType);
}

JSSmart<CJSValue> CNativeControlEmbed::CheckNextChange()
{
    m_pInternal->m_nCurrentChangesNumber++;
    if (-1 != m_pInternal->m_nMaxChangesNumber)
    {
        if (m_pInternal->m_nCurrentChangesNumber >= m_pInternal->m_nMaxChangesNumber)
        {
            return CJSContext::createBool(false);
        }
    }
    return CJSContext::createBool(true);
}

JSSmart<CJSValue> CNativeControlEmbed::GetCountChanges()
{
    int nCount = 0;
    if (m_pInternal->m_pChanges != NULL)
    {
        nCount = (int)m_pInternal->m_pChanges->size();
    }

    return CJSContext::createInt(nCount);
}

JSSmart<CJSValue> CNativeControlEmbed::GetChangesFile(JSSmart<CJSValue> index)
{
    int nIndex = index->toInt32();
    std::wstring strFile = L"";
    if (m_pInternal->m_pChanges != NULL)
        strFile = (*m_pInternal->m_pChanges)[nIndex];
    return CJSContext::createString(strFile);
}

JSSmart<CJSValue> CNativeControlEmbed::Save_AllocNative(JSSmart<CJSValue> nLen)
{
    int Len = nLen->toInt32();
    m_pInternal->Save_Alloc(Len);
    return CJSContext::createUint8Array(m_pInternal->m_pSaveBinary, m_pInternal->m_nSaveLen, true);
}

JSSmart<CJSValue> CNativeControlEmbed::Save_ReAllocNative(JSSmart<CJSValue> pos, JSSmart<CJSValue> len)
{
    int _pos = pos->toInt32();
    int _len = len->toInt32();
    m_pInternal->Save_ReAlloc(_pos, _len);
    return CJSContext::createUint8Array(m_pInternal->m_pSaveBinary, m_pInternal->m_nSaveLen, true);
}

JSSmart<CJSValue> CNativeControlEmbed::Save_End(JSSmart<CJSValue> pos, JSSmart<CJSValue> len)
{
    std::string sHeader = pos->toStringA();
    int _len = len->toInt32();
    m_pInternal->Save_End(sHeader, _len);
    return NULL;
}

JSSmart<CJSValue> CNativeControlEmbed::AddImageInChanges(JSSmart<CJSValue> img)
{
    std::wstring sImage = img->toStringW();
    if (sImage.empty())
        return NULL;

    std::map<std::wstring, bool>::const_iterator iter = m_pInternal->m_mapImagesInChanges.find(sImage);
    if (iter == m_pInternal->m_mapImagesInChanges.end())
        m_pInternal->m_mapImagesInChanges.insert(std::pair<std::wstring, bool>(sImage, true));
    return NULL;
}

JSSmart<CJSValue> CNativeControlEmbed::ConsoleLog(JSSmart<CJSValue> message)
{
    m_pInternal->ConsoleLog(message->toStringA());
    return NULL;
}

JSSmart<CJSValue> CNativeControlEmbed::SaveChanges(JSSmart<CJSValue> sParam, JSSmart<CJSValue> nDeleteIndex, JSSmart<CJSValue> nCount)
{
    m_pInternal->DumpChanges(sParam->toStringA(), nDeleteIndex->toInt32(), nCount->toInt32());
    return NULL;
}

JSSmart<CJSValue> CNativeControlEmbed::zipOpenFile(JSSmart<CJSValue> name)
{
    bool bIsOpen = m_pInternal->m_oZipWorker.Open(name->toStringW());
    if (!bIsOpen)
        return CJSContext::createNull();

    JSSmart<CJSObject> obj = CJSContext::createObject();
    for (std::vector<std::wstring>::iterator i = m_pInternal->m_oZipWorker.m_arFiles.begin(); i != m_pInternal->m_oZipWorker.m_arFiles.end(); i++)
    {
        std::string sFile = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(*i);
        obj->set(sFile.c_str(), CJSContext::createString(sFile));
    }

    return obj->toValue();
}

JSSmart<CJSValue> CNativeControlEmbed::zipOpenFileBase64(JSSmart<CJSValue> name)
{
    bool bIsOpen = m_pInternal->m_oZipWorker.OpenBase64(name->toStringA());
    if (!bIsOpen)
        return CJSContext::createNull();

    JSSmart<CJSObject> obj = CJSContext::createObject();
    for (std::vector<std::wstring>::iterator i = m_pInternal->m_oZipWorker.m_arFiles.begin(); i != m_pInternal->m_oZipWorker.m_arFiles.end(); i++)
    {
        std::string sFile = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(*i);
        obj->set(sFile.c_str(), CJSContext::createString(sFile));
    }

    return obj->toValue();
}

JSSmart<CJSValue> CNativeControlEmbed::zipGetFileAsString(JSSmart<CJSValue> name)
{
    BYTE* pData = NULL;
    DWORD len = 0;
    m_pInternal->m_oZipWorker.GetFileData(name->toStringW(), pData, len);
    return  CJSContext::createString((char*)pData, len);
}

JSSmart<CJSValue> CNativeControlEmbed::zipGetFileAsBinary(JSSmart<CJSValue> name)
{
    return CJSContext::createUint8Array(m_pInternal->m_oZipWorker.m_sTmpFolder + L"/" + name->toStringW());
}

JSSmart<CJSValue> CNativeControlEmbed::zipCloseFile()
{
    m_pInternal->m_oZipWorker.Close();
    return NULL;
}

JSSmart<CJSValue> CNativeControlEmbed::GetImageUrl(JSSmart<CJSValue> sUrl)
{
    std::wstring Url = sUrl->toStringW();

    if (!m_pInternal->m_pWorker)
        m_pInternal->m_pWorker = new CImagesWorker(m_pInternal->m_strImagesDirectory);

    std::wstring sRet = m_pInternal->m_pWorker->GetImage(Url);

    return CJSContext::createString(sRet);
}

JSSmart<CJSValue> CNativeControlEmbed::GetImagesPath()
{
    return CJSContext::createString(m_pInternal->m_strImagesDirectory);
}

#include "./../../graphics/MetafileToRenderer.h"
#include "./../../raster/BgraFrame.h"
JSSmart<CJSValue> CNativeControlEmbed::GetImageOriginalSize(JSSmart<CJSValue> sUrl)
{
	IMetafileToRenderter oRenderer(NULL);
	oRenderer.SetMediaDirectory(NSDirectory::GetFolderPath(m_pInternal->m_strImagesDirectory));
	std::wstring sPath = oRenderer.GetImagePath(sUrl->toStringW());
	CBgraFrame oFrame;
	if (oFrame.OpenFile(sPath))
	{
		JSSmart<CJSObject> ret = CJSContext::createObject();
		ret->set("W", oFrame.get_Width());
		ret->set("H", oFrame.get_Height());
		return ret->toValue();
	}
	return CJSContext::createUndefined();
}
