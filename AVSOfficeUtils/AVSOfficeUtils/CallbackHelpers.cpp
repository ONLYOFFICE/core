#include "StdAfx.h"
#include "CallbackHelpers.h"
#include "OfficeUtils.h"

// ========= ExtractedFileCallback ==============

ExtractedFileCallback::ExtractedFileCallback(IExtractedFileEvent* data_receiver) : invokable_object(data_receiver)
{
}

void ExtractedFileCallback::Invoke(_bstr_t file_name, SAFEARRAY** arr)
{
	if(invokable_object)
	{
		invokable_object->ExtractedFile(file_name, arr);
	}
}

// ========= RequestFileCallback ==============

RequestFileCallback::RequestFileCallback(IRequestFileEvent* data_receiver) : invokable_object(data_receiver)
{
}

bool RequestFileCallback::Invoke(BSTR* file_name, SAFEARRAY** arr)
{
	if(invokable_object)
	{
		VARIANT_BOOL is_data_attached;
		invokable_object->RequestFile(file_name, arr, &is_data_attached);
		return VARIANT_TRUE == is_data_attached;
	}
}
