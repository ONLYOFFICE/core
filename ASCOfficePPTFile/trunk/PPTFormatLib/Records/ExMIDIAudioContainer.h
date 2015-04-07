#pragma once
#include "../Reader/Records.h"

class CRecordExMIDIAudioContainer : public CRecordsContainer
{
public:
	
	CRecordExMIDIAudioContainer()
	{
	}

	~CRecordExMIDIAudioContainer()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		CRecordsContainer::ReadFromStream(oHeader, pStream);
	}
	virtual CString ToString()
	{
		return CRecordsContainer::ToString();
	}
};
