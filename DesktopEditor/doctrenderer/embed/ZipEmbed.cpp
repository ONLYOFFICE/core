#include "ZipEmbed.h"

JSSmart<CJSValue> CZipEmbed::open(JSSmart<CJSValue> typedArray)
{
    RELEASEOBJECT(m_pFolder);
    if (!typedArray->isTypedArray())
        return CJSContext::createNull();

    JSSmart<CJSTypedArray> pArray = typedArray->toTypedArray();
    CJSDataBuffer buffer = pArray->getData();

	m_pFolder = new CZipFolderMemory(buffer.Data, (DWORD)buffer.Len);
    if (buffer.IsExternalize)
        buffer.Free();

    std::vector<std::wstring> arFiles = m_pFolder->getFiles(L"", true);
    if (arFiles.empty())
        return CJSContext::createNull();

    JSSmart<CJSArray> retFiles = CJSContext::createArray((int)arFiles.size());

    int nCurCount = 0;
    for (std::vector<std::wstring>::const_iterator i = arFiles.begin(); i != arFiles.end(); i++)
    {
        const std::wstring& val = *i;
        retFiles->set(nCurCount++, CJSContext::createString(val.empty() ? val : val.substr(1)));
    }

    return retFiles->toValue();
}
JSSmart<CJSValue> CZipEmbed::create()
{
    RELEASEOBJECT(m_pFolder);
    m_pFolder = new CZipFolderMemory();
    return CJSContext::createBool(true);
}
JSSmart<CJSValue> CZipEmbed::save()
{
    if (!m_pFolder)
        return CJSContext::createNull();

    IFolder::CBuffer* pBuffer = m_pFolder->finalize();

    size_t nBufferSize = (size_t)pBuffer->Size;
    BYTE* pMemory = NSJSBase::NSAllocator::Alloc(nBufferSize);
    memcpy(pMemory, pBuffer->Buffer, nBufferSize);
    RELEASEOBJECT(pBuffer);

    return NSJSBase::CJSContext::createUint8Array(pMemory, (int)nBufferSize, false);
}
JSSmart<CJSValue> CZipEmbed::getFile(JSSmart<CJSValue> filePath)
{
    if (!m_pFolder || !filePath->isString())
        return CJSContext::createNull();

    std::wstring sFilePath = filePath->toStringW();
    IFolder::CBuffer* pBuffer;
    if (!m_pFolder->read(sFilePath, pBuffer))
        return CJSContext::createNull();

    size_t nBufferSize = (size_t)pBuffer->Size;
    BYTE* pMemory = NSJSBase::NSAllocator::Alloc(nBufferSize);
    memcpy(pMemory, pBuffer->Buffer, nBufferSize);
    RELEASEOBJECT(pBuffer);

    return NSJSBase::CJSContext::createUint8Array(pMemory, (int)nBufferSize, false);
}
JSSmart<CJSValue> CZipEmbed::addFile(JSSmart<CJSValue> filePath, JSSmart<CJSValue> typedArray)
{
    if (!m_pFolder || !filePath->isString() || !typedArray->isTypedArray())
        return CJSContext::createBool(false);

    JSSmart<CJSTypedArray> typedArray2 = typedArray->toTypedArray();
    CJSDataBuffer buffer = typedArray2->getData();
    m_pFolder->write(filePath->toStringW(), buffer.Data, (DWORD)buffer.Len);

    if (buffer.IsExternalize)
        buffer.Free();

    return CJSContext::createBool(true);
}
JSSmart<CJSValue> CZipEmbed::removeFile(JSSmart<CJSValue> filePath)
{
    if (!m_pFolder || !filePath->isString())
        return CJSContext::createNull();

    m_pFolder->remove(filePath->toStringW());
    return CJSContext::createUndefined();
}
JSSmart<CJSValue> CZipEmbed::close()
{
    RELEASEOBJECT(m_pFolder);
    return CJSContext::createUndefined();
}
