#pragma once
#include "AVSVideoFileTemplate.h"

// IVideoFile3
[ object, uuid("3A836AA4-429F-4742-A6B6-98D579FE744F"), dual, pointer_default(unique)]
__interface IVideoFile3 : IAVSVideoFileTemplate
{
	//см. ..\Common\VideoFileDefine.h #define VIDEOFILE_TYPE_XXX
	[id(303000 + 1), propget]	HRESULT VideoFileType([out, retval] long *pVal);
	[id(303000 + 1), propput]	HRESULT VideoFileType([in] long newVal);

	[id(303000 + 10)]			HRESULT GetFileType([in]BSTR FilePath, [out, retval] long *pVal);
	//получить конкретный интерфейс
	[id(303000 + 20)]			HRESULT GetInternalInterface([out, retval] IUnknown **pVal);

	[id(303000 + 30), propget]	HRESULT TransformEnable([out, retval] VARIANT_BOOL *pVal);
	[id(303000 + 30), propput]	HRESULT TransformEnable([in] VARIANT_BOOL newVal);
	//IAVSVideoFormat
	[id(303000 + 31), propget]	HRESULT VideoReadingFormat([out, retval] IUnknown **pVal);
	[id(303000 + 31), propput]	HRESULT VideoReadingFormat([in] IUnknown *pVal);
	//IAVSAudioFormat
	[id(303000 + 35), propget]	HRESULT AudioReadingFormat([out, retval] IUnknown **pVal);
	[id(303000 + 35), propput]	HRESULT AudioReadingFormat([in] IUnknown *pVal);

	[id(303000 + 40)]			HRESULT SeekToFrame([in] long Frame);
};
