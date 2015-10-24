#pragma once

#include "../Records/CurrentUserAtom.h"

using namespace NSOfficePPT;

class CUserEdit
{ 
public:
	DWORD m_nOffsetLastEdit; 
	DWORD m_nOffsetPersistDirectory;  
	                          
	DWORD m_nDocumentRef; 
	DWORD m_nMaxPersistWritten;

public:
	CUserEdit()
	{
		m_nOffsetLastEdit = 0;
		m_nOffsetPersistDirectory = 0;
		m_nDocumentRef = 0;
		m_nMaxPersistWritten = 0;
	}
	~CUserEdit()
	{
	}

	CUserEdit& operator =(const CUserEdit& oSrc)
	{
		m_nOffsetLastEdit			= oSrc.m_nOffsetLastEdit;
		m_nOffsetPersistDirectory	= oSrc.m_nOffsetPersistDirectory;
		m_nDocumentRef				= oSrc.m_nDocumentRef;
		m_nMaxPersistWritten		= oSrc.m_nMaxPersistWritten;
		return (*this);
	}

	void FromAtom(CRecordUserEditAtom* pAtom)
	{
		m_nOffsetLastEdit = pAtom->m_nOffsetLastEdit;
		m_nOffsetPersistDirectory = pAtom->m_nOffsetPersistDirectory;
		m_nDocumentRef = pAtom->m_nOffsetDocPersistIdRef;
		m_nMaxPersistWritten = pAtom->m_nPersistIdSeed;
	}
}; 

class CCurrentUser 
{ 
public:
	bool			m_bIsEncrypt;  
	DWORD			m_nOffsetToCurrentEdit;

	std::wstring	m_strName;

public:
	CCurrentUser()
	{
		m_bIsEncrypt			= false;
		m_nOffsetToCurrentEdit	= 0;
	}
	~CCurrentUser()
	{
	}

	CCurrentUser& operator =(const CCurrentUser& oSrc)
	{
		m_bIsEncrypt			= oSrc.m_bIsEncrypt;
		m_nOffsetToCurrentEdit	= oSrc.m_nOffsetToCurrentEdit;
		m_strName				= oSrc.m_strName;
		return (*this);
	}

	void FromAtom(CRecordCurrentUserAtom* pAtom)
	{
		m_bIsEncrypt			= (NO_ENCRYPT == pAtom->m_nToken);
		m_nOffsetToCurrentEdit	= pAtom->m_nOffsetToCurEdit;
		m_strName				= pAtom->m_strUNICODEUserName;
	}

	CString ToString()
	{
        return L"";
	}
};


///----------- constants----------
const int MST_FLAG_OBJECTS		= 0x01; 
const int MST_FLAG_SCHEME		= 0x02; 
const int MST_FLAG_BACKGROUND	= 0x04; 

#define MAX_OBJECTS_IN_LAYOUT 8

const int S_HEADERFOOTER_DATE =        0x01; 
const int S_HEADERFOOTER_TODAYDATE =   0x02; 
const int S_HEADERFOOTER_USERDATE =    0x04; 
const int S_HEADERFOOTER_SLIDENUMBER = 0x08; 
const int S_HEADERFOOTER_HEADER =      0x10; 
const int S_HEADERFOOTER_FOOTER =      0x20;

#define PSR_NAMEDSHOW_SIZE    32 

//---------------------------------

struct SDocInfoAtom 
{ 
    SColorAtom  PenColor;       
    INT         RestartTime; 
    SHORT       StartSlide; 
    SHORT       EndSlide; 
    USHORT      NamedShow[PSR_NAMEDSHOW_SIZE]; 
    BYTE        Flags;
}; 
 
struct SSlideInfoAtom 
{ 
	INT        SlideTime;     // how long to show the slide in ticks 
	UINT       SoundRef;      
	USHORT     Effect;        // type of transition (2 character signature) 
	USHORT     Flags;         // set of flags that determine type of build 
	BYTE	   Speed;         // speed of transition 
};  
 
struct PSR_AnimationInfoAtom 
{ 
	SColorAtom  DimColor; 
	UINT        Flags; 
	UINT        SoundRef;      
	USHORT      OrderID;     
	USHORT      DelayTime; 
	USHORT      SlideCount; 
	BYTE        BuildType; 
	BYTE        FlyMethod; 
	BYTE        FlyDirection;      
	BYTE        AfterEffect;      
	BYTE        SubEffect;     
	BYTE        OleVerb; 
}; 
 
struct SInteractiveInfoAtom 
{ 
	UINT  SoundRef;      
	UINT  ExHyperlinkID; 
	BYTE  Action; 
	BYTE  OleVerb; 
	BYTE  Jump; 
	BYTE  Flags; 
};

