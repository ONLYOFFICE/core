#pragma once

void CorrectString(CString& strValue)
	{
		strValue.Replace(_T("&"),	_T("&amp;"));			
		strValue.Replace(_T("'"),	_T("&apos;"));
		strValue.Replace(_T("<"),	_T("&lt;"));
		strValue.Replace(_T(">"),	_T("&gt;"));
		strValue.Replace(_T("\""),	_T("&quot;"));
	}
CString CorrectString2(CString& str)
	{
		CString strValue = str;
		CorrectString(strValue);
		return strValue;
	}