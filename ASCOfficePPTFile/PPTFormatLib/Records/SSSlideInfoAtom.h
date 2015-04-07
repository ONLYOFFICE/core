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

	virtual CString ToString()
	{
		XmlUtils::CXmlWriter oWriter;
		CString strName = GetRecordName((DWORD)m_oHeader.RecType);
		
		oWriter.WriteNodeBegin(strName, TRUE);
		oWriter.WriteAttribute(_T("length"), CDirectory::ToString(m_oHeader.RecLen));
		oWriter.WriteAttribute(_T("type"), CDirectory::ToString(m_oHeader.RecType));
		oWriter.WriteAttribute(_T("instance"), CDirectory::ToString(m_oHeader.RecInstance));

		oWriter.WriteNodeEnd(strName, TRUE, FALSE);
		
		CDirectory::WriteValueToNode(_T("SlideTime"), (LONG)m_nSlideTime, &oWriter);
		CDirectory::WriteValueToNode(_T("SoundRef"), m_nSoundRef, &oWriter);
		CDirectory::WriteValueToNode(_T("EffectDuration"), (DWORD)m_nEffectDirection, &oWriter);
		CDirectory::WriteValueToNode(_T("EffectType"), (DWORD)m_nEffectType, &oWriter);
		CDirectory::WriteValueToNode(_T("ManualAdvance"), m_bManualAdvance, &oWriter);
		CDirectory::WriteValueToNode(_T("Hidden"), m_bHidden, &oWriter);
		CDirectory::WriteValueToNode(_T("Sound"), m_bSound, &oWriter);
		CDirectory::WriteValueToNode(_T("LoopSound"), m_bLoopSound, &oWriter);
		CDirectory::WriteValueToNode(_T("StopSound"), m_bStopSound, &oWriter);
		CDirectory::WriteValueToNode(_T("AutoAdvance"), m_bAutoAdvance, &oWriter);
		CDirectory::WriteValueToNode(_T("CursorVisible"), m_bCursorVisible, &oWriter);

		oWriter.WriteNodeEnd(strName);

		return oWriter.GetXmlString();
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