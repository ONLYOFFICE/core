#pragma once
#include "../Reader/Records.h"

class CRecordSoundDataBlob : public CUnknownRecord
{
	BYTE* m_pData;
	LONG  m_lSize;

public:
	
	CRecordSoundDataBlob()
	{
		m_pData = NULL;
		m_lSize = 0;
	}

	~CRecordSoundDataBlob()
	{
		ReleaseData();
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;
		m_lSize = m_oHeader.RecLen;

		if (0 < m_lSize)
		{
			m_pData = new BYTE[m_lSize];
			pStream->read(m_pData, m_lSize);
		}
	}

	void ReleaseData()
	{
		RELEASEARRAYOBJECTS(m_pData);
	}

	void SaveToFile(std::wstring strFile)
	{
		if ((NULL == m_pData) || (0 >= m_lSize))
			return;

		NSFile::CFileBinary oFile;
		oFile.CreateFileW(strFile);

		oFile.WriteFile((BYTE*)m_pData, (DWORD)m_lSize);
		oFile.CloseFile();

		ReleaseData();
	}
};