#include "ZipEmbed.h"
#include "../../raster/BgraFrame.h"
#include "../../graphics/pro/Image.h"
#include "../../raster/ImageFileFormatChecker.h"

JSSmart<CJSValue> CZipEmbed::open(JSSmart<CJSValue> typedArray_or_Folder)
{
	RELEASEOBJECT(m_pFolder);

	if (typedArray_or_Folder->isTypedArray())
	{
		JSSmart<CJSTypedArray> pArray = typedArray_or_Folder->toTypedArray();
		CJSDataBuffer buffer = pArray->getData();

		m_pFolder = new CZipFolderMemory(buffer.Data, (DWORD)buffer.Len);
		if (buffer.IsExternalize)
			buffer.Free();
	}
	else if (typedArray_or_Folder->isString())
	{
		m_pFolder = new CFolderSystem(typedArray_or_Folder->toStringW());
	}

	if (!m_pFolder)
		return CJSContext::createNull();

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

JSSmart<CJSValue> CZipEmbed::getPaths()
{
	if (!m_pFolder)
		return CJSContext::createArray(0);

	std::vector<std::wstring> arFiles = m_pFolder->getFiles(L"", true);
	JSSmart<CJSArray> retFiles = CJSContext::createArray((int)arFiles.size());

	int nCurCount = 0;
	for (std::vector<std::wstring>::const_iterator i = arFiles.begin(); i != arFiles.end(); i++)
	{
		const std::wstring& val = *i;
		retFiles->set(nCurCount++, CJSContext::createString(val.empty() ? val : val.substr(1)));
	}

	return retFiles->toValue();
}

JSSmart<CJSValue> CZipEmbed::decodeImage(JSSmart<CJSValue> typedArray, JSSmart<CJSValue> isRgba)
{
	JSSmart<CJSTypedArray> oArray = typedArray->toTypedArray();
	NSJSBase::CJSDataBuffer oBuffer = oArray->getData();

	CBgraFrame oFrame;
	if (isRgba.is_init() && isRgba->isBool())
		oFrame.put_IsRGBA(isRgba->toBool());
	else
		oFrame.put_IsRGBA(true);

	if (!oFrame.Decode(oBuffer.Data, (int)oBuffer.Len))
		return CJSContext::createUndefined();

	JSSmart<CJSObject> oDecoded = CJSContext::createObject();

	size_t nFileSize = 4 * oFrame.get_Width() * oFrame.get_Height();
	BYTE* pData = NSAllocator::Alloc(nFileSize);
	memcpy(pData, oFrame.get_Data(), nFileSize);
	oDecoded->set("data", CJSContext::createUint8Array(pData, (int)nFileSize, false));
	oDecoded->set("width", CJSContext::createInt(oFrame.get_Width()));
	oDecoded->set("height", CJSContext::createInt(oFrame.get_Height()));
	oDecoded->set("stride", CJSContext::createInt(oFrame.get_Stride()));

	return oDecoded->toValue();
}
JSSmart<CJSValue> CZipEmbed::encodeImageData(JSSmart<CJSValue> typedArray, JSSmart<CJSValue> w, JSSmart<CJSValue> h, JSSmart<CJSValue> stride, JSSmart<CJSValue> format, JSSmart<CJSValue> isRgba)
{
	JSSmart<CJSTypedArray> oArray = typedArray->toTypedArray();
	NSJSBase::CJSDataBuffer oBuffer = oArray->getData();

	CBgraFrame oFrame;
	if (isRgba.is_init() && isRgba->isBool())
		oFrame.put_IsRGBA(isRgba->toBool());
	else
		oFrame.put_IsRGBA(true);

	oFrame.put_Data(oBuffer.Data);
	oFrame.put_Width(w->toInt32());
	oFrame.put_Height(h->toInt32());

	if (stride.is_init() && stride->isNumber())
		oFrame.put_Stride(stride->toInt32());
	else
		oFrame.put_Stride(4 * oFrame.get_Stride());

	BYTE* pBuffer = NULL;
	int nEncodedSize = 0;

	if (oFrame.Encode(pBuffer, nEncodedSize, format->toInt32()))
	{
		BYTE* pData = NSAllocator::Alloc((size_t)nEncodedSize);
		memcpy(pData, oFrame.get_Data(), (size_t)nEncodedSize);
		oFrame.FreeEncodedMemory(pBuffer);
		oFrame.put_Data(NULL);

		return CJSContext::createUint8Array(pData, nEncodedSize, false);
	}

	oFrame.put_Data(NULL);
	return CJSContext::createUndefined();
}
JSSmart<CJSValue> CZipEmbed::encodeImage(JSSmart<CJSValue> typedArray, JSSmart<CJSValue> format)
{
	JSSmart<CJSTypedArray> oArray = typedArray->toTypedArray();
	NSJSBase::CJSDataBuffer oBuffer = oArray->getData();
	CImageFileFormatChecker oChecker;
	bool bIsImageFile = oChecker.isImageFile(oBuffer.Data, (DWORD)oBuffer.Len);

	if (bIsImageFile)
	{
		switch (oChecker.eFileType)
		{
		case _CXIMAGE_FORMAT_WMF:
		case _CXIMAGE_FORMAT_EMF:
		{
			if (_CXIMAGE_FORMAT_SVG == format->toInt32())
			{
		#ifndef GRAPHICS_DISABLE_METAFILE
				MetaFile::IMetaFile* pMetaFile = MetaFile::Create(NULL);
				pMetaFile->LoadFromBuffer(oBuffer.Data, (unsigned int)oBuffer.Len);
				std::wstring wsSvg = pMetaFile->ConvertToSvg();
				std::string sSvg = U_TO_UTF8(wsSvg);
				pMetaFile->Release();

				BYTE* pData = NSAllocator::Alloc(sSvg.length());
				memcpy(pData, sSvg.c_str(), sSvg.length());

				if (oBuffer.IsExternalize)
					oBuffer.Free();

				return CJSContext::createUint8Array(pData, sSvg.length(), false);
		#endif
			}
			break;
		}
		default:
			CBgraFrame oFrame;
			oFrame.Decode(oBuffer.Data, (int)oBuffer.Len);

			BYTE* pBuffer = NULL;
			int nEncodedSize = 0;

			if (oFrame.Encode(pBuffer, nEncodedSize, format->toInt32()))
			{
				BYTE* pData = NSAllocator::Alloc((size_t)nEncodedSize);
				memcpy(pData, oFrame.get_Data(), (size_t)nEncodedSize);
				oFrame.FreeEncodedMemory(pBuffer);
				oFrame.put_Data(NULL);

				if (oBuffer.IsExternalize)
					oBuffer.Free();

				return CJSContext::createUint8Array(pData, nEncodedSize, false);
			}
			break;
		}
	}

	if (oBuffer.IsExternalize)
		oBuffer.Free();

	return CJSContext::createUndefined();
}
JSSmart<CJSValue> CZipEmbed::getImageType(JSSmart<CJSValue> typedArray)
{
	JSSmart<CJSTypedArray> oArray = typedArray->toTypedArray();
	NSJSBase::CJSDataBuffer oBuffer = oArray->getData();
	CImageFileFormatChecker oChecker;
	bool bIsImageFile = oChecker.isImageFile(oBuffer.Data, (DWORD)oBuffer.Len);
	if (oBuffer.IsExternalize)
		oBuffer.Free();
	return CJSContext::createInt(bIsImageFile ? oChecker.eFileType : 0);
}
