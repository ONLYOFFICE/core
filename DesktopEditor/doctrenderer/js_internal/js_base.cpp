#include "js_base.h"

namespace NSJSBase {

	CJSValue::CJSValue()
	{
	}

	CJSValue::~CJSValue()
	{
	}

	JSSmart<CJSObject> CJSValue::toObjectSmart()
	{
		return toObject();
	}

	CJSEmbedObjectPrivateBase::CJSEmbedObjectPrivateBase()
	{
	}

	CJSEmbedObjectPrivateBase::~CJSEmbedObjectPrivateBase()
	{
	}

	CJSEmbedObject::CJSEmbedObject()
	{
		embed_native_internal = NULL;
	}

	CJSEmbedObject::~CJSEmbedObject()
	{
		RELEASEOBJECT(embed_native_internal);
	}

	void* CJSEmbedObject::getObject()
	{
		return NULL;
	}

	std::vector<std::string> CJSEmbedObject::getNames()
	{
		return std::vector<std::string>();
	}

	void CJSEmbedObject::initFunctions()
	{
	}

	JSSmart<CJSValue> CJSEmbedObject::Call(const int& index, CJSFunctionArguments* args)
	{
		return m_functions[index](args);
	}

	CJSObject::CJSObject()
	{
	}

	CJSObject::~CJSObject()
	{
	}

	CJSArray::CJSArray()
	{
	}

	CJSArray::~CJSArray()
	{
	}

	BYTE* CJSDataBuffer::Copy()
	{
		if (0 == Len)
			return NULL;
		BYTE* pMem = new BYTE[Len];
		if (!pMem)
			return NULL;

		memcpy(pMem, Data, Len);
		return pMem;
	}

	CJSDataBuffer::CJSDataBuffer()
	{
		Data = NULL;
		Len = 0;
		IsExternalize = false;
	}

	void CJSDataBuffer::Free()
	{
		if (Data)
		{
			NSAllocator::Free(Data, Len);
			Data = NULL;
		}
	}

	CJSTypedArray::CJSTypedArray(BYTE* data, int count)
	{
	}

	CJSTypedArray::~CJSTypedArray()
	{
	}

	CJSFunction::CJSFunction()
	{
	}

	CJSFunction::~CJSFunction()
	{
	}

	CJSTryCatch::CJSTryCatch()
	{
	}

	CJSTryCatch::~CJSTryCatch()
	{
	}

	CJSValue* CJSContext::createUint8Array(const std::wstring& sFilePath)
	{
		NSFile::CFileBinary oFileBinary;
		if (oFileBinary.OpenFile(sFilePath))
		{
			long nFileSize = oFileBinary.GetFileSize();
			BYTE* pData = NSAllocator::Alloc((size_t)nFileSize);
			DWORD dwSizeRead;
			if (oFileBinary.ReadFile(pData, (DWORD)nFileSize, dwSizeRead))
			{
				return CJSContext::createUint8Array(pData, (int)nFileSize, false);
			}
			NSAllocator::Free(pData, (size_t)nFileSize);
		}
		return CJSContext::createNull();
	}

	CJSContextScope::CJSContextScope(JSSmart<CJSContext> context) : m_context(context)
	{
		m_context->Enter();
	}

	CJSContextScope::~CJSContextScope()
	{
		m_context->Exit();
	}
}
