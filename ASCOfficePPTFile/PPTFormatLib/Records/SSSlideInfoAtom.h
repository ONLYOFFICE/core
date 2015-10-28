#pragma once
#include "../Reader/Records.h"

class CSlideShowSlideInfoAtom : public CUnknownRecord
{
public:

	INT		m_nSlideTime;
	DWORD	m_nSoundRef;

	BYTE	m_nEffectDirection;
	BYTE	m_nEffectType;

	bool	m_bManualAdvance;
	//  reserved1 (1 bit)
	bool	m_bHidden;
	//  reserved1 (1 bit)
	bool	m_bSound;
	//  reserved1 (1 bit)
	bool	m_bLoopSound;
	//  reserved1 (1 bit)
	bool	m_bStopSound;
	//  reserved1 (1 bit)
	bool	m_bAutoAdvance;
	//  reserved1 (1 bit)
	bool	m_bCursorVisible;
	//  reserved1 (3 bit)

	BYTE	m_nSpeed;


public:
	
	CSlideShowSlideInfoAtom()
	{
		m_nSlideTime	= -1;
		m_nSoundRef		= 0xFFFFFFFE;
		
		m_nEffectDirection	= 0;
		m_nEffectType		= 0;

		m_bManualAdvance	= false;
		m_bHidden			= false;
		m_bSound			= false;
		m_bLoopSound		= false;
		m_bStopSound		= false;
		m_bAutoAdvance		= false;
		m_bCursorVisible	= false;

		m_nSpeed			= 1;		
	}

	~CSlideShowSlideInfoAtom()
	{
	}

	virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
	{
		m_oHeader = oHeader;
		
		m_nSlideTime		=	(INT)StreamUtils::ReadDWORD ( pStream );
		m_nSoundRef			=	StreamUtils::ReadDWORD ( pStream );
	
		m_nEffectDirection	=	StreamUtils::ReadBYTE ( pStream );
		m_nEffectType		=	StreamUtils::ReadBYTE ( pStream );

		BYTE bValue			=	StreamUtils::ReadBYTE ( pStream );
		
		m_bManualAdvance	=	(0x01 == (0x01 & bValue));
		m_bHidden			=	(0x04 == (0x04 & bValue));
		m_bSound			=	(0x10 == (0x10 & bValue));
		m_bLoopSound		=	(0x40 == (0x40 & bValue));

		bValue				=	StreamUtils::ReadBYTE ( pStream );
		
		m_bStopSound		=	(0x01 == (0x01 & bValue));
		m_bAutoAdvance		=	(0x04 == (0x04 & bValue));
		m_bCursorVisible	=	(0x10 == (0x10 & bValue));

		//DWORD dwValue		=	StreamUtils::ReadDWORD ( pStream );
		//m_nSpeed			=	( 0x01 == ( 0x01 & ((BYTE)dwValue) ) );
		m_nSpeed			=	StreamUtils::ReadBYTE( pStream );
		StreamUtils::StreamSkip(3, pStream);
	}

	double GetTimeTransition()
	{
		double dTime = 500.0;
		if (0 == m_nSpeed)
		{
			dTime = 750.0;
		}
		else if (2 == m_nSpeed)
		{
			dTime = 250.0;
		}

		return dTime;
	}

	double GetTimeSlide()
	{
		return (double)m_nSlideTime;
	}
};