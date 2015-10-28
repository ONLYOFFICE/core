#pragma once
#include "../Reader/Records.h"

class CRecordMasterTextPropAtom : public CUnknownRecord
{
public:
	struct SMasterTextPropRun
	{
		DWORD lCount;
		WORD lIndentLevel;

		SMasterTextPropRun()
		{
			lCount = 0;
			lIndentLevel = 0;
		}

		SMasterTextPropRun(const SMasterTextPropRun& oSrc)
		{
			lCount = oSrc.lCount;
			lIndentLevel = oSrc.lIndentLevel;
		}

		SMasterTextPropRun& operator=(const SMasterTextPropRun& oSrc)
		{
			lCount = oSrc.lCount;
			lIndentLevel = oSrc.lIndentLevel;

			return *this;
		}
	};

	std::vector<SMasterTextPropRun> m_arrProps;

	CRecordMasterTextPropAtom()
	{
	}

	~CRecordMasterTextPropAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;
		m_arrProps.clear();

		size_t nCount = m_oHeader.RecLen / 6;
		while (nCount != 0)
		{
			--nCount;

			SMasterTextPropRun oRun;
			oRun.lCount = StreamUtils::ReadDWORD(pStream);
			oRun.lIndentLevel = StreamUtils::ReadWORD(pStream);

			m_arrProps.push_back(oRun);
		}
	}

};