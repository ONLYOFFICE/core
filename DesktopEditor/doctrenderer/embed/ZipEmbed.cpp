#include "ZipEmbed.h"

JSSmart<CJSValue> CZipEmbed::open(JSSmart<CJSValue> typedArray)
{
    close();
    if (!typedArray->isTypedArray())
        return CJSContext::createBool(false);

    JSSmart<CJSTypedArray> pArray = typedArray->toTypedArray();
    CJSDataBuffer buffer = pArray->getData();

    m_pFolder = new CZipFolderMemory(buffer.Data, (DWORD)buffer.Len);
    buffer.Free();

    return CJSContext::createBool(true);
}
JSSmart<CJSValue> CZipEmbed::create()
{
    close();
    m_pFolder = new CZipFolderMemory();
    return CJSContext::createBool(true);
}
JSSmart<CJSValue> CZipEmbed::save()
{
    if (!m_pFolder)
        return CJSContext::createNull();

    IFolder::CBuffer* pBuffer = m_pFolder->finalize();
    BYTE* pMemory = NSJSBase::NSAllocator::Alloc((size_t)pBuffer->Size);
    memcpy(pMemory, pBuffer->Buffer, (size_t)pBuffer->Size);
    RELEASEOBJECT(pBuffer);

    return NSJSBase::CJSContext::createUint8Array(pMemory, (int)pBuffer->Size, false);
}
JSSmart<CJSValue> CZipEmbed::getFile(JSSmart<CJSValue> filePath)
{
    if (!m_pFolder || !filePath->isString())
        return CJSContext::createNull();

    std::wstring sFilePath = filePath->toStringW();
    IFolder::CBuffer* pBuffer;
    if (m_pFolder->read(sFilePath, pBuffer))
        return CJSContext::createNull();

    BYTE* pMemory = NSJSBase::NSAllocator::Alloc((size_t)pBuffer->Size);
    memcpy(pMemory, pBuffer->Buffer, (size_t)pBuffer->Size);
    RELEASEOBJECT(pBuffer);

    return NSJSBase::CJSContext::createUint8Array(pMemory, (int)pBuffer->Size, false);
}
JSSmart<CJSValue> CZipEmbed::addFile(JSSmart<CJSValue> filePath, JSSmart<CJSValue> typedArray)
{
    if (!m_pFolder || !filePath->isString() || !typedArray->isTypedArray())
        return CJSContext::createBool(false);

    CJSDataBuffer buffer = typedArray->toTypedArray()->getData();
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
