#include "js_base.h"
#include "js_base_p.h"

namespace NSJSBase {

	CJSValue::CJSValue()
	{
	}

	CJSValue::~CJSValue()
	{
	}

	CJSEmbedObjectPrivateBase::CJSEmbedObjectPrivateBase()
	{
	}

	CJSEmbedObjectPrivateBase::~CJSEmbedObjectPrivateBase()
	{
	}

	CJSEmbedObjectAdapterBase::CJSEmbedObjectAdapterBase()
	{
	}

	CJSEmbedObjectAdapterBase::~CJSEmbedObjectAdapterBase()
	{
	}

	CJSEmbedObject::CJSEmbedObject()
	{
		embed_native_internal = nullptr;
		m_pAdapter = nullptr;
		m_isExternalize = false;
	}

	CJSEmbedObject::~CJSEmbedObject()
	{
		RELEASEOBJECT(embed_native_internal);
		RELEASEOBJECT(m_pAdapter);
	}

	void* CJSEmbedObject::getObject()
	{
		return nullptr;
	}

	void CJSEmbedObject::SetExternalize(const bool& isExternalize)
	{
		m_isExternalize = isExternalize;
	}

	bool CJSEmbedObject::GetExternalize()
	{
		return m_isExternalize;
	}

	CJSEmbedObjectAdapterBase* CJSEmbedObject::getAdapter()
	{
		return nullptr;
	}

	void CJSContext::AddEmbedCreator(const std::string& name,
									 EmbedObjectCreator creator,
									 const bool& isAllowedInJS)
	{
		CEmbedObjectRegistrator& oRegistrator = CEmbedObjectRegistrator::getInstance();
		oRegistrator.Register(name, creator, isAllowedInJS);
	}

	CJSObject::CJSObject()
	{
	}

	CJSObject::~CJSObject()
	{
	}

	void CJSObject::set(const char* name, JSSmart<CJSObject> obj)
	{
		this->set(name, obj->toValue());
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

	JSSmart<CJSObject> CJSContext::createEmbedObject(const std::string& name)
	{
		// Allow creation for embedded class in JS while in current scope
		CEmbedObjectRegistrator& oRegistrator = CEmbedObjectRegistrator::getInstance();
		JSSmart<CEmbedObjectRegistrator::CAllowedCreationScope> oCreationScope = oRegistrator.AllowCreationInScope(name);
		if (!oCreationScope.IsInit())
			return nullptr;
		// Call CreateEmbedObject() from JS
		JSSmart<CJSContext> context = CJSContext::GetCurrent();
		JSSmart<CJSValue> args[1];
		args[0] = CJSContext::createString(name);
		JSSmart<CJSValue> res = context->GetGlobal()->call_func("CreateEmbedObject", 1 , args);
		if (!res->isObject())
			return nullptr;
		return res->toObject();
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
