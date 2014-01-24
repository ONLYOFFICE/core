#pragma once
#include "xmlutils.h"


class CError
{
private:INT32 m_nSeverity;
private:LONG m_lID;
private:CString m_sFilename;
private:CString m_sDescription;
private:int m_nFileID;

public:CError()
	   {
			m_nSeverity = 0;
			m_sFilename = CString( _T("") );
			m_lID = 0;
			m_nFileID = 0;
	   }
public:~CError()
	   {
	   }
public: CString GetFilename()
		{
			return m_sFilename;
		}
public: VOID SetFilename(CString sFilename)
		{
			m_sFilename = sFilename;
		}
public: int GetFileID()
		{
			return m_nFileID;
		}
public: VOID SetFileID(int nFileId)
		{
			m_nFileID = nFileId;
		}
public: CString GetDescription()
		{
			return m_sDescription;
		}
public: VOID SetDescription(CString sDescription)
		{
			m_sDescription = sDescription;
		}
public: INT32 GetSeverity()
		{
			return m_nSeverity;
		}
public: VOID SetSeverity(INT32 nSeverity)
		{
			m_nSeverity = nSeverity;
		}
public: LONG GetID()
		{
			return m_lID;
		}
public: VOID SetID(LONG lID)
		{
			m_lID = lID;
		}
public: CString ToString()
		{
			CString sResult;
			sResult.AppendFormat( _T("<Error type=\"%d\" filename=\"%s\"/>"), m_nSeverity, m_sFilename );
			return sResult;
		}
public: CString ToXml()
		{
			return CString( _T("") ); 
		}
};