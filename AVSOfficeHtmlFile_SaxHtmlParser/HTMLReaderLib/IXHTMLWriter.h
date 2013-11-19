#pragma once
#include "HTMLTagWrapper.h"

class IXHTMLWriter
{
public:
	virtual BOOL BeginFile () = 0;
	virtual BOOL StartTag (CHTMLTagWrapper *pTag, CHTMLTagWrapper *pPrevTag) = 0;
	virtual BOOL EndTag (CHTMLTagWrapper *pTag) = 0;
	virtual void Characters (CHTMLTagWrapper *pTagWrapper, CString aText, bool aBeforeLastChild) = 0;
	virtual BOOL EndFile () = 0;
	virtual void SetOutput (CString &aOut) = 0;
	virtual DWORD ErrorCode () = 0;
};