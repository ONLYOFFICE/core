/*
 * (c) Copyright Ascensio System SIA 2010-2024
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

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
