#pragma once
#include "../Reader/Records.h"

class CRecordPlaceHolderAtom : public CUnknownRecord
{
public:
	int	 m_nPosition;
	BYTE m_nPlacementID;
	BYTE m_nSize;

public:
	
	CRecordPlaceHolderAtom()
	{
	}

	~CRecordPlaceHolderAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		m_nPosition = StreamUtils::ReadLONG(pStream);
		m_nPlacementID = StreamUtils::ReadBYTE(pStream);
		m_nSize = StreamUtils::ReadBYTE(pStream);

		StreamUtils::StreamSkip(2, pStream);
	}
};

class CRecordRoundTripHFPlaceholder12Atom : public CUnknownRecord
{
public:
	BYTE m_nPlacementID;

public:
	
	CRecordRoundTripHFPlaceholder12Atom()
	{
	}

	~CRecordRoundTripHFPlaceholder12Atom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		m_nPlacementID	= StreamUtils::ReadBYTE(pStream);
	}
};

