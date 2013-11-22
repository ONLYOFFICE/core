#pragma once

#include "stdio.h"
#include "malloc.h"
#include "string.h"

class CImageRaw3Thumbnail
{
protected:
	void * data;
	size_t datasize;
	DWORD  type;
public:
	CImageRaw3Thumbnail():data(NULL),datasize(0),type(0)
	{
	}

	HRESULT Reset()
	{
		if (data)
			free(data);
		datasize = 0;
		data = 0;
		return S_OK;
	}
	
	HRESULT SetData(void * data_,size_t datasize_)
	{
		Reset();
		data = malloc(datasize_);
		if (!data)
			return E_OUTOFMEMORY;
		
		if (memcpy_s(data,datasize_,data_,datasize_))
		{
			free(data); data = 0;
			return E_FAIL;
		}
		datasize = datasize_;
		return S_OK;
	}

	HRESULT CopyData(void * data_,size_t datasize_)
	{
		Reset();		
		data = data_;
		datasize = datasize_;
		return S_OK;
	}

	HRESULT CopyTo(void ** data_,size_t * datasize_)
	{
		if (!data_ || !datasize_)
			return E_POINTER;

		*data_ = 0;
		*datasize_ = 0;

		if (!data)
			return S_FALSE;

		*data_ = malloc(datasize);
		if (*data_)
		{
			if (memcpy_s(*data_,datasize,data,datasize))
				return E_FAIL;
			*datasize_ = datasize;
			return S_OK;
		}
		return S_FALSE;
	}
};